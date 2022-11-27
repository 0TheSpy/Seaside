#ifndef ENGINE_IENGINETRACE_H
#define ENGINE_IENGINETRACE_H
#ifdef _WIN32
#pragma once
#endif

#include "basehandle.h"
#include "utlvector.h"    
#include "vector4d.h"

#include "bspflags.h"

class Vector;
class IHandleEntity;
struct Ray_t;
class CGameTrace;
typedef CGameTrace trace_t;
class ICollideable;
class QAngle;
class CTraceListData;
class CPhysCollide;
struct cplane_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				       
	TRACE_ENTITIES_ONLY,			        
	TRACE_EVERYTHING_FILTER_PROPS,	                
};

abstract_class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity * pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

#include <functional>
class CTraceFilter : public ITraceFilter
{
	using FilterCallbackFn = std::function<bool(IHandleEntity*, int)>;
public:
	 
	CTraceFilter(const IHandleEntity* pSkipEntity, TraceType_t iTraceType = TRACE_EVERYTHING)
		: pSkip((void*)pSkipEntity), iTraceType(iTraceType) { }

	CTraceFilter(FilterCallbackFn&& checkCallback, TraceType_t iTraceType = TRACE_EVERYTHING)
		: checkCallback(std::move(checkCallback)), iTraceType(iTraceType) { }

	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int )
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
	FilterCallbackFn checkCallback = nullptr;
	TraceType_t iTraceType = TRACE_EVERYTHING;
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}
};


class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		return false;
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_WORLD_ONLY;
	}
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		return false;
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceFilterHitAll : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		return true;
	}
};


abstract_class IEntityEnumerator
{
public:
	virtual bool EnumEntity(IHandleEntity * pHandleEntity) = 0;
};


#define INTERFACEVERSION_ENGINETRACE_SERVER	"EngineTraceServer003"
#define INTERFACEVERSION_ENGINETRACE_CLIENT	"EngineTraceClient003"
abstract_class IEngineTrace
{
public:
	virtual int   GetPointContents(const Vector& vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;


	virtual int   GetPointContents_WorldOnly(const Vector& vecAbsPosition, int contentsMask = MASK_ALL) = 0; 

	virtual int		GetPointContents_Collideable(ICollideable* pCollide, const Vector& vecAbsPosition) = 0;

	virtual void	ClipRayToEntity(const Ray_t& ray, unsigned int fMask, IHandleEntity* pEnt, trace_t* pTrace) = 0;

	virtual void	ClipRayToCollideable(const Ray_t& ray, unsigned int fMask, ICollideable* pCollide, trace_t* pTrace) = 0;

	virtual void    TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace) = 0;

	virtual void	SetupLeafAndEntityListRay(const Ray_t& ray, CTraceListData& traceData) = 0;
	virtual void    SetupLeafAndEntityListBox(const Vector& vecBoxMin, const Vector& vecBoxMax, CTraceListData& traceData) = 0;
	virtual void	TraceRayAgainstLeafAndEntityList(const Ray_t& ray, CTraceListData& traceData, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace) = 0;

	virtual void	SweepCollideable(ICollideable* pCollide, const Vector& vecAbsStart, const Vector& vecAbsEnd,
		const QAngle& vecAngles, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace) = 0;

	virtual void	EnumerateEntities(const Ray_t& ray, bool triggers, IEntityEnumerator* pEnumerator) = 0;

	virtual void	EnumerateEntities(const Vector& vecAbsMins, const Vector& vecAbsMaxs, IEntityEnumerator* pEnumerator) = 0;

	virtual ICollideable* GetCollideable(IHandleEntity* pEntity) = 0;

	virtual int GetStatByIndex(int index, bool bClear) = 0;


	virtual void GetBrushesInAABB(const Vector& vMins, const Vector& vMaxs, CUtlVector<int>* pOutput, int iContentsMask = 0xFFFFFFFF) = 0;

	virtual CPhysCollide* GetCollidableFromDisplacementsInAABB(const Vector& vMins, const Vector& vMaxs) = 0;

	virtual bool GetBrushInfo(int iBrush, CUtlVector<Vector4D>* pPlanesOut, int* pContentsOut) = 0;

	virtual bool PointOutsideWorld(const Vector& ptTest) = 0;           

	virtual int GetLeafContainingPoint(const Vector& ptTest) = 0;
};

#endif  