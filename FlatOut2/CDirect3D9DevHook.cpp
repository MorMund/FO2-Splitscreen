#include "stdafx.h"
#include "CDirect3D9DevHook.h"

HRESULT CDirect3D9DevHook::EndScene()
{
	return  m_ptr->EndScene();
}
