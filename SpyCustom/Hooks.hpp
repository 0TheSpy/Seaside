#pragma once


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
    material->SetMaterialVarFlag(MATERIAL_VAR_FLAT, g_Options.materials.value->arr[d].flat);
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



VMTHook* CacheHook = nullptr;
MDLHandle_t __fastcall hkFindMDL(void* ecx, void* edx, char* FilePath)
{
    static auto oFindMDL = CacheHook->GetOriginal<MDLHandle_t(__thiscall*)(void*, char*)>(10);


    if (g_Options.models.value->arr[2].active && strstr(FilePath, "models/weapons/v_models/arms") && !strstr(FilePath, "gloves"))
    {
#ifdef DEBUG
        printf("Replacing %s with %s\n", FilePath, g_Options.models.value->arr[2].vmodel_repl);
#endif
        sprintf(FilePath, g_Options.models.value->arr[2].vmodel_repl);
    }

    if (g_Options.models.value->arr[3].active && strstr(FilePath, "models/weapons/v_knife"))
    {
#ifdef DEBUG
        printf("Replacing %s with %s\n", FilePath, g_Options.models.value->arr[3].vmodel_repl);
#endif
        sprintf(FilePath, g_Options.models.value->arr[3].vmodel_repl);
    }

    for (int i = 4; i < g_Options.models.value->itemcount; i++)
    {
        if (g_Options.models.value->arr[i].active && g_Options.models.value->arr[i].findMDLmethode && strstr(FilePath, g_Options.models.value->arr[i].vmodel_orig))
        {
#ifdef DEBUG
            printf("Replacing %s with %s\n", FilePath, g_Options.models.value->arr[i].vmodel_repl);
#endif
            sprintf(FilePath, g_Options.models.value->arr[i].vmodel_repl);
        }
    }


    return oFindMDL(ecx, FilePath);
}

 

VMTHook* ClientModeHook = nullptr;
static void __stdcall hkdoPostScreenEffects(void* param) noexcept
{
    static auto ofunc = ClientModeHook->GetOriginal<void(__thiscall*)(IClientMode*, void*)>(44);

    if (*g_Options.entityloop_count) {
        if (iff.g_pEngineClient->IsInGame())
        {
            int localplayer_index = iff.g_pEngineClient->GetLocalPlayer();
            C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(localplayer_index));

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

    ofunc(iff.g_ClientMode, param);
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