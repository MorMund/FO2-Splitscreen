#include "stdafx.h"
#include "VirtualClient.h"

VirtualClient::VirtualClient(PVOID* oCalls, int callCount)
	: VirtualIP(FALSE, oCalls, callCount)
{
	m_localAddr = GetClientVirtAddr();
}

VirtualClient::~VirtualClient()
{
}

BOOL VirtualClient::Init()
{
	if (!VirtualIP::Init())
		return FALSE;
	sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = 0;
	client.sin_addr = in4addr_loopback;
	if (m_bind(m_virtSocket, (SOCKADDR *)&client, sizeof(client)) == SOCKET_ERROR)
	{
		std::cout << "Virtual network client socket bind error : " << WSAGetLastError() << std::endl;
		return FALSE;
	}

	int namelen = sizeof(client);
	if (getsockname(m_virtSocket, (SOCKADDR*)&client, &namelen) == SOCKET_ERROR)
	{
		std::cout << "Virtual network client socket address retrieval error : " << WSAGetLastError() << std::endl;
		return FALSE;
	}

	SetClientVirtualPort(ntohs(client.sin_port));
	return TRUE;
}

int VirtualClient::RegisterSocket(SOCKET s, const sockaddr* addr)
{
	// Flush virtual buffer queue
	char buffer[2048];
	WSABUF buf = { 2048,buffer };
	DWORD numBytesRecvd;
	DWORD flags = 0;
	sockaddr_in from;
	int fromLen = sizeof(sockaddr_in);
	int r = 0;
	WSAOVERLAPPED over;
	over.hEvent = m_virtSockState.overlappedRecvEvent;
	while (true)
	{
		r = m_recvFrom(m_virtSocket, &buf, 1, &numBytesRecvd, &flags, (sockaddr*)&from, &fromLen, &over, NULL);
		if (0 == r)
		{
			DWORD trans, flags;
			if (!m_overlapped(m_virtSocket, &over, &trans, FALSE, &flags))
			{
				std::cout << "Unexpected error while flushing virtual socket (GetOverlappedResult)! Error : " << WSAGetLastError() << std::endl;
				break;
			}
			else
			{
				std::cout << "Flushed virtual packet" << std::endl;
				continue;
			}
		}
		else if (r == SOCKET_ERROR && WSA_IO_PENDING == WSAGetLastError())
		{
			DWORD wait = m_waitMult(1, &m_virtSockState.overlappedRecvEvent, false, 1, FALSE);
			if (wait == WSA_WAIT_TIMEOUT)
			{
				if (!CancelIo((HANDLE)m_virtSocket))
				{
					std::cout << "Could not cancel peek receive! Error : " << GetLastError() << std::endl;
				}

				break;
			}
			else if (wait == WSA_WAIT_EVENT_0)
			{
				DWORD trans, flags;
				if (!m_overlapped(m_virtSocket, &over, &trans, FALSE, &flags))
				{
					std::cout << "Unexpected error while flushing virtual socket (GetOverlappedResult)! Error : " << WSAGetLastError() << std::endl;
					break;
				}
				else
				{
					std::cout << "Flushed virtual packet" << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "Unexpected error while flushing virtual socket (WaitForMultipleEvents)! Error : " << WSAGetLastError() << std::endl;
				break;
			}
		}
		else
		{
			std::cout << "Unexpected error while flushing virtual socket (RecvFrom)! Error : " << WSAGetLastError() << std::endl;
			break;
		}
	}

	VirtualIP::RegisterSocket(s, addr);
	m_socketStates[s]->s = s;
	m_socketStates[s]->virtRecvState = VirtRecv_StateNone;
	m_socketStates[s]->port = ((sockaddr_in*)addr)->sin_port;

	return 0;
}

int VirtualClient::ReleaseSocket(SOCKET s)
{
	return VirtualIP::ReleaseSocket(s);;
}

int VirtualClient::HandleVirtNetwork(SocketState** pprecvSock)
{
	int ret = VirtualIP::HandleVirtNetwork(pprecvSock);
	if (ret == VirtHandleNet_UnknownPort)
	{
		std::cout << "Virtual server sending to unknown port!" << std::endl;
	}

	return ret;
}

int VirtualClient::DSendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr * lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped)
{
	SocketState* state = m_socketStates[s];
	VirtIPHeader header;
	if ((((sockaddr_in*)lpTo)->sin_addr.s_addr & 0x00FFFFFF) != 0x0050a8c0 && INADDR_BROADCAST != ((sockaddr_in*)lpTo)->sin_addr.s_addr)
	{
		CHAR addr[16];
		InetNtopA(AF_INET, &((sockaddr_in*)lpTo)->sin_addr, addr, sizeof(addr));
		std::cout << "Sending to unknown address!" << addr << std::endl;
	}

	header.virtIP = ((sockaddr_in*)lpTo)->sin_addr;
	header.virtPort = ((sockaddr_in*)lpTo)->sin_port;
	memcpy_s(state->sendBuffer, sizeof(state->sendBuffer), &header, sizeof(header));
	memcpy_s(state->sendBuffer + sizeof(header), sizeof(state->sendBuffer) - sizeof(header), lpBuffers->buf, lpBuffers->len);
	DWORD numBytesSend = 0;
	state->virtSendBuffers.buf = state->sendBuffer;
	state->virtSendBuffers.len = sizeof(header) + lpBuffers->len;
	int toClientID;
	if (INADDR_BROADCAST == ((sockaddr_in*)lpTo)->sin_addr.s_addr)
	{
		toClientID = INSTANCESETTINGS_HOSTINST;
	}
	else
	{
		toClientID = GetClientID(header.virtIP = ((sockaddr_in*)lpTo)->sin_addr);
	}

	sockaddr_in toPhysAddr;
	toPhysAddr.sin_family = AF_INET;
	toPhysAddr.sin_addr = in4addr_loopback;
	toPhysAddr.sin_port = htons(GetClientPhysHPort(toClientID));
	WritePacketInfoToLog(L"Send,Virt,Imme", sockaddr_in{ AF_INET,state->port,m_localAddr }, *((sockaddr_in*)lpTo), lpBuffers->len);
	int r = m_sendTo(m_virtSocket, &state->virtSendBuffers, 1, &numBytesSend, dwFlags, (sockaddr*)&toPhysAddr, sizeof(sockaddr_in), lpOverlapped, NULL);
	int err = WSAGetLastError();
	if (r)
	{
		std::cout << "Sending packet failed with " << r << " and WSALastError " << WSAGetLastError() << std::endl;
	}
	return r;
}

int VirtualClient::DRecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, sockaddr * lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped)
{
	SocketState* state = m_socketStates[s];
	state->lpNumberOfBytesRecvd = lpNumberOfBytesRecvd;
	state->lpRecvBuffers = lpBuffers;
	state->overlappedRecvEvent = lpOverlapped->hEvent;
	state->lpRecvOverlapped = lpOverlapped;
	state->lpRecvFrom = lpFrom;
	state->lpRecvFromlen = lpFromlen;
	WSASetLastError(WSA_IO_PENDING);
	return SOCKET_ERROR;
}

BOOL VirtualClient::DGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags)
{
	SocketState* state = m_socketStates[s];
	BOOLEAN ret;
	if (VirtRecv_StateVirtOverlapped == state->virtRecvState)
	{
		state->virtRecvState = VirtRecv_StateNone;
		VirtReceiveBuffer buf = state->virtRecvBuffers.front();
		state->virtRecvBuffers.pop();
		VirtIPHeader virtHead = *((VirtIPHeader*)buf.buffer);

		// Check if virtual client send broadcast
		if (in4addr_broadcast.s_addr == virtHead.virtIP.s_addr)
		{
			((sockaddr_in*)state->lpRecvFrom)->sin_addr = in4addr_any;
		}
		else
		{
			int id = GetClientID(buf.recvFrom.sin_port);
			((sockaddr_in*)state->lpRecvFrom)->sin_addr = GetClientVirtAddr(id);
		}

		((sockaddr_in*)state->lpRecvFrom)->sin_port = virtHead.virtPort;
		((sockaddr_in*)state->lpRecvFrom)->sin_family = AF_INET;
		memcpy_s(state->lpRecvBuffers->buf, state->lpRecvBuffers->len,
			buf.buffer + sizeof(VirtIPHeader), sizeof(buf.buffer) - sizeof(VirtIPHeader));
		*lpcbTransfer = buf.numBytesRecvd - sizeof(VirtIPHeader);
		CheckPacketIntegrity(state->lpRecvBuffers->buf, *lpcbTransfer);
		ret = TRUE;
	}
	else if (VirtRecv_StateVirtBuffered >= state->virtRecvState)
	{
		std::cout << "Unexpected virtual receive state!" << std::endl;
		ret = FALSE;
	}
	else
	{
		std::cout << "Unexpected virtual receive state! Virtual client can not receive physical packets." << std::endl;
		ret = FALSE;
	}

	WritePacketInfoToLog(L"Recv,Virt,Asyn", *((sockaddr_in*)state->lpRecvFrom), sockaddr_in{ AF_INET,state->port,m_localAddr }, *lpcbTransfer);
	return ret;
}

DWORD VirtualClient::DWaitForEvents(DWORD cEvents, const WSAEVENT * lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
{
	return VirtualIP::DWaitForEvents(cEvents, lphEvents, fWaitAll, dwTimeout, fAlertable);
}

BOOL VirtualClient::DWSAResetEvent(WSAEVENT hEvent)
{
	return m_reset(hEvent);
}
