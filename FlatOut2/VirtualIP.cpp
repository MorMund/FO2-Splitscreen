#include "stdafx.h"
#include "VirtualIP.h"

hostent hostQuery;
char hostNameQuery[256];
char* hostAddrQueryList[2];
char hostAddrQuery[sizeof(in_addr)];

const char* VirtualIP::tag = "NETWORK";

VirtualIP::VirtualIP(BOOL isServer, PVOID* oCalls, int callCount)
{
	m_sharedSettings = InstanceSettings::GetSettings();
	m_settings = m_sharedSettings->GetNetworkSettings();
	InetPtonA(AF_INET, m_settings.virtAddressRange, &m_addressRange);
#if oCall_Count != 10
#error Call defs not updated
#endif	
	m_recvFrom = (RecvFrom)oCalls[oCall_recvFrom];
	m_sendTo = (SendTo)oCalls[oCall_sendTo];
	m_overlapped = (OverlappedResult)oCalls[oCall_overlapped];
	m_bind = (sBind)oCalls[oCall_wbind];
	m_closeSock = (sClose)oCalls[oCall_wclosesock];
	m_waitMult = (WSAWaitMultiple)oCalls[oCall_wWaitMult];
	m_getAdaptInfo = (GetAdaptInfo)oCalls[oCall_getAdaptInfo];
	m_reset = (WSAEReset)oCalls[oCall_resetEvent];
	m_hostName = (hostName)oCalls[oCall_hostName];
	m_hostByName = (hostbyname)oCalls[oCall_hostByName];
}

VirtualIP::~VirtualIP()
{
}

void VirtualIP::SetClientVirtualPort(u_short portH)
{
	m_sharedSettings->SetInstanceVirtPort(portH);
}

BOOL VirtualIP::Init()
{
	m_virtSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_virtSocket == INVALID_SOCKET)
		return FALSE;
	m_virtSockState.overlappedRecvEvent = WSACreateEvent();
	m_virtSockState.virtRecvState = VirtRecv_StateNone;
	m_virtSockState.port = m_settings.virtHostPort;
	m_virtSockState.s = m_virtSocket;
	m_virtSockState.virtSendBuffers.buf = m_virtSockState.sendBuffer;
	m_virtSockState.virtSendBuffers.len = sizeof(m_virtSockState.sendBuffer);
	VirtReceiveBuffer virtBuf;
	m_virtSockState.virtRecvBuffers.push(virtBuf);
	m_virtSockState.virtRecvBuffers.back().wsaBuf.buf = m_virtSockState.virtRecvBuffers.back().buffer;
	m_virtSockState.virtRecvBuffers.back().wsaBuf.len = sizeof(m_virtSockState.virtRecvBuffers.back().buffer);
	m_socketStates[m_virtSocket] = &m_virtSockState;
	return TRUE;
}

int VirtualIP::RegisterSocket(SOCKET s, const sockaddr * addr)
{
	int c = m_socketStates.count(s);
	if (c)
	{
		return -1;
	}
	SocketState ss;
	m_internalSocketState.push_back(ss);
	m_socketStates[s] = &m_internalSocketState.back();
	return 0;
}

int VirtualIP::ReleaseSocket(SOCKET s)
{
	m_socketStates.erase(s);
	return 0;
}

int VirtualIP::PrepareEvent(SocketState* recvSocket, DWORD cEvents, const WSAEVENT * lphEvents)
{
	WSAEVENT sockEv = recvSocket->overlappedRecvEvent;
	for (size_t i = 0; i < cEvents; i++)
	{
		if (sockEv == lphEvents[i])
		{
			recvSocket->virtRecvState = VirtRecv_StateVirtOverlapped;
			m_ignoredEvents[sockEv] = TRUE;
			return WSA_WAIT_EVENT_0 + i;
		}
	}

	return -1;
}

DWORD VirtualIP::DWaitForEvents(DWORD cEvents, const WSAEVENT * lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
{
	WSAEVENT events[10];
	while (true)
	{
		int v = UpdateVirtualSocket(NULL, cEvents, lphEvents);
		// Check if the currently waiting socket(s) have a buffered virtual packet
		for(auto statePair = m_socketStates.begin(); statePair != m_socketStates.end(); ++statePair)
		{
			if (statePair->second->virtRecvBuffers.size() > 0)
			{
				int eventIndex = PrepareEvent(statePair->second, cEvents, lphEvents);
				if (-1 != eventIndex)
				{
					return eventIndex;
				}
			}
		}

		memcpy_s(events, sizeof(events), lphEvents, cEvents * sizeof(WSAEVENT));
		events[cEvents] = m_virtSockState.overlappedRecvEvent;
		DWORD r = m_waitMult(cEvents + 1, events, fWaitAll, dwTimeout, fAlertable);
		if (WSA_WAIT_TIMEOUT == r || (r - WSA_WAIT_EVENT_0) < cEvents)
		{
			// Is physical event
			return r;
		}
		else
		{
			SocketState* state;
			int virtNet = HandleVirtNetwork(&state);
			if (virtNet != VirtHandleNet_Buffered)
			{
				std::ostringstream msg;
				msg << "Virtual network receive error! " << WSAGetLastError();
				Logging::getInstance().error(tag, msg.str());
			}

			int eventIndex = PrepareEvent(state, cEvents, lphEvents);
			if (-1 != eventIndex)
			{
				return eventIndex;
			}
		}
	}
}

DWORD VirtualIP::DGetAdaptInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen)
{
	DWORD err = m_getAdaptInfo(pAdapterInfo, pOutBufLen);
	if (err == ERROR_SUCCESS)
	{
		IP_ADAPTER_INFO virtualAdapter;
		ZeroMemory(&virtualAdapter, sizeof(IP_ADAPTER_INFO));
		memcpy_s(virtualAdapter.Address, sizeof(virtualAdapter.Address), pAdapterInfo->Address, sizeof(pAdapterInfo->Address));
		virtualAdapter.AddressLength = pAdapterInfo->AddressLength;
		// The game client refuses messages from its own MAC address, could also be solved by patching packets
		virtualAdapter.Address[pAdapterInfo->AddressLength - 1] += GetClientID();
		strcpy_s(virtualAdapter.Description, "FlatOut2 Virtual Network Adapter");
		strcpy_s(virtualAdapter.AdapterName, pAdapterInfo->AdapterName);
		virtualAdapter.DhcpEnabled = pAdapterInfo->DhcpEnabled;
		virtualAdapter.DhcpServer = pAdapterInfo->DhcpServer;
		virtualAdapter.GatewayList = pAdapterInfo->GatewayList;
		virtualAdapter.HaveWins = pAdapterInfo->HaveWins;
		virtualAdapter.PrimaryWinsServer = pAdapterInfo->PrimaryWinsServer;
		virtualAdapter.SecondaryWinsServer = pAdapterInfo->SecondaryWinsServer;
		virtualAdapter.Type = pAdapterInfo->Type;
		virtualAdapter.LeaseExpires = pAdapterInfo->LeaseExpires;
		virtualAdapter.LeaseObtained = pAdapterInfo->LeaseObtained;
		virtualAdapter.Index = 0;
		virtualAdapter.Next = NULL;

		IP_ADDR_STRING virtualIPList;
		InetNtopA(AF_INET, &m_localAddr, virtualIPList.IpAddress.String, sizeof(virtualIPList.IpAddress.String));
		virtualIPList.Next = NULL;
		virtualIPList.Context = NULL;
		strcpy_s(virtualIPList.IpMask.String, "255.255.255.0");
		virtualAdapter.IpAddressList = virtualIPList;

		memcpy_s(pAdapterInfo, *pOutBufLen, &virtualAdapter, sizeof(virtualAdapter));
		return ERROR_SUCCESS;
	}
	else if (err == ERROR_BUFFER_OVERFLOW)
	{
		Logging::getInstance().error(tag, std::string("GetAdapterInfo virtual client buffer overflow not implemented!"));
		return ERROR_INVALID_DATA;
	}
	else
	{
		return err;
	}
}

int VirtualIP::HandleVirtNetwork(SocketState** pprecvSock)
{
	*pprecvSock = NULL;
	SocketState virtSS = *m_socketStates[m_virtSocket];
	VirtReceiveBuffer &virtBuf = virtSS.virtRecvBuffers.back();
	DWORD transfer;
	DWORD flags = 0;
	m_virtSockState.isReceiving = false;
	BOOL r = m_overlapped(m_virtSocket, &virtSS.virtOverlapped, &transfer, false, &flags);
	if (!r)
	{
		std::ostringstream msg;
		msg << "GetOverlappedResult of the virtual socket failed with : " << WSAGetLastError();
		Logging::getInstance().error(tag, msg.str());
		return VirtHandleNet_Error;
	}

	VirtIPHeader virtHead = *((VirtIPHeader*)virtBuf.wsaBuf.buf);
	for (auto statePair = m_socketStates.begin(); statePair != m_socketStates.end(); ++statePair)
	{
		SocketState* sockState = statePair->second;
		if (sockState->port == virtHead.virtPort)
		{
			*pprecvSock = sockState;
			if (sockState->virtRecvBuffers.size() >= VirtualBuffer_MaxQueue)
			{
				Logging::getInstance().error(tag, std::string("Socket virtual buffer queue is full!"));
				return VirtHandleNet_Error;
			}

			VirtReceiveBuffer buf;
			memcpy_s(buf.buffer, sizeof(buf.buffer), virtBuf.wsaBuf.buf, virtBuf.wsaBuf.len);
			buf.numBytesRecvd = transfer;
			buf.recvFrom = virtBuf.recvFrom;
			buf.recvFromLen = virtBuf.recvFromLen;
			sockState->virtRecvBuffers.push(buf);
			sockState->virtRecvState = VirtRecv_StateVirtBuffered;
			return VirtHandleNet_Buffered;
		}
	}

	return VirtHandleNet_UnknownPort;
}

int VirtualIP::UpdateVirtualSocket(SOCKET s, DWORD cEvents, const WSAEVENT* lphEvents)
{
	BOOL newPacket;

	while (true)
	{
		newPacket = FALSE;
		// Check if the virtual socket is currently waiting for an overlapped receive
		if (m_virtSockState.isReceiving)
		{
			// See if there is an overlapped result
			DWORD wait = m_waitMult(1, &m_virtSockState.overlappedRecvEvent, FALSE, 0, FALSE);
			if (WSA_WAIT_FAILED == wait)
			{
				std::ostringstream msg;
				msg << "An unexpected error occurred while polling the virtual socket. Error : " << WSAGetLastError();
				Logging::getInstance().error(tag, msg.str());
				throw new std::exception("An unexpected error occurred while polling the virtual socket.");
			}
			else if (WSA_WAIT_EVENT_0 == wait)
			{
				// There is a new overlapped result
				newPacket = TRUE;
			}
			else
			{
				// There is no new virtual packet
				return -1;
			}
		}
		else
		{
			// Start receiving a virtual packet
			DWORD flags = 0;
			m_virtSockState.virtOverlapped.hEvent = m_virtSockState.overlappedRecvEvent;
			VirtReceiveBuffer &virtBuf = m_virtSockState.virtRecvBuffers.back();
			virtBuf.recvFromLen = sizeof(sockaddr_in);
			DWORD vres = m_recvFrom(m_virtSocket,
				&virtBuf.wsaBuf,
				1,
				NULL,
				&flags,
				(sockaddr*)&virtBuf.recvFrom,
				&virtBuf.recvFromLen,
				&m_virtSockState.virtOverlapped,
				NULL);
			if (vres == SOCKET_ERROR)
			{
				// There was no packet waiting or a socket error occurred
				int wErr = WSAGetLastError();
				if (wErr == WSA_IO_PENDING)
				{
					m_virtSockState.isReceiving = TRUE;
					return  -1;
				}
				else
				{
					std::ostringstream msg;
					msg << "Virtual network receive error! " << WSAGetLastError();
					Logging::getInstance().error(tag, msg.str());
					return  VirtHandleNet_Error;
				}
			}
			else
			{
				// A packet could be received immediately
				newPacket = TRUE;
			}
		}

		if (!newPacket)
		{
			Logging::getInstance().error(tag, std::string("Unexpected state in virtual socket update!"));
			throw new std::exception("Unexpected state in virtual socket update!");
		}

		SocketState* state;
		int i = HandleVirtNetwork(&state);
		if (i != VirtHandleNet_Buffered)
		{
			std::ostringstream msg;
			msg << "Virtual network Error : " << i;
			Logging::getInstance().error(tag, msg.str());
		}
		else
		{
			//Check if a socket is waiting for this virtual packet
			if (state->s == s)
			{
				return WSA_WAIT_EVENT_0;
			}
			else
			{
				for (size_t i = 0; i < cEvents; i++)
				{
					if (lphEvents[i] == state->overlappedRecvEvent)
					{
						return WSA_WAIT_EVENT_0 + i;
					}
				}
			}
		}
	}
}

int VirtualIP::GetHostName(char * name, int namelen)
{
	StringCbPrintfA(name, namelen, "%s%d", VirtNet_HostName, m_sharedSettings->GetInstanceID());
	return 0;
}

hostent * VirtualIP::GetHostByName(const char * name)
{
	if (0 == strncmp(VirtNet_HostName, name, sizeof(VirtNet_HostName) - 1))
	{
		const char* clIDStr = name + sizeof(VirtNet_HostName) - 1;
		int clID = atoi(clIDStr);
		auto clientVirtAddr = GetClientVirtAddr(clID);

		strcpy_s(hostNameQuery, name);
		memcpy_s(hostAddrQuery, sizeof(hostAddrQuery), &clientVirtAddr, sizeof(in_addr));
		hostAddrQueryList[0] = hostAddrQuery;
		hostAddrQueryList[1] = NULL;
		hostQuery.h_name = hostNameQuery;
		hostQuery.h_aliases = NULL;
		hostQuery.h_addrtype = AF_INET;
		hostQuery.h_length = sizeof(in_addr);
		hostQuery.h_addr_list = hostAddrQueryList;
		return &hostQuery;
	}
	else
	{
		std::ostringstream msg;
		msg << "Looking up non virtual host name : " << name;
		Logging::getInstance().warn(tag, msg.str());
		return m_hostByName(name);
	}
}

void VirtualIP::WritePacketInfoToLog(const char* tags, sockaddr_in from, sockaddr_in to, int len)
{
	std::ostringstream msg;
	char addrFrom[16];
	char addrTo[16];
	InetNtopA(AF_INET, &from.sin_addr, addrFrom, sizeof(addrFrom));
	InetNtopA(AF_INET, &to.sin_addr, addrTo, sizeof(addrTo));
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	msg << sysTime.wHour << ':' << sysTime.wMinute << ':' << sysTime.wSecond << '.' << sysTime.wMilliseconds
		<< " [" << tags << "] " << addrFrom << ':' << ntohs(from.sin_port) << "->" << addrTo << ':' << ntohs(to.sin_port) << ' ' << len << " bytes";
	Logging::getInstance().debug(tag, msg.str());
}

bool VirtualIP::CheckPacketIntegrity(char * buf, int buflen)
{
	bool failed = FALSE;
	if (*(WORD*)buf != (buflen - 2))
	{
		Logging::getInstance().error(tag, std::string("Packet integrity check failed : Header packet length info does not match packet length!"));
		failed = TRUE;
	}

	if (strncmp(buf + FO2Packet_MagicOffset, FO2Packet_Magic, sizeof(FO2Packet_Magic) - 1) != 0)
	{
		Logging::getInstance().error(tag, std::string("Packet integrity check failed : Header invalid magic!"));
		failed = TRUE;
	}

	return !failed;
}

in_addr VirtualIP::GetClientVirtAddr(int clientID)
{
	if (!IsValidClientID(clientID))
	{
		throw new std::exception("Client ID out of range.");
	}

	in_addr addr;
	int s = sizeof(u_long);
	addr = m_addressRange;
	addr.S_un.S_un_b.s_b4 = clientID + m_settings.virtAddressOffset;
	return addr;
}

in_addr VirtualIP::GetClientVirtAddr()
{
	return GetClientVirtAddr(m_sharedSettings->GetInstanceID());
}

int VirtualIP::GetClientID(short netOrdPort)
{
	int id = m_sharedSettings->GetInstanceID(ntohs(netOrdPort));
	if (!IsValidClientID(id))
	{
		throw new std::exception("Client ID out of range.");
	}

	return id;
}

int VirtualIP::GetClientID(in_addr virtAddr)
{
	int id = virtAddr.S_un.S_un_b.s_b4 - m_settings.virtAddressOffset;
	if (!IsValidClientID(id))
	{
		throw new std::exception("Client ID out of range.");
	}

	return id;
}

int VirtualIP::GetClientID()
{
	return m_sharedSettings->GetInstanceID();
}

u_short VirtualIP::GetClientPhysHPort(int clientID)
{
	if (!IsValidClientID(clientID))
	{
		throw new std::exception("Client ID out of range.");
	}

	return m_sharedSettings->GetInstanceVirtPort(clientID);
}

bool VirtualIP::IsValidClientID(int clientID)
{
	return clientID >= 0 && clientID < m_sharedSettings->GetInstanceCount();
}
