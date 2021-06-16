#ifndef COLLISIONPROPERTY_H
#define COLLISIONPROPERTY_H
#ifdef _WIN32
#pragma once
#endif

#include "networkvar.h"
#include "ICollideable.h"
#include "vector.h"
#include "ispatialpartition.h"


class CBaseEntity;
class IHandleEntity;
class QAngle;
class Vector;
struct Ray_t;
class IPhysicsObject;


void UpdateDirtySpatialPartitionEntities();


enum SurroundingBoundsType_t
{
	USE_OBB_COLLISION_BOUNDS = 0,
	USE_BEST_COLLISION_BOUNDS,		       
	USE_HITBOXES,
	USE_SPECIFIED_BOUNDS,
	USE_GAME_CODE,
	USE_ROTATION_EXPANDED_BOUNDS,
	USE_COLLISION_BOUNDS_NEVER_VPHYSICS,

	SURROUNDING_TYPE_BIT_COUNT = 3
};


class CCollisionProperty : public ICollideable
{
	DECLARE_CLASS_NOBASE(CCollisionProperty);
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_PREDICTABLE();

#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif

public:
	CCollisionProperty();
	~CCollisionProperty();

	void Init(CBaseEntity* pEntity);

	virtual IHandleEntity* GetEntityHandle();
	virtual const Vector& OBBMinsPreScaled() const { return m_vecMinsPreScaled.Get(); }
	virtual const Vector& OBBMaxsPreScaled() const { return m_vecMaxsPreScaled.Get(); }
	virtual const Vector& OBBMins() const { return m_vecMins.Get(); }
	virtual const Vector& OBBMaxs() const { return m_vecMaxs.Get(); }
	virtual void			WorldSpaceTriggerBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs) const;
	virtual bool			TestCollision(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr);
	virtual bool			TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr);
	virtual int				GetCollisionModelIndex();
	virtual const model_t* GetCollisionModel();
	virtual const Vector& GetCollisionOrigin() const;
	virtual const QAngle& GetCollisionAngles() const;
	virtual const matrix3x4_t& CollisionToWorldTransform() const;
	virtual SolidType_t		GetSolid() const;
	virtual int				GetSolidFlags() const;
	virtual IClientUnknown* GetIClientUnknown();
	virtual int				GetCollisionGroup() const;
	virtual void			WorldSpaceSurroundingBounds(Vector* pVecMins, Vector* pVecMaxs);
	virtual bool			ShouldTouchTrigger(int triggerSolidFlags) const;
	virtual const matrix3x4_t* GetRootParentToWorldTransform() const;

public:
	void			CreatePartitionHandle();
	void			DestroyPartitionHandle();
	unsigned short	GetPartitionHandle() const;

	void			MarkPartitionHandleDirty();

	void			SetCollisionBounds(const Vector& mins, const Vector& maxs);

	void			RefreshScaledCollisionBounds(void);

	void			UseTriggerBounds(bool bEnable, float flBloat = 0.0f);

	void			SetSurroundingBoundsType(SurroundingBoundsType_t type, const Vector* pMins = NULL, const Vector* pMaxs = NULL);

	void			SetSolid(SolidType_t val);

	const Vector& OBBSize() const;

	float			BoundingRadius() const;
	float			BoundingRadius2D() const;

	const Vector& OBBCenter() const;

	const Vector& WorldSpaceCenter() const;

	void			ClearSolidFlags(void);
	void			RemoveSolidFlags(int flags);
	void			AddSolidFlags(int flags);
	bool			IsSolidFlagSet(int flagMask) const;
	void		 	SetSolidFlags(int flags);
	bool			IsSolid() const;

	void			UpdatePartition();

	bool			IsBoundsDefinedInEntitySpace() const;

	const Vector& CollisionToWorldSpace(const Vector& in, Vector* pResult) const;

	const Vector& WorldToCollisionSpace(const Vector& in, Vector* pResult) const;

	const Vector& WorldDirectionToCollisionSpace(const Vector& in, Vector* pResult) const;

	void			RandomPointInBounds(const Vector& vecNormalizedMins, const Vector& vecNormalizedMaxs, Vector* pPoint) const;

	bool			IsPointInBounds(const Vector& vecWorldPt) const;

	void			WorldSpaceAABB(Vector* pWorldMins, Vector* pWorldMaxs) const;

	const Vector& CollisionSpaceMins(void) const;

	const Vector& CollisionSpaceMaxs(void) const;

	const Vector& NormalizedToCollisionSpace(const Vector& in, Vector* pResult) const;

	const Vector& NormalizedToWorldSpace(const Vector& in, Vector* pResult) const;

	const Vector& WorldToNormalizedSpace(const Vector& in, Vector* pResult) const;

	const Vector& CollisionToNormalizedSpace(const Vector& in, Vector* pResult) const;

	void			CalcNearestPoint(const Vector& vecWorldPt, Vector* pVecNearestWorldPt) const;

	float			CalcDistanceFromPoint(const Vector& vecWorldPt) const;

	bool			DoesRotationInvalidateSurroundingBox() const;

	bool			DoesVPhysicsInvalidateSurroundingBox() const;

	void			MarkSurroundingBoundsDirty();

	float			ComputeSupportMap(const Vector& vecDirection) const;

private:
	void CollisionAABBToWorldAABB(const Vector& entityMins, const Vector& entityMaxs, Vector* pWorldMins, Vector* pWorldMaxs) const;

	void ComputeVPhysicsSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs);

	bool ComputeHitboxSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs);
	bool ComputeEntitySpaceHitboxSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs);

	void ComputeCollisionSurroundingBox(bool bUseVPhysics, Vector* pVecWorldMins, Vector* pVecWorldMaxs);

	void ComputeRotationExpandedBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs);

	void ComputeSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs);

	void CheckForUntouch();

	void UpdateServerPartitionMask();

	CBaseEntity* GetOuter();
	const CBaseEntity* GetOuter() const;

private:
	CBaseEntity* m_pOuter;

	CNetworkVector(m_vecMinsPreScaled);
	CNetworkVector(m_vecMaxsPreScaled);
	CNetworkVector(m_vecMins);
	CNetworkVector(m_vecMaxs);
	float m_flRadius;

	CNetworkVar(unsigned short, m_usSolidFlags);

	SpatialPartitionHandle_t m_Partition;
	CNetworkVar(unsigned char, m_nSurroundType);

	CNetworkVar(unsigned char, m_nSolidType);
	CNetworkVar(unsigned char, m_triggerBloat);

	CNetworkVector(m_vecSpecifiedSurroundingMinsPreScaled);
	CNetworkVector(m_vecSpecifiedSurroundingMaxsPreScaled);
	CNetworkVector(m_vecSpecifiedSurroundingMins);
	CNetworkVector(m_vecSpecifiedSurroundingMaxs);

#if 0
	short	m_surroundingMins[3];
	short	m_surroundingMaxs[3];
#else
	Vector	m_vecSurroundingMins;
	Vector	m_vecSurroundingMaxs;
#endif

	friend class CBaseEntity;
};


#ifdef CLIENT_DLL
EXTERN_RECV_TABLE(DT_CollisionProperty);
#else
EXTERN_SEND_TABLE(DT_CollisionProperty);
#endif


inline CBaseEntity* CCollisionProperty::GetOuter()
{
	return m_pOuter;
}

inline const CBaseEntity* CCollisionProperty::GetOuter() const
{
	return m_pOuter;
}


inline unsigned short CCollisionProperty::GetPartitionHandle() const
{
	return m_Partition;
}


inline const Vector& CCollisionProperty::OBBSize() const
{
	Vector& temp = AllocTempVector();
	VectorSubtract(m_vecMaxs, m_vecMins, temp);
	return temp;
}


inline float CCollisionProperty::BoundingRadius() const
{
	return m_flRadius;
}


inline bool CCollisionProperty::IsBoundsDefinedInEntitySpace() const
{
	return ((m_usSolidFlags & FSOLID_FORCE_WORLD_ALIGNED) == 0) &&
		(m_nSolidType != SOLID_BBOX) && (m_nSolidType != SOLID_NONE);
}

inline void CCollisionProperty::ClearSolidFlags(void)
{
	SetSolidFlags(0);
}

inline void CCollisionProperty::RemoveSolidFlags(int flags)
{
	SetSolidFlags(m_usSolidFlags & ~flags);
}

inline void CCollisionProperty::AddSolidFlags(int flags)
{
	SetSolidFlags(m_usSolidFlags | flags);
}

inline int CCollisionProperty::GetSolidFlags(void) const
{
	return m_usSolidFlags;
}

inline bool CCollisionProperty::IsSolidFlagSet(int flagMask) const
{
	return (m_usSolidFlags & flagMask) != 0;
}

inline bool CCollisionProperty::IsSolid() const
{
	return ::IsSolid((SolidType_t)(unsigned char)m_nSolidType, m_usSolidFlags);
}


inline const Vector& CCollisionProperty::OBBCenter() const
{
	Vector& vecResult = AllocTempVector();
	VectorLerp(m_vecMins, m_vecMaxs, 0.5f, vecResult);
	return vecResult;
}


inline const Vector& CCollisionProperty::WorldSpaceCenter() const
{
	Vector& vecResult = AllocTempVector();
	CollisionToWorldSpace(OBBCenter(), &vecResult);
	return vecResult;
}


inline const Vector& CCollisionProperty::CollisionToWorldSpace(const Vector& in, Vector* pResult) const
{
	if (!IsBoundsDefinedInEntitySpace() || (GetCollisionAngles() == vec3_angle))
	{
		VectorAdd(in, GetCollisionOrigin(), *pResult);
	}
	else
	{
		VectorTransform(in, CollisionToWorldTransform(), *pResult);
	}
	return *pResult;
}


inline const Vector& CCollisionProperty::WorldToCollisionSpace(const Vector& in, Vector* pResult) const
{
	if (!IsBoundsDefinedInEntitySpace() || (GetCollisionAngles() == vec3_angle))
	{
		VectorSubtract(in, GetCollisionOrigin(), *pResult);
	}
	else
	{
		VectorITransform(in, CollisionToWorldTransform(), *pResult);
	}
	return *pResult;
}


inline const Vector& CCollisionProperty::WorldDirectionToCollisionSpace(const Vector& in, Vector* pResult) const
{
	if (!IsBoundsDefinedInEntitySpace() || (GetCollisionAngles() == vec3_angle))
	{
		*pResult = in;
	}
	else
	{
		VectorIRotate(in, CollisionToWorldTransform(), *pResult);
	}
	return *pResult;
}


inline void CCollisionProperty::WorldSpaceAABB(Vector* pWorldMins, Vector* pWorldMaxs) const
{
	CollisionAABBToWorldAABB(m_vecMins, m_vecMaxs, pWorldMins, pWorldMaxs);
}


inline const Vector& CCollisionProperty::CollisionSpaceMins(void) const
{
	return m_vecMins;
}

inline const Vector& CCollisionProperty::CollisionSpaceMaxs(void) const
{
	return m_vecMaxs;
}


inline bool CCollisionProperty::DoesRotationInvalidateSurroundingBox() const
{
	if (IsSolidFlagSet(FSOLID_ROOT_PARENT_ALIGNED))
		return true;

	switch (m_nSurroundType)
	{
	case USE_COLLISION_BOUNDS_NEVER_VPHYSICS:
	case USE_OBB_COLLISION_BOUNDS:
	case USE_BEST_COLLISION_BOUNDS:
		return IsBoundsDefinedInEntitySpace();

	case USE_HITBOXES:
	case USE_GAME_CODE:
		return true;

	case USE_ROTATION_EXPANDED_BOUNDS:
	case USE_SPECIFIED_BOUNDS:
		return false;

	default:
		Assert(0);
		return true;
	}
}


#endif  