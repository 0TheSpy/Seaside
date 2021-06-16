#include "PlayerInventory.hpp"


void CSharedObjectTypeCache::AddObject(void* obj)
{
    typedef void(__thiscall* tOriginal)(void*, void*);
    getvfunc<tOriginal>(this, 1)(this, obj);
}

void CSharedObjectTypeCache::RemoveObject(void* obj)
{
    typedef void(__thiscall* tOriginal)(void*, void*);
    getvfunc<tOriginal>(this, 3)(this, obj);
}

std::vector<CEconItem*> CSharedObjectTypeCache::GetEconItems()
{
    std::vector<CEconItem*> ret;

    auto size = *reinterpret_cast<size_t*>(this + 0x18);

    auto data = *reinterpret_cast<uintptr_t**>(this + 0x4);

    for (size_t i = 0; i < size; i++)
        ret.push_back(reinterpret_cast<CEconItem*>(data[i]));

    return ret;
}


template<typename TYPE>
void CEconItem::SetAttributeValue(int index, TYPE val)
{
    auto v15 = (DWORD*)GetItemSchema();
    auto v16 = *(DWORD*)(v15[72] + 4 * index);

    static auto fnSetDynamicAttributeValue
        = reinterpret_cast<int(__thiscall*)(CEconItem*, DWORD, void*)>(
            FindPatternV2("client.dll", "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A 00")
            );

    fnSetDynamicAttributeValue(this, v16, &val);
}




int C_EconItemDefinition::get_equipped_position()
{
    return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x24C);
}

const char* C_EconItemDefinition::get_world_model_name()
{
    return *reinterpret_cast<const char**>(uintptr_t(this) + 0x9C);
}





uint32_t* CEconItem::GetAccountID()
{
    return reinterpret_cast<uint32_t*>(this + 0x1C);
}
uint64_t* CEconItem::GetItemID()
{
    return reinterpret_cast<uint64_t*>(this + 0x8);
}

uint64_t* CEconItem::GetOriginalID()
{
    return reinterpret_cast<uint64_t*>(this + 0x10);
}

uint16_t* CEconItem::GetDefIndex()
{
    return reinterpret_cast<uint16_t*>(this + 0x24);
}

uint32_t* CEconItem::GetInventory()
{
    return reinterpret_cast<uint32_t*>(this + 0x20);
}

unsigned char* CEconItem::GetFlags()
{
    return reinterpret_cast<unsigned char*>(this + 0x30);
}

unsigned short* CEconItem::GetEconItemData()
{
    return reinterpret_cast<unsigned short*>(this + 0x26);
}



C_EconItemDefinition* C_EconItemView::get_static_data()
{
    static auto fn = reinterpret_cast<C_EconItemDefinition * (__thiscall*)(void*)>(FindPatternV2("client.dll", "55 8B EC 51 53 8B D9 8B ? ? ? ? ? 56 57 8B ? ? ? ? ? 85 FF 74 16"));

    return fn(this);
}


CEconItem* C_EconItemView::get_soc_data()
{
    static auto fn = reinterpret_cast<CEconItem * (__thiscall*)(C_EconItemView*)>(FindPatternV2("client.dll", "55 8B EC 83 E4 F0 83 EC 18 56 8B F1 57 8B 86"));

    return fn(this);
}



void CPlayerInventory::RemoveItem(uint64_t ID)
{
    static auto fnRemoveItem
        = reinterpret_cast<int(__thiscall*)(void*, int64_t)>(
            FindPatternV2("client.dll", "55 8B EC 83 E4 F8 56 57 FF 75 0C 8B F1")
            );

    fnRemoveItem(this, ID);
}

CSharedObjectTypeCache* CPlayerInventory::GetBaseTypeCache()
{
    static auto fnGCSDK_CGCClient_FindSOCache
        = reinterpret_cast<uintptr_t(__thiscall*)(uintptr_t, uint64_t, uint64_t, bool)>(
            FindPatternV2("client.dll", "55 8B EC 83 E4 F8 83 EC 1C 0F 10 45 08")
            );

    static auto fnGCSDK_CSharedObjectCache_CreateBaseTypeCache
        = reinterpret_cast<CSharedObjectTypeCache * (__thiscall*)(uintptr_t, int)>(
            FindPatternV2("client.dll", "55 8B EC 51 53 56 8B D9 8D 45 08")
            );

    static auto g_GCClientSystem = **reinterpret_cast<uintptr_t**>(FindPatternV2("client.dll", "8B 0D ? ? ? ? 6A 00 83 EC 10") + 0x2);
    auto SOCahce = fnGCSDK_CGCClient_FindSOCache(g_GCClientSystem + 0x60, *reinterpret_cast<uint64_t*>(this + 0x8), *reinterpret_cast<uint64_t*>(this + 0x10), 0);

    return fnGCSDK_CSharedObjectCache_CreateBaseTypeCache(SOCahce, 1);
}

void CPlayerInventory::RemoveItem(CEconItem* item)
{
    RemoveItem(*item->GetItemID());
    GetBaseTypeCache()->RemoveObject(item);
}

void CPlayerInventory::ClearInventory()
{
    auto BaseTypeCache = this->GetBaseTypeCache();
    auto items = BaseTypeCache->GetEconItems();
    for (auto item : items)
    {
        RemoveItem(*item->GetItemID());
        BaseTypeCache->RemoveObject(item);
    }
}

uint32_t CPlayerInventory::GetSteamID()
{
    return *reinterpret_cast<uint32_t*>(this + 0x8);
}

CUtlVector< C_EconItemView* >* CPlayerInventory::GetInventoryItems()
{
    return reinterpret_cast<CUtlVector<C_EconItemView*>*>(this + 0x2C);
}

bool CPlayerInventory::AddEconItem(CEconItem* item, int a3, int a4, char a5)
{
    static auto fnAddEconItem
        = reinterpret_cast<C_EconItemView * (__thiscall*)(void*, CEconItem*, int, int, char)>(
            FindPatternV2("client.dll", "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 14 53 56 57 8B F9 8B 08")
            );

    GetBaseTypeCache()->AddObject(item);


    auto ret = fnAddEconItem(this, item, a3, a4, a5);

    return ret;
}


CPlayerInventory* CSInventoryManager::GetLocalPlayerInventory()  
{
    static auto local_inventory_offset = *reinterpret_cast<uintptr_t*>(FindPatternV2("client.dll", "8B 8B ? ? ? ? E8 ? ? ? ? 89 44 24 18") + 0x2);
    return *reinterpret_cast<CPlayerInventory**>(this + local_inventory_offset);
}

CEconItem* CreateEconItem()
{
    static auto fnCreateSharedObjectSubclass_EconItem_
        = reinterpret_cast<CEconItem * (__stdcall*)()>(
            *reinterpret_cast<uintptr_t*>(FindPatternV2("client.dll", "C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4") + 3)
            );
    return fnCreateSharedObjectSubclass_EconItem_();
}
uintptr_t GetItemSchema()
{
    static auto fnGetItemSchema
        = reinterpret_cast<uintptr_t(__stdcall*)()>(
            FindPatternV2("client.dll", "A1 ? ? ? ? 85 C0 75 53")
            );
    return fnGetItemSchema();
}
