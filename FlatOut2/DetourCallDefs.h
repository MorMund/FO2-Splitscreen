#include "stdafx.h"

#pragma once
typedef IDirect3D9* (WINAPI *dxCreate)(UINT);
typedef HRESULT(WINAPI *diCreate)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
typedef HWND(WINAPI* CreateWinExA)
(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x,
	int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
typedef BOOL(WINAPI* PeekMessageT)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
typedef HHOOK(WINAPI* SetWinHookExA)(int idHook, HOOKPROC  lpfn, HINSTANCE hMod, DWORD dwThreadId);
typedef LRESULT(WINAPI* CallNextHook)(HHOOK hhk, int code, WPARAM wParam, LPARAM lParam);
typedef LRESULT(CALLBACK* KBHookProc)(int code, WPARAM wParam, LPARAM lParam);
typedef int(WSAAPI* sBind)(SOCKET s, const struct sockaddr *name, int namelen);
typedef int(WSAAPI* sClose)(SOCKET s);
typedef int(WSAAPI* WSAStart)(WORD wVersionRequested, LPWSADATA lpWSAData);
typedef int(WSAAPI* WSAClean)();
typedef DWORD(WSAAPI* WSAWaitMultiple)(DWORD cEvents, const WSAEVENT *lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);
typedef BOOL(WSAAPI* OverlappedResult)(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
typedef int (WSAAPI* RecvFrom)(
	SOCKET s, LPWSABUF lpBuffers, DWORD  dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
	struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef int(WSAAPI* SendTo)(
	SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr *lpTo,
	int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef u_short(WSAAPI* HTONS)(u_short hostshort);
typedef BOOL(WSAAPI* WSAEReset)(WSAEVENT hEvent);
typedef DWORD(WINAPI* GetAdaptInfo)(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);
typedef int (WSAAPI* hostName)(char* name, int namelen);
typedef struct hostent* FAR(WSAAPI* hostbyname)(const char* name);
typedef int(_stdcall* _WinMain)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
typedef void(_stdcall* PlayMovie)(UINT unknown1, LPCSTR vidPath, UINT unknown2);
typedef void(_stdcall* VoidCall)();
#define COM_METHOD(TYPE, METHOD) TYPE STDMETHODCALLTYPE METHOD