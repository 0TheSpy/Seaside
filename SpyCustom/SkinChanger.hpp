#ifndef SKINCHANGER
#define SKINCHANGER
#pragma once

#include <vector>
#include <string>

#include "sdk/GameEventManager.h"
#include "VMT.hpp"
#include "Interfaces.hpp"
#include "Options.hpp" 
#include "sdk/c_baseentity.h"
#include "PatternScan.hpp"

extern VMTHook* GameEventManagerHook;
extern  VMTHook* ClientHook;

#include "NetVarManager.hpp"  

extern bool needupdate;

#include "vmt_smart_hook.hpp"

 

struct iitem
{
    iitem(int defindex1, std::string name1, std::string viewmodel1, std::string worldmodel1)
    {
        defindex = defindex1;
        name = name1;
        viewmodel = viewmodel1;
        worldmodel = worldmodel1;
        Preview = std::to_string(defindex1).append(": ").append(name1);
    }
    int defindex;
    std::string name;
    std::string viewmodel;
    std::string worldmodel;
    std::string Preview;
    bool isSelected;
};


/*
struct hud_weapons_t
{
    std::int32_t* get_weapon_count()
    {
        return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
    }
};
*/

struct String_t
{
    char* szBuffer;	 
    int nCapacity;	 
    int _unknown;	 
    int nLength;	 
}; 

class CEconItemRarityDefinition
{
public:
    int id;
    int color;
    String_t name;
    String_t loc_key;
    String_t loc_key_weapon;
    String_t loot_list;
    String_t recycle_list;
    String_t drop_sound;
    String_t endmatchitemrevealrarity;
    String_t pointertosomething3;
    void* pointertosomething4;
    void* pointertosomething5;
    char dumb10[8];
};
 
class CStickerKit
{
public:
    int id;
    int item_rarity;
    String_t name;
    String_t description;
    String_t item_name;
    String_t material_name;
    String_t image_inventory;
    int tournament_event_id;
    int tournament_team_id;
    int tournament_player_id;
    bool is_custom_sticker_material;
    float rotate_end;
    float rotate_start;
    float scale_min;
    float scale_max;
    float wear_min;
    float wear_max;
    String_t image_inventory2;
    String_t image_inventory_large;
    std::uint32_t pad0[4];
};


class CPaintKit
{
public:
    int nID;                        
    String_t sName;                 
    String_t sDescriptionString;    
    String_t sDescriptionTag;       
    String_t pad;                   
    String_t pattern;               
    String_t pad1;                  
    String_t sLogoMaterial;         
    int bBaseDiffuseOverride;                   
    int rarity;                     
    int nStyle;                     
    int color1;
    int color2;
    int color3;
    int color4;
    int logoColor1;
    int logoColor2;
    int logoColor3;
    int logoColor4;
    float flWearDefault;
    float flWearRemapMin;
    float flWearRemapMax;
    char nFixedSeed;
    char uchPhongExponent;
    char uchPhongAlbedoBoost;
    char uchPhongIntensity; 
    float flPatternScale;
    float flPatternOffsetXStart;
    float flPatternOffsetXEnd;
    float flPatternOffsetYStart;
    float flPatternOffsetYEnd;
    float flPatternRotateStart;
    float flPatternRotateEnd;
    float flLogoScale;
    float flLogoOffsetX;
    float flLogoOffsetY;
    float flLogoRotation;
    int bIgnoreWeaponSizeScale;
    int nViewModelExponentOverrideSize;
    int bOnlyFirstMaterial;
    float pearlescent;
    int sVmtPath[4];
    int kvVmtOverrides;
};  




struct EconItemQualityDefinition {
    int id;
    const char* name;
    unsigned weight;
    bool explicitMatchesOnly;
    bool canSupportSet;
    const char* hexColor;
};

extern std::vector<iitem> rarities;
extern std::vector<iitem> knives;
extern std::vector<iitem> gloves;
extern std::vector<iitem> agents;
extern std::vector<iitem> medals;
extern std::vector<iitem> musickits;
extern std::vector <iitem> qualities;

static inline auto is_knife(const int di) -> bool
{
    for (short i = 0; i < knives.size(); i++)
        if (knives[i].defindex == di) return true;
    return false;
}

bool __fastcall hkFireEventClientSide(void* thisptr, void* edx, IGameEvent* event);
void __fastcall hkFrameStageNotify(IBaseClientDLL* thisptr, void* edx, ClientFrameStage_t stage);




class CCStrike15ItemSchema;
class CCStrike15ItemSystem
{
public:
    virtual CCStrike15ItemSchema* GetItemSchemaInterface() = 0;
};


template <typename Key, typename Value>
struct Node_t
{
    int nPreviousId;		
    int nNextId;			
    void* parent;		
    int type;			
    Key iPaintKitId;		
    Value value;			
};

template <typename Key, typename Value>
struct Head_t
{
    Node_t<Key, Value>* pMemory;		
    int nAllocationCount;			
    int nGrowSize;					
    int nStartElement;				
    int nNextAvailable;				
    int _unknown;					
    int nLastElement;				
}; 


class CEconItemDefinition
{
public:


    VIRTUAL_METHOD(int, getWeaponId, 0, (), (this))
    VIRTUAL_METHOD(const char*, getItemBaseName, 2, (), (this))
    VIRTUAL_METHOD(const char*, getItemTypeName, 3, (), (this))
    VIRTUAL_METHOD(const char*, getPlayerDisplayModel, 6, (), (this))
    VIRTUAL_METHOD(const char*, getWorldDisplayModel, 7, (), (this))
    VIRTUAL_METHOD(std::uint8_t, getRarity, 12, (), (this))

    char pad_0x0000[0x8]; 
    int32_t m_iItemDefinitionIndex;  
    char pad_0x000C[0x40]; 
    char* m_szHudName;  
    char pad_0x0050[0x4]; 
    char* m_szWeaponType;  
    char pad_0x0058[0x4]; 
    char* m_szWeaponDescription;  
    char pad_0x0060[0x34]; 
    char* m_szViewModel;  
    char pad_0x0098[0x4]; 
    char* m_szWorldModel;  
    char* m_szWorldModelDropped;  
    char pad_0x001[0x58];
    char* stickersdecalmdl;
    char pad_0x002[0x0C];
    char* stickersdecalmdl1;
    char* inventoryicon;
    char pad_0x003[0xA4]; 
    char* m_szWeaponName1;
    char pad_0x004[0x14];
    char* m_szWeaponName;
    char pad_0x005[0x8];
    char* m_szWeaponName3;
    char pad_0x01C4[0x27C];
};


void InitSkinChanger();

 


template <typename T>
struct UtlMemory {
    T& operator[](int i) const noexcept { return memory[i]; };

    T* memory;
    int allocationCount;
    int growSize;
};

template <typename Key, typename Value>
struct UtlMap {
    auto begin() const noexcept { return memory.memory; }
    auto end() const noexcept { return memory.memory + numElements; }

    int find(Key key) const noexcept
    {
        auto curr = root;

        while (curr != -1) {
            const auto el = memory[curr];

            if (el.iPaintKitId < key)
                curr = el.nNextId;
            else if (el.iPaintKitId > key)
                curr = el.nPreviousId;
            else
                break;
        }
        return curr;
    }
    void* lessFunc;
    UtlMemory<Node_t<Key, Value>> memory;
    int root;
    int numElements;
    int firstFree;
    int lastAlloc;
    Node_t<Key, Value>* elements;
};

struct AlternateIconData {
    String_t simpleName;
    String_t largeSimpleName;
    String_t iconURLSmall;
    String_t iconURLLarge;
    PAD(28)
};



template <typename T>
class UtlVector {
public:
    constexpr T& operator[](int i) noexcept { return memory[i]; };
    constexpr const T& operator[](int i) const noexcept { return memory[i]; };

    T* memory; 
    int allocationCount;
    int growSize;
    int size;
    T* elements;
};


struct ItemListEntry {
    int itemDef;
    int paintKit;
    PAD(20)

    auto weaponId() const noexcept
    {
        return itemDef;
    }
};

class EconLootListDefinition {
public:
    VIRTUAL_METHOD(const char*, getName, 0, (), (this))
    VIRTUAL_METHOD(const UtlVector<ItemListEntry>&, getLootListContents, 1, (), (this))
};

class EconItemSetDefinition {
public:
    VIRTUAL_METHOD(const char*, getLocKey, 1, (), (this))
    VIRTUAL_METHOD(int, getItemCount, 4, (), (this))
    VIRTUAL_METHOD(int, getItemDef, 5, (int index), (this, index))
    VIRTUAL_METHOD(int, getItemPaintKit, 6, (int index), (this, index))
};




class CEconColorDefinition
{
public:
    int xz;
    int xz2;
};

class CEconGraffitiTintDefinition
{
public:
    int xz;
    int xz2;
};

class CEconMusicDefinition
{
public:
    int id;
    char* name;
    char* desc;
    char* model;
    char* music;
};

template <typename T>
class MusicVector {
public:
    constexpr T& operator[](int i) noexcept { return memory[i]; };
    constexpr const T& operator[](int i) const noexcept { return memory[i]; };

    T* memory;
    PAD(0xC)
    int itemcount;
};

class ItemSchema { 
public:
    PAD(0x64) 
        UtlMap<int, CEconItemRarityDefinition> rars;
        UtlMap<int, EconItemQualityDefinition> qualities;
    PAD(0x48)
        UtlMap<int, CEconItemDefinition*> itemsSorted;
    PAD(0x104)
        UtlMap<std::uint64_t, AlternateIconData> alternateIcons;
    PAD(0x48)
    UtlMap<int, CPaintKit*> paintKits;
    UtlMap<int, CStickerKit*> stickerKits;
    UtlMap<const char*, CStickerKit*> sticker_kits_namekey;
    int32_t pad6[24];
    UtlMap<const char*, KeyValues*> prefabs;
    UtlVector<CEconColorDefinition*> colors;
    UtlVector<CEconGraffitiTintDefinition*> graffiti_tints;
    int32_t pad7[20];
    MusicVector<CEconMusicDefinition*> music_definitions;

    VIRTUAL_METHOD(CEconItemDefinition*, getItemDefinitionInterface, 4, (int id), (this, id))
        VIRTUAL_METHOD(int, getItemSetCount, 28, (), (this))
        VIRTUAL_METHOD(EconItemSetDefinition*, getItemSet, 29, (int index), (this, index))
        VIRTUAL_METHOD(EconLootListDefinition*, getLootList, 32, (int index), (this, index))
        VIRTUAL_METHOD(int, getLootListCount, 34, (), (this))
        VIRTUAL_METHOD(CEconItemDefinition*, getItemDefinitionByName, 42, (const char* name), (this, name))
};


class ItemSystem {
public:
    VIRTUAL_METHOD(ItemSchema*, getItemSchema, 0, (), (this))
};
 
extern std::add_pointer_t<ItemSystem* __cdecl()> itemSystem;

#endif
