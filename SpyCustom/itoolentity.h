#ifndef ITOOLENTITY_H
#define ITOOLENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "utlvector.h"
#include "Color.h"
#include "basehandle.h"
#include "iclientrenderable.h"
#include "ishadowmgr.h"


class IServerEntity;
class IClientEntity;
class IToolSystem;
class IClientRenderable;
class Vector;
class QAngle;
class CBaseEntity;
class CBaseAnimating;
class CTakeDamageInfo;
class ITempEntsSystem;
class IEntityFactoryDictionary;


typedef unsigned int HTOOLHANDLE;
enum
{
	HTOOLHANDLE_INVALID = 0
};


enum ClientShadowFlags_t
{
	SHADOW_FLAGS_USE_RENDER_TO_TEXTURE = (SHADOW_FLAGS_LAST_FLAG << 1),
	SHADOW_FLAGS_ANIMATING_SOURCE = (SHADOW_FLAGS_LAST_FLAG << 2),
	SHADOW_FLAGS_USE_DEPTH_TEXTURE = (SHADOW_FLAGS_LAST_FLAG << 3),
	CLIENT_SHADOW_FLAGS_LAST_FLAG = SHADOW_FLAGS_USE_DEPTH_TEXTURE
};


typedef void* EntitySearchResult;


class IClientTools : public IBaseInterface
{
public:
	virtual HTOOLHANDLE		AttachToEntity(EntitySearchResult entityToAttach) = 0;
	virtual void			DetachFromEntity(EntitySearchResult entityToDetach) = 0;

	virtual bool			IsValidHandle(HTOOLHANDLE handle) = 0;

	virtual int				GetNumRecordables() = 0;
	virtual HTOOLHANDLE		GetRecordable(int index) = 0;

	virtual EntitySearchResult	NextEntity(EntitySearchResult currentEnt) = 0;
	EntitySearchResult			FirstEntity() { return NextEntity(NULL); }

	virtual void			SetEnabled(HTOOLHANDLE handle, bool enabled) = 0;
	virtual void			SetRecording(HTOOLHANDLE handle, bool recording) = 0;
	virtual bool			ShouldRecord(HTOOLHANDLE handle) = 0;

	virtual HTOOLHANDLE		GetToolHandleForEntityByIndex(int entindex) = 0;

	virtual int				GetModelIndex(HTOOLHANDLE handle) = 0;
	virtual const char* GetModelName(HTOOLHANDLE handle) = 0;
	virtual const char* GetClassname(HTOOLHANDLE handle) = 0;

	virtual void			AddClientRenderable(IClientRenderable* pRenderable, int renderGroup) = 0;
	virtual void			RemoveClientRenderable(IClientRenderable* pRenderable) = 0;
	virtual void			SetRenderGroup(IClientRenderable* pRenderable, int renderGroup) = 0;
	virtual void			MarkClientRenderableDirty(IClientRenderable* pRenderable) = 0;
	virtual void			UpdateProjectedTexture(ClientShadowHandle_t h, bool bForce) = 0;

	virtual bool			DrawSprite(IClientRenderable* pRenderable, float scale, float frame, int rendermode, int renderfx, const Color& color, float flProxyRadius, int* pVisHandle) = 0;

	virtual EntitySearchResult	GetLocalPlayer() = 0;
	virtual bool			GetLocalPlayerEyePosition(Vector& org, QAngle& ang, float& fov) = 0;

	virtual ClientShadowHandle_t CreateShadow(CBaseHandle handle, int nFlags) = 0;
	virtual void			DestroyShadow(ClientShadowHandle_t h) = 0;

	virtual ClientShadowHandle_t CreateFlashlight(const FlashlightState_t& lightState) = 0;
	virtual void			DestroyFlashlight(ClientShadowHandle_t h) = 0;
	virtual void			UpdateFlashlightState(ClientShadowHandle_t h, const FlashlightState_t& lightState) = 0;

	virtual void			AddToDirtyShadowList(ClientShadowHandle_t h, bool force = false) = 0;
	virtual void			MarkRenderToTextureShadowDirty(ClientShadowHandle_t h) = 0;

	virtual void			EnableRecordingMode(bool bEnable) = 0;
	virtual bool			IsInRecordingMode() const = 0;

	virtual void			TriggerTempEntity(KeyValues* pKeyValues) = 0;

	virtual int				GetOwningWeaponEntIndex(int entindex) = 0;
	virtual int				GetEntIndex(EntitySearchResult entityToAttach) = 0;

	virtual int				FindGlobalFlexcontroller(char const* name) = 0;
	virtual char const* GetGlobalFlexControllerName(int idx) = 0;

	virtual EntitySearchResult	GetOwnerEntity(EntitySearchResult currentEnt) = 0;

	virtual bool			IsPlayer(EntitySearchResult currentEnt) = 0;
	virtual bool			IsBaseCombatCharacter(EntitySearchResult currentEnt) = 0;
	virtual bool			IsNPC(EntitySearchResult currentEnt) = 0;

	virtual Vector			GetAbsOrigin(HTOOLHANDLE handle) = 0;
	virtual QAngle			GetAbsAngles(HTOOLHANDLE handle) = 0;

	virtual void			ReloadParticleDefintions(const char* pFileName, const void* pBufData, int nLen) = 0;

	virtual void			PostToolMessage(KeyValues* pKeyValues) = 0;

	virtual void			EnableParticleSystems(bool bEnable) = 0;

	virtual bool			IsRenderingThirdPerson() const = 0;
};

#define VCLIENTTOOLS_INTERFACE_VERSION "VCLIENTTOOLS001"


class IServerTools : public IBaseInterface
{
public:
	virtual IServerEntity* GetIServerEntity(IClientEntity* pClientEntity) = 0;
	virtual bool SnapPlayerToPosition(const Vector& org, const QAngle& ang, IClientEntity* pClientPlayer = NULL) = 0;
	virtual bool GetPlayerPosition(Vector& org, QAngle& ang, IClientEntity* pClientPlayer = NULL) = 0;
	virtual bool SetPlayerFOV(int fov, IClientEntity* pClientPlayer = NULL) = 0;
	virtual int GetPlayerFOV(IClientEntity* pClientPlayer = NULL) = 0;
	virtual bool IsInNoClipMode(IClientEntity* pClientPlayer = NULL) = 0;

	virtual CBaseEntity* FirstEntity(void) = 0;
	virtual CBaseEntity* NextEntity(CBaseEntity* pEntity) = 0;
	virtual CBaseEntity* FindEntityByHammerID(int iHammerID) = 0;

	virtual bool GetKeyValue(CBaseEntity* pEntity, const char* szField, char* szValue, int iMaxLen) = 0;
	virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, const char* szValue) = 0;
	virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, float flValue) = 0;
	virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, const Vector& vecValue) = 0;

	virtual CBaseEntity* CreateEntityByName(const char* szClassName) = 0;
	virtual void DispatchSpawn(CBaseEntity* pEntity) = 0;

	virtual void ReloadParticleDefintions(const char* pFileName, const void* pBufData, int nLen) = 0;

	virtual void AddOriginToPVS(const Vector& org) = 0;
	virtual void MoveEngineViewTo(const Vector& vPos, const QAngle& vAngles) = 0;

	virtual bool DestroyEntityByHammerId(int iHammerID) = 0;
	virtual CBaseEntity* GetBaseEntityByEntIndex(int iEntIndex) = 0;
	virtual void RemoveEntity(CBaseEntity* pEntity) = 0;
	virtual void RemoveEntityImmediate(CBaseEntity* pEntity) = 0;
	virtual IEntityFactoryDictionary* GetEntityFactoryDictionary(void) = 0;

	virtual void SetMoveType(CBaseEntity* pEntity, int val) = 0;
	virtual void SetMoveType(CBaseEntity* pEntity, int val, int moveCollide) = 0;
	virtual void ResetSequence(CBaseAnimating* pEntity, int nSequence) = 0;
	virtual void ResetSequenceInfo(CBaseAnimating* pEntity) = 0;

	virtual void ClearMultiDamage(void) = 0;
	virtual void ApplyMultiDamage(void) = 0;
	virtual void AddMultiDamage(const CTakeDamageInfo& pTakeDamageInfo, CBaseEntity* pEntity) = 0;
	virtual void RadiusDamage(const CTakeDamageInfo& info, const Vector& vecSrc, float flRadius, int iClassIgnore, CBaseEntity* pEntityIgnore) = 0;

	virtual ITempEntsSystem* GetTempEntsSystem(void) = 0;
};

typedef IServerTools IServerTools001;

#define VSERVERTOOLS_INTERFACE_VERSION_1	"VSERVERTOOLS001"
#define VSERVERTOOLS_INTERFACE_VERSION		"VSERVERTOOLS002"
#define VSERVERTOOLS_INTERFACE_VERSION_INT	2

class IServerChoreoTools : public IBaseInterface
{
public:

	virtual EntitySearchResult	NextChoreoEntity(EntitySearchResult currentEnt) = 0;
	EntitySearchResult			FirstChoreoEntity() { return NextChoreoEntity(NULL); }
	virtual const char* GetSceneFile(EntitySearchResult sr) = 0;

	virtual int					GetEntIndex(EntitySearchResult sr) = 0;
	virtual void				ReloadSceneFromDisk(int entindex) = 0;
};

#define VSERVERCHOREOTOOLS_INTERFACE_VERSION "VSERVERCHOREOTOOLS001"

#endif  