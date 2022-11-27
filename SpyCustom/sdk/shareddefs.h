#ifndef SHADERAPI_SHAREDDEFS_H
#define SHADERAPI_SHAREDDEFS_H

#ifdef _WIN32
#pragma once
#endif


enum ShaderShadeMode_t
{
	SHADER_FLAT = 0,
	SHADER_SMOOTH
};

enum ShaderTexCoordComponent_t
{
	SHADER_TEXCOORD_S = 0,
	SHADER_TEXCOORD_T,
	SHADER_TEXCOORD_U
};

enum ShaderTexFilterMode_t
{
	SHADER_TEXFILTERMODE_NEAREST,
	SHADER_TEXFILTERMODE_LINEAR,
	SHADER_TEXFILTERMODE_NEAREST_MIPMAP_NEAREST,
	SHADER_TEXFILTERMODE_LINEAR_MIPMAP_NEAREST,
	SHADER_TEXFILTERMODE_NEAREST_MIPMAP_LINEAR,
	SHADER_TEXFILTERMODE_LINEAR_MIPMAP_LINEAR,
	SHADER_TEXFILTERMODE_ANISOTROPIC
};

enum ShaderTexWrapMode_t
{
	SHADER_TEXWRAPMODE_CLAMP,
	SHADER_TEXWRAPMODE_REPEAT,
	SHADER_TEXWRAPMODE_BORDER
};


enum TextureStage_t
{
	SHADER_TEXTURE_STAGE0 = 0,
	SHADER_TEXTURE_STAGE1,
};

enum Sampler_t
{
	SHADER_SAMPLER0 = 0,
	SHADER_SAMPLER1,
	SHADER_SAMPLER2,
	SHADER_SAMPLER3,
	SHADER_SAMPLER4,
	SHADER_SAMPLER5,
	SHADER_SAMPLER6,
	SHADER_SAMPLER7,
	SHADER_SAMPLER8,
	SHADER_SAMPLER9,
	SHADER_SAMPLER10,
	SHADER_SAMPLER11,
	SHADER_SAMPLER12,
	SHADER_SAMPLER13,
	SHADER_SAMPLER14,
	SHADER_SAMPLER15,
};

enum VertexTextureSampler_t
{
	SHADER_VERTEXTEXTURE_SAMPLER0 = 0,
	SHADER_VERTEXTEXTURE_SAMPLER1,
	SHADER_VERTEXTEXTURE_SAMPLER2,
	SHADER_VERTEXTEXTURE_SAMPLER3,
};


#if defined( _X360 )
#define REVERSE_DEPTH_ON_X360                       
#endif

#if defined( REVERSE_DEPTH_ON_X360 )
#define ReverseDepthOnX360() true
#else
#define ReverseDepthOnX360() false
#endif






#include "bittools.h"

#define TICK_INTERVAL			(iff.g_pGlobals->interval_per_tick)


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)


#define ANIMATION_CYCLE_BITS		15

#define ANIMATION_CYCLE_MINFRAC		(1.0f / (1<<ANIMATION_CYCLE_BITS))

#define CONCEPT_WEIGHT 5.0f



class CViewVectors
{
public:
	CViewVectors() {}

	CViewVectors(
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight)
	{
		m_vView = vView;
		m_vHullMin = vHullMin;
		m_vHullMax = vHullMax;
		m_vDuckHullMin = vDuckHullMin;
		m_vDuckHullMax = vDuckHullMax;
		m_vDuckView = vDuckView;
		m_vObsHullMin = vObsHullMin;
		m_vObsHullMax = vObsHullMax;
		m_vDeadViewHeight = vDeadViewHeight;
	}

	Vector m_vView;

	Vector m_vHullMin;
	Vector m_vHullMax;

	Vector m_vDuckHullMin;
	Vector m_vDuckHullMax;
	Vector m_vDuckView;

	Vector m_vObsHullMin;
	Vector m_vObsHullMax;

	Vector m_vDeadViewHeight;
};

#define VEC_VIEW			g_pGameRules->GetViewVectors()->m_vView
#define VEC_HULL_MIN		g_pGameRules->GetViewVectors()->m_vHullMin
#define VEC_HULL_MAX		g_pGameRules->GetViewVectors()->m_vHullMax

#define VEC_DUCK_HULL_MIN	g_pGameRules->GetViewVectors()->m_vDuckHullMin
#define VEC_DUCK_HULL_MAX	g_pGameRules->GetViewVectors()->m_vDuckHullMax
#define VEC_DUCK_VIEW		g_pGameRules->GetViewVectors()->m_vDuckView

#define VEC_OBS_HULL_MIN	g_pGameRules->GetViewVectors()->m_vObsHullMin
#define VEC_OBS_HULL_MAX	g_pGameRules->GetViewVectors()->m_vObsHullMax

#define VEC_DEAD_VIEWHEIGHT	g_pGameRules->GetViewVectors()->m_vDeadViewHeight


#define WATERJUMP_HEIGHT			8

#define MAX_CLIMB_SPEED		200


#define TIME_TO_DUCK_MSECS		400

#define TIME_TO_UNDUCK_MSECS		200

inline float FractionDucked(int msecs)
{
	return clamp((float)msecs / (float)TIME_TO_DUCK_MSECS, 0.0f, 1.0f);
}

inline float FractionUnDucked(int msecs)
{
	return clamp((float)msecs / (float)TIME_TO_UNDUCK_MSECS, 0.0f, 1.0f);
}

#define MAX_WEAPON_SLOTS		6	    
#define MAX_WEAPON_POSITIONS	20	       
#define MAX_ITEM_TYPES			6	    
#define MAX_WEAPONS				48	     

#define MAX_ITEMS				5	    

#define WEAPON_NOCLIP			-1	             

#define	MAX_AMMO_TYPES	32		 
#define MAX_AMMO_SLOTS  32		   

#define HUD_PRINTNOTIFY		1
#define HUD_PRINTCONSOLE	2
#define HUD_PRINTTALK		3
#define HUD_PRINTCENTER		4


#define CLOSE_CAPTION_WARNIFMISSING	( 1<<0 )
#define CLOSE_CAPTION_FROMPLAYER	( 1<<1 )
#define CLOSE_CAPTION_GENDER_MALE	( 1<<2 )
#define CLOSE_CAPTION_GENDER_FEMALE	( 1<<3 )

#define	HIDEHUD_WEAPONSELECTION		( 1<<0 )	      
#define	HIDEHUD_FLASHLIGHT			( 1<<1 )
#define	HIDEHUD_ALL					( 1<<2 )
#define HIDEHUD_HEALTH				( 1<<3 )	       
#define HIDEHUD_PLAYERDEAD			( 1<<4 )	     
#define HIDEHUD_NEEDSUIT			( 1<<5 )	          
#define HIDEHUD_MISCSTATUS			( 1<<6 )	          
#define HIDEHUD_CHAT				( 1<<7 )	        
#define	HIDEHUD_CROSSHAIR			( 1<<8 )	  
#define	HIDEHUD_VEHICLE_CROSSHAIR	( 1<<9 )	   
#define HIDEHUD_INVEHICLE			( 1<<10 )
#define HIDEHUD_BONUS_PROGRESS		( 1<<11 )	        
#define HIDEHUD_RADAR				( 1<<12 )	   

#define HIDEHUD_BITCOUNT			13

#define bits_SUIT_DEVICE_SPRINT		0x00000001
#define bits_SUIT_DEVICE_FLASHLIGHT	0x00000002
#define bits_SUIT_DEVICE_BREATHER	0x00000004

#define MAX_SUIT_DEVICES			3


#define MAX_PLAYERS				33      


#define MAX_PLACE_NAME_LENGTH		18

#define TEAM_ANY				-1	     
#define	TEAM_INVALID			-1
#define TEAM_UNASSIGNED			0	     
#define TEAM_SPECTATOR			1	  
#define LAST_SHARED_TEAM		TEAM_SPECTATOR

#define FIRST_GAME_TEAM			(LAST_SHARED_TEAM+1)

#define MAX_TEAMS				32	       
#define MAX_TEAM_NAME_LENGTH	32	      

#define WEAPON_NOT_CARRIED				0	     
#define WEAPON_IS_CARRIED_BY_PLAYER		1	      
#define WEAPON_IS_ACTIVE				2	            

#define SKILL_EASY		1
#define SKILL_MEDIUM	2
#define SKILL_HARD		3


#define ITEM_FLAG_SELECTONEMPTY		(1<<0)
#define ITEM_FLAG_NOAUTORELOAD		(1<<1)
#define ITEM_FLAG_NOAUTOSWITCHEMPTY	(1<<2)
#define ITEM_FLAG_LIMITINWORLD		(1<<3)
#define ITEM_FLAG_EXHAUSTIBLE		(1<<4)	            
#define ITEM_FLAG_DOHITLOCATIONDMG	(1<<5)	          
#define ITEM_FLAG_NOAMMOPICKUPS		(1<<6)	         
#define ITEM_FLAG_NOITEMPICKUP		(1<<7)	             

#define MAX_VIEWMODELS			2

#define MAX_BEAM_ENTS			10

#define TRACER_TYPE_DEFAULT		0x00000001
#define TRACER_TYPE_GUNSHIP		0x00000002
#define TRACER_TYPE_STRIDER		0x00000004     
#define TRACER_TYPE_GAUSS		0x00000008
#define TRACER_TYPE_WATERBULLET	0x00000010

#define MUZZLEFLASH_TYPE_DEFAULT	0x00000001
#define MUZZLEFLASH_TYPE_GUNSHIP	0x00000002
#define MUZZLEFLASH_TYPE_STRIDER	0x00000004

enum
{
	MUZZLEFLASH_AR2 = 0,
	MUZZLEFLASH_SHOTGUN,
	MUZZLEFLASH_SMG1,
	MUZZLEFLASH_SMG2,
	MUZZLEFLASH_PISTOL,
	MUZZLEFLASH_COMBINE,
	MUZZLEFLASH_357,
	MUZZLEFLASH_RPG,
	MUZZLEFLASH_COMBINE_TURRET,

	MUZZLEFLASH_FIRSTPERSON = 0x100,
};

#define TRACER_FLAG_WHIZ			0x0001
#define TRACER_FLAG_USEATTACHMENT	0x0002

#define TRACER_DONT_USE_ATTACHMENT	-1

enum
{
	ENTITY_DISSOLVE_NORMAL = 0,
	ENTITY_DISSOLVE_ELECTRICAL,
	ENTITY_DISSOLVE_ELECTRICAL_LIGHT,
	ENTITY_DISSOLVE_CORE,

	ENTITY_DISSOLVE_BITS = 3
};

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10			          

enum PLAYER_ANIM
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
	PLAYER_IN_VEHICLE,

	PLAYER_RELOAD,
	PLAYER_START_AIMING,
	PLAYER_LEAVE_AIMING,
};

#ifdef HL2_DLL
#define PLAYER_FATAL_FALL_SPEED		922.5f             
#define PLAYER_MAX_SAFE_FALL_SPEED	526.5f             
#define PLAYER_LAND_ON_FLOATING_OBJECT	173         
#define PLAYER_MIN_BOUNCE_SPEED		173
#define PLAYER_FALL_PUNCH_THRESHOLD 303.0f                          
#else
#define PLAYER_FATAL_FALL_SPEED		1024    
#define PLAYER_MAX_SAFE_FALL_SPEED	580    
#define PLAYER_LAND_ON_FLOATING_OBJECT	200         
#define PLAYER_MIN_BOUNCE_SPEED		200
#define PLAYER_FALL_PUNCH_THRESHOLD (float)350              
#endif
#define DAMAGE_FOR_FALL_SPEED		100.0f / ( PLAYER_FATAL_FALL_SPEED - PLAYER_MAX_SAFE_FALL_SPEED )      


#define AUTOAIM_2DEGREES  0.0348994967025
#define AUTOAIM_5DEGREES  0.08715574274766
#define AUTOAIM_8DEGREES  0.1391731009601
#define AUTOAIM_10DEGREES 0.1736481776669
#define AUTOAIM_20DEGREES 0.3490658503989

#define AUTOAIM_SCALE_DEFAULT		1.0f
#define AUTOAIM_SCALE_DIRECT_ONLY	0.0f

#define DMG_GENERIC			0			    
#define DMG_CRUSH			(1 << 0)	       
#define DMG_BULLET			(1 << 1)	 
#define DMG_SLASH			(1 << 2)	   
#define DMG_BURN			(1 << 3)	  
#define DMG_VEHICLE			(1 << 4)	    
#define DMG_FALL			(1 << 5)	   
#define DMG_BLAST			(1 << 6)	   
#define DMG_CLUB			(1 << 7)	   
#define DMG_SHOCK			(1 << 8)	  
#define DMG_SONIC			(1 << 9)	   
#define DMG_ENERGYBEAM		(1 << 10)	       
#define DMG_PREVENT_PHYSICS_FORCE		(1 << 11)	     
#define DMG_NEVERGIB		(1 << 12)	                
#define DMG_ALWAYSGIB		(1 << 13)	                
#define DMG_DROWN			(1 << 14)	 


#define DMG_PARALYZE		(1 << 15)	    
#define DMG_NERVEGAS		(1 << 16)	    
#define DMG_POISON			(1 << 17)	         
#define DMG_RADIATION		(1 << 18)	  
#define DMG_DROWNRECOVER	(1 << 19)	  
#define DMG_ACID			(1 << 20)	     
#define DMG_SLOWBURN		(1 << 21)	   

#define DMG_REMOVENORAGDOLL	(1<<22)		                 
#define DMG_PHYSGUN			(1<<23)		        
#define DMG_PLASMA			(1<<24)		   
#define DMG_AIRBOAT			(1<<25)		     

#define DMG_DISSOLVE		(1<<26)		 
#define DMG_BLAST_SURFACE	(1<<27)		            
#define DMG_DIRECT			(1<<28)
#define DMG_BUCKSHOT		(1<<29)		       

#define DMG_LASTGENERICFLAG	DMG_BUCKSHOT



#define	DAMAGE_NO				0
#define DAMAGE_EVENTS_ONLY		1		       
#define	DAMAGE_YES				2
#define	DAMAGE_AIM				3

enum
{
	OBS_MODE_NONE = 0,	    
	OBS_MODE_DEATHCAM,	      
	OBS_MODE_FREEZECAM,	        
	OBS_MODE_FIXED,		      
	OBS_MODE_IN_EYE,	       
	OBS_MODE_CHASE,		       
	OBS_MODE_ROAMING,	  

	NUM_OBSERVER_MODES,
};

#define LAST_PLAYER_OBSERVERMODE	OBS_MODE_ROAMING

enum {
	OBS_ALLOW_ALL = 0,	     
	OBS_ALLOW_TEAM,		         
	OBS_ALLOW_NONE,		           

	OBS_ALLOW_NUM_MODES,
};

enum
{
	TYPE_TEXT = 0,	     
	TYPE_INDEX,		      
	TYPE_URL,		   
	TYPE_FILE,		    
};

enum
{
	VGUI_SCREEN_ACTIVE = 0x1,
	VGUI_SCREEN_VISIBLE_TO_TEAMMATES = 0x2,
	VGUI_SCREEN_ATTACHED_TO_VIEWMODEL = 0x4,
	VGUI_SCREEN_TRANSPARENT = 0x8,
	VGUI_SCREEN_ONLY_USABLE_BY_OWNER = 0x10,

	VGUI_SCREEN_MAX_BITS = 5
};

typedef enum
{
	USE_OFF = 0,
	USE_ON = 1,
	USE_SET = 2,
	USE_TOGGLE = 3
} USE_TYPE;

#define COLOR_RED		Color(255, 64, 64, 255)
#define COLOR_BLUE		Color(153, 204, 255, 255)
#define COLOR_YELLOW	Color(255, 178, 0, 255)
#define COLOR_GREEN		Color(153, 255, 153, 255)
#define COLOR_GREY		Color(204, 204, 204, 255)

enum
{
	DONT_BLEED = -1,

	BLOOD_COLOR_RED = 0,
	BLOOD_COLOR_YELLOW,
	BLOOD_COLOR_GREEN,
	BLOOD_COLOR_MECH,

#if defined( HL2_EPISODIC )
	BLOOD_COLOR_ANTLION,		     
	BLOOD_COLOR_ZOMBIE,			     
	BLOOD_COLOR_ANTLION_WORKER,
	BLOOD_COLOR_BLOB,
	BLOOD_COLOR_BLOB_FROZEN,
#endif  

#if defined( INFESTED_DLL )
	BLOOD_COLOR_BLOB,
	BLOOD_COLOR_BLOB_FROZEN,
#endif  

	BLOOD_COLOR_BRIGHTGREEN,
};

enum PassengerRole_t
{
	VEHICLE_ROLE_NONE = -1,

	VEHICLE_ROLE_DRIVER = 0,	   

	LAST_SHARED_VEHICLE_ROLE,
};

enum
{
	FX_WATER_IN_SLIME = 0x1,
};


#define	MAX_CONTEXT_LENGTH		32
#define NO_THINK_CONTEXT	-1

enum
{
	EFL_KILLME = (1 << 0),	                   
	EFL_DORMANT = (1 << 1),	       
	EFL_NOCLIP_ACTIVE = (1 << 2),	         
	EFL_SETTING_UP_BONES = (1 << 3),	         
	EFL_KEEP_ON_RECREATE_ENTITIES = (1 << 4),                

	EFL_DIRTY_SHADOWUPDATE = (1 << 5),	         
	EFL_NOTIFY = (1 << 6),	           

	EFL_FORCE_CHECK_TRANSMIT = (1 << 7),

	EFL_BOT_FROZEN = (1 << 8),	        
	EFL_SERVER_ONLY = (1 << 9),	  
	EFL_NO_AUTO_EDICT_ATTACH = (1 << 10),         

	EFL_DIRTY_ABSTRANSFORM = (1 << 11),
	EFL_DIRTY_ABSVELOCITY = (1 << 12),
	EFL_DIRTY_ABSANGVELOCITY = (1 << 13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = (1 << 14),
	EFL_DIRTY_SPATIAL_PARTITION = (1 << 15),
	EFL_HAS_PLAYER_CHILD = (1 << 16),	        

	EFL_IN_SKYBOX = (1 << 17),	            
											EFL_USE_PARTITION_WHEN_NOT_SOLID = (1 << 18),	              
											EFL_TOUCHING_FLUID = (1 << 19),	        

											EFL_IS_BEING_LIFTED_BY_BARNACLE = (1 << 20),
											EFL_NO_ROTORWASH_PUSH = (1 << 21),		       
											EFL_NO_THINK_FUNCTION = (1 << 22),
											EFL_NO_GAME_PHYSICS_SIMULATION = (1 << 23),

											EFL_CHECK_UNTOUCH = (1 << 24),
											EFL_DONTBLOCKLOS = (1 << 25),		     
											EFL_DONTWALKON = (1 << 26),		       
											EFL_NO_DISSOLVE = (1 << 27),		    
											EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1 << 28),	      
											EFL_NO_WATER_VELOCITY_CHANGE = (1 << 29),	         
											EFL_NO_PHYSCANNON_INTERACTION = (1 << 30),	        
											EFL_NO_DAMAGE_FORCES = (1 << 31),	      
};

const int FX_BLOODSPRAY_DROPS = 0x01;
const int FX_BLOODSPRAY_GORE = 0x02;
const int FX_BLOODSPRAY_CLOUD = 0x04;
const int FX_BLOODSPRAY_ALL = 0xFF;

#define MAX_SCREEN_OVERLAYS		10

enum
{
	GROUNDLINK = 0,
	TOUCHLINK,
	STEPSIMULATION,
	MODELSCALE,
	POSITIONWATCHER,
	PHYSICSPUSHLIST,
	VPHYSICSUPDATEAI,
	VPHYSICSWATCHER,

	NUM_DATAOBJECT_TYPES,
};

class CBaseEntity;

class CBaseEntity;

enum FireBulletsFlags_t
{
	FIRE_BULLETS_FIRST_SHOT_ACCURATE = 0x1,		       
	FIRE_BULLETS_DONT_HIT_UNDERWATER = 0x2,		          
	FIRE_BULLETS_ALLOW_WATER_SURFACE_IMPACTS = 0x4,	         
	FIRE_BULLETS_TEMPORARY_DANGER_SOUND = 0x8,		              

	FIRE_BULLETS_NO_PIERCING_SPARK = 0x16,	           
	FIRE_BULLETS_HULL = 0x32,	         
	FIRE_BULLETS_ANGULAR_SPREAD = 0x64,	              
};


struct FireBulletsInfo_t
{
	FireBulletsInfo_t()
	{
		m_iShots = 1;
		m_vecSpread.Init(0, 0, 0);
		m_flDistance = 8192;
		m_iTracerFreq = 4;
		m_flDamage = 0.0f;
		m_flPlayerDamage = 0.0f;
		m_pAttacker = NULL;
		m_nFlags = 0;
		m_pAdditionalIgnoreEnt = NULL;
		m_flDamageForceScale = 1.0f;

#ifdef _DEBUG
		m_iAmmoType = -1;
		m_vecSrc.Init(VEC_T_NAN, VEC_T_NAN, VEC_T_NAN);
		m_vecDirShooting.Init(VEC_T_NAN, VEC_T_NAN, VEC_T_NAN);
#endif
		m_bPrimaryAttack = true;
	}

	FireBulletsInfo_t(int nShots, const Vector& vecSrc, const Vector& vecDir, const Vector& vecSpread, float flDistance, int nAmmoType, bool bPrimaryAttack = true)
	{
		m_iShots = nShots;
		m_vecSrc = vecSrc;
		m_vecDirShooting = vecDir;
		m_vecSpread = vecSpread;
		m_flDistance = flDistance;
		m_iAmmoType = nAmmoType;
		m_iTracerFreq = 4;
		m_flDamage = 0;
		m_flPlayerDamage = 0;
		m_pAttacker = NULL;
		m_nFlags = 0;
		m_pAdditionalIgnoreEnt = NULL;
		m_flDamageForceScale = 1.0f;
		m_bPrimaryAttack = bPrimaryAttack;
	}

	int m_iShots;
	Vector m_vecSrc;
	Vector m_vecDirShooting;
	Vector m_vecSpread;
	float m_flDistance;
	int m_iAmmoType;
	int m_iTracerFreq;
	float m_flDamage;
	float m_flPlayerDamage;	            
	int m_nFlags;			  
	float m_flDamageForceScale;
	CBaseEntity* m_pAttacker;
	CBaseEntity* m_pAdditionalIgnoreEnt;
	bool m_bPrimaryAttack;
};

struct StepSimulationStep
{
	int			nTickCount;
	Vector		vecOrigin;
	Quaternion	qRotation;
};

struct StepSimulationData
{
	bool		m_bOriginActive;
	bool		m_bAnglesActive;

	StepSimulationStep	m_Previous2;

	StepSimulationStep	m_Previous;

	StepSimulationStep	m_Discontinuity;

	StepSimulationStep	m_Next;
	QAngle		m_angNextRotation;

	int			m_nLastProcessTickCount;
	Vector		m_vecNetworkOrigin;
	int			m_networkCell[3];
	QAngle		m_angNetworkAngles;
};

struct ModelScale
{
	float		m_flModelScaleStart;
	float		m_flModelScaleGoal;
	float		m_flModelScaleFinishTime;
	float		m_flModelScaleStartTime;
};

#include "soundflags.h"

struct CSoundParameters;
typedef short HSOUNDSCRIPTHANDLE;
struct EmitSound_t
{
	EmitSound_t() :
		m_nChannel(0),
		m_pSoundName(0),
		m_flVolume(VOL_NORM),
		m_SoundLevel(SNDLVL_NONE),
		m_nFlags(0),
		m_nPitch(PITCH_NORM),
		m_pOrigin(0),
		m_flSoundTime(0.0f),
		m_pflSoundDuration(0),
		m_bEmitCloseCaption(true),
		m_bWarnOnMissingCloseCaption(false),
		m_bWarnOnDirectWaveReference(false),
		m_nSpeakerEntity(-1),
		m_UtlVecSoundOrigin(),
		m_hSoundScriptHandle(-1)
	{
	}

	EmitSound_t(const CSoundParameters& src);

	int							m_nChannel;
	char const* m_pSoundName;
	float						m_flVolume;
	soundlevel_t				m_SoundLevel;
	int							m_nFlags;
	int							m_nPitch;
	const Vector* m_pOrigin;
	float						m_flSoundTime;                  
	float* m_pflSoundDuration;
	bool						m_bEmitCloseCaption;
	bool						m_bWarnOnMissingCloseCaption;
	bool						m_bWarnOnDirectWaveReference;
	int							m_nSpeakerEntity;
	mutable CUtlVector< Vector >	m_UtlVecSoundOrigin;               
	mutable HSOUNDSCRIPTHANDLE		m_hSoundScriptHandle;
};

#define MAX_ACTORS_IN_SCENE 16

#define MAX_CONTROL_POINTS			8
#define MAX_CONTROL_POINT_GROUPS	8

#define MAX_PREVIOUS_POINTS			3

#define MAX_CONTROL_POINT_TEAMS		8

#define MAX_CAPLAYOUT_LENGTH		32

#define MAX_ROUND_NAME				32

#define MAX_ROUND_IMAGE_NAME		64

#define TEAMPLAY_ROUND_WIN_SCORE	1

enum
{
	CP_WARN_NORMAL = 0,
	CP_WARN_FINALCAP,
	CP_WARN_NO_ANNOUNCEMENTS
};

#define PREDICTION_ERROR_CHECK_LEVEL 0

#define PREDICTION_ERROR_CHECK_STACKS_FOR_MISSING 0

enum
{
	RT_STATE_SETUP,		     
	RT_STATE_NORMAL,	     
};

enum
{
	SIMULATION_TIME_WINDOW_BITS = 8,
};


#define CELL_COUNT( bits ) ( (MAX_COORD_INTEGER*2) / (1 << (bits)) )               
#define CELL_COUNT_BITS( bits ) MINIMUM_BITS_NEEDED( CELL_COUNT( bits ) )          
#define CELL_BASEENTITY_ORIGIN_CELL_BITS 5         


#ifdef GAME_HAS_NO_USE_KEY
#define COMMENTARY_BUTTONS		(IN_ATTACK | IN_ATTACK2 | IN_USE)
#else
#define COMMENTARY_BUTTONS		(IN_USE)
#endif

bool IsHeadTrackingEnabled();


#define MAX_SPLITSCREEN_PLAYERS 1


inline bool IsSplitScreenSupported()
{
	return (MAX_SPLITSCREEN_PLAYERS > 1) ? true : false;
}

enum InvalidatePhysicsBits_t
{
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,		                  
	BOUNDS_CHANGED = 0x10,		            
	SEQUENCE_CHANGED = 0x20,		            																				
};

enum Class_T
{
	CLASS_NONE = 0,
	CLASS_PLAYER,
	CLASS_PLAYER_ALLY,
	CLASS_PLAYER_ALLY_VITAL,
	CLASS_ANTLION,
	CLASS_BARNACLE,
	CLASS_BLOB,
	CLASS_BULLSEYE,
	CLASS_CITIZEN_PASSIVE,
	CLASS_CITIZEN_REBEL,
	CLASS_COMBINE,
	CLASS_COMBINE_GUNSHIP,
	CLASS_CONSCRIPT,
	CLASS_HEADCRAB,
	CLASS_MANHACK,
	CLASS_METROPOLICE,
	CLASS_MILITARY,
	CLASS_SCANNER,
	CLASS_STALKER,
	CLASS_VORTIGAUNT,
	CLASS_ZOMBIE,
	CLASS_PROTOSNIPER,
	CLASS_MISSILE,
	CLASS_FLARE,
	CLASS_EARTH_FAUNA,
	CLASS_HACKED_ROLLERMINE,
	CLASS_COMBINE_HUNTER,

	LAST_SHARED_ENTITY_CLASS,
};

#define FACTION_NONE				0					               
#define LAST_SHARED_FACTION			(FACTION_NONE)
#define NUM_SHARED_FACTIONS			(FACTION_NONE + 1)










#endif  