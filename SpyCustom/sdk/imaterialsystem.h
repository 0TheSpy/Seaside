#ifndef IMATERIALSYSTEM_H
#define IMATERIALSYSTEM_H

#ifdef _WIN32
#pragma once
#endif

#define OVERBRIGHT 2.0f
#define OO_OVERBRIGHT ( 1.0f / 2.0f )
#define GAMMA 2.2f
#define TEXGAMMA 2.2f

#include "interface.h"
#include "refcount.h"
#include "vector.h"
#include "vector4d.h"
#include "vmatrix.h"
#include "iappsystem.h"
#include "imageformat.h"
#include "texture_group_names.h"
#include "vtf.h"
#include "deformations.h"

enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

#include "imaterialsystemhardwareconfig.h"
#include "IColorCorrection.h"

#if !defined( _GAMECONSOLE )
#endif

#if defined(_PS3)
typedef void* HPS3FONT;    
class CPS3FontMetrics;
class CPS3CharMetrics;

enum GpuDataTransferCache_t
{
	PS3GPU_DATA_TRANSFER_CREATECACHELINK = 0x80000000,
	PS3GPU_DATA_TRANSFER_CACHE2REAL = 0x01000000,
	PS3GPU_DATA_TRANSFER_REAL2CACHE = 0x02000000,
	PS3GPU_DATA_TRANSFER_MASK = 0xFF000000,
};
#endif

class IMaterial;
class IMesh;
class IVertexBuffer;
class IIndexBuffer;
struct MaterialSystem_Config_t;
class VMatrix;
struct matrix3x4_t;
class ITexture;
struct MaterialSystemHardwareIdentifier_t;
class KeyValues;
class IShader;
class IVertexTexture;
class IMorph;
class IMatRenderContext;
class ICallQueue;
struct MorphWeight_t;
class IFileList;
struct VertexStreamSpec_t;
struct ShaderStencilState_t;
struct MeshInstanceData_t;
class IClientMaterialSystem;
class CPaintMaterial;
class IPaintmapDataManager;
class IPaintmapTextureManager;
struct GPUMemoryStats;
class ICustomMaterialManager;
class ICompositeTextureGenerator;

typedef uint64 VertexFormat_t;

enum ShaderParamType_t
{
	SHADER_PARAM_TYPE_TEXTURE,
	SHADER_PARAM_TYPE_INTEGER,
	SHADER_PARAM_TYPE_COLOR,
	SHADER_PARAM_TYPE_VEC2,
	SHADER_PARAM_TYPE_VEC3,
	SHADER_PARAM_TYPE_VEC4,
	SHADER_PARAM_TYPE_ENVMAP,	 
	SHADER_PARAM_TYPE_FLOAT,
	SHADER_PARAM_TYPE_BOOL,
	SHADER_PARAM_TYPE_FOURCC,
	SHADER_PARAM_TYPE_MATRIX,
	SHADER_PARAM_TYPE_MATERIAL,
	SHADER_PARAM_TYPE_STRING,
};

enum MaterialMatrixMode_t
{
	MATERIAL_VIEW = 0,
	MATERIAL_PROJECTION,

	MATERIAL_MATRIX_UNUSED0,
	MATERIAL_MATRIX_UNUSED1,
	MATERIAL_MATRIX_UNUSED2,
	MATERIAL_MATRIX_UNUSED3,
	MATERIAL_MATRIX_UNUSED4,
	MATERIAL_MATRIX_UNUSED5,
	MATERIAL_MATRIX_UNUSED6,
	MATERIAL_MATRIX_UNUSED7,

	MATERIAL_MODEL,

	NUM_MATRIX_MODES = MATERIAL_MODEL + 1,
};

const int NUM_MODEL_TRANSFORMS = 53;
const int MATERIAL_MODEL_MAX = MATERIAL_MODEL + NUM_MODEL_TRANSFORMS;

enum MaterialPrimitiveType_t
{
	MATERIAL_POINTS = 0x0,
	MATERIAL_LINES,
	MATERIAL_TRIANGLES,
	MATERIAL_TRIANGLE_STRIP,
	MATERIAL_LINE_STRIP,
	MATERIAL_LINE_LOOP,	    
	MATERIAL_POLYGON,	     
	MATERIAL_QUADS,
	MATERIAL_SUBD_QUADS_EXTRA,    
	MATERIAL_SUBD_QUADS_REG,      
	MATERIAL_INSTANCED_QUADS,        

	MATERIAL_HETEROGENOUS
};

enum TessellationMode_t
{
	TESSELLATION_MODE_DISABLED = 0,
	TESSELLATION_MODE_ACC_PATCHES_EXTRA,
	TESSELLATION_MODE_ACC_PATCHES_REG
};

enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,					 
	MATERIAL_PROPERTY_OPACITY,								   
	MATERIAL_PROPERTY_REFLECTIVITY,							 
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS				 
};

enum MaterialPropertyOpacityTypes_t
{
	MATERIAL_ALPHATEST = 0,
	MATERIAL_OPAQUE,
	MATERIAL_TRANSLUCENT
};

enum MaterialBufferTypes_t
{
	MATERIAL_FRONT = 0,
	MATERIAL_BACK
};

enum MaterialCullMode_t
{
	MATERIAL_CULLMODE_CCW,	      
	MATERIAL_CULLMODE_CW,	      
	MATERIAL_CULLMODE_NONE	  
};

enum MaterialIndexFormat_t
{
	MATERIAL_INDEX_FORMAT_UNKNOWN = -1,
	MATERIAL_INDEX_FORMAT_16BIT = 0,
	MATERIAL_INDEX_FORMAT_32BIT,
};

enum MaterialFogMode_t
{
	MATERIAL_FOG_NONE,
	MATERIAL_FOG_LINEAR,
	MATERIAL_FOG_LINEAR_BELOW_FOG_Z,
};

enum MaterialHeightClipMode_t
{
	MATERIAL_HEIGHTCLIPMODE_DISABLE,
	MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT,
	MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT
};

enum MaterialNonInteractiveMode_t
{
	MATERIAL_NON_INTERACTIVE_MODE_NONE = -1,
	MATERIAL_NON_INTERACTIVE_MODE_STARTUP = 0,
	MATERIAL_NON_INTERACTIVE_MODE_LEVEL_LOAD,

	MATERIAL_NON_INTERACTIVE_MODE_COUNT,
};


#define MATERIAL_MORPH_DECAL ( (IMorph*)1 )


enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};


#include "lightdesc.h"

enum
{
	MATERIAL_MAX_LIGHT_COUNT = 4,
};

struct MaterialLightingState_t
{
	Vector			m_vecAmbientCube[6];		       
	Vector			m_vecLightingOrigin;		        
	int				m_nLocalLightCount;
	LightDesc_t		m_pLocalLightDesc[MATERIAL_MAX_LIGHT_COUNT];

	MaterialLightingState_t& operator=(const MaterialLightingState_t& src)
	{
		memcpy(this, &src, sizeof(MaterialLightingState_t) - MATERIAL_MAX_LIGHT_COUNT * sizeof(LightDesc_t));
		size_t byteCount = src.m_nLocalLightCount * sizeof(LightDesc_t);
		Assert(byteCount <= sizeof(m_pLocalLightDesc));
		memcpy(m_pLocalLightDesc, &src.m_pLocalLightDesc, byteCount);
		return *this;
	}
};



#define CREATERENDERTARGETFLAGS_HDR				0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP		0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004
#define CREATERENDERTARGETFLAGS_NOEDRAM			0x00000008      
#define CREATERENDERTARGETFLAGS_TEMP			0x00000010           
#define CREATERENDERTARGETFLAGS_ALIASCOLORANDDEPTHSURFACES 0x00000020                      


struct MaterialBatchData_t
{
	IMaterial* m_pMaterial;
	matrix3x4_t* m_pModelToWorld;
	const ITexture* m_pEnvCubemap;
	int						m_nLightmapPageId;
	MaterialPrimitiveType_t m_nPrimType;
	int						m_nIndexOffset;
	int						m_nIndexCount;
	int						m_nVertexOffsetInBytes;
	const IIndexBuffer* m_pIndexBuffer;
	const IVertexBuffer* m_pVertexBuffer;
};


enum MorphFormatFlags_t
{
	MORPH_POSITION = 0x0001,	 
	MORPH_NORMAL = 0x0002,	 
	MORPH_WRINKLE = 0x0004,	 
	MORPH_SPEED = 0x0008,	 
	MORPH_SIDE = 0x0010,	 
};


typedef unsigned int MorphFormat_t;


enum StandardLightmap_t
{
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_INVALID = -4,
};


struct MaterialSystem_SortInfo_t
{
	IMaterial* material;
	int			lightmapPageID;
};


#define MAX_FB_TEXTURES 4

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;			       
	int m_nMinDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};


struct MaterialVideoMode_t
{
	int m_Width;			          
	int m_Height;			       
	ImageFormat m_Format;	      
	int m_RefreshRate;		       
};


struct UberlightState_t
{
	UberlightState_t()
	{
		m_fNearEdge = 2.0f;
		m_fFarEdge = 100.0f;
		m_fCutOn = 10.0f;
		m_fCutOff = 650.0f;
		m_fShearx = 0.0f;
		m_fSheary = 0.0f;
		m_fWidth = 0.3f;
		m_fWedge = 0.05f;
		m_fHeight = 0.3f;
		m_fHedge = 0.05f;
		m_fRoundness = 0.8f;
	}

	float m_fNearEdge;
	float m_fFarEdge;
	float m_fCutOn;
	float m_fCutOff;
	float m_fShearx;
	float m_fSheary;
	float m_fWidth;
	float m_fWedge;
	float m_fHeight;
	float m_fHedge;
	float m_fRoundness;

	IMPLEMENT_OPERATOR_EQUAL(UberlightState_t);
};

struct FlashlightState_t
{
	FlashlightState_t()
	{
		m_bEnableShadows = false;						        
		m_bDrawShadowFrustum = false;
		m_flShadowMapResolution = 1024.0f;
		m_flShadowFilterSize = 3.0f;
		m_flShadowSlopeScaleDepthBias = 16.0f;
		m_flShadowDepthBias = 0.0005f;
		m_flShadowJitterSeed = 0.0f;
		m_flShadowAtten = 0.0f;
		m_flAmbientOcclusion = 0.0f;
		m_nShadowQuality = 0;
		m_bShadowHighRes = false;

		m_bScissor = false;
		m_nLeft = -1;
		m_nTop = -1;
		m_nRight = -1;
		m_nBottom = -1;

		m_bUberlight = false;

		m_bVolumetric = false;
		m_flNoiseStrength = 0.8f;
		m_flFlashlightTime = 0.0f;
		m_nNumPlanes = 64;
		m_flPlaneOffset = 0.0f;
		m_flVolumetricIntensity = 1.0f;

		m_bOrtho = false;
		m_fOrthoLeft = -1.0f;
		m_fOrthoRight = 1.0f;
		m_fOrthoTop = -1.0f;
		m_fOrthoBottom = 1.0f;

		m_fBrightnessScale = 1.0f;
		m_pSpotlightTexture = NULL;
		m_pProjectedMaterial = NULL;
		m_bShareBetweenSplitscreenPlayers = false;
	}

	Vector m_vecLightOrigin;
	Quaternion m_quatOrientation;
	float m_NearZ;
	float m_FarZ;
	float m_fHorizontalFOVDegrees;
	float m_fVerticalFOVDegrees;
	bool  m_bOrtho;
	float m_fOrthoLeft;
	float m_fOrthoRight;
	float m_fOrthoTop;
	float m_fOrthoBottom;
	float m_fQuadraticAtten;
	float m_fLinearAtten;
	float m_fConstantAtten;
	float m_FarZAtten;
	float m_Color[4];
	float m_fBrightnessScale;
	ITexture* m_pSpotlightTexture;
	IMaterial* m_pProjectedMaterial;
	int m_nSpotlightTextureFrame;

	bool  m_bEnableShadows;
	bool  m_bDrawShadowFrustum;
	float m_flShadowMapResolution;
	float m_flShadowFilterSize;
	float m_flShadowSlopeScaleDepthBias;
	float m_flShadowDepthBias;
	float m_flShadowJitterSeed;
	float m_flShadowAtten;
	float m_flAmbientOcclusion;
	int   m_nShadowQuality;
	bool  m_bShadowHighRes;

	float m_flProjectionSize;
	float m_flProjectionRotation;

	bool m_bUberlight;
	UberlightState_t m_uberlightState;

	bool m_bVolumetric;
	float m_flNoiseStrength;
	float m_flFlashlightTime;
	int m_nNumPlanes;
	float m_flPlaneOffset;
	float m_flVolumetricIntensity;
	bool m_bShareBetweenSplitscreenPlayers;	          

	bool DoScissor() const { return m_bScissor; }
	int GetLeft()	 const { return m_nLeft; }
	int GetTop()	 const { return m_nTop; }
	int GetRight()	 const { return m_nRight; }
	int GetBottom()	 const { return m_nBottom; }

private:

	friend class CShadowMgr;

	bool m_bScissor;
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;

	IMPLEMENT_OPERATOR_EQUAL(FlashlightState_t);
};

#define MAX_CASCADED_SHADOW_MAPPING_CASCADES (4)

#define CASCADED_SHADOW_MAPPING_CONSTANT_BUFFER_SIZE (26)

struct CascadedShadowMappingState_t
{
	uint m_nNumCascades;
	bool m_bIsRenderingViewModels;

	Vector4D m_vLightColor;

	Vector m_vLightDir;
	float m_flPadding1;

	struct
	{
		float m_flInvShadowTextureWidth;
		float m_flInvShadowTextureHeight;
		float m_flHalfInvShadowTextureWidth;
		float m_flHalfInvShadowTextureHeight;
	} m_TexParams;

	struct
	{
		float m_flShadowTextureWidth;
		float m_flShadowTextureHeight;
		float m_flSplitLerpFactorBase;
		float m_flSplitLerpFactorInvRange;
	} m_TexParams2;

	struct
	{
		float m_flDistLerpFactorBase;
		float m_flDistLerpFactorInvRange;
		float m_flUnused0;
		float m_flUnused1;
	} m_TexParams3;

	VMatrix m_matWorldToShadowTexMatrices[MAX_CASCADED_SHADOW_MAPPING_CASCADES];
	Vector4D m_vCascadeAtlasUVOffsets[MAX_CASCADED_SHADOW_MAPPING_CASCADES];

	Vector4D m_vCamPosition;
};

enum MaterialInitFlags_t
{
	MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE = 0x2,
	MATERIAL_INIT_REFERENCE_RASTERIZER = 0x4,
};

enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

enum RestoreChangeFlags_t
{
	MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 0x1,
	MATERIAL_RESTORE_RELEASE_MANAGED_RESOURCES = 0x2,
};


enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,			           
	RT_SIZE_DEFAULT = 1,				                 
	RT_SIZE_PICMIP = 2,				         
	RT_SIZE_HDR = 3,					   
	RT_SIZE_FULL_FRAME_BUFFER = 4,	                
	RT_SIZE_OFFSCREEN = 5,			        
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6                    
};


enum AntiAliasingHintEnum_t
{
	AA_HINT_MESHES,
	AA_HINT_TEXT,
	AA_HINT_DEBUG_TEXT,
	AA_HINT_HEAVY_UI_OVERLAY,
	AA_HINT_ALIASING_PUSH,
	AA_HINT_ALIASING_POP,
	AA_HINT_POSTPROCESS,
	AA_HINT_MOVIE,
	AA_HINT_MENU
};

typedef void (*MaterialBufferReleaseFunc_t)(int nChangeFlags);	  
typedef void (*MaterialBufferRestoreFunc_t)(int nChangeFlags);	  
typedef void (*ModeChangeCallbackFunc_t)(void);
typedef void (*EndFrameCleanupFunc_t)(void);
typedef void (*OnLevelShutdownFunc_t)(void* pUserData);
typedef bool (*EndFramePriorToNextContextFunc_t)(void);

typedef unsigned short MaterialHandle_t;

DECLARE_POINTER_HANDLE(OcclusionQueryObjectHandle_t);
#define INVALID_OCCLUSION_QUERY_OBJECT_HANDLE ( (OcclusionQueryObjectHandle_t)0 )

class IMaterialProxyFactory;
class ITexture;
class IMaterialSystemHardwareConfig;
class CShadowMgr;

DECLARE_POINTER_HANDLE(MaterialLock_t);

struct MaterialTextureInfo_t
{
	int iExcludeInformation;
};

struct ApplicationPerformanceCountersInfo_t
{
	float msMain;
	float msMST;
	float msGPU;
	float msFlip;
	float msTotal;
};

struct ApplicationInstantCountersInfo_t
{
	uint m_nCpuActivityMask;
	uint m_nDeferredWordsAllocated;
};


struct WorldListIndicesInfo_t
{
	uint m_nTotalIndices;         
	uint m_nMaxBatchIndices;          
};

struct AspectRatioInfo_t
{
	bool m_bIsWidescreen;
	bool m_bIsHidef;
	float m_flFrameBufferAspectRatio;        
	float m_flPhysicalAspectRatio;           
	float m_flFrameBuffertoPhysicalScalar;    
	float m_flPhysicalToFrameBufferScalar;    
	bool m_bInitialized;

	AspectRatioInfo_t() :
		m_bIsWidescreen(false),
		m_bIsHidef(false),
		m_flFrameBufferAspectRatio(4.0f / 3.0f),
		m_flPhysicalAspectRatio(4.0f / 3.0f),
		m_flFrameBuffertoPhysicalScalar(1.0f),
		m_flPhysicalToFrameBufferScalar(1.0f),
		m_bInitialized(false)
	{
	}
};

abstract_class IMaterialSystem : public IAppSystem
{
public:

	virtual bool Connect(CreateInterfaceFn factory) = 0;								
	virtual void Disconnect() = 0;								
	virtual void* QueryInterface(const char* pInterfaceName) = 0;								
	virtual InitReturnVal_t Init() = 0;								
	virtual void Shutdown() = 0;								

	virtual CreateInterfaceFn	Init(char const* pShaderAPIDLL,								
		IMaterialProxyFactory* pMaterialProxyFactory,
		CreateInterfaceFn fileSystemFactory,
		CreateInterfaceFn cvarFactory = NULL) = 0;

	virtual void				SetShaderAPI(char const* pShaderAPIDLL) = 0;								

	virtual void				SetAdapter(int nAdapter, int nFlags) = 0;								

	virtual void				ModInit() = 0;								
	virtual void				ModShutdown() = 0;								

	virtual void				SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;								
	virtual MaterialThreadMode_t GetThreadMode() = 0;								
	virtual bool				IsRenderThreadSafe() = 0;								
	virtual void				ExecuteQueued() = 0;								
	#ifdef _CERT
		static
	#else
		virtual
	#endif
				void            OnDebugEvent(const char* pEvent = "") {}								

		virtual IMaterialSystemHardwareConfig* GetHardwareConfig(const char* pVersion, int* returnCode) = 0;								


		virtual bool				UpdateConfig(bool bForceUpdate) = 0;								

		virtual bool				OverrideConfig(const MaterialSystem_Config_t& config, bool bForceUpdate) = 0;								

		virtual const MaterialSystem_Config_t& GetCurrentConfigForVideoCard() const = 0;								

		virtual bool				GetRecommendedConfigurationInfo(int nDXLevel, KeyValues* pKeyValues) = 0;								


		virtual int					GetDisplayAdapterCount() const = 0;								

		virtual int					GetCurrentAdapter() const = 0;								

		virtual void				GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;								

		virtual int					GetModeCount(int adapter) const = 0;								

		virtual void				GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;								

		virtual void				AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;								

		virtual void				GetDisplayMode(MaterialVideoMode_t& mode) const = 0;								

		virtual bool				SetMode(void* hwnd, const MaterialSystem_Config_t& config) = 0;								

		virtual bool				SupportsMSAAMode(int nMSAAMode) = 0;								

		virtual const MaterialSystemHardwareIdentifier_t& GetVideoCardIdentifier(void) const = 0;								

		virtual void				SpewDriverInfo() const = 0;								

		virtual void				GetBackBufferDimensions(int& width, int& height) const = 0;								
		virtual ImageFormat			GetBackBufferFormat() const = 0;								
		virtual const AspectRatioInfo_t& GetAspectRatioInfo() const = 0;								

		virtual bool				SupportsHDRMode(HDRType_t nHDRModede) = 0;								


		virtual bool				AddView(void* hwnd) = 0;								
		virtual void				RemoveView(void* hwnd) = 0;								

		virtual void				SetView(void* hwnd) = 0;								


		virtual void				BeginFrame(float frameTime) = 0;								
		virtual void				EndFrame() = 0;								
		virtual void				Flush(bool flushHardware = false) = 0;								
		virtual uint32				GetCurrentFrameCount() = 0;								

		virtual void				SwapBuffers() = 0;								

		virtual void				EvictManagedResources() = 0;								

		virtual void				ReleaseResources(void) = 0;								
		virtual void				ReacquireResources(void) = 0;								


		virtual void				AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;								
		virtual void				RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;								

		virtual void				AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;								
		virtual void				RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;								

		virtual void				AddEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;								
		virtual void				RemoveEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;								

		virtual void				OnLevelShutdown() = 0;								
		virtual bool				AddOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void* pUserData) = 0;								
		virtual bool				RemoveOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void* pUserData) = 0;								

		virtual void				OnLevelLoadingComplete() = 0;								

		virtual void				ResetTempHWMemory(bool bExitingLevel = false) = 0;								

		virtual void				HandleDeviceLost() = 0;								


		virtual int					ShaderCount() const = 0;								
		virtual int					GetShaders(int nFirstShader, int nMaxCount, IShader** ppShaderList) const = 0;								

		virtual int					ShaderFlagCount() const = 0;								
		virtual const char* ShaderFlagName(int nIndex) const = 0;								

		virtual void				GetShaderFallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;								


		virtual IMaterialProxyFactory* GetMaterialProxyFactory() = 0;								

		virtual void				SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;								


		virtual void				EnableEditorMaterials() = 0;								
		virtual void                EnableGBuffers() = 0;								

		virtual void				SetInStubMode(bool bInStubMode) = 0;								


		virtual void				DebugPrintUsedMaterials(const char* pSearchSubString, bool bVerbose) = 0;								
		virtual void				DebugPrintUsedTextures(void) = 0;								

		virtual void				ToggleSuppressMaterial(char const* pMaterialName) = 0;								
		virtual void				ToggleDebugMaterial(char const* pMaterialName) = 0;								


		virtual bool				UsingFastClipping(void) = 0;								

		virtual int					StencilBufferBits(void) = 0;         								


		virtual void				UncacheAllMaterials() = 0;								

		virtual void				UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;								

		virtual void				CacheUsedMaterials() = 0;								

		virtual void				ReloadTextures() = 0;								

		virtual void				ReloadMaterials(const char* pSubString = NULL) = 0;								

		virtual IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues) = 0;								

		virtual IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) = 0;	

		virtual bool				LoadKeyValuesFromVMTFile(KeyValues& vmtKeyValues, const char* pMaterialName, bool bUsesUNCFilename) = 0;						

		virtual MaterialHandle_t	FirstMaterial() const = 0;							

		virtual MaterialHandle_t	NextMaterial(MaterialHandle_t h) const = 0;							

		virtual MaterialHandle_t	InvalidMaterial() const = 0;							

		virtual IMaterial* GetMaterial(MaterialHandle_t h) const = 0;							

		virtual int					GetNumMaterials() const = 0;							

		virtual ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0) = 0;							

		virtual bool				IsTextureLoaded(char const* pTextureName) const = 0;						

		virtual ITexture* CreateProceduralTexture(const char* pTextureName,						
			const char* pTextureGroupName,
			int w,
			int h,
			ImageFormat fmt,
			int nFlags) = 0;

	#if defined( _X360 )

		virtual ITexture* CreateGamerpicTexture(const char* pTextureName,
			const char* pTextureGroupName,
			int nFlags) = 0;

		virtual bool				UpdateLocalGamerpicTexture(ITexture* pTexture, DWORD userIndex) = 0;

		virtual bool				UpdateRemoteGamerpicTexture(ITexture* pTexture, XUID xuid) = 0;

	#endif  

		virtual void				BeginRenderTargetAllocation() = 0;						
		virtual void				EndRenderTargetAllocation() = 0;             						

		virtual ITexture* CreateRenderTargetTexture(int w,						
			int h,
			RenderTargetSizeMode_t sizeMode,	           
			ImageFormat	format,
			MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED) = 0;

		virtual ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName,				         						
			int w,
			int h,
			RenderTargetSizeMode_t sizeMode,	           
			ImageFormat format,
			MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
			unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			unsigned int renderTargetFlags = 0) = 0;

		virtual ITexture* CreateNamedRenderTargetTexture(const char* pRTName,						
			int w,
			int h,
			RenderTargetSizeMode_t sizeMode,	           
			ImageFormat format,
			MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
			bool bClampTexCoords = true,
			bool bAutoMipMap = false) = 0;

		virtual ITexture* CreateNamedRenderTargetTextureEx2(const char* pRTName,				         						
			int w,
			int h,
			RenderTargetSizeMode_t sizeMode,	           
			ImageFormat format,
			MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
			unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			unsigned int renderTargetFlags = 0) = 0;

		virtual void				BeginLightmapAllocation() = 0;						
		virtual void				EndLightmapAllocation() = 0;						

		virtual void				CleanupLightmaps() = 0;						

		virtual int 				AllocateLightmap(int width, int height,						
			int offsetIntoLightmapPage[2],
			IMaterial* pMaterial) = 0;
		virtual int					AllocateWhiteLightmap(IMaterial* pMaterial) = 0;						

		virtual void				UpdateLightmap(int lightmapPageID, int lightmapSize[2],						
			int offsetIntoLightmapPage[2],
			float* pFloatImage, float* pFloatImageBump1,
			float* pFloatImageBump2, float* pFloatImageBump3) = 0;

		virtual int					GetNumSortIDs() = 0;						
		virtual void				GetSortInfo(MaterialSystem_SortInfo_t* sortInfoArray) = 0;						

		virtual void				GetLightmapPageSize(int lightmap, int* width, int* height) const = 0;						

		virtual void				ResetMaterialLightmapPageInfo() = 0;						

		virtual bool				IsStereoSupported() = 0;						
		virtual bool				IsStereoActiveThisFrame() const = 0;						
		virtual void				NVStereoUpdate() = 0;						

		virtual void				ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;						

	#if defined( _X360 )
		virtual void				ListUsedMaterials(void) = 0;
		virtual HXUIFONT			OpenTrueTypeFont(const char* pFontname, int tall, int style) = 0;
		virtual void				CloseTrueTypeFont(HXUIFONT hFont) = 0;
		virtual bool				GetTrueTypeFontMetrics(HXUIFONT hFont, wchar_t wchFirst, wchar_t wchLast, XUIFontMetrics* pFontMetrics, XUICharMetrics* pCharMetrics) = 0;
		virtual bool				GetTrueTypeGlyphs(HXUIFONT hFont, int numChars, wchar_t* pWch, int* pOffsetX, int* pOffsetY, int* pWidth, int* pHeight, unsigned char* pRGBA, int* pRGBAOffset) = 0;
		virtual void				PersistDisplay() = 0;
		virtual void* GetD3DDevice() = 0;
		virtual bool				OwnGPUResources(bool bEnable) = 0;
	#elif defined(_PS3)
		virtual void				ListUsedMaterials(void) = 0;
		virtual HPS3FONT			OpenTrueTypeFont(const char* pFontname, int tall, int style) = 0;
		virtual void				CloseTrueTypeFont(HPS3FONT hFont) = 0;
		virtual bool				GetTrueTypeFontMetrics(HPS3FONT hFont, int nFallbackTall, wchar_t wchFirst, wchar_t wchLast, CPS3FontMetrics* pFontMetrics, CPS3CharMetrics* pCharMetrics) = 0;
		virtual bool				GetTrueTypeGlyphs(HPS3FONT hFont, int nFallbackTall, int numChars, wchar_t* pWch, int* pOffsetX, int* pOffsetY, int* pWidth, int* pHeight, unsigned char* pRGBA, int* pRGBAOffset) = 0;

		virtual bool PS3InitFontLibrary(unsigned fontFileCacheSizeInBytes, unsigned maxNumFonts) { return false; };
		virtual void PS3DumpFontLibrary() { return; }
		virtual void* PS3GetFontLibPtr() { return NULL; }
	#if 0               
	  struct PS3FontLibraryRAII
	  {
		  PS3FontLibraryRAII(IMaterialSystem* imatsys,
			  unsigned int fontFileCacheSizeInBytes = 256 * 1024, unsigned int maxNumFonts = 64) : m_pmatsys(imatsys)
			  {
   imatsys->PS3InitFontLibrary(fontFileCacheSizeInBytes, maxNumFonts);
}
~PS3FontLibraryRAII()
	{
m_pmatsys->PS3DumpFontLibrary();
}

IMaterialSystem* m_pmatsys;
};
#endif
	  enum VRAMScreenShotInfoColorFormat_t
	  {
   kX8R8G8B8 = 0, kX8B8G8R8 = 1, kR16G16B16X16 = 2
};
virtual void TransmitScreenshotToVX() = 0;

virtual void CompactRsxLocalMemory(char const* szReason) = 0;
virtual void SetFlipPresentFrequency(int nNumVBlanks) = 0;
#endif   

	virtual void SpinPresent(uint nFrames) = 0;						

	virtual IMatRenderContext* GetRenderContext() = 0;						

	virtual void				BeginUpdateLightmaps(void) = 0;						
	virtual void				EndUpdateLightmaps(void) = 0;						

	virtual MaterialLock_t		Lock() = 0;						
	virtual void				Unlock(MaterialLock_t) = 0;

	virtual IMatRenderContext* CreateRenderContext(MaterialContextType_t type) = 0;

	virtual IMatRenderContext* SetRenderContext(IMatRenderContext*) = 0;

	virtual bool				SupportsCSAAMode(int nNumSamples, int nQualityLevel) = 0;

	virtual void				RemoveModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;

	virtual IMaterial* FindProceduralMaterial(const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues = NULL) = 0;

	virtual void				AddTextureAlias(const char* pAlias, const char* pRealName) = 0;
	virtual void				RemoveTextureAlias(const char* pAlias) = 0;

	virtual int					AllocateDynamicLightmap(int lightmapSize[2], int* pOutOffsetIntoPage, int frameID) = 0;

	virtual void				SetExcludedTextures(const char* pScriptName, bool bUsingWeaponModelCache) = 0;
	virtual void				UpdateExcludedTextures(void) = 0;
	virtual void				ClearForceExcludes(void) = 0;

	virtual bool				IsInFrame() const = 0;

	virtual void				CompactMemory() = 0;

	virtual void				GetGPUMemoryStats(GPUMemoryStats& stats) = 0;

	virtual void ReloadFilesInList(IFileList* pFilesToReload) = 0;

	virtual bool				GetTextureInformation(char const* szTextureName, MaterialTextureInfo_t& info) const = 0;

	virtual void FinishRenderTargetAllocation(void) = 0;

	virtual void ReEnableRenderTargetAllocation_IRealizeIfICallThisAllTexturesWillBeUnloadedAndLoadTimeWillSufferHorribly(void) = 0;
	virtual	bool				AllowThreading(bool bAllow, int nServiceThread) = 0;

	virtual bool				GetRecommendedVideoConfig(KeyValues* pKeyValues) = 0;

	virtual IClientMaterialSystem* GetClientMaterialSystemInterface() = 0;

	virtual bool				CanDownloadTextures() const = 0;
	virtual int					GetNumLightmapPages() const = 0;

	virtual void RegisterPaintmapDataManager(IPaintmapDataManager* pDataManager) = 0;                    
	virtual void BeginUpdatePaintmaps(void) = 0;
	virtual void EndUpdatePaintmaps(void) = 0;
	virtual void UpdatePaintmap(int paintmap, BYTE* pPaintData, int numRects, Rect_t* pRects) = 0;

	virtual ITexture* CreateNamedMultiRenderTargetTexture(const char* pRTName,				         
		int w,
		int h,
		RenderTargetSizeMode_t sizeMode,	           
		ImageFormat format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
		unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
		unsigned int renderTargetFlags = 0) = 0;

	virtual void				RefreshFrontBufferNonInteractive() = 0;

	virtual uint32 GetFrameTimestamps(ApplicationPerformanceCountersInfo_t& apci, ApplicationInstantCountersInfo_t& aici) = 0;

#if defined( DX_TO_GL_ABSTRACTION ) && !defined( _GAMECONSOLE )
	virtual void				DoStartupShaderPreloading(void) = 0;
#endif	

	virtual void				AddEndFramePriorToNextContextFunc(EndFramePriorToNextContextFunc_t func) = 0;
	virtual void				RemoveEndFramePriorToNextContextFunc(EndFramePriorToNextContextFunc_t func) = 0;

	virtual ICustomMaterialManager* GetCustomMaterialManager() = 0;
	virtual ICompositeTextureGenerator* GetCompositeTextureGenerator() = 0;
};


abstract_class IMatRenderContext : public IRefCounted
{
public:
	virtual void				BeginRender() = 0;
	virtual void				EndRender() = 0;

	virtual void				Flush(bool flushHardware = false) = 0;

	virtual void				BindLocalCubemap(ITexture* pTexture) = 0;

	virtual void				SetRenderTarget(ITexture* pTexture) = 0;
	virtual ITexture* GetRenderTarget(void) = 0;

	virtual void				GetRenderTargetDimensions(int& width, int& height) const = 0;

	virtual void				Bind(IMaterial* material, void* proxyData = 0) = 0;
	virtual void				BindLightmapPage(int lightmapPageID) = 0;

	virtual void				DepthRange(float zNear, float zFar) = 0;

	virtual void				ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;

	virtual void				ReadPixels(int x, int y, int width, int height, unsigned char* data, ImageFormat dstFormat, ITexture* pRenderTargetTexture = NULL) = 0;
	virtual void				ReadPixelsAsync(int x, int y, int width, int height, unsigned char* data, ImageFormat dstFormat, ITexture* pRenderTargetTexture = NULL, CThreadEvent* pPixelsReadEvent = NULL) = 0;
	virtual void				ReadPixelsAsyncGetResult(int x, int y, int width, int height, unsigned char* data, ImageFormat dstFormat, CThreadEvent* pGetResultEvent = NULL) = 0;

	virtual void				SetLightingState(const MaterialLightingState_t& state) = 0;
	virtual void				SetLights(int nCount, const LightDesc_t* pLights) = 0;

	virtual void				SetAmbientLightCube(Vector4D cube[6]) = 0;

	virtual void				CopyRenderTargetToTexture(ITexture* pTexture) = 0;

	virtual void				SetFrameBufferCopyTexture(ITexture* pTexture, int textureIndex = 0) = 0;
	virtual ITexture* GetFrameBufferCopyTexture(int textureIndex) = 0;

	virtual void				MatrixMode(MaterialMatrixMode_t matrixMode) = 0;
	virtual void				PushMatrix(void) = 0;
	virtual void				PopMatrix(void) = 0;
	virtual void				LoadMatrix(VMatrix const& matrix) = 0;
	virtual void				LoadMatrix(matrix3x4_t const& matrix) = 0;
	virtual void				MultMatrix(VMatrix const& matrix) = 0;
	virtual void				MultMatrix(matrix3x4_t const& matrix) = 0;
	virtual void				MultMatrixLocal(VMatrix const& matrix) = 0;
	virtual void				MultMatrixLocal(matrix3x4_t const& matrix) = 0;
	virtual void				GetMatrix(MaterialMatrixMode_t matrixMode, VMatrix* matrix) = 0;
	virtual void				GetMatrix(MaterialMatrixMode_t matrixMode, matrix3x4_t* matrix) = 0;
	virtual void				LoadIdentity(void) = 0;
	virtual void				Ortho(double left, double top, double right, double bottom, double zNear, double zFar) = 0;
	virtual void				PerspectiveX(double fovx, double aspect, double zNear, double zFar) = 0;
	virtual void				PickMatrix(int x, int y, int width, int height) = 0;
	virtual void				Rotate(float angle, float x, float y, float z) = 0;
	virtual void				Translate(float x, float y, float z) = 0;
	virtual void				Scale(float x, float y, float z) = 0;
	virtual void				Viewport(int x, int y, int width, int height) = 0;
	virtual void				GetViewport(int& x, int& y, int& width, int& height) const = 0;

	virtual void				CullMode(MaterialCullMode_t cullMode) = 0;
	virtual void				FlipCullMode(void) = 0;              

	virtual void				BeginGeneratingCSMs() = 0;
	virtual void				EndGeneratingCSMs() = 0;
	virtual void				PerpareForCascadeDraw(int cascade, float fShadowSlopeScaleDepthBias, float fShadowDepthBias) = 0;

	virtual void				SetHeightClipMode(MaterialHeightClipMode_t nHeightClipMode) = 0;
	virtual void				SetHeightClipZ(float z) = 0;

	virtual void				FogMode(MaterialFogMode_t fogMode) = 0;
	virtual void				FogStart(float fStart) = 0;
	virtual void				FogEnd(float fEnd) = 0;
	virtual void				SetFogZ(float fogZ) = 0;
	virtual MaterialFogMode_t	GetFogMode(void) = 0;

	virtual void				FogColor3f(float r, float g, float b) = 0;
	virtual void				FogColor3fv(float const* rgb) = 0;
	virtual void				FogColor3ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void				FogColor3ubv(unsigned char const* rgb) = 0;

	virtual void				GetFogColor(unsigned char* rgb) = 0;

	virtual void				SetNumBoneWeights(int numBones) = 0;

	virtual IMesh* CreateStaticMesh(VertexFormat_t fmt, const char* pTextureBudgetGroup, IMaterial* pMaterial = NULL, VertexStreamSpec_t* pStreamSpec = NULL) = 0;
	virtual void DestroyStaticMesh(IMesh* mesh) = 0;

	virtual IMesh* GetDynamicMesh(
		bool buffered = true,
		IMesh* pVertexOverride = 0,
		IMesh* pIndexOverride = 0,
		IMaterial* pAutoBind = 0) = 0;

	virtual IVertexBuffer* CreateStaticVertexBuffer(VertexFormat_t fmt, int nVertexCount, const char* pTextureBudgetGroup) = 0;
	virtual IIndexBuffer* CreateStaticIndexBuffer(MaterialIndexFormat_t fmt, int nIndexCount, const char* pTextureBudgetGroup) = 0;
	virtual void DestroyVertexBuffer(IVertexBuffer*) = 0;
	virtual void DestroyIndexBuffer(IIndexBuffer*) = 0;
	virtual IVertexBuffer* GetDynamicVertexBuffer(int streamID, VertexFormat_t vertexFormat, bool bBuffered = true) = 0;
	virtual IIndexBuffer* GetDynamicIndexBuffer() = 0;
	virtual void BindVertexBuffer(int streamID, IVertexBuffer* pVertexBuffer, int nOffsetInBytes, int nFirstVertex, int nVertexCount, VertexFormat_t fmt, int nRepetitions = 1) = 0;
	virtual void BindIndexBuffer(IIndexBuffer* pIndexBuffer, int nOffsetInBytes) = 0;
	virtual void Draw(MaterialPrimitiveType_t primitiveType, int firstIndex, int numIndices) = 0;
	virtual int  SelectionMode(bool selectionMode) = 0;
	virtual void SelectionBuffer(unsigned int* pBuffer, int size) = 0;
	virtual void ClearSelectionNames() = 0;
	virtual void LoadSelectionName(int name) = 0;
	virtual void PushSelectionName(int name) = 0;
	virtual void PopSelectionName() = 0;

	virtual void		ClearColor3ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void		ClearColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

	virtual void	OverrideDepthEnable(bool bEnable, bool bDepthWriteEnable, bool bDepthTestEnable = true) = 0;

	virtual void	DrawScreenSpaceQuad(IMaterial* pMaterial) = 0;

	virtual void	SyncToken(const char* pToken) = 0;

	virtual float	ComputePixelWidthOfSphere(const Vector& origin, float flRadius) = 0;

	virtual OcclusionQueryObjectHandle_t CreateOcclusionQueryObject(void) = 0;
	virtual void DestroyOcclusionQueryObject(OcclusionQueryObjectHandle_t) = 0;

	virtual void BeginOcclusionQueryDrawing(OcclusionQueryObjectHandle_t) = 0;
	virtual void EndOcclusionQueryDrawing(OcclusionQueryObjectHandle_t) = 0;

	virtual int OcclusionQuery_GetNumPixelsRendered(OcclusionQueryObjectHandle_t) = 0;

	virtual void SetFlashlightMode(bool bEnable) = 0;

	virtual void SetFlashlightState(const FlashlightState_t& state, const VMatrix& worldToTexture) = 0;

	virtual bool IsCascadedShadowMapping() const = 0;
	virtual void SetCascadedShadowMapping(bool bEnable) = 0;
	virtual void SetCascadedShadowMappingState(const CascadedShadowMappingState_t& state, ITexture* pDepthTextureAtlas) = 0;

	virtual MaterialHeightClipMode_t GetHeightClipMode() = 0;

	virtual float	ComputePixelDiameterOfSphere(const Vector& vecAbsOrigin, float flRadius) = 0;

	virtual void	EnableUserClipTransformOverride(bool bEnable) = 0;
	virtual void	UserClipTransform(const VMatrix& worldToView) = 0;

	virtual bool GetFlashlightMode() const = 0;

	virtual bool IsCullingEnabledForSinglePassFlashlight() const = 0;
	virtual void EnableCullingForSinglePassFlashlight(bool bEnable) = 0;

	virtual void ResetOcclusionQueryObject(OcclusionQueryObjectHandle_t) = 0;

	virtual IMorph* CreateMorph(MorphFormat_t format, const char* pDebugName) = 0;
	virtual void DestroyMorph(IMorph* pMorph) = 0;

	virtual void BindMorph(IMorph* pMorph) = 0;

	virtual void SetFlexWeights(int nFirstWeight, int nCount, const MorphWeight_t* pWeights) = 0;

	virtual void* LockRenderData(int nSizeInBytes) = 0;
	virtual void			UnlockRenderData(void* pData) = 0;

	template< class E > E* LockRenderDataTyped(int nCount, const E* pSrcData = NULL);

	virtual void			AddRefRenderData() = 0;
	virtual void			ReleaseRenderData() = 0;

	virtual bool			IsRenderData(const void* pData) const = 0;

	virtual void ReadPixelsAndStretch(Rect_t* pSrcRect, Rect_t* pDstRect, unsigned char* pBuffer, ImageFormat dstFormat, int nDstStride) = 0;

	virtual void GetWindowSize(int& width, int& height) const = 0;

	virtual void DrawScreenSpaceRectangle(
		IMaterial* pMaterial,
		int destx, int desty,
		int width, int height,
		float src_texture_x0, float src_texture_y0,			       
		float src_texture_x1, float src_texture_y1,			       
		int src_texture_width, int src_texture_height,		   
		void* pClientRenderable = NULL,
		int nXDice = 1,
		int nYDice = 1) = 0;

	virtual void LoadBoneMatrix(int boneIndex, const matrix3x4_t& matrix) = 0;

	virtual void PushRenderTargetAndViewport() = 0;

	virtual void PushRenderTargetAndViewport(ITexture* pTexture) = 0;

	virtual void PushRenderTargetAndViewport(ITexture* pTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;

	virtual void PushRenderTargetAndViewport(ITexture* pTexture, ITexture* pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;

	virtual void PopRenderTargetAndViewport(void) = 0;

	virtual void BindLightmapTexture(ITexture* pLightmapTexture) = 0;

	virtual void CopyRenderTargetToTextureEx(ITexture* pTexture, int nRenderTargetID, Rect_t* pSrcRect, Rect_t* pDstRect = NULL) = 0;
	virtual void CopyTextureToRenderTargetEx(int nRenderTargetID, ITexture* pTexture, Rect_t* pSrcRect, Rect_t* pDstRect = NULL) = 0;

	virtual void PerspectiveOffCenterX(double fovx, double aspect, double zNear, double zFar, double bottom, double top, double left, double right) = 0;

	virtual void SetFloatRenderingParameter(int parm_number, float value) = 0;
	virtual void SetIntRenderingParameter(int parm_number, int value) = 0;
	virtual void SetVectorRenderingParameter(int parm_number, Vector const& value) = 0;

	virtual void SetStencilState(const ShaderStencilState_t& state) = 0;
	virtual void ClearStencilBufferRectangle(int xmin, int ymin, int xmax, int ymax,int value) = 0;

	virtual void SetRenderTargetEx(int nRenderTargetID, ITexture* pTexture) = 0;

	virtual void PushCustomClipPlane(const float* pPlane) = 0;
	virtual void PopCustomClipPlane(void) = 0;

	virtual void GetMaxToRender(IMesh* pMesh, bool bMaxUntilFlush, int* pMaxVerts, int* pMaxIndices) = 0;

	virtual int GetMaxVerticesToRender(IMaterial* pMaterial) = 0;
	virtual int GetMaxIndicesToRender() = 0;
	virtual void DisableAllLocalLights() = 0;
	virtual int CompareMaterialCombos(IMaterial* pMaterial1, IMaterial* pMaterial2, int lightMapID1, int lightMapID2) = 0;

	virtual IMesh* GetFlexMesh() = 0;

	virtual void SetFlashlightStateEx(const FlashlightState_t& state, const VMatrix& worldToTexture, ITexture* pFlashlightDepthTexture) = 0;

	virtual ITexture* GetLocalCubemap() = 0;

	virtual void ClearBuffersObeyStencil(bool bClearColor, bool bClearDepth) = 0;

	virtual bool EnableClipping(bool bEnable) = 0;

	virtual void GetFogDistances(float* fStart, float* fEnd, float* fFogZ) = 0;

	virtual void BeginPIXEvent(unsigned long color, const char* szName) = 0;
	virtual void EndPIXEvent() = 0;
	virtual void SetPIXMarker(unsigned long color, const char* szName) = 0;

	virtual void BeginBatch(IMesh* pIndices) = 0;
	virtual void BindBatch(IMesh* pVertices, IMaterial* pAutoBind = NULL) = 0;
	virtual void DrawBatch(MaterialPrimitiveType_t primType, int firstIndex, int numIndices) = 0;
	virtual void EndBatch() = 0;

	virtual ICallQueue* GetCallQueue() = 0;

	virtual void GetWorldSpaceCameraPosition(Vector* pCameraPos) = 0;
	virtual void GetWorldSpaceCameraVectors(Vector* pVecForward, Vector* pVecRight, Vector* pVecUp) = 0;

	virtual void				SetToneMappingScaleLinear(const Vector& scale) = 0;
	virtual Vector				GetToneMappingScaleLinear(void) = 0;

	virtual void				SetShadowDepthBiasFactors(float fSlopeScaleDepthBias, float fDepthBias) = 0;

	virtual void				PerformFullScreenStencilOperation(void) = 0;

	virtual void				SetLightingOrigin(Vector vLightingOrigin) = 0;

	virtual void				PushScissorRect(const int nLeft, const int nTop, const int nRight, const int nBottom) = 0;
	virtual void				PopScissorRect() = 0;

	virtual void				BeginMorphAccumulation() = 0;
	virtual void				EndMorphAccumulation() = 0;
	virtual void				AccumulateMorph(IMorph* pMorph, int nMorphCount, const MorphWeight_t* pWeights) = 0;

	virtual void				PushDeformation(DeformationBase_t const* Deformation) = 0;
	virtual void				PopDeformation() = 0;
	virtual int					GetNumActiveDeformations() const = 0;

	virtual bool				GetMorphAccumulatorTexCoord(Vector2D* pTexCoord, IMorph* pMorph, int nVertex) = 0;

	virtual IMesh* GetDynamicMeshEx(VertexFormat_t vertexFormat, bool bBuffered = true,
		IMesh* pVertexOverride = 0,	IMesh* pIndexOverride = 0, IMaterial* pAutoBind = 0) = 0;

	virtual void				FogMaxDensity(float flMaxDensity) = 0;

#if defined( _X360 )
	virtual void				PushVertexShaderGPRAllocation(int iVertexShaderCount = 64) = 0;
	virtual void				PopVertexShaderGPRAllocation(void) = 0;

	virtual void				FlushHiStencil() = 0;

#endif

#if defined( _GAMECONSOLE )
	virtual void                BeginConsoleZPass(const WorldListIndicesInfo_t& indicesInfo) = 0;
	virtual void                BeginConsoleZPass2(int nSlack) = 0;
	virtual void				EndConsoleZPass() = 0;
#endif

#if defined( _PS3 )
	virtual void				FlushTextureCache() = 0;
#endif
	virtual void                AntiAliasingHint(int nHint) = 0;

	virtual IMaterial* GetCurrentMaterial() = 0;
	virtual int  GetCurrentNumBones() const = 0;
	virtual void* GetCurrentProxy() = 0;

	virtual void EnableColorCorrection(bool bEnable) = 0;
	virtual ColorCorrectionHandle_t AddLookup(const char* pName) = 0;
	virtual bool RemoveLookup(ColorCorrectionHandle_t handle) = 0;
	virtual void LockLookup(ColorCorrectionHandle_t handle) = 0;
	virtual void LoadLookup(ColorCorrectionHandle_t handle, const char* pLookupName) = 0;
	virtual void UnlockLookup(ColorCorrectionHandle_t handle) = 0;
	virtual void SetLookupWeight(ColorCorrectionHandle_t handle, float flWeight) = 0;
	virtual void ResetLookupWeights() = 0;
	virtual void SetResetable(ColorCorrectionHandle_t handle, bool bResetable) = 0;

	virtual void SetFullScreenDepthTextureValidityFlag(bool bIsValid) = 0;

	virtual void SetNonInteractiveLogoTexture(ITexture* pTexture, float flNormalizedX, float flNormalizedY, float flNormalizedW, float flNormalizedH) = 0;
	virtual void SetNonInteractivePacifierTexture(ITexture* pTexture, float flNormalizedX, float flNormalizedY, float flNormalizedSize) = 0;
	virtual void SetNonInteractiveTempFullscreenBuffer(ITexture* pTexture, MaterialNonInteractiveMode_t mode) = 0;
	virtual void EnableNonInteractiveMode(MaterialNonInteractiveMode_t mode) = 0;
	virtual void RefreshFrontBufferNonInteractive() = 0;

	virtual void FlipCulling(bool bFlipCulling) = 0;

	virtual void SetTextureRenderingParameter(int parm_number, ITexture* pTexture) = 0;

	virtual void EnableSinglePassFlashlightMode(bool bEnable) = 0;

	virtual bool SinglePassFlashlightModeEnabled() const = 0;

	virtual void DrawInstances(int nInstanceCount, const MeshInstanceData_t* pInstance) = 0;

	virtual void OverrideAlphaWriteEnable(bool bOverrideEnable, bool bAlphaWriteEnable) = 0;
	virtual void OverrideColorWriteEnable(bool bOverrideEnable, bool bColorWriteEnable) = 0;

	virtual void ClearBuffersObeyStencilEx(bool bClearColor, bool bClearAlpha, bool bClearDepth) = 0;

	virtual int GetSubDBufferWidth() = 0;
	virtual float* LockSubDBuffer(int nNumRows) = 0;
	virtual void UnlockSubDBuffer() = 0;

	virtual void UpdateGameTime(float flTime) = 0;

	virtual void			PrintfVA(char* fmt, va_list vargs) = 0;
	virtual void			Printf(char* fmt, ...) = 0;
	virtual float			Knob(char* knobname, float* setvalue = NULL) = 0;

#if defined( INCLUDE_SCALEFORM )
	virtual void SetScaleformSlotViewport(int slot, int x, int y, int w, int h) = 0;
	virtual void RenderScaleformSlot(int slot) = 0;
	virtual void ForkRenderScaleformSlot(int slot) = 0;
	virtual void JoinRenderScaleformSlot(int slot) = 0;

	virtual void SetScaleformCursorViewport(int x, int y, int w, int h) = 0;
	virtual void RenderScaleformCursor() = 0;

	virtual void AdvanceAndRenderScaleformSlot(int slot) = 0;
	virtual void AdvanceAndRenderScaleformCursor() = 0;
#endif

	virtual void SetRenderingPaint(bool bEnable) = 0;

	virtual ColorCorrectionHandle_t FindLookup(const char* pName) = 0;
};


template< class E > inline E* IMatRenderContext::LockRenderDataTyped(int nCount, const E* pSrcData)
{
	int nSizeInBytes = nCount * sizeof(E);
	E* pDstData = (E*)LockRenderData(nSizeInBytes);
	if (pSrcData && pDstData)
	{
		memcpy(pDstData, pSrcData, nSizeInBytes);
	}
	return pDstData;
}


abstract_class IClientMaterialSystem
{
public:
	virtual unsigned int GetCurrentRecordingEntity() = 0;
	virtual void PostToolMessage(unsigned int hEntity, KeyValues* pMsg) = 0;
	virtual void SetMaterialProxyData(void* pProxyData) = 0;
};

#define VCLIENTMATERIALSYSTEM_INTERFACE_VERSION "VCLIENTMATERIALSYSTEM001"


class CMatRenderDataReference
{
public:
	CMatRenderDataReference();
	CMatRenderDataReference(IMatRenderContext* pRenderContext);
	~CMatRenderDataReference();
	void Lock(IMatRenderContext* pRenderContext);
	void Release();

private:
	IMatRenderContext* m_pRenderContext;
};


inline CMatRenderDataReference::CMatRenderDataReference()
{
	m_pRenderContext = NULL;
}

inline CMatRenderDataReference::CMatRenderDataReference(IMatRenderContext* pRenderContext)
{
	m_pRenderContext = NULL;
	Lock(pRenderContext);
}

inline CMatRenderDataReference::~CMatRenderDataReference()
{
	Release();
}

inline void CMatRenderDataReference::Lock(IMatRenderContext* pRenderContext)
{
	if (!m_pRenderContext)
	{
		m_pRenderContext = pRenderContext;
		m_pRenderContext->AddRefRenderData();
	}
}

inline void CMatRenderDataReference::Release()
{
	if (m_pRenderContext)
	{
		m_pRenderContext->ReleaseRenderData();
		m_pRenderContext = NULL;
	}
}


template< typename E >
class CMatRenderData
{
public:
	CMatRenderData(IMatRenderContext* pRenderContext);
	CMatRenderData(IMatRenderContext* pRenderContext, int nCount, const E* pSrcData = NULL);
	~CMatRenderData();
	E* Lock(int nCount, const E* pSrcData = NULL);
	void Release();
	bool IsValid() const;
	const E* Base() const;
	E* Base();
	const E& operator[](int i) const;
	E& operator[](int i);

private:
	IMatRenderContext* m_pRenderContext;
	E* m_pRenderData;
	int m_nCount;
	bool m_bNeedsUnlock;
};

template< typename E >
inline CMatRenderData<E>::CMatRenderData(IMatRenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;
	m_nCount = 0;
	m_pRenderData = 0;
	m_bNeedsUnlock = false;
}

template< typename E >
inline CMatRenderData<E>::CMatRenderData(IMatRenderContext* pRenderContext, int nCount, const E* pSrcData)
{
	m_pRenderContext = pRenderContext;
	m_nCount = 0;
	m_pRenderData = 0;
	m_bNeedsUnlock = false;
	Lock(nCount, pSrcData);
}

template< typename E >
inline CMatRenderData<E>::~CMatRenderData()
{
	Release();
}

template< typename E >
inline bool CMatRenderData<E>::IsValid() const
{
	return m_pRenderData != NULL;
}

template< typename E >
inline E* CMatRenderData<E>::Lock(int nCount, const E* pSrcData)
{
	m_nCount = nCount;
	if (pSrcData && m_pRenderContext->IsRenderData(pSrcData))
	{
		m_pRenderData = const_cast<E*>(pSrcData);
		m_pRenderContext->AddRefRenderData();
		m_bNeedsUnlock = false;
		return m_pRenderData;
	}
	m_pRenderData = m_pRenderContext->LockRenderDataTyped<E>(nCount, pSrcData);
	m_bNeedsUnlock = true;
	return m_pRenderData;
}

template< typename E >
inline void CMatRenderData<E>::Release()
{
	if (m_pRenderContext && m_pRenderData)
	{
		if (m_bNeedsUnlock)
		{
			m_pRenderContext->UnlockRenderData(m_pRenderData);
		}
		else
		{
			m_pRenderContext->ReleaseRenderData();
		}
	}
	m_pRenderData = NULL;
	m_nCount = 0;
	m_bNeedsUnlock = false;
}

template< typename E >
inline E* CMatRenderData<E>::Base()
{
	return m_pRenderData;
}

template< typename E >
inline const E* CMatRenderData<E>::Base() const
{
	return m_pRenderData;
}

template< typename E >
inline E& CMatRenderData<E>::operator[](int i)
{
	Assert((i >= 0) && (i < m_nCount));
	return m_pRenderData[i];
}

template< typename E >
inline const E& CMatRenderData<E>::operator[](int i) const
{
	Assert((i >= 0) && (i < m_nCount));
	return m_pRenderData[i];
}


class CMatRenderContextPtr : public CRefPtr<IMatRenderContext>
{
	typedef CRefPtr<IMatRenderContext> BaseClass;
public:
	CMatRenderContextPtr() {}
	CMatRenderContextPtr(IMatRenderContext* pInit) : BaseClass(pInit) { if (BaseClass::m_pObject) BaseClass::m_pObject->BeginRender(); }
	CMatRenderContextPtr(IMaterialSystem* pFrom) : BaseClass(pFrom->GetRenderContext()) { if (BaseClass::m_pObject) BaseClass::m_pObject->BeginRender(); }
	~CMatRenderContextPtr() { if (BaseClass::m_pObject) BaseClass::m_pObject->EndRender(); }

	IMatRenderContext* operator=(IMatRenderContext* p) { if (p) p->BeginRender(); return BaseClass::operator=(p); }

	void SafeRelease() { if (BaseClass::m_pObject) BaseClass::m_pObject->EndRender(); BaseClass::SafeRelease(); }
	void AssignAddRef(IMatRenderContext* pFrom) { if (BaseClass::m_pObject) BaseClass::m_pObject->EndRender(); BaseClass::AssignAddRef(pFrom); BaseClass::m_pObject->BeginRender(); }

	void GetFrom(IMaterialSystem* pFrom) { AssignAddRef(pFrom->GetRenderContext()); }


private:
	CMatRenderContextPtr(const CMatRenderContextPtr& from);
	void operator=(const CMatRenderContextPtr& from);

};

#define PIX_VALVE_ORANGE	0xFFF5940F

class PIXEvent
{
public:
	PIXEvent(IMatRenderContext* pRenderContext, const char* szName, unsigned long color = PIX_VALVE_ORANGE)
	{
		m_pRenderContext = pRenderContext;
		Assert(m_pRenderContext);
		Assert(szName);
		m_pRenderContext->BeginPIXEvent(color, szName);
	}
	~PIXEvent()
	{
		m_pRenderContext->EndPIXEvent();
	}
private:
	IMatRenderContext* m_pRenderContext;
};


#if ( defined( _X360 ) && ( defined( PROFILE ) || defined( _DEBUG ) ) )
#define PIX_ENABLE 1		              
#else
#define PIX_ENABLE 0
#endif


#if PIX_ENABLE
#	define PIXEVENT PIXEvent _pixEvent
#else
#	define PIXEVENT 
#endif

#ifdef MATERIAL_SYSTEM_DEBUG_CALL_QUEUE
#include "tier1/callqueue.h"
#include "tier1/fmtstr.h"
static void DoMatSysQueueMark(IMaterialSystem* pMaterialSystem, const char* psz)
{
	CMatRenderContextPtr pRenderContext(pMaterialSystem);
	if (pRenderContext->GetCallQueue())
		pRenderContext->GetCallQueue()->QueueCall(Plat_DebugString, CUtlEnvelope<const char*>(psz));
}

#define MatSysQueueMark( pMaterialSystem, ...) DoMatSysQueueMark( pMaterialSystem, CFmtStr( __VA_ARGS__ ) )
#else
#define MatSysQueueMark( msg, ...) ((void)0)
#endif

#ifdef _GAMECONSOLE
#define MS_NO_DYNAMIC_BUFFER_COPY 1
#endif

#endif  
