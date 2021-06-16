#ifndef RAGDOLL_SHARED_H
#define RAGDOLL_SHARED_H
#ifdef _WIN32
#pragma once
#endif

class IPhysicsObject;
class IPhysicsConstraint;
class IPhysicsConstraintGroup;
class IPhysicsCollision;
class IPhysicsEnvironment;
class IPhysicsSurfaceProps;
struct matrix3x4_t;

struct vcollide_t;
struct studiohdr_t;
class CStudioHdr;
class CBoneAccessor;

#include "vector.h"
#include "bone_accessor.h"

#define RAGDOLL_MAX_ELEMENTS	24
#define RAGDOLL_INDEX_BITS		5			    

#define CORE_DISSOLVE_FADE_START 0.2f
#define CORE_DISSOLVE_MODEL_FADE_START 0.1f
#define CORE_DISSOLVE_MODEL_FADE_LENGTH 0.05f
#define CORE_DISSOLVE_FADEIN_LENGTH 0.1f

struct ragdollelement_t
{
	Vector				originParentSpace;
	IPhysicsObject* pObject;		      
	IPhysicsConstraint* pConstraint;	         
	int					parentIndex;
};

struct ragdollanimatedfriction_t
{
	float					flFrictionTimeIn;
	float					flFrictionTimeOut;
	float					flFrictionTimeHold;
	int						iMinAnimatedFriction;
	int						iMaxAnimatedFriction;
};

struct ragdoll_t
{
	int						listCount;
	bool					allowStretch;
	bool					unused;
	IPhysicsConstraintGroup* pGroup;
	ragdollelement_t 	list[RAGDOLL_MAX_ELEMENTS];
	int					boneIndex[RAGDOLL_MAX_ELEMENTS];
	ragdollanimatedfriction_t animfriction;
};

struct ragdollparams_t
{
	void* pGameData;
	vcollide_t* pCollide;
	CStudioHdr* pStudioHdr;
	int			modelIndex;
	Vector		forcePosition;
	Vector		forceVector;
	int			forceBoneIndex;
	const matrix3x4_t* pCurrentBones;
	float		jointFrictionScale;
	bool		allowStretch;
	bool		fixedConstraints;
};

class CRagdollLRURetirement : public CAutoGameSystemPerFrame
{
public:
	CRagdollLRURetirement(char const* name) : CAutoGameSystemPerFrame(name)
	{
	}

	virtual void Update(float frametime);
	virtual void FrameUpdatePostEntityThink(void);

	void MoveToTopOfLRU(CBaseAnimating* pRagdoll, bool bImportant = false);
	void SetMaxRagdollCount(int iMaxCount) { m_iMaxRagdolls = iMaxCount; }

	virtual void LevelInitPreEntity(void);
	int CountRagdolls(bool bOnlySimulatingRagdolls) { return bOnlySimulatingRagdolls ? m_iSimulatedRagdollCount : m_iRagdollCount; }

private:
	typedef CHandle<CBaseAnimating> CRagdollHandle;
	CUtlLinkedList< CRagdollHandle > m_LRU;
	CUtlLinkedList< CRagdollHandle > m_LRUImportantRagdolls;

	int m_iMaxRagdolls;
	int m_iSimulatedRagdollCount;
	int m_iRagdollCount;
};

extern CRagdollLRURetirement s_RagdollLRU;

class CRagdollLowViolenceManager
{
public:
	CRagdollLowViolenceManager() { m_bLowViolence = false; }
	void SetLowViolence(const char* pMapName);
	bool IsLowViolence(void) { return m_bLowViolence; }

private:
	bool m_bLowViolence;
};

extern CRagdollLowViolenceManager g_RagdollLVManager;


bool RagdollCreate(ragdoll_t& ragdoll, const ragdollparams_t& params, IPhysicsEnvironment* pPhysEnv);

void RagdollActivate(ragdoll_t& ragdoll, vcollide_t* pCollide, int modelIndex, bool bForceWake = true);
void RagdollSetupCollisions(ragdoll_t& ragdoll, vcollide_t* pCollide, int modelIndex);
void RagdollDestroy(ragdoll_t& ragdoll);

bool RagdollGetBoneMatrix(const ragdoll_t& ragdoll, CBoneAccessor& pBoneToWorld, int objectIndex);

int RagdollExtractBoneIndices(int* boneIndexOut, CStudioHdr* pStudioHdr, vcollide_t* pCollide);

void RagdollComputeExactBbox(const ragdoll_t& ragdoll, const Vector& origin, Vector& outMins, Vector& outMaxs);
bool RagdollIsAsleep(const ragdoll_t& ragdoll);
void RagdollSetupAnimatedFriction(IPhysicsEnvironment* pPhysEnv, ragdoll_t* ragdoll, int iModelIndex);

void RagdollApplyAnimationAsVelocity(ragdoll_t& ragdoll, const matrix3x4_t* pBoneToWorld);
void RagdollApplyAnimationAsVelocity(ragdoll_t& ragdoll, const matrix3x4_t* pPrevBones, const matrix3x4_t* pCurrentBones, float dt);

void RagdollSolveSeparation(ragdoll_t& ragdoll, CBaseEntity* pEntity);

#endif  