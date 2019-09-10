#include "stdafx.h"
#include "WinSockDetours.h"

VirtualIP *virtIP = NULL;
PVOID oBind;
PVOID oRecvFrom;
PVOID oSendTo;
PVOID oOverlappedRes;
PVOID oWSAStart;
PVOID oWSAClean;
PVOID oCloseSocket;
PVOID oWaitMult;
PVOID oWSAEReset;
PVOID oGetAdapterInfo;
PVOID oGetHostName;
PVOID oGetHostByName;
LARGE_INTEGER countFreq;

int WINAPI MyBind(SOCKET s, const struct sockaddr *name, int namelen)
{
	return virtIP->RegisterSocket(s, name);
}

int WINAPI MyWSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD  dwBufferCount, LPDWORD lpNumberOfBytesRecvd,
	LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	return virtIP->DRecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped);
}

int WINAPI MyWSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent,
	DWORD dwFlags, const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	return virtIP->DSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iToLen, lpOverlapped);
}

BOOL WSAAPI MyWSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags)
{
	return virtIP->DGetOverlappedResult(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
}

int WSAAPI MyCloseSocket(SOCKET s)
{
	return virtIP->ReleaseSocket(s);
}

DWORD WINAPI MyGetAdaptInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen)
{
	return virtIP->DGetAdaptInfo(pAdapterInfo, pOutBufLen);
}

DWORD WSAAPI MyWaitForMulti(DWORD cEvents, const WSAEVENT *lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
{
	LARGE_INTEGER before;
	LARGE_INTEGER after;
	QueryPerformanceCounter(&before);
	DWORD r = virtIP->DWaitForEvents(cEvents, lphEvents, fWaitAll, dwTimeout, fAlertable);
	QueryPerformanceCounter(&after);
	if (((double)(after.QuadPart - before.QuadPart) / countFreq.QuadPart) > dwTimeout && dwTimeout > 0)
	{
		std::ostringstream msg;
		msg << "WaitForMultipleEvents took longer than dwTimeout : "
			<< std::setprecision(2) << ((double)(after.QuadPart - before.QuadPart) / countFreq.QuadPart) * 1000
			<< " ms";
		Logging::getInstance().error("NETWORK", msg.str());
	}
	return r;
}

BOOL WSAAPI MyResetEvent(WSAEVENT hEvent)
{
	return virtIP->DWSAResetEvent(hEvent);
}

int WSAAPI MyCleanup()
{
	Logging::getInstance().error("NETWORK", std::string("WinSock clean up"));
	return ((WSAClean)oWSAClean)();
}

int WSAAPI MyWSAStart(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	int r = ((WSAStart)oWSAStart)(wVersionRequested, lpWSAData);
	if (r == 0)
	{
#if oCall_Count != 10
#error Call defs not updated
#endif
		PVOID calls[10];
		calls[oCall_recvFrom] = oRecvFrom;
		calls[oCall_sendTo] = oSendTo;
		calls[oCall_overlapped] = oOverlappedRes;
		calls[oCall_wbind] = oBind;
		calls[oCall_wclosesock] = oCloseSocket;
		calls[oCall_wWaitMult] = oWaitMult;
		calls[oCall_getAdaptInfo] = oGetAdapterInfo;
		calls[oCall_resetEvent] = oWSAEReset;
		calls[oCall_hostName] = oGetHostName;
		calls[oCall_hostByName] = oGetHostByName;
		if (InstanceSettings::GetSettings()->IsHostInstance())
		{
			VirtualHost* vHost = new VirtualHost(InstanceSettings::GetSettings()->GetInstanceCount(), calls, 8);
			if (!vHost->Init())
			{
				Logging::getInstance().error("NETWORK", std::string("Virtual network host initialization failure!"));
			}
			virtIP = (VirtualIP*)vHost;
		}
		else
		{
			VirtualClient* virtCl = new VirtualClient(calls, 8);
			virtIP = (VirtualIP*)virtCl;
			if (!virtCl->Init())
			{
				Logging::getInstance().error("NETWORK", std::string("Virtual network client initialization failure!"));
			}
		}
	}
	return r;
}

int WSAAPI myGethostname(char* name, int namelen)
{
	return virtIP->GetHostName(name, namelen);
}

struct hostent* FAR WSAAPI myGethostbyname(const char* name)
{
	return virtIP->GetHostByName(name);
}

void WinSockDetour()
{
	QueryPerformanceFrequency(&countFreq);

	HMODULE wsock2 = LoadLibrary(L"Ws2_32.dll");
	HMODULE Iphlpapi = LoadLibrary(L"Iphlpapi.dll");

	oBind = GetProcAddress(wsock2, "bind");
	oSendTo = GetProcAddress(wsock2, "WSASendTo");
	oRecvFrom = GetProcAddress(wsock2, "WSARecvFrom");
	oOverlappedRes = GetProcAddress(wsock2, "WSAGetOverlappedResult");
	oCloseSocket = GetProcAddress(wsock2, "closesocket");
	oWSAClean = GetProcAddress(wsock2, "WSACleanup");
	oWaitMult = GetProcAddress(wsock2, "WSAWaitForMultipleEvents");
	oWSAStart = GetProcAddress(wsock2, "WSAStartup");
	oWSAEReset = GetProcAddress(wsock2, "WSAResetEvent");
	oGetHostName = GetProcAddress(wsock2, "gethostname");
	oGetHostByName = GetProcAddress(wsock2, "gethostbyname");
	oGetAdapterInfo = GetProcAddress(Iphlpapi, "GetAdaptersInfo");

	DetourAttach(&oBind, MyBind);
	DetourAttach(&oSendTo, MyWSASendTo);
	DetourAttach(&oRecvFrom, MyWSARecvFrom);
	DetourAttach(&oOverlappedRes, MyWSAGetOverlappedResult);
	DetourAttach(&oCloseSocket, MyCloseSocket);
	DetourAttach(&oWSAClean, MyCleanup);
	DetourAttach(&oWaitMult, MyWaitForMulti);
	DetourAttach(&oWSAStart, MyWSAStart);
	DetourAttach(&oGetHostName, myGethostname);
	DetourAttach(&oGetHostByName, myGethostbyname);
	DetourAttach(&oGetAdapterInfo, MyGetAdaptInfo);
	DetourAttach(&oWSAEReset, MyResetEvent);
}