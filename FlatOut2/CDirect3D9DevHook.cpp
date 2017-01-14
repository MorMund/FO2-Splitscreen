#include "stdafx.h"
#include "CDirect3D9DevHook.h"

void CDirect3D9DevHook::SetDebugText(LPWSTR dbgText)
{
	m_dbgText = dbgText;
}

HRESULT CDirect3D9DevHook::EndScene()
{
	if (m_dbgText != NULL)
	{
		RECT debugInfRect;
		SetRect(&debugInfRect, 20, 50, 500, 100);
		m_dbgFont->DrawTextW(NULL, m_dbgText, -1, &debugInfRect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
	}
	return  m_ptr->EndScene();
}
