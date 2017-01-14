#pragma once
#include "stdafx.h"
#include "CDIDevice8Hook.h"

BOOL WINAPI EnumCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

class CDirectInput8Hook : public IDirectInput8
{
private:
	IDirectInput8* m_ptr;
	CDIDevice8Hook* m_dev;
public:
	CDirectInput8Hook(IDirectInput8* ptr) : m_ptr(ptr)
	{
	}
public:
	CDIDevice8Hook* _stdcall GetDevice();
	HRESULT _stdcall EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
	HRESULT _stdcall CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W * lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
	COM_METHOD(HRESULT, QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) { return m_ptr->QueryInterface(riid, ppvObj); }
	COM_METHOD(ULONG, AddRef(THIS)) { return  m_ptr->AddRef(); }
	COM_METHOD(ULONG, Release(THIS)) { return  m_ptr->Release(); }

	COM_METHOD(HRESULT, GetDeviceStatus)(THIS_ REFGUID a) { return  m_ptr->GetDeviceStatus(a); }
	COM_METHOD(HRESULT, RunControlPanel)(THIS_ HWND a, DWORD b) { return m_ptr->RunControlPanel(a, b); }
	COM_METHOD(HRESULT, Initialize)(THIS_ HINSTANCE a, DWORD b) { return  m_ptr->Initialize(a, b); }
	COM_METHOD(HRESULT, FindDevice)(THIS_ REFGUID a, LPCWSTR b, LPGUID c) { return m_ptr->FindDevice(a, b, c); }
	COM_METHOD(HRESULT, EnumDevicesBySemantics)(THIS_ LPCWSTR a, LPDIACTIONFORMATW b, LPDIENUMDEVICESBYSEMANTICSCBW c, LPVOID d, DWORD e) { return m_ptr->EnumDevicesBySemantics(a, b, c, d, e); }
	COM_METHOD(HRESULT, ConfigureDevices)(THIS_ LPDICONFIGUREDEVICESCALLBACK a, LPDICONFIGUREDEVICESPARAMSW b, DWORD c, LPVOID d) { return  m_ptr->ConfigureDevices(a, b, c, d); }
};