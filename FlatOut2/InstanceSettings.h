#include "stdafx.h"
#include "CDirect3D9Hook.h"
#include "CDirectInput8Hook.h"

#ifndef INSTANCESETTINGS
#define INSTANCESETTINGS
#define INSTANCESETTINGS_HOSTINST 0
#define INSTANCESETTINGS_MEMNAME L"Local\\FO2_SS_Shared"
#define INSTANCESETTINGS_WAITNAME L"Local\\FO2_SS_Wait"

struct SET_InstanceSettings
{
	int instanceID;
	GUID instanceController;
	HWND mainHwnd = NULL;
	DWORD procID = NULL;
	RECT windowPos;
	CDirect3D9Hook* d3d = NULL;
	CDirectInput8Hook* directInput = NULL;
};

struct SET_NetSettings
{
	u_short virtHostPort;
	u_char virtAddressOffset;
	char virtAddressRange[16];
};

struct SET_GlobalSettings
{
	int instanceCount;
	BOOL useInputEmulation;
	BOOL skipIntros;
	SET_NetSettings network;
	Logging::Level logFileVerbosity;
	Logging::Level consoleVerbosity;
	GUID controller[FO2_MaxClientCount];
	RECT windowPos[FO2_MaxClientCount];
};

struct SET_SharedSettings
{
	u_short virtClientPorts[FO2_MaxClientCount];
	SET_GlobalSettings globals;
	SET_InstanceSettings instances[FO2_MaxClientCount];
};

class InstanceSettings
{
public:
	~InstanceSettings();
public:
	static InstanceSettings* InitSettings(int instanceID);
	static InstanceSettings* GetSettings();
	SET_InstanceSettings GetLocalSettings();
	SET_NetSettings GetNetworkSettings();
	BOOL IsHostInstance();
	int GetInstanceCount();
	int GetInstanceID();
	int GetInstanceID(u_short clientPortH);
	u_short GetInstanceVirtPort(int instanceID);
	Logging::Level GetConsoleVerbosity() { return m_settings->globals.consoleVerbosity; }
	Logging::Level GetLogFileVerbosity() { return m_settings->globals.logFileVerbosity; }
	void SetInstanceVirtPort(u_short clientPortH);
	void SetGameWindowHandle(HWND window);
	void SetDirect3DHook(CDirect3D9Hook* d3d);
	void SetDirectInputHook(CDirectInput8Hook* dinput);
	BOOL UseInputEmulation();
	BOOL SkipIntros();
private:
	InstanceSettings();
private:
	static InstanceSettings* m_instanceSettings;
private:
	HANDLE m_sharedMem;
	SET_SharedSettings* m_settings;
	int m_instanceID;
};
#endif