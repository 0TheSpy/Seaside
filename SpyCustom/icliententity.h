#ifndef ICLIENTENTITY_H
#define ICLIENTENTITY_H
#ifdef _WIN32
#pragma once
#endif


#include "iclientrenderable.h"
#include "iclientnetworkable.h"
#include "iclientthinkable.h"
#include "string_t.h"



struct Ray_t;
class CGameTrace;
typedef CGameTrace trace_t;
class CMouthInfo;
class IClientEntityInternal;
struct SpatializationInfo_t;



abstract_class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:


	virtual void			Release(void) = 0;
	virtual void Unknown000(void) = 0; 
	const Vector& GetAbsOrigin() 
	{
		typedef Vector& (__thiscall* GetAbsOriginFn)(void*);
		return getvfunc<GetAbsOriginFn>(this, 10)(this);
	}
	
	const Vector& GetAbsAngles(void)  
	{
		typedef Vector& (__thiscall* GetAbsAnglesFn)(void*);
		return getvfunc<GetAbsAnglesFn>(this, 11)(this);
	}

	const void SetModelIndex(int index)  
	{
		typedef void (__thiscall* SetModelIndexFn)(void*, int);
		return getvfunc<SetModelIndexFn>(this, 75)(this, index);
	}

	void preDataUpdate(int updateType)
	{
		typedef void(__thiscall* SetModelIndexFn)(void*, int);
		return getvfunc<SetModelIndexFn>(this, 2)(this + sizeof(uintptr_t) * 2, updateType);
	}

	virtual CMouthInfo* GetMouth(void) = 0;

	virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;

	virtual bool			IsBlurred(void) = 0;
#if 0	
	virtual string_t		GetModelName(void) const = 0;
#endif

	int GetIndex()
	{
		return *(int*)((uintptr_t)this + 0x64);
	}


};


#endif  
