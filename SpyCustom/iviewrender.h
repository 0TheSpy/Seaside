#if !defined( IVIEWRENDER_H )
#define IVIEWRENDER_H
#ifdef _WIN32
#pragma once
#endif


#include "ivrenderview.h"


#define MAX_DEPTH_TEXTURE_SHADOWS 1
#define MAX_DEPTH_TEXTURE_HIGHRES_SHADOWS 0

#define MAX_DEPTH_TEXTURE_SHADOWS_TOOLS 8
#define MAX_DEPTH_TEXTURE_HIGHRES_SHADOWS_TOOLS 0


enum DrawFlags_t
{
	DF_RENDER_REFRACTION = 0x1,
	DF_RENDER_REFLECTION = 0x2,

	DF_CLIP_Z = 0x4,
	DF_CLIP_BELOW = 0x8,

	DF_RENDER_UNDERWATER = 0x10,
	DF_RENDER_ABOVEWATER = 0x20,
	DF_RENDER_WATER = 0x40,

	DF_SSAO_DEPTH_PASS = 0x80,

	DF_RENDER_PSEUDO_TRANSLUCENT_WATER = 0x100,                     
	DF_WATERHEIGHT = 0x200,
	DF_DRAW_SSAO = 0x400,
	DF_DRAWSKYBOX = 0x800,

	DF_FUDGE_UP = 0x1000,

	DF_DRAW_ENTITITES = 0x2000,

	DF_SKIP_WORLD = 0x4000,
	DF_SKIP_WORLD_DECALS_AND_OVERLAYS = 0x8000,

	DF_UNUSED5 = 0x10000,
	DF_SAVEGAMESCREENSHOT = 0x20000,
	DF_CLIP_SKYBOX = 0x40000,

	DF_DRAW_SIMPLE_WORLD_MODEL = 0x80000,	           

	DF_SHADOW_DEPTH_MAP = 0x100000,	      

	DF_FAST_ENTITY_RENDERING = 0x200000,                 
	DF_DRAW_SIMPLE_WORLD_MODEL_WATER = 0x400000,	           
};

class CViewSetup;
class C_BaseEntity;
struct vrect_t;
class C_BaseViewModel;

abstract_class IViewRender
{
public:
	virtual void		Init(void) = 0;

	virtual void		LevelInit(void) = 0;
	virtual void		LevelShutdown(void) = 0;

	virtual void		Shutdown(void) = 0;

	virtual void		OnRenderStart() = 0;

	virtual	void		Render(vrect_t* rect) = 0;

	virtual void		RenderView(const CViewSetup& view, const CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw) = 0;

	virtual int GetDrawFlags() = 0;

	virtual void		StartPitchDrift(void) = 0;
	virtual void		StopPitchDrift(void) = 0;

	virtual VPlane* GetFrustum() = 0;

	virtual bool		ShouldDrawBrushModels(void) = 0;

	virtual const CViewSetup* GetPlayerViewSetup(int nSlot = -1) const = 0;
	virtual const CViewSetup* GetViewSetup(void) const = 0;

	virtual void		DisableVis(void) = 0;

	virtual int			BuildWorldListsNumber() const = 0;

	virtual void		SetCheapWaterStartDistance(float flCheapWaterStartDistance) = 0;
	virtual void		SetCheapWaterEndDistance(float flCheapWaterEndDistance) = 0;

	virtual void		GetWaterLODParams(float& flCheapWaterStartDistance, float& flCheapWaterEndDistance) = 0;

	virtual void		DriftPitch(void) = 0;

	virtual void		SetScreenOverlayMaterial(IMaterial* pMaterial) = 0;
	virtual IMaterial* GetScreenOverlayMaterial() = 0;

	virtual void		WriteSaveGameScreenshot(const char* pFilename) = 0;
	virtual void		WriteSaveGameScreenshotOfSize(const char* pFilename, int width, int height) = 0;

	virtual void		QueueOverlayRenderView(const CViewSetup& view, int nClearFlags, int whatToDraw) = 0;

	virtual float		GetZNear() = 0;
	virtual float		GetZFar() = 0;

	virtual void		GetScreenFadeDistances(float* pMin, float* pMax, float* pScale) = 0;

	virtual C_BaseEntity* GetCurrentlyDrawingEntity() = 0;
	virtual void		SetCurrentlyDrawingEntity(C_BaseEntity* pEnt) = 0;

	virtual bool		UpdateShadowDepthTexture(ITexture* pRenderTarget, ITexture* pDepthTexture, const CViewSetup& shadowView, bool bRenderWorldAndObjects = true, bool bRenderViewModels = false) = 0;

	virtual void		FreezeFrame(float flFreezeTime) = 0;

	virtual void		InitFadeData(void) = 0;
};

extern IViewRender* view;

#endif  