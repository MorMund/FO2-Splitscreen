#include "stdafx.h"
#include "InstanceSettings.h"

InstanceSettings* InstanceSettings::m_instanceSettings = NULL;

InstanceSettings::~InstanceSettings()
{
	if (m_settings != NULL)
	{
		UnmapViewOfFile(m_settings);
	}

	if (m_sharedMem != NULL)
	{
		CloseHandle(m_sharedMem);
	}
}

InstanceSettings* InstanceSettings::InitSettings(int instanceID)
{
	if (m_instanceSettings != NULL)
	{
		throw std::exception("Tried to initialize instance settings more than once!");
	}
	m_instanceSettings = new InstanceSettings();
	m_instanceSettings->m_sharedMem = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		INSTANCESETTINGS_MEMNAME);
	if (m_instanceSettings->m_sharedMem == NULL)
	{
		int err = GetLastError();
		CloseHandle(m_instanceSettings->m_sharedMem);
		throw new std::exception("Could not open settings shared memory!", err);
	}
	m_instanceSettings->m_settings = (SET_SharedSettings*)MapViewOfFile(m_instanceSettings->m_sharedMem,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(SET_SharedSettings));
	if (m_instanceSettings->m_settings == NULL)
	{
		int err = GetLastError();
		throw new std::exception("Could not map settings shared memory!", err);
	}
	m_instanceSettings->m_instanceID = instanceID;
	SET_InstanceSettings* instSet = &m_instanceSettings->m_settings->instances[instanceID];
	instSet->instanceID = instanceID;
	instSet->procID = GetCurrentProcessId();
	instSet->instanceController = m_instanceSettings->m_settings->globals.controller[instanceID];
	instSet->windowPos = m_instanceSettings->m_settings->globals.windowPos[instanceID];
	if (m_instanceSettings->m_instanceID == INSTANCESETTINGS_HOSTINST)
	{
		HANDLE wait = OpenEvent(EVENT_ALL_ACCESS, false, INSTANCESETTINGS_WAITNAME);
		if (NULL == wait)
		{
			std::ostringstream msg;
			msg << "Could not open launcher wait handle! Error : " << GetLastError();
			Logging::getInstance().error("SETTINGS", msg.str());
		}

		if (!SetEvent(wait))
		{
			std::ostringstream msg;
			msg << "Could not set launcher wait event! Error : " << GetLastError();
			Logging::getInstance().error("SETTINGS", msg.str());
		}

		CloseHandle(wait);
	}
	return m_instanceSettings;
}

InstanceSettings * InstanceSettings::GetSettings()
{
	return m_instanceSettings;
}

SET_InstanceSettings  InstanceSettings::GetLocalSettings()
{
	return m_instanceSettings->m_settings->instances[m_instanceID];
}

SET_NetSettings InstanceSettings::GetNetworkSettings()
{
	return m_settings->globals.network;
}

BOOL InstanceSettings::IsHostInstance()
{
	return (INSTANCESETTINGS_HOSTINST == m_instanceID);
}

int InstanceSettings::GetInstanceCount()
{
	return m_settings->globals.instanceCount;
}

int InstanceSettings::GetInstanceID()
{
	return m_instanceID;
}

int InstanceSettings::GetInstanceID(u_short clientPortH)
{
	if (clientPortH == 0)
	{
		return -1;
	}

	for (int i = 0; i < sizeof(m_settings->virtClientPorts); i++)
	{
		if (m_settings->virtClientPorts[i] == clientPortH)
		{
			return i;
		}
	}

	return -1;
}

void InstanceSettings::SetInstanceVirtPort(u_short clientPortH)
{
	m_settings->virtClientPorts[m_instanceID] = clientPortH;
}

u_short InstanceSettings::GetInstanceVirtPort(int instanceID)
{
	if (instanceID == INSTANCESETTINGS_HOSTINST)
	{
		return m_settings->globals.network.virtHostPort;
	}
	else if (FO2_MaxClientCount <= instanceID)
	{
		throw std::exception("Argument out of Range");
	}
	return m_settings->virtClientPorts[instanceID];
}

void InstanceSettings::SetGameWindowHandle(HWND window)
{
	m_instanceSettings->m_settings->instances[m_instanceID].mainHwnd = window;
}

void InstanceSettings::SetDirect3DHook(CDirect3D9Hook * d3d)
{
	m_instanceSettings->m_settings->instances[m_instanceID].d3d = d3d;
}

void InstanceSettings::SetDirectInputHook(CDirectInput8Hook * dinput)
{
	m_instanceSettings->m_settings->instances[m_instanceID].directInput = dinput;
}

BOOL InstanceSettings::UseInputEmulation()
{
	return m_settings->globals.useInputEmulation;
}

BOOL InstanceSettings::SkipIntros()
{
	return m_settings->globals.skipIntros;
}

InstanceSettings::InstanceSettings()
{
}
