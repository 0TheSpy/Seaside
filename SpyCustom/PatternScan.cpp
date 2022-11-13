#include "PatternScan.hpp"


DWORD WaitOnModuleHandle(std::string moduleName)
{
    DWORD ModuleHandle = NULL;
    while (!ModuleHandle)
    {
        ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
        if (!ModuleHandle)
            Sleep(50);
    }
    return ModuleHandle;
}

DWORD FindPatternV2_(std::string moduleName, std::string pattern)
{
    const char* pat = pattern.c_str();
    DWORD firstMatch = 0;
    DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
    MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
    DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
    for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
    {
        if (!*pat) {
            printfdbg("FP %s: Found %s at %x\n", moduleName.c_str(), pattern.c_str(), firstMatch);
            return firstMatch;
        }

        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
        {
            if (!firstMatch)
                firstMatch = pCur;

            if (!pat[2]) {
                printfdbg("FP %s: Found %s at %x\n", moduleName.c_str(), pattern.c_str(), firstMatch);
                return firstMatch;
            }

            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
                pat += 3;

            else
                pat += 2;     
        }
        else
        {
            pat = pattern.c_str();
            firstMatch = 0;
        }
    }
    printfdbg("FP %s: Nothing found! %s\n", moduleName.c_str(), pattern.c_str());
    return NULL;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
    for (; *szMask; ++szMask, ++Mask, ++Data)
    {
        if (*szMask == 'x' && *Mask != *Data)
        {
            return false;
        }
    }
    return (*szMask) == 0;
}

DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
    DWORD Address = WaitOnModuleHandle(moduleName.c_str());
    MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
    DWORD Length = ModInfo.SizeOfImage;
    for (DWORD c = 0; c < Length; c += 1)
    {
        if (bCompare((BYTE*)(Address + c), Mask, szMask))
        {
            return (DWORD)(Address + c);
        }
    }
    return 0;
}
