#include "stdafx.h"
#include "VirtualHost.h"

VirtualHost::VirtualHost(int clientCount, PVOID* oCalls, int callCount)
	: VirtualIP(TRUE, oCalls, callCount)
{
	m_clCount = clientCount;
	m_localAddr = m_addressRange;
	m_localAddr.S_un.S_un_b.s_b4 = m_settings.virtAddressOffset;
}

VirtualHost::~VirtualHost()
{
}

BOOL VirtualHost::Init()
{
	if (!VirtualIP::Init())
		return FALSE;

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(m_settings.virtHostPort);
	server.sin_addr = in4addr_loopback;
	if (m_bind(m_virtSocket, (SOCKADDR *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "Virtual network server socket bind error : " << WSAGetLastError() << std::endl;
		return FALSE;
	}

	SetClientVirtualPort(m_settings.virtHostPort);
	return TRUE;
}

int VirtualHost::RegisterSocket(SOCKET s, const sockaddr* addr)
{
	VirtualIP::RegisterSocket(s, addr);
	int r = m_bind(s, addr, sizeof(sockaddr));
	if (r == 0)
	{
		m_socketStates[s]->s = s;
		m_socketStates[s]->virtRecvState = VirtRecv_StateNone;
		m_socketStates[s]->port = ((sockaddr_in*)addr)->sin_port;
	}
	else
	{
		std::cout << "Register socket failed : " << WSAGetLastError() << std::endl;
	}

	return r;
}

int VirtualHost::ReleaseSocket(SOCKET s)
{
	VirtualIP::ReleaseSocket(s);
	return m_closeSock(s);
}

int VirtualHost::HandleVirtNetwork(SocketState** pprecvSock)
{
	int ret = VirtualIP::HandleVirtNetwork(pprecvSock);
	if (ret == VirtHandleNet_UnknownPort)
	{
		std::cout << "Virtual client sending to unknown port!" << std::endl;
	}

	return ret;
}

int VirtualHost::DSendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr * lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped)
{
	sockaddr_in to = *(sockaddr_in*)lpTo;
	// Decides between a broadcast, sending to a virtual client and finally sending to a physical client
	if (in4addr_broadcast.s_addr == to.sin_addr.s_addr) // Is broadcast?
	{
		WritePacketInfoToLog(L"Send,BrCa,Imme", sockaddr_in{ AF_INET,m_socketStates[s]->port,m_localAddr }, to, lpBuffers->len);
		BroadcastVirtual(lpBuffers->buf, lpBuffers->len, to.sin_port);
	}
	else
	{
		u_long toNetwork = ntohl(to.sin_addr.s_addr) & VirtNet_SubnetMask;
		u_long virtNetwork = ntohl(m_addressRange.s_addr) & VirtNet_SubnetMask;
		if (toNetwork == virtNetwork &&
			to.sin_addr.S_un.S_un_b.s_b4 > m_settings.virtAddressOffset &&
			to.sin_addr.S_un.S_un_b.s_b4 < (m_clCount + m_settings.virtAddressOffset))
		{
			int cl = GetClientID(to.sin_addr);
			char virtBuffer[2048];
			DWORD numSent;
			sockaddr_in virtClAddr;
			virtClAddr.sin_family = AF_INET;
			virtClAddr.sin_addr = in4addr_loopback;
			virtClAddr.sin_port = htons(GetClientPhysHPort(cl));
			VirtIPHeader virtHead;
			virtHead.virtIP = m_localAddr;
			virtHead.virtPort = to.sin_port;
			memcpy_s(virtBuffer, sizeof(virtBuffer), &virtHead, sizeof(virtHead));
			memcpy_s(virtBuffer + sizeof(virtHead), sizeof(virtBuffer) - sizeof(virtHead), lpBuffers->buf, lpBuffers->len);
			WSABUF buf;
			buf.buf = virtBuffer;
			buf.len = sizeof(virtHead) + lpBuffers->len;
			WritePacketInfoToLog(L"Send,Virt,Imme", sockaddr_in{ AF_INET,m_socketStates[s]->port,m_localAddr }, to, lpBuffers->len);
			DWORD r = m_sendTo(m_virtSocket, &buf, 1, &numSent, NULL, (sockaddr*)&virtClAddr, sizeof(virtClAddr), NULL, NULL);
			if (r != 0)
			{
				std::cout << "Virtual send to client " << cl << "failed with " << r << " and WSA error " << WSAGetLastError() << "!" << std::endl;
			}
			return r;
		}
	}

	WritePacketInfoToLog(L"Send,Phys,Imme", sockaddr_in{ AF_INET,m_socketStates[s]->port,in4addr_loopback }, to, lpBuffers->len);
	return m_sendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iToLen, lpOverlapped, NULL);
}

int VirtualHost::DRecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, sockaddr * lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped)
{
	SocketState* state = m_socketStates[s];
	// Set current receive buffers
	state->lpRecvOverlapped = lpOverlapped;
	state->lpRecvBuffers = lpBuffers;
	state->lpRecvFrom = lpFrom;
	state->lpRecvFromlen = lpFromlen;
	state->overlappedRecvEvent = lpOverlapped->hEvent;
	// 1. Check if physical socket is currently waiting for an overlapped receive
	if (!state->isReceiving)
	{
		VirtReceiveBuffer *physBuf = &state->physRecvBuffer;
		physBuf->wsaBuf.buf = physBuf->buffer;
		physBuf->wsaBuf.len = sizeof(physBuf->buffer);
		physBuf->recvFromLen = sizeof(sockaddr_in);
		int ret = m_recvFrom(s, &physBuf->wsaBuf, 1, &physBuf->numBytesRecvd, lpFlags, (sockaddr*)&physBuf->recvFrom, &physBuf->recvFromLen, lpOverlapped, NULL);
		// 1 a) Immediate physical packet was received.
		// 1 b) No physical packet was waiting, the call can now handle potential virtual packets.
		if (0 == ret)
		{
			CompletePhysicalReceive(state);
			CheckPacketIntegrity(state->lpRecvBuffers->buf, *lpNumberOfBytesRecvd);
			WritePacketInfoToLog(L"Recv,Phys,Imme", *(sockaddr_in*)lpFrom, sockaddr_in{ AF_INET,state->port,m_localAddr }, *lpNumberOfBytesRecvd);
			return 0;
		}
		else
		{
			int err = WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				std::cout << "ReceiveFrom failed with Error : " << err << std::endl;
			}

			state->isReceiving = TRUE;
		}

	}
	// 2. a) Check if virtual packets are queued for this socket, if so this socket can receive an immediate packet.
	// 2. b) Check if there is a new virtual packet for this socket.
	if (0 < state->virtRecvBuffers.size() || WSA_WAIT_EVENT_0 == UpdateVirtualSocket(s, 0, NULL))
	{
		ImmediateVirtualRecv(state, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFrom);
		WritePacketInfoToLog(L"Recv,Virt,Imme", *(sockaddr_in*)lpFrom, sockaddr_in{ AF_INET,state->port,m_localAddr }, *lpNumberOfBytesRecvd);
		return 0;
	}


	// There are no new packets for this socket.
	WSASetLastError(WSA_IO_PENDING);
	return SOCKET_ERROR;
}

BOOL VirtualHost::DGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags)
{
	SocketState* state = m_socketStates[s];
	BOOLEAN ret;
	if (VirtRecv_StateVirtOverlapped == state->virtRecvState)
	{
		state->virtRecvState = VirtRecv_StateNone;
		VirtReceiveBuffer buf = state->virtRecvBuffers.front();
		state->virtRecvBuffers.pop();
		int virtCLID = GetClientID(buf.recvFrom.sin_port);
		VirtIPHeader virtHead = *((VirtIPHeader*)buf.buffer);

		// Check if virtual client send broadcast
		if (in4addr_broadcast.s_addr == virtHead.virtIP.s_addr)
		{
			((sockaddr_in*)state->lpRecvFrom)->sin_addr = in4addr_any;
		}
		else
		{
			((sockaddr_in*)state->lpRecvFrom)->sin_addr = GetClientVirtAddr(virtCLID);
		}
		((sockaddr_in*)state->lpRecvFrom)->sin_port = virtHead.virtPort;
		((sockaddr_in*)state->lpRecvFrom)->sin_family = AF_INET;
		memcpy_s(state->lpRecvBuffers->buf, state->lpRecvBuffers->len,
			buf.buffer + sizeof(VirtIPHeader), sizeof(buf.buffer) - sizeof(VirtIPHeader));
		*lpcbTransfer = buf.numBytesRecvd - sizeof(VirtIPHeader);
		CheckPacketIntegrity(state->lpRecvBuffers->buf, *lpcbTransfer);
		WritePacketInfoToLog(L"Recv,Virt,Asyn",
			*((sockaddr_in*)state->lpRecvFrom),
			sockaddr_in{ AF_INET,virtHead.virtPort,m_localAddr },
			*lpcbTransfer);
		ret = TRUE;
	}
	else if (state->virtRecvState >= VirtRecv_StateVirtBuffered)
	{
		std::cout << "Unexpected virtual receive state!" << std::endl;
		ret = FALSE;
	}
	else // Physical receive result
	{
		state->isReceiving = FALSE;
		ret = m_overlapped(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
		if (ret)
		{
			CompletePhysicalReceive(state);
			CheckPacketIntegrity(state->lpRecvBuffers->buf, *lpcbTransfer);
			WritePacketInfoToLog(L"Recv,Phys,Asyn",
				*((sockaddr_in*)state->lpRecvFrom),
				sockaddr_in{ AF_INET, ((sockaddr_in*)state->lpRecvFrom)->sin_port,m_localAddr },
				*lpcbTransfer);
		}
		else
		{
			std::cout << "Overlapped physical receive failed with :" << WSAGetLastError() << std::endl;
		}
	}

	return ret;
}

DWORD VirtualHost::DWaitForEvents(DWORD cEvents, const WSAEVENT * lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
{
	return VirtualIP::DWaitForEvents(cEvents, lphEvents, fWaitAll, dwTimeout, fAlertable);
}

BOOL VirtualHost::DWSAResetEvent(WSAEVENT hEvent)
{
	if (m_ignoredEvents[hEvent])
	{
		m_ignoredEvents[hEvent] = FALSE;
		return TRUE;
	}
	else
	{
		return m_reset(hEvent);
	}
}

void VirtualHost::CompletePhysicalReceive(SocketState * state)
{
	VirtReceiveBuffer *physBuf = &state->physRecvBuffer;
	memcpy_s(state->lpRecvBuffers->buf, state->lpRecvBuffers->len, physBuf->buffer, sizeof(physBuf->buffer));
	*state->lpRecvFrom = *(sockaddr*)&physBuf->recvFrom;
	*state->lpRecvFromlen = physBuf->recvFromLen;

}

void VirtualHost::BroadcastVirtual(char * buffer, int buflen, short nport)
{
	for (int i = 1; i < m_clCount; i++)
	{
		u_short clPort = GetClientPhysHPort(i);
		if (0 == clPort) // Client is not yet listening
		{
			continue;
		}
		char virtBuffer[2048];
		DWORD numSent;
		sockaddr_in virtClAddr;
		virtClAddr.sin_family = AF_INET;
		virtClAddr.sin_addr = in4addr_loopback;
		virtClAddr.sin_port = htons(clPort);
		VirtIPHeader virtHead;
		virtHead.virtIP = in4addr_broadcast;
		virtHead.virtPort = nport;
		memcpy_s(virtBuffer, sizeof(virtBuffer), &virtHead, sizeof(virtHead));
		memcpy_s(virtBuffer + sizeof(virtHead), sizeof(virtBuffer) - sizeof(virtHead), buffer, buflen);
		WSABUF buf;
		buf.buf = virtBuffer;
		buf.len = sizeof(virtHead) + buflen;
		DWORD r = m_sendTo(m_virtSocket, &buf, 1, &numSent, NULL, (sockaddr*)&virtClAddr, sizeof(virtClAddr), NULL, NULL);
		if (r != 0)
		{
			std::cout << "Virtual broadcast to client " << i << "failed with " << r << " and WSA error " << WSAGetLastError() << "!" << std::endl;
		}
	}
}

void VirtualHost::ImmediateVirtualRecv(SocketState* state, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, sockaddr * lpFrom)
{
	VirtReceiveBuffer buf = state->virtRecvBuffers.front();
	state->virtRecvBuffers.pop();
	int virtCLID = GetClientID(buf.recvFrom.sin_port);
	VirtIPHeader virtHead = *((VirtIPHeader*)buf.buffer);

	// Check if virtual client send broadcast
	if (in4addr_broadcast.s_addr == virtHead.virtIP.s_addr)
	{
		((sockaddr_in*)state->lpRecvFrom)->sin_addr = in4addr_any;
	}
	else
	{
		((sockaddr_in*)state->lpRecvFrom)->sin_addr = GetClientVirtAddr(virtCLID);
	}
	((sockaddr_in*)lpFrom)->sin_port = virtHead.virtPort;
	((sockaddr_in*)lpFrom)->sin_family = AF_INET;
	memcpy_s(lpBuffers->buf, lpBuffers->len,
		buf.buffer + sizeof(VirtIPHeader), sizeof(buf.buffer) - sizeof(VirtIPHeader));
	*lpNumberOfBytesRecvd = buf.numBytesRecvd - sizeof(VirtIPHeader);
	CheckPacketIntegrity(lpBuffers->buf, *lpNumberOfBytesRecvd);
}
