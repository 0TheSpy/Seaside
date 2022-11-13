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

//protected:
	static void SetSingleton(IMoveHelper* pMoveHelper) { sm_pSingleton = pMoveHelper; }

	virtual			~IMoveHelper() {}

	static IMoveHelper* sm_pSingleton;


	void SetHost_v(C_BaseEntity* player) {
		typedef void(__thiscall* OriginalFn)(void*, C_BaseEntity*);
		getvfunc<OriginalFn>(this, 1)(this, player);
	}
};

#define IMPLEMENT_MOVEHELPER()	\
	IMoveHelper* IMoveHelper::sm_pSingleton = 0

inline IMoveHelper* MoveHelper()
{
	return IMoveHelper::GetSingleton();
}

//movehelper_client.cpp 
//https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/movehelper_client.cpp

class CMoveHelperClient : public IMoveHelper
{
public:
	CMoveHelperClient(void);
	virtual			~CMoveHelperClient(void);

	char const* GetName(EntityHandle_t handle) const;

	// touch lists
	virtual void	ResetTouchList(void);
	virtual bool	AddToTouched(const trace_t& tr, const Vector& impactvelocity);
	virtual void	ProcessImpacts(void);

	// Numbered line printf
	virtual void	Con_NPrintf(int idx, char const* fmt, ...);

	virtual bool	PlayerFallingDamage(void);
	virtual void	PlayerSetAnimation(PLAYER_ANIM eAnim);

	// These have separate server vs client impementations
	virtual void	StartSound(const Vector& origin, int channel, char const* sample, float volume, soundlevel_t soundlevel, int fFlags, int pitch);
	virtual void	StartSound(const Vector& origin, const char* soundname);
	virtual void	PlaybackEventFull(int flags, int clientindex, unsigned short eventindex, float delay, Vector& origin, Vector& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	virtual IPhysicsSurfaceProps* GetSurfaceProps(void);

	virtual bool IsWorldEntity(const CBaseHandle& handle);

	inline void			SetHost(C_BaseEntity* host);

//private:
	// results, tallied on client and server, but only used by server to run SV_Impact.
	// we store off our velocity in the trace_t structure so that we can determine results
	// of shoving boxes etc. around.
	struct touchlist_t
	{
		Vector	deltavelocity;
		trace_t trace;

		touchlist_t() {}

	private:
		touchlist_t(const touchlist_t& src);
	};

	CUtlVector<touchlist_t>			m_TouchList;

	C_BaseEntity* m_pHost;
};


//-----------------------------------------------------------------------------
// Indicates which entity we're going to move
//-----------------------------------------------------------------------------

inline void CMoveHelperClient::SetHost(C_BaseEntity* host)
{
	m_pHost = host;

	// In case any stuff is ever left over, sigh...
	ResetTouchList();
}





#endif  