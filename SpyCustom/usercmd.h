#if !defined( USERCMD_H )
#define USERCMD_H
#ifdef _WIN32
#pragma once
#endif

#include "vector.h"
#include "utlvector.h"
#include "imovehelper.h"
#include "checksum_crc.h"


class bf_read;
class bf_write;

class CEntityGroundContact
{
public:
	int					entindex;
	float				minheight;
	float				maxheight;
};

#ifdef DOTA_DLL

class CUnitOrders
{
public:

	CUnitOrders()
	{
		m_nOrderSequenceNumber = 0;
	}

	void Reset(void)
	{
		m_nUnits.RemoveAll();
		m_nOrderType = 0;
		m_nTargetIndex = 0;
		m_vPosition = vec3_origin;
		m_nAbilityIndex = 0;
	}

	int		m_nOrderSequenceNumber;
	CUtlVector <int>		m_nUnits;
	int		m_nOrderType;
	int		m_nTargetIndex;
	int		m_nAbilityIndex;
	Vector  m_vPosition;
};

#endif


class CUserCmd
{
public:
	CUserCmd()
	{
		Reset();
	}

	virtual ~CUserCmd() { };

	void Reset()
	{
		command_number = 0;
		tick_count = 0;
		viewangles.Init();
		aimdirection.Init();
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
#ifndef CLIENT_DLL
		server_random_seed = 0;
#endif
		mousedx = 0;
		mousedy = 0;

		hasbeenpredicted = false;
#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
		entitygroundcontact.RemoveAll();
#endif

#ifdef DOTA_DLL
		dota_unitorders.Reset();
#endif

#if defined ( PORTAL2 )
		player_held_entity = 0;
		held_entity_was_grabbed_through_portal = 0;

		command_acknowledgements_pending = 0;
		predictedPortalTeleportations = 0;
#endif  

		headangles.Init();
		headoffset.Init();

#if defined( INFESTED_DLL ) || defined( DOTA_DLL )
		crosshairtrace = vec3_origin;
#endif

#ifdef INFESTED_DLL
		crosshair_entity = 0;
		forced_action = 0;
		sync_kill_ent = 0;
		skill_dest.Init();
		skill_dest_ent = 0;

#endif

#if defined( KEEP_COMMAND_REPREDICTION_COUNT )
		debug_RepredictionCount = 0;
#endif
	}

	CUserCmd& operator =(const CUserCmd& src)
	{
		if (this == &src)
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		aimdirection = src.aimdirection;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
#ifndef CLIENT_DLL
		server_random_seed = src.server_random_seed;
#endif
		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;

#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
		entitygroundcontact = src.entitygroundcontact;
#endif

#if defined ( PORTAL2 )
		player_held_entity = src.player_held_entity;
		held_entity_was_grabbed_through_portal = src.held_entity_was_grabbed_through_portal;
		command_acknowledgements_pending = src.command_acknowledgements_pending;
		predictedPortalTeleportations = src.predictedPortalTeleportations;
#endif  

		headangles = src.headangles;
		headoffset = src.headoffset;
#if defined( INFESTED_DLL ) || defined( DOTA_DLL )
		crosshairtrace = src.crosshairtrace;
#endif

#ifdef INFESTED_DLL
		crosshair_entity = src.crosshair_entity;
		forced_action = src.forced_action;
		sync_kill_ent = src.sync_kill_ent;
		skill_dest = src.skill_dest;
		skill_dest_ent = src.skill_dest_ent;
#endif

#ifdef DOTA_DLL
		dota_unitorders = src.dota_unitorders;
#endif

#if defined( KEEP_COMMAND_REPREDICTION_COUNT )
		debug_RepredictionCount = src.debug_RepredictionCount;
#endif

		return *this;
	}

	CUserCmd(const CUserCmd& src)
	{
		*this = src;
	}

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

#if defined( INFESTED_DLL ) || defined( DOTA_DLL )
		CRC32_ProcessBuffer(&crc, &crosshairtrace, sizeof(crosshairtrace));
#endif

#if defined ( PORTAL2 )
		CRC32_ProcessBuffer(&crc, &player_held_entity, sizeof(player_held_entity));
		CRC32_ProcessBuffer(&crc, &held_entity_was_grabbed_through_portal, sizeof(held_entity_was_grabbed_through_portal));
		CRC32_ProcessBuffer(&crc, &command_acknowledgements_pending, sizeof(command_acknowledgements_pending));
		CRC32_ProcessBuffer(&crc, &predictedPortalTeleportations, sizeof(predictedPortalTeleportations));
#endif  

#ifdef INFESTED_DLL
		CRC32_ProcessBuffer(&crc, &crosshair_entity, sizeof(crosshair_entity));
		CRC32_ProcessBuffer(&crc, &forced_action, sizeof(forced_action));
		CRC32_ProcessBuffer(&crc, &sync_kill_ent, sizeof(sync_kill_ent));
		CRC32_ProcessBuffer(&crc, &skill_dest, sizeof(skill_dest));
		CRC32_ProcessBuffer(&crc, &skill_dest_ent, sizeof(skill_dest_ent));
#endif
		CRC32_Final(&crc);

		return crc;
	}

	int		command_number;

	int		tick_count;

	QAngle	viewangles;
	Vector	aimdirection;	    
	float	forwardmove;
	float	sidemove;
	float	upmove;
	int		buttons;
	byte    impulse;
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	    

#ifndef CLIENT_DLL
	int		server_random_seed;       
#endif

	short	mousedx;		       
	short	mousedy;		       

	bool	hasbeenpredicted;

#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
	CUtlVector< CEntityGroundContact > entitygroundcontact;
#endif

#if defined ( DOTA_DLL )
	CUnitOrders dota_unitorders;
#endif

#if defined ( PORTAL2 )
	short player_held_entity;
	short held_entity_was_grabbed_through_portal;

	unsigned short command_acknowledgements_pending;                                
	uint8 predictedPortalTeleportations;             
#endif  

	QAngle headangles;
	Vector headoffset;
#if defined( INFESTED_DLL ) || defined( DOTA_DLL )
	Vector crosshairtrace;		       
#endif

#ifdef INFESTED_DLL
	short crosshair_entity;			        
	byte forced_action;
	short sync_kill_ent;
	Vector skill_dest;
	short skill_dest_ent;
#endif

#if defined( KEEP_COMMAND_REPREDICTION_COUNT )
	unsigned int debug_RepredictionCount;
#endif
};

void ReadUsercmd(bf_read* buf, CUserCmd* move, CUserCmd* from);
void WriteUsercmd(bf_write* buf, const CUserCmd* to, const CUserCmd* from);

#endif  