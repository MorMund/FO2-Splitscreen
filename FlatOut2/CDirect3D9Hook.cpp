#include "stdafx.h"
#include "CDirect3D9Hook.h"

void CDirect3D9Hook::SetDebugText(LPWSTR text)
{
	if (m_pdev != NULL)
	{
		m_pdev->SetDebugText(text);
	}
}

HRESULT CDirect3D9Hook::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DDevice9 ** ppReturnedDeviceInterface)
{
	pPresentationParameters->Windowed = true;
	HRESULT hr = m_ptr->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	m_pdev = new CDirect3D9DevHook(*ppReturnedDeviceInterface);
	*ppReturnedDeviceInterface = m_pdev;
	return hr;
}
