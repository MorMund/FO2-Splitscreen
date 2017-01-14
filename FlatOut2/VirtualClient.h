#pragma once
#include "stdafx.h"
#include "VirtualIP.h"


class VirtualClient : VirtualIP
{
public:
	VirtualClient(PVOID* oCalls, int callCount);
	~VirtualClient();
private:
	int HandleVirtNetwork(SocketState** pprecvSock);
public:
	// Inherited via VirtualIP
	virtual BOOL Init() override;
	virtual int RegisterSocket(SOCKET s, const sockaddr* addr) override;
	virtual int ReleaseSocket(SOCKET s) override;
	virtual int DSendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr * lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped) override;
	virtual int DRecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, sockaddr * lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped) override;
	virtual int DGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags) override;
	virtual DWORD DWaitForEvents(DWORD cEvents, const WSAEVENT * lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable) override;
	virtual BOOL DWSAResetEvent(WSAEVENT hEvent) override;
};

