#include "stdafx.h"
#include "User32Detours.h"
#include "TestScriptRunner.h"
#include "WinSockDetours.h"
#include "GameDetours.h"

PVOID oCreateWinExA;
PVOID oPeekMessageA;
PVOID oSetWinHookExA;
PVOID oCallNextHookEx;
PVOID kbHook;

LPWSTR controllerGuid;

TestScriptRunner* script = NULL;
BOOL runScript = FALSE;
BOOL markKeyDown = FALSE;

BOOL isInjectedHook = FALSE;
BOOL xboxRJoystickXCentered = TRUE;
BOOL xboxRJoystickYCentered = TRUE;
BOOL disableJoyInput = FALSE;

std::list<UINT> lastEmulatedPresses;

void EmulateKeyPress(DWORD key)
{
	isInjectedHook = TRUE;
	lastEmulatedPresses.push_back(key);
	LPARAM lparam = 0x00000001 | (LPARAM)(MapVirtualKey(key, MAPVK_VK_TO_VSC) << 16);
	((KBHookProc)kbHook)(HC_ACTION, key, lparam);
}

void EmulateKeyRelease(DWORD key)
{
	/*isInjectedHook = TRUE;
	LPARAM lparam = 0x00000002 | (LPARAM)(MapVirtualKey(key, MAPVK_VK_TO_VSC) << 16);
	((KBHookProc)kbHook)(HC_ACTION, key, lparam);*/
}

void setupScript()
{
	script = new TestScriptRunner(L"testScript.txt", EmulateKeyPress);
	runScript = TRUE;
}

BOOL WINAPI MyPeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	SET_InstanceSettings instSet = InstanceSettings::GetSettings()->GetLocalSettings();
	BOOL ret = ((PeekMessageT)oPeekMessageA)(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
	if (ret && WM_KEYDOWN == lpMsg->message && VK_F2 == lpMsg->wParam)
	{
		markKeyDown = TRUE;
		Logging::getInstance().info("SCRIPTING", std::string("MARK"));
	}
	else
	{
		markKeyDown = FALSE;
	}

	if (InstanceSettings::GetSettings()->UseInputEmulation())
	{
		if (!ret)
		{
			if (!lastEmulatedPresses.empty())
			{
				for (std::list<UINT>::iterator key = lastEmulatedPresses.begin(); key != lastEmulatedPresses.end(); ++key)
				{
					EmulateKeyRelease(*key);
				}

				lastEmulatedPresses.clear();
			}

			if (runScript)
			{
				DWORD r = script->RunOnce();
				switch (r)
				{
				case  SR_SUCCESS:
				case SR_WAIT:
					break;
				case SR_EOF:
					runScript = false;
					break;
				case SR_ERROR:
				default:
					std::ostringstream msg;
					msg << "Unknown script error : " << r;
					Logging::getInstance().error("SCRIPTING", msg.str());
					runScript = false;
					break;
				}
			}

			if (instSet.directInput != NULL)
			{
				CDIDevice8Hook* dev = instSet.directInput->GetDevice();
				if (dev != NULL)
				{
					if (dev->IsButtonPress(XBOXBT_BACK))
					{
						disableJoyInput = !disableJoyInput;
					}

					if (!disableJoyInput)
					{
						if (dev->IsButtonPress(XBOXBT_A))
						{
							EmulateKeyPress(VK_RETURN);
						}
						else if (dev->IsButtonPress(XBOXBT_B))
						{
							EmulateKeyPress(VK_ESCAPE);
						}
						int upDown, rightLeft;
						dev->GetStickDirection(&upDown, &rightLeft);
						if (rightLeft == 1 && xboxRJoystickXCentered)
						{
							EmulateKeyPress(VK_RIGHT);
							xboxRJoystickXCentered = false;
						}
						else if (rightLeft == -1 && xboxRJoystickXCentered)
						{
							EmulateKeyPress(VK_LEFT);
							xboxRJoystickXCentered = false;
						}
						else if (rightLeft == 0)
						{
							xboxRJoystickXCentered = true;
						}

						if (upDown == 1 && xboxRJoystickYCentered)
						{
							EmulateKeyPress(VK_DOWN);
							xboxRJoystickYCentered = false;
						}
						else if (upDown == -1 && xboxRJoystickYCentered)
						{
							EmulateKeyPress(VK_UP);
							xboxRJoystickYCentered = false;
						}
						else if (upDown == 0)
						{
							xboxRJoystickYCentered = true;
						}
					}
				}
			}
		}
	}

	return ret;
}

LRESULT WINAPI MyCallNextHook(HHOOK hhk, int code, WPARAM wParam, LPARAM lParam)
{
	if (isInjectedHook)
	{
		isInjectedHook = FALSE;
		return S_OK;
	}
	else
	{
		return ((CallNextHook)oCallNextHookEx)(hhk, code, wParam, lParam);
	}
}

HHOOK WINAPI MySetWindowsHookExA(int idHook, HOOKPROC  lpfn, HINSTANCE hMod, DWORD dwThreadId)
{
	DWORD currentT = GetCurrentThreadId();
	kbHook = lpfn;
	HHOOK ret = ((SetWinHookExA)oSetWinHookExA)(idHook, lpfn, hMod, dwThreadId);
	return ret;
}

HWND WINAPI MyCreateWindowExA(
	DWORD dwExStyle,
	LPCSTR lpClassName,
	LPCSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam)
{
	SET_InstanceSettings instSet = InstanceSettings::GetSettings()->GetLocalSettings();
	if (FO2_AllowAttach2)
	{
		Logging::getInstance().debug("DEBUGGER", std::string("Waiting for 5 seconds."));
		Sleep(5000);
	}

	if (NULL != script && InstanceSettings::GetSettings()->IsHostInstance())
	{
		script->SetSetting(std::string("HOST"), 1);
	}
	HWND handle = ((CreateWinExA)oCreateWinExA)(
		dwExStyle,
		lpClassName,
		lpWindowName,
		dwStyle,
		instSet.windowPos.left,
		instSet.windowPos.top,
		instSet.windowPos.right,
		instSet.windowPos.bottom,
		hWndParent,
		hMenu,
		hInstance,
		lpParam);
	if (strcmp(lpClassName, "BDX9 Render Window") == 0)
	{
		InstanceSettings::GetSettings()->SetGameWindowHandle(handle);
	}

	return handle;
}

void User32Detour()
{
	SET_InstanceSettings instSet = InstanceSettings::GetSettings()->GetLocalSettings();
	if (InstanceSettings::GetSettings()->GetConsoleVerbosity() > Logging::Off)
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		FILE* con;
		freopen_s(&con, "CON", "w", stdout);
		std::ostringstream msg;
		msg << "Attached console for instance : " << InstanceSettings::GetSettings()->GetInstanceID() << " process id : " << GetCurrentProcessId();
		Logging::getInstance().debug("DEBUGGER", msg.str());
	}
	HMODULE user32DLL = LoadLibrary(L"User32.dll");
	oCreateWinExA = GetProcAddress(user32DLL, "CreateWindowExA");
	oPeekMessageA = GetProcAddress(user32DLL, "PeekMessageA");
	oSetWinHookExA = GetProcAddress(user32DLL, "SetWindowsHookExA");
	oCallNextHookEx = GetProcAddress(user32DLL, "CallNextHookEx");
	DetourAttach(&oCreateWinExA, MyCreateWindowExA);
	DetourAttach(&oPeekMessageA, MyPeekMessage);
	DetourAttach(&oSetWinHookExA, MySetWindowsHookExA);
	DetourAttach(&oCallNextHookEx, MyCallNextHook);
#ifdef _DEBUG
	setupScript();
#endif // DEBUG
}
