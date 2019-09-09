// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "targetver.h"
#define DIRECTINPUT_VERSION 0x0800
#define DEFAULT_ERRMSG_BUFFER 4096
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Strsafe.h>
#include <shellapi.h>
#include <detours.h>
#include <d3d9.h>
#include <initguid.h>
#include <dinput.h>
#include <Iphlpapi.h>
#include <Psapi.h>

// TODO: reference additional headers your program requires here
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <queue>
#include <tchar.h>
#include "Logging.h"
#include "FlatOut2.h"
#include "VirtualClient.h"
#include "VirtualHost.h"

