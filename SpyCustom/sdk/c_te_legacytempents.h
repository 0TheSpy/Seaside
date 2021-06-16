#if !defined( C_TE_LEGACYTEMPENTS_H )
#define C_TE_LEGACYTEMPENTS_H
#ifdef _WIN32
#pragma once
#endif

class C_BaseEntity;
class C_LocalTempEntity;
struct model_t;

#include "mempool.h"
#include "UtlLinkedList.h"

#include "particlemgr.h"
typedef CParticleSubTexture* PMaterialHandle;
#include "cliententitylist.h"

#if defined( SDK_DLL )
enum
{
	CS_SHELL_9MM = 0,
	CS_SHELL_57,
	CS_SHELL_12GAUGE,
	CS_SHELL_556,
	CS_SHELL_762NATO,
	CS_SHELL_338MAG,
};
#endif

abstract_class ITempEnts
{
public:
	virtual						~ITempEnts() {}

	virtual void				Init(void) = 0;
	virtual void				Shutdown(void) = 0;
	virtual void				LevelInit() = 0;
	virtual void				LevelShutdown() = 0;

	virtual void				Update(void) = 0;
	virtual void				Clear(void) = 0;

	virtual void				BloodSprite(const Vector& org, int r, int g, int b, int a, int modelIndex, int modelIndex2, float size) = 0;
	virtual void				RicochetSprite(const Vector& pos, model_t* pmodel, float duration, float scale) = 0;
	virtual void				MuzzleFlash(int type, ClientEntityHandle_t hEntity, int attachmentIndex, bool firstPerson) = 0;
	virtual void				MuzzleFlash(const Vector& pos1, const QAngle& angles, int type, ClientEntityHandle_t hEntity, bool firstPerson) = 0;
	virtual C_LocalTempEntity* SpawnTempModel(model_t* pModel, const Vector& vecOrigin, const QAngle& vecAngles, const Vector& vecVelocity, float flLifeTime, int iFlags) = 0;
	virtual void				BreakModel(const Vector& pos, const QAngle& angles, const Vector& size, const Vector& dir, float random, float life, int count, int modelIndex, char flags) = 0;
	virtual void				Bubbles(const Vector& mins, const Vector& maxs, float height, int modelIndex, int count, float speed) = 0;
	virtual void				BubbleTrail(const Vector& start, const Vector& end, float flWaterZ, int modelIndex, int count, float speed) = 0;
	virtual void				Sprite_Explode(C_LocalTempEntity* pTemp, float scale, int flags) = 0;
	virtual void				FizzEffect(C_BaseEntity* pent, int modelIndex, int density, int current) = 0;
	virtual C_LocalTempEntity* DefaultSprite(const Vector& pos, int spriteIndex, float framerate) = 0;
	virtual void				Sprite_Smoke(C_LocalTempEntity* pTemp, float scale) = 0;
	virtual C_LocalTempEntity* TempSprite(const Vector& pos, const Vector& dir, float scale, int modelIndex, int rendermode, int renderfx, float a, float life, int flags, const Vector& normal = vec3_origin) = 0;
	virtual void				AttachTentToPlayer(int client, int modelIndex, float zoffset, float life) = 0;
	virtual void				KillAttachedTents(int client) = 0;
	virtual void				Sprite_Spray(const Vector& pos, const Vector& dir, int modelIndex, int count, int speed, int iRand) = 0;
	virtual void				Sprite_Trail(const Vector& vecStart, const Vector& vecEnd, int modelIndex, int nCount, float flLife, float flSize, float flAmplitude, int nRenderamt, float flSpeed) = 0;
	virtual void				RocketFlare(const Vector& pos) = 0;
	virtual void				PlaySound(C_LocalTempEntity* pTemp, float damp) = 0;
	virtual void				PhysicsProp(int modelindex, int skin, const Vector& pos, const QAngle& angles, const Vector& vel, int flags, int effects = 0) = 0;
	virtual C_LocalTempEntity* ClientProjectile(const Vector& vecOrigin, const Vector& vecVelocity, const Vector& vecAccel, int modelindex, int lifetime, CBaseEntity* pOwner, const char* pszImpactEffect = NULL, const char* pszParticleEffect = NULL) = 0;
};


class CTempEnts : public ITempEnts
{
public:
	CTempEnts(void);
	virtual					~CTempEnts(void);
public:
	virtual void			Init(void);
	virtual void			Shutdown(void);

	virtual void			LevelInit();

	virtual void			LevelShutdown();

	virtual void			Update(void);
	virtual void			Clear(void);

	virtual void			BloodSprite(const Vector& org, int r, int g, int b, int a, int modelIndex, int modelIndex2, float size);
	virtual void			RicochetSprite(const Vector& pos, model_t* pmodel, float duration, float scale);

	virtual void			MuzzleFlash(int type, ClientEntityHandle_t hEntity, int attachmentIndex, bool firstPerson);
	virtual void			MuzzleFlash(const Vector& pos1, const QAngle& angles, int type, ClientEntityHandle_t hEntity, bool firstPerson = false);

	virtual void			BreakModel(const Vector& pos, const QAngle& angles, const Vector& size, const Vector& dir, float random, float life, int count, int modelIndex, char flags);
	virtual void			Bubbles(const Vector& mins, const Vector& maxs, float height, int modelIndex, int count, float speed);
	virtual void			BubbleTrail(const Vector& start, const Vector& end, float height, int modelIndex, int count, float speed);
	virtual void			Sprite_Explode(C_LocalTempEntity* pTemp, float scale, int flags);
	virtual void			FizzEffect(C_BaseEntity* pent, int modelIndex, int density, int current);
	virtual C_LocalTempEntity* DefaultSprite(const Vector& pos, int spriteIndex, float framerate);
	virtual void			Sprite_Smoke(C_LocalTempEntity* pTemp, float scale);
	virtual C_LocalTempEntity* TempSprite(const Vector& pos, const Vector& dir, float scale, int modelIndex, int rendermode, int renderfx, float a, float life, int flags, const Vector& normal = vec3_origin);
	virtual void			AttachTentToPlayer(int client, int modelIndex, float zoffset, float life);
	virtual void			KillAttachedTents(int client);
	virtual void			Sprite_Spray(const Vector& pos, const Vector& dir, int modelIndex, int count, int speed, int iRand);
	void					Sprite_Trail(const Vector& vecStart, const Vector& vecEnd, int modelIndex, int nCount, float flLife, float flSize, float flAmplitude, int nRenderamt, float flSpeed);

	virtual void			PlaySound(C_LocalTempEntity* pTemp, float damp);
	virtual C_LocalTempEntity* SpawnTempModel(model_t* pModel, const Vector& vecOrigin, const QAngle& vecAngles, const Vector& vecVelocity, float flLifeTime, int iFlags);
	void					RocketFlare(const Vector& pos);
	void					PhysicsProp(int modelindex, int skin, const Vector& pos, const QAngle& angles, const Vector& vel, int flags, int effects = 0);
	C_LocalTempEntity* ClientProjectile(const Vector& vecOrigin, const Vector& vecVelocity, const Vector& vecAcceleration, int modelindex, int lifetime, CBaseEntity* pOwner, const char* pszImpactEffect = NULL, const char* pszParticleEffect = NULL);

private:
	enum
	{
		MAX_TEMP_ENTITIES = 500,
		MAX_TEMP_ENTITY_SPRITES = 200,
		MAX_TEMP_ENTITY_STUDIOMODEL = 50,
	};

	CClassMemoryPool< C_LocalTempEntity >	m_TempEntsPool;
	CUtlLinkedList< C_LocalTempEntity*, unsigned short >	m_TempEnts;

	struct model_t* m_pSpriteMuzzleFlash[10];
	struct model_t* m_pSpriteAR2Flash[4];
	struct model_t* m_pShells[3];
	struct model_t* m_pSpriteCombineFlash[2];

#if defined ( SDK_DLL )
	struct model_t* m_pCS_9MMShell;
	struct model_t* m_pCS_57Shell;
	struct model_t* m_pCS_12GaugeShell;
	struct model_t* m_pCS_556Shell;
	struct model_t* m_pCS_762NATOShell;
	struct model_t* m_pCS_338MAGShell;
#endif

protected:
	C_LocalTempEntity* TempEntAlloc(const Vector& org, model_t* model);
	C_LocalTempEntity* TempEntAllocHigh(const Vector& org, model_t* model);

private:

	inline void				CacheMuzzleFlashes(void);
	PMaterialHandle			m_Material_MuzzleFlash_Player[4];
	PMaterialHandle			m_Material_MuzzleFlash_NPC[4];
	PMaterialHandle			m_Material_Combine_MuzzleFlash_Player[2];
	PMaterialHandle			m_Material_Combine_MuzzleFlash_NPC[2];

private:
	CTempEnts(const CTempEnts&);

	void					TempEntFree(int index);
	C_LocalTempEntity* TempEntAlloc();
	bool					FreeLowPriorityTempEnt();

	int						AddVisibleTempEntity(C_LocalTempEntity* pEntity);

	void					MuzzleFlash_AR2_Player(const Vector& origin, const QAngle& angles, ClientEntityHandle_t hEntity);
	void					MuzzleFlash_AR2_NPC(const Vector& origin, const QAngle& angles, ClientEntityHandle_t hEntity);

	void					MuzzleFlash_SMG1_Player(ClientEntityHandle_t hEntity, int attachmentIndex);
	void					MuzzleFlash_SMG1_NPC(ClientEntityHandle_t hEntity, int attachmentIndex);

	void					MuzzleFlash_Shotgun_Player(ClientEntityHandle_t hEntity, int attachmentIndex);
	void					MuzzleFlash_Shotgun_NPC(ClientEntityHandle_t hEntity, int attachmentIndex);

	void					MuzzleFlash_Pistol_Player(ClientEntityHandle_t hEntity, int attachmentIndex);
	void					MuzzleFlash_Pistol_NPC(ClientEntityHandle_t hEntity, int attachmentIndex);

	void					MuzzleFlash_Combine_Player(ClientEntityHandle_t hEntity, int attachmentIndex);
	void					MuzzleFlash_Combine_NPC(ClientEntityHandle_t hEntity, int attachmentIndex);

	void					MuzzleFlash_357_Player(ClientEntityHandle_t hEntity, int attachmentIndex);

	void					MuzzleFlash_RPG_NPC(ClientEntityHandle_t hEntity, int attachmentIndex);
};


extern ITempEnts* tempents;


#endif  