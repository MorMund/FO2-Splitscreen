#pragma once
#include "stdafx.h"

#define XBOXBT_A 0
#define XBOXBT_B 1
#define XBOXBT_X 2
#define XBOXBT_Y 3
#define XBOXBT_BACK 6
#define XBOXBT_START 7

#define XBOX_JOY_THRESHOLD 8000
#define XBOX_JOY_R_THRESHOLD 5000

class CDIDevice8Hook : public IDirectInputDevice8
{
private:
	IDirectInputDevice8* m_ptr;
	DIJOYSTATE2 m_currentJoyState;
	DIJOYSTATE2 m_lastJoyState;
public:
	CDIDevice8Hook(IDirectInputDevice8* ptr) : m_ptr(ptr) {}
public:
	BOOL _stdcall IsButtonPress(DWORD button);
	void _stdcall GetStickDirection(LPINT upDown, LPINT rightLeft);
	HRESULT _stdcall SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
	HRESULT _stdcall GetDeviceState(DWORD cbData, LPVOID lpvData);
	COM_METHOD(HRESULT, QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) { return m_ptr->QueryInterface(riid, ppvObj); }
	COM_METHOD(ULONG, AddRef)(THIS) { return m_ptr->AddRef(); }
	COM_METHOD(ULONG, Release)(THIS) { return m_ptr->Release(); }
	COM_METHOD(HRESULT, GetCapabilities)(THIS_ LPDIDEVCAPS a) { return m_ptr->GetCapabilities(a); }
	COM_METHOD(HRESULT, EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW a, LPVOID b, DWORD c) { return m_ptr->EnumObjects(a, b, c); }
	COM_METHOD(HRESULT, GetProperty)(THIS_ REFGUID a, LPDIPROPHEADER b) { return m_ptr->GetProperty(a, b); }
	COM_METHOD(HRESULT, SetProperty)(THIS_ REFGUID a, LPCDIPROPHEADER b) { return m_ptr->SetProperty(a, b); }
	COM_METHOD(HRESULT, Acquire)(THIS) { return m_ptr->Acquire(); }
	COM_METHOD(HRESULT, Unacquire)(THIS) { return m_ptr->Unacquire(); }
	COM_METHOD(HRESULT, GetDeviceData)(THIS_ DWORD a, LPDIDEVICEOBJECTDATA b, LPDWORD c, DWORD d) { return m_ptr->GetDeviceData(a, b, c, d); }
	COM_METHOD(HRESULT, SetDataFormat)(THIS_ LPCDIDATAFORMAT a) { return m_ptr->SetDataFormat(a); }
	COM_METHOD(HRESULT, SetEventNotification)(THIS_ HANDLE a) { return m_ptr->SetEventNotification(a); }
	COM_METHOD(HRESULT, GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEW a, DWORD b, DWORD c) { return m_ptr->GetObjectInfo(a, b, c); }
	COM_METHOD(HRESULT, GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEW a) { return m_ptr->GetDeviceInfo(a); }
	COM_METHOD(HRESULT, RunControlPanel)(THIS_ HWND a, DWORD b) { return m_ptr->RunControlPanel(a, b); }
	COM_METHOD(HRESULT, Initialize)(THIS_ HINSTANCE a, DWORD b, REFGUID c) { return m_ptr->Initialize(a, b, c); }
	COM_METHOD(HRESULT, CreateEffect)(THIS_ REFGUID a, LPCDIEFFECT b, LPDIRECTINPUTEFFECT * c, LPUNKNOWN d) { return m_ptr->CreateEffect(a, b, c, d); }
	COM_METHOD(HRESULT, EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKW a, LPVOID b, DWORD c) { return m_ptr->EnumEffects(a, b, c); }
	COM_METHOD(HRESULT, GetEffectInfo)(THIS_ LPDIEFFECTINFOW a, REFGUID b) { return m_ptr->GetEffectInfo(a, b); }
	COM_METHOD(HRESULT, GetForceFeedbackState)(THIS_ LPDWORD a) { return m_ptr->GetForceFeedbackState(a); }
	COM_METHOD(HRESULT, SendForceFeedbackCommand)(THIS_ DWORD a) { return m_ptr->SendForceFeedbackCommand(a); }
	COM_METHOD(HRESULT, EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK a, LPVOID b, DWORD c) { return m_ptr->EnumCreatedEffectObjects(a, b, c); }
	COM_METHOD(HRESULT, Escape)(THIS_ LPDIEFFESCAPE a) { return m_ptr->Escape(a); }
	COM_METHOD(HRESULT, Poll)(THIS) { return m_ptr->Poll(); }
	COM_METHOD(HRESULT, SendDeviceData)(THIS_ DWORD a, LPCDIDEVICEOBJECTDATA b, LPDWORD c, DWORD d) { return m_ptr->SendDeviceData(a, b, c, d); }
	COM_METHOD(HRESULT, EnumEffectsInFile)(THIS_ LPCWSTR a, LPDIENUMEFFECTSINFILECALLBACK b, LPVOID c, DWORD d) { return m_ptr->EnumEffectsInFile(a, b, c, d); }
	COM_METHOD(HRESULT, WriteEffectToFile)(THIS_ LPCWSTR a, DWORD b, LPDIFILEEFFECT c, DWORD d) { return m_ptr->WriteEffectToFile(a, b, c, d); }
	COM_METHOD(HRESULT, SetActionMap)(THIS_ LPDIACTIONFORMATW a, LPCWSTR b, DWORD c) { return m_ptr->SetActionMap(a, b, c); }
	COM_METHOD(HRESULT, GetImageInfo)(THIS_ LPDIDEVICEIMAGEINFOHEADERW a) { return m_ptr->GetImageInfo(a); }
	COM_METHOD(HRESULT, BuildActionMap)(THIS_ LPDIACTIONFORMATW a, LPCWSTR b, DWORD c) { return m_ptr->BuildActionMap(a, b, c); }
};
