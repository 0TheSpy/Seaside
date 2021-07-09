#ifndef PARTICLES_H
#define PARTICLES_H
#ifdef _WIN32
#pragma once
#endif

#include "mathlib.h"
#include "vector.h"
#include "ssemath.h"
#include "imaterialsystem.h"
#include "dmxelement.h"
#include "utlintrusivelist.h"
#include "random.h"
#include "utlobjectreference.h"
#include "UtlStringMap.h"
#include "utlmap.h"
#include "MaterialSystemUtil.h"
#include "trace.h"
#include "utlsoacontainer.h"

#if defined( CLIENT_DLL )
#include "c_pixel_visibility.h"
#endif

struct DmxElementUnpackStructure_t;
class CParticleSystemDefinition;
class CParticleCollection;
class CParticleOperatorInstance;
class CParticleSystemDictionary;
class CUtlBuffer;
class IParticleOperatorDefinition;
class CSheet;
class CMeshBuilder;
extern float s_pRandomFloats[];


#define MAX_RANDOM_FLOATS 4096
#define RANDOM_FLOAT_MASK ( MAX_RANDOM_FLOATS - 1 )


#define MAX_PARTICLE_ATTRIBUTES 32

#define DEFPARTICLE_ATTRIBUTE( name, bit )						\
	const int PARTICLE_ATTRIBUTE_##name##_MASK = (1 << bit);	\
	const int PARTICLE_ATTRIBUTE_##name = bit;

DEFPARTICLE_ATTRIBUTE(XYZ, 0);

DEFPARTICLE_ATTRIBUTE(LIFE_DURATION, 1);

DEFPARTICLE_ATTRIBUTE(PREV_XYZ, 2);

DEFPARTICLE_ATTRIBUTE(RADIUS, 3);

DEFPARTICLE_ATTRIBUTE(ROTATION, 4);

DEFPARTICLE_ATTRIBUTE(ROTATION_SPEED, 5);

DEFPARTICLE_ATTRIBUTE(TINT_RGB, 6);

DEFPARTICLE_ATTRIBUTE(ALPHA, 7);

DEFPARTICLE_ATTRIBUTE(CREATION_TIME, 8);

DEFPARTICLE_ATTRIBUTE(SEQUENCE_NUMBER, 9);

DEFPARTICLE_ATTRIBUTE(TRAIL_LENGTH, 10);

DEFPARTICLE_ATTRIBUTE(PARTICLE_ID, 11);

DEFPARTICLE_ATTRIBUTE(YAW, 12);

DEFPARTICLE_ATTRIBUTE(SEQUENCE_NUMBER1, 13);

DEFPARTICLE_ATTRIBUTE(HITBOX_INDEX, 14);

DEFPARTICLE_ATTRIBUTE(HITBOX_RELATIVE_XYZ, 15);

DEFPARTICLE_ATTRIBUTE(ALPHA2, 16);

DEFPARTICLE_ATTRIBUTE(TRACE_P0, 17);						    
DEFPARTICLE_ATTRIBUTE(TRACE_P1, 18);						    
DEFPARTICLE_ATTRIBUTE(TRACE_HIT_T, 19);					   
DEFPARTICLE_ATTRIBUTE(TRACE_HIT_NORMAL, 20);				      


#define MAX_PARTICLE_CONTROL_POINTS 64

#define ATTRIBUTES_WHICH_ARE_VEC3S_MASK ( PARTICLE_ATTRIBUTE_TRACE_P0_MASK | PARTICLE_ATTRIBUTE_TRACE_P1_MASK | \
										  PARTICLE_ATTRIBUTE_TRACE_HIT_NORMAL | PARTICLE_ATTRIBUTE_XYZ_MASK | \
                                          PARTICLE_ATTRIBUTE_PREV_XYZ_MASK | PARTICLE_ATTRIBUTE_TINT_RGB_MASK | \
                                          PARTICLE_ATTRIBUTE_HITBOX_RELATIVE_XYZ_MASK )
#define ATTRIBUTES_WHICH_ARE_0_TO_1 (PARTICLE_ATTRIBUTE_ALPHA_MASK | PARTICLE_ATTRIBUTE_ALPHA2_MASK)
#define ATTRIBUTES_WHICH_ARE_ANGLES (PARTICLE_ATTRIBUTE_ROTATION_MASK | PARTICLE_ATTRIBUTE_YAW_MASK )
#define ATTRIBUTES_WHICH_ARE_INTS (PARTICLE_ATTRIBUTE_PARTICLE_ID_MASK | PARTICLE_ATTRIBUTE_HITBOX_INDEX_MASK )

#if defined( _X360 )
#define MAX_PARTICLES_IN_A_SYSTEM 2000
#else
#define MAX_PARTICLES_IN_A_SYSTEM 5000
#endif

#define MEASURE_PARTICLE_PERF 0


enum ParticleFunctionType_t
{
	FUNCTION_RENDERER = 0,
	FUNCTION_OPERATOR,
	FUNCTION_INITIALIZER,
	FUNCTION_EMITTER,
	FUNCTION_CHILDREN,	                     
	FUNCTION_FORCEGENERATOR,
	FUNCTION_CONSTRAINT,
	PARTICLE_FUNCTION_COUNT
};

struct CParticleVisibilityInputs
{
	float	m_flCameraBias;
	float	m_flInputMin;
	float	m_flInputMax;
	float	m_flAlphaScaleMin;
	float	m_flAlphaScaleMax;
	float	m_flRadiusScaleMin;
	float	m_flRadiusScaleMax;
	float	m_flProxyRadius;
	float	m_flBBoxScale;
	bool	m_bUseBBox;
	int		m_nCPin;
};

struct ModelHitBoxInfo_t
{
	Vector m_vecBoxMins;
	Vector m_vecBoxMaxes;
	matrix3x4_t m_Transform;
};

class CModelHitBoxesInfo
{
public:
	float m_flLastUpdateTime;
	float m_flPrevLastUpdateTime;
	int m_nNumHitBoxes;
	int m_nNumPrevHitBoxes;
	ModelHitBoxInfo_t* m_pHitBoxes;
	ModelHitBoxInfo_t* m_pPrevBoxes;

	bool CurAndPrevValid(void) const
	{
		return (m_nNumHitBoxes && (m_nNumPrevHitBoxes == m_nNumHitBoxes));
	}

	CModelHitBoxesInfo(void)
	{
		m_flLastUpdateTime = -1;
		m_nNumHitBoxes = 0;
		m_nNumPrevHitBoxes = 0;
		m_pHitBoxes = NULL;
		m_pPrevBoxes = NULL;
	}

	~CModelHitBoxesInfo(void)
	{
		if (m_pHitBoxes)
			delete[] m_pHitBoxes;
		if (m_pPrevBoxes)
			delete[] m_pPrevBoxes;
	}

};




#define PARTICLE_SYSTEM_QUERY_INTERFACE_VERSION "VParticleSystemQuery001"

class IParticleSystemQuery : public IAppSystem
{
public:
	virtual void GetLightingAtPoint(const Vector& vecOrigin, Color& tint) = 0;
	virtual void TraceLine(const Vector& vecAbsStart,
		const Vector& vecAbsEnd, unsigned int mask,
		const class IHandleEntity* ignore,
		int collisionGroup,
		CBaseTrace* ptr) = 0;

	virtual bool MovePointInsideControllingObject(CParticleCollection* pParticles,
		void* pObject,
		Vector* pPnt)
	{
		return true;
	}

	virtual	bool IsPointInControllingObjectHitBox(
		CParticleCollection* pParticles,
		int nControlPointNumber, Vector vecPos, bool bBBoxOnly = false)
	{
		return true;
	}

	virtual int GetCollisionGroupFromName(const char* pszCollisionGroupName)
	{
		return 0;											  
	}

	virtual void GetRandomPointsOnControllingObjectHitBox(
		CParticleCollection* pParticles,
		int nControlPointNumber,
		int nNumPtsOut,
		float flBBoxScale,
		int nNumTrysToGetAPointInsideTheModel,
		Vector* pPntsOut,
		Vector vecDirectionBias,
		Vector* pHitBoxRelativeCoordOut = NULL,
		int* pHitBoxIndexOut = NULL) = 0;


	virtual int GetControllingObjectHitBoxInfo(
		CParticleCollection* pParticles,
		int nControlPointNumber,
		int nBufSize,										     
		ModelHitBoxInfo_t* pHitBoxOutputBuffer)
	{
		return 0;
	}

	virtual Vector GetLocalPlayerPos(void)
	{
		return vec3_origin;
	}

	virtual void GetLocalPlayerEyeVectors(Vector* pForward, Vector* pRight = NULL, Vector* pUp = NULL)
	{
		*pForward = vec3_origin;
		*pRight = vec3_origin;
		*pUp = vec3_origin;
	}

	virtual float GetPixelVisibility(int* pQueryHandle, const Vector& vecOrigin, float flScale) = 0;

	virtual void SetUpLightingEnvironment(const Vector& pos)
	{
	}
};


typedef int ParticleSystemHandle_t;

class CParticleSystemMgr
{
public:
	CParticleSystemMgr();
	~CParticleSystemMgr();

	bool Init(IParticleSystemQuery* pQuery);

	void AddBuiltinSimulationOperators(void);
	void AddBuiltinRenderingOperators(void);



	void AddParticleOperator(ParticleFunctionType_t nOpType, IParticleOperatorDefinition* pOpFactory);

	bool ReadParticleConfigFile(const char* pFileName, bool bPrecache, bool bDecommitTempMemory = true);
	bool ReadParticleConfigFile(CUtlBuffer& buf, bool bPrecache, bool bDecommitTempMemory = true, const char* pFileName = NULL);
	void DecommitTempMemory();

	bool WriteParticleConfigFile(const char* pParticleSystemName, CUtlBuffer& buf, bool bPreventNameBasedLookup = false);
	bool WriteParticleConfigFile(const DmObjectId_t& id, CUtlBuffer& buf, bool bPreventNameBasedLookup = false);

	CParticleCollection* CreateParticleCollection(const char* pParticleSystemName, float flDelay = 0.0f, int nRandomSeed = 0);

	CParticleCollection* CreateParticleCollection(const DmObjectId_t& id, float flDelay = 0.0f, int nRandomSeed = 0);

	bool IsParticleSystemDefined(const char* pParticleSystemName);
	bool IsParticleSystemDefined(const DmObjectId_t& id);

	ParticleSystemHandle_t GetParticleSystemIndex(const char* pParticleSystemName);

	const char* GetParticleSystemNameFromIndex(ParticleSystemHandle_t iIndex);

	int GetParticleSystemCount(void);

	CUtlVector< IParticleOperatorDefinition*>& GetAvailableParticleOperatorList(ParticleFunctionType_t nWhichList);

	const DmxElementUnpackStructure_t* GetParticleSystemDefinitionUnpackStructure();

	void ShouldLoadSheets(bool bLoadSheets);
	CSheet* FindOrLoadSheet(char const* pszFname, ITexture* pTexture);
	CSheet* FindOrLoadSheet(IMaterial* pMaterial);
	void FlushAllSheets(void);


	void ResetRenderCache(void);
	void AddToRenderCache(CParticleCollection* pParticles);
	void DrawRenderCache(bool bShadowDepth);

	IParticleSystemQuery* Query(void) { return m_pQuery; }

	const char* GetParticleFieldName(int nParticleField) const;

	CParticleSystemDefinition* FindParticleSystem(const char* pName);
	CParticleSystemDefinition* FindParticleSystem(const DmObjectId_t& id);

	void CommitProfileInformation(bool bCommit);			      

	void DumpProfileInformation(void);					  

	void PrecacheParticleSystem(const char* pName);
	void UncacheAllParticleSystems();

	void SetLastSimulationTime(float flTime);
	float GetLastSimulationTime() const;

	int Debug_GetTotalParticleCount() const;
	bool Debug_FrameWarningNeededTestAndReset();
	float ParticleThrottleScaling() const;		            
	bool ParticleThrottleRandomEnable() const;	            

	void TallyParticlesRendered(int nVertexCount, int nIndexCount = 0);

private:
	struct RenderCache_t
	{
		IMaterial* m_pMaterial;
		CUtlVector< CParticleCollection* > m_ParticleCollections;
	};

	struct BatchStep_t
	{
		CParticleCollection* m_pParticles;
		CParticleOperatorInstance* m_pRenderer;
		void* m_pContext;
		int m_nFirstParticle;
		int m_nParticleCount;
		int m_nVertCount;
	};

	struct Batch_t
	{
		int m_nVertCount;
		int m_nIndexCount;
		CUtlVector< BatchStep_t > m_BatchStep;
	};

	bool ReadParticleDefinitions(CUtlBuffer& buf, const char* pFileName, bool bPrecache, bool bDecommitTempMemory);
	void AddParticleSystem(CDmxElement* pParticleSystem);

	CDmxElement* CreateParticleDmxElement(const DmObjectId_t& id);
	CDmxElement* CreateParticleDmxElement(const char* pParticleSystemName);

	bool WriteParticleConfigFile(CDmxElement* pParticleSystem, CUtlBuffer& buf, bool bPreventNameBasedLookup);

	void BuildBatchList(int iRenderCache, IMatRenderContext* pRenderContext, CUtlVector< Batch_t >& batches);

	CUtlVector<IParticleOperatorDefinition*> m_ParticleOperators[PARTICLE_FUNCTION_COUNT];

	CParticleSystemDictionary* m_pParticleSystemDictionary;

	typedef CUtlStringMap< CSheet* > SheetsCache_t;
	SheetsCache_t m_SheetList;

	void AttachKillList(CParticleCollection* pParticles);
	void DetachKillList(CParticleCollection* pParticles);

	CParticleCollection* m_pVisualizedParticles;
	DmObjectId_t m_VisualizedOperatorId;
	IParticleSystemQuery* m_pQuery;
	CUtlVector< RenderCache_t > m_RenderCache;
	IMaterial* m_pShadowDepthMaterial;
	float m_flLastSimulationTime;

	bool m_bDidInit;
	bool m_bUsingDefaultQuery;
	bool m_bShouldLoadSheets;

	int m_nNumFramesMeasured;

	enum { c_nNumFramesTracked = 10 };
	int		m_nParticleVertexCountHistory[c_nNumFramesTracked];
	float	m_fParticleCountScaling;
	int		m_nParticleIndexCount;
	int		m_nParticleVertexCount;
	bool	m_bFrameWarningNeeded;

	friend class CParticleSystemDefinition;
	friend class CParticleCollection;
};

extern CParticleSystemMgr* g_pParticleSystemMgr;


enum ParticleOperatorId_t
{
	OPERATOR_GENERIC = -2,		         
	OPERATOR_SINGLETON = -1,	            

	OPERATOR_PI_POSITION,		         
	OPERATOR_PI_RADIUS,
	OPERATOR_PI_ALPHA,
	OPERATOR_PI_TINT_RGB,
	OPERATOR_PI_ROTATION,
	OPERATOR_PI_YAW,

	OPERATOR_ID_COUNT,
};


class IParticleOperatorDefinition
{
public:
	virtual const char* GetName() const = 0;
	virtual CParticleOperatorInstance* CreateInstance(const DmObjectId_t& id) const = 0;
	virtual const DmxElementUnpackStructure_t* GetUnpackStructure() const = 0;
	virtual ParticleOperatorId_t GetId() const = 0;
	virtual bool IsObsolete() const = 0;
	virtual size_t GetClassSize() const = 0;

#if MEASURE_PARTICLE_PERF
	float m_flMaxExecutionTime;
	float m_flTotalExecutionTime;
	float m_flUncomittedTime;

	FORCEINLINE void RecordExecutionTime(float flETime)
	{
		m_flUncomittedTime += flETime;
		m_flMaxExecutionTime = MAX(m_flMaxExecutionTime, flETime);
	}

	FORCEINLINE float TotalRecordedExecutionTime(void) const
	{
		return m_flTotalExecutionTime;
	}

	FORCEINLINE float MaximumRecordedExecutionTime(void) const
	{
		return m_flMaxExecutionTime;
	}
#endif
};


class CParticleOperatorInstance
{
public:
	void* operator new(size_t nSize);
	void* operator new(size_t size, int nBlockUse, const char* pFileName, int nLine);
	void operator delete(void* pData);
	void operator delete(void* p, int nBlockUse, const char* pFileName, int nLine);

	virtual void InitParams(CParticleSystemDefinition* pDef, CDmxElement* pElement)
	{
	}

	virtual size_t GetRequiredContextBytes() const
	{
		return 0;
	}

	virtual void InitializeContextData(CParticleCollection* pParticles, void* pContext) const
	{
	}

	virtual uint32 GetWrittenAttributes(void) const = 0;
	virtual uint32 GetReadAttributes(void) const = 0;
	virtual uint64 GetReadControlPointMask() const
	{
		return 0;
	}

	virtual uint32 GetReadInitialAttributes(void) const
	{
		return 0;
	}

	virtual void Operate(CParticleCollection* pParticles, float flOpStrength, void* pContext) const
	{
	}

	virtual void Render(IMatRenderContext* pRenderContext,
		CParticleCollection* pParticles, void* pContext) const
	{
	}

	virtual bool IsBatchable() const
	{
		return true;
	}

	virtual void RenderUnsorted(CParticleCollection* pParticles, void* pContext, IMatRenderContext* pRenderContext, CMeshBuilder& meshBuilder, int nVertexOffset, int nFirstParticle, int nParticleCount) const
	{
	}

	virtual int GetParticlesToRender(CParticleCollection* pParticles, void* pContext, int nFirstParticle, int nRemainingVertices, int nRemainingIndices, int* pVertsUsed, int* pIndicesUsed) const
	{
		*pVertsUsed = 0;
		*pIndicesUsed = 0;
		return 0;
	}


	virtual uint32 Emit(CParticleCollection* pParticles, float flOpCurStrength,
		void* pContext) const
	{
		return 0;
	}

	virtual void StopEmission(CParticleCollection* pParticles, void* pContext, bool bInfiniteOnly = false) const
	{
	}
	virtual void StartEmission(CParticleCollection* pParticles, void* pContext, bool bInfiniteOnly = false) const
	{
	}
	virtual void Restart(CParticleCollection* pParticles, void* pContext) {}

	virtual void InitParticleSystem(CParticleCollection* pParticles, void* pContext) const
	{
	}


	virtual void AddForces(FourVectors* AccumulatedForces,
		CParticleCollection* pParticles,
		int nBlocks,
		float flCurStrength,
		void* pContext) const
	{
	}


	virtual void SetupConstraintPerFrameData(CParticleCollection* pParticles,
		void* pContext) const
	{
	}


	virtual bool EnforceConstraint(int nStartBlock,
		int nNumBlocks,
		CParticleCollection* pParticles,
		void* pContext,
		int nNumValidParticlesInLastChunk) const
	{
		return false;
	}

	virtual bool IsFinalConstraint(void) const
	{
		return false;
	}

	virtual bool InitMultipleOverride()
	{
		return false;
	}


	virtual bool IsScrubSafe()
	{
		return false;
	}

	virtual void InitNewParticlesScalar(CParticleCollection* pParticles, int nFirstParticle, int n_particles, int attribute_write_mask, void* pContext) const
	{
	}

	virtual void InitNewParticlesBlock(CParticleCollection* pParticles, int start_block, int n_blocks, int attribute_write_mask, void* pContext) const
	{
		InitNewParticlesScalar(pParticles, 4 * start_block, 4 * n_blocks, attribute_write_mask, pContext);
	}

	void InitNewParticles(CParticleCollection* pParticles, int nFirstParticle, int n_particles, int attribute_write_mask, void* pContext) const;


	virtual bool MayCreateMoreParticles(CParticleCollection* pParticles, void* pContext) const
	{
		return false;
	}

	const IParticleOperatorDefinition* GetDefinition()
	{
		return m_pDef;
	}

	virtual bool ShouldRunBeforeEmitters(void) const
	{
		return false;
	}

	virtual bool RequiresOrderInvariance(void) const
	{
		return false;
	}

	virtual void SkipToTime(float flTime, CParticleCollection* pParticles, void* pContext) const {}

	const DmObjectId_t& GetId() { return m_Id; }

	virtual void Render(CParticleCollection* pParticles) const {}

	int RandomInt(int nMin, int nMax)
	{
		Assert(0);
		return 0;
	}

	float RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f)
	{
		Assert(0);
		return 0.0f;
	}

	float RandomFloatExp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f)
	{
		Assert(0);
		return 0.0f;
	}

	float m_flOpStartFadeInTime;
	float m_flOpEndFadeInTime;
	float m_flOpStartFadeOutTime;
	float m_flOpEndFadeOutTime;
	float m_flOpFadeOscillatePeriod;

	virtual ~CParticleOperatorInstance(void)
	{
	}

protected:
	void InitScalarAttributeRandomRangeBlock(int nAttributeId, float fMinValue, float fMaxValue,
		CParticleCollection* pParticles, int nStartBlock, int nBlockCount) const;
	void InitScalarAttributeRandomRangeExpBlock(int nAttributeId, float fMinValue, float fMaxValue, float fExp,
		CParticleCollection* pParticles, int nStartBlock, int nBlockCount) const;
	void AddScalarAttributeRandomRangeBlock(int nAttributeId, float fMinValue, float fMaxValue, float fExp,
		CParticleCollection* pParticles, int nStartBlock, int nBlockCount, bool bRandomlyInvert) const;

private:
	friend class CParticleCollection;

	const IParticleOperatorDefinition* m_pDef;
	void SetDefinition(const IParticleOperatorDefinition* pDef, const DmObjectId_t& id)
	{
		m_pDef = pDef;
		CopyUniqueId(id, &m_Id);
	}

	DmObjectId_t m_Id;

	template <typename T> friend class CParticleOperatorDefinition;
};

class CParticleRenderOperatorInstance : public CParticleOperatorInstance
{
public:

	CParticleVisibilityInputs VisibilityInputs;
};

template < class T >
class CParticleOperatorDefinition : public IParticleOperatorDefinition
{
public:
	CParticleOperatorDefinition(const char* pFactoryName, ParticleOperatorId_t id, bool bIsObsolete) : m_pFactoryName(pFactoryName), m_Id(id)
	{
#if MEASURE_PARTICLE_PERF
		m_flTotalExecutionTime = 0.0f;
		m_flMaxExecutionTime = 0.0f;
		m_flUncomittedTime = 0.0f;
#endif
		m_bIsObsolete = bIsObsolete;
	}

	virtual const char* GetName() const
	{
		return m_pFactoryName;
	}

	virtual ParticleOperatorId_t GetId() const
	{
		return m_Id;
	}

	virtual CParticleOperatorInstance* CreateInstance(const DmObjectId_t& id) const
	{
		CParticleOperatorInstance* pOp = new T;
		pOp->SetDefinition(this, id);
		return pOp;
	}

	virtual const DmxElementUnpackStructure_t* GetUnpackStructure() const
	{
		return m_pUnpackParams;
	}

	virtual bool IsObsolete() const
	{
		return m_bIsObsolete;
	}

	virtual size_t GetClassSize() const
	{
		return sizeof(T);
	}

private:
	const char* m_pFactoryName;
	ParticleOperatorId_t m_Id;
	bool m_bIsObsolete;
	static DmxElementUnpackStructure_t* m_pUnpackParams;
};

#define DECLARE_PARTICLE_OPERATOR( _className )				\
	DECLARE_DMXELEMENT_UNPACK()								\
	friend class CParticleOperatorDefinition<_className >

#define DEFINE_PARTICLE_OPERATOR( _className, _operatorName, _id )	\
	static CParticleOperatorDefinition<_className> s_##_className##Factory( _operatorName, _id, false )

#define DEFINE_PARTICLE_OPERATOR_OBSOLETE( _className, _operatorName, _id )	\
	static CParticleOperatorDefinition<_className> s_##_className##Factory( _operatorName, _id, true )

#define BEGIN_PARTICLE_OPERATOR_UNPACK( _className )										\
	BEGIN_DMXELEMENT_UNPACK( _className )													\
	DMXELEMENT_UNPACK_FIELD( "operator start fadein","0", float, m_flOpStartFadeInTime )	\
	DMXELEMENT_UNPACK_FIELD( "operator end fadein","0", float, m_flOpEndFadeInTime )		\
	DMXELEMENT_UNPACK_FIELD( "operator start fadeout","0", float, m_flOpStartFadeOutTime )	\
	DMXELEMENT_UNPACK_FIELD( "operator end fadeout","0", float, m_flOpEndFadeOutTime ) \
    DMXELEMENT_UNPACK_FIELD( "operator fade oscillate","0", float, m_flOpFadeOscillatePeriod )

#define END_PARTICLE_OPERATOR_UNPACK( _className )		\
	END_DMXELEMENT_UNPACK_TEMPLATE( _className, CParticleOperatorDefinition<_className>::m_pUnpackParams )

#define BEGIN_PARTICLE_RENDER_OPERATOR_UNPACK( _className )											\
	BEGIN_PARTICLE_OPERATOR_UNPACK( _className )													\
	DMXELEMENT_UNPACK_FIELD( "Visibility Proxy Input Control Point Number", "-1", int, VisibilityInputs.m_nCPin )	\
	DMXELEMENT_UNPACK_FIELD( "Visibility Proxy Radius", "1.0", float, VisibilityInputs.m_flProxyRadius )				\
	DMXELEMENT_UNPACK_FIELD( "Visibility input minimum","0", float, VisibilityInputs.m_flInputMin )					\
	DMXELEMENT_UNPACK_FIELD( "Visibility input maximum","1", float, VisibilityInputs.m_flInputMax )					\
	DMXELEMENT_UNPACK_FIELD( "Visibility Alpha Scale minimum","0", float, VisibilityInputs.m_flAlphaScaleMin )		\
	DMXELEMENT_UNPACK_FIELD( "Visibility Alpha Scale maximum","1", float, VisibilityInputs.m_flAlphaScaleMax )		\
	DMXELEMENT_UNPACK_FIELD( "Visibility Radius Scale minimum","1", float, VisibilityInputs.m_flRadiusScaleMin )		\
	DMXELEMENT_UNPACK_FIELD( "Visibility Radius Scale maximum","1", float, VisibilityInputs.m_flRadiusScaleMax )	\
	DMXELEMENT_UNPACK_FIELD( "Visibility Camera Depth Bias", "0", float, VisibilityInputs.m_flCameraBias )

#define REGISTER_PARTICLE_OPERATOR( _type, _className )	\
	g_pParticleSystemMgr->AddParticleOperator( _type, &s_##_className##Factory )



enum
{
	PCFLAGS_FIRST_FRAME = 0x1,
	PCFLAGS_PREV_CONTROL_POINTS_INITIALIZED = 0x2,
};



#define DEBUG_PARTICLE_SORT 0

struct ParticleRenderData_t
{
	float m_flSortKey;										    
	int   m_nIndex;									        
	float m_flRadius;					       
#if VALVE_LITTLE_ENDIAN
	uint8 m_nAlpha;							           
	uint8 m_nAlphaPad[3];										     
#else
	uint8 m_nAlphaPad[3];										     
	uint8 m_nAlpha;							           
#endif
};

struct ExtendedParticleRenderData_t : ParticleRenderData_t
{
	float m_flX;
	float m_flY;
	float m_flZ;
	float m_flPad;
};


typedef struct ALIGN16 _FourInts
{
	int32 m_nValue[4];
} ALIGN16_POST FourInts;



struct CPathParameters
{
	int m_nStartControlPointNumber;
	int m_nEndControlPointNumber;
	int m_nBulgeControl;
	float m_flBulge;
	float m_flMidPoint;

	void ClampControlPointIndices(void)
	{
		m_nStartControlPointNumber = MAX(0, MIN(MAX_PARTICLE_CONTROL_POINTS - 1, m_nStartControlPointNumber));
		m_nEndControlPointNumber = MAX(0, MIN(MAX_PARTICLE_CONTROL_POINTS - 1, m_nEndControlPointNumber));
	}
};

struct CParticleVisibilityData
{
	float	m_flAlphaVisibility;
	float	m_flRadiusVisibility;
	float	m_flCameraBias;
	bool	m_bUseVisibility;
};

struct CParticleControlPoint
{
	Vector m_Position;
	Vector m_PrevPosition;

	Vector m_ForwardVector;
	Vector m_UpVector;
	Vector m_RightVector;

	void* m_pObject;

	int m_nParent;
};


struct CParticleSIMDTransformation
{
	FourVectors m_v4Origin;
	FourVectors m_v4Fwd;
	FourVectors m_v4Up;
	FourVectors m_v4Right;


	FORCEINLINE void VectorRotate(FourVectors& InPnt)
	{
		fltx4 fl4OutX = SubSIMD(AddSIMD(MulSIMD(InPnt.x, m_v4Fwd.x), MulSIMD(InPnt.z, m_v4Up.x)), MulSIMD(InPnt.y, m_v4Right.x));
		fltx4 fl4OutY = SubSIMD(AddSIMD(MulSIMD(InPnt.x, m_v4Fwd.y), MulSIMD(InPnt.z, m_v4Up.y)), MulSIMD(InPnt.y, m_v4Right.y));
		InPnt.z = SubSIMD(AddSIMD(MulSIMD(InPnt.x, m_v4Fwd.z), MulSIMD(InPnt.z, m_v4Up.z)), MulSIMD(InPnt.y, m_v4Right.z));
		InPnt.x = fl4OutX;
		InPnt.y = fl4OutY;
	}

	FORCEINLINE void VectorTransform(FourVectors& InPnt)
	{
		VectorRotate(InPnt);
		InPnt.x = AddSIMD(InPnt.x, m_v4Origin.x);
		InPnt.y = AddSIMD(InPnt.y, m_v4Origin.y);
		InPnt.z = AddSIMD(InPnt.z, m_v4Origin.z);
	}
};

#define NUM_COLLISION_CACHE_MODES 4


struct CUtlString_simple {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};

class CParticleSystemDefinition {
	BYTE pad_0[308];
public:
	CUtlString_simple m_Name;
};

struct CParticleAttributeAddressTable {
	float* m_pAttributes[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nFloatStrides[MAX_PARTICLE_ATTRIBUTES];

	FORCEINLINE float* FloatAttributePtr(int nAttribute, int nParticleNumber) const {
		int block_ofs = nParticleNumber / 4;
		return m_pAttributes[nAttribute] +
			m_nFloatStrides[nAttribute] * block_ofs +
			(nParticleNumber & 3);
	}

};

class CParticleCollection {
	BYTE pad_0[48];
public:
	int m_nActiveParticles;
private:
	BYTE pad_1[12];
public:
	CUtlReference<CParticleSystemDefinition> m_pDef;
private:
	BYTE pad_2[60];
public:
	CParticleCollection* m_pParent;
private:
	BYTE pad_3[84];
public:
	CParticleAttributeAddressTable m_ParticleAttributes;
};




























#endif	 
