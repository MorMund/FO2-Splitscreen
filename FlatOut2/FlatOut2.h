// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FLATOUT2_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FLATOUT2_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef FLATOUT2_EXPORTS
#define FLATOUT2_API extern "C" __declspec(dllexport) 
#else
#define FLATOUT2_API extern "C" __declspec(dllimport) 
#endif

#define injectDllName "FlatOut2.dll"
#define FO2_AllowAttach FALSE
#define FO2_AllowAttach2 FALSE
#define FO2_MaxClientCount 8

struct ControllerInfo {
	WCHAR deviceName[512];
	WCHAR instanceName[512];
	GUID guid;
};

FLATOUT2_API int _stdcall CreateNewInstance(LPWSTR cmdLine);
FLATOUT2_API BOOL _stdcall GetSharedSettingsSize(int* settingsSize, int sizeCount);
FLATOUT2_API BOOL _stdcall GetControllerGuids(ControllerInfo* devices, LPINT deviceCount);
FLATOUT2_API BOOL _stdcall GetResolutions(LPRECT resolutions, LPINT resoCount);
extern HMODULE dllHandle;


