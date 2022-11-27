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
	 Vector& GetAbsOrigin() 
	{
		typedef Vector& (__thiscall* GetAbsOriginFn)(void*);
		return getvfunc<GetAbsOriginFn>(this, 10)(this);
	}
	
	 Vector& GetAbsAngles(void)  
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

	Vector GetViewPos()
	{
		static auto EyePosition = reinterpret_cast<float* (__thiscall*)(void*, Vector*)>(
			FindPatternV2("client.dll", "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90")
			);

		Vector output; 
		EyePosition(this, &output);
		return output;
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
