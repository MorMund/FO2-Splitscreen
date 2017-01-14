#include "stdafx.h"
#include "CDirectInput8Hook.h"

struct DIEnumCallback
{
	LPDIENUMDEVICESCALLBACKW origCallback;
	LPVOID origRef;
};

BOOL WINAPI EnumCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DIEnumCallback* orig = (DIEnumCallback*)pvRef;
	GUID instanceDev = InstanceSettings::GetSettings()->GetLocalSettings().instanceController;
	if (lpddi->guidInstance == instanceDev && GUID_NULL != instanceDev)
	{
		BOOL ret = orig->origCallback(lpddi, orig->origRef);
		return DIENUM_STOP;
	}
	else
	{
		return DIENUM_CONTINUE;
	}
}

CDIDevice8Hook * CDirectInput8Hook::GetDevice()
{
	return m_dev;
}

HRESULT CDirectInput8Hook::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	DIEnumCallback* eCallback = new DIEnumCallback();
	eCallback->origCallback = lpCallback;
	eCallback->origRef = pvRef;
	HRESULT res = m_ptr->EnumDevices(dwDevType, EnumCallback, eCallback, dwFlags);
	delete eCallback;
	return res;
}

HRESULT CDirectInput8Hook::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W * lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	HRESULT res = m_ptr->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);
	IDirectInputDevice8* inputDev = static_cast<IDirectInputDevice8*>(*lplpDirectInputDevice);
	m_dev = new CDIDevice8Hook(inputDev);
	(*lplpDirectInputDevice) = static_cast<IDirectInputDevice8*>(m_dev);
	return  res;
}
