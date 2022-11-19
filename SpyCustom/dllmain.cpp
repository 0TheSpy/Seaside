#pragma once
#include <iostream>
#include "Interfaces.hpp"
#include "NetVarManager.hpp"
#include "VMT.hpp"
#include "EventListener.hpp"
#include <string>
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "Other.hpp"
#include "sdk/c_baseentity.h" 
#include "PatternScan.hpp"
#include "SkinChanger.hpp"
#include "Config.hpp"
#include "protobuffs.hpp"
using namespace std;
#include "proxies.hpp"
#include "Hooks.hpp"
#include "Menu.hpp"
#include "SkinChanger.hpp"
  
#include "resources.h"

void OnLevelInit()
{
    if (g_Options.skyboxactive)
    {
        iff.g_pCVar->FindVar("sv_skyname")->SetValue((char*)g_Options.skyboxtemp.value.get());
        iff.g_pCVar->FindVar("r_3dsky")->SetValue(false);
    }

    if (g_Options.fogactive)
    { 
        SetValueUnrestricted("fog_enable", 1);
        SetValueUnrestricted("fog_enableskybox", 1);
        SetValueUnrestricted("fog_override", 1);
         
        iff.g_pCVar->FindVar("fog_color")->SetValue(
            std::string("").
            append(std::to_string(g_Options.fogcolor.value->r * 255)).
            append(" ").
            append(std::to_string(g_Options.fogcolor.value->g * 255)).
            append(" ").
            append(std::to_string(g_Options.fogcolor.value->b * 255)).
            append(" ").c_str()
        );
        iff.g_pCVar->FindVar("fog_colorskybox")->SetValue(
            std::string("").
            append(std::to_string(g_Options.fogcolor.value->r * 255)).
            append(" ").
            append(std::to_string(g_Options.fogcolor.value->g * 255)).
            append(" ").
            append(std::to_string(g_Options.fogcolor.value->b * 255)).
            append(" ").c_str()
        ); 
        SetValueUnrestricted("fog_maxdensity", g_Options.fogdensity);
        SetValueUnrestricted("fog_maxdensityskybox", g_Options.fogdensity);
        SetValueUnrestricted("fog_start", g_Options.fogstart);
        SetValueUnrestricted("fog_startskybox", g_Options.fogstart);
        SetValueUnrestricted("fog_end", g_Options.fogend);
        SetValueUnrestricted("fog_endskybox", g_Options.fogend);
    }
    else iff.g_pCVar->FindVar("fog_override")->SetValue(false);

    iff.g_pCVar->FindVar("r_rainlength")->SetValue(*g_Options.rainlength);
    iff.g_pCVar->FindVar("r_rainspeed")->SetValue(*g_Options.rainspeed);
    iff.g_pCVar->FindVar("r_rainradius")->SetValue(*g_Options.rainradius);
    iff.g_pCVar->FindVar("cl_windspeed")->SetValue(*g_Options.windspeed);
    iff.g_pCVar->FindVar("r_rainwidth")->SetValue(*g_Options.rainwidth);
    iff.g_pCVar->FindVar("r_RainSideVel")->SetValue(*g_Options.rainsidevel);
    iff.g_pCVar->FindVar("r_rainalpha")->SetValue(*g_Options.rainalpha);
     
    SetValueUnrestricted("mat_force_tonemap_scale", g_Options.tonemapscale);

    SetValueUnrestricted("r_aspectratio", g_Options.aspectratio);
    iff.g_pCVar->FindVar("mat_postprocess_enable")->SetValue(!g_Options.postproc);
    iff.g_pCVar->FindVar("cl_csm_enabled")->SetValue(!g_Options.shadows);
    iff.g_pCVar->FindVar("cl_ragdoll_gravity")->SetValue(g_Options.ragdollgravity);
    SetValueUnrestricted("cl_phys_timescale", g_Options.ragdolltime); 
    SetValueUnrestricted("viewmodel_fov", g_Options.viewmodel_fov);
    SetValueUnrestricted("viewmodel_offset_x", g_Options.viewmodel_x);
    SetValueUnrestricted("viewmodel_offset_y", g_Options.viewmodel_y);
    SetValueUnrestricted("viewmodel_offset_z", g_Options.viewmodel_z);

    if (g_Options.viewmodel_moving)
    {
        SetValueUnrestricted("cl_viewmodel_shift_left_amt", 0);
        SetValueUnrestricted("cl_viewmodel_shift_right_amt", 0);
        SetValueUnrestricted("cl_bob_lower_amt", 0);
        SetValueUnrestricted("cl_bobamt_lat", 0);
        SetValueUnrestricted("cl_bobamt_vert", 0);
        SetValueUnrestricted("cl_wpn_sway_scale", 0);
    }

    SetValueUnrestricted("mat_fullbright", g_Options.fullbright);
    SetValueUnrestricted("mat_drawgray", g_Options.drawgray); 
    SetValueUnrestricted("mat_showlowresimage", g_Options.showlowresimage); 


    colorWorld();


}
 

void OnLoadCfg()
{
    for (int i = 0; i < 5; i++)
    {
        if (g_Options.customtextures.value->arr[i].texturelink) {
            g_Options.customtextures.value->arr[i].texturelink = CreateMaterial(
                string(g_Options.customtextures.value->arr[i].Name),
                string(g_Options.customtextures.value->arr[i].keyvalue)); 
            opt.dmeitems.at(i) = g_Options.customtextures.value->arr[i].Preview;
        }
    }

    OnLevelInit();

    const auto& paintkitMap = itemSystem()->getItemSchema()->paintKits;

    for (int n = 0; n < g_Options.weapons.value->weaponzcount; n++)
    {
        CPaintKit* pk = (CPaintKit*)paintkitMap.memory[g_Options.weapons.value->arr[n].skinidc].value;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].link = (DWORD)pk;
        pk->rarity = g_Options.weapons.value->arr[n].rarity;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].rarity =  g_Options.weapons.value->arr[n].rarity;
        pk->pearlescent = g_Options.weapons.value->arr[n].pearl;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].pearl =  g_Options.weapons.value->arr[n].pearl;
        pk->color1 = g_Options.weapons.value->arr[n].color1;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].color1 = g_Options.weapons.value->arr[n].color1;
        pk->color2 = g_Options.weapons.value->arr[n].color2;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].color2 = g_Options.weapons.value->arr[n].color2;
        pk->color3 = g_Options.weapons.value->arr[n].color3;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].color3 = g_Options.weapons.value->arr[n].color3;
        pk->color4 = g_Options.weapons.value->arr[n].color4;
        opt.sc_skins[g_Options.weapons.value->arr[n].skinidc].color4 = g_Options.weapons.value->arr[n].color4;

    }
     
    Sleep(100);
    iff.g_pClientState->ForceFullUpdate();
    Sleep(100);
    opt.needupdate = 1;
    Sleep(100);
    ProtoFeatures.SendClientHello();
    Sleep(100);
    ProtoFeatures.SendMatchmakingClient2GCHello();
    Sleep(100); 

    opt.loading = 0;
}
 


DWORD WINAPI HackThread(HMODULE hModule)
{

    while (!GetModuleHandleA("serverbrowser.dll"))
        Sleep(1000);

#ifdef DEBUG
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printfdbg("Cheat launched\n");
#endif
            
    iff.Init();

    opt.hModuleGlobal = hModule;

    NetvarSys::Get().Initialize();
      
    void* ptrDevMsg = GetProcAddress(GetModuleHandleA("tier0.dll"), "?DevMsg@@YAXPBDZZ");
    printfdbg("DevMsg %x\n", ptrDevMsg);

    if (ptrDevMsg)
        oDevMsg = (pDevMsg)DetourFunction(
            (PBYTE)(ptrDevMsg),
            (PBYTE)hkDevMsg);

    void* ptrDevWarningMsg = GetProcAddress(GetModuleHandleA("tier0.dll"), "?DevWarning@@YAXPBDZZ");
    printfdbg("DevWarningMsg %x\n", ptrDevWarningMsg);

    if (ptrDevWarningMsg)
        oDevWarningMsg = (pDevWarningMsg)DetourFunction(
            (PBYTE)(ptrDevWarningMsg),
            (PBYTE)hkDevWarningMsg);
     
    DMEHook = new VMTHook(iff.g_pMdlRender);
    DMEHook->SwapPointer(21, reinterpret_cast<void*>(DrawModelExecute));
    DMEHook->ApplyNewTable();
    
    D3DHook = new VMTHook(iff.g_pD3DDevice9);
    D3DHook->SwapPointer(42, reinterpret_cast<void*>(hkEndScene));
    D3DHook->ApplyNewTable();

    GameEventManagerHook = new VMTHook(iff.g_pGameEvents);
    GameEventManagerHook->SwapPointer(9, reinterpret_cast<void*>(hkFireEventClientSide));
    GameEventManagerHook->ApplyNewTable();
     
    ProtoHook = new VMTHook(iff.g_SteamGameCoordinator);
    ProtoHook->SwapPointer(0, reinterpret_cast<void*>(hkGCSendMessage));
    ProtoHook->SwapPointer(2, reinterpret_cast<void*>(hkGCRetrieveMessage));
    ProtoHook->ApplyNewTable();

    CacheHook = new VMTHook(iff.g_pMdlCache);
    CacheHook->SwapPointer(10, reinterpret_cast<void*>(hkFindMDL));
    CacheHook->ApplyNewTable();

    SoundHook = new VMTHook(iff.g_pEngineSound);
    SoundHook->SwapPointer(5, reinterpret_cast<void*>(hkEmitSound1));
    SoundHook->ApplyNewTable();

    
    VGUISurfHook = new VMTHook(iff.g_pVGuiSurface);
    VGUISurfHook->SwapPointer(67, reinterpret_cast<void*>(hkLockCursor));
    VGUISurfHook->ApplyNewTable();
    
    ClientModeHook = new VMTHook(iff.g_ClientMode);
    ClientModeHook->SwapPointer(44, reinterpret_cast<void*>(hkdoPostScreenEffects));
    ClientModeHook->SwapPointer(18, reinterpret_cast<void*>(hkOverrideView));
    ClientModeHook->SwapPointer(24, reinterpret_cast<void*>(hkCreateMove));
    ClientModeHook->ApplyNewTable();
     
    FileSystemHook = new VMTHook(iff.g_pFullFileSystem);
    FileSystemHook->SwapPointer(101, reinterpret_cast<void*>(hkGetUnverifiedFileHashes));
    FileSystemHook->ApplyNewTable();

    InitSkinChanger();

    oGetAccountData = (pGetAccountData)DetourFunction(
        (PBYTE)(iff.fn_get_account_data),
        (PBYTE)hkGetAccountData);

    oParticleCollectionSimulate = (pParticleCollectionSimulate)DetourFunction(
        (PBYTE)(iff.ParticleCollectionSimulateAdr),
        (PBYTE)hkParticleCollectionSimulate);

    Sleep(1000);
    
    ClientHook = new VMTHook(iff.g_pClient);
    ClientHook->SwapPointer(37, reinterpret_cast<void*>(hkFrameStageNotify));
    ClientHook->SwapPointer(38, reinterpret_cast<void*>(hkDispatchUserMessage));  
    ClientHook->ApplyNewTable();
     
    iff.g_pGameConsole->Clear();

	EventListener* eventListener = new EventListener();

    Color color = { 255,255,0,255 }; 
    iff.g_pCVar->ConsoleColorPrintf(color, "Seaside loaded!\n"); 
     
    ConVar* sv_skyname = iff.g_pCVar->FindVar("sv_skyname");
      
    int proxyindex = 0;
    for (ClientClass* pClass = iff.g_pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
            RecvTable* pClassTable = pClass->m_pRecvTable;
            for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
                RecvProp* pProp = &pClassTable->m_pProps[nIndex];
                if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
                    continue;
                proxyindex = nIndex;
                fnSequenceProxyFn = pProp->m_ProxyFn;
                pProp->m_ProxyFn = (RecvVarProxyFn)SetViewModelSequence;
                break;
            }
            break;
        }
    }
      
    Config::Get().Refresh();

    ifstream loadcfg("seaside_autoload");
    if (loadcfg.is_open())
    {
        printfdbg("autoload found\n");

        string line; 
        getline(loadcfg, line);
         
        printfdbg("Trying to load %s CFG \n", line.c_str());


        for (int i = 0; i < Config::Get().configs.size(); i++) 
            if (Config::Get().configs[i] == line)
            {
                printfdbg("Loading %s CFG \n", line.c_str());
                opt.autoconfig = i;
            }
            
        if (opt.autoconfig == -1)
            {
            printfdbg("Cannot find %s CFG \n", line.c_str());
            }
        else
        {
            Sleep(1000);
            Config::Get().Load(opt.autoconfig);
        }
        loadcfg.close();
    }
    else
    {
        printfdbg("Autoload NOT found, creating material\n");
        g_Options.customtextures.value->arr[0].texturelink = CreateMaterial(
            string(g_Options.customtextures.value->arr[0].Name),
            string(g_Options.customtextures.value->arr[0].keyvalue));
    }
     
    
    ofstream loadcod("csgo/sound/hitsound_cod.wav", std::ios::binary);
    if (loadcod) { 
        loadcod.write((char*)&hitsound_cod[0], sizeof(hitsound_cod));
        loadcod.close();
    }
    ofstream loadcrit("csgo/sound/hitsound_crit.wav", std::ios::binary);
    if (loadcrit) { 
        loadcrit.write((char*)&hitsound_crit[0], sizeof(hitsound_crit));
        loadcrit.close();
    }
    
      
    void* fn_getplmoney = (void*)FindPatternV2("client.dll", "55 8B EC 56 8B 75 08 83 FE 3F");
    if (fn_getplmoney)
        oGetPlayerMoney = (pGetPlayerMoney)DetourFunction(
            (PBYTE)(fn_getplmoney),
            (PBYTE)hkGetPlayerMoney);
     
    while (!opt.unhook)
    { 
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            opt.show = !opt.show;
            printfdbg("Show menu: %d\n", opt.show);
            if (!opt.show)
                iff.g_pInputSystem->EnableInput(1);
            else
                iff.g_pInputSystem->EnableInput(0);
        }

        if (!opt.netchannedlhooked && iff.g_pEngineClient->IsConnected())
            HookNetchannel();

        if (iff.g_pEngineClient->IsInGame()) {
            if (!*g_Options.dme_gettextures)
            {
                IMaterial* mats[1];
                for (int i = 5; i < g_Options.materials.value->itemcount; i++) {
                    int mdlindex = iff.g_pMdlInfo->GetModelIndex(g_Options.models.value->arr[i - 2].vmodel_orig);
                    if (mdlindex != -1) {
                        model_t* pModel = (model_t*)iff.g_pMdlInfo->GetModel(mdlindex);
                        iff.g_pMdlInfo->GetModelMaterials(pModel, 1, mats);
                        const char* matname = mats[0]->GetName();
                        if (g_Options.materials.value->arr[i].texture[0] == 0x0)
                            strcpy(g_Options.materials.value->arr[i].texture, matname);
                        if (g_Options.materials.value->arr[i].texture_temp[0] == 0x0)
                            strcpy(g_Options.materials.value->arr[i].texture_temp, matname);
                    }
                }
                printfdbg("Model materials dumped\n");
                *g_Options.dme_gettextures = true;
            }



        }
        else
        {
            opt.disconnected = 1;
        }



        Sleep(20); 
    }
    opt.show = false;

    
    for (ClientClass* pClass = iff.g_pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
            RecvTable* pClassTable = pClass->m_pRecvTable;
            RecvProp* pProp = &pClassTable->m_pProps[proxyindex];
            pProp->m_ProxyFn = fnSequenceProxyFn;
        }
    }
    

    if (iff.g_pEngineClient->IsConnected()) {
        iff.g_pClientState->ForceFullUpdate();
    }

    iff.g_pInputSystem->EnableInput(1);

    iff.g_pCVar->FindVar("r_3dsky")->SetValue(true);

    sv_skyname->m_nFlags &= FCVAR_CHEAT;
    sv_skyname->SetValue("sky_urb01");

    iff.g_pCVar->FindVar("fog_override")->SetValue(0);
    iff.g_pCVar->FindVar("mat_force_tonemap_scale")->SetValue(0.0f);
    
    SetValueUnrestricted("developer", 0);
    SetValueUnrestricted("sv_show_usermessage", 0);

    ResetMisc();
     
    DMEHook->RestoreOldTable(); 
    D3DHook->RestoreOldTable();
    ClientHook->RestoreOldTable();
    GameEventManagerHook->RestoreOldTable();
    ProtoHook->RestoreOldTable();
    CacheHook->RestoreOldTable();
    SoundHook->RestoreOldTable();
    VGUISurfHook->RestoreOldTable();
    ClientModeHook->RestoreOldTable();
    FileSystemHook->RestoreOldTable();
    if (opt.netchannedlhooked) {
        DetourRemove(reinterpret_cast<BYTE*>(oShutdown), reinterpret_cast<BYTE*>(hkShutdown));
        DetourRemove(reinterpret_cast<BYTE*>(oSendNetMsg), reinterpret_cast<BYTE*>(hkSendNetMsg));
    }

    DetourRemove(reinterpret_cast<BYTE*>(oGetAccountData), reinterpret_cast<BYTE*>(hkGetAccountData));
    DetourRemove(reinterpret_cast<BYTE*>(oParticleCollectionSimulate), reinterpret_cast<BYTE*>(hkParticleCollectionSimulate));
      
    if (ptrDevMsg)
        DetourRemove(reinterpret_cast<BYTE*>(oDevMsg), reinterpret_cast<BYTE*>(hkDevMsg));
    if (ptrDevWarningMsg)
        DetourRemove(reinterpret_cast<BYTE*>(oDevWarningMsg), reinterpret_cast<BYTE*>(hkDevWarningMsg));
     
    if (fn_getplmoney)
        DetourRemove(reinterpret_cast<BYTE*>(oGetPlayerMoney), reinterpret_cast<BYTE*>(hkGetPlayerMoney));
         
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)oWndProc);

	delete eventListener;

    ProtoFeatures.SendClientHello();
    ProtoFeatures.SendMatchmakingClient2GCHello();
    ProtoFeatures.SendClientGcRankUpdate();


    Sleep(1000);
    
#ifdef DEBUG
    if (f) fclose(f);
    FreeConsole();
#endif
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hdl = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
        if (hdl) CloseHandle(hdl);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

