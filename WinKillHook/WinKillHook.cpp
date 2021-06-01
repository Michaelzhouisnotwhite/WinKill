#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x401

#include <Windows.h>
#include <WinUser.h>

#include "WinKillHook.h"

HHOOK hook = nullptr;
HWND hwnd = nullptr;
HINSTANCE instance = nullptr;

BOOL APIENTRY DllMain(HANDLE module, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		instance = static_cast<HINSTANCE>(module);
		break;

	case DLL_PROCESS_DETACH:
		winkill_remove_hook();
		break;
	}

	return TRUE;
}

WINKILLHOOK_API LRESULT CALLBACK keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		SHORT nkeyStateLCtrl = GetKeyState(VK_LCONTROL);
		SHORT nkeyStateShift = GetKeyState(VK_SHIFT);
		SHORT nkeyLAlt = GetKeyState(VK_LMENU);
		switch (wParam)
		{
		case WM_KEYDOWN:
			{
				if (true)
				{
					
				}
				break;
			}
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			{
				DWORD keyCode = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam)->vkCode;

				if ((keyCode == VK_LWIN) || (keyCode == VK_RWIN))
				{
					return 1;
				}
			}
			break;
		}
	}

	return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

WINKILLHOOK_API bool winkill_install_hook(HWND owner)
{
	if ((!hook) && owner)
	{
		hwnd = owner;
		hook = ::SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, instance, NULL);
	}

	return (hook != nullptr);
}

WINKILLHOOK_API bool winkill_remove_hook()
{
	if ((hook) && (UnhookWindowsHookEx(hook)))
	{
		hook = nullptr;
		hwnd = nullptr;
	}

	return (hook == nullptr);
}
