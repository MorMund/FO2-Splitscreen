#include "stdafx.h"
#include "DetourCallDefs.h"
#include "User32Detours.h"
#include "WinSockDetours.h"
#include "DXDetours.h"
#include "DIDetours.h"
#include "GameDetours.h"

static int (WINAPI* TrueEntryPoint)(VOID) = NULL;

int WINAPI entryPointWithDetours(VOID)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	User32Detour();
	WinSockDetour();
	DXDetour();
	DIDetour();
	GameDetours();
	if (DetourTransactionCommit() != NO_ERROR)
	{
		Logging::getInstance().error("DETOURS", std::string("Detour Error!"));
		throw std::exception("Detour Error!");
	}

	return TrueEntryPoint();
}

void GenerateExceptionData(WCHAR* buffer, size_t size, PEXCEPTION_RECORD exception)
{
	switch (exception->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		if (exception->NumberParameters < 2)
		{
			StringCbPrintf(buffer, size, L"Access Violation (no info)");
		}
		else
		{
			auto type = exception->ExceptionInformation[0];
			
			StringCbPrintf(buffer, size, L"%s ACCESS VIOLATION at 0x%x",
				type == 0 ? L"READ" : type == 1 ? L"WRITE" : type == 8 ? L"DEP" : L"UNKNOWN",
				exception->ExceptionInformation[1]);
		}
		break;
	default:
		StringCbPrintf(buffer, size, L"Error Code : 0x%x", exception->ExceptionRecord->ExceptionCode);
		break;
	}
}

LONG WINAPI TerminateHandler(LPEXCEPTION_POINTERS exception)
{
	WCHAR buffer[4096];
	WCHAR dllPathBuf[4096];
	WCHAR excepData[1024];
	HMODULE errMod = NULL;
	uint32_t errOffset = 0;
	if (
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPWSTR)exception->ExceptionRecord->ExceptionAddress, &errMod))
	{
		GetModuleFileName(errMod, dllPathBuf, sizeof(dllPathBuf));
		auto name = wcsrchr(dllPathBuf, '\\') + 1;
		errOffset = (uint32_t)exception->ExceptionRecord->ExceptionAddress - (uint32_t)errMod;
		GenerateExceptionData(excepData, sizeof(excepData), exception->ExceptionRecord);
		StringCbPrintf(buffer, sizeof(buffer), L"Unhandled exception at %s+0x%x(0x%x)! %s", name, errOffset, exception->ExceptionRecord->ExceptionAddress, excepData);
	}
	else {
		StringCbPrintf(buffer, sizeof(buffer), L"Unhandled exception at 0x%x! Error Code :0x%x", exception->ExceptionRecord->ExceptionAddress, exception->ExceptionRecord->ExceptionCode);
	}
	MessageBox(NULL, buffer, L"Unhandled Exception", MB_OK | MB_ICONERROR);
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL APIENTRY DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
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
	if (dwReason == DLL_PROCESS_ATTACH) {
		DetourRestoreAfterWith();
		if (InstanceSettings::GetSettings() == NULL)
		{
			int nArgs = -1, instanceID = -1, i = 0;
			LPWSTR* szArglist;
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
						std::ostringstream msg;
						msg << "Unknown launch arg \"" << cmd << "\".";
						Logging::getInstance().error("START", msg.str());
						break;
					}
				}
				i++;
			}
			LocalFree(szArglist);
			InstanceSettings::InitSettings(instanceID);
		}

		// NB: DllMain can't call LoadLibrary, so we hook the app entry point.
		TrueEntryPoint = (int (WINAPI*)(VOID))DetourGetEntryPoint(NULL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueEntryPoint, entryPointWithDetours);
		if (DetourTransactionCommit() != NO_ERROR)
		{
			throw std::exception("Detour Error!");
		}
		return TRUE;
	}
}

