#ifndef RAGDOLL_H
#define RAGDOLL_H

#ifdef _WIN32
#pragma once
#endif

#include "ragdoll_shared.h"

#define RAGDOLL_VISUALIZE	0

class C_BaseEntity;
class CStudioHdr;
struct mstudiobone_t;
class Vector;
class IPhysicsObject;
class CBoneAccessor;

abstract_class IRagdoll
{
public:
	virtual ~IRagdoll() {}

	virtual void RagdollBone(C_BaseEntity * ent, mstudiobone_t * pbones, int boneCount, bool* boneSimulated, CBoneAccessor & pBoneToWorld) = 0;
	virtual const Vector& GetRagdollOrigin() = 0;
	virtual void GetRagdollBounds(Vector& mins, Vector& maxs) = 0;
	virtual int RagdollBoneCount() const = 0;
	virtual IPhysicsObject* GetElement(int elementNum) = 0;
	virtual void DrawWireframe(void) = 0;
	virtual void VPhysicsUpdate(IPhysicsObject* pObject) = 0;
};

class CRagdoll : public IRagdoll
{
public:
	CRagdoll();
	~CRagdoll(void);

	DECLARE_SIMPLE_DATADESC();

	void Init(
		C_BaseEntity* ent,
		CStudioHdr* pstudiohdr,
		const Vector& forceVector,
		int forceBone,
		const matrix3x4_t* pDeltaBones0,
		const matrix3x4_t* pDeltaBones1,
		const matrix3x4_t* pCurrentBonePosition,
		float boneDt);

	virtual void RagdollBone(C_BaseEntity* ent, mstudiobone_t* pbones, int boneCount, bool* boneSimulated, CBoneAccessor& pBoneToWorld);
	virtual const Vector& GetRagdollOrigin();
	virtual void GetRagdollBounds(Vector& theMins, Vector& theMaxs);
	void	BuildRagdollBounds(C_BaseEntity* ent);

	virtual IPhysicsObject* GetElement(int elementNum);
	virtual IPhysicsConstraintGroup* GetConstraintGroup() { return m_ragdoll.pGroup; }
	virtual void DrawWireframe();
	virtual void VPhysicsUpdate(IPhysicsObject* pPhysics);
	virtual int RagdollBoneCount() const { return m_ragdoll.listCount; }


	void	SetInitialBonePosition(CStudioHdr* pstudiohdr, const CBoneAccessor& pDesiredBonePosition);

	bool IsValid() { return m_ragdoll.listCount > 0; }
	bool IsAsleep(void) const { return m_allAsleep; }

	void ResetRagdollSleepAfterTime(void);
	float GetLastVPhysicsUpdateTime() const { return m_lastUpdate; }

private:

	void			CheckSettleStationaryRagdoll();
	void			PhysForceRagdollToSleep();

	ragdoll_t	m_ragdoll;
	Vector		m_mins, m_maxs;
	Vector		m_origin;
	float		m_lastUpdate;
	bool		m_allAsleep;
	Vector		m_vecLastOrigin;
	float		m_flLastOriginChangeTime;
	float		m_flAwakeTime;

#if RAGDOLL_VISUALIZE
	matrix3x4_t			m_savedBone1[MAXSTUDIOBONES];
	matrix3x4_t			m_savedBone2[MAXSTUDIOBONES];
	matrix3x4_t			m_savedBone3[MAXSTUDIOBONES];
#endif

public:

	ragdoll_t* GetRagdoll(void) { return &m_ragdoll; }
};


CRagdoll* CreateRagdoll(
	C_BaseEntity* ent,
	CStudioHdr* pstudiohdr,
	const Vector& forceVector,
	int forceBone,
	const matrix3x4_t* pDeltaBones0,
	const matrix3x4_t* pDeltaBones1,
	const matrix3x4_t* pCurrentBonePosition,
	float boneDt);


void NoteRagdollCreationTick(C_BaseEntity* pRagdoll);
bool WasRagdollCreatedOnCurrentTick(C_BaseEntity* pRagdoll);

#endif  