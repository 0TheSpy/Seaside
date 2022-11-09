#include "Interfaces.hpp"
#include "PatternScan.hpp"
#include "Options.hpp"

IF iff;

void* IF::GetInterface(const char* dllname, const char* interfacename)
{
    tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(dllname), "CreateInterface");
    int returnCode = 0;
    void* ointerface = CreateInterface(interfacename, &returnCode);
#ifdef DEBUG
    printf("%s = %x\n", interfacename, ointerface);
#endif
    return ointerface;
}

PVOID FindHudElement(const char* name)
{
    void* pointer1 = (void*)(FindPatternV2("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
    static void* pThis = *reinterpret_cast<DWORD**>(pointer1);
    void* pointer2 = (void*)(FindPatternV2("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
    static auto find_hud_element
        = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(
            pointer2
            );
    return (void*)find_hud_element(pThis, name);
}


#define VTBL(classptr) (*(DWORD*)classptr)
#define PVFN_(classptr, offset) (VTBL(classptr) + offset)
#define VFN_(classptr, offset) *(DWORD*)PVFN_(classptr, offset)
#define VFN(classptr, offset) VFN_(classptr, (offset * sizeof(void*)))


void IF::Init()
{ 
    g_pEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
    g_pGameUI = (CGameUI*)GetInterface("client.dll", "GameUI011");
    g_pVGuiPanel = (vgui::IPanel*)GetInterface("vgui2.dll", "VGUI_Panel009");
    g_pGameEvents = (CGameEventManager*)GetInterface("engine.dll", "GAMEEVENTSMANAGER002");
    g_pEngineClient = (IVEngineClient*)GetInterface("engine.dll", "VEngineClient014");
    g_pEnginetrace = (IEngineTrace*)GetInterface("engine.dll", "EngineTraceClient004");
    g_pEnginevgui = (IEngineVGui*)GetInterface("engine.dll", "VEngineVGui001");
    g_pVGui = (vgui::IVGui*)GetInterface("vgui2.dll", "VGUI_ivgui008");
    g_pClient = (IBaseClientDLL*)GetInterface("client.dll", "VClient018");
    g_pMdlInfo = (IVModelInfoClient*)GetInterface("engine.dll", "VModelInfoClient004");
    g_pMdlRender = (IVModelRender*)GetInterface("engine.dll", "VEngineModel016");
    g_pMaterialSystem = (IMaterialSystem*)GetInterface("materialsystem.dll", "VMaterialSystem080");
    g_pCVar = (ICvar*)GetInterface("vstdlib.dll", "VEngineCvar007");
    g_pVGuiSystem = (vgui::ISystem*)GetInterface("vgui2.dll", "VGUI_System010");
    g_pVGuiSchemeManager = (vgui::ISchemeManager*)GetInterface("vgui2.dll", "VGUI_Scheme010");
    g_pVGuiInput = (vgui::IInput*)GetInterface("vgui2.dll", "VGUI_Input005");
    g_pFullFileSystem = (IFileSystem*)GetInterface("filesystem_stdio.dll", "VFileSystem017");
    g_pGameConsole = (IGameConsole*)GetInterface("client.dll", "GameConsole004");
    g_pEfx = (IVEfx*)GetInterface("engine.dll", "VEngineEffects001"); 
    g_pPhysProps = (IPhysicsSurfaceProps*)GetInterface("vphysics.dll", "VPhysicsSurfaceProps001");
    g_pRenderView = (IVRenderView*)GetInterface("engine.dll", "VEngineRenderView014");
    g_pDebugOverlay = (IVDebugOverlay*)GetInterface("engine.dll", "VDebugOverlay004");
    g_pEffects = (IEffects*)GetInterface("client.dll", "IEffects001");
    g_pStudioRender = (IStudioRender*)GetInterface("studiorender.dll", "VStudioRender026");

    typedef PVOID(__cdecl* oKeyValuesSystem)();
    oKeyValuesSystem pkeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");
    keyValuesSystem = (IKeyValuesSystem*)pkeyValuesSystem();

    printfdbg("KeyValuesSystem = %x\n", keyValuesSystem);

    myConMsg = (CONMSGPROC)GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConMsg@@YAXPBDZZ");
    myConColorMsg = (CONCOLORMSGPROC)GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");

    g_pGlobals = **(CGlobalVarsBase***)(FindPatternV2("client.dll", "A1 ? ? ? ? 5E 8B 40 10") + 1);
    g_pInput = *(CInput**)(FindPatternV2("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
    g_pViewRenderBeams = *(IViewRenderBeams**)(FindPatternV2("client.dll", "B9 ?? ?? ?? ?? A1 ?? ?? ?? ?? FF 10 A1 ?? ?? ?? ?? B9") + 0x1);

    printfdbg("g_pGlobals = %x\n", g_pGlobals);
    printfdbg("g_pInput = %x\n", g_pInput);
    printfdbg("g_pViewRenderBeams = %x\n", g_pViewRenderBeams);

    FX_Tesla = (FX_TeslaFn)FindPatternV2("client.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 8B 47 18");

    printfdbg("FX_TeslaFn %x\n", FX_Tesla);

    BloodSprayFn = (FX_BloodSprayFn)FindPatternV2("client.dll", "55 8B EC 8B 4D 08 F3 0F 10 51 ? 8D");

    printfdbg("FX_BloodSprayFn %x\n", BloodSprayFn);

    DispatchEffect = (FXDispatchEffect)FindPatternV2("client.dll", "55 8B EC 83 E4 F8 83 EC 20 56 57 8B F9 C7 44 24");

    printfdbg("DispatchEffect %x\n", DispatchEffect);

    g_pChatElement = (CHudChat*)FindHudElement("CHudChat");

    printfdbg("Chat element %x\n", g_pChatElement);

    LoadFromFile = (void*)FindPatternV2("client.dll", "55 8B EC 83 EC 0C 53 8B 5D 08 56 8B F1 3B"); 

    printfdbg("LoadFromFile %x\n", LoadFromFile);

    g_pD3DDevice9 = **(IDirect3DDevice9***)(FindPatternV2("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

    printfdbg("g_pD3DDevice9 %x\n", g_pD3DDevice9);

    g_pMemAlloc = *(IMemAlloc**)(GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc"));

    printfdbg("g_pMemAlloc %x\n", g_pMemAlloc);

    g_pClientState = **reinterpret_cast<CClientState***>(getvfunc<uintptr_t>(g_pEngineClient, 12) + 0x10); 

    printfdbg("g_pClientState %x\n", g_pClientState);

    CHudElement* g_pHudElement = (CHudElement*)FindHudElement("CCSGO_HudDeathNotice");  

    printfdbg("Hud element %x\n", g_pHudElement);

    auto SteamClient = ((ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamClient"))();
    g_SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface((void*)1, (void*)1, "SteamGameCoordinator001");
    g_SteamUser = SteamClient->GetISteamUser((void*)1, (void*)1, "SteamUser019");

    printfdbg("SteamClient %X\n", SteamClient);
    printfdbg("g_SteamGameCoordinator %X\n", g_SteamGameCoordinator);
    printfdbg("g_SteamUser %X\n", g_SteamUser);

    g_pClientLeafSystem = (IClientLeafSystem*)GetInterface("client.dll", "ClientLeafSystem002");

    printfdbg("g_pClientLeafSystem %x\n", g_pClientLeafSystem);

    g_pNetworkStringTableContainer = (INetworkStringTableContainer*)GetInterface("engine.dll", "VEngineClientStringTable001");

    printfdbg("g_pNetworkStringTableContainer %x\n", g_pNetworkStringTableContainer);

    getPlayerViewmodelArmConfigForPlayerModel = relativeToAbsolute<decltype(getPlayerViewmodelArmConfigForPlayerModel)>(FindPatternV2("client.dll", "E8 ? ? ? ? 89 87 ? ? ? ? 6A") + 1);

    g_pMdlCache = (IMDLCache*)GetInterface("datacache.dll", "MDLCache004");
    g_pEngineSound = (IEngineSound*)GetInterface("engine.dll", "IEngineSoundClient003");

    g_pClientShadowMgr = *(IClientShadowMgr**)(FindPatternV2("client.dll", "A1 ? ? ? ? FF 90 ? ? ? ? 6A 00 6A 00") + 1);

    printfdbg("g_pClientShadowMgr %x\n", g_pClientShadowMgr);

    g_ViewRender = *(CCSViewRender**)(FindPatternV2("client.dll", "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1); 

    printfdbg("g_ViewRender %x\n", g_ViewRender);

    g_ClientMode = **(IClientMode***)((*(DWORD**)g_pClient)[10] + 0x5);

    printfdbg("g_ClientMode %x\n", g_ClientMode);

    g_pInputSystem = (IInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion001");
    g_pVGuiSurface = (vgui::ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface031");  

    fn_get_account_data = relativeToAbsolute<decltype(fn_get_account_data)>(FindPatternV2("client.dll", "E8 ? ? ? ? 85 C0 74 EE") + 1);

    printfdbg("fn_get_account_data %x\n", fn_get_account_data);

    ParticleCollectionSimulateAdr = relativeToAbsolute<decltype(ParticleCollectionSimulateAdr)>(FindPatternV2("client.dll", "E8 ? ? ? ? 8B 0E 83 C1 10") + 1);

    printfdbg("ParticleCollectionSimulateAdr %x\n", ParticleCollectionSimulateAdr);

}


auto is_code_ptr(void* ptr) -> bool
{
    constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

    MEMORY_BASIC_INFORMATION out;
    VirtualQuery(ptr, &out, sizeof out);

    return out.Type
        && !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
        && out.Protect & protect_flags;
}

 
const char* GetVisibleValue(const char* cvar)
{
    PVOID addr = iff.g_pCVar->FindVar(cvar);
    if (!addr) return 0;
    char dummy[255];
    memcpy(dummy, *(PVOID*)PVOID(*(int*)&addr + 0x24), 255);
    return dummy;
}
 
float GetVisibleFloat(const char* cvar)
{
    return std::stof(GetVisibleValue(cvar));
}

 
void SetFloatUnrestricted(const char* cvar, float value)
{
    PVOID addr = iff.g_pCVar->FindVar(cvar);
    if (!addr)
    {
        printfdbg("Can't find cvar %s\n", cvar);

        PlaySoundA((char*)"null", opt.hModuleGlobal, SND_ASYNC);
        return;
    }
    float valX = value;
    int valXX = *(int*)&valX ^ (DWORD)addr;
    memcpy(PVOID(*(int*)&addr + 0x2C), &valXX, sizeof(valXX));
}
 
void SetIntUnrestricted(const char* cvar, int value)
{
    PVOID addr = iff.g_pCVar->FindVar(cvar);
    if (!addr)
    {
        printfdbg("Can't find cvar %s\n", cvar);

        PlaySoundA((char*)"null", opt.hModuleGlobal, SND_ASYNC);
        return;
    }
    int valXX = value ^ (DWORD)addr;
    memcpy(PVOID(*(int*)&addr + 0x30), &valXX, sizeof(valXX));
}

void SetValueUnrestricted(const char* cvar, float value)
{
    printfdbg("Set %s to %f\n", cvar, value);

    SetFloatUnrestricted(cvar, value);
    SetIntUnrestricted(cvar, (int)value);
}

void NET_SetConVar(const char* name, const char* value)
{
    //__asm pushad

    static void* pvSetConVar = NULL;

    if (!pvSetConVar)
        pvSetConVar = (void*)FindPatternV2(XorStr("engine.dll"), XorStr("8D 4C 24 1C E8 ? ? ? ? 56"));

    if (pvSetConVar)
        Invoke_NET_SetConVar(pvSetConVar, name, value);

    //__asm popad
}

void SetName(const char* pszName)
{
    NET_SetConVar(XorStr("name"), pszName);
}
