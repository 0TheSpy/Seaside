#pragma once

typedef const void(__thiscall* pShutdown)(void*, void*, const char*);
pShutdown oShutdown;
void __fastcall hkShutdown(void* thisptr, void* unk1, void* unk2, const char* reason) noexcept
{
#ifdef DEBUG
    printf("shutdown (%x) HOOKED %s\n", thisptr, reason);
#endif

    if (*g_Options.discmsg_active) {
#ifdef DEBUG
        printf("set new reason %s\n", g_Options.discmsg.value->mystring);
#endif
        char customreason[256] = "#";
        strcat_s( customreason, MakeControlChars(g_Options.discmsg.value->mystring));

        oShutdown(thisptr, nullptr, customreason);
    }
    else
        oShutdown(thisptr, nullptr, reason);
}

inline void HookNetchannel()
{
    DWORD ptrShutdown = *((DWORD*)iff.g_pEngineClient->GetNetChannelInfo()) + 36 * 4;
    DWORD addrShutdown = *(DWORD*)ptrShutdown; 
    oShutdown = (pShutdown)DetourFunction(
        (PBYTE)(addrShutdown),
        (PBYTE)hkShutdown);
#ifdef DEBUG
    printf("Detoured at %x\n", addrShutdown); 
#endif
    opt.netchannedlhooked = 1;
}

