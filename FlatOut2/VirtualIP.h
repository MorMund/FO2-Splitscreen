#pragma once
#include "stdafx.h"
#include "DetourCallDefs.h"
#include "InstanceSettings.h"

#define VirtRecv_StateNone 0
#define VirtRecv_StateVirtBuffered 1
#define VirtRecv_StateVirtOverlapped 2

#define VirtHandleNet_Buffered -1
#define VirtHandleNet_UnknownPort -2
#define VirtHandleNet_Error -3

#define VirtNet_SubnetMask 0xFFFFFF00
#define VirtNet_HostName "FO2-VirtClient"

#define oCall_recvFrom 0
#define oCall_sendTo 1
#define oCall_overlapped 2
#define oCall_wbind  3 
#define oCall_wclosesock 4
#define oCall_wWaitMult 5
#define oCall_getAdaptInfo 6
#define oCall_resetEvent 7
#define oCall_hostName 8
#define oCall_hostByName 9
#define oCall_Count 10

#define VirtualBuffer_MaxQueue 64

#define VirtualLog_MutexName L"Local\\FO2_SS_VirtNetLock"
#define VirtualLog_FileName L"netLog.txt"

#define FO2Packet_MagicOffset 10
#define FO2Packet_Magic "FO14"

struct VirtReceiveBuffer {
	char buffer[2048];
	WSABUF wsaBuf;
	sockaddr_in recvFrom;
	int recvFromLen = sizeof(sockaddr_in);
	DWORD numBytesRecvd;
};

struct VirtIPHeader {
	in_addr virtIP;
	u_short virtPort;
};

struct SocketState {
	SOCKET s;
	u_short port;
	BOOL isReceiving = FALSE;
	DWORD virtRecvState = VirtRecv_StateNone;
	WSAEVENT overlappedRecvEvent;
	LPWSABUF lpRecvBuffers;
	LPWSAOVERLAPPED lpRecvOverlapped;
	LPDWORD lpNumberOfBytesRecvd;
	sockaddr* lpRecvFrom;
	LPINT lpRecvFromlen;
	std::queue<VirtReceiveBuffer> virtRecvBuffers;
	char sendBuffer[2048];
	VirtReceiveBuffer physRecvBuffer;
	WSABUF virtSendBuffers;
	WSAOVERLAPPED virtOverlapped;
};

class VirtualIP
{
public:
	VirtualIP(BOOL isServer, PVOID* oCalls, int callCount);
	~VirtualIP();
	void SetClientVirtualPort(u_short portH);
	virtual BOOL Init();
	virtual int RegisterSocket(SOCKET s, const sockaddr* addr);
	virtual int ReleaseSocket(SOCKET s) = 0;
	virtual int DSendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent,
		DWORD dwFlags, const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped) = 0;
	virtual int DRecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD  dwBufferCount, LPDWORD lpNumberOfBytesRecvd,
		LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped) = 0;
	virtual BOOL DGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags) = 0;
	virtual DWORD DWaitForEvents(DWORD cEvents, const WSAEVENT *lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);
	virtual DWORD DGetAdaptInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);
	virtual BOOL DWSAResetEvent(WSAEVENT hEvent) = 0;
	virtual int GetHostName(char* name, int namelen);
	virtual struct hostent* FAR GetHostByName(const char* name);
protected:
	static const char* tag;
protected:
	virtual int HandleVirtNetwork(SocketState** pprecvSock);
protected:
	in_addr GetClientVirtAddr(int clientID);
	in_addr GetClientVirtAddr();
	int GetClientID(short netOrdPort);
	int GetClientID(in_addr virtAddr);
	int GetClientID();
	u_short GetClientPhysHPort(int clientID);
	int UpdateVirtualSocket(SOCKET s, DWORD cEvents, const WSAEVENT* lphEvents);
	void WritePacketInfoToLog(const char* tags, sockaddr_in from, sockaddr_in to, int len);
	bool CheckPacketIntegrity(char* buf, int buflen);
private:
	int PrepareEvent(SocketState* recvSocket, DWORD cEvents, const WSAEVENT * lphEvents);
	bool IsValidClientID(int clientID);
protected:
	RecvFrom m_recvFrom;
	SendTo m_sendTo;
	OverlappedResult m_overlapped;
	sBind m_bind;
	sClose m_closeSock;
	WSAWaitMultiple m_waitMult;
	GetAdaptInfo m_getAdaptInfo;
	WSAEReset m_reset;
	hostName m_hostName;
	hostbyname m_hostByName;
	BOOL m_isServer;
	in_addr m_localAddr;
	std::unordered_map<SOCKET, SocketState*> m_socketStates;
	SET_NetSettings m_settings;
	in_addr m_addressRange;
	SOCKET m_virtSocket = SOCKET_ERROR;
	SocketState m_virtSockState;
	std::unordered_map<WSAEVENT, BOOL> m_ignoredEvents;
private:
	std::list<SocketState> m_internalSocketState;
	InstanceSettings* m_sharedSettings;
};

