// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "dllmain.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        
        while (!(G::Window = FindWindowA(("Valve001"), NULL))) Sleep(200);
        Hooks::oldWindowProc = (WNDPROC)SetWindowLongPtr(G::Window, GWL_WNDPROC, (LONG_PTR)Hooks::WndProc);
        boost::thread t_cheat(U::init);
    }
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        U::uninstall_hooks();
    }
    return TRUE;
}

