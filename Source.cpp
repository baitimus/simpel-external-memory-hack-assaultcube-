
     
#include <Windows.h>
#include<TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp
#include <vector>
#include "mem.h"

 

std::vector<DWORD> ammoPointers{ 0x8,0x77C,0x30,0x98,0x9C,0x30,0x2BC };
std::vector<DWORD> healtpointers{ 0x8,0x2F8,0x98,0x4E8 };


int main() {

    HWND hgamewindow = FindWindow(NULL, "AssaultCube");
    if (hgamewindow == NULL) {
        std::cout << "Start the game!" << std::endl;
        return 0;
    }
    DWORD pID = NULL; // ID of our Game
    GetWindowThreadProcessId(hgamewindow, &pID);
    HANDLE processHandle = NULL;
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) { // error handling
        std::cout << "Failed to open process" << std::endl;
        return 0;
    }

    char gameName[] = "ac_client.exe";
    DWORD gameBaseMemoryAddress = GetModuleBaseAddress(_T(gameName), pID);
    DWORD offsetGameToBaseAdress = 0x00183828;
    
    DWORD baseaddress = NULL;
    //Get value at gamebase+offset -> store it in baseAddress
    DWORD baseadress = ReadProcessMemory(processHandle, (LPVOID)(gameBaseMemoryAddress + offsetGameToBaseAdress), &baseaddress, sizeof(baseaddress), NULL);
    std::cout << "debugginfo: baseaddress = " << std::hex << baseaddress << std::endl;
    const int  baseaddressforever = baseaddress;

    //Calculate the dynamic addresses
    DWORD dynamicAmmoAddress = CalculateMemoryaddress(processHandle, ammoPointers, baseaddressforever);
    std::cout << "debugginfo: dynamicAmmoAddress = " << std::hex << dynamicAmmoAddress << std::endl;
    DWORD dynamicHealthAddress = CalculateMemoryaddress(processHandle, healtpointers, baseaddressforever);
    std::cout << "debugginfo: dynamicHealthAddress = " << std::hex << dynamicHealthAddress << std::endl;
    int newAmmo = 29;

    while (true)
    {
     Sleep(150);


        WriteProcessMemory(processHandle, (LPVOID)(dynamicAmmoAddress), &newAmmo, 4, 0);
    }   
               
               
          
         
           
}
