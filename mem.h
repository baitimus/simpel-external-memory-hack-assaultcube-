#include <Windows.h>
#include<TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp
#include <vector>#pragma once


DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}
DWORD CalculateMemoryaddress(HANDLE processhandle, const std::vector<DWORD>& offsets, DWORD baseaddress)
{
    for (int i = 0; i < offsets.size() - 1; i++) // -1 because we dont want the value at the last offset
    {
        ReadProcessMemory(processhandle, (LPVOID)(baseaddress + offsets.at(i)), &baseaddress, sizeof(baseaddress), NULL);



    }
    baseaddress += offsets.at(offsets.size() - 1); //Add Last offset -> done!!


    return baseaddress;


}