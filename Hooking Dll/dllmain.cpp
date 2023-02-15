#include <Windows.h>
#include <iostream>
#include "hook.h"

DWORD returnAddress;
void __declspec(naked) IncreasePlayerHealth()
{
	__asm
	{
		add eax, 0x64		
		mov ds:[0x00DDC000], eax
		jmp [returnAddress]
	}
}

DWORD WINAPI DllAttach(HMODULE Base) {
#ifdef _DEBUG
	AllocConsole();
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);	
#endif

	std::cout << "[+] Placing hook..." << "\n";

	int sizeOfHook = 8;
	DWORD hookAddress = 0x00DD22F6;
	returnAddress = hookAddress + sizeOfHook;
	Hook((void*)hookAddress, &IncreasePlayerHealth, sizeOfHook);

	std::cout << "[+] Hook inserted" << "\n";

	while (!(GetAsyncKeyState(VK_DELETE) & 1))
	{		
		Sleep(1);
	}
	FreeLibraryAndExitThread(Base, 0);

}

DWORD WINAPI DllDetach() {
#ifdef _DEBUG
	fclose((FILE*)stdin);
	fclose((FILE*)stdout);

	HWND hw_ConsoleHwnd = GetConsoleWindow();
	FreeConsole();
	PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0);
#endif

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DllAttach, hModule, NULL, NULL);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{

		DllDetach();
	}
	return TRUE;	
}

