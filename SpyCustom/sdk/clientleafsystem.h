#if !defined( CLIENTLEAFSYSTEM_H )
#define CLIENTLEAFSYSTEM_H
#ifdef _WIN32
#pragma once
#endif

#include "IGameSystem.h"
#include "IClientLeafSystem.h"
#include "IVRenderView.h"
#include "mempool.h"
#include "refcount.h"

#include "ivrenderview.h" 

struct WorldListInfo_t;
class IClientRenderable;
class Vector;
class CGameTrace;
typedef CGameTrace trace_t;
struct Ray_t;
class Vector2D;
class CStaticProp;





enum
{
	DETAIL_PROP_RENDER_HANDLE = (ClientRenderHandle_t)0xfffe
};


struct DistanceFadeInfo_t
{
	float m_flMaxDistSqr;		       
	float m_flMinDistSqr;		      
	float m_flFalloffFactor;	       
};


class CClientRenderablesList : public CRefCounted<>
{
	DECLARE_FIXEDSIZE_ALLOCATOR(CClientRenderablesList);

public:
	enum
	{
		MAX_GROUP_ENTITIES = 4096,
		MAX_BONE_SETUP_DEPENDENCY = 64,
	};

	struct CEntry
	{
		IClientRenderable* m_pRenderable;
		unsigned short		m_iWorldListInfoLeaf;       
		RenderableInstance_t m_InstanceData;
		uint8				m_nModelType : 7;		  
		uint8				m_TwoPass : 1;
	};

	DistanceFadeInfo_t	m_DetailFade;
	CEntry				m_RenderGroups[RENDER_GROUP_COUNT][MAX_GROUP_ENTITIES];
	int					m_RenderGroupCounts[RENDER_GROUP_COUNT];
	int					m_nBoneSetupDependencyCount;
	IClientRenderable* m_pBoneSetupDependency[MAX_BONE_SETUP_DEPENDENCY];
};


class CViewModelRenderablesList
{
public:
	enum
	{
		VM_GROUP_OPAQUE = 0,
		VM_GROUP_TRANSLUCENT,
		VM_GROUP_COUNT,
	};

	struct CEntry
	{
		IClientRenderable* m_pRenderable;
		RenderableInstance_t m_InstanceData;
	};

	typedef CUtlVectorFixedGrowable< CEntry, 32 > RenderGroups_t;

	RenderGroups_t	m_RenderGroups[VM_GROUP_COUNT];
};


struct SetupRenderInfo_t
{
	WorldListInfo_t* m_pWorldListInfo;
	CClientRenderablesList* m_pRenderList;
	Vector m_vecRenderOrigin;
	Vector m_vecRenderForward;
	int m_nRenderFrame;
	int m_nDetailBuildFrame;	      
	float m_flRenderDistSq;
	int m_nViewID;
	bool m_bDrawDetailObjects : 1;
	bool m_bDrawTranslucentObjects : 1;

	SetupRenderInfo_t()
	{
		m_bDrawDetailObjects = true;
		m_bDrawTranslucentObjects = true;
	}
};


struct ScreenSizeComputeInfo_t
{
	VMatrix m_matViewProj;
	Vector m_vecViewUp;
	int m_nViewportHeight;
};

void ComputeScreenSizeInfo(ScreenSizeComputeInfo_t* pInfo);
float ComputeScreenSize(const Vector& vecOrigin, float flRadius, const ScreenSizeComputeInfo_t& info);


typedef unsigned short ClientLeafShadowHandle_t;
enum
{
	CLIENT_LEAF_SHADOW_INVALID_HANDLE = (ClientLeafShadowHandle_t)~0
};


abstract_class IClientLeafShadowEnum
{
public:
	virtual void EnumShadow(ClientShadowHandle_t userId) = 0;
};


class CClientLeafSubSystemData
{
public:
	virtual ~CClientLeafSubSystemData(void)
	{
	}
};


#define CLSUBSYSTEM_DETAILOBJECTS 0
#define N_CLSUBSYSTEMS 1



abstract_class IClientLeafSystem : public IClientLeafSystemEngine, public IGameSystemPerFrame
{
public:
	virtual void AddRenderable(IClientRenderable * pRenderable, bool bRenderWithViewModels, RenderableTranslucencyType_t nType, RenderableModelType_t nModelType, uint32 nSplitscreenEnabled = 0xFFFFFFFF) = 0;

	virtual bool IsRenderableInPVS(IClientRenderable* pRenderable) = 0;

	virtual void SetSubSystemDataInLeaf(int leaf, int nSubSystemIdx, CClientLeafSubSystemData* pData) = 0;
	virtual CClientLeafSubSystemData* GetSubSystemDataInLeaf(int leaf, int nSubSystemIdx) = 0;

	virtual void SetDetailObjectsInLeaf(int leaf, int firstDetailObject, int detailObjectCount) = 0;
	virtual void GetDetailObjectsInLeaf(int leaf, int& firstDetailObject, int& detailObjectCount) = 0;

	virtual void DrawDetailObjectsInLeaf(int leaf, int frameNumber, int& firstDetailObject, int& detailObjectCount) = 0;

	virtual bool ShouldDrawDetailObjectsInLeaf(int leaf, int frameNumber) = 0;

	virtual void RenderableChanged(ClientRenderHandle_t handle) = 0;

	virtual void BuildRenderablesList(const SetupRenderInfo_t& info) = 0;

	virtual void CollateViewModelRenderables(CViewModelRenderablesList* pList) = 0;

	virtual void DrawStaticProps(bool enable) = 0;

	virtual void DrawSmallEntities(bool enable) = 0;

	virtual ClientLeafShadowHandle_t AddShadow(ClientShadowHandle_t userId, unsigned short flags) = 0;
	virtual void RemoveShadow(ClientLeafShadowHandle_t h) = 0;

	virtual void ProjectShadow(ClientLeafShadowHandle_t handle, int nLeafCount, const int* pLeafList) = 0;

	virtual void ProjectFlashlight(ClientLeafShadowHandle_t handle, int nLeafCount, const int* pLeafList) = 0;

	virtual void EnumerateShadowsInLeaves(int leafCount, WorldListLeafData_t* pLeaves, IClientLeafShadowEnum* pEnum) = 0;

	virtual int GetRenderableLeaves(ClientRenderHandle_t handle, int leaves[128]) = 0;

	virtual bool GetRenderableLeaf(ClientRenderHandle_t handle, int* pOutLeaf, const int* pInIterator = 0, int* pOutIterator = 0) = 0;

	virtual void EnableAlternateSorting(ClientRenderHandle_t handle, bool bEnable) = 0;

	virtual void RenderWithViewModels(ClientRenderHandle_t handle, bool bEnable) = 0;
	virtual bool IsRenderingWithViewModels(ClientRenderHandle_t handle) const = 0;

	virtual void SetTranslucencyType(ClientRenderHandle_t handle, RenderableTranslucencyType_t nType) = 0;
	virtual RenderableTranslucencyType_t GetTranslucencyType(ClientRenderHandle_t handle) const = 0;
	virtual void SetModelType(ClientRenderHandle_t handle, RenderableModelType_t nType = RENDERABLE_MODEL_UNKNOWN_TYPE) = 0;
	virtual void EnableSplitscreenRendering(ClientRenderHandle_t handle, uint32 nFlags) = 0;

	virtual void EnableRendering(ClientRenderHandle_t handle, bool bEnable) = 0;

	virtual void EnableBloatedBounds(ClientRenderHandle_t handle, bool bEnable) = 0;

	virtual void DisableCachedRenderBounds(ClientRenderHandle_t handle, bool bDisable) = 0;

	virtual void RecomputeRenderableLeaves() = 0;

	virtual void DisableLeafReinsertion(bool bDisable) = 0;

	virtual RenderGroup_t GenerateRenderListEntry(IClientRenderable* pRenderable, CClientRenderablesList::CEntry& entryOut) = 0;
};


extern IClientLeafSystem* g_pClientLeafSystem;
inline IClientLeafSystem* ClientLeafSystem()
{
	return g_pClientLeafSystem;
}


#endif	 
