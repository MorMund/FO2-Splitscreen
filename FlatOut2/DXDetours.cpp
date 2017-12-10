#include "stdafx.h"
#include "DXDetours.h"

PVOID oDX9Create;
CDirect3D9Hook* d3d;

IDirect3D9* WINAPI myCreateDx9(UINT sdkVer)
{
	IDirect3D9* idx = ((dxCreate)oDX9Create)(sdkVer);
	if (idx == NULL)
	{
		WCHAR errMsg[DEFAULT_ERRMSG_BUFFER];
		StringCbPrintf(errMsg, sizeof(errMsg), L"Failed to create DirectX9 object. SDK Version : %d", sdkVer);
		MessageBox(NULL, errMsg, L"DirectX Error", MB_OK | MB_ICONERROR);
		throw std::exception("Failed to create DirectX9 object.");
	}
	d3d = new CDirect3D9Hook(idx);
	IDirect3D9* proxy = static_cast<IDirect3D9*>(d3d);
	InstanceSettings::GetSettings()->SetDirect3DHook(d3d);
	return proxy;
}

void DXDetour()
{
	HMODULE dxDLL = LoadLibrary(L"d3d9.dll");
	oDX9Create = GetProcAddress(dxDLL, "Direct3DCreate9");
	DetourAttach(&oDX9Create, myCreateDx9);
}