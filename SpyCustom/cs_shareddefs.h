#ifndef CS_SHAREDDEFS_H
#define CS_SHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif

#if !defined (_GAMECONSOLE) && !defined( GC_DLL )
#endif

#include "cs_achievementdefs.h"

#define PANEL_CHOOSE_CLASS			"choose_class"

#define CSTRIKE_VISUALS_DATA_BYTES 96

#define MAX_HOSTAGES				12
#define MAX_HOSTAGE_RESCUES			4
#define HOSTAGE_RULE_CAN_PICKUP		1

#define MAX_MATCH_STATS_ROUNDS 30
#define MATCH_STATS_TEAM_SWAP_ROUND	15

#define IRONSIGHT     
#define GRENADE_UNDERHAND_FEATURE_ENABLED     
#define GRENADE_DEFAULT_SIZE 2.0

#define CS_CONTROLLABLE_BOTS_ENABLED 1

#if !defined( CSTRIKE_DEMO_PRESSBUILD )
#define CSTRIKE_DEMO_PRESSBUILD		0
#endif

#if !defined( CSTRIKE_E3_BUILD )
#define CSTRIKE_E3_BUILD		0
#endif

#define CSTRIKE_DEFAULT_AVATAR		"avatar_default_64"
#define CSTRIKE_DEFAULT_T_AVATAR "avatar_default-t_64"
#define CSTRIKE_DEFAULT_CT_AVATAR "avatar_default_64"
extern const float CS_PLAYER_SPEED_RUN;
extern const float CS_PLAYER_SPEED_VIP;
extern const float CS_PLAYER_SPEED_SHIELD;
extern const float CS_PLAYER_SPEED_STOPPED;
extern const float CS_PLAYER_SPEED_HAS_HOSTAGE;
extern const float CS_PLAYER_SPEED_OBSERVER;

extern const float CS_PLAYER_SPEED_DUCK_MODIFIER;
extern const float CS_PLAYER_SPEED_WALK_MODIFIER;
extern const float CS_PLAYER_SPEED_CLIMB_MODIFIER;
extern const float CS_PLAYER_HEAVYARMOR_FLINCH_MODIFIER;

extern const float CS_PLAYER_DUCK_SPEED_IDEAL;

#ifdef CSTRIKE15
#ifdef CLIENT_DLL
#define OLD_CROUCH_PROTOCOL_INDEX 13546
inline bool IsPreCrouchUpdateDemo(void) { return (engine->IsHLTV() || engine->IsPlayingDemo()) && engine->GetConnectionDataProtocol() <= OLD_CROUCH_PROTOCOL_INDEX; }
#endif
#endif

template< class T >
class CUtlVectorInitialized : public CUtlVector< T >
{
public:
	CUtlVectorInitialized(T* pMemory, int numElements) : CUtlVector< T >(pMemory, numElements)
	{
		CUtlVector< T >::SetSize(numElements);
	}
};

namespace TeamJoinFailedReason
{
	enum Type
	{
		CHANGED_TOO_OFTEN,
		BOTH_TEAMS_FULL,
		TERRORISTS_FULL,
		CTS_FULL,
		CANT_JOIN_SPECTATOR,
		HUMANS_CAN_ONLY_JOIN_TS,
		HUMANS_CAN_ONLY_JOIN_CTS,
		TOO_MANY_TS,
		TOO_MANY_CTS,
	};
};

#define CS_HOSTAGE_TRANSTIME_PICKUP		0.1
#define CS_HOSTAGE_TRANSTIME_DROP		0.25
#define CS_HOSTAGE_TRANSTIME_RESCUE		4.0

enum EHostageStates_t
{
	k_EHostageStates_Idle = 0,
	k_EHostageStates_BeingUntied,
	k_EHostageStates_GettingPickedUp,
	k_EHostageStates_BeingCarried,
	k_EHostageStates_FollowingPlayer,
	k_EHostageStates_GettingDropped,
	k_EHostageStates_Rescued,
	k_EHostageStates_Dead,
};

#define CONSTANT_UNITS_SMOKEGRENADERADIUS 166
#define CONSTANT_UNITS_GENERICGRENADERADIUS 115

const float SmokeGrenadeRadius = CONSTANT_UNITS_SMOKEGRENADERADIUS;
const float FlashbangGrenadeRadius = CONSTANT_UNITS_GENERICGRENADERADIUS;
const float HEGrenadeRadius = CONSTANT_UNITS_GENERICGRENADERADIUS;
const float MolotovGrenadeRadius = CONSTANT_UNITS_GENERICGRENADERADIUS;
const float DecoyGrenadeRadius = CONSTANT_UNITS_GENERICGRENADERADIUS;


#define ADDON_FLASHBANG_1		0x0001
#define ADDON_FLASHBANG_2		0x0002
#define ADDON_HE_GRENADE		0x0004
#define ADDON_SMOKE_GRENADE		0x0008
#define ADDON_C4				0x0010
#define ADDON_DEFUSEKIT			0x0020
#define ADDON_PRIMARY			0x0040
#define ADDON_PISTOL			0x0080
#define ADDON_PISTOL2			0x0100
#define ADDON_DECOY				0x0200
#define ADDON_KNIFE				0x0400
#define ADDON_MASK				0x0800
#define ADDON_TAGRENADE			0x1000
#define NUM_ADDON_BITS			13

#define ADDON_CLIENTSIDE_HOLIDAY_HAT			( 0x1 << NUM_ADDON_BITS )
#define ADDON_CLIENTSIDE_GHOST					( 0x2 << NUM_ADDON_BITS )
#define ADDON_CLIENTSIDE_ASSASSINATION_TARGET	( 0x4 << NUM_ADDON_BITS )
#define NUM_CLIENTSIDE_ADDON_BITS 3


#define WEAPON_SLOT_RIFLE		0	  
#define WEAPON_SLOT_PISTOL		1	  
#define WEAPON_SLOT_KNIFE		2
#define WEAPON_SLOT_GRENADES	3
#define WEAPON_SLOT_C4			4

#define WEAPON_SLOT_FIRST		0
#define WEAPON_SLOT_LAST		4


#define TEAM_TERRORIST			2
#define	TEAM_CT					3
#define TEAM_MAXCOUNT			4	        

#define TEAM_TERRORIST_BASE0	TEAM_TERRORIST - 2
#define TEAM_CT_BASE0			TEAM_CT - 2

#define CS_DEATH_ANIMATION_TIME			0.5

#define CS_KILLS_FOR_DOMINATION			4

#define CS_DEATH_DOMINATION				0x0001	    
#define CS_DEATH_REVENGE				0x0002	     


#define DMG_HEADSHOT		(DMG_LASTGENERICFLAG<<1)


struct PlayerViewmodelArmConfig
{
	const char* szPlayerModelSearchSubStr;
	const char* szSkintoneIndex;
	const char* szAssociatedGloveModel;
	const char* szAssociatedSleeveModel;
	const char* szAssociatedSleeveModelEconOverride;
};

enum CSPlayerState
{
	STATE_ACTIVE = 0,

	STATE_WELCOME,			     

	STATE_PICKINGTEAM,			  
	STATE_PICKINGCLASS,			  

	STATE_DEATH_ANIM,			        
	STATE_DEATH_WAIT_FOR_KEY,	            
	STATE_OBSERVER_MODE,		     

	STATE_GUNGAME_RESPAWN,		       
	STATE_DORMANT,				     

	NUM_PLAYER_STATES
};


enum e_RoundEndReason
{
	Invalid_Round_End_Reason = -1,
	RoundEndReason_StillInProgress = 0,
	Target_Bombed = 1,
	VIP_Escaped = 2,
	VIP_Assassinated = 3,
	Terrorists_Escaped = 4,
	CTs_PreventEscape = 5,
	Escaping_Terrorists_Neutralized = 6,
	Bomb_Defused = 7,
	CTs_Win = 8,
	Terrorists_Win = 9,
	Round_Draw = 10,
	All_Hostages_Rescued = 11,
	Target_Saved = 12,
	Hostages_Not_Rescued = 13,
	Terrorists_Not_Escaped = 14,
	VIP_Not_Escaped = 15,
	Game_Commencing = 16,
	Terrorists_Surrender = 17,
	CTs_Surrender = 18,
	Terrorists_Planted = 19,
	CTs_ReachedHostage = 20,
	RoundEndReason_Count = 21,
};

enum GamePhase
{
	GAMEPHASE_WARMUP_ROUND,
	GAMEPHASE_PLAYING_STANDARD,
	GAMEPHASE_PLAYING_FIRST_HALF,
	GAMEPHASE_PLAYING_SECOND_HALF,
	GAMEPHASE_HALFTIME,
	GAMEPHASE_MATCH_ENDED,
	GAMEPHASE_MAX
};

enum GrenadeType_t
{
	GRENADE_TYPE_EXPLOSIVE,
	GRENADE_TYPE_FLASH,
	GRENADE_TYPE_FIRE,
	GRENADE_TYPE_DECOY,
	GRENADE_TYPE_SMOKE,
	GRENADE_TYPE_SENSOR,

	GRENADE_TYPE_TOTAL,
};

#define PUSHAWAY_THINK_INTERVAL		(1.0f / 20.0f)
#define GLOWUPDATE_DEFAULT_THINK_INTERVAL		0.2f

#define CS_MAX_PLAYER_MODELS 10
#define MAX_MODEL_STRING_SIZE 256

#define CS_CLASS_NONE 0

#define MAX_ENDMATCH_VOTE_PANELS 10
const PlayerViewmodelArmConfig* GetPlayerViewmodelArmConfigForPlayerModel(const char* szPlayerModel);

class PlayerModelInfo
{
public:
	PlayerModelInfo();

#if !defined( CLIENT_DLL )

	bool IsTClass(int classID);
	bool IsCTClass(int classID);

	int GetFirstTClass(void) const { return 1; }
	int GetFirstCTClass(void) const { return m_NumTModels + 1; }
	int GetFirstClass(void) const { return GetFirstTClass(); }

	int GetLastTClass(void) const { return m_NumTModels; }
	int GetLastCTClass(void) const { return m_NumTModels + m_NumCTModels; }
	int GetLastClass(void) const { return GetLastCTClass(); }

	int GetNextClassForTeam(int team);

	const char* GetClassName(int classID);
	const char* GetClassModelPath(int classID);

	int GetNumTModels(void) const { return m_NumTModels; }
	int GetNumCTModels(void) const { return m_NumCTModels; }

	void InitializeForCurrentMap(void);

private:
	void AddModel(const char* modelName);
	void SetTViewModelArms(const char* modelName);
	void SetCTViewModelArms(const char* modelName);

#endif  

public:
	static const char* g_customizationModelCT;
	static const char* g_customizationModelT;
	static const char* g_defaultTModel;
	static const char* g_defaultCTModel;

	static PlayerModelInfo* GetPtr(void) { return &s_PlayerModelInfo; }

private:
	static PlayerModelInfo s_PlayerModelInfo;

#if !defined( CLIENT_DLL )
	char m_ClassModelPaths[CS_MAX_PLAYER_MODELS][MAX_MODEL_STRING_SIZE];
	char m_ClassNames[CS_MAX_PLAYER_MODELS][MAX_MODEL_STRING_SIZE];
	char m_mapName[MAX_MODEL_STRING_SIZE];

	int m_NumTModels;
	int m_NumCTModels;
	int m_NumModels;
	int m_nNextClassT;
	int m_nNextClassCT;
#endif  
};

enum CSMvpReason_t
{
	CSMVP_UNDEFINED = 0,
	CSMVP_ELIMINATION,
	CSMVP_BOMBPLANT,
	CSMVP_BOMBDEFUSE,
	CSMVP_HOSTAGERESCUE,

	CSMVP_GUNGAMEWINNER,
};

#define CS_MUZZLEFLASH_NONE -1
#define CS_MUZZLEFLASH_NORM	0
#define CS_MUZZLEFLASH_X	1

extern const char* pszWinPanelCategoryHeaders[];

namespace AcquireResult
{
	enum Type
	{
		Allowed,
		InvalidItem,
		AlreadyOwned,
		AlreadyPurchased,
		ReachedGrenadeTypeLimit,
		ReachedGrenadeTotalLimit,
		NotAllowedByTeam,
		NotAllowedByMap,
		NotAllowedByMode,
		NotAllowedForPurchase,
		NotAllowedByProhibition,
	};
}

namespace AcquireMethod
{
	enum Type
	{
		PickUp,
		Buy,
	};
}

namespace QuestProgress
{
	enum Reason
	{
		QUEST_NONINITIALIZED,

		QUEST_OK,

		QUEST_NOT_ENOUGH_PLAYERS,

		QUEST_WARMUP,

		QUEST_NOT_CONNECTED_TO_STEAM,

		QUEST_NONOFFICIAL_SERVER,

		QUEST_NO_ENTITLEMENT,

		QUEST_NO_QUEST,

		QUEST_PLAYER_IS_BOT,

		QUEST_WRONG_MAP,
		QUEST_WRONG_MODE,

		QUEST_NOT_SYNCED_WITH_SERVER,

		QUEST_REASON_MAX,
	};

	const int QuestReasonBits = 4;
	COMPILE_TIME_ASSERT(QUEST_REASON_MAX <= (1 << QuestReasonBits));

	const char* ReasonString(Reason reason);              
}

#endif  