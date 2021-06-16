#ifndef BASEENTITY_SHARED_H
#define BASEENTITY_SHARED_H
#ifdef _WIN32
#pragma once
#endif


#define		FCAP_MUST_SPAWN				0x00000001		   
#define		FCAP_ACROSS_TRANSITION		0x00000002		     
#define		FCAP_FORCE_TRANSITION		0x00000004		    
#define		FCAP_NOTIFY_ON_TRANSITION	0x00000008		          

#define		FCAP_IMPULSE_USE			0x00000010		      
#define		FCAP_CONTINUOUS_USE			0x00000020		      
#define		FCAP_ONOFF_USE				0x00000040		      
#define		FCAP_DIRECTIONAL_USE		0x00000080		         
#define		FCAP_USE_ONGROUND			0x00000100
#define		FCAP_USE_IN_RADIUS			0x00000200
#define		FCAP_SAVE_NON_NETWORKABLE	0x00000400

#define		FCAP_MASTER					0x10000000		         
#define		FCAP_WCEDIT_POSITION		0x40000000		         
#define		FCAP_DONT_SAVE				0x80000000		   


#define NUM_PARENTATTACHMENT_BITS	6

#define VPHYSICS_MAX_OBJECT_LIST_COUNT	1024

enum InvalidatePhysicsBits_t
{
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,
};


#if defined( CLIENT_DLL )
#include "c_baseentity.h"
#include "c_baseanimating.h"
#else
#include "baseentity.h"

#ifdef HL2_EPISODIC
#include "info_darknessmode_lightsource.h"
#endif  

#endif

#if !defined( NO_ENTITY_PREDICTION )
inline bool CBaseEntity::IsPlayerSimulated(void) const
{
	return m_bIsPlayerSimulated;
}

inline CBasePlayer* CBaseEntity::GetSimulatingPlayer(void)
{
	return m_hPlayerSimulationOwner;
}
#endif

inline MoveType_t CBaseEntity::GetMoveType() const
{
	return (MoveType_t)(unsigned char)m_MoveType;
}

inline MoveCollide_t CBaseEntity::GetMoveCollide() const
{
	return (MoveCollide_t)(unsigned char)m_MoveCollide;
}

inline int CBaseEntity::GetCollisionGroup() const
{
	return m_CollisionGroup;
}

inline int	CBaseEntity::GetFlags(void) const
{
	return m_fFlags;
}

inline bool CBaseEntity::IsAlive(void)
{
	return m_lifeState == LIFE_ALIVE;
}

inline CBaseEntity* CBaseEntity::GetOwnerEntity() const
{
	return m_hOwnerEntity.Get();
}

inline CBaseEntity* CBaseEntity::GetEffectEntity() const
{
	return m_hEffectEntity.Get();
}

inline int CBaseEntity::GetPredictionRandomSeed(void)
{
	return m_nPredictionRandomSeed;
}

inline CBasePlayer* CBaseEntity::GetPredictionPlayer(void)
{
	return m_pPredictionPlayer;
}

inline void CBaseEntity::SetPredictionPlayer(CBasePlayer* player)
{
	m_pPredictionPlayer = player;
}


inline bool CBaseEntity::IsSimulatedEveryTick() const
{
	return m_bSimulatedEveryTick;
}

inline bool CBaseEntity::IsAnimatedEveryTick() const
{
	return m_bAnimatedEveryTick;
}

inline void CBaseEntity::SetSimulatedEveryTick(bool sim)
{
	if (m_bSimulatedEveryTick != sim)
	{
		m_bSimulatedEveryTick = sim;
#ifdef CLIENT_DLL
		Interp_UpdateInterpolationAmounts(GetVarMapping());
#endif
	}
}

inline void CBaseEntity::SetAnimatedEveryTick(bool anim)
{
	if (m_bAnimatedEveryTick != anim)
	{
		m_bAnimatedEveryTick = anim;
#ifdef CLIENT_DLL
		Interp_UpdateInterpolationAmounts(GetVarMapping());
#endif
	}
}

inline float CBaseEntity::GetAnimTime() const
{
	return m_flAnimTime;
}

inline float CBaseEntity::GetSimulationTime() const
{
	return m_flSimulationTime;
}

inline void CBaseEntity::SetAnimTime(float at)
{
	m_flAnimTime = at;
}

inline void CBaseEntity::SetSimulationTime(float st)
{
	m_flSimulationTime = st;
}

inline int CBaseEntity::GetEffects(void) const
{
	return m_fEffects;
}

inline void CBaseEntity::RemoveEffects(int nEffects)
{
#if !defined( CLIENT_DLL )
#ifdef HL2_EPISODIC
	if (nEffects & (EF_BRIGHTLIGHT | EF_DIMLIGHT))
	{
		if (!IsPlayer())
		{
			RemoveEntityFromDarknessCheck(this);
		}
	}
#endif  
#endif  

	m_fEffects &= ~nEffects;
	if (nEffects & EF_NODRAW)
	{
#ifndef CLIENT_DLL
		NetworkProp()->MarkPVSInformationDirty();
		DispatchUpdateTransmitState();
#else
		UpdateVisibility();
#endif
	}
}

inline void CBaseEntity::ClearEffects(void)
{
#if !defined( CLIENT_DLL )
#ifdef HL2_EPISODIC
	if (m_fEffects & (EF_BRIGHTLIGHT | EF_DIMLIGHT))
	{
		if (!IsPlayer())
		{
			RemoveEntityFromDarknessCheck(this);
		}
	}
#endif  
#endif  

	m_fEffects = 0;
#ifndef CLIENT_DLL
	DispatchUpdateTransmitState();
#else
	UpdateVisibility();
#endif
}

inline bool CBaseEntity::IsEffectActive(int nEffects) const
{
	return (m_fEffects & nEffects) != 0;
}

#define BASEENTITY_MSG_REMOVE_DECALS	1

extern float k_flMaxEntityPosCoord;
extern float k_flMaxEntityEulerAngle;
extern float k_flMaxEntitySpeed;
extern float k_flMaxEntitySpinRate;

inline bool IsEntityCoordinateReasonable(const vec_t c)
{
	float r = k_flMaxEntityPosCoord;
	return c > -r && c < r;
}

inline bool IsEntityPositionReasonable(const Vector& v)
{
	float r = k_flMaxEntityPosCoord;
	return
		v.x > -r && v.x < r&&
		v.y > -r && v.y < r&&
		v.z > -r && v.z < r;
}

extern int CheckEntityVelocity(Vector& v);

inline bool IsEntityQAngleReasonable(const QAngle& q)
{
	float r = k_flMaxEntityEulerAngle;
	return
		q.x > -r && q.x < r&&
		q.y > -r && q.y < r&&
		q.z > -r && q.z < r;
}

inline bool IsEntityAngularVelocityReasonable(const Vector& q)
{
	float r = k_flMaxEntitySpinRate;
	return
		q.x > -r && q.x < r&&
		q.y > -r && q.y < r&&
		q.z > -r && q.z < r;
}

inline bool IsEntityQAngleVelReasonable(const QAngle& q)
{
	float r = k_flMaxEntitySpinRate;
	return
		q.x > -r && q.x < r&&
		q.y > -r && q.y < r&&
		q.z > -r && q.z < r;
}

extern bool CheckEmitReasonablePhysicsSpew();

#endif  