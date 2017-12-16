#include "stdafx.h"
#include "CDirect3D9Hook.h"

CDirect3D9Hook::CDirect3D9Hook(IDirect3D9 * ptr)
{
	if (ptr == NULL) {
		MessageBox(NULL, L"Error in CDirect3D9Hook::CDirect3D9Hook. DirectX object pointer was null!", L"DirectX Error", MB_OK | MB_ICONERROR);
		throw std::exception("Error in CDirect3D9Hook::CDirect3D9Hook. DirectX object pointer was null!");
	}

	m_ptr = ptr;
}

BOOL WriteErrorLog(const char* filename, HRESULT hr, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DDevice9 ** ppReturnedDeviceInterface)
{
	std::ofstream errorLog = std::ofstream(filename, std::ofstream::out | std::ofstream::app);
	errorLog
		<< "Error log CDirect3D9Hook::CreateDevice." << std::endl
		<< "HRESULT : " << hr << std::endl
		<< "Adapter : " << Adapter << std::endl
		<< "DeviceType : " << DeviceType << std::endl
		<< "hFocusWindow : " << hFocusWindow << std::endl
		<< "BehaviorFlags : " << BehaviorFlags << std::endl
		<< "pPresentationParameters : " << pPresentationParameters << std::endl
		<< "ppReturnedDeviceInterface : " << ppReturnedDeviceInterface << std::endl
		<< "Presentation Parameters : " << std::endl
		<< "\tBackBufferWidth : " << pPresentationParameters->BackBufferWidth << std::endl
		<< "\tBackBufferHeight : " << pPresentationParameters->BackBufferHeight << std::endl
		<< "\tBackBufferFormat : " << pPresentationParameters->BackBufferFormat << std::endl
		<< "\tBackBufferCount : " << pPresentationParameters->BackBufferCount << std::endl
		<< "\tMultiSampleType : " << pPresentationParameters->MultiSampleType << std::endl
		<< "\tMultiSampleQuality : " << pPresentationParameters->MultiSampleQuality << std::endl
		<< "\tSwapEffect : " << pPresentationParameters->SwapEffect << std::endl
		<< "\thDeviceWindow : " << pPresentationParameters->hDeviceWindow << std::endl
		<< "\tWindowed : " << pPresentationParameters->Windowed << std::endl
		<< "\tEnableAutoDepthStencil : " << pPresentationParameters->EnableAutoDepthStencil << std::endl
		<< "\tAutoDepthStencilFormat : " << pPresentationParameters->AutoDepthStencilFormat << std::endl
		<< "\tFlags : " << pPresentationParameters->Flags << std::endl
		<< "\tFullScreen_RefreshRateInHz : " << pPresentationParameters->FullScreen_RefreshRateInHz << std::endl;
	bool good = errorLog.good();
	errorLog.close();
	return good;
}

HRESULT CDirect3D9Hook::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DDevice9 ** ppReturnedDeviceInterface)
{
	pPresentationParameters->Windowed = true;
	pPresentationParameters->FullScreen_RefreshRateInHz = 0;
	HRESULT hr = m_ptr->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	if (hr != D3D_OK)
	{
		WCHAR errMsg[DEFAULT_ERRMSG_BUFFER];
		const WCHAR* error =
			(hr == D3DERR_DEVICELOST ? L"D3DERR_DEVICELOST" :
				hr == D3DERR_INVALIDCALL ? L"D3DERR_INVALIDCALL" :
				hr == D3DERR_NOTAVAILABLE ? L"D3DERR_NOTAVAILABLE" :
				hr == D3DERR_OUTOFVIDEOMEMORY ? L"D3DERR_OUTOFVIDEOMEMORY" : L"D3DERR_UNKNOWN");
		if (WriteErrorLog("d3derror.txt", hr, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface))
		{
			StringCbPrintf(errMsg, sizeof(errMsg), L"Failed to create DirectX9 Device : %s(%d). Additional info was written to d3derror.txt.", error, hr);
		}
		else
		{
			StringCbPrintf(errMsg, sizeof(errMsg), L"Failed to create DirectX9 Device : %s(%d). Additional info could NOT be written to disk.", error, hr);
		}
		MessageBox(NULL, errMsg, L"DirectX Error", MB_OK | MB_ICONERROR);
		throw new std::exception("Failed to create DirectX9 Device.");
	}
	m_pdev = new CDirect3D9DevHook(*ppReturnedDeviceInterface);
	*ppReturnedDeviceInterface = m_pdev;
	return hr;
}
