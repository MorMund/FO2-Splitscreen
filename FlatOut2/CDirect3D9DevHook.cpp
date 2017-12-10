#include "stdafx.h"
#include "CDirect3D9DevHook.h"

CDirect3D9DevHook::CDirect3D9DevHook(IDirect3DDevice9 * ptr)
{
	if (ptr == NULL) {
		MessageBox(NULL, L"Error in CDirect3D9DevHook::CDirect3D9DevHook. DirectX object pointer was null!", L"DirectX Error", MB_OK | MB_ICONERROR);
		throw std::exception("Error in CDirect3D9DevHook::CDirect3D9DevHook. DirectX object pointer was null!");
	}

	m_ptr = ptr;
}

HRESULT CDirect3D9DevHook::EndScene()
{
	return  m_ptr->EndScene();
}
