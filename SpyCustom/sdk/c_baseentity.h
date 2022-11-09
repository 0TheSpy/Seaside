#ifndef CBASEENT
#define CBASEENT

#pragma once

#include "icliententity.h"
#include "NetVarManager.hpp"
#include "PatternScan.hpp"
#include "netvars.hpp"


namespace VirtualMethod
{
	template <typename T, std::size_t Idx, typename ...Args>
	constexpr T call(void* classBase, Args... args) noexcept
	{
		return (*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[Idx](classBase, args...);
	}
}

#define VIRTUAL_METHOD(returnType, name, idx, args, argsRaw) \
returnType name args noexcept \
{ \
    return VirtualMethod::call<returnType, idx>argsRaw; \
}

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)
#define PAD(size) \
private: \
    std::byte PAD_NAME(__COUNTER__) [size]; \
public:


	class C_BaseEntity : public IClientEntity
	{
	public:

		NETVAR_OFFSET(GetIndex, "CBaseEntity", "m_bIsAutoaimTarget", +0x4, int);
		NETVAR(GetModelIndex, "CBaseEntity", "m_nModelIndex", unsigned);

		NETVAR(Ragdoll, "CCSPlayer", "m_hRagdoll", int);  

		NETVAR2(GetViewmodelArmConfig, "DT_CSPlayer", "m_pViewmodelArmConfig", PlayerViewmodelArmConfig*);
		NETVAR2(GetArmsModel, "DT_CSPlayer", "m_szArmsModel", char*); 

		

		void SetModelIndex(const int index)
		{
			getvfunc<void(__thiscall*)(C_BaseEntity*, int)>(this, 75)(this, index);
		}

		NETVAR_OFFSET(m_dwBoneMatrix, "CBaseAnimating", "m_nForceBone", +0x1C, uintptr_t); 

		Vector GetBonePosition(int iBone) {
			uintptr_t bm = m_dwBoneMatrix();
			matrix3x4_t boneMatrix =  *(matrix3x4_t*) (bm +iBone * sizeof(matrix3x4_t)) ;
			return Vector(boneMatrix[0][3], boneMatrix[1][3], boneMatrix[2][3]);
		}

		NETVAR(body, "CBaseAnimating", "m_nBody", int);
		NETVAR(GetModelScale, "CBaseAnimating", "m_flModelScale", float);
		NETVAR(GetScaleType, "CBaseAnimating", "m_ScaleType", float);
		NETVAR(GetFrozen, "CBaseAnimating", "m_flFrozen", float);
		  
		NETVAR(GetMins, "CBaseEntity", "m_vecMins", Vector); 
		NETVAR(GetMaxs, "CBaseEntity", "m_vecMaxs", Vector);  

		NETVAR2(IsScoped, "DT_CSPlayer", "m_bIsScoped", bool);
		

		int GetSequenceActivity(int sequence, studiohdr_t* hdr)
		{
			static auto getSequenceActivity = (DWORD)(FindPatternV2("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));            
			static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);
			return GetSequenceActivity(this, hdr, sequence);
		}

		char* GetSequenceActivity(int sequence)
		{
			static auto getSequenceActivity = (DWORD)(FindPatternV2("client.dll", "55 8B EC 83 7D 08 FF 56 8B F1"));
			static auto GetSequenceActivity = reinterpret_cast<char* (__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);
			return GetSequenceActivity(this, 0, sequence);
		}

		void SendViewModelMatchingSequence(int sequence)
		{
			printf("Set new sequence %d\n",sequence);
			typedef  void(__thiscall* OriginalFn)(void*, int);
			return getvfunc<OriginalFn>(this, 246)(this, sequence);
		}

	};

	class C_BaseCombatCharacter : public C_BaseEntity
	{
	public:
		NETVAR(GetWeapons, "CBaseCombatCharacter", "m_hMyWeapons", std::array<CBaseHandle, MAX_WEAPONS>);
		PNETVAR(GetWearables, "CBaseCombatCharacter", "m_hMyWearables", CBaseHandle); 
	};

	class C_BasePlayer : public C_BaseCombatCharacter
	{
	public:
		NETVAR(GetLifeState, "CBasePlayer", "m_lifeState", int);
		NETVAR(GetViewModel, "CBasePlayer", "m_hViewModel[0]", CBaseHandle);

		NETVAR(GetOrigin, "CBaseEntity", "m_vecOrigin", Vector);
		NETVAR(GetAngles, "CBaseEntity", "m_angRotation", Vector);
		NETVAR(GetHealth, "CBasePlayer", "m_iHealth", int);
		NETVAR(GetViewOffset, "CBasePlayer", "m_vecViewOffset[0]", Vector);
		NETVAR(GetTeam, "CBaseEntity", "m_iTeamNum", int);

		NETVAR2(GetFlags, "DT_CSPlayer", "m_fFlags", int);
		NETVAR_OFFSET(GetMoveType, "CBaseEntity", "m_nRenderMode", +0x1, int);
		NETVAR2(GetNightvision, "DT_CSPlayer", "m_bNightVisionOn", bool);
		NETVAR2(GetNightvisionAlpha, "DT_CSPlayer", "m_flNightVisionAlpha", float);
	}; 

	class C_BaseCombatWeapon : public C_BaseEntity
	{
	public:
		NETVAR(GetViewModelIndex, "CBaseCombatWeapon", "m_iViewModelIndex", int);
		NETVAR(GetWorldModelIndex, "CBaseCombatWeapon", "m_iWorldModelIndex", int);
		NETVAR(GetWorldDroppedModelIndex, "CBaseCombatWeapon", "m_iWorldDroppedModelIndex", int);
		NETVAR(GetWeaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel", CBaseHandle);
	};

	class C_BaseAttributableItem : public C_BaseCombatWeapon
	{
	public:
		NETVAR(GetAccountID, "CBaseAttributableItem", "m_iAccountID", int);
		NETVAR(GetItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
		NETVAR(GetItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh", int);
		NETVAR(GetEntityQuality, "CBaseAttributableItem", "m_iEntityQuality", int);
		NETVAR(GetCustomName, "CBaseAttributableItem", "m_szCustomName", char[32]);
		NETVAR(GetFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned);
		NETVAR(GetFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned);
		NETVAR(GetFallbackWear, "CBaseAttributableItem", "m_flFallbackWear", float);
		NETVAR(GetFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned);
		NETVAR(initialized, "CBaseAttributableItem", "m_bInitialized", bool)
	};

	class C_BaseViewModel : public C_BaseEntity
	{
	public:
		NETVAR(GetOwner, "CBaseViewModel", "m_hOwner", CBaseHandle);
		NETVAR(GetWeapon, "CBaseViewModel", "m_hWeapon", CBaseHandle);
		NETPROP(GetSequenceProp, "CBaseViewModel", "m_nSequence");

		NETVAR(GetSequence, "CBaseViewModel", "m_nSequence", int);
	};

	class C_PlayerResource
	{
	public:
		NETPROP(GetTeamProp, "CPlayerResource", "m_iTeam");
	};

	class C_CS_PlayerResource : public C_PlayerResource
	{
	public:
		NETVAR(GetRanks, "CCSPlayerResource", "m_iCompetitiveRanking", int[MAX_PLAYERS]);
		NETVAR(GetRankType, "CCSPlayerResource", "m_iCompetitiveRankType", int[MAX_PLAYERS]);
		NETVAR(GetWins, "CCSPlayerResource", "m_iCompetitiveWins", int[MAX_PLAYERS]);
		NETVAR(GetLevel, "CCSPlayerResource", "m_nPersonaDataPublicLevel", int[MAX_PLAYERS]);
		NETVAR(GetComLeader, "CCSPlayerResource", "m_nPersonaDataPublicCommendsLeader", int[MAX_PLAYERS]);
		NETVAR(GetComTeacher, "CCSPlayerResource", "m_nPersonaDataPublicCommendsTeacher", int[MAX_PLAYERS]);
		NETVAR(GetComFriendly, "CCSPlayerResource", "m_nPersonaDataPublicCommendsFriendly", int[MAX_PLAYERS]);

		NETVAR(GetClanTags, "CCSPlayerResource", "m_szClan", char[MAX_PLAYERS][32]);
		NETVAR(GetCoins, "CCSPlayerResource", "m_nActiveCoinRank", unsigned[MAX_PLAYERS]);
		NETVAR(GetMusicKits, "CCSPlayerResource", "m_nMusicID", unsigned[MAX_PLAYERS]);

		NETVAR2(GetKills, "DT_CSPlayerResource", "m_iKills", unsigned[MAX_PLAYERS]);
		NETVAR2(GetAssists, "DT_CSPlayerResource", "m_iAssists", unsigned[MAX_PLAYERS]);
		NETVAR2(GetDeaths, "DT_CSPlayerResource", "m_iDeaths", unsigned[MAX_PLAYERS]);

		NETVAR(GetMVPs, "CCSPlayerResource", "m_iMVPs", int[MAX_PLAYERS]);
		NETVAR(GetScore, "CCSPlayerResource", "m_iScore", int[MAX_PLAYERS]);

		NETVAR(IsVIP, "CCSPlayerResource", "m_iPlayerVIP", int[MAX_PLAYERS]);

	};

	class CBaseWeaponWorldModel : public C_BaseEntity
	{
	};
	
	class C_Precipitation : public C_BaseEntity
	{ 
	public:
		NETVAR2(GetPrecipitationType, "DT_Precipitation", "m_nPrecipType", PrecipitationType_t); 
	};

#endif
