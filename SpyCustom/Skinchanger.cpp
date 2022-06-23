#include "SkinChanger.hpp"
#include "Other.hpp"

VMTHook* GameEventManagerHook = nullptr;
VMTHook* ClientHook = nullptr;
std::vector<iitem> rarities;
std::vector<iitem> knives;
std::vector<iitem> gloves;
std::vector<iitem> agents;
std::vector<iitem> medals;
std::vector<iitem> musickits;
std::vector <iitem> qualities;
 
std::add_pointer_t<ItemSystem* __cdecl()> itemSystem;




enum class EStickerAttributeType
{
    Index,
    Wear,
    Scale,
    Rotation
};

static auto s_econ_item_interface_wrapper_offset = std::uint16_t(0);

struct GetStickerAttributeBySlotIndexFloat
{
    static auto __fastcall hooked(void* thisptr, void*, const int slot,
        const EStickerAttributeType attribute, const float unknown) -> float
    {
        auto item = reinterpret_cast<C_BaseAttributableItem*>(std::uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

        const auto defindex = item->GetItemDefinitionIndex();
         
        int config = GetCfgIndex(defindex);

        if (config != -1)
        {
            switch (attribute)
            {
            case EStickerAttributeType::Wear:
                return g_Options.weapons.value->arr[config].stickers[slot].wear;
            case EStickerAttributeType::Scale:
                return g_Options.weapons.value->arr[config].stickers[slot].scale;
            case EStickerAttributeType::Rotation:
                return g_Options.weapons.value->arr[config].stickers[slot].rotation;
            default:
                break;
            }
        }

        return m_original(thisptr, nullptr, slot, attribute, unknown);
    }

    static decltype(&hooked) m_original;
};

decltype(GetStickerAttributeBySlotIndexFloat::m_original) GetStickerAttributeBySlotIndexFloat::m_original;

struct GetStickerAttributeBySlotIndexInt
{
    static auto __fastcall hooked(void* thisptr, void*, const int slot,
        const EStickerAttributeType attribute, const int unknown) -> int
    {
        auto item = reinterpret_cast<C_BaseAttributableItem*>(std::uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

        if (attribute == EStickerAttributeType::Index)
        {
            const auto defindex = item->GetItemDefinitionIndex();

            int config = GetCfgIndex(defindex);

            if (config != -1)
                return g_Options.weapons.value->arr[config].stickers[slot].stickerid;
        }

        return m_original(thisptr, nullptr, slot, attribute, unknown);
    }

    static decltype(&hooked) m_original;
};

decltype(GetStickerAttributeBySlotIndexInt::m_original) GetStickerAttributeBySlotIndexInt::m_original;


void ApplyStickers(C_BaseAttributableItem* item)
{
    if (!s_econ_item_interface_wrapper_offset)
        s_econ_item_interface_wrapper_offset = netvar_manager::get().get_offset(FNV("CBaseAttributableItem->m_Item")) + 0xC;

    static vmt_multi_hook hook;

    const auto econ_item_interface_wrapper = std::uintptr_t(item) + s_econ_item_interface_wrapper_offset;

    if (hook.initialize_and_hook_instance(reinterpret_cast<void*>(econ_item_interface_wrapper)))
    {
        hook.apply_hook<GetStickerAttributeBySlotIndexFloat>(4);
        hook.apply_hook<GetStickerAttributeBySlotIndexInt>(5);
    }
}


bool __fastcall hkFireEventClientSide(void* thisptr, void* edx, IGameEvent* event)
{
    static auto ofunc = GameEventManagerHook->GetOriginal<bool(__fastcall*)(void*, void*, IGameEvent*)>(9);
    if (event) {
        if (!strcmp(event->GetName(), "player_death") && iff.g_pEngineClient->GetPlayerForUserID(event->GetInt("attacker")) == iff.g_pEngineClient->GetLocalPlayer())
        {
#ifdef DEBUG
            printf("BE playerdeath with %s\n", event->GetString("weapon"));
#endif
            for (int i = 0; i < g_Options.weapons.value->weaponzcount; i++)  
            {
                if (_tcsstr(g_Options.weapons.value->arr[i].killfeedicon, event->GetString("weapon")))
                {
                    if (g_Options.weapons.value->arr[i].active && g_Options.weapons.value->arr[i].killfeediconactive)  
                        event->SetString("weapon", g_Options.weapons.value->arr[i].killfeediconreplace);

                    if (!g_Options.weapons.value->arr[i].ownerunk && g_Options.weapons.value->arr[i].stattrak != -1)
                        g_Options.weapons.value->arr[i].stattrak += 1;


                }
            }

        }
    }

    return ofunc(thisptr, edx, event);

}



C_CS_PlayerResource** g_player_resource;
 
#include <array>
bool Changer()
{
    static bool initinterface = 0;
    if (!initinterface) {
        const auto team_arr_prop = C_CS_PlayerResource::GetTeamProp();
        const auto team_prop = team_arr_prop->m_pDataTable->m_pProps;
        const auto proxy_addr = std::uintptr_t(team_prop->m_ProxyFn);
        g_player_resource = *reinterpret_cast<C_CS_PlayerResource***>(proxy_addr + 0x10);
#ifdef DEBUG
        printf("g_player_resource initiated %x\n", g_player_resource);
#endif
        initinterface = 1;
    }

    if (!iff.g_pEngineClient->IsConnected()) {
        return 0;
    }

    int localplayer_index = iff.g_pEngineClient->GetLocalPlayer();
    C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(localplayer_index));

    if (!localplayer)
        return 0;
      

    auto player_resource = *g_player_resource;

    if (g_Options.weapons.value->arr[4].active)
        player_resource->GetCoins()[localplayer_index] = g_Options.weapons.value->arr[4].modeldefindex; 

    if (g_Options.weapons.value->arr[5].active)
        player_resource->GetMusicKits()[localplayer_index] = g_Options.weapons.value->arr[5].modeldefindex; 
     
    if (g_Options.profile_active)
    { 
        int ranktype = player_resource->GetRankType()[localplayer_index];
        int selectedrank = 0;
        if (ranktype == 7) selectedrank = 1;
        if (ranktype == 10) selectedrank = 2;

        player_resource->GetRanks()[localplayer_index] = g_Options.rankz.value->arr[selectedrank].rank;
        player_resource->GetWins()[localplayer_index] = g_Options.rankz.value->arr[selectedrank].wins;

        player_resource->GetLevel()[localplayer_index] = g_Options.level;
        player_resource->GetComLeader()[localplayer_index] = g_Options.comleader;
        player_resource->GetComTeacher()[localplayer_index] = g_Options.comteacher;
        player_resource->GetComFriendly()[localplayer_index] = g_Options.comfriendly; 
    }

    if (g_Options.fakescore)
    { 
        player_resource->GetMVPs()[localplayer_index] = g_Options.MVPs;
        player_resource->GetScore()[localplayer_index] = g_Options.score;
        player_resource->GetKills()[localplayer_index] = g_Options.kills;
        player_resource->GetAssists()[localplayer_index] = g_Options.assists;
        player_resource->GetDeaths()[localplayer_index] = g_Options.deaths; 
    }


    int team = localplayer->GetTeam(); char* model = 0;
    if (team == TEAM_TERRORIST && g_Options.weapons.value->arr[2].active && g_Options.weapons.value->arr[2].modelactive)
        model = g_Options.weapons.value->arr[2].model;
    if (team == TEAM_CT && g_Options.weapons.value->arr[3].active && g_Options.weapons.value->arr[3].modelactive)
        model = g_Options.weapons.value->arr[3].model;

    
    if (model != 0) {
        localplayer->GetModelIndex() = iff.g_pMdlInfo->GetModelIndex(model);
        if (const auto modelprecache = iff.g_pNetworkStringTableContainer->FindTable("modelprecache")) 
        {
            modelprecache->AddString(false, model);
            const auto viewmodelArmConfig = iff.getPlayerViewmodelArmConfigForPlayerModel(model);
            modelprecache->AddString(false, viewmodelArmConfig[2]);
            modelprecache->AddString(false, viewmodelArmConfig[3]);
             
          
        }

        

        const auto m_hRagdoll = (C_BaseEntity*)iff.g_pEntityList->GetClientEntityFromHandle(localplayer->Ragdoll());
        if (m_hRagdoll)
            m_hRagdoll->GetModelIndex() = iff.g_pMdlInfo->GetModelIndex(model);
        
    }


    if (localplayer->GetLifeState() != LIFE_ALIVE)
    {
        if (g_Options.weapons.value->arr[1].active && g_Options.weapons.value->arr[1].modelactive) {
            auto destroyglove = iff.g_pEntityList->GetClientEntityFromHandle(localplayer->GetWearables()[0]);
            if (destroyglove) {
                destroyglove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
                destroyglove->GetClientNetworkable()->Release();
#ifdef DEBUG
                printf("destroyed glove\n");
#endif
            }
        }
        return 0;
    }

    player_info_t localplayer_info;
    if (!iff.g_pEngineClient->GetPlayerInfo(localplayer_index, &localplayer_info))
        return 0;

    CBaseHandle viewmodelHandle = localplayer->GetViewModel();
    C_BaseViewModel* pViewModel = (C_BaseViewModel*)iff.g_pEntityList->GetClientEntityFromHandle(viewmodelHandle);
    const auto view_model_weapon = (C_BaseAttributableItem*)iff.g_pEntityList->GetClientEntityFromHandle(pViewModel->GetWeapon());

    int idi = view_model_weapon->GetItemDefinitionIndex();
    if (idi == WEAPON_KNIFE) idi = WEAPON_KNIFE_T;

    int modelcfgindex = GetCfgIndex2(idi);
    if (modelcfgindex != -1 && g_Options.models.value->arr[modelcfgindex].active)
    {
        char* model = g_Options.models.value->arr[modelcfgindex].vmodel_repl;
        const auto override_model_index = iff.g_pMdlInfo->GetModelIndex(model);
        view_model_weapon->SetModelIndex(override_model_index)  ;
        pViewModel->SetModelIndex(override_model_index)  ;
        if (const auto modelprecache = iff.g_pNetworkStringTableContainer->FindTable("modelprecache"))
        {
            modelprecache->AddString(false, model);
        }
    }
    
    int seqact = opt.needtogetseqact;
    if (seqact)
    {
        int oldmodelindex = pViewModel->GetModelIndex(); 
        pViewModel->SetModelIndex(iff.g_pMdlInfo->GetModelIndex(g_Options.models.value->arr[seqact].vmodel_orig));
        for (int i = 0; i < 20; i++)
            strcpy(g_Options.models.value->arr[seqact].seqs[i].seq_orig_def, (char*)pViewModel->GetSequenceActivity(i)); 
        pViewModel->SetModelIndex(iff.g_pMdlInfo->GetModelIndex(g_Options.models.value->arr[seqact].vmodel_repl));
        for (int i = 0; i < 20; i++)
        {
            strcpy(g_Options.models.value->arr[seqact].seqs[i].seq_repl_def, (char*)pViewModel->GetSequenceActivity(i));

            char newpreview[32] = "";
            strcat_s(newpreview, std::to_string(i).c_str());
            strcat_s(newpreview, ": ");
            strcat_s(newpreview, g_Options.models.value->arr[seqact].seqs[i].seq_repl_def);
            strcpy(g_Options.models.value->arr[seqact].seqs[i].seq_repl_def_preview, newpreview);
        }
        pViewModel->SetModelIndex(oldmodelindex);
        opt.needtogetseqact = 0;
    }
    
    if (g_Options.weapons.value->arr[1].active && g_Options.weapons.value->arr[1].modelactive) 
    {
        auto hWearables = localplayer->GetWearables();

        static int glove_handle;
        C_BaseAttributableItem* glove = (C_BaseAttributableItem*)iff.g_pEntityList->GetClientEntityFromHandle(hWearables[0] );

        if (!glove)     
        {
            C_BaseAttributableItem* our_glove = (C_BaseAttributableItem*)iff.g_pEntityList->GetClientEntityFromHandle(glove_handle);

            if (our_glove)     
            {
                hWearables[0] = glove_handle;
                glove = our_glove;
            }
        }

        if (!glove)
        {
            static std::add_pointer_t<C_BaseEntity* __cdecl(int, int)> createWearable = nullptr;
             
            if (!createWearable) {
                createWearable = []() -> decltype(createWearable)
                {
                    for (auto clientClass = iff.g_pClient->GetAllClasses(); clientClass; clientClass = clientClass->m_pNext)
                        if (clientClass->m_ClassID == 54) 
                            return (std::add_pointer_t<C_BaseEntity* __cdecl(int, int)>)clientClass->m_pCreateFn;
                    return nullptr;
                }();
            }

            const auto serial = rand() % 0x1000;
            auto entry = iff.g_pEntityList->GetHighestEntityIndex() + 1;
            for (int i = 65; i < iff.g_pEntityList->GetHighestEntityIndex(); i++)
            {
                auto pEntity = iff.g_pEntityList->GetClientEntity(i);
                if (pEntity && pEntity->GetClientClass()->m_ClassID == 70)           
                {
                    entry = i;
                    break;
                }
            }

            createWearable(entry, serial);
            glove = (C_BaseAttributableItem*)iff.g_pEntityList->GetClientEntity(entry);
            glove->initialized() = true;
            hWearables[0] = entry | serial << 16;

            static int(__thiscall * equipWearable)(void* wearable, void* player) = reinterpret_cast<decltype(equipWearable)>(FindPatternV2("client.dll", "55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9"));
            equipWearable(glove, localplayer);
            localplayer->body() = 1;        
            glove->SetModelIndex(iff.g_pMdlInfo->GetModelIndex(g_Options.weapons.value->arr[1].worldmodel));
            iff.g_pClientState->ForceFullUpdate();
        }

        if (glove)
        {
            glove->GetItemDefinitionIndex() = g_Options.weapons.value->arr[1].modeldefindex;
            glove->GetItemIDHigh() = -1;
            glove->GetAccountID() = localplayer_info.xuidlow;
            glove->GetFallbackPaintKit() = g_Options.weapons.value->arr[1].skinid;
            glove->GetFallbackSeed() = g_Options.weapons.value->arr[1].seed;
            glove->GetFallbackWear() = g_Options.weapons.value->arr[1].wear;

        }
    }

     
    if (g_Options.weapons.value->arr[0].active && g_Options.weapons.value->arr[0].modelactive) 
    {

        static int lastmdlindex = -1;
        int curmdlindex = pViewModel->GetModelIndex();
        if (lastmdlindex != curmdlindex)  
        {
            lastmdlindex = curmdlindex;
            int defindex = view_model_weapon->GetItemDefinitionIndex();

            if (!is_knife(defindex)) {
                for (int i = 0; i < 20; i++)    
                    if (_tcsstr(pViewModel->GetSequenceActivity(i), "draw"))
                    {
                        pViewModel->SendViewModelMatchingSequence(i); break;
                    }
            }
        }

        if (view_model_weapon && is_knife(view_model_weapon->GetItemDefinitionIndex())) 
        {
            const auto override_model_index = iff.g_pMdlInfo->GetModelIndex(g_Options.weapons.value->arr[0].model);
            pViewModel->GetModelIndex() = override_model_index; 

        }
    }




 
    auto& weapons = localplayer->GetWeapons();

    for (auto weapon_handle : weapons)
    {
        if (weapon_handle == INVALID_EHANDLE_INDEX)
            break;

        C_BaseAttributableItem* weapon = static_cast<C_BaseAttributableItem*>(iff.g_pEntityList->GetClientEntityFromHandle(weapon_handle));

        if (!weapon)
            continue;

        short item_definition_index = weapon->GetItemDefinitionIndex();

        if (item_definition_index == WEAPON_C4 && g_Options.weapons.value->arr[GetCfgIndex(item_definition_index)].active)
            weapon->body() = 2; 
        
        if (item_definition_index == 42) item_definition_index = 59;

         

        int configindex = GetCfgIndex(item_definition_index); 
        if (configindex == -1) continue; 

        if (g_Options.weapons.value->arr[configindex].active)
        {
            if (is_knife(item_definition_index) && g_Options.weapons.value->arr[0].modelactive)
            {
                weapon->GetItemDefinitionIndex() = g_Options.weapons.value->arr[0].modeldefindex;
                short mdlindex = iff.g_pMdlInfo->GetModelIndex(g_Options.weapons.value->arr[0].model);
                weapon->GetModelIndex() = mdlindex;
                auto m_pWorld = (C_BaseEntity*)iff.g_pEntityList->GetClientEntityFromHandle(weapon->GetWeaponWorldModel());
                m_pWorld->GetModelIndex() = mdlindex+1;
            }
            weapon->GetItemIDHigh() = -1;

            if (!g_Options.weapons.value->arr[configindex].ownerunk)
                weapon->GetAccountID() = localplayer_info.xuidlow;
            weapon->GetEntityQuality() = g_Options.weapons.value->arr[configindex].quality;
            snprintf(weapon->GetCustomName(), 32, "%s", g_Options.weapons.value->arr[configindex].nametag);

            weapon->GetFallbackPaintKit() = g_Options.weapons.value->arr[configindex].skinid;
            weapon->GetFallbackSeed() = g_Options.weapons.value->arr[configindex].seed;
            weapon->GetFallbackWear() = g_Options.weapons.value->arr[configindex].wear;
            weapon->GetFallbackStatTrak() = g_Options.weapons.value->arr[configindex].stattrak;

            ApplyStickers(weapon);

             

        }
    }


    

    if (*g_Options.nvgsON)
        NightvisionRun(localplayer);
   
    if (*g_Options.flashlightON)
        FlashlightRun(localplayer);

    if (*g_Options.weatheractive)
        do_precipitation();

    return 1;
}




void __fastcall hkFrameStageNotify(IBaseClientDLL* thisptr, void* edx, ClientFrameStage_t stage)
{
    static auto oFrameStageNotify = ClientHook->GetOriginal<void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t stage)>(37);
    
    if ( (*g_Options.playerloop_count || *g_Options.entityloop_count) && stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
    {
        for (int i = 1; i < 65; i++)
        {
            C_BasePlayer* pEntity = (C_BasePlayer*)iff.g_pEntityList->GetClientEntity(i);

            if (i == iff.g_pEngineClient->GetLocalPlayer()) continue;

            if (!pEntity || pEntity->IsDormant())  
                continue;

            int team = pEntity->GetTeam();
            if ((team < 2) | (team > 3)) continue;
            team -= 2;

            if (g_Options.models.value->arr[team].active) {

                char* model = g_Options.models.value->arr[team].vmodel_repl;

                pEntity->GetModelIndex() = iff.g_pMdlInfo->GetModelIndex(model);
                if (const auto modelprecache = iff.g_pNetworkStringTableContainer->FindTable("modelprecache"))
                {
                    modelprecache->AddString(false, model);
                    const auto viewmodelArmConfig = iff.getPlayerViewmodelArmConfigForPlayerModel(model);
                    modelprecache->AddString(false, viewmodelArmConfig[2]);
                    modelprecache->AddString(false, viewmodelArmConfig[3]);
                }

                const auto m_hRagdoll = (C_BaseEntity*)iff.g_pEntityList->GetClientEntityFromHandle(pEntity->Ragdoll());
                if (m_hRagdoll)
                    m_hRagdoll->SetModelIndex(iff.g_pMdlInfo->GetModelIndex(model));
            }

            if (g_Options.models.value->arr[team].active_scale)
                pEntity->GetModelScale() = g_Options.models.value->arr[team].scale;

        }
    }

    if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {

            Changer();
              

        if (opt.needupdate) {
            static auto clear_hud_weapon_icon_ptr = FindPatternV2("client.dll", "E8 ? ? ? ? 8B F0 C6 44 24 ? ? C6 44 24") + 1; 
            static auto clearHudWeapon = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(clear_hud_weapon_icon_ptr);
            auto element = FindHudElement("CCSGO_HudWeaponSelection");
            auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0x28); 

            if (hud_weapons && *hud_weapons->get_weapon_count() > 0)
                for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
                    i = clearHudWeapon(hud_weapons, i);

            opt.needupdate = 0;
        }
    }

    oFrameStageNotify(thisptr, stage);
}




void InitSkinChanger()
{
    itemSystem = relativeToAbsolute<decltype(itemSystem)>(FindPatternV2("client.dll", "E8 ? ? ? ? 0F B7 0F") + 1);

#ifdef DEBUG
    printf("itemschema2 %x\n", itemSystem()->getItemSchema());
#endif

    for (const auto& node : itemSystem()->getItemSchema()->itemsSorted) {           
        const auto item = node.value;

        if (std::strcmp(item->m_szWeaponType, "#CSGO_Type_Knife") == 0)
            if (item->m_szWorldModel)
                knives.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, item->m_szViewModel, item->m_szWorldModel));
            else knives.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, item->m_szViewModel, ""));

        if (std::strcmp(item->m_szWeaponType, "#CSGO_Type_Collectible") == 0)
            medals.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, "", ""));

        if (std::strcmp(item->m_szWeaponType, "#Type_Hands") == 0)
            if (item->m_szWorldModel)
                gloves.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, item->m_szViewModel, item->m_szWorldModel));
            else gloves.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, item->m_szViewModel, ""));

        if (std::strcmp(item->m_szWeaponType, "#Type_CustomPlayer") == 0)
            if (item->m_szWorldModel)
                agents.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, item->m_szViewModel, item->m_szWorldModel));
            else agents.push_back(iitem(item->m_iItemDefinitionIndex, item->m_szWeaponName3, item->m_szViewModel, ""));

    }

    for (const auto& node : itemSystem()->getItemSchema()->qualities) {   
        const auto item = node.value;
        qualities.push_back(iitem(item.id, item.name, "", ""));
    }

    for (const auto& node : itemSystem()->getItemSchema()->rars) {  
        const auto item = node.value;
        rarities.push_back(iitem(item.id, item.name.szBuffer, "", ""));
    }

    auto musickitsS = itemSystem()->getItemSchema()->music_definitions;
#ifdef DEBUG
    std::cout << musickitsS.memory << " " << musickitsS.itemcount << std::endl;
#endif
    for (int i = (int)musickitsS.memory; i < (int)musickitsS.memory + musickitsS.itemcount * 0x18; i += 0x18)
    {
        CEconMusicDefinition* music = *(CEconMusicDefinition**)(i + 0x14);
        musickits.push_back(iitem(music->id, music->name, "", ""));
    }

    const auto& paintkitMap = itemSystem()->getItemSchema()->paintKits;
    int paintkitcount = 0;
    for (const auto& node : paintkitMap) {
        const auto pKit = node.value;
        opt.sc_skins.push_back(Options_my::skinstruct(pKit->nID, pKit->sName.szBuffer, paintkitcount, pKit->rarity, pKit->color1, pKit->color2, pKit->color3, pKit->color4, pKit->pearlescent, (DWORD)&pKit->nID));
        paintkitcount++;
    }

    const auto& stickerMap = itemSystem()->getItemSchema()->stickerKits;
    int stickercount = 0;
    for (const auto& node : stickerMap) { 
        const auto stickerKit = node.value; 
        opt.sc_stickers.push_back(Options_my::stickerstruct(stickerKit->id, stickerKit->name.szBuffer, stickercount));
        stickercount++; 
    }

#ifdef DEBUG
    printf("counts %d %d %d %d %d %d %d\n", 
        knives.size(), gloves.size(), agents.size(), medals.size(), musickits.size(), opt.sc_skins.size(), opt.sc_stickers.size());
#endif

    const auto& prefabMap = itemSystem()->getItemSchema()->prefabs;
    for (const auto& node : prefabMap) {
        KeyValues* kv = node.value; 
        int cfgindex = GetCfgIndexByPrefab(fnv2::hash(kv->GetName()));
        if (cfgindex == -1) continue;
        int soundcount = 0;
        const char* vmodel_orig = kv->GetString("model_player");
        strcpy(g_Options.models.value->arr[cfgindex].vmodel_orig, vmodel_orig);
        strcpy(g_Options.models.value->arr[cfgindex].vmodel_repl_temp, vmodel_orig);
        strcpy(g_Options.models.value->arr[cfgindex].vmodel_repl, vmodel_orig);

        g_Options.materials.value->arr[cfgindex + 2].model_hash = fnv2::hash(vmodel_orig);
       

        kv = kv->FindKey("visuals");
        if (kv) {
            char* sound_single_shot = (char*)kv->GetString("sound_single_shot");
            if (sound_single_shot[0] != 0x0) {
                strcpy(g_Options.models.value->arr[cfgindex].sounds[soundcount].name, sound_single_shot);
                g_Options.models.value->arr[cfgindex].sounds[soundcount].hash = GenerateSoundEntryHash(sound_single_shot);
                soundcount++;
            }

            char* sound_single_shot_accurate = (char*)kv->GetString("sound_single_shot_accurate");
            if (sound_single_shot_accurate[0] != 0x0) {
                strcpy(g_Options.models.value->arr[cfgindex].sounds[soundcount].name, sound_single_shot);
                g_Options.models.value->arr[cfgindex].sounds[soundcount].hash = GenerateSoundEntryHash(sound_single_shot);
                soundcount++;
            }

            char* sound_special1 = (char*)kv->GetString("sound_special1");
            if (sound_special1[0] != 0x0) {
                strcpy(g_Options.models.value->arr[cfgindex].sounds[soundcount].name, sound_special1);
                g_Options.models.value->arr[cfgindex].sounds[soundcount].hash = GenerateSoundEntryHash(sound_special1);
                soundcount++;
            }
        }
        g_Options.models.value->arr[cfgindex].soundcount = soundcount;
    }



    c_vpk_archive pak01_archive;
    if (pak01_archive.load("csgo/pak01_dir.vpk"))
        for (auto const& pair : pak01_archive.files) 
            if (!pair.first.find("materials/panorama/images/icons/equipment"))
                opt.killfeedicons.push_back(pair.first.substr(42, pair.first.length() - 46));
        

    g_Options.weapons.value->arr[0].modeldefindex = knives[0].defindex;
    strcpy(g_Options.weapons.value->arr[0].model, knives[0].viewmodel.c_str());
    strcpy(g_Options.weapons.value->arr[0].worldmodel, knives[0].worldmodel.c_str());
    g_Options.weapons.value->arr[0].modelactive = 1;

    g_Options.weapons.value->arr[1].modeldefindex = gloves[0].defindex;
    strcpy(g_Options.weapons.value->arr[1].model, gloves[0].viewmodel.c_str());
    strcpy(g_Options.weapons.value->arr[1].worldmodel, gloves[0].worldmodel.c_str());
    g_Options.weapons.value->arr[1].modelactive = 1;

    g_Options.weapons.value->arr[2].modeldefindex = agents[0].defindex;
    strcpy(g_Options.weapons.value->arr[2].model, agents[0].viewmodel.c_str()); 
    g_Options.weapons.value->arr[2].modelactive = 1;

    g_Options.weapons.value->arr[3].modelcount = 1;
    g_Options.weapons.value->arr[3].modeldefindex = agents[1].defindex;
    strcpy(g_Options.weapons.value->arr[3].model, agents[1].viewmodel.c_str()); 
    g_Options.weapons.value->arr[3].modelactive = 1;

    g_Options.weapons.value->arr[4].modeldefindex = medals[0].defindex;
    g_Options.weapons.value->arr[4].modelactive = 1;

    g_Options.weapons.value->arr[5].modeldefindex = musickits[0].defindex;
    g_Options.weapons.value->arr[5].modelactive = 1;

    g_Options.models.value->arr[2].findMDLmethode = 1;

}
