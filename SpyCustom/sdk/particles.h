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

class CParticleCollection
{
public:
	~CParticleCollection(void);

	void Restart();

	void RecomputeBounds(void);

	void SetControlPoint(int nWhichPoint, const Vector& v);
	void SetControlPointObject(int nWhichPoint, void* pObject);

	void SetControlPointOrientation(int nWhichPoint, const Vector& forward,
		const Vector& right, const Vector& up);
	void SetControlPointOrientation(int nWhichPoint, const Quaternion& q);
	void SetControlPointForwardVector(int nWhichPoint, const Vector& v);
	void SetControlPointUpVector(int nWhichPoint, const Vector& v);
	void SetControlPointRightVector(int nWhichPoint, const Vector& v);
	void SetControlPointParent(int nWhichPoint, int n);

	const float* GetFloatAttributePtr(int nAttribute, int nParticleNumber) const;
	const int* GetIntAttributePtr(int nAttribute, int nParticleNumber) const;
	const fltx4* GetM128AttributePtr(int nAttribute, size_t* pStrideOut) const;
	const FourVectors* Get4VAttributePtr(int nAttribute, size_t* pStrideOut) const;
	const FourInts* Get4IAttributePtr(int nAttribute, size_t* pStrideOut) const;
	const int* GetIntAttributePtr(int nAttribute, size_t* pStrideOut) const;


	int* GetIntAttributePtrForWrite(int nAttribute, int nParticleNumber);

	float* GetFloatAttributePtrForWrite(int nAttribute, int nParticleNumber);
	fltx4* GetM128AttributePtrForWrite(int nAttribute, size_t* pStrideOut);
	FourVectors* Get4VAttributePtrForWrite(int nAttribute, size_t* pStrideOut);

	const float* GetInitialFloatAttributePtr(int nAttribute, int nParticleNumber) const;
	const fltx4* GetInitialM128AttributePtr(int nAttribute, size_t* pStrideOut) const;
	const FourVectors* GetInitial4VAttributePtr(int nAttribute, size_t* pStrideOut) const;
	float* GetInitialFloatAttributePtrForWrite(int nAttribute, int nParticleNumber);
	fltx4* GetInitialM128AttributePtrForWrite(int nAttribute, size_t* pStrideOut);

	void Simulate(float dt, bool updateBboxOnly);
	void SkipToTime(float t);

	void Render(IMatRenderContext* pRenderContext, bool bTranslucentOnly = false, void* pCameraObject = NULL);

	bool IsValid(void) const;
	const char* GetName() const;

	bool IsFinished(void);

	bool IsValidAttributePtr(int nAttribute, const void* pPtr) const;

	void SwapPosAndPrevPos(void);

	void SetNActiveParticles(int nCount);
	void KillParticle(int nPidx);

	void StopEmission(bool bInfiniteOnly = false, bool bRemoveAllParticles = false, bool bWakeOnStop = false);
	void StartEmission(bool bInfiniteOnly = false);
	void SetDormant(bool bDormant);

	const Vector& GetControlPointAtCurrentTime(int nControlPoint) const;
	void GetControlPointOrientationAtCurrentTime(int nControlPoint, Vector* pForward, Vector* pRight, Vector* pUp) const;
	void GetControlPointTransformAtCurrentTime(int nControlPoint, matrix3x4_t* pMat);
	void GetControlPointTransformAtCurrentTime(int nControlPoint, VMatrix* pMat);
	int GetControlPointParent(int nControlPoint) const;

	void GetControlPointAtTime(int nControlPoint, float flTime, Vector* pControlPoint) const;
	void GetControlPointAtPrevTime(int nControlPoint, Vector* pControlPoint) const;
	void GetControlPointOrientationAtTime(int nControlPoint, float flTime, Vector* pForward, Vector* pRight, Vector* pUp);
	void GetControlPointTransformAtTime(int nControlPoint, float flTime, matrix3x4_t* pMat);
	void GetControlPointTransformAtTime(int nControlPoint, float flTime, VMatrix* pMat);
	void GetControlPointTransformAtTime(int nControlPoint, float flTime, CParticleSIMDTransformation* pXForm);
	int GetHighestControlPoint(void) const;

	bool HasMoved() const;

	bool ReadsControlPoint(int nPoint) const;

	int RandomInt(int nMin, int nMax);
	float RandomFloat(float flMin, float flMax);
	float RandomFloatExp(float flMin, float flMax, float flExponent);
	void RandomVector(float flMin, float flMax, Vector* pVector);
	void RandomVector(const Vector& vecMin, const Vector& vecMax, Vector* pVector);
	float RandomVectorInUnitSphere(Vector* pVector);	       

	int RandomInt(int nRandomSampleId, int nMin, int nMax);
	float RandomFloat(int nRandomSampleId, float flMin, float flMax);
	float RandomFloatExp(int nRandomSampleId, float flMin, float flMax, float flExponent);
	void RandomVector(int nRandomSampleId, float flMin, float flMax, Vector* pVector);
	void RandomVector(int nRandomSampleId, const Vector& vecMin, const Vector& vecMax, Vector* pVector);
	float RandomVectorInUnitSphere(int nRandomSampleId, Vector* pVector);	       

	fltx4 RandomFloat(const FourInts& ParticleID, int nRandomSampleOffset);


	int OperatorRandomSampleOffset() const;

	void GetBounds(Vector* pMin, Vector* pMax);

	void VisualizeOperator(const DmObjectId_t* pOpId = NULL);

	bool UsesPowerOfTwoFrameBufferTexture(bool bThisFrame) const;

	bool UsesFullFrameBufferTexture(bool bThisFrame) const;

	bool IsTranslucent() const;

	bool IsTwoPass() const;

	bool IsBatchable() const;

	int GetRendererCount() const;
	CParticleOperatorInstance* GetRenderer(int i);
	void* GetRendererContext(int i);


	bool CheckIfOperatorShouldRun(CParticleOperatorInstance const* op, float* pflCurStrength = NULL);

	Vector TransformAxis(const Vector& SrcAxis, bool bLocalSpace, int nControlPointNumber = 0);

	const ParticleRenderData_t* GetRenderList(IMatRenderContext* pRenderContext, bool bSorted, int* pNparticles, CParticleVisibilityData* pVisibilityData);

	void CalculatePathValues(CPathParameters const& PathIn,
		float flTimeStamp,
		Vector* pStartPnt,
		Vector* pMidPnt,
		Vector* pEndPnt
	);

	int GetGroupID() const;

	void InitializeNewParticles(int nFirstParticle, int nParticleCount, uint32 nInittedMask);

	void UpdateHitBoxInfo(int nControlPointNumber);

	void UnlinkFromDefList();

	CParticleCollection* GetNextCollectionUsingSameDef() { return m_pNextDef; }

	CUtlReference< CSheet > m_Sheet;



protected:
	CParticleCollection();

	bool Init(const char* pParticleSystemName);
	bool Init(CParticleSystemDefinition* pDef);

	void BloatBoundsUsingControlPoint();

private:
	void GenerateSortedIndexList(Vector vecCameraPos, CParticleVisibilityData* pVisibilityData, bool bSorted);

	void Init(CParticleSystemDefinition* pDef, float flDelay, int nRandomSeed);
	void InitStorage(CParticleSystemDefinition* pDef);
	void InitParticleCreationTime(int nFirstParticle, int nNumToInit);
	void CopyInitialAttributeValues(int nStartParticle, int nNumParticles);
	void ApplyKillList(void);
	void SetAttributeToConstant(int nAttribute, float fValue);
	void SetAttributeToConstant(int nAttribute, float fValueX, float fValueY, float fValueZ);
	void InitParticleAttributes(int nStartParticle, int nNumParticles, int nAttrsLeftToInit);

	void FillAttributeWithConstant(int nAttribute, float fValue);

	void UpdatePrevControlPoints(float dt);

	float* GetConstantAttributeMemory(int nAttribute);

	void SwapAdjacentParticles(int hParticle);

	void UnlinkParticle(int hParticle);

	void InsertParticleBefore(int hParticle, int hBefore);

	void MoveParticle(int nInitialIndex, int nNewIndex);

	float ComputeSqrDistanceToParticle(int hParticle, const Vector& vecPosition) const;

	void GrowDistSqrBounds(float flDistSqr);

	void SimulateFirstFrame();

	bool SystemContainsParticlesWithBoolSet(bool CParticleCollection::* pField) const;
	bool ContainsOpaqueCollections();
	bool ComputeUsesPowerOfTwoFrameBufferTexture();
	bool ComputeUsesFullFrameBufferTexture();
	bool ComputeIsTranslucent();
	bool ComputeIsTwoPass();
	bool ComputeIsBatchable();
	bool ComputeRequiresOrderInvariance();

	void LabelTextureUsage(void);

	void LinkIntoDefList();

public:
	fltx4 m_fl4CurTime;										  

	int m_nPaddedActiveParticles;	      
	float m_flCurTime;				  

	int m_nActiveParticles;			    
	float m_flDt;
	float m_flPreviousDt;
	float m_flNextSleepTime;								        

	CUtlReference< CParticleSystemDefinition > m_pDef;
	int m_nAllocatedParticles;
	int m_nMaxAllowedParticles;
	bool m_bDormant;
	bool m_bEmissionStopped;
	bool m_bRequiresOrderInvariance;

	int m_LocalLightingCP;
	Color m_LocalLighting;

	CParticleControlPoint m_ControlPoints[MAX_PARTICLE_CONTROL_POINTS];

	CModelHitBoxesInfo m_ControlPointHitBoxes[MAX_PARTICLE_CONTROL_POINTS];

	uint8* m_pOperatorContextData;
	CParticleCollection* m_pNext;							    
	CParticleCollection* m_pPrev;							    

	struct CWorldCollideContextData* m_pCollisionCacheData[NUM_COLLISION_CACHE_MODES];        
	CParticleCollection* m_pParent;

	CUtlIntrusiveDList<CParticleCollection>  m_Children;	      

	void* operator new(size_t nSize);
	void* operator new(size_t size, int nBlockUse, const char* pFileName, int nLine);
	void operator delete(void* pData);
	void operator delete(void* p, int nBlockUse, const char* pFileName, int nLine);


protected:
	bool m_bBoundsValid;
	Vector m_MinBounds;
	Vector m_MaxBounds;
	int m_nHighestCP;                 

private:


	unsigned char* m_pParticleMemory;						         
	unsigned char* m_pParticleInitialMemory;				         
	unsigned char* m_pConstantMemory;

	int m_nPerParticleInitializedAttributeMask;
	int m_nPerParticleUpdatedAttributeMask;
	int m_nPerParticleReadInitialAttributeMask;				           
	float* m_pParticleAttributes[MAX_PARTICLE_ATTRIBUTES];
	float* m_pParticleInitialAttributes[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nParticleFloatStrides[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nParticleInitialFloatStrides[MAX_PARTICLE_ATTRIBUTES];

	float* m_pConstantAttributes;

	uint64 m_nControlPointReadMask;							        
	int m_nParticleFlags;									 
	bool m_bIsScrubbable : 1;
	bool m_bIsRunningInitializers : 1;
	bool m_bIsRunningOperators : 1;
	bool m_bIsTranslucent : 1;
	bool m_bIsTwoPass : 1;
	bool m_bAnyUsesPowerOfTwoFrameBufferTexture : 1;		         
	bool m_bAnyUsesFullFrameBufferTexture : 1;
	bool m_bIsBatchable : 1;

	bool m_bUsesPowerOfTwoFrameBufferTexture;			         
	bool m_bUsesFullFrameBufferTexture;

	int m_nDrawnFrames;
	int m_nSimulatedFrames;

	Vector m_Center;										    

	int m_nUniqueParticleId;

	int m_nRandomQueryCount;
	int m_nRandomSeed;
	int m_nOperatorRandomSampleOffset;

	float m_flMinDistSqr;
	float m_flMaxDistSqr;
	float m_flOOMaxDistSqr;
	Vector m_vecLastCameraPos;
	float m_flLastMinDistSqr;
	float m_flLastMaxDistSqr;

	int m_nNumParticlesToKill;
	int* m_pParticleKillList;

	CParticleCollection* m_pNextDef;
	CParticleCollection* m_pPrevDef;

	void LoanKillListTo(CParticleCollection* pBorrower) const;
	bool HasAttachedKillList(void) const;


	CParticleOperatorInstance* m_pRenderOp;
	friend class CParticleSystemMgr;
	friend class CParticleOperatorInstance;
};



class CM128InitialAttributeIterator : public CStridedConstPtr<fltx4>
{
public:
	FORCEINLINE CM128InitialAttributeIterator(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->GetInitialM128AttributePtr(nAttribute, &m_nStride);
	}
};


class CM128AttributeIterator : public CStridedConstPtr<fltx4>
{
public:
	FORCEINLINE CM128AttributeIterator(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->GetM128AttributePtr(nAttribute, &m_nStride);
	}
};

class C4IAttributeIterator : public CStridedConstPtr<FourInts>
{
public:
	FORCEINLINE C4IAttributeIterator(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->Get4IAttributePtr(nAttribute, &m_nStride);
	}
};

class CM128AttributeWriteIterator : public CStridedPtr<fltx4>
{
public:
	FORCEINLINE CM128AttributeWriteIterator(void)
	{
	}
	FORCEINLINE void Init(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->GetM128AttributePtrForWrite(nAttribute, &m_nStride);
	}
	FORCEINLINE CM128AttributeWriteIterator(int nAttribute, CParticleCollection* pParticles)
	{
		Init(nAttribute, pParticles);
	}
};

class C4VAttributeIterator : public CStridedConstPtr<FourVectors>
{
public:
	FORCEINLINE C4VAttributeIterator(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->Get4VAttributePtr(nAttribute, &m_nStride);
	}
};

class C4VInitialAttributeIterator : public CStridedConstPtr<FourVectors>
{
public:
	FORCEINLINE C4VInitialAttributeIterator(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->GetInitial4VAttributePtr(nAttribute, &m_nStride);
	}
};

class C4VAttributeWriteIterator : public CStridedPtr<FourVectors>
{
public:
	FORCEINLINE C4VAttributeWriteIterator(int nAttribute, CParticleCollection* pParticles)
	{
		m_pData = pParticles->Get4VAttributePtrForWrite(nAttribute, &m_nStride);
	}
};


inline bool CParticleCollection::HasAttachedKillList(void) const
{
	return m_pParticleKillList != NULL;
}

inline bool CParticleCollection::ReadsControlPoint(int nPoint) const
{
	return (m_nControlPointReadMask & (1ULL << nPoint)) != 0;
}

inline void CParticleCollection::SetNActiveParticles(int nCount)
{
	Assert(nCount <= m_nMaxAllowedParticles);
	m_nActiveParticles = nCount;
	m_nPaddedActiveParticles = (nCount + 3) / 4;
}

inline void CParticleCollection::SwapPosAndPrevPos(void)
{
	Assert(m_nParticleFloatStrides[PARTICLE_ATTRIBUTE_XYZ] == m_nParticleFloatStrides[PARTICLE_ATTRIBUTE_PREV_XYZ]);
	V_swap(m_pParticleAttributes[PARTICLE_ATTRIBUTE_XYZ], m_pParticleAttributes[PARTICLE_ATTRIBUTE_PREV_XYZ]);
}

inline void CParticleCollection::LoanKillListTo(CParticleCollection* pBorrower) const
{
	Assert(!pBorrower->m_pParticleKillList);
	pBorrower->m_nNumParticlesToKill = 0;
	pBorrower->m_pParticleKillList = m_pParticleKillList;
}

inline void CParticleCollection::SetAttributeToConstant(int nAttribute, float fValue)
{
	float* fconst = m_pConstantAttributes + 4 * 3 * nAttribute;
	fconst[0] = fconst[1] = fconst[2] = fconst[3] = fValue;
}

inline void CParticleCollection::SetAttributeToConstant(int nAttribute, float fValueX, float fValueY, float fValueZ)
{
	float* fconst = m_pConstantAttributes + 4 * 3 * nAttribute;
	fconst[0] = fconst[1] = fconst[2] = fconst[3] = fValueX;
	fconst[4] = fconst[5] = fconst[6] = fconst[7] = fValueY;
	fconst[8] = fconst[9] = fconst[10] = fconst[11] = fValueZ;
}

inline void CParticleCollection::SetControlPoint(int nWhichPoint, const Vector& v)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));
	m_nHighestCP = MAX(m_nHighestCP, nWhichPoint);
	m_ControlPoints[nWhichPoint].m_Position = v;
	for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
	{
		i->SetControlPoint(nWhichPoint, v);
	}
}

inline void CParticleCollection::SetControlPointObject(int nWhichPoint, void* pObject)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));
	m_ControlPoints[nWhichPoint].m_pObject = pObject;
	for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
	{
		i->SetControlPointObject(nWhichPoint, pObject);
	}
}

inline void CParticleCollection::SetControlPointOrientation(int nWhichPoint, const Vector& forward,
	const Vector& right, const Vector& up)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));

	if (fabs(DotProduct(forward, up)) <= 0.1f
		&& fabs(DotProduct(forward, right)) <= 0.1f
		&& fabs(DotProduct(right, up)) <= 0.1f)
	{
		m_ControlPoints[nWhichPoint].m_ForwardVector = forward;
		m_ControlPoints[nWhichPoint].m_UpVector = up;
		m_ControlPoints[nWhichPoint].m_RightVector = right;

		for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
		{
			i->SetControlPointOrientation(nWhichPoint, forward, right, up);
		}
	}
	else
	{
		Warning("Attempt to set particle collection %s to invalid orientation matrix\n", GetName());
	}
}

inline Vector CParticleCollection::TransformAxis(const Vector& SrcAxis, bool bLocalSpace,
	int nControlPointNumber)
{
	if (bLocalSpace)
	{
		return												 
			(SrcAxis.x * m_ControlPoints[nControlPointNumber].m_RightVector) +
			(SrcAxis.y * m_ControlPoints[nControlPointNumber].m_ForwardVector) +
			(SrcAxis.z * m_ControlPoints[nControlPointNumber].m_UpVector);
	}
	else
		return SrcAxis;
}


inline void CParticleCollection::SetControlPointOrientation(int nWhichPoint, const Quaternion& q)
{
	matrix3x4_t mat;
	Vector vecForward, vecUp, vecRight;
	QuaternionMatrix(q, mat);
	MatrixVectors(mat, &vecForward, &vecRight, &vecUp);
	SetControlPointOrientation(nWhichPoint, vecForward, vecRight, vecUp);
}

inline void CParticleCollection::SetControlPointForwardVector(int nWhichPoint, const Vector& v)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));
	m_ControlPoints[nWhichPoint].m_ForwardVector = v;
	for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
	{
		i->SetControlPointForwardVector(nWhichPoint, v);
	}
}

inline void CParticleCollection::SetControlPointUpVector(int nWhichPoint, const Vector& v)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));
	m_ControlPoints[nWhichPoint].m_UpVector = v;
	for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
	{
		i->SetControlPointUpVector(nWhichPoint, v);
	}
}

inline void CParticleCollection::SetControlPointRightVector(int nWhichPoint, const Vector& v)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));
	m_ControlPoints[nWhichPoint].m_RightVector = v;
	for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
	{
		i->SetControlPointRightVector(nWhichPoint, v);
	}
}

inline void CParticleCollection::SetControlPointParent(int nWhichPoint, int n)
{
	Assert((nWhichPoint >= 0) && (nWhichPoint < MAX_PARTICLE_CONTROL_POINTS));
	m_ControlPoints[nWhichPoint].m_nParent = n;
	for (CParticleCollection* i = m_Children.m_pHead; i; i = i->m_pNext)
	{
		i->SetControlPointParent(nWhichPoint, n);
	}
}


inline float* CParticleCollection::GetConstantAttributeMemory(int nAttribute)
{
	return m_pConstantAttributes + 3 * 4 * nAttribute;
}

inline int CParticleCollection::OperatorRandomSampleOffset() const
{
	return m_nOperatorRandomSampleOffset;
}

inline int CParticleCollection::RandomInt(int nRandomSampleId, int nMin, int nMax)
{
	float flRand = s_pRandomFloats[(m_nRandomSeed + nRandomSampleId) & RANDOM_FLOAT_MASK];
	flRand *= (nMax + 1 - nMin);
	int nRand = (int)flRand + nMin;
	return nRand;
}

inline float CParticleCollection::RandomFloat(int nRandomSampleId, float flMin, float flMax)
{
	float flRand = s_pRandomFloats[(m_nRandomSeed + nRandomSampleId) & RANDOM_FLOAT_MASK];
	flRand *= (flMax - flMin);
	flRand += flMin;
	return flRand;
}

inline fltx4 CParticleCollection::RandomFloat(const FourInts& ParticleID, int nRandomSampleOffset)
{
	fltx4 Retval;
	int nOfs = m_nRandomSeed + nRandomSampleOffset;
	SubFloat(Retval, 0) = s_pRandomFloats[(nOfs + ParticleID.m_nValue[0]) & RANDOM_FLOAT_MASK];
	SubFloat(Retval, 1) = s_pRandomFloats[(nOfs + ParticleID.m_nValue[1]) & RANDOM_FLOAT_MASK];
	SubFloat(Retval, 2) = s_pRandomFloats[(nOfs + ParticleID.m_nValue[2]) & RANDOM_FLOAT_MASK];
	SubFloat(Retval, 3) = s_pRandomFloats[(nOfs + ParticleID.m_nValue[3]) & RANDOM_FLOAT_MASK];
	return Retval;
}


inline float CParticleCollection::RandomFloatExp(int nRandomSampleId, float flMin, float flMax, float flExponent)
{
	float flRand = s_pRandomFloats[(m_nRandomSeed + nRandomSampleId) & RANDOM_FLOAT_MASK];
	flRand = powf(flRand, flExponent);
	flRand *= (flMax - flMin);
	flRand += flMin;
	return flRand;
}

inline void CParticleCollection::RandomVector(int nRandomSampleId, float flMin, float flMax, Vector* pVector)
{
	float flDelta = flMax - flMin;
	int nBaseId = m_nRandomSeed + nRandomSampleId;

	pVector->x = s_pRandomFloats[nBaseId & RANDOM_FLOAT_MASK];
	pVector->x *= flDelta;
	pVector->x += flMin;

	pVector->y = s_pRandomFloats[(nBaseId + 1) & RANDOM_FLOAT_MASK];
	pVector->y *= flDelta;
	pVector->y += flMin;

	pVector->z = s_pRandomFloats[(nBaseId + 2) & RANDOM_FLOAT_MASK];
	pVector->z *= flDelta;
	pVector->z += flMin;
}

inline void CParticleCollection::RandomVector(int nRandomSampleId, const Vector& vecMin, const Vector& vecMax, Vector* pVector)
{
	int nBaseId = m_nRandomSeed + nRandomSampleId;
	pVector->x = RandomFloat(nBaseId, vecMin.x, vecMax.x);
	pVector->y = RandomFloat(nBaseId + 1, vecMin.y, vecMax.y);
	pVector->z = RandomFloat(nBaseId + 2, vecMin.z, vecMax.z);
}

inline int CParticleCollection::RandomInt(int nMin, int nMax)
{
	return RandomInt(m_nRandomQueryCount++, nMin, nMax);
}

inline float CParticleCollection::RandomFloat(float flMin, float flMax)
{
	return RandomFloat(m_nRandomQueryCount++, flMin, flMax);
}

inline float CParticleCollection::RandomFloatExp(float flMin, float flMax, float flExponent)
{
	return RandomFloatExp(m_nRandomQueryCount++, flMin, flMax, flExponent);
}

inline void CParticleCollection::RandomVector(float flMin, float flMax, Vector* pVector)
{
	RandomVector(m_nRandomQueryCount++, flMin, flMax, pVector);
}

inline void CParticleCollection::RandomVector(const Vector& vecMin, const Vector& vecMax, Vector* pVector)
{
	RandomVector(m_nRandomQueryCount++, vecMin, vecMax, pVector);
}

inline float CParticleCollection::RandomVectorInUnitSphere(Vector* pVector)
{
	return RandomVectorInUnitSphere(m_nRandomQueryCount++, pVector);
}


inline const float* CParticleCollection::GetFloatAttributePtr(int nAttribute, int nParticleNumber) const
{
	Assert(nParticleNumber < m_nAllocatedParticles);
	int block_ofs = nParticleNumber / 4;
	return m_pParticleAttributes[nAttribute] +
		m_nParticleFloatStrides[nAttribute] * block_ofs +
		(nParticleNumber & 3);
}

inline int* CParticleCollection::GetIntAttributePtrForWrite(int nAttribute, int nParticleNumber)
{
	return reinterpret_cast<int*>(GetFloatAttributePtrForWrite(nAttribute, nParticleNumber));
}

inline const int* CParticleCollection::GetIntAttributePtr(int nAttribute, int nParticleNumber) const
{
	return (int*)GetFloatAttributePtr(nAttribute, nParticleNumber);
}

inline const fltx4* CParticleCollection::GetM128AttributePtr(int nAttribute, size_t* pStrideOut) const
{
	*(pStrideOut) = m_nParticleFloatStrides[nAttribute] / 4;
	return reinterpret_cast<fltx4*>(m_pParticleAttributes[nAttribute]);
}

inline const FourInts* CParticleCollection::Get4IAttributePtr(int nAttribute, size_t* pStrideOut) const
{
	*(pStrideOut) = m_nParticleFloatStrides[nAttribute] / 4;
	return reinterpret_cast<FourInts*>(m_pParticleAttributes[nAttribute]);
}

inline const int32* CParticleCollection::GetIntAttributePtr(int nAttribute, size_t* pStrideOut) const
{
	*(pStrideOut) = m_nParticleFloatStrides[nAttribute];
	return reinterpret_cast<int32*>(m_pParticleAttributes[nAttribute]);
}

inline const FourVectors* CParticleCollection::Get4VAttributePtr(int nAttribute, size_t* pStrideOut) const
{
	*(pStrideOut) = m_nParticleFloatStrides[nAttribute] / 12;
	return reinterpret_cast<const FourVectors*>(m_pParticleAttributes[nAttribute]);
}

inline FourVectors* CParticleCollection::Get4VAttributePtrForWrite(int nAttribute, size_t* pStrideOut)
{
	*(pStrideOut) = m_nParticleFloatStrides[nAttribute] / 12;
	return reinterpret_cast<FourVectors*>(m_pParticleAttributes[nAttribute]);
}

inline const FourVectors* CParticleCollection::GetInitial4VAttributePtr(int nAttribute, size_t* pStrideOut) const
{
	*(pStrideOut) = m_nParticleInitialFloatStrides[nAttribute] / 12;
	return reinterpret_cast<FourVectors*>(m_pParticleInitialAttributes[nAttribute]);
}

inline float* CParticleCollection::GetFloatAttributePtrForWrite(int nAttribute, int nParticleNumber)
{
	Assert(!m_bIsRunningInitializers || (m_nPerParticleInitializedAttributeMask & (1 << nAttribute)));
	Assert(!m_bIsRunningOperators || (m_nPerParticleUpdatedAttributeMask & (1 << nAttribute)));

	Assert(m_nParticleFloatStrides[nAttribute] != 0);

	Assert(nParticleNumber < m_nAllocatedParticles);
	int block_ofs = nParticleNumber / 4;
	return m_pParticleAttributes[nAttribute] +
		m_nParticleFloatStrides[nAttribute] * block_ofs +
		(nParticleNumber & 3);
}

inline fltx4* CParticleCollection::GetM128AttributePtrForWrite(int nAttribute, size_t* pStrideOut)
{
	if (!HushAsserts())
	{
		Assert(!m_bIsRunningInitializers || (m_nPerParticleInitializedAttributeMask & (1 << nAttribute)));
		Assert(!m_bIsRunningOperators || (m_nPerParticleUpdatedAttributeMask & (1 << nAttribute)));
		Assert(m_nParticleFloatStrides[nAttribute] != 0);
	}

	*(pStrideOut) = m_nParticleFloatStrides[nAttribute] / 4;
	return reinterpret_cast<fltx4*>(m_pParticleAttributes[nAttribute]);
}

inline const float* CParticleCollection::GetInitialFloatAttributePtr(int nAttribute, int nParticleNumber) const
{
	Assert(nParticleNumber < m_nAllocatedParticles);
	int block_ofs = nParticleNumber / 4;
	return m_pParticleInitialAttributes[nAttribute] + m_nParticleInitialFloatStrides[nAttribute] * block_ofs + (nParticleNumber & 3);
}

inline const fltx4* CParticleCollection::GetInitialM128AttributePtr(int nAttribute, size_t* pStrideOut) const
{
	*(pStrideOut) = m_nParticleInitialFloatStrides[nAttribute] / 4;
	return reinterpret_cast<fltx4*>(m_pParticleInitialAttributes[nAttribute]);
}

inline float* CParticleCollection::GetInitialFloatAttributePtrForWrite(int nAttribute, int nParticleNumber)
{
	Assert(nParticleNumber < m_nAllocatedParticles);
	Assert(m_nPerParticleReadInitialAttributeMask & (1 << nAttribute));
	int block_ofs = nParticleNumber / 4;
	return m_pParticleInitialAttributes[nAttribute] + m_nParticleInitialFloatStrides[nAttribute] * block_ofs + (nParticleNumber & 3);
}

inline fltx4* CParticleCollection::GetInitialM128AttributePtrForWrite(int nAttribute, size_t* pStrideOut)
{
	Assert(m_nPerParticleReadInitialAttributeMask & (1 << nAttribute));
	*(pStrideOut) = m_nParticleInitialFloatStrides[nAttribute] / 4;
	return reinterpret_cast<fltx4*>(m_pParticleInitialAttributes[nAttribute]);
}

inline bool CParticleCollection::IsValidAttributePtr(int nAttribute, const void* pPtr) const
{
	if (pPtr < m_pParticleAttributes[nAttribute])
		return false;

	size_t nArraySize = m_nParticleFloatStrides[nAttribute] * m_nAllocatedParticles / 4;
	void* pMaxPtr = m_pParticleAttributes[nAttribute] + nArraySize;
	return (pPtr <= pMaxPtr);
}


FORCEINLINE void CParticleCollection::KillParticle(int nPidx)
{
	Assert(nPidx < m_nActiveParticles + 4);

	Assert(m_nNumParticlesToKill < MAX_PARTICLES_IN_A_SYSTEM);
	m_pParticleKillList[m_nNumParticlesToKill++] = nPidx;
}

inline void CParticleCollection::FillAttributeWithConstant(int nAttribute, float fValue)
{
	size_t stride;
	fltx4* pAttr = GetM128AttributePtrForWrite(nAttribute, &stride);
	fltx4 fill = ReplicateX4(fValue);
	for (int i = 0; i < m_nPaddedActiveParticles; i++)
	{
		*(pAttr) = fill;
		pAttr += stride;
	}
}


FORCEINLINE void SetVectorAttribute(float* pAttribute, float x, float y, float z)
{
	pAttribute[0] = x;
	pAttribute[4] = y;
	pAttribute[8] = z;
}

FORCEINLINE void SetVectorAttribute(float* pAttribute, const Vector& v)
{
	pAttribute[0] = v.x;
	pAttribute[4] = v.y;
	pAttribute[8] = v.z;
}

FORCEINLINE void SetVectorFromAttribute(Vector& v, const float* pAttribute)
{
	v.x = pAttribute[0];
	v.y = pAttribute[4];
	v.z = pAttribute[8];
}


FORCEINLINE float CParticleCollection::ComputeSqrDistanceToParticle(int hParticle, const Vector& vecPosition) const
{
	const float* xyz = GetFloatAttributePtr(PARTICLE_ATTRIBUTE_XYZ, hParticle);
	Vector vecParticlePosition(xyz[0], xyz[4], xyz[8]);
	return vecParticlePosition.DistToSqr(vecPosition);
}


FORCEINLINE void CParticleCollection::GrowDistSqrBounds(float flDistSqr)
{
	if (m_flLastMinDistSqr > flDistSqr)
	{
		m_flLastMinDistSqr = flDistSqr;
	}
	else if (m_flLastMaxDistSqr < flDistSqr)
	{
		m_flLastMaxDistSqr = flDistSqr;
	}
}




struct ParticleChildrenInfo_t
{
	DmObjectId_t m_Id;
	CUtlString m_Name;
	bool m_bUseNameBasedLookup;
	float m_flDelay;		          
};


class CParticleSystemDefinition
{
	DECLARE_DMXELEMENT_UNPACK();
	DECLARE_REFERENCED_CLASS(CParticleSystemDefinition);


public:
	CParticleSystemDefinition(void);
	~CParticleSystemDefinition(void);

	void Read(CDmxElement* pElement);
	CDmxElement* Write();

	const char* MaterialName() const;
	IMaterial* GetMaterial() const;
	const char* GetName() const;
	const DmObjectId_t& GetId() const;

	bool UsesPowerOfTwoFrameBufferTexture();

	bool UsesFullFrameBufferTexture();

	bool ShouldAlwaysPrecache() const;

	bool ShouldBatch() const;

	bool IsViewModelEffect() const;

	CParticleCollection* FirstCollection();

	float GetCullRadius() const;
	float GetCullFillCost() const;
	int GetCullControlPoint() const;
	const char* GetCullReplacementDefinition() const;

	bool HasRetirementBeenChecked(int nFrame) const;
	void MarkRetirementCheck(int nFrame);

	void MarkReadsControlPoint(int nPoint);
	bool ReadsControlPoint(int nPoint) const;

private:
	void Precache();
	void Uncache();
	bool IsPrecached() const;

	void UnlinkAllCollections();

	void SetupContextData();
	void ParseChildren(CDmxElement* pElement);
	void ParseOperators(const char* pszName, ParticleFunctionType_t nFunctionType,
		CDmxElement* pElement, CUtlVector<CParticleOperatorInstance*>& out_list);
	void WriteChildren(CDmxElement* pElement);
	void WriteOperators(CDmxElement* pElement, const char* pOpKeyName,
		const CUtlVector<CParticleOperatorInstance*>& inList);
	CUtlVector<CParticleOperatorInstance*>* GetOperatorList(ParticleFunctionType_t type);
	CParticleOperatorInstance* FindOperatorById(ParticleFunctionType_t type, const DmObjectId_t& id);

private:
	int m_nInitialParticles;
	int m_nPerParticleUpdatedAttributeMask;
	int m_nPerParticleInitializedAttributeMask;
	int m_nInitialAttributeReadMask;
	int m_nAttributeReadMask;
	uint64 m_nControlPointReadMask;
	Vector m_BoundingBoxMin;
	Vector m_BoundingBoxMax;
	char m_pszMaterialName[MAX_PATH];
	CMaterialReference m_Material;
	CParticleCollection* m_pFirstCollection;
	char m_pszCullReplacementName[128];
	float m_flCullRadius;
	float m_flCullFillCost;
	int m_nCullControlPoint;
	int m_nRetireCheckFrame;

	Color m_ConstantColor;
	float m_flConstantRadius;
	float m_flConstantRotation;
	float m_flConstantRotationSpeed;
	int m_nConstantSequenceNumber;
	int m_nConstantSequenceNumber1;
	int m_nGroupID;
	float m_flMaximumTimeStep;
	float m_flMaximumSimTime;					         
	float m_flMinimumSimTime;            
	int m_nMinimumFrames;					          


	bool m_bViewModelEffect;


	size_t m_nContextDataSize;
	DmObjectId_t m_Id;

public:
	float m_flMaxDrawDistance;								       
	float m_flNoDrawTimeToGoToSleep;						           

	int m_nMaxParticles;
	int m_nSkipRenderControlPoint;							       
	CUtlString m_Name;

	CUtlVector<CParticleOperatorInstance*> m_Operators;
	CUtlVector<CParticleOperatorInstance*> m_Renderers;
	CUtlVector<CParticleOperatorInstance*> m_Initializers;
	CUtlVector<CParticleOperatorInstance*> m_Emitters;
	CUtlVector<CParticleOperatorInstance*> m_ForceGenerators;
	CUtlVector<CParticleOperatorInstance*> m_Constraints;
	CUtlVector<ParticleChildrenInfo_t> m_Children;

	CUtlVector<size_t> m_nOperatorsCtxOffsets;
	CUtlVector<size_t> m_nRenderersCtxOffsets;
	CUtlVector<size_t> m_nInitializersCtxOffsets;
	CUtlVector<size_t> m_nEmittersCtxOffsets;
	CUtlVector<size_t> m_nForceGeneratorsCtxOffsets;
	CUtlVector<size_t> m_nConstraintsCtxOffsets;

	float m_flTotalSimTime;
	float m_flUncomittedTotalSimTime;
	float m_flMaxMeasuredSimTime;
	int m_nMaximumActiveParticles;
	bool m_bShouldSort;
	bool m_bShouldBatch;
	bool m_bIsPrecached : 1;
	bool m_bAlwaysPrecache : 1;

	friend class CParticleCollection;
	friend class CParticleSystemMgr;
};


inline CParticleSystemDefinition::CParticleSystemDefinition(void)
{
	m_nControlPointReadMask = 0;
	m_nInitialAttributeReadMask = 0;
	m_nPerParticleInitializedAttributeMask = 0;
	m_nPerParticleUpdatedAttributeMask = 0;
	m_nAttributeReadMask = 0;
	m_flTotalSimTime = 0.0;
	m_flMaxMeasuredSimTime = 0.0;
	m_nMaximumActiveParticles = 0;
	m_bIsPrecached = false;
	m_bAlwaysPrecache = false;
	m_bShouldBatch = false;
	m_bShouldSort = true;
	m_pFirstCollection = NULL;
	m_flCullRadius = 0.0f;
	m_flCullFillCost = 1.0f;
	m_nRetireCheckFrame = 0;
}

inline CParticleSystemDefinition::~CParticleSystemDefinition(void)
{
	UnlinkAllCollections();
	m_Operators.PurgeAndDeleteElements();
	m_Renderers.PurgeAndDeleteElements();
	m_Initializers.PurgeAndDeleteElements();
	m_Emitters.PurgeAndDeleteElements();
	m_ForceGenerators.PurgeAndDeleteElements();
	m_Constraints.PurgeAndDeleteElements();
}

inline CParticleCollection* CParticleSystemDefinition::FirstCollection()
{
	return m_pFirstCollection;
}

inline float CParticleSystemDefinition::GetCullRadius() const
{
	return m_flCullRadius;
}

inline float CParticleSystemDefinition::GetCullFillCost() const
{
	return m_flCullFillCost;
}

inline const char* CParticleSystemDefinition::GetCullReplacementDefinition() const
{
	return m_pszCullReplacementName;
}

inline int CParticleSystemDefinition::GetCullControlPoint() const
{
	return m_nCullControlPoint;
}

inline void CParticleSystemDefinition::MarkReadsControlPoint(int nPoint)
{
	m_nControlPointReadMask |= (1ULL << nPoint);
}

inline bool CParticleSystemDefinition::ReadsControlPoint(int nPoint) const
{
	return (m_nControlPointReadMask & (1ULL << nPoint)) != 0;
}

inline bool CParticleSystemDefinition::HasRetirementBeenChecked(int nFrame) const
{
	return m_nRetireCheckFrame == nFrame;
}

inline void CParticleSystemDefinition::MarkRetirementCheck(int nFrame)
{
	m_nRetireCheckFrame = nFrame;
}

inline bool CParticleSystemDefinition::ShouldBatch() const
{
	return m_bShouldBatch;
}

inline bool CParticleSystemDefinition::IsViewModelEffect() const
{
	return m_bViewModelEffect;
}

inline const char* CParticleSystemDefinition::MaterialName() const
{
	return m_pszMaterialName;
}

inline const DmObjectId_t& CParticleSystemDefinition::GetId() const
{
	return m_Id;
}

inline int CParticleCollection::GetGroupID(void) const
{
	return m_pDef->m_nGroupID;
}

FORCEINLINE const Vector& CParticleCollection::GetControlPointAtCurrentTime(int nControlPoint) const
{
	Assert(nControlPoint <= GetHighestControlPoint());
	Assert(m_pDef->ReadsControlPoint(nControlPoint));
	return m_ControlPoints[nControlPoint].m_Position;
}

FORCEINLINE void CParticleCollection::GetControlPointOrientationAtCurrentTime(int nControlPoint, Vector* pForward, Vector* pRight, Vector* pUp) const
{
	Assert(nControlPoint <= GetHighestControlPoint());
	Assert(m_pDef->ReadsControlPoint(nControlPoint));

	*pForward = m_ControlPoints[nControlPoint].m_ForwardVector;
	*pRight = m_ControlPoints[nControlPoint].m_RightVector;
	*pUp = m_ControlPoints[nControlPoint].m_UpVector;
}

FORCEINLINE int CParticleCollection::GetControlPointParent(int nControlPoint) const
{
	Assert(nControlPoint <= GetHighestControlPoint());
	Assert(m_pDef->ReadsControlPoint(nControlPoint));
	return m_ControlPoints[nControlPoint].m_nParent;
}

FORCEINLINE bool CParticleCollection::IsValid(void) const
{
	return (m_pDef != NULL && m_pDef->GetMaterial());
}


#endif	 