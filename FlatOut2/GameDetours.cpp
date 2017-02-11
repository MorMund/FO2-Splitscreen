#include "stdafx.h"
#include "GameDetours.h"

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
}
