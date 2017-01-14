#pragma once
#include "stdafx.h"
#include "CDirect3D9DevHook.h"

class CDirect3D9Hook : public IDirect3D9
{
private:
	IDirect3D9* m_ptr;
	CDirect3D9DevHook* m_pdev;
public:
	CDirect3D9Hook(IDirect3D9* ptr) : m_ptr(ptr) {}
public:
	void _stdcall SetDebugText(LPWSTR text);
	HRESULT _stdcall CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);

	/*** IUnknown methods ***/
	COM_METHOD(HRESULT, QueryInterface)(THIS_ REFIID riid, void** ppvObj) { /*TODO*/ return m_ptr->QueryInterface(riid, ppvObj); }
	COM_METHOD(ULONG, AddRef)(THIS) { /*TODO*/ return m_ptr->AddRef(); }
	COM_METHOD(ULONG, Release)(THIS) { /*TODO*/ return m_ptr->Release(); }

	/*** IDirect3D9 methods ***/
	COM_METHOD(HRESULT, RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) { /*TODO*/ return m_ptr->RegisterSoftwareDevice(pInitializeFunction); }
	COM_METHOD(UINT, GetAdapterCount)(THIS) { /*TODO*/ return m_ptr->GetAdapterCount(); }
	COM_METHOD(HRESULT, GetAdapterIdentifier)(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) { /*TODO*/ return m_ptr->GetAdapterIdentifier(Adapter, Flags, pIdentifier); }
	COM_METHOD(UINT, GetAdapterModeCount)(THIS_ UINT Adapter, D3DFORMAT Format) { /*TODO*/ return m_ptr->GetAdapterModeCount(Adapter, Format); }
	COM_METHOD(HRESULT, EnumAdapterModes)(THIS_ UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) { /*TODO*/ return m_ptr->EnumAdapterModes(Adapter, Format, Mode, pMode); }
	COM_METHOD(HRESULT, GetAdapterDisplayMode)(THIS_ UINT Adapter, D3DDISPLAYMODE* pMode) { /*TODO*/ return m_ptr->GetAdapterDisplayMode(Adapter, pMode); }
	COM_METHOD(HRESULT, CheckDeviceType)(THIS_ UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) { /*TODO*/ return m_ptr->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed); }
	COM_METHOD(HRESULT, CheckDeviceFormat)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) { /*TODO*/ return m_ptr->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
	COM_METHOD(HRESULT, CheckDeviceMultiSampleType)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) { /*TODO*/ return m_ptr->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels); }
	COM_METHOD(HRESULT, CheckDepthStencilMatch)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) { /*TODO*/ return m_ptr->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
	COM_METHOD(HRESULT, CheckDeviceFormatConversion)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) { /*TODO*/ return m_ptr->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat); }
	COM_METHOD(HRESULT, GetDeviceCaps)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) { /*TODO*/ return m_ptr->GetDeviceCaps(Adapter, DeviceType, pCaps); }
	COM_METHOD(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) { /*TODO*/ return m_ptr->GetAdapterMonitor(Adapter); }
};
