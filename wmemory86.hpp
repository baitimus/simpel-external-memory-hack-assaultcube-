#ifndef WMEMORY86_HPP
#define WMEMORY86_HPP

#include <windows.h>
#include <tlhelp32.h>

typedef signed __int32 sint32;
typedef signed __int16 sint16;
typedef signed __int8 sint8;

typedef sint32* sptr32;
typedef sint16* sptr16;
typedef sint8* sptr8;

typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;

typedef uint32* uptr32;
typedef uint16* uptr16;
typedef uint8* uptr8;

#define adr32 const uint32

class Wmemory86
{
public:
    Wmemory86(const wchar_t* name)
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32W process = { sizeof(process) };

            if (Process32FirstW(snapshot, &process))
            {
                do
                {
                    if (_wcsicmp(process.szExeFile, name) == 0)
                    {
                        processId = process.th32ProcessID;

                        break;
                    }
                } while (Process32NextW(snapshot, &process));
            }

            CloseHandle(snapshot);

            if (processId != 0)
                processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        }
    }

    uint32 GetModuleBase(const wchar_t* name)
    {
        uint32 base = NULL;

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (snapshot == INVALID_HANDLE_VALUE)
            return 0;

        MODULEENTRY32W module = { sizeof(module) };

        if (Module32FirstW(snapshot, &module))
        {
            do
            {
                if (_wcsicmp(module.szModule, name) == 0)
                {
                    base = reinterpret_cast<uint32>(module.modBaseAddr);

                    break;
                }
            } while (Module32NextW(snapshot, &module));
        }

        CloseHandle(snapshot);

        return base;
    }

    template <typename T>
    T ReadMemory(uint32 address)
    {
        T value;
        ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), NULL);

        return value;
    }

    template <typename T>
    void WriteMemory(uint32 address, const T &value)
    {
        WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), NULL);
    }

private:
    uint32 processId = 0;

    HANDLE processHandle = nullptr;
};

#endif
