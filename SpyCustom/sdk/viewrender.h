#if !defined( VIEWRENDER_H )
#define VIEWRENDER_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"
#include "utlstack.h"
#include "tier2.h"
#include "iviewrender.h"
#include "ivrenderview.h" 
#include "view_shared.h"

#if defined(_PS3)
#include "buildrenderables_ps3.h"
#endif
#include "volumeculler.h"
#include "jobthread.h"

#include "bspfile.h"
#include "const.h" 

class ConVar;
class CClientRenderablesList;
class IClientVehicle;
class C_PointCamera;
class C_EnvProjectedTexture;
class IScreenSpaceEffect;
class CClientViewSetup;
class CViewRender;
struct ClientWorldListInfo_t;
class C_BaseEntity;
class CJob;

#ifdef HL2_EPISODIC
class CStunEffect;
#endif  

struct IntroDataBlendPass_t
{
	int m_BlendMode;
	float m_Alpha;                   
};

struct IntroData_t
{
	bool	m_bDrawPrimary;
	Vector	m_vecCameraView;
	QAngle	m_vecCameraViewAngles;
	float	m_playerViewFOV;
	CUtlVector<IntroDataBlendPass_t> m_Passes;

	float	m_flCurrentFadeColor[4];
};

extern IntroData_t* g_pIntroData;

enum view_id_t
{
	VIEW_ILLEGAL = -2,
	VIEW_NONE = -1,
	VIEW_MAIN = 0,
	VIEW_3DSKY = 1,
	VIEW_MONITOR = 2,
	VIEW_REFLECTION = 3,
	VIEW_REFRACTION = 4,
	VIEW_INTRO_PLAYER = 5,
	VIEW_INTRO_CAMERA = 6,
	VIEW_SHADOW_DEPTH_TEXTURE = 7,
	VIEW_SSAO = 8,
	VIEW_ID_COUNT
};
view_id_t CurrentViewID();

#if defined(_PS3)
#define PASS_BUILDLISTS_PS3 (1<<0)
#define PASS_DRAWLISTS_PS3  (1<<1)
#else
#define PASS_BUILDLISTS (1<<0)
#define PASS_DRAWLISTS  (1<<1)
#endif

class CPitchDrift
{
public:
	float		pitchvel;
	bool		nodrift;
	float		driftmove;
	double		laststop;
};



struct ViewCustomVisibility_t
{
	ViewCustomVisibility_t()
	{
		m_nNumVisOrigins = 0;
		m_VisData.m_fDistToAreaPortalTolerance = FLT_MAX;
		m_iForceViewLeaf = -1;
	}

	void AddVisOrigin(const Vector& origin)
	{
		AssertMsg(m_nNumVisOrigins < MAX_VIS_LEAVES, "Added more origins than will fit in the array!");

		if (m_nNumVisOrigins >= MAX_VIS_LEAVES)
			return;

		m_rgVisOrigins[m_nNumVisOrigins++] = origin;
	}

	void ForceVisOverride(VisOverrideData_t& visData)
	{
		m_VisData = visData;
	}

	void ForceViewLeaf(int iViewLeaf)
	{
		m_iForceViewLeaf = iViewLeaf;
	}

	int				m_nNumVisOrigins;
	Vector			m_rgVisOrigins[MAX_VIS_LEAVES];

	VisOverrideData_t m_VisData;

	int				m_iForceViewLeaf;
};

struct WaterRenderInfo_t
{
	bool m_bCheapWater : 1;
	bool m_bReflect : 1;
	bool m_bRefract : 1;
	bool m_bReflectEntities : 1;
	bool m_bReflectOnlyMarkedEntities : 1;
	bool m_bDrawWaterSurface : 1;
	bool m_bOpaqueWater : 1;
	bool m_bPseudoTranslucentWater : 1;
	bool m_bReflect2DSkybox : 1;
};

class CBase3dView : public CRefCounted<>,
	protected CViewSetup
{
	DECLARE_CLASS_NOBASE(CBase3dView);
public:
	explicit CBase3dView(CViewRender* pMainView);

	VPlane* GetFrustum();
	virtual int		GetDrawFlags() { return 0; }

#ifdef PORTAL
	virtual	void	EnableWorldFog() {};
#endif

protected:
	VPlane* m_Frustum;
	CViewRender* m_pMainView;
	int				m_nSlot;
};

struct FrustumCache_t
{
	int			m_nFrameCount;
	Frustum_t	m_Frustums[MAX_SPLITSCREEN_PLAYERS];

	FrustumCache_t() { m_nFrameCount = 0; }

	bool IsValid(void);
	void SetUpdated(void);

	void Add(const CViewSetup* pView, int iSlot);
};

FrustumCache_t* FrustumCache(void);

class CRendering3dView : public CBase3dView
{
	DECLARE_CLASS(CRendering3dView, CBase3dView);
public:
	explicit CRendering3dView(CViewRender* pMainView);
	virtual ~CRendering3dView() { ReleaseLists(); }

	void Setup(const CViewSetup& setup);

	virtual int		GetDrawFlags();
	virtual void	Draw() {};

protected:

	void			EnableWorldFog(void);
	void			SetFogVolumeState(const VisibleFogVolumeInfo_t& fogInfo, bool bUseHeightFog);

	void			SetupRenderablesList(int viewID, bool bFastEntityRendering = false, bool bDrawDepthViewNonCachedObjectsOnly = false);

	void			BuildWorldRenderLists(bool bDrawEntities, int iForceViewLeaf = -1, bool bUseCacheIfEnabled = true, bool bShadowDepth = false, float* pReflectionWaterHeight = NULL);

#if defined(_PS3)
	void			SetupRenderablesList_PS3_Epilogue(void);

	void			BuildWorldRenderLists_PS3_Epilogue(bool bShadowDepth);
	void			BuildRenderableRenderLists_PS3_Epilogue(void);
#else
	void			BuildWorldRenderLists_Epilogue(bool bShadowDepth);
	void			BuildRenderableRenderLists_Epilogue(int viewID);
#endif

	void			BuildRenderableRenderLists(int viewID, bool bFastEntityRendering = false, bool bDrawDepthViewNonCachedObjectsOnly = false);

	void			BuildShadowDepthRenderableRenderLists();

	void			DrawWorld(IMatRenderContext* pRenderContext, float waterZAdjust);

	enum RenderablesRenderPath_t
	{
		RENDERABLES_RENDER_PATH_NORMAL = 0,
		RENDERABLES_RENDER_PATH_SHADOWDEPTH_DEFAULT = (1 << 0),
		RENDERABLES_RENDER_PATH_SHADOWDEPTH_BUILD_GEOCACHE = (1 << 1),
		RENDERABLES_RENDER_PATH_SHADOWDEPTH_USE_GEOCACHE = (1 << 2),
	};
	void			DrawOpaqueRenderables(IMatRenderContext* pRenderContext, RenderablesRenderPath_t eRenderPath, ERenderDepthMode_t DepthMode, CUtlVector< CClientRenderablesList::CEntry* >* pDeferClippedOpaqueRenderables_Out, RenderGroup_t nGroup = RENDER_GROUP_OPAQUE);
	void			DrawDeferredClippedOpaqueRenderables(IMatRenderContext* pRenderContext, RenderablesRenderPath_t eRenderPath, ERenderDepthMode_t DepthMode, CUtlVector< CClientRenderablesList::CEntry* >* pDeferClippedOpaqueRenderables);
	void			DrawTranslucentRenderables(bool bInSkybox, bool bShadowDepth);

#if defined( PORTAL )
	void			DrawRecursivePortalViews(void);
#endif

	void			DrawTranslucentRenderablesNoWorld(bool bInSkybox);

	void			DrawNoZBufferTranslucentRenderables(void);

	void			DrawTranslucentWorldInLeaves(IMatRenderContext* pRenderContext, bool bShadowDepth);

	void			DrawTranslucentWorldAndDetailPropsInLeaves(IMatRenderContext* pRenderContext, int iCurLeaf, int iFinalLeaf, int nEngineDrawFlags, int& nDetailLeafCount, LeafIndex_t* pDetailLeafList, bool bShadowDepth);

	void			PruneWorldListInfo();

	void			BeginConsoleZPass();
	void			EndConsoleZPass();


#ifdef PORTAL
	virtual bool	ShouldDrawPortals() { return true; }
#endif

	void ReleaseLists();

	int m_DrawFlags;
	int m_ClearFlags;


	IWorldRenderList* m_pWorldRenderList;

#if defined( CSTRIKE15 ) && defined(_PS3)
	CClientRenderablesList* m_pRenderablesList[MAX_CONCURRENT_BUILDVIEWS];
	ClientWorldListInfo_t* m_pWorldListInfo[MAX_CONCURRENT_BUILDVIEWS];
#else
	CClientRenderablesList* m_pRenderables;
	ClientWorldListInfo_t* m_pWorldListInfo;
#endif

	ViewCustomVisibility_t* m_pCustomVisibility;
};


class CRenderExecutor
{
	DECLARE_CLASS_NOBASE(CRenderExecutor);
public:

	virtual void AddView(CRendering3dView* pView) = 0;

	virtual void Execute() = 0;

protected:
	explicit CRenderExecutor(CViewRender* pMainView) : m_pMainView(pMainView) {}
	CViewRender* m_pMainView;
};

class CSimpleRenderExecutor : public CRenderExecutor
{
	DECLARE_CLASS(CSimpleRenderExecutor, CRenderExecutor);
public:
	explicit CSimpleRenderExecutor(CViewRender* pMainView) : CRenderExecutor(pMainView) {}

	void AddView(CRendering3dView* pView);

	void Execute() {}
};


#if defined(_PS3)
class CConcurrentViewBuilderPS3
{
public:

	CConcurrentViewBuilderPS3();
	~CConcurrentViewBuilderPS3() { Purge(); };

	void Init(void);
	void Purge(void);

	void ResetBuildViewID(void);

	int	GetBuildViewID(void);

	void PushBuildView(void);
	void PopBuildView(void);

	void SyncViewBuilderJobs(void);

	void SPUBuildRWJobsOn(bool jobOn) { m_bSPUBuildRWJobsOn = jobOn; };
	bool IsSPUBuildRWJobsOn(void) { return m_bSPUBuildRWJobsOn; };

	int GetPassFlags(void) { return m_passFlags; };
	void SetPassFlags(int flags) { m_passFlags = flags; };

	IWorldRenderList* GetWorldRenderListElement(void);
	void SetWorldRenderListElement(IWorldRenderList* pRenderList);

	int GetDrawFlags(void) { return m_drawFlags; };
	void SetDrawFlags(int drawFlags) { m_drawFlags = drawFlags; };

	unsigned int GetVisFlags(void);
	void SetVisFlags(unsigned int visFlags);

	void CacheFrustumData(Frustum_t* pFrustum, Frustum_t* pAreaFrustum, void* pRenderAreaBits, int numArea, bool bViewerInSolidSpace);
	void CacheBuildViewVolumeCuller(void* pVC);
	void* GetBuildViewVolumeCuller(void);
	Frustum_t* GetBuildViewFrustum(void);
	Frustum_t* GetBuildViewAreaFrustum(void);
	unsigned char* GetBuildViewRenderAreaBits(void);

	int GetNumAreaFrustum(void);
	Frustum_t* GetBuildViewAreaFrustumID(int frustumID);


	void PushBuildRenderableJobs(void);

private:

	int						m_buildViewID;
	int						m_nextFreeBuildViewID;

	int* m_pBuildViewStack;
	int						m_buildViewStack[MAX_CONCURRENT_BUILDVIEWS];

	int						m_passFlags;
	int						m_drawFlags;
	unsigned int			m_visFlags[MAX_CONCURRENT_BUILDVIEWS];


	IWorldRenderList* m_pWorldRenderListCache[MAX_CONCURRENT_BUILDVIEWS] ALIGN16;

	Frustum_t				m_gFrustum[MAX_CONCURRENT_BUILDVIEWS] ALIGN16;
	CUtlVector<Frustum_t>	m_gAreaFrustum[MAX_CONCURRENT_BUILDVIEWS] ALIGN16;
	unsigned char			m_gRenderAreaBits[MAX_CONCURRENT_BUILDVIEWS][32] ALIGN16;
	bool					m_bViewerInSolidSpace[MAX_CONCURRENT_BUILDVIEWS] ALIGN16;

	CVolumeCuller			m_volumeCullerCache[MAX_CONCURRENT_BUILDVIEWS] ALIGN16;

	bool					m_bSPUBuildRWJobsOn;
};

extern CConcurrentViewBuilderPS3 g_viewBuilder;

#else

class CConcurrentViewData
{
public:

	void Init(void);
	void Purge(void);

	CVolumeCuller			m_volumeCuller;
	Frustum_t				m_Frustum;
	CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t, 16 > > m_AreaFrustums;
	Frustum_t* m_frustumList[MAX_MAP_AREAS];

	IWorldRenderList* m_pWorldRenderList;
	ClientWorldListInfo_t* m_pWorldListInfo;
	CClientRenderablesList* m_pRenderablesList;

	CJob* m_pBuildWorldListJob;
	CJob* m_pBuildRenderablesListJob;
	bool					m_bWaitForWorldList;
};

class CConcurrentViewBuilder
{
public:

	CConcurrentViewBuilder();
	~CConcurrentViewBuilder();

	void					Init(void);
	void					Purge(void);

	void					SetBuildWRThreaded(bool bThreaded) { m_bThreaded = bThreaded; }
	bool					GetBuildWRThreaded(void) { return m_bThreaded; }

	void					ResetBuildViewID(void);

	int						GetBuildViewID(void);

	void					PushBuildView(void);
	void					PopBuildView(void);

	int						GetPassFlags(void) { return m_passFlags; };
	void					SetPassFlags(int flags) { m_passFlags = flags; };

	void					CacheBuildViewVolumeCuller(const CVolumeCuller* pVC);
	const CVolumeCuller* GetBuildViewVolumeCuller(int buildViewID = -1) const;

	void					CacheFrustumData(const Frustum_t& frustum, const CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t, 16 > >& aeraFrustums);
	void					CacheFrustumData(void);
	const Frustum_t* GetBuildViewFrustum(int buildViewID = -1) const;
	const CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t, 16 > >* GetBuildViewAeraFrustums(int buildViewID = -1) const;
	Frustum_t** GetBuildViewFrustumList(int buildViewID = -1);

	IWorldRenderList* GetWorldRenderListElement(void);
	void					SetWorldRenderListElement(IWorldRenderList* pRenderList);

	ClientWorldListInfo_t* GetClientWorldListInfoElement(int buildViewID = -1);
	WorldListInfo_t* GetWorldListInfoElement(int buildViewID);
	void					SetWorldListInfoElement(ClientWorldListInfo_t* pWorldListInfo, int buildViewID = -1);

	CClientRenderablesList* GetRenderablesListElement(int buildViewID = -1);
	void					SetRenderablesListElement(CClientRenderablesList* pRenderables);

	void					QueueBuildWorldListJob(CJob* pJob);
	void					WaitForBuildWorldListJob(void);
	void					FlushBuildWorldListJob(void);

	void					QueueBuildRenderablesListJob(CJob* pJob);
	void					WaitForBuildRenderablesListJob(void);
	void					FlushBuildRenderablesListJob(void);
	void					AddDependencyToWorldList(void);

private:

	void AddJobToThreadPool(CJob* pJob);
	void AddToSequentialJobs(CJob* pJob);
	void TryRunSequentialJobs(void);
	void WaitForCurrentSequentialJobAndRunPending();
	void InternalWaitForBuildWorldListJob(int buildViewID);

	class SequentialJobs : public CJob
	{
	public:

		explicit SequentialJobs(CJob* pJob = NULL);
		~SequentialJobs();

		void AddJob(CJob* pJob);

		virtual JobStatus_t DoExecute();

	private:

		CUtlVectorFixed<CJob*, 16> m_jobs;
	};

	bool					m_bThreaded;

	int						m_buildViewID;
	int						m_nextFreeBuildViewID;

	int* m_pBuildViewStack;
	int						m_buildViewStack[MAX_CONCURRENT_BUILDVIEWS];

	int						m_passFlags;

	CConcurrentViewData		m_viewData[MAX_CONCURRENT_BUILDVIEWS];

	SequentialJobs* m_pCurrentSeqJobs;
	SequentialJobs* m_pPendingSeqJobs;

	CUtlVector<CJob*>		mJobsAddedToThreadPool;
};

extern CConcurrentViewBuilder g_viewBuilder;

#endif

class CViewRender : public IViewRender
{
	DECLARE_CLASS_NOBASE(CViewRender);
public:
	virtual void	Init(void);
	virtual void	Shutdown(void);

	const CViewSetup* GetPlayerViewSetup(int nSlot = -1) const;

	virtual void	StartPitchDrift(void);
	virtual void	StopPitchDrift(void);

	virtual float	GetZNear();
	virtual float	GetZFar();

	virtual void	OnRenderStart();
	void			DriftPitch(void);

	static CViewRender* GetMainView() { return assert_cast<CViewRender*>(view); }

	void			AddViewToScene(CRendering3dView* pView) { m_SimpleExecutor.AddView(pView); }

	CMaterialReference& GetWhite();

	virtual void	InitFadeData(void);

protected:
	void			SetUpView();

	void			SetUpOverView();
	void			SetUpChaseOverview();

	virtual void	WriteSaveGameScreenshotOfSize(const char* pFilename, int width, int height);
	void			WriteSaveGameScreenshot(const char* filename);

	CViewSetup& GetView(int nSlot = -1);
	const CViewSetup& GetView(int nSlot = -1) const;

	CViewSetup		m_UserView[MAX_SPLITSCREEN_PLAYERS];
	bool			m_bAllowViewAccess;
	CPitchDrift		m_PitchDrift;

	virtual void	RenderPreScene(const CViewSetup& view) { }
	virtual void	PreViewDrawScene(const CViewSetup& view) {}
	virtual void	PostViewDrawScene(const CViewSetup& view) {}

	float			m_flOldChaseOverviewScale;
	float			m_flIdealChaseOverviewScale;
	float			m_flNextIdealOverviewScaleUpdate;
public:
	CViewRender();
	virtual			~CViewRender(void) {}

public:

	void			SetupVis(const CViewSetup& view, unsigned int& visFlags, ViewCustomVisibility_t* pCustomVisibility = NULL);


	virtual	void	Render(vrect_t* rect);
	virtual void	RenderView(const CViewSetup& view, const CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw);
	virtual void	RenderPlayerSprites();
	virtual void	Render2DEffectsPreHUD(const CViewSetup& view);
	virtual void	Render2DEffectsPostHUD(const CViewSetup& view);
	virtual void	RenderSmokeOverlay(bool bPreViewModel = true) {};
	float			GetLastRenderSmokeOverlayAmount() const { return m_flSmokeOverlayAmount; }


	void			DisableFog(void);

	void			LevelInit(void);
	void			LevelShutdown(void);

	bool			ShouldDrawEntities(void);
	bool			ShouldDrawBrushModels(void);

	const CViewSetup* GetViewSetup() const;

	void			DisableVis(void);

	void			MoveViewModels();

	void			GetViewModelPosition(int nIndex, Vector* pPos, QAngle* pAngle);

	void			SetCheapWaterStartDistance(float flCheapWaterStartDistance);
	void			SetCheapWaterEndDistance(float flCheapWaterEndDistance);

	void			GetWaterLODParams(float& flCheapWaterStartDistance, float& flCheapWaterEndDistance);

	virtual void	QueueOverlayRenderView(const CViewSetup& view, int nClearFlags, int whatToDraw);

	virtual void	GetScreenFadeDistances(float* pMin, float* pMax, float* pScale);

	virtual C_BaseEntity* GetCurrentlyDrawingEntity();
	virtual void		  SetCurrentlyDrawingEntity(C_BaseEntity* pEnt);

	virtual bool		UpdateShadowDepthTexture(ITexture* pRenderTarget, ITexture* pDepthTexture, const CViewSetup& shadowView, bool bRenderWorldAndObjects = true, bool bRenderViewModels = false);

	int GetBaseDrawFlags() { return m_BaseDrawFlags; }
	virtual bool ShouldForceNoVis() { return m_bForceNoVis; }
	int				BuildRenderablesListsNumber() const { return m_BuildRenderableListsNumber; }
	int				IncRenderablesListsNumber() { return ++m_BuildRenderableListsNumber; }

	int				BuildWorldListsNumber() const;
	int				IncWorldListsNumber() { return ++m_BuildWorldListsNumber; }

	virtual VPlane* GetFrustum() { return (m_pActiveRenderer) ? m_pActiveRenderer->GetFrustum() : m_Frustum; }

	virtual int		GetDrawFlags() { return (m_pActiveRenderer) ? m_pActiveRenderer->GetDrawFlags() : 0; }

	CBase3dView* GetActiveRenderer() { return m_pActiveRenderer; }
	CBase3dView* SetActiveRenderer(CBase3dView* pActiveRenderer) { CBase3dView* pPrevious = m_pActiveRenderer; m_pActiveRenderer = pActiveRenderer; return pPrevious; }

	void			FreezeFrame(float flFreezeTime);

	void SetWaterOverlayMaterial(IMaterial* pMaterial)
	{
		m_UnderWaterOverlayMaterial.Init(pMaterial);
	}

	void			DrawViewModelsShadowDepth(const CViewSetup& view);

protected:
	int				m_BuildWorldListsNumber;


	void			ViewDrawScene(bool bDrew3dSkybox, SkyboxVisibility_t nSkyboxVisible, const CViewSetup& view, int nClearFlags, view_id_t viewID, bool bDrawViewModel = false, int baseDrawFlags = 0, ViewCustomVisibility_t* pCustomVisibility = NULL);

	void			DrawMonitors(const CViewSetup& cameraView);

	bool			DrawOneMonitor(ITexture* pRenderTarget, int cameraNum, C_PointCamera* pCameraEnt, const CViewSetup& cameraView, void* localPlayer,
		int x, int y, int width, int height);

	bool			ShouldDrawViewModel(bool drawViewmodel);
#ifdef IRONSIGHT
	void			DrawViewModels(const CViewSetup& view, bool drawViewmodel, bool bDrawScopeLensMask = false);
#else
	void			DrawViewModels(const CViewSetup& view, bool drawViewmodel);
#endif

	void			PerformScreenSpaceEffects(int x, int y, int w, int h);

	void			SetScreenOverlayMaterial(IMaterial* pMaterial);
	IMaterial* GetScreenOverlayMaterial();
	void			PerformScreenOverlay(int x, int y, int w, int h);

	void DrawUnderwaterOverlay(void);

	void			DrawWorldAndEntities(bool drawSkybox, const CViewSetup& view, int nClearFlags, ViewCustomVisibility_t* pCustomVisibility = NULL);

	virtual void			ViewDrawScene_Intro(const CViewSetup& view, int nClearFlags, const IntroData_t& introData);

#ifdef PORTAL 
	void			ViewDrawScene_PortalStencil(const CViewSetup& view, ViewCustomVisibility_t* pCustomVisibility);
	void			Draw3dSkyboxworld_Portal(const CViewSetup& view, int& nClearFlags, bool& bDrew3dSkybox, SkyboxVisibility_t& nSkyboxVisible, ITexture* pRenderTarget = NULL);
#endif  

#ifdef PORTAL2
	void			ViewDrawPhoto(ITexture* pRenderTarget, C_BaseEntity* pEnt);      
#endif

	void			DetermineWaterRenderInfo(const VisibleFogVolumeInfo_t& fogVolumeInfo, WaterRenderInfo_t& info);

	bool			UpdateRefractIfNeededByList(CViewModelRenderablesList::RenderGroups_t& list);
	void			DrawRenderablesInList(CViewModelRenderablesList::RenderGroups_t& list, int flags = 0);

	void			SetupMain3DView(int nSlot, const CViewSetup& view, const CViewSetup& hudViewSetup, int& nClearFlags, ITexture* pRenderTarget);
	void			CleanupMain3DView(const CViewSetup& view);

	void			GetLetterBoxRectangles(int nSlot, const CViewSetup& view, CUtlVector< vrect_t >& vecLetterBoxRectangles);
	void			DrawLetterBoxRectangles(int nSlot, const CUtlVector< vrect_t >& vecLetterBoxRectangles);

	void			EnableWaterDepthFeathing(IMaterial* pWaterMaterial, bool bEnable);

#if defined(_PS3)
	void			InitSPUBuildRenderingJobs(void);
#endif

	CViewSetup		m_CurrentView;

	bool			m_bForceNoVis;

	const ConVar* m_pDrawEntities;
	const ConVar* m_pDrawBrushModels;

	CMaterialReference	m_TranslucentSingleColor;
	CMaterialReference	m_ModulateSingleColor;
	CMaterialReference	m_ScreenOverlayMaterial;
	CMaterialReference m_UnderWaterOverlayMaterial;

	Vector				m_vecLastFacing;
	float				m_flCheapWaterStartDistance;
	float				m_flCheapWaterEndDistance;

	CViewSetup			m_OverlayViewSetup;
	int					m_OverlayClearFlags;
	int					m_OverlayDrawFlags;
	bool				m_bDrawOverlay;

	int					m_BaseDrawFlags;	          
	C_BaseEntity* m_pCurrentlyDrawingEntity;

#ifdef PORTAL
	friend class CPortalRender;         
	friend class CPortalRenderable;
#endif
	int				m_BuildRenderableListsNumber;

	friend class CBase3dView;

	Frustum m_Frustum;

	CBase3dView* m_pActiveRenderer;
	CSimpleRenderExecutor m_SimpleExecutor;

	struct FreezeParams_t
	{
		FreezeParams_t() : m_bTakeFreezeFrame(false), m_flFreezeFrameUntil(0.0f) {}

		bool			m_bTakeFreezeFrame;
		float			m_flFreezeFrameUntil;
	};

	FreezeParams_t		m_FreezeParams[MAX_SPLITSCREEN_PLAYERS];

	FadeData_t			m_FadeData;

	CMaterialReference	m_WhiteMaterial;

	CON_COMMAND_MEMBER_F(CViewRender, "screenfademinsize", OnScreenFadeMinSize, "Modify global screen fade min size in pixels", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY);
	CON_COMMAND_MEMBER_F(CViewRender, "screenfademaxsize", OnScreenFadeMaxSize, "Modify global screen fade max size in pixels", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY);

	float				m_flSmokeOverlayAmount;
};

#endif  