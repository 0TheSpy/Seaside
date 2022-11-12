#pragma once
#include "pbwrap.hpp"

//https://github.com/SteamDatabase/Protobufs/blob/master/csgo/cstrike15_gcmessages.proto

#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientReserve 9107
#define k_EMsgGCClientWelcome 4004
#define k_EMsgGCClientHello 4006
#define k_EMsgGCAdjustItemEquippedState 1059
#define k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello 9109
#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello 9110
#define k_EMsgGCCStrike15_v2_ClientGCRankUpdate  9194

using namespace pbwrap;

struct CMsgClientHello : pbmsg<8> {
    PBMSG_CTOR;
    PBFIELD(3, types::Uint32, client_session_need);
};

struct MatchmakingGC2ClientHello : pbmsg<20> {
    struct PlayerRankingInfo : pbmsg<6> {
        PBMSG_CTOR;
        PBFIELD(1, types::Uint32, account_id);
        PBFIELD(2, types::Uint32, rank_id);
        PBFIELD(3, types::Uint32, wins);
        PBFIELD(6, types::Uint32, rank_type_id);
    };
    struct PlayerCommendationInfo : pbmsg<4> {
        PBMSG_CTOR;
        PBFIELD(1, types::Uint32, cmd_friendly);
        PBFIELD(2, types::Uint32, cmd_teaching);
        PBFIELD(4, types::Uint32, cmd_leader);
    };

    PBMSG_CTOR;
    PBFIELD(7, PlayerRankingInfo, ranking);
    PBFIELD(8, PlayerCommendationInfo, commendation);
    PBFIELD(17, types::Int32, player_level);
    PBFIELD(18, types::Int32, player_cur_xp);
    PBFIELD(4, types::Uint32, penalty_seconds);
    PBFIELD(5, types::Uint32, penalty_reason);
    PBFIELD(6, types::Int32, vac_banned);
};

struct CMsgGCCStrike15_v2_ClientGCRankUpdate : pbmsg<1> {
    PBMSG_CTOR;
    PBFIELD(1, MatchmakingGC2ClientHello::PlayerRankingInfo, ranking);
};

struct CSOEconItemEquipped : pbmsg<2> {
    PBMSG_CTOR;
    PBFIELD(1, types::Int32, new_class);
    PBFIELD(2, types::Int32, new_slot);
};

struct CSOEconItemAttribute : pbmsg<3> {
    PBMSG_CTOR;
    PBFIELD(1, types::Uint32, def_index);
    PBFIELD(2, types::Uint32, value);
    PBFIELD(3, types::Bytes, value_bytes);
};

struct CSOEconItem : pbmsg<19> {
    PBMSG_CTOR;
    PBFIELD(1, types::Uint64, id);
    PBFIELD(2, types::Uint32, account_id);
    PBFIELD(3, types::Uint32, inventory);
    PBFIELD(4, types::Int32, def_index);
    PBFIELD(5, types::Uint32, quantity);
    PBFIELD(6, types::Uint32, level);
    PBFIELD(7, types::Uint32, quality);
    PBFIELD(8, types::Uint32, flags);
    PBFIELD(9, types::Uint32, origin);
    PBFIELD(10, types::String, custom_name);
    PBFIELD(11, types::String, custom_desc);
    PBFIELD(12, CSOEconItemAttribute, attribute); 
    PBFIELD(14, types::Bool, in_use);
    PBFIELD(15, types::Uint32, style);
    PBFIELD(16, types::Uint64, original_id);
    PBFIELD(18, CSOEconItemEquipped, equipped_state);
    PBFIELD(19, types::Uint32, rarity);
};

struct CMsgClientWelcome : pbmsg<11> {
    struct SubscribedType : pbmsg<2> {
        PBMSG_CTOR;
        PBFIELD(1, types::Int32, type_id);
        PBFIELD(2, CSOEconItem, object_data);
    };

    struct CMsgSOCacheSubscribed : pbmsg<4> {
        PBMSG_CTOR;
        PBFIELD(2, SubscribedType, objects);
    };

    PBMSG_CTOR;
    PBFIELD(3, CMsgSOCacheSubscribed, outofdate_subscribed_caches);
};

struct CMsgAdjustItemEquippedState : pbmsg<4> {
    PBMSG_CTOR;
    PBFIELD(1, types::Uint64, item_id);
    PBFIELD(2, types::Uint32, new_class);
    PBFIELD(3, types::Uint32, new_slot);
    PBFIELD(4, types::Bool, swap);
};

// https://github.com/SteamDatabase/Protobufs/blob/master/csgo/cstrike15_usermessages.proto

struct CCSUsrMsg_HintText : pbmsg<1> {
	PBMSG_CTOR;
	PBFIELD(1, types::String, text);
};

struct CCSUsrMsg_KeyHintText : pbmsg<1> {
	PBMSG_CTOR;
	PBFIELD(1, types::String, hints);
};

struct CCSUsrMsg_HudText : pbmsg<1> {
	PBMSG_CTOR;
	PBFIELD(1, types::String, text);
};

struct CCSUsrMsg_TextMsg : pbmsg<3> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, msg_dst);
	PBFIELD(3, types::String, params);
};

struct CCSUsrMsg_SayText : pbmsg<4> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, ent_idx);
	PBFIELD(2, types::String, text);
	PBFIELD(3, types::Bool, chat);
	PBFIELD(4, types::Bool, textallchat);
};

struct CCSUsrMsg_SayText2 : pbmsg<5> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, ent_idx); 
	PBFIELD(2, types::Bool, chat); 
	PBFIELD(3, types::String, msg_name);
	PBFIELD(4, types::String, params);
	PBFIELD(5, types::Bool, textallchat);
};

struct CCSUsrMsg_RadioText : pbmsg<4> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, msg_dst);
	PBFIELD(2, types::Int32, client);
	PBFIELD(3, types::String, msg_name);
	PBFIELD(4, types::String, params);
};

struct CMsgRGBA : pbmsg<4> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, r);
	PBFIELD(2, types::Int32, g);
	PBFIELD(3, types::Int32, b);
	PBFIELD(4, types::Int32, a);
};

struct CMsgVector2D : pbmsg<2> {
	PBMSG_CTOR;
	PBFIELD(1, types::Float, x);
	PBFIELD(2, types::Float, y); 
};

struct CCSUsrMsg_HudMsg : pbmsg<11> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, channel);
	PBFIELD(2, CMsgVector2D, pos);
	PBFIELD(3, CMsgRGBA, clr1);
	PBFIELD(4, CMsgRGBA, clr2);
	PBFIELD(5, types::Int32, effect);
	PBFIELD(6, types::Float, fade_in_time);
	PBFIELD(7, types::Float, fade_out_time);
	PBFIELD(9, types::Float, hold_time);
	PBFIELD(10, types::Float, fx_time);
	PBFIELD(11, types::String, text);
};

struct CCSUsrMsg_SendAudio : pbmsg<1> {
	PBMSG_CTOR;
	PBFIELD(1, types::String, radio_sound); 
};

struct CMsgPlayerInfo : pbmsg<6> {
	PBMSG_CTOR;
	PBFIELD(1, types::String, name);
	PBFIELD(2, types::Fixed64, xuid);
	PBFIELD(3, types::Int32, userid);
	PBFIELD(4, types::Fixed64, steamid);
	PBFIELD(5, types::Bool, fakeplayer);
	PBFIELD(6, types::Bool, ishltv);
};

struct CMsg_CVars : pbmsg<2> {
	PBMSG_CTOR;

	struct CVar : pbmsg<2> {
		PBMSG_CTOR;
		PBFIELD(1, types::String, name);
		PBFIELD(2, types::String, value);
	};

	PBFIELD(1, CMsg_CVars::CVar, cvars);
};

struct CNETMsg_SetConVar : pbmsg<1> {
	PBMSG_CTOR;
	PBFIELD(1, CMsg_CVars, convars);
};
 
struct CCSUsrMsg_ShowMenu : pbmsg<3> {
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, bits_valid_slots);
	PBFIELD(2, types::Int32, display_time);
	PBFIELD(3, types::String, menu_string);
};
 
enum ECstrike15UserMessages {
	CS_UM_VGUIMenu = 1,
	CS_UM_Geiger = 2,
	CS_UM_Train = 3,
	CS_UM_HudText = 4,
	CS_UM_SayText = 5,
	CS_UM_SayText2 = 6,
	CS_UM_TextMsg = 7,
	CS_UM_HudMsg = 8,
	CS_UM_ResetHud = 9,
	CS_UM_GameTitle = 10,
	CS_UM_Shake = 12,
	CS_UM_Fade = 13,
	CS_UM_Rumble = 14,
	CS_UM_CloseCaption = 15,
	CS_UM_CloseCaptionDirect = 16,
	CS_UM_SendAudio = 17,
	CS_UM_RawAudio = 18,
	CS_UM_VoiceMask = 19,
	CS_UM_RequestState = 20,
	CS_UM_Damage = 21,
	CS_UM_RadioText = 22,
	CS_UM_HintText = 23,
	CS_UM_KeyHintText = 24,
	CS_UM_ProcessSpottedEntityUpdate = 25,
	CS_UM_ReloadEffect = 26,
	CS_UM_AdjustMoney = 27,
	CS_UM_UpdateTeamMoney = 28,
	CS_UM_StopSpectatorMode = 29,
	CS_UM_KillCam = 30,
	CS_UM_DesiredTimescale = 31,
	CS_UM_CurrentTimescale = 32,
	CS_UM_AchievementEvent = 33,
	CS_UM_MatchEndConditions = 34,
	CS_UM_DisconnectToLobby = 35,
	CS_UM_PlayerStatsUpdate = 36,
	CS_UM_DisplayInventory = 37,
	CS_UM_WarmupHasEnded = 38,
	CS_UM_ClientInfo = 39,
	CS_UM_XRankGet = 40,
	CS_UM_XRankUpd = 41,
	CS_UM_CallVoteFailed = 45,
	CS_UM_VoteStart = 46,
	CS_UM_VotePass = 47,
	CS_UM_VoteFailed = 48,
	CS_UM_VoteSetup = 49,
	CS_UM_ServerRankRevealAll = 50,
	CS_UM_SendLastKillerDamageToClient = 51,
	CS_UM_ServerRankUpdate = 52,
	CS_UM_ItemPickup = 53,
	CS_UM_ShowMenu = 54,
	CS_UM_BarTime = 55,
	CS_UM_AmmoDenied = 56,
	CS_UM_MarkAchievement = 57,
	CS_UM_MatchStatsUpdate = 58,
	CS_UM_ItemDrop = 59,
	CS_UM_GlowPropTurnOff = 60,
	CS_UM_SendPlayerItemDrops = 61,
	CS_UM_RoundBackupFilenames = 62,
	CS_UM_SendPlayerItemFound = 63,
	CS_UM_ReportHit = 64,
	CS_UM_XpUpdate = 65,
	CS_UM_QuestProgress = 66,
	CS_UM_ScoreLeaderboardData = 67,
	CS_UM_PlayerDecalDigitalSignature = 68,
	CS_UM_WeaponSound = 69,
	CS_UM_UpdateScreenHealthBar = 70,
	CS_UM_EntityOutlineHighlight = 71,
	CS_UM_SSUI = 72,
	CS_UM_SurvivalStats = 73,
	CS_UM_DisconnectToLobby2 = 74,
	CS_UM_EndOfMatchAllPlayersData = 75,
	CS_UM_RoundImpactScoreData = 79,
	CS_UM_CurrentRoundOdds = 80,
	CS_UM_DeepStats = 81,
	CS_UM_UtilMsg = 82
};

enum ECSUsrMsg_DisconnectToLobby_Action {
	k_ECSUsrMsg_DisconnectToLobby_Action_Default = 0,
	k_ECSUsrMsg_DisconnectToLobby_Action_GoQueue = 1
};

 