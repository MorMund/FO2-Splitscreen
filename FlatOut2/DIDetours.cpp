#include "stdafx.h"
#include "DIDetours.h"

PVOID oDIn8Create;

HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT res = ((diCreate)oDIn8Create)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	IDirectInput8* input = static_cast<IDirectInput8*>(*ppvOut);
	CDirectInput8Hook* directInput = new CDirectInput8Hook(input);
	InstanceSettings::GetSettings()->SetDirectInputHook(directInput);
	(*ppvOut) = static_cast<IDirectInput8*>(directInput);
	return res;
}

void DIDetour()
{
	HMODULE diDLL = LoadLibrary(L"Dinput8.dll");
	oDIn8Create = GetProcAddress(diDLL, "DirectInput8Create");
	DetourAttach(&oDIn8Create, MyDirectInput8Create);
}
