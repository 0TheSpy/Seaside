#ifndef IMOVEHELPER_H
#define IMOVEHELPER_H

#ifdef _WIN32
#pragma once
#endif


enum PLAYER_ANIM;
class IPhysicsSurfaceProps;
class Vector;
struct model_t;
struct cmodel_t;
struct vcollide_t;
class CGameTrace;
enum soundlevel_t;

enum
{
	WL_NotInWater = 0,
	WL_Feet,
	WL_Waist,
	WL_Eyes
};


typedef CBaseHandle EntityHandle_t;


#define INVALID_ENTITY_HANDLE INVALID_EHANDLE_INDEX

abstract_class IMoveHelper
{
public:
	static IMoveHelper * GetSingleton() { return sm_pSingleton; }

	virtual	char const* GetName(EntityHandle_t handle) const = 0;

	virtual void	ResetTouchList(void) = 0;
	virtual bool	AddToTouched(const CGameTrace& tr, const Vector& impactvelocity) = 0;
	virtual void	ProcessImpacts(void) = 0;

	virtual void	Con_NPrintf(int idx, PRINTF_FORMAT_STRING char const* fmt, ...) = 0;

	virtual void	StartSound(const Vector& origin, int channel, char const* sample, float volume, soundlevel_t soundlevel, int fFlags, int pitch) = 0;
	virtual void	StartSound(const Vector& origin, const char* soundname) = 0;
	virtual void	PlaybackEventFull(int flags, int clientindex, unsigned short eventindex, float delay, Vector& origin, Vector& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = 0;

	virtual bool	PlayerFallingDamage(void) = 0;

	virtual void	PlayerSetAnimation(PLAYER_ANIM playerAnim) = 0;

	virtual IPhysicsSurfaceProps* GetSurfaceProps(void) = 0;

	virtual bool IsWorldEntity(const CBaseHandle& handle) = 0;

protected:
	static void SetSingleton(IMoveHelper* pMoveHelper) { sm_pSingleton = pMoveHelper; }

	virtual			~IMoveHelper() {}

	static IMoveHelper* sm_pSingleton;
};

#define IMPLEMENT_MOVEHELPER()	\
	IMoveHelper* IMoveHelper::sm_pSingleton = 0

inline IMoveHelper* MoveHelper()
{
	return IMoveHelper::GetSingleton();
}


#endif  