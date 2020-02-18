#include "stdafx.h"
#include "GameDetours.h"
#include "BinaryPatching.h"

PVOID oFO2PlayMov;
PVOID oFO2PlayIntros;

void _stdcall MyFO2PlayMovie(UINT unknown1, LPCSTR vidPath, UINT unknown2)
{
	((PlayMovie)oFO2PlayMov)(unknown1, vidPath, unknown2);
}

void _stdcall MyFO2PlayIntro()
{
	return;
}

/* The function's signature is LoadImageA except the first parameter which is patched to contain the value of the EAX register.
This register (sometimes) contains the reason why the configuration window is show.*/
HANDLE IncorrectConfigHandler(DWORD incorrectConfigError, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad) {
	ExitProcess(1);
}

PVOID indirectConfigHandler = IncorrectConfigHandler;
void GameDetour()
{
	// TODO : Use relative addresses.
	oFO2PlayIntros = FO2_PlayIntroAddress;
	oFO2PlayMov = FO2_PlayMovieAddress;
	if (InstanceSettings::GetSettings()->SkipIntros())
	{
		DetourAttach(&oFO2PlayIntros, MyFO2PlayIntro);
		DetourAttach(&oFO2PlayMov, MyFO2PlayMovie);
	}

	BYTE expectedInstr[] = { 0xff, 0x15 ,0x48 ,0x22 ,0x65 ,0x00 };
	if (!PatchCallInstruction((PBYTE)FO2_InvalidConfigAddress, expectedInstr, &indirectConfigHandler)
		|| !PatchPushInstruction((PBYTE)FO2_InvalidConfigAddress - 1, 0x55, BINPATCH_REGISTER_OFFSETS::eax)) {
		throw std::exception("Failed to patch instruction");
	}
}
