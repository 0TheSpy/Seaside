#pragma once

#include "sdk/particles.h"
#include "ProtobuffMessages.h" 
#include "Aimbot.hpp"

VMTHook* SoundHook = nullptr;
void __fastcall hkEmitSound1(void* _this, int edx, IRecipientFilter& filter, int iEntIndex, int iChannel, char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk) {
    static auto ofunc = SoundHook->GetOriginal<decltype(&hkEmitSound1)>(5);

    if (*g_Options.soundhook_count) {
        soundlist vv = GetCfgWavpathBySoundHash(nSoundEntryHash);

        if (vv.cfgindex != -1)
        {
            Sounds ff = g_Options.models.value->arr[vv.cfgindex].sounds[vv.sndindex];
            return ofunc(iff.g_pEngineSound, edx, filter, iEntIndex, iChannel, pSoundEntry, -1, ff.wavpath, ff.volume, nSeed, flAttenuation, 0, ff.pitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);
        }
    }

    return ofunc(iff.g_pEngineSound, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);

}
 
VMTHook* DMEHook = nullptr;

void replacemat(int d)
{
    IMaterial* material;

    if (g_Options.materials.value->arr[d].customtextureselected != -1)
        if (g_Options.customtextures.value->arr[g_Options.materials.value->arr[d].customtextureselected].texturelink != nullptr)
            material = (IMaterial*)g_Options.customtextures.value->arr[g_Options.materials.value->arr[d].customtextureselected].texturelink;
        else material = iff.g_pMaterialSystem->FindMaterial("__err", TEXTURE_GROUP_MODEL);
    else
        material = iff.g_pMaterialSystem->FindMaterial(g_Options.materials.value->arr[d].texture, TEXTURE_GROUP_MODEL);
    material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Options.materials.value->arr[d].wireframe);
    material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.materials.value->arr[d].ignorez);
    material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.materials.value->arr[d].nodraw);
    material->ColorModulate(g_Options.materials.value->arr[d].coloralpha.x, g_Options.materials.value->arr[d].coloralpha.y, g_Options.materials.value->arr[d].coloralpha.z);
    material->AlphaModulate(g_Options.materials.value->arr[d].coloralpha.w);
    iff.g_pMdlRender->ForcedMaterialOverride(material);
}

void __stdcall DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
    static auto ofunc = DMEHook->GetOriginal<void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*)>(21);

    if (iff.g_pMdlRender->IsForcedMaterialOverride() || opt.loading || !*g_Options.dmeloop_count)
        return ofunc(iff.g_pMdlRender, ctx, state, pInfo, pCustomBoneToWorld);


    const char* szName = iff.g_pMdlInfo->GetModelName(pInfo.pModel);
      
    if (g_Options.materials.value->arr[3].active)
        if (strstr(szName, "player/legacy/t") != nullptr)
            replacemat(3);

    if (g_Options.materials.value->arr[4].active)
        if (strstr(szName, "player/legacy/ct") != nullptr)
            replacemat(4);

    if (strstr(szName, "weapons/v_")) {
        int namehash = fnv2::hashRuntime(szName);


        bool is_arm = strstr(szName, "arms") != nullptr;
        bool is_sleeve = strstr(szName, "sleeve") != nullptr;
        bool is_wep = !is_arm && !is_sleeve;


        for (int d = 0; d < g_Options.materials.value->itemcount; d++)
        {

            if (d == 1 && !is_arm) continue;
            if (d == 2 && !is_sleeve) continue;
            if (d == 0 && !is_wep) continue;

            if (d == 3 || d == 4) continue;
            if ((d > 2 && d != 5) && namehash != g_Options.materials.value->arr[d].model_hash) continue;

            if (d == 5 && !strstr(szName, "knife")) continue;

            if (g_Options.materials.value->arr[d].active) {
                replacemat(d);
            }
        }
    }  

    ofunc(iff.g_pMdlRender, ctx, state, pInfo, pCustomBoneToWorld);

    iff.g_pMdlRender->ForcedMaterialOverride(nullptr);
}

  
typedef const void(__thiscall* pParticleCollectionSimulate)(void*);
pParticleCollectionSimulate oParticleCollectionSimulate; 
void __fastcall hkParticleCollectionSimulate(CParticleCollection* thisPtr, void* edx)
{

    static auto original = reinterpret_cast<bool(__thiscall*)(CParticleCollection * thisPtr)>(oParticleCollectionSimulate);
    
    if (!g_Options.worldcoloractive || !iff.g_pEngineClient->IsConnected())
    {
        original(thisPtr);
        return;
    }
    
    original(thisPtr);

    CParticleCollection* root_colection = thisPtr;
    while (root_colection->m_pParent)
        root_colection = root_colection->m_pParent;

    const char* root_name = root_colection->m_pDef.m_pObject->m_Name.buffer;

    for (int i = 0; i < thisPtr->m_nActiveParticles; i++)
    {
        float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_TINT_RGB, i);
        pColor[0] = g_Options.smokeskycolor.value->r;
        pColor[4] = g_Options.smokeskycolor.value->g;
        pColor[8] = g_Options.smokeskycolor.value->b;
        float* pAlpha = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_ALPHA, i);
        *pAlpha = g_Options.smokeskycolor.value->a;
    }
     
}



VMTHook* CacheHook = nullptr;
MDLHandle_t __fastcall hkFindMDL(void* ecx, void* edx, char* FilePath)
{
    static auto oFindMDL = CacheHook->GetOriginal<MDLHandle_t(__thiscall*)(void*, char*)>(10);


    if (g_Options.models.value->arr[2].active && strstr(FilePath, "models/weapons/v_models/arms") && !strstr(FilePath, "gloves"))
    {
        printfdbg("Replacing %s with %s\n", FilePath, g_Options.models.value->arr[2].vmodel_repl);
        sprintf(FilePath, g_Options.models.value->arr[2].vmodel_repl);
    }

    if (g_Options.models.value->arr[3].active && strstr(FilePath, "models/weapons/v_knife"))
    { 
        printfdbg("Replacing %s with %s\n", FilePath, g_Options.models.value->arr[3].vmodel_repl);
        sprintf(FilePath, g_Options.models.value->arr[3].vmodel_repl);
    }

    for (int i = 4; i < g_Options.models.value->itemcount; i++)
    {
        if (g_Options.models.value->arr[i].active && g_Options.models.value->arr[i].findMDLmethode && strstr(FilePath, g_Options.models.value->arr[i].vmodel_orig))
        {
            printfdbg("Replacing %s with %s\n", FilePath, g_Options.models.value->arr[i].vmodel_repl);
            sprintf(FilePath, g_Options.models.value->arr[i].vmodel_repl);
        }
    }


    return oFindMDL(ecx, FilePath);
}

 

VMTHook* ClientModeHook = nullptr;
static void __stdcall hkdoPostScreenEffects(void* param) noexcept
{
    static auto ofunc = ClientModeHook->GetOriginal<void(__thiscall*)(IClientMode*, void*)>(44);

    int localplayer_index = iff.g_pEngineClient->GetLocalPlayer();
    C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(localplayer_index));

    if (*g_Options.entityloop_count) {
        if (iff.g_pEngineClient->IsInGame())
        {
            for (int i = iff.g_pEngineClient->GetMaxClients() + 1; i <= iff.g_pEntityList->GetHighestEntityIndex(); ++i)
            {
                C_BasePlayer* pEntity = (C_BasePlayer*)iff.g_pEntityList->GetClientEntity(i);

                if (!pEntity)
                    continue;

                bool bDormant = pEntity->IsDormant();

                if (bDormant)
                    continue;

                int cfgindex = GetCfgIndexByClassId(fnv2::hashRuntime(pEntity->GetClientClass()->GetName()));

                if (cfgindex != -1)
                {
                    if (g_Options.models.value->arr[cfgindex].active_scale)
                        pEntity->GetModelScale() = g_Options.models.value->arr[cfgindex].scale;

                    if (g_Options.models.value->arr[cfgindex].active_w)
                    {
                        char* model = g_Options.models.value->arr[cfgindex].wmodel_repl;
                        pEntity->SetModelIndex(iff.g_pMdlInfo->GetModelIndex(model));
                        if (const auto modelprecache = iff.g_pNetworkStringTableContainer->FindTable("modelprecache"))
                            modelprecache->AddString(false, model);
                    }
                }


            }
        }
    }


    for (int i = 0; i < iff.g_GlowObjectManager->m_GlowObjectDefinitions.Count(); i++)
    {
        CGlowObjectManager::GlowObjectDefinition_t& glow = iff.g_GlowObjectManager->m_GlowObjectDefinitions[i];

        if (glow.m_nNextFreeSlot != CGlowObjectManager::GlowObjectDefinition_t::ENTRY_IN_USE)
            continue;

        if (!glow.m_pEntity)
            continue;

        if (fnv2::hash(glow.m_pEntity->GetClientClass()->GetName()) == fnv2::hash("CCSPlayer"))
        {
            if (localplayer->GetTeam() != ((C_BasePlayer*)glow.m_pEntity)->GetTeam() && g_Options.glowObjects.value->arr[0].enabled)
            {
                memcpy(&glow.m_vGlowColor, &g_Options.glowObjects.value->arr[0].color, sizeof(float) * 4);
                glow.m_bRenderWhenOccluded = true;
                glow.m_bRenderWhenUnoccluded = false;
                glow.m_flGlowAlphaMax = g_Options.glowObjects.value->arr[0].alphamax;
                glow.m_nRenderStyle = g_Options.glowObjects.value->arr[0].glowstyle;
                glow.m_bFullBloomRender = false;
                glow.m_nFullBloomStencilTestValue = 0;
            }

            if (localplayer->GetTeam() == ((C_BasePlayer*)glow.m_pEntity)->GetTeam() && g_Options.glowObjects.value->arr[1].enabled)
            {
                memcpy(&glow.m_vGlowColor, &g_Options.glowObjects.value->arr[1].color, sizeof(float) * 4);
                glow.m_bRenderWhenOccluded = true;
                glow.m_bRenderWhenUnoccluded = false;
                glow.m_flGlowAlphaMax = g_Options.glowObjects.value->arr[1].alphamax;
                glow.m_nRenderStyle = g_Options.glowObjects.value->arr[1].glowstyle;
                glow.m_bFullBloomRender = false;
                glow.m_nFullBloomStencilTestValue = 0;
            } 
        }
    } 

    if (g_Options.flashalpha != 255.0f)
    {
        localplayer->GetFlashMaxAlpha() = g_Options.flashalpha;
    } 

    ofunc(iff.g_ClientMode, param);
}

void __stdcall hkOverrideView(CViewSetup* vsView) {

    static auto ofunc = ClientModeHook->GetOriginal<void(__stdcall*)(CViewSetup*)>(18);
     
    C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(iff.g_pEngineClient->GetLocalPlayer()));
    if (iff.g_pEngineClient->IsInGame() && localplayer)  
    {

        if (!localplayer->IsScoped())
            vsView->fov = g_Options.fov;

        auto ViewModel = reinterpret_cast<C_BaseViewModel*>(iff.g_pEntityList->GetClientEntityFromHandle(localplayer->GetViewModel()));
        if (ViewModel)
        {
            Vector eyeAng = vsView->angles; 
            eyeAng.x += g_Options.viewmodel_ang_x; 
            eyeAng.y += g_Options.viewmodel_ang_y;
            eyeAng.z += g_Options.viewmodel_ang_z;
            ViewModel->GetAbsAngles() = eyeAng; 
        }
    }
     
      
    ofunc(vsView);
}



VMTHook* VGUISurfHook = nullptr;
void __fastcall hkLockCursor(void* _this)
{
    static auto ofunc = VGUISurfHook->GetOriginal<decltype(&hkLockCursor)>(67);

    if (opt.show) {
        iff.g_pVGuiSurface->UnlockCursor();
        iff.g_pInputSystem->ResetInputState();
        return;
    }

    ofunc(iff.g_pVGuiSurface);
}


VMTHook* FileSystemHook = nullptr;

int __stdcall hkGetUnverifiedFileHashes(void* _this, void* someclass, int nMaxFiles)
{
    return 0;
}



enum class account_status_t : int
{
    none = 0,
    not_identifying,
    awaiting_cooldown,
    eligible,
    eligible_with_takeover,
    elevated,  
    account_cooldown
};

typedef const int(__thiscall* pGetAccountData)(void*);
pGetAccountData oGetAccountData;
int __fastcall hkGetAccountData(void* _this, void* edx) noexcept 
{
    const auto ret = oGetAccountData(_this);

    account_status_t& account_status = *reinterpret_cast<account_status_t*>(ret + 24);
    static const bool is_originally_prime_account = account_status == account_status_t::elevated;    
    if (!is_originally_prime_account)
    {
        account_status = g_Options.prime ? account_status_t::elevated : account_status_t::none;
    }

    return ret;
    
}


typedef const void(__thiscall* pShutdown)(void*, void*, const char*);
pShutdown oShutdown;
void __fastcall hkShutdown(void* thisptr, void* unk1, void* unk2, const char* reason) noexcept
{
    printfdbg("shutdown (%x) HOOKED %s\n", thisptr, reason);

    if (*g_Options.discmsg_active) {
        printfdbg("set new reason %s\n", g_Options.discmsg.value->mystring);
        char customreason[256] = "#";
        strcat_s(customreason, MakeControlChars(g_Options.discmsg.value->mystring));

        oShutdown(thisptr, nullptr, customreason);
    }
    else
        oShutdown(thisptr, nullptr, reason);
}


typedef const bool(__thiscall* pSendNetMsg)(void*, INetMessage*, bool, bool);
pSendNetMsg oSendNetMsg;
bool __fastcall hkSendNetMsg(void* channel, uint32_t, INetMessage* msg, bool reliable, bool voice)
{   
    if (*g_Options.debugstuff)
    {
        int type = msg->GetType(); 
        if (type != net_Tick && type != svc_SendTable)
            //printfdbg("NetMessage %s %s\n", msg->GetName(), msg->ToString());
            iff.myConMsg("[Seaside] NetMessage %s %s\n", msg->GetName(), msg->ToString());
    }

    if (*g_Options.changing_name && msg->GetType() == net_SetConVar)
    {  
        if (*g_Options.changing_name > 1) {
            *g_Options.changing_name = 0;
            printfdbg("Blocking net_SetConVar packet\n");
            return false;
        }

        *g_Options.changing_name += 1;
    }
     
    return oSendNetMsg(channel, msg, reliable, voice);
}

inline void HookNetchannel()
{
    //iff.g_pClientState->m_NetChannel
    DWORD ptrShutdown = *((DWORD*)iff.g_pEngineClient->GetNetChannelInfo()) + 36 * 4;
    DWORD addrShutdown = *(DWORD*)ptrShutdown;
    oShutdown = (pShutdown)DetourFunction(
        (PBYTE)(addrShutdown),
        (PBYTE)hkShutdown);
    printfdbg("Netchannel:Shutdown detoured at %x\n", addrShutdown);

    DWORD ptrSendNetMsg = *((DWORD*)iff.g_pEngineClient->GetNetChannelInfo()) + 40 * 4;
    DWORD addrSendNetMsg = *(DWORD*)ptrSendNetMsg;
    oSendNetMsg = (pSendNetMsg)DetourFunction(
        (PBYTE)(addrSendNetMsg),
        (PBYTE)hkSendNetMsg);
    printfdbg("Netchannel:SendNetMsg detoured at %x\n", addrSendNetMsg);

    opt.netchannedlhooked = 1;
}
  
float oldtick = 0; int c4id = 0;

float scaleDamageArmor(float flDamage, int armor_value)
{
    float flArmorRatio = 0.5f;
    float flArmorBonus = 0.5f;
    if (armor_value > 0) {
        float flNew = flDamage * flArmorRatio;
        float flArmor = (flDamage - flNew) * flArmorBonus;

        if (flArmor > static_cast<float>(armor_value)) {
            flArmor = static_cast<float>(armor_value) * (1.f / flArmorBonus);
            flNew = flDamage - flArmor;
        }

        flDamage = flNew;
    }
    return flDamage;
} 

namespace prediction {
    void start(CUserCmd* cmd, C_BasePlayer* localplayer);
    void end(C_BasePlayer* localplayer);
     
    inline CMoveData m_MoveData;
    inline float m_flOldCurtime;
    inline float m_flOldFrametime;
    inline int* m_pPredictionRandomSeed;
};

void prediction::start(CUserCmd* cmd, C_BasePlayer* localplayer) 
{   
    if (!m_pPredictionRandomSeed)
        m_pPredictionRandomSeed = *reinterpret_cast<int**>(FindPatternV2("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
    *m_pPredictionRandomSeed = MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF; // cmd->random_seed & 0x7FFFFFFF;
    m_flOldCurtime = iff.g_pGlobals->curtime;
    m_flOldFrametime = iff.g_pGlobals->frametime;
    iff.g_pGlobals->curtime = localplayer->GetTickBase() * iff.g_pGlobals->interval_per_tick;
    iff.g_pGlobals->frametime = iff.g_pGlobals->interval_per_tick;
    iff.g_pGameMovement->StartTrackPredictionErrors(localplayer);
    memset(&m_MoveData, 0, sizeof(m_MoveData));
    iff.g_pMoveHelper->SetHost_v((CBaseEntity*)localplayer);
    iff.g_pPrediction->SetupMove_v(localplayer, cmd, iff.g_pMoveHelper, &m_MoveData);
    iff.g_pGameMovement->ProcessMovement(localplayer, &m_MoveData);
    iff.g_pPrediction->FinishMove_v(localplayer, cmd, &m_MoveData);
}

void prediction::end(C_BasePlayer* localplayer)
{  
    iff.g_pGameMovement->FinishTrackPredictionErrors(localplayer); 
    iff.g_pMoveHelper->SetHost_v(nullptr); 
    *m_pPredictionRandomSeed = -1;
    iff.g_pGlobals->curtime = m_flOldCurtime;
    iff.g_pGlobals->frametime = m_flOldFrametime;
}


void fastStop(C_BasePlayer* localplayer, int flags, CUserCmd* cmd) noexcept
{  
    if (!localplayer || localplayer->GetLifeState() != LIFE_ALIVE)
        return;
    if (localplayer->GetMoveType() == MOVETYPE_NOCLIP || localplayer->GetMoveType() == MOVETYPE_LADDER || !(flags & FL_ONGROUND) || cmd->buttons & IN_JUMP)
        return;
    if (cmd->buttons & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK))
        return;

    const auto velocity = localplayer->GetVelocity();
    const auto speed = velocity.length2D();
    if (speed < 15.0f)
        return;

    Vector direction = velocity.toAngle();
    direction.y = cmd->viewangles.y - direction.y;

    const auto negatedDirection = Vector::fromAngle(direction) * -speed;
    cmd->forwardmove = negatedDirection.x;
    cmd->sidemove = negatedDirection.y;
} 

void FastLadder(C_BasePlayer* localplayer, CUserCmd* cmd)
{
    static bool checkkkk = false;
    float Up_down;

    Vector angle;
    iff.g_pEngineClient->GetViewAngles(angle);
    auto oldangles = cmd->viewangles;
    if (localplayer->GetMoveType() == MOVETYPE_LADDER)
    {
        if (angle.x < 15)
            Up_down = -89;
        else Up_down = 89;
        if (cmd->buttons & IN_FORWARD)
        {
            cmd->viewangles.x = Up_down;
            if (oldangles.y > 135 && oldangles.y <= 180)
            {
                cmd->viewangles.y = 90.0f;
                cmd->buttons |= IN_MOVELEFT;
            }
            else if (oldangles.y <= 135 && oldangles.y >= 90)
            {
                cmd->viewangles.y = 179.0f;
                cmd->buttons |= IN_MOVERIGHT;
            }
            else if (oldangles.y <= -135 && oldangles.y >= -180.0f)
            {
                cmd->viewangles.y = -90.f;
                cmd->buttons |= IN_MOVERIGHT;
            }
            else if (oldangles.y <= 90 && oldangles.y >= 45)
            {
                cmd->viewangles.y = 0;
                cmd->buttons |= IN_MOVELEFT;
            }
            else if (oldangles.y >= -90 && oldangles.y <= -45)
            {
                cmd->viewangles.y = -0;
                cmd->buttons |= IN_MOVERIGHT;
            }
            else if (oldangles.y <= -90 && oldangles.y >= -135)
            {
                cmd->viewangles.y = -179.0;
                cmd->buttons |= IN_MOVELEFT;
            }
            else if (oldangles.y <= 45 && oldangles.y > 0)
            {
                cmd->viewangles.y = 90;
                cmd->buttons |= IN_MOVERIGHT;
            }
            else if (oldangles.y <= -0 && oldangles.y >= -45)
            {
                cmd->viewangles.y = -90;
                cmd->buttons |= IN_MOVELEFT;
            }
        }
    }
    //duck on edge of ladder??
    if (localplayer->GetMoveType() == MOVETYPE_LADDER && !checkkkk)
    {
        checkkkk = true;
    }
    if (localplayer->GetMoveType() != MOVETYPE_LADDER && checkkkk)
    {
        cmd->buttons |= IN_DUCK;
        checkkkk = false;
    }

}


bool bSendPacket = 1;
bool __stdcall hkCreateMove2(float frame_time, CUserCmd* pCmd);

__declspec(naked) bool __stdcall hkCreateMove(float frame_time, CUserCmd* pCmd)
{ 
    __asm
    {
        push ebp
        mov ebp, esp
        mov bSendPacket, bl
    }

    hkCreateMove2(frame_time, pCmd);

__asm{
        mov bl, bSendPacket
        pop ebp
        ret 0x08
    }
}


bool __stdcall hkCreateMove2(float frame_time, CUserCmd* pCmd)
{
    static auto ofunc = ClientModeHook->GetOriginal<bool(__stdcall*)( float, CUserCmd*)>(24); 
          
    short localid = iff.g_pEngineClient->GetLocalPlayer();
    C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(localid));

    iff.pLocal = localplayer;

    if (!localplayer) return ofunc(frame_time, pCmd);

    if (g_Options.rcs) localplayer->GetViewPunchAngle() = Vector(0, 0, 0);
     
    if (!pCmd->command_number) return ofunc(frame_time, pCmd);
     
    const auto pre_flags = localplayer->GetFlags();

    bool interval = !((pCmd->tick_count + 1) % 10);
       
    if (g_Options.faststop)
        fastStop(localplayer, pre_flags, pCmd);
    if (g_Options.fastladder)
        FastLadder(localplayer, pCmd);
    if (g_Options.slidewalk)
        pCmd->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT; 
    if (g_Options.fastduck)
        pCmd->buttons |= IN_BULLRUSH;
     
    if (g_Options.bunnyhop && iff.g_pInputSystem->IsButtonDown(KEY_SPACE) && localplayer->GetMoveType() != MOVETYPE_LADDER)
        if (!(pre_flags & FL_ONGROUND) && pCmd->buttons & (IN_JUMP))
        {
            pCmd->buttons &= ~(IN_JUMP);

            if (g_Options.autostrafe) { 
                if (pCmd->mousedx < 0)
                    pCmd->sidemove = -450.0f;
                if (pCmd->mousedx > 0)
                    pCmd->sidemove = 450.0f; 
            }
        }
     
    if (*g_Options.c4timer && *(C_GameRulesProxy**)iff.GameRulesProxy) {  
        bool isbombplanted = (*(C_GameRulesProxy**)iff.GameRulesProxy)->IsBombPlanted();
        if (localplayer && isbombplanted)
        {
            float tick = localplayer->GetTickBase() * iff.g_pGlobals->interval_per_tick; 

            if (!c4id)
                for (int i = iff.g_pEngineClient->GetMaxClients() + 1; i <= iff.g_pEntityList->GetHighestEntityIndex() + 1; i++)
                {
                    auto entityList = iff.g_pEntityList->GetClientEntity(i); 
                    if (entityList && _tcsstr(entityList->GetClientClass()->GetName(), "CPlantedC4") != NULL && tick < ((CPlantedC4*)entityList)->GetC4Blow())
                    { 
                        printfdbg("Found PlantedC4 %d (%f %f)\n", i, tick, ((CPlantedC4*)entityList)->GetC4Blow());
                        c4id = i;
                        break;
                    }
                }

            if (c4id) {
                float C4Blow = ((CPlantedC4*)iff.g_pEntityList->GetClientEntity(c4id))->GetC4Blow();
                 
                if (interval)   
                { 
                    //calc damage
                    const auto damagePercentage = 1.0f;
                    auto flDamage = 500.f; // 500 - default, if radius is not written on the map https://i.imgur.com/mUSaTHj.png
                    auto flBombRadius = flDamage * 3.5f; 
                    auto flDistanceToLocalPlayer = (
                        (iff.g_pEntityList->GetClientEntity(c4id)->GetAbsOrigin() + ((C_BaseEntity*)iff.g_pEntityList->GetClientEntity(c4id))->GetViewOffset()) - 
                        (localplayer->GetAbsOrigin() + localplayer->GetViewOffset()) ).Length();// 
                    auto fSigma = flBombRadius / 3.0f;
                    auto fGaussianFalloff = exp(-flDistanceToLocalPlayer * flDistanceToLocalPlayer / (2.0f * fSigma * fSigma));
                    auto flAdjustedDamage = flDamage * fGaussianFalloff * damagePercentage;  
                    flAdjustedDamage = scaleDamageArmor(flAdjustedDamage, localplayer->GetArmorValue()); 
                    int healthleft = localplayer->GetHealth() - flAdjustedDamage; 
                     
                    char str[0x100] = "";
                    //snprintf(str, 0x100, "C4 <font color=\"#ffff00\">%d</font> HP <font color=\"#%s\">%d</font>", (int)(C4Blow - tick), healthleft < 1 ? "ff0000" : "00ff00", healthleft);
                    //iff.HudUniqueAlerts->GetPanel2D()->ShowAlert_v(str, false);   
                    snprintf(str, 0x100, " C4 explode in <font color=\"#%s\">%.*f</font> sec\x0", healthleft < 1 ? "ff0000" : "ffff00", 1, C4Blow - tick);
                    TextMsg(str);
                }
            }
        }
        else if (c4id) c4id = 0;
    }
     
    if (*g_Options.rankreveal && (pCmd->buttons & IN_SCORE) != 0) 
        iff.g_pClient->DispatchUserMessage(CS_UM_ServerRankRevealAll, 0, 0, nullptr);
    
     
    if (g_Options.speclist && interval && localplayer->GetHealth() > 0)
    { 
        string spectatorList = "Spectating you:\n \n";
        bool isSomeoneSpectatingYou = false;
        for (short i = 1; i < iff.g_pEngineClient->GetMaxClients() + 1; i++)
        {
            if (i == localid) continue; 

            C_BasePlayer* Entity = (C_BasePlayer*)iff.g_pEntityList->GetClientEntity(i);
            if (Entity && Entity->GetLifeState() == LIFE_DEAD && Entity->GetHealth() < 1 && !Entity->IsDormant() && Entity->GetObserverTarget() == localid)
            {
                isSomeoneSpectatingYou = true;

                player_info_t pinfo;
                iff.g_pEngineClient->GetPlayerInfo(i, &pinfo);

                spectatorList.append(pinfo.name).append("\n"); 
            }
        }
        if (isSomeoneSpectatingYou) ShowMenu(spectatorList);
    }
         
    /*
    if (localplayer && g_Options.predict)
    {
        prediction::start(pCmd, localplayer); 
        auto post_flags = localplayer->GetFlags();  
        if ((post_flags & FL_ONGROUND || post_flags & FL_PARTIALGROUND) && !(pre_flags & FL_ONGROUND || pre_flags & FL_PARTIALGROUND)) //predicting that we're gonna hit the ground
        { 
            pCmd->buttons |= IN_DUCK;
            pCmd->buttons &= IN_JUMP;
        }  
        prediction::end(localplayer);
    }
    */
     
    if (g_Options.blockbot) {
        // Get our ground entity.
        auto pGroundEntity = iff.g_pEntityList->GetClientEntityFromHandle(localplayer->GetGroundEntity());

        // Check if there's a player under us.
        if (pGroundEntity && _tcsstr(pGroundEntity->GetClientClass()->GetName(), "CCSPlayer"))
        {
            Vector viewAngles;
            iff.g_pEngineClient->GetViewAngles(viewAngles);
            Vector localpos = localplayer->GetAbsOrigin();
            Vector entpos = pGroundEntity->GetAbsOrigin();
            Vector delta = localpos - entpos;
            float deltaXold = delta[0]; float deltaYold = delta[1];
            delta[0] = deltaXold * cos(viewAngles[1] * PI / 180) + deltaYold * sin(viewAngles[1] * PI / 180);
            delta[1] = -deltaXold * sin(viewAngles[1] * PI / 180) + deltaYold * cos(viewAngles[1] * PI / 180); 
            pCmd->forwardmove = std::clamp(-delta[0] * 40, -450.f, 450.f);
            pCmd->sidemove = std::clamp(delta[1] * 40, -450.f, 450.f);
        }
    }

    if (g_Options.rcs) {
        Vector aimPunchAngles = localplayer->GetAimPunchAngle(); 
        Vector currentViewAngles; iff.g_pEngineClient->GetViewAngles(currentViewAngles); 
        Vector newAngles = (currentViewAngles - aimPunchAngles * 2);
        newAngles.Clamp(); newAngles.Normalize();
        pCmd->viewangles = newAngles;   
        //localplayer->GetViewPunchAngle() = Vector(0, 0, 0);
    } 
      
    
    if (g_Options.fakelag)
    {
        auto choked = iff.g_pClientState->m_nChokedCommands;

        if (choked < g_Options.fakelag)
            bSendPacket = 0;
        else
            bSendPacket = 1;
    }
    
     
    /*
    if (!iff.g_pClientState->m_nChokedCommands)
    {
        static bool flip_movement = false;

        if (pCmd->forwardmove == 0.0f && !(pCmd->buttons & (int)IN_JUMP))
        {
            pCmd->forwardmove += flip_movement ? -1.01f : 1.01f;
        }//nonsense
        flip_movement = !flip_movement;
    }
    */

    if (g_Options.aimbot)
        if (g_Options.aimbotautoshoot || iff.g_pInputSystem->IsButtonDown(MOUSE_LEFT))
            AimBot(pCmd);

    pCmd->viewangles.Clamp();
    pCmd->viewangles.Normalize();
    pCmd->forwardmove = std::clamp(pCmd->forwardmove, -450.f, 450.f);
    pCmd->sidemove = std::clamp(pCmd->sidemove, -450.f, 450.f);
    pCmd->upmove = std::clamp(pCmd->upmove, -320.f, 320.f);

    //return ofunc(frame_time, pCmd); 
     
    return false;
} 



typedef const __int64(__cdecl* pDevMsg)(_In_z_ _Printf_format_string_ char const* const _Format, ...);
pDevMsg oDevMsg; //(char* a1, int a2, char a3)
__int64 __cdecl hkDevMsg(_In_z_ _Printf_format_string_ char const* const _Format, ...)
{ 
    int _Result;
    va_list _ArgList;
    __crt_va_start(_ArgList, _Format); 
    if (*g_Options.debugstuff) {
        iff.myConMsg("[Seaside] DevMsg: ");
        iff.myConMsg(_Format, _ArgList);
    }
    printfdbg("DevMsg: ");
    _Result = _vfprintf_l(stdout, _Format, NULL, _ArgList);
    __crt_va_end(_ArgList);   
    return oDevMsg(_Format, NULL, _ArgList);
}

typedef const __int64(__cdecl* pDevWarningMsg)(_In_z_ _Printf_format_string_ char const* const _Format, ...);
pDevMsg oDevWarningMsg;  
__int64 __cdecl hkDevWarningMsg(_In_z_ _Printf_format_string_ char const* const _Format, ...)
{
    int _Result;
    va_list _ArgList;
    __crt_va_start(_ArgList, _Format);
    if (*g_Options.debugstuff) {
        iff.myConMsg("[Seaside] DevWarningMsg: ");
        iff.myConMsg(_Format, _ArgList);
    }
    printfdbg("DevWarningMsg: ");
    _Result = _vfprintf_l(stdout, _Format, NULL, _ArgList);
    __crt_va_end(_ArgList); 
    return oDevWarningMsg(_Format, NULL, _ArgList);
}
 
int msgcount = 0;

//sv_show_usermessage 2 for local server //https://www.unknowncheats.me/forum/counterstrike-global-offensive/492173-dispatchusermessage-client-call.html
bool __fastcall hkDispatchUserMessage(void* thisptr, void*, int msg_type, int32 nFlags, int size, bf_read& msg_data)
{   
    printfdbg("DispatchUserMessage type %d flags %d size %d data -> %x\n", msg_type, nFlags, size, &msg_data);
    if (*g_Options.debugstuff) { 
        iff.myConMsg("[Seaside] DispatchUserMessage type %d flags %d size %d data -> %x\n", msg_type, nFlags, size, &msg_data);
    }

    static auto ofunc = ClientHook->GetOriginal<bool(__thiscall*)(void*, int, int32, int, const void*)>(38);
        
    /* //read chat example
    if (msg_type == CS_UM_SayText2)
    {
        bf_read read = bf_read(reinterpret_cast<const void*>(&msg_data), size); 
        auto unk1 = read.ReadByte(); 
        auto ent_index = read.ReadByte(); 
        char databuf[1024]; 
        read.ReadBytes(databuf, 3); 
        char msg_name[1024] = ""; 
        read.ReadBytes(msg_name, read.ReadByte()); 
        read.ReadByte(); // \" 
        char player_name[1024] = "";
        read.ReadBytes(player_name, read.ReadByte()); 
        read.ReadByte(); // \" 
        char message[1024] = "";
        read.ReadBytes(message, read.ReadByte());  
    }
    */
      
    return ofunc(thisptr,msg_type,nFlags,size, &msg_data);
}

//static auto element = FindHudElement("CCSGO_HudRadar"); //money from radarbase? 
typedef const int(__fastcall* pGetPlayerMoney)(void* _this, void* edx, int ent_index);
pGetPlayerMoney oGetPlayerMoney;
int __fastcall hkGetPlayerMoney(void* this_, void* edx, int ent_index) 
{     
    static C_CS_PlayerResource** g_player_resource = C_CS_PlayerResource::GetPlayerResource(); 
    auto player = iff.g_pEntityList->GetClientEntity(ent_index);
    auto localplayer = ((C_BasePlayer*)iff.g_pEntityList->GetClientEntity(iff.g_pEngineClient->GetLocalPlayer()));
     
    if (!*g_Options.moneyreveal || !player || !localplayer || !(*g_player_resource) || ((C_BasePlayer*)player)->GetTeam() == localplayer->GetTeam() ) return oGetPlayerMoney(this_, edx, ent_index);
      
    if (player->IsDormant())
    {
        int money = (*g_player_resource)->GetMatchStats_CashEarned_Total()[ent_index] - (*g_player_resource)->GetTotalCashSpent()[ent_index]; 
        money += iff.g_pCVar->FindVar("mp_startmoney")->GetInt(); 
        int maxmoney = iff.g_pCVar->FindVar("mp_maxmoney")->GetInt();
        if (money > maxmoney) money = maxmoney;
       // printfdbg("Dormant %d Money %d (cash earned %d, total spent %d)\n", ent_index, money, (*g_player_resource)->GetMatchStats_CashEarned_Total()[ent_index], (*g_player_resource)->GetTotalCashSpent()[ent_index] );
        return money;
    }
    else  
    {
       // printfdbg("NotDormant %d Money %d\n", ent_index, ((C_BasePlayer*)player)->GetAccount());  
        return ((C_BasePlayer*)player)->GetAccount();
    }
}

VMTHook* ViewRenderHook = nullptr;
  
void __fastcall hkRenderSmokeOverlay(uintptr_t ecx, uintptr_t edx, bool a1)
{
    static auto oRenderSmokeOverlay = ViewRenderHook->GetOriginal<void(__thiscall*)(uintptr_t, bool)>(41);
    if (!g_Options.wfsmoke)
        oRenderSmokeOverlay(ecx, a1);
    else *reinterpret_cast<float*>(std::uintptr_t(iff.g_ViewRender) + 0x588) = 0.0f;
}