// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include "mem.h"
#include "proc.h"

std::string invincibilityStatus = "off", unlimited_ammoStatus = "off", armorStatus = "off", rapidFireStatus = "off";

void drawing() {
    std::cout << "**************************************************************************" << std::endl;
    std::cout << "				        Sauerbraten internal cheat            				" << std::endl;
    std::cout << "**************************************************************************" << std::endl;

    std::cout << "[NUMPAD1] Invicibility                  ->" << invincibilityStatus << std::endl;

    std::cout << "[NUMPAD2] Unlimited ammo                ->" << unlimited_ammoStatus << std::endl;

    std::cout << "[NUMPAD3] Rapid fire                    ->" << rapidFireStatus << std::endl;

    std::cout << "[NUMPAD4] Unlimited armor               ->" << armorStatus << std::endl;

    std::cout << "[END] Exit" << std::endl;

}

void update() {
    system("cls");
    drawing();
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Successfully injected. Ez\n";

    Sleep(2000);

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"sauerbraten.exe");

    uintptr_t localEntityPtr = moduleBase + 0x3472E0; //can find health and armor in here
    uintptr_t localPlayerPtr = moduleBase + 0x2A5730; // can find weapon and pos/angles info in here

    bool bHealth = false, bAmmo = false, bRecoil = false, bRapideFire = false, bArmor = false;

    update();

    while (true) //infinite loop
    {
        if (GetAsyncKeyState(VK_END) & 1)   //Security
        {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
            invincibilityStatus = "on";
            update();
            if (bHealth == false)
            {
                invincibilityStatus = "off";
                update();
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
            unlimited_ammoStatus = "on";
            update();
            if (bAmmo == false)
            {
                unlimited_ammoStatus = "off";
                update();
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRapideFire = !bRapideFire;
            rapidFireStatus = "on";
            update();
            if (bRapideFire == false)
            {
                rapidFireStatus = "off";
                update();
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            bArmor = !bArmor;
            armorStatus = "on";
            update();
            if (bArmor == false)
            {
                armorStatus = "off";
                update();
            }
        }

        if (localPlayerPtr) // to prevent writing addresses that doesn't exist yet
        {
            if (bHealth)
            {
                *(int*)mem::FindDMAAddy(localEntityPtr, { 0x0, 0x340 }) = 1337;
            }

            if (bArmor)
            {
                *(int*)mem::FindDMAAddy(localEntityPtr, { 0x0, 0x348 }) = 1337;
            }

            if (bRapideFire)
            {
                *(int*)mem::FindDMAAddy(localPlayerPtr, { 0x190 }) = 0;
            }

            if (bAmmo)
            {
                *(int*)mem::FindDMAAddy(localPlayerPtr, { 0x1AC }) = 1337;
                *(int*)mem::FindDMAAddy(localPlayerPtr, { 0x1A0 }) = 1337;
                *(int*)mem::FindDMAAddy(localPlayerPtr, { 0x1A4 }) = 1337;
                *(int*)mem::FindDMAAddy(localPlayerPtr, { 0x198 }) = 1337;
                *(int*)mem::FindDMAAddy(localPlayerPtr, { 0x1A8 }) = 1337;
            }
        }
    }

    fclose(f);                              //
    FreeConsole();                          //Security to avoid crashing the game when closing the windows
    FreeLibraryAndExitThread(hModule, 0);   //
    return 0;                               //
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

