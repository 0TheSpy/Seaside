
#ifndef PARTICLEMGR_H
#define PARTICLEMGR_H

#ifdef _WIN32
#pragma once
#endif

#include "imaterial.h"
#include "imaterialsystem.h"
#include "vector.h"
#include "vmatrix.h"
#include "Mathlib.h"
#include "iclientrenderable.h"
#include "clientleafsystem.h"
#include "fasttimer.h"
#include "utllinkedlist.h"
#include "UtlDict.h"
#ifdef WIN32
#include <typeinfo>
#else
#include <typeinfo>
#endif
#include "utlintrusivelist.h"
#include "utlobjectreference.h"
#include "utlstring.h"


class IParticleEffect;
class IClientParticleListener;
struct Particle;
class ParticleDraw;
class CMeshBuilder;
class CUtlMemoryPool;
class CEffectMaterial;
class CParticleSimulateIterator;
class CParticleRenderIterator;
class IThreadPool;
class CParticleSystemDefinition;
class CParticleMgr;
class CNewParticleEffect;
class CParticleCollection;

#define INVALID_MATERIAL_HANDLE	NULL



class CParticleSubTexture;
class CParticleSubTextureGroup;


struct Particle
{
	Particle* m_pPrev, * m_pNext;

	CParticleSubTexture* m_pSubTexture;

	Vector m_Pos;			       
};


typedef CParticleSubTexture* PMaterialHandle;

class CEffectMaterial
{
public:
	CEffectMaterial();

public:
	CParticleSubTextureGroup* m_pGroup;

	Particle m_Particles;
	CEffectMaterial* m_pHashedNext;
};


class CParticleSubTextureGroup
{
public:
	CParticleSubTextureGroup();
	~CParticleSubTextureGroup();

	IMaterial* m_pPageMaterial;
};

class CParticleSubTexture
{
public:
	CParticleSubTexture();

	float m_tCoordMins[2];	         
	float m_tCoordMaxs[2];	        

	CParticleSubTextureGroup* m_pGroup;
	CParticleSubTextureGroup m_DefaultGroup;	          
#ifdef _DEBUG
	char* m_szDebugName;
#endif

	IMaterial* m_pMaterial;
};


abstract_class IParticleEffect
{
	public:

		virtual			~IParticleEffect() {}

		virtual void	Update(float fTimeDelta) {}

		virtual void	StartRender(VMatrix & effectMatrix) {}

		virtual bool	ShouldSimulate() const = 0;
		virtual void	SetShouldSimulate(bool bSim) = 0;
		virtual void	SimulateParticles(CParticleSimulateIterator* pIterator) = 0;

		virtual void	RenderParticles(CParticleRenderIterator* pIterator) = 0;

		virtual void	NotifyRemove() {}

		virtual void	NotifyDestroyParticle(Particle* pParticle) {}

		virtual const Vector& GetSortOrigin() = 0;

		virtual const Vector* GetParticlePosition(Particle* pParticle) { return &pParticle->m_Pos; }

		virtual const char* GetEffectName() { return "???"; }
};

#define REGISTER_EFFECT( effect )														\
	IParticleEffect* effect##_Factory()													\
	{																					\
		return new effect;																\
	}																					\
	struct effect##_RegistrationHelper													\
	{																					\
		effect##_RegistrationHelper()													\
		{																				\
			ParticleMgr()->RegisterEffect( typeid( effect ).name(), effect##_Factory );	\
		}																				\
	};																					\
	static effect##_RegistrationHelper g_##effect##_RegistrationHelper

#define REGISTER_EFFECT_USING_CREATE( effect )											\
	IParticleEffect* effect##_Factory()													\
	{																					\
		return effect::Create( #effect ).GetObject();									\
	}																					\
	struct effect##_RegistrationHelper													\
	{																					\
		effect##_RegistrationHelper()													\
		{																				\
			ParticleMgr()->RegisterEffect( typeid( effect ).name(), effect##_Factory );	\
		}																				\
	};																					\
	static effect##_RegistrationHelper g_##effect##_RegistrationHelper


class CParticleEffectBinding : public CDefaultClientRenderable
{
	friend class CParticleMgr;
	friend class CParticleSimulateIterator;
	friend class CNewParticleEffect;

public:
	CParticleEffectBinding();
	~CParticleEffectBinding();


public:

	void			SimulateParticles(float flTimeDelta);

	PMaterialHandle	FindOrAddMaterial(const char* pMaterialName);

	Particle* AddParticle(int sizeInBytes, PMaterialHandle pMaterial);

	void			SetBBox(const Vector& bbMin, const Vector& bbMax, bool bDisableAutoUpdate = true);
	void			GetWorldspaceBounds(Vector* pMins, Vector* pMaxs);

	const matrix3x4_t& GetLocalSpaceTransform() const;
	void			SetLocalSpaceTransform(const matrix3x4_t& transform);

	bool			EnlargeBBoxToContain(const Vector& pt);

	void			SetDrawThruLeafSystem(int bDraw);

	void			SetDrawBeforeViewModel(int bDraw);

	int				GetRemoveFlag() { return GetFlag(FLAGS_REMOVE); }
	void			SetRemoveFlag() { SetFlag(FLAGS_REMOVE, 1); }

	int				GetAlwaysSimulate() { return GetFlag(FLAGS_ALWAYSSIMULATE); }
	void			SetAlwaysSimulate(int bAlwaysSimulate) { SetFlag(FLAGS_ALWAYSSIMULATE, bAlwaysSimulate); }

	void			SetIsNewParticleSystem(void) { SetFlag(FLAGS_NEW_PARTICLE_SYSTEM, 1); }
	int				WasDrawnPrevFrame() { return GetFlag(FLAGS_DRAWN_PREVFRAME); }
	void			SetWasDrawnPrevFrame(int bWasDrawnPrevFrame) { SetFlag(FLAGS_DRAWN_PREVFRAME, bWasDrawnPrevFrame); }

	int				IsEffectCameraSpace() { return GetFlag(FLAGS_CAMERASPACE); }
	void			SetEffectCameraSpace(int bCameraSpace) { SetFlag(FLAGS_CAMERASPACE, bCameraSpace); }

	int				GetAutoApplyLocalTransform() const { return GetFlag(FLAGS_AUTOAPPLYLOCALTRANSFORM); }
	void			SetAutoApplyLocalTransform(int b) { SetFlag(FLAGS_AUTOAPPLYLOCALTRANSFORM, b); }

	int				GetAutoUpdateBBox() { return GetFlag(FLAGS_AUTOUPDATEBBOX); }
	void			SetAutoUpdateBBox(int bAutoUpdate) { SetFlag(FLAGS_AUTOUPDATEBBOX, bAutoUpdate); }

	int				GetNumActiveParticles();

	void			SetParticleCullRadius(float flMaxParticleRadius);

	int				GetActiveParticleList(int nCount, Particle** ppParticleList);

	void			DetectChanges();

private:
	void			SetFlag(int flag, int bOn) { if (bOn) m_Flags |= flag; else m_Flags &= ~flag; }
	int				GetFlag(int flag) const { return m_Flags & flag; }

	void			Init(CParticleMgr* pMgr, IParticleEffect* pSim);
	void			Term();

	void			RemoveParticle(Particle* pParticle);

	void			StartDrawMaterialParticles(
		CEffectMaterial* pMaterial,
		float flTimeDelta,
		IMesh*& pMesh,
		CMeshBuilder& builder,
		ParticleDraw& particleDraw,
		bool bWireframe);

	int				DrawMaterialParticles(
		bool bBucketSort,
		CEffectMaterial* pMaterial,
		float flTimeDelta,
		bool bWireframe
	);

	void			GrowBBoxFromParticlePositions(CEffectMaterial* pMaterial, bool& bboxSet, Vector& bbMin, Vector& bbMax);

	void			RenderStart(VMatrix& mTempModel, VMatrix& mTempView);
	void			RenderEnd(VMatrix& mModel, VMatrix& mView);

	void			BBoxCalcStart(Vector& bbMin, Vector& bbMax);
	void			BBoxCalcEnd(bool bboxSet, Vector& bbMin, Vector& bbMax);

	void			DoBucketSort(
		CEffectMaterial* pMaterial,
		float* zCoords,
		int nZCoords,
		float minZ,
		float maxZ);

	int				GetRemovalInProgressFlag() { return GetFlag(FLAGS_REMOVALINPROGRESS); }
	void			SetRemovalInProgressFlag() { SetFlag(FLAGS_REMOVALINPROGRESS, 1); }

	int				GetNeedsBBoxUpdate() { return GetFlag(FLAGS_NEEDS_BBOX_UPDATE); }
	void			SetNeedsBBoxUpdate(int bFirstUpdate) { SetFlag(FLAGS_NEEDS_BBOX_UPDATE, bFirstUpdate); }

	int				GetFirstFrameFlag() { return GetFlag(FLAGS_FIRST_FRAME); }
	void			SetFirstFrameFlag(int bFirstUpdate) { SetFlag(FLAGS_FIRST_FRAME, bFirstUpdate); }

	int				WasDrawn() { return GetFlag(FLAGS_DRAWN); }
	void			SetDrawn(int bDrawn) { SetFlag(FLAGS_DRAWN, bDrawn); }

	bool			RecalculateBoundingBox();

	CEffectMaterial* GetEffectMaterial(CParticleSubTexture* pSubTexture);

public:

	virtual const Vector& GetRenderOrigin(void);
	virtual const QAngle& GetRenderAngles(void);
	virtual const matrix3x4_t& RenderableToWorldTransform();
	virtual void					GetRenderBounds(Vector& mins, Vector& maxs);
	virtual bool					ShouldDraw(void);
	virtual int						DrawModel(int flags, const RenderableInstance_t& instance);

private:
	enum
	{
		FLAGS_REMOVE = (1 << 0),	   
		FLAGS_REMOVALINPROGRESS = (1 << 1),          
											FLAGS_NEEDS_BBOX_UPDATE = (1 << 2),	           
											FLAGS_AUTOUPDATEBBOX = (1 << 3),	      
											FLAGS_ALWAYSSIMULATE = (1 << 4),   
											FLAGS_DRAWN = (1 << 5),	          
											FLAGS_DRAWN_PREVFRAME = (1 << 6),	         
																				FLAGS_CAMERASPACE = (1 << 7),	  
																				FLAGS_DRAW_THRU_LEAF_SYSTEM = (1 << 8),	             
																				FLAGS_DRAW_BEFORE_VIEW_MODEL = (1 << 9),              
																				FLAGS_AUTOAPPLYLOCALTRANSFORM = (1 << 10),         
																				FLAGS_FIRST_FRAME = (1 << 11),	                
																				FLAGS_NEW_PARTICLE_SYSTEM = (1 << 12)     
	};


	VMatrix m_LocalSpaceTransform;
	bool m_bLocalSpaceTransformIdentity;	           

	Vector							m_Min;
	Vector							m_Max;

	Vector							m_LastMin;
	Vector							m_LastMax;

	float							m_flParticleCullRadius;

	unsigned short					m_nActiveParticles;

	unsigned short					m_FrameCode;

	unsigned short					m_ListIndex;

	IParticleEffect* m_pSim;
	CParticleMgr* m_pParticleMgr;

	int								m_Flags;

	enum { EFFECT_MATERIAL_HASH_SIZE = 8 };
	CEffectMaterial* m_EffectMaterialHash[EFFECT_MATERIAL_HASH_SIZE];

#ifdef INFESTED_PARTICLES
public:
#endif
	CUtlLinkedList<CEffectMaterial*, unsigned short> m_Materials;

	unsigned short					m_UpdateBBoxCounter;
};


class CParticleLightInfo
{
public:
	Vector	m_vPos;
	Vector	m_vColor;	 
	float	m_flIntensity;
};

typedef IParticleEffect* (*CreateParticleEffectFN)();

enum
{
	TOOLPARTICLESYSTEMID_INVALID = -1,
};


class CParticleCollection;

class CNonDrawingParticleSystem
{
public:
	CNonDrawingParticleSystem* m_pNext;
	CNonDrawingParticleSystem* m_pPrev;
	CParticleCollection* m_pSystem;

	FORCEINLINE CParticleCollection* operator()(void) const
	{
		return m_pSystem;
	}

	FORCEINLINE CParticleCollection* Get(void) const
	{
		return m_pSystem;
	}

	~CNonDrawingParticleSystem(void);
};




class CClientTools;

class CParticleMgr
{
	friend class CParticleEffectBinding;
	friend class CParticleCollection;
	friend class CNonDrawingParticleSystem;
	friend class CClientTools;

public:

	CParticleMgr();
	virtual			~CParticleMgr();

	bool			Init(unsigned long nPreallocatedParticles, IMaterialSystem* pMaterial);

	void			Term();

	void			LevelInit();

	void			RegisterEffect(const char* pEffectType, CreateParticleEffectFN func);
	IParticleEffect* CreateEffect(const char* pEffectType);

	bool			AddEffect(CParticleEffectBinding* pEffect, IParticleEffect* pSim);
	void			RemoveEffect(CParticleEffectBinding* pEffect);

	void			AddEffect(CNewParticleEffect* pEffect);
	void			RemoveEffect(CNewParticleEffect* pEffect);

	void			RemoveAllEffects();

	void			IncrementFrameCode();

	void			Simulate(float fTimeDelta);

	void			PostRender();

	void			DrawBeforeViewModelEffects();

	VMatrix& GetModelView();

	Particle* AllocParticle(int size);
	void			FreeParticle(Particle*);

	PMaterialHandle	GetPMaterial(const char* pMaterialName);
	IMaterial* PMaterialToIMaterial(PMaterialHandle hMaterial);

	void RepairPMaterial(PMaterialHandle hMaterial);

	void GetDirectionalLightInfo(CParticleLightInfo& info) const;
	void SetDirectionalLightInfo(const CParticleLightInfo& info);

	void SpewInfo(bool bDetail);

	void AddEffectListener(IClientParticleListener* pListener);
	void RemoveEffectListener(IClientParticleListener* pListener);

	int AllocateToolParticleEffectId();

	void RemoveAllNewEffects();

	CNewParticleEffect* FirstNewEffect();
	CNewParticleEffect* NextNewEffect(CNewParticleEffect* pEffect);

	void RenderParticleSystems(bool bEnable);
	bool ShouldRenderParticleSystems() const;

	void RemoveOldParticleEffects(float flTime);              
	int GetNumParticles() const { return m_nCurrentParticlesAllocated; }


	CNonDrawingParticleSystem* CreateNonDrawingEffect(const char* pEffectName);

private:
	struct RetireInfo_t
	{
		CParticleCollection* m_pCollection;
		float m_flScreenArea;
		bool m_bFirstFrame;
	};

	void UpdateAllEffects(float flTimeDelta);

	void UpdateNewEffects(float flTimeDelta);				    

	void SpewActiveParticleSystems();

	CParticleSubTextureGroup* FindOrAddSubTextureGroup(IMaterial* pPageMaterial);

	int ComputeParticleDefScreenArea(int nInfoCount, RetireInfo_t* pInfo, float* pTotalArea, CParticleSystemDefinition* pDef,
		const CViewSetup& view, const VMatrix& worldToPixels, float flFocalDist);

	bool RetireParticleCollections(CParticleSystemDefinition* pDef, int nCount, RetireInfo_t* pInfo, float flScreenArea, float flMaxTotalArea);
	void BuildParticleSimList(CUtlVector< CNewParticleEffect* >& list);
	bool EarlyRetireParticleSystems(int nCount, CNewParticleEffect** ppEffects);
	static int RetireSort(const void* p1, const void* p2);

private:

	int m_nCurrentParticlesAllocated;

	CParticleLightInfo m_DirectionalLight;

	unsigned short					m_FrameCode;

	bool							m_bUpdatingEffects;
	bool							m_bRenderParticleEffects;

	CUtlLinkedList<CParticleEffectBinding*, unsigned short>		m_Effects;

	CUtlIntrusiveDList< CNewParticleEffect > m_NewEffects;
	CUtlIntrusiveDList< CNonDrawingParticleSystem > m_NonDrawingParticleSystems;



	CUtlVector< IClientParticleListener*> m_effectListeners;

	IMaterialSystem* m_pMaterialSystem;

	VMatrix							m_mModelView;

	CUtlVector<CParticleSubTextureGroup*>				m_SubTextureGroups;	    
	CUtlDict<CParticleSubTexture*, unsigned short>		m_SubTextures;		    
	CParticleSubTexture m_DefaultInvalidSubTexture;         

	CUtlMap< const char*, CreateParticleEffectFN > m_effectFactories;

	int m_nToolParticleEffectId;

	IThreadPool* m_pThreadPool[2];
};

inline int CParticleMgr::AllocateToolParticleEffectId()
{
	return m_nToolParticleEffectId++;
}

class IClientParticleListener
{
public:
	virtual void OnParticleEffectAdded(IParticleEffect* pEffect) = 0;
	virtual void OnParticleEffectRemoved(IParticleEffect* pEffect) = 0;
};



float	Helper_GetTime();
float	Helper_GetFrameTime();
float	Helper_RandomFloat(float minVal, float maxVal);
int		Helper_RandomInt(int minVal, int maxVal);



inline VMatrix& CParticleMgr::GetModelView()
{
	return m_mModelView;
}



inline const matrix3x4_t& CParticleEffectBinding::GetLocalSpaceTransform() const
{
	return m_LocalSpaceTransform.As3x4();
}



CParticleMgr* ParticleMgr();




struct StandardParticle_t : public Particle
{
	void			SetColor(float r, float g, float b);
	void			SetAlpha(float a);

	Vector			m_Velocity;

	float			m_Lifetime;

	unsigned char	m_EffectData;	            
	unsigned short	m_EffectDataWord;

	unsigned char	m_Color[4];		         
};


inline void TransformParticle(const VMatrix& vMat, const Vector& vIn, Vector& vOut)
{
	vOut.x = vMat.m[0][0] * vIn.x + vMat.m[0][1] * vIn.y + vMat.m[0][2] * vIn.z + vMat.m[0][3];
	vOut.y = vMat.m[1][0] * vIn.x + vMat.m[1][1] * vIn.y + vMat.m[1][2] * vIn.z + vMat.m[1][3];
	vOut.z = vMat.m[2][0] * vIn.x + vMat.m[2][1] * vIn.y + vMat.m[2][2] * vIn.z + vMat.m[2][3];
}


inline void StandardParticle_t::SetColor(float r, float g, float b)
{
	m_Color[0] = (unsigned char)(r * 255.9f);
	m_Color[1] = (unsigned char)(g * 255.9f);
	m_Color[2] = (unsigned char)(b * 255.9f);
}

inline void StandardParticle_t::SetAlpha(float a)
{
	m_Color[3] = (unsigned char)(a * 255.9f);
}



inline void UnlinkParticle(Particle* pParticle)
{
	pParticle->m_pPrev->m_pNext = pParticle->m_pNext;
	pParticle->m_pNext->m_pPrev = pParticle->m_pPrev;
}

inline void InsertParticleBefore(Particle* pInsert, Particle* pNext)
{
	pInsert->m_pNext = pNext;
	pInsert->m_pPrev = pNext->m_pPrev;
	pInsert->m_pNext->m_pPrev = pInsert->m_pPrev->m_pNext = pInsert;
}

inline void InsertParticleAfter(Particle* pInsert, Particle* pPrev)
{
	pInsert->m_pPrev = pPrev;
	pInsert->m_pNext = pPrev->m_pNext;

	pInsert->m_pNext->m_pPrev = pInsert->m_pPrev->m_pNext = pInsert;
}

inline void SwapParticles(Particle* pPrev, Particle* pCur)
{
	UnlinkParticle(pCur);
	InsertParticleBefore(pCur, pPrev);
}


#include "particle_iterators.h"


#endif
