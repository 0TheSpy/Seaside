#ifndef PLAYERINV
#define PLAYERINV
#pragma once

#include "PatternScan.hpp"
#include "Interfaces.hpp"

#include "utlvector.h"
#include "stdint.h" 
#include <vector>

#include "icliententity.h" 



enum ItemQuality
{
    ITEM_QUALITY_DEFAULT,
    ITEM_QUALITY_GENUINE,
    ITEM_QUALITY_VINTAGE,
    ITEM_QUALITY_UNUSUAL,
    ITEM_QUALITY_SKIN,
    ITEM_QUALITY_COMMUNITY,
    ITEM_QUALITY_DEVELOPER,
    ITEM_QUALITY_SELFMADE,
    ITEM_QUALITY_CUSTOMIZED,
    ITEM_QUALITY_STRANGE,
    ITEM_QUALITY_COMPLETED,
    ITEM_QUALITY_UNK2,
    ITEM_QUALITY_TOURNAMENT
};

enum ItemRarity
{
    ITEM_RARITY_DEFAULT,
    ITEM_RARITY_COMMON,
    ITEM_RARITY_UNCOMMON,
    ITEM_RARITY_RARE,
    ITEM_RARITY_MYTHICAL,
    ITEM_RARITY_LEGENDARY,
    ITEM_RARITY_ANCIENT,
    ITEM_RARITY_IMMORTAL
};

class CEconItem
{
    unsigned short* GetEconItemData();
    void UpdateEquippedState(unsigned int state);
public:
    uint32_t* GetInventory();
    uint32_t* GetAccountID();
    uint16_t* GetDefIndex();
    uint64_t* GetItemID();
    uint64_t* GetOriginalID();
    unsigned char* GetFlags();
    void SetQuality(ItemQuality quality);
    void SetRarity(ItemRarity rarity);
    void SetOrigin(int origin);
    void SetLevel(int level);
    void SetInUse(bool in_use);
    void SetCustomName(const char* name);
    void SetCustomDesc(const char* name);
    void SetPaintSeed(float seed);
    void SetPaintKit(float kit);
    void SetPaintWear(float wear);
    void SetStatTrak(int val);
    void AddSticker(int index, int kit, float wear, float scale, float rotation);

    template<typename TYPE>
    void SetAttributeValue(int index, TYPE val);
    
};

class C_EconItemDefinition
{
public:
    int get_equipped_position();
    const char* get_world_model_name();
};

class C_EconItemView
{
private:
    using str_32 = char[32];
public:
    int32_t m_bInitialized;
    int16_t m_iItemDefinitionIndex;
    int32_t m_iEntityLevel;
    int32_t m_iAccountID;
    int32_t m_iItemIDLow;
    int32_t m_iItemIDHigh;
    int32_t m_iEntityQuality;
    str_32 m_iCustomName;

    CUtlVector<IRefCounted*>& m_CustomMaterials();
    CUtlVector<IRefCounted*>& m_VisualsDataProcessors();

    C_EconItemDefinition* get_static_data();
    CEconItem* get_soc_data();
};


class CSharedObjectTypeCache
{
public:
    void AddObject(void* obj);
    void RemoveObject(void* obj);
    std::vector<CEconItem*> GetEconItems();
};


class CPlayerInventory
{
public:
    void RemoveItem(uint64_t ID);
    void RemoveItem(CEconItem* item);
    void ClearInventory();
    CSharedObjectTypeCache* GetBaseTypeCache();
    uint32_t GetSteamID();
    CUtlVector<C_EconItemView*>* GetInventoryItems();
    bool AddEconItem(CEconItem* item, int a3, int a4, char a5);
};


class CSInventoryManager
{
public:
    CPlayerInventory* GetLocalPlayerInventory();
};


uintptr_t GetItemSchema();
CEconItem* CreateEconItem();




#endif