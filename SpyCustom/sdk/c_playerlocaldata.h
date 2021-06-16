#ifndef C_PLAYERLOCALDATA_H
#define C_PLAYERLOCALDATA_H
#ifdef _WIN32
#pragma once
#endif

#include "basetypes.h"
#include "vector.h"
#include "playernet_vars.h"
#include "interpolatedvar.h" 
#include "predictable_entity.h" 

class CPlayerLocalData
{
public:
	DECLARE_PREDICTABLE();
	DECLARE_CLASS_NOBASE(CPlayerLocalData);
	DECLARE_EMBEDDED_NETWORKVAR();

	CPlayerLocalData() :
		m_iv_vecPunchAngle("CPlayerLocalData::m_iv_vecPunchAngle"),
		m_iv_vecPunchAngleVel("CPlayerLocalData::m_iv_vecPunchAngleVel")
	{
		m_iv_vecPunchAngle.Setup(&m_vecPunchAngle.m_Value, LATCH_SIMULATION_VAR);
		m_iv_vecPunchAngleVel.Setup(&m_vecPunchAngleVel.m_Value, LATCH_SIMULATION_VAR);
		m_flFOVRate = 0;
	}

	unsigned char			m_chAreaBits[MAX_AREA_STATE_BYTES];				   
	unsigned char			m_chAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES];    

	int						m_iHideHUD;			        

	float					m_flFOVRate;		      


	bool					m_bDucked;
	bool					m_bDucking;
	bool					m_bInDuckJump;
	float					m_flDucktime;
	float					m_flDuckJumpTime;
	float					m_flJumpTime;
	int						m_nStepside;
	float					m_flFallVelocity;
	int						m_nOldButtons;
	Vector					m_vecClientBaseVelocity;
	CNetworkQAngle(m_vecPunchAngle);		    
	CInterpolatedVar< QAngle >	m_iv_vecPunchAngle;

	CNetworkQAngle(m_vecPunchAngleVel);		      
	CInterpolatedVar< QAngle >	m_iv_vecPunchAngleVel;
	bool					m_bDrawViewmodel;
	bool					m_bWearingSuit;
	bool					m_bPoisoned;
	float					m_flStepSize;
	bool					m_bAllowAutoMovement;

	sky3dparams_t			m_skybox3d;
	fogplayerparams_t		m_PlayerFog;
	audioparams_t			m_audio;

	bool					m_bSlowMovement;

};

#endif  