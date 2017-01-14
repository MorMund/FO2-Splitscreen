#include "stdafx.h"
#include "DetourCallDefs.h"
#include "User32Detours.h"
#include "WinSockDetours.h"
#include "DXDetours.h"
#include "DIDetours.h"
#include "DebugOutput.h"

void earlyDetour()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	User32Detour();
	WinSockDetour();
	DXDetour();
	DIDetour();
	if (DetourTransactionCommit() != NO_ERROR)
	{
		std::cout << "Detour Error!" << std::endl;
		dbgError = L"Detour Error!";
		throw std::exception("Detour Error!");
	}
}

LONG WINAPI TerminateHandler(LPEXCEPTION_POINTERS exception)
{
	WCHAR buffer[4096];
	StringCbPrintf(buffer, sizeof(buffer), L"Unhandled exception at 0x%x! Error Code :0x%x", exception->ExceptionRecord->ExceptionAddress, exception->ExceptionRecord->ExceptionCode);
	MessageBox(NULL, buffer, L"Unhandled Exception", MB_OK | MB_ICONERROR);
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	dllHandle = hModule;
	WCHAR modBuf[512];
	SetUnhandledExceptionFilter(TerminateHandler);
	if (GetModuleBaseName(GetCurrentProcess(), NULL, modBuf, sizeof(modBuf)) != NULL)
	{
		if (lstrcmpi(modBuf, L"FlatOut2.exe") != 0)
		{
			return TRUE;
		}
	}
	else
	{
		throw new std::exception();
	}
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		earlyDetour();
		if (InstanceSettings::GetSettings() == NULL)
		{
			int nArgs = -1, instanceID = -1, i = 0;
			LPWSTR *szArglist;
			szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
			while (i < nArgs)
			{
				TCHAR ident = szArglist[i][0];
				if (ident == '-')
				{
					TCHAR cmd = szArglist[i][1];
					switch (cmd)
					{
					case 'i':
						instanceID = _ttoi(szArglist[++i]);
						break;
					default:
						std::cout << "Unknown command \"" << cmd << "\".";
						break;
					}
				}
				i++;
			}
			LocalFree(szArglist);
			InstanceSettings::InitSettings(instanceID);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

