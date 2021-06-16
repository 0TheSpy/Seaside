#pragma once
#include "ProtoParse.h"
#include "ProtobuffMessages.h"
#include <ctime>

#include "Options.hpp"

struct wskin
{
	int wId;
	int paintkit;
};

#define START_MUSICKIT_INDEX 1500000
#define START_ITEM_INDEX     2000000

static void clear_equip_state(CMsgClientWelcome::SubscribedType& object);
static void apply_medals(CMsgClientWelcome::SubscribedType& object);
static void apply_music_kits(CMsgClientWelcome::SubscribedType& object);

static void add_item(CMsgClientWelcome::SubscribedType& object, int index, ItemDefinitionIndex itemIndex, int quality, int rarity, int paintKit, int seed, float wear, std::string name, std::string desc, sticker stickers[5], int stattrak, int flag);

static int GetAvailableClassID(int definition_index);
static int GetSlotID(int definition_index);
static std::vector<uint32_t> music_kits = { 3, 4, 5, 6, 7, 8 };


template<typename T>
inline std::string get_4bytes(T value)
{
	return std::string{ reinterpret_cast<const char*>(reinterpret_cast<void*>(&value)), 4 };
}

template<typename T>
inline CSOEconItemAttribute make_econ_item_attribute(int def_index, T value)
{
	CSOEconItemAttribute attribute;
	attribute.def_index().set(def_index);
	attribute.value_bytes().set(get_4bytes(value));
	return attribute;
}

inline CSOEconItemEquipped make_equipped_state(int team, int slot)
{
	CSOEconItemEquipped equipped_state;
	equipped_state.new_class().set(team);
	equipped_state.new_slot().set(slot);
	return equipped_state;
}

static std::string inventory_changer(void* pubDest, uint32_t* pcubMsgSize) {
	CMsgClientWelcome msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8);
	if (!msg.outofdate_subscribed_caches().has())
		return msg.serialize();

	auto cache = msg.outofdate_subscribed_caches().get();

	static auto fix_null_inventory = [&cache]()
	{
		auto objects = cache.objects().get_all();
		auto it = std::find_if(objects.begin(), objects.end(), [](decltype(objects.front()) o)
		{
			return o.type_id().has() && o.type_id().get() == 1;
		});

		if (it == objects.end())
		{
			CMsgClientWelcome::SubscribedType null_object;
			null_object.type_id().set(1);
			cache.objects().add(null_object);
		}
	};

	fix_null_inventory();

	auto objects = cache.objects().get_all();
	for (size_t i = 0; i < objects.size(); i++) {
		auto object = objects[i];

		if (!object.type_id().has())
			continue;

		switch (object.type_id().get())
		{
		case 1:  
		{
			if (true) 
				object.object_data().clear();

			int indexcount = 0;

			for (int i = 0; i < g_Options.weapons.value->weaponzcount; i++)
				if (g_Options.weapons.value->arr[i].active) {
					indexcount++;
					add_item(object, indexcount,
						(ItemDefinitionIndex)g_Options.weapons.value->arr[i].modeldefindex,
						g_Options.weapons.value->arr[i].quality,
						g_Options.weapons.value->arr[i].rarity,
						g_Options.weapons.value->arr[i].skinid,
						g_Options.weapons.value->arr[i].seed,
						g_Options.weapons.value->arr[i].wear,
						g_Options.weapons.value->arr[i].nametag,
						g_Options.weapons.value->arr[i].desctag,
						g_Options.weapons.value->arr[i].stickers,
						g_Options.weapons.value->arr[i].stattrak,
						i);
				}

			cache.objects().set(object, i);
		}
		break;
		}
	}
	msg.outofdate_subscribed_caches().set(cache);

	return msg.serialize();
}

static bool inventory_changer_presend(void* pubData, uint32_t& cubData)
{
	CMsgAdjustItemEquippedState msg((void*)((DWORD)pubData + 8), cubData - 8);
	if (msg.item_id().has() && (msg.new_class().get() == 0 || msg.new_slot().get() == 54))
	{
		auto ItemIndex = msg.item_id().get() - START_MUSICKIT_INDEX;

		if (ItemIndex > 38 || ItemIndex < 3)
			return true;

		  msg.new_slot().get() == 0xFFFF ? 0 : ItemIndex - 2;

		return false;
	}
	if (!msg.item_id().has() || !msg.new_class().get() || !msg.new_slot().get())
		return true;

	return false;
}


static void clear_equip_state(CMsgClientWelcome::SubscribedType& object)
{
	auto object_data = object.object_data().get_all();
	for (size_t j = 0; j < object_data.size(); j++)
	{
		auto item = object_data[j];

		if (!item.equipped_state().has())
			continue;

		auto null_equipped_state = make_equipped_state(0, 0);

		auto equipped_state = item.equipped_state().get_all();
		for (size_t k = 0; k < equipped_state.size(); k++)
			item.equipped_state().set(null_equipped_state, k);

		object.object_data().set(item, j);
	}
}


static void add_item(CMsgClientWelcome::SubscribedType& object, int index, ItemDefinitionIndex itemIndex, int quality, int rarity, int paintKit, int seed, float wear, std::string name, std::string desc, sticker stickers[5], int stattrak, int flag)
{
	uint32_t steamid = iff.g_SteamUser->GetSteamID().GetAccountID();

	CSOEconItem item;
	item.id().set(START_ITEM_INDEX + itemIndex);
	item.account_id().set(steamid);
	if (flag == 5)
		item.def_index().set(1314);
	else item.def_index().set(itemIndex);
	item.inventory().set(START_ITEM_INDEX + index);
	item.origin().set(24);
	item.quantity().set(1);
	item.level().set(1);
	item.style().set(0);
	item.flags().set(0);
	item.in_use().set(true);
	item.original_id().set(0);
	item.rarity().set(rarity);
	item.quality().set(quality);

	if (name.size() > 0)
		item.custom_name().set(name);

	if (desc.size() > 0) 
		item.custom_desc().set(desc);

	if (flag > 5) {
		int avalTeam = GetAvailableClassID(itemIndex);
		if (avalTeam == TEAM_SPECTATOR || avalTeam == TEAM_TERRORIST) {
			item.equipped_state().add(make_equipped_state(TEAM_TERRORIST, GetSlotID(itemIndex)));
		}
		if (avalTeam == TEAM_SPECTATOR || avalTeam == TEAM_CT) {
			item.equipped_state().add(make_equipped_state(TEAM_CT, GetSlotID(itemIndex)));
		}
	}

	item.attribute().add(make_econ_item_attribute(6, float(paintKit)));
	item.attribute().add(make_econ_item_attribute(7, float(seed)));
	item.attribute().add(make_econ_item_attribute(8, float(wear)));


	for (int j = 0; j < 5; j++)
	{
		item.attribute().add(make_econ_item_attribute(113 + 4 * j, stickers[j].stickerid ));   
		item.attribute().add(make_econ_item_attribute(114 + 4 * j, stickers[j].wear));       
		item.attribute().add(make_econ_item_attribute(115 + 4 * j, stickers[j].scale));          
		item.attribute().add(make_econ_item_attribute(116 + 4 * j, stickers[j].rotation));          
	}

	if (stattrak != -1)
	{
		item.attribute().add(make_econ_item_attribute(80, stattrak)); 
		item.attribute().add(make_econ_item_attribute(81, 0));
	}

	if (flag == 0)  
	{
		item.equipped_state().add(make_equipped_state(TEAM_TERRORIST, 0));
		item.equipped_state().add(make_equipped_state(TEAM_CT, 0));
	}

	if (flag == 1)  
	{
		item.equipped_state().add(make_equipped_state(TEAM_TERRORIST, 41));
		item.equipped_state().add(make_equipped_state(TEAM_CT, 41));
	}

	if (flag == 2)    
		item.equipped_state().set(make_equipped_state(2, 38)); 

	if (flag == 3)    
		item.equipped_state().set(make_equipped_state(3, 38));


	if (flag == 4)  
		item.equipped_state().set(make_equipped_state(0, 55));


	if (flag == 5) 
	{
		item.attribute().add(make_econ_item_attribute(166, itemIndex));    
		item.inventory().set(START_MUSICKIT_INDEX + itemIndex);
		item.id().set(START_MUSICKIT_INDEX + itemIndex);
		item.equipped_state().set(make_equipped_state(0, 54)); 
	}

	object.object_data().add(item);
}

static int GetAvailableClassID(int definition_index)
{
	switch (definition_index)
	{
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_ELITE:
	case WEAPON_P250:
	case WEAPON_CZ75A:
	case WEAPON_DEAGLE:
	case WEAPON_REVOLVER:
	case WEAPON_MP7:
	case WEAPON_UMP45:
	case WEAPON_P90:
	case WEAPON_BIZON:
	case WEAPON_SSG08:
	case WEAPON_AWP:
	case WEAPON_NOVA:
	case WEAPON_XM1014:
	case WEAPON_M249:
	case WEAPON_NEGEV:
	case GLOVE_STUDDED_BLOODHOUND:
	case GLOVE_SPORTY:
	case GLOVE_SLICK:
	case GLOVE_LEATHER_WRAP:
	case GLOVE_MOTORCYCLE:
	case GLOVE_SPECIALIST:
		return TEAM_SPECTATOR;

	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_MAC10:
	case WEAPON_G3SG1:
	case WEAPON_TEC9:
	case WEAPON_GALILAR:
	case WEAPON_SG553:
	case WEAPON_SAWEDOFF:
	case WEAPON_C4:
		return TEAM_TERRORIST;

	case WEAPON_AUG:
	case WEAPON_FAMAS:
	case WEAPON_MAG7:
	case WEAPON_FIVESEVEN:
	case WEAPON_USP_SILENCER:
	case WEAPON_HKP2000:
	case WEAPON_MP9:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_M4A1:
	case WEAPON_SCAR20:
		return TEAM_CT;

	default:
		return TEAM_UNASSIGNED;
	}
}

static int GetSlotID(int definition_index)
{
	switch (definition_index)
	{
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
		return 0;
	case WEAPON_C4:
		return 1;
	case WEAPON_USP_SILENCER:
	case WEAPON_HKP2000:
	case WEAPON_GLOCK:
		return 2;
	case WEAPON_ELITE:
		return 3;
	case WEAPON_P250:
		return 4;
	case WEAPON_TEC9:
	case WEAPON_CZ75A:
	case WEAPON_FIVESEVEN:
		return 5;
	case WEAPON_DEAGLE:
	case WEAPON_REVOLVER:
		return 6;
	case WEAPON_MP9:
	case WEAPON_MAC10:
		return 8;
	case WEAPON_MP7:
		return 9;
	case WEAPON_UMP45:
		return 10;
	case WEAPON_P90:
		return 11;
	case WEAPON_BIZON:
		return 12;
	case WEAPON_FAMAS:
	case WEAPON_GALILAR:
		return 14;
	case WEAPON_M4A1_SILENCER:
	case WEAPON_M4A1:
	case WEAPON_AK47:
		return 15;
	case WEAPON_SSG08:
		return 16;
	case WEAPON_SG553:
	case WEAPON_AUG:
		return 17;
	case WEAPON_AWP:
		return 18;
	case WEAPON_G3SG1:
	case WEAPON_SCAR20:
		return 19;
	case WEAPON_NOVA:
		return 20;
	case WEAPON_XM1014:
		return 21;
	case WEAPON_SAWEDOFF:
	case WEAPON_MAG7:
		return 22;
	case WEAPON_M249:
		return 23;
	case WEAPON_NEGEV:
		return 24;
	case GLOVE_STUDDED_BLOODHOUND:
	case GLOVE_SPORTY:
	case GLOVE_SLICK:
	case GLOVE_LEATHER_WRAP:
	case GLOVE_MOTORCYCLE:
	case GLOVE_SPECIALIST:
		return 41;
	default:
		return -1;
	}
}