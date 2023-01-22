#include "stdafx.h"
#include "DsDetours.h"

PVOID oDsCreate8;

HRESULT MyDsCreate8(LPCGUID lpcGuidDevice, LPDIRECTSOUND8* ppDS8, LPUNKNOWN pUnkOuter)
{
	HRESULT result = ((DsCreate8)oDsCreate8)(lpcGuidDevice, ppDS8, pUnkOuter);
	CDirectSound8Hook* hook = new CDirectSound8Hook(*ppDS8);
	*ppDS8 = static_cast<LPDIRECTSOUND8>(hook);
	return result;
}

void DsDetour()
{
	HMODULE dsDLL = LoadLibrary(L"dsound.dll");
	oDsCreate8 = GetProcAddress(dsDLL, "DirectSoundCreate8");
	DetourAttach(&oDsCreate8, MyDsCreate8);
}