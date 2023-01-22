// FlatOut2.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FlatOut2.h"
#include "InstanceSettings.h"

HMODULE dllHandle = NULL;

struct DevicesArray
{
	ControllerInfo* devices;
	int deviceIndex;
	int deviceCount;
};

BOOL _stdcall DIEnumDevicesCallback(LPCDIDEVICEINSTANCEW lpddi, LPVOID pvRef)
{
	DevicesArray* devs = (DevicesArray*)pvRef;
	if (devs->deviceIndex < devs->deviceCount)
	{
		ControllerInfo* dev = &devs->devices[devs->deviceIndex];
		memcpy_s(dev->deviceName, sizeof(dev->deviceName), lpddi->tszProductName, sizeof(lpddi->tszProductName));
		memcpy_s(dev->instanceName, sizeof(dev->instanceName), lpddi->tszInstanceName, sizeof(lpddi->tszInstanceName));
		dev->guid = lpddi->guidInstance;
	}

	devs->deviceIndex++;
	return DIENUM_CONTINUE;
}

FLATOUT2_API  int _stdcall CreateNewInstance(LPWSTR cmdLine)
{
	std::cout << "Launching FlatOut2 executable.\n";
	STARTUPINFO suInfo = STARTUPINFO();
	ZeroMemory(&suInfo, sizeof(STARTUPINFO));
	suInfo.cb = sizeof(STARTUPINFO);
	suInfo.dwX = CW_USEDEFAULT;
	suInfo.dwY = CW_USEDEFAULT;
	suInfo.dwXSize = CW_USEDEFAULT;
	suInfo.dwYSize = CW_USEDEFAULT;
	suInfo.dwFlags |= STARTF_USESTDHANDLES;

	LPCSTR hookDllPath = "FlatOut2.dll";

	PROCESS_INFORMATION pInf;
	BOOL result = DetourCreateProcessWithDlls(L"FlatOut2.exe", cmdLine, NULL, NULL, TRUE, NULL, NULL, NULL, &suInfo, &pInf, 1, &hookDllPath, NULL);
	if (!result) {
		return S_FALSE;
	}

	return S_OK;
}

FLATOUT2_API BOOL _stdcall GetSharedSettingsSize(int* settingsSize, int sizeCount)
{
	if (sizeCount != 3)
	{
		return false;
	}
	else
	{
		settingsSize[0] = sizeof(SET_SharedSettings);
		settingsSize[1] = sizeof(SET_GlobalSettings);
		settingsSize[2] = sizeof(SET_InstanceSettings);
		return true;
	}
}

FLATOUT2_API  BOOL _stdcall GetControllerGuids(ControllerInfo* devices, LPINT deviceCount)
{
	IDirectInput8* di;
	HRESULT res = DirectInput8Create(dllHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&di, NULL);
	if (res != DI_OK)
	{
		return FALSE;
	}
	DevicesArray dev = { devices,0,*deviceCount };
	res = di->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback, &dev, DIEDFL_ALLDEVICES);
	if (res != DI_OK)
	{
		di->Release();
		return FALSE;
	}
	*deviceCount = dev.deviceIndex;
	di->Release();
	return TRUE;
}

FLATOUT2_API BOOL _stdcall GetResolutions(LPRECT resolutions, LPINT resoCount)
{
	IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == d3d)
	{
		return FALSE;
	}
	UINT adaptCount = d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	int resoIndex = 0;
	for (UINT adaptIndex = 0; adaptIndex < adaptCount; adaptIndex++)
	{
		D3DDISPLAYMODE mode;
		if (d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, adaptIndex, &mode) != D3D_OK)
		{
			d3d->Release();
			return FALSE;
		}
		if ((0 != mode.RefreshRate && 60 != mode.RefreshRate) || D3DFMT_X8R8G8B8 != mode.Format)
		{
			continue;
		}
		if (resoIndex < *resoCount)
		{
			resolutions[resoIndex] = { 0,0,(LONG)mode.Width,(LONG)mode.Height };
		}
		resoIndex++;
	}
	*resoCount = resoIndex;
	return TRUE;
}
