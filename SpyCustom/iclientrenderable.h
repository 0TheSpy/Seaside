#ifndef ICLIENTRENDERABLE_H
#define ICLIENTRENDERABLE_H
#ifdef _WIN32
#pragma once
#endif

#include "mathlib.h"
#include "interface.h"
#include "iclientunknown.h"
#include "client_render_handle.h"
#include "ivmodelrender.h"

struct model_t;
struct matrix3x4_t;

extern void DefaultRenderBoundsWorldspace(IClientRenderable* pRenderable, Vector& absMins, Vector& absMaxs);

typedef unsigned short ClientShadowHandle_t;

enum
{
	CLIENTSHADOW_INVALID_HANDLE = (ClientShadowHandle_t)~0
};

enum ShadowType_t
{
	SHADOWS_NONE = 0,
	SHADOWS_SIMPLE,
	SHADOWS_RENDER_TO_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,	      
	SHADOWS_RENDER_TO_DEPTH_TEXTURE,
};


abstract_class IPVSNotify
{
public:
	virtual void OnPVSStatusChanged(bool bInPVS) = 0;
};

struct RenderableInstance_t
{
	uint8 m_nAlpha;
};

abstract_class IClientRenderable
{
public:
	virtual IClientUnknown * GetIClientUnknown() = 0;

	virtual Vector const& GetRenderOrigin(void) = 0;
	virtual QAngle const& GetRenderAngles(void) = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;

	virtual ClientShadowHandle_t	GetShadowHandle() const = 0;

	virtual ClientRenderHandle_t& RenderHandle() = 0;

	virtual const model_t* GetModel() const = 0;
	virtual int						DrawModel(int flags) = 0;

	virtual int		GetBody() = 0;

	virtual void	ComputeFxBlend() = 0;
	virtual int		GetFxBlend(void) = 0;

	virtual void	GetColorModulation(float* color) = 0;

	virtual bool	LODTest() = 0;

	virtual bool	SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;

	virtual void	SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void	DoAnimationEvents(void) = 0;

	virtual IPVSNotify* GetPVSNotifyInterface() = 0;

	virtual void	GetRenderBounds(Vector& mins, Vector& maxs) = 0;

	virtual void	GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;

	virtual void	GetShadowRenderBounds(Vector& mins, Vector& maxs, ShadowType_t shadowType) = 0;

	virtual bool	ShouldReceiveProjectedTextures(int flags) = 0;

	virtual bool	GetShadowCastDistance(float* pDist, ShadowType_t shadowType) const = 0;
	virtual bool	GetShadowCastDirection(Vector* pDirection, ShadowType_t shadowType) const = 0;

	virtual bool	IsShadowDirty() = 0;
	virtual void	MarkShadowDirty(bool bDirty) = 0;

	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;

	virtual ShadowType_t ShadowCastType() = 0;

	virtual void CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t GetModelInstance() = 0;

	virtual const matrix3x4_t& RenderableToWorldTransform() = 0;

	virtual int LookupAttachment(const char* pAttachmentName) = 0;
	virtual	bool GetAttachment(int number, Vector& origin, QAngle& angles) = 0;
	virtual bool GetAttachment(int number, matrix3x4_t& matrix) = 0;

	virtual float* GetRenderClipPlane(void) = 0;

	virtual int		GetSkin() = 0;

	virtual bool	IsTwoPass(void) = 0;

	virtual void	OnThreadedDrawSetup() = 0;

	virtual bool	UsesFlexDelayedWeights() = 0;

	virtual void	RecordToolMessage() = 0;

	virtual bool	IgnoresZBuffer(void) const = 0;
};


abstract_class CDefaultClientRenderable : public IClientUnknown, public IClientRenderable
{
public:
	CDefaultClientRenderable()
	{
		m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;
	}

	virtual const Vector & GetRenderOrigin(void) = 0;
	virtual const QAngle& GetRenderAngles(void) = 0;
	virtual const matrix3x4_t& RenderableToWorldTransform() = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual void					OnThreadedDrawSetup() {}
	virtual int                     GetRenderFlags(void) { return 0; } 
	virtual ClientShadowHandle_t	GetShadowHandle() const
	{
		return CLIENTSHADOW_INVALID_HANDLE;
	}

	virtual ClientRenderHandle_t& RenderHandle()
	{
		return m_hRenderHandle;
	}

	virtual int						GetBody() { return 0; }
	virtual int						GetSkin() { return 0; }
	virtual bool					UsesFlexDelayedWeights() { return false; }

	virtual const model_t* GetModel() const { return NULL; }
	virtual int						DrawModel(int flags) { return 0; }
	virtual bool					LODTest() { return true; }
	virtual bool					SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) { return true; }
	virtual void					SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) {}
	virtual void					DoAnimationEvents(void) {}
	virtual IPVSNotify* GetPVSNotifyInterface() { return NULL; }
	virtual void					GetRenderBoundsWorldspace(Vector& absMins, Vector& absMaxs) { DefaultRenderBoundsWorldspace(this, absMins, absMaxs); }

	virtual void	GetColorModulation(float* color)
	{
		Assert(color);
		color[0] = color[1] = color[2] = 1.0f;
	}

	virtual bool	ShouldReceiveProjectedTextures(int flags)
	{
		return false;
	}

	virtual bool	GetShadowCastDistance(float* pDist, ShadowType_t shadowType) const { return false; }
	virtual bool	GetShadowCastDirection(Vector* pDirection, ShadowType_t shadowType) const { return false; }

	virtual void	GetShadowRenderBounds(Vector& mins, Vector& maxs, ShadowType_t shadowType)
	{
		GetRenderBounds(mins, maxs);
	}

	virtual bool IsShadowDirty() { return false; }
	virtual void MarkShadowDirty(bool bDirty) {}
	virtual IClientRenderable* GetShadowParent() { return NULL; }
	virtual IClientRenderable* FirstShadowChild() { return NULL; }
	virtual IClientRenderable* NextShadowPeer() { return NULL; }
	virtual ShadowType_t ShadowCastType() { return SHADOWS_NONE; }
	virtual void CreateModelInstance() {}
	virtual ModelInstanceHandle_t GetModelInstance() { return MODEL_INSTANCE_INVALID; }

	virtual int LookupAttachment(const char* pAttachmentName) { return -1; }
	virtual	bool GetAttachment(int number, Vector& origin, QAngle& angles) { return false; }
	virtual bool GetAttachment(int number, matrix3x4_t& matrix) { return false; }
	virtual bool ComputeLightingOrigin(int nAttachmentIndex, Vector modelLightingCenter, const matrix3x4_t& matrix, Vector& transformedLightingCenter) { return false; } 

	virtual float* GetRenderClipPlane() { return NULL; }

	virtual void RecordToolMessage() {}
	virtual bool	ShouldDrawForSplitScreenUser(int nSlot) { return true; } 
	virtual uint8	OverrideAlphaModulation(uint8 nAlpha) { return nAlpha; } 
	virtual uint8	OverrideShadowAlphaModulation(uint8 nAlpha) { return nAlpha; } 
	virtual void* GetClientModelRenderable() { return 0; }  

	public:
		virtual void SetRefEHandle(const CBaseHandle& handle) { Assert(false); }
		virtual const CBaseHandle& GetRefEHandle() const { Assert(false); return *((CBaseHandle*)0); }

		virtual IClientUnknown* GetIClientUnknown() { return this; }
		virtual ICollideable* GetCollideable() { return 0; }
		virtual IClientRenderable* GetClientRenderable() { return this; }
		virtual IClientNetworkable* GetClientNetworkable() { return 0; }
		virtual IClientEntity* GetIClientEntity() { return 0; }
		virtual C_BaseEntity* GetBaseEntity() { return 0; }
		virtual IClientThinkable* GetClientThinkable() { return 0; }
		virtual void* GetClientAlphaProperty() { return 0; }  


	public:
		ClientRenderHandle_t m_hRenderHandle;
};


#endif  