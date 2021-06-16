#ifndef PLAYERNET_VARS_H
#define PLAYERNET_VARS_H
#ifdef _WIN32
#pragma once
#endif

#include "shared_classnames.h"
#include "networkvar.h" 
#include "datamap.h" 

#define NUM_AUDIO_LOCAL_SOUNDS	8

struct fogparams_t
{
	DECLARE_CLASS_NOBASE(fogparams_t);
	DECLARE_EMBEDDED_NETWORKVAR();

#ifndef CLIENT_DLL
	DECLARE_SIMPLE_DATADESC();
#endif

	bool operator !=(const fogparams_t& other) const;

	CNetworkVector(dirPrimary);
	CNetworkColor32(colorPrimary);
	CNetworkColor32(colorSecondary);
	CNetworkColor32(colorPrimaryLerpTo);
	CNetworkColor32(colorSecondaryLerpTo);
	CNetworkVar(float, start);
	CNetworkVar(float, end);
	CNetworkVar(float, farz);
	CNetworkVar(float, maxdensity);

	CNetworkVar(float, startLerpTo);
	CNetworkVar(float, endLerpTo);
	CNetworkVar(float, lerptime);
	CNetworkVar(float, duration);
	CNetworkVar(bool, enable);
	CNetworkVar(bool, blend);
};

#ifdef CLIENT_DLL
#define CFogController C_FogController
#endif

class CFogController;

struct fogplayerparams_t
{
	DECLARE_CLASS_NOBASE(fogplayerparams_t);
	DECLARE_EMBEDDED_NETWORKVAR();

#ifndef CLIENT_DLL
	DECLARE_SIMPLE_DATADESC();
#endif

	CNetworkHandle(CFogController, m_hCtrl);
	float					m_flTransitionTime;

	color32					m_OldColor;
	float					m_flOldStart;
	float					m_flOldEnd;

	color32					m_NewColor;
	float					m_flNewStart;
	float					m_flNewEnd;

	fogplayerparams_t()
	{
		m_hCtrl.Set(NULL);
		m_flTransitionTime = -1.0f;
		m_OldColor.r = m_OldColor.g = m_OldColor.b = m_OldColor.a = 0;
		m_flOldStart = 0.0f;
		m_flOldEnd = 0.0f;
		m_NewColor.r = m_NewColor.g = m_NewColor.b = m_NewColor.a = 0;
		m_flNewStart = 0.0f;
		m_flNewEnd = 0.0f;
	}
};

struct sky3dparams_t
{
	DECLARE_CLASS_NOBASE(sky3dparams_t);
	DECLARE_EMBEDDED_NETWORKVAR();

#ifndef CLIENT_DLL
	DECLARE_SIMPLE_DATADESC();
#endif

	CNetworkVar(int, scale);
	CNetworkVector(origin);
	CNetworkVar(int, area);

	CNetworkVarEmbedded(fogparams_t, fog);
};

struct audioparams_t
{
	DECLARE_CLASS_NOBASE(audioparams_t);
	DECLARE_EMBEDDED_NETWORKVAR();

#ifndef CLIENT_DLL
	DECLARE_SIMPLE_DATADESC();
#endif

	CNetworkArray(Vector, localSound, NUM_AUDIO_LOCAL_SOUNDS)
		CNetworkVar(int, soundscapeIndex);	       
	CNetworkVar(int, localBits);			          
	CNetworkHandle(CBaseEntity, ent);		     
};


#endif  