#ifndef PARTICLE_PARSE_H
#define PARTICLE_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "utlstring.h"
#include "ifilelist.h"

enum ParticleAttachment_t
{
	PATTACH_ABSORIGIN = 0,			      
	PATTACH_ABSORIGIN_FOLLOW,		         
	PATTACH_CUSTOMORIGIN,			        
	PATTACH_POINT,					       
	PATTACH_POINT_FOLLOW,			          

	PATTACH_WORLDORIGIN,			          

	PATTACH_ROOTBONE_FOLLOW,		            

	MAX_PATTACH_TYPES,
};

extern int GetAttachTypeFromString(const char* pszString);

#define PARTICLE_DISPATCH_FROM_ENTITY		(1<<0)
#define PARTICLE_DISPATCH_RESET_PARTICLES	(1<<1)

struct te_tf_particle_effects_colors_t
{
	Vector m_vecColor1;
	Vector m_vecColor2;
};

struct te_tf_particle_effects_control_point_t
{
	ParticleAttachment_t m_eParticleAttachment;
	Vector m_vecOffset;
};

void ParseParticleEffects(bool bLoadSheets, bool bPrecache);
void ParseParticleEffectsMap(const char* pMapName, bool bLoadSheets, IFileList* pFilesToReload = NULL);

void GetParticleManifest(CUtlVector<CUtlString>& list);

void PrecacheStandardParticleSystems();

class IFileList;
void ReloadParticleEffectsInList(IFileList* pFilesToReload);

void DispatchParticleEffect(const char* pszParticleName, ParticleAttachment_t iAttachType, CBaseEntity* pEntity, const char* pszAttachmentName, bool bResetAllParticlesOnEntity = false);
void DispatchParticleEffect(const char* pszParticleName, ParticleAttachment_t iAttachType, CBaseEntity* pEntity = NULL, int iAttachmentPoint = -1, bool bResetAllParticlesOnEntity = false);
void DispatchParticleEffect(const char* pszParticleName, Vector vecOrigin, QAngle vecAngles, CBaseEntity* pEntity = NULL);
void DispatchParticleEffect(const char* pszParticleName, Vector vecOrigin, Vector vecStart, QAngle vecAngles, CBaseEntity* pEntity = NULL);
void DispatchParticleEffect(int iEffectIndex, Vector vecOrigin, Vector vecStart, QAngle vecAngles, CBaseEntity* pEntity = NULL);

void DispatchParticleEffect(const char* pszParticleName, ParticleAttachment_t iAttachType, CBaseEntity* pEntity, const char* pszAttachmentName, Vector vecColor1, Vector vecColor2, bool bUseColors = true, bool bResetAllParticlesOnEntity = false);
void DispatchParticleEffect(const char* pszParticleName, Vector vecOrigin, QAngle vecAngles, Vector vecColor1, Vector vecColor2, bool bUseColors = true, CBaseEntity* pEntity = NULL, int iAttachType = PATTACH_CUSTOMORIGIN);


void StopParticleEffects(CBaseEntity* pEntity);


#endif  