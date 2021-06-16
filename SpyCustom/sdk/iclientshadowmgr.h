#ifndef ICLIENTSHADOWMGR_H
#define ICLIENTSHADOWMGR_H

#ifdef _WIN32
#pragma once
#endif

#include "igamesystem.h"
#include "icliententityinternal.h"
#include "ishadowmgr.h"
#include "ivrenderview.h"
#include "itoolentity.h"

#include "IGameSystem.h" 

struct FlashlightState_t;


enum ShadowReceiver_t
{
	SHADOW_RECEIVER_BRUSH_MODEL = 0,
	SHADOW_RECEIVER_STATIC_PROP,
	SHADOW_RECEIVER_STUDIO_MODEL,
};


class IClientShadowMgr : public IGameSystemPerFrame
{
public:
	virtual ClientShadowHandle_t CreateShadow(ClientEntityHandle_t entity, int nEntIndex, int flags, void* pSplitScreenBits = 0) = 0;
	virtual void DestroyShadow(ClientShadowHandle_t handle) = 0;

	virtual ClientShadowHandle_t CreateFlashlight(const FlashlightState_t& lightState) = 0;
	virtual void UpdateFlashlightState(ClientShadowHandle_t shadowHandle, const FlashlightState_t& lightState) = 0;
	virtual void DestroyFlashlight(ClientShadowHandle_t handle) = 0;

	virtual ClientShadowHandle_t CreateProjection(const FlashlightState_t& lightState) = 0;
	virtual void UpdateProjectionState(ClientShadowHandle_t shadowHandle, const FlashlightState_t& lightState) = 0;
	virtual void DestroyProjection(ClientShadowHandle_t handle) = 0;

	virtual void UpdateProjectedTexture(ClientShadowHandle_t handle, bool force = false) = 0;

	virtual void AddToDirtyShadowList(ClientShadowHandle_t handle, bool force = false) = 0;
	virtual void AddToDirtyShadowList(IClientRenderable* pRenderable, bool force = false) = 0;

	virtual void AddShadowToReceiver(ClientShadowHandle_t handle,
		IClientRenderable* pRenderable, ShadowReceiver_t type) = 0;

	virtual void RemoveAllShadowsFromReceiver(
		IClientRenderable* pRenderable, ShadowReceiver_t type) = 0;

	virtual void ComputeShadowTextures(const CViewSetup& view, int leafCount, WorldListLeafData_t* pLeafList) = 0;

	virtual void UnlockAllShadowDepthTextures() = 0;

	virtual void RenderShadowTexture(int w, int h) = 0;

	virtual void SetShadowDirection(const Vector& dir) = 0;
	virtual const Vector& GetShadowDirection() const = 0;

	virtual void SetShadowColor(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void SetShadowDistance(float flMaxDistance) = 0;
	virtual void SetShadowBlobbyCutoffArea(float flMinArea) = 0;
	virtual void SetFalloffBias(ClientShadowHandle_t handle, unsigned char ucBias) = 0;

	virtual void MarkRenderToTextureShadowDirty(ClientShadowHandle_t handle) = 0;

	virtual void AdvanceFrame() = 0;

	virtual void SetFlashlightTarget(ClientShadowHandle_t shadowHandle, EHANDLE targetEntity) = 0;

	virtual void SetFlashlightLightWorld(ClientShadowHandle_t shadowHandle, bool bLightWorld) = 0;

	virtual void SetShadowsDisabled(bool bDisabled) = 0;

	virtual void ComputeShadowDepthTextures(const CViewSetup& pView, bool bSetup = false) = 0;

	virtual void DrawVolumetrics(const CViewSetup& view) = 0;

	virtual void SetShadowFromWorldLightsEnabled(bool bEnable) = 0;

	virtual void DrawDeferredShadows(const CViewSetup& view, int leafCount, WorldListLeafData_t* pLeafList) = 0;

	virtual void InitRenderTargets() = 0;

	virtual void ReprojectShadows() = 0;

	virtual void UpdateSplitscreenLocalPlayerShadowSkip() = 0;

	virtual void GetFrustumExtents(ClientShadowHandle_t handle, Vector& vecMin, Vector& vecMax) = 0;

	virtual void ShutdownRenderTargets(void) = 0;
};



extern IClientShadowMgr* g_pClientShadowMgr;

#endif  