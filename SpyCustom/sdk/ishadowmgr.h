#ifndef ISHADOWMGR_H
#define ISHADOWMGR_H

#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "vmatrix.h"


class IMaterial;
class Vector;
class Vector2D;
struct model_t;
typedef unsigned short ModelInstanceHandle_t;
class IClientRenderable;
class ITexture;

#define ENGINE_SHADOWMGR_INTERFACE_VERSION	"VEngineShadowMgr002"


enum ShadowFlags_t
{
	SHADOW_FLAGS_FLASHLIGHT = (1 << 0),
	SHADOW_FLAGS_SHADOW = (1 << 1),
	SHADOW_FLAGS_LAST_FLAG = SHADOW_FLAGS_SHADOW
};

#define SHADOW_FLAGS_PROJECTED_TEXTURE_TYPE_MASK ( SHADOW_FLAGS_FLASHLIGHT | SHADOW_FLAGS_SHADOW )



typedef unsigned short ShadowHandle_t;

enum
{
	SHADOW_HANDLE_INVALID = (ShadowHandle_t)~0
};


enum ShadowCreateFlags_t
{
	SHADOW_CACHE_VERTS = (1 << 0),
	SHADOW_FLASHLIGHT = (1 << 1),

	SHADOW_LAST_FLAG = SHADOW_FLASHLIGHT,
};


struct ShadowInfo_t
{
	VMatrix		m_WorldToShadow;

	float			m_FalloffOffset;
	float			m_MaxDist;
	float			m_FalloffAmount;	       
	Vector2D		m_TexOrigin;
	Vector2D		m_TexSize;
	unsigned char	m_FalloffBias;
};

struct FlashlightState_t;

abstract_class IShadowMgr
{
public:
	virtual ShadowHandle_t CreateShadow(IMaterial * pMaterial, IMaterial * pModelMaterial, void* pBindProxy, int creationFlags) = 0;
	virtual void DestroyShadow(ShadowHandle_t handle) = 0;

	virtual void SetShadowMaterial(ShadowHandle_t handle, IMaterial* pMaterial, IMaterial* pModelMaterial, void* pBindProxy) = 0;

	virtual void ProjectShadow(ShadowHandle_t handle, const Vector& origin,
		const Vector& projectionDir, const VMatrix& worldToShadow, const Vector2D& size,
		int nLeafCount, const int* pLeafList,
		float maxHeight, float falloffOffset, float falloffAmount, const Vector& vecCasterOrigin) = 0;

	virtual void ProjectFlashlight(ShadowHandle_t handle, const VMatrix& worldToShadow, int nLeafCount, const int* pLeafList) = 0;

	virtual const ShadowInfo_t& GetInfo(ShadowHandle_t handle) = 0;

	virtual const Frustum_t& GetFlashlightFrustum(ShadowHandle_t handle) = 0;

	virtual void AddShadowToBrushModel(ShadowHandle_t handle,
		model_t* pModel, const Vector& origin, const QAngle& angles) = 0;

	virtual void RemoveAllShadowsFromBrushModel(model_t* pModel) = 0;

	virtual void SetShadowTexCoord(ShadowHandle_t handle, float x, float y, float w, float h) = 0;

	virtual void AddShadowToModel(ShadowHandle_t shadow, ModelInstanceHandle_t instance) = 0;
	virtual void RemoveAllShadowsFromModel(ModelInstanceHandle_t instance) = 0;

	virtual void ClearExtraClipPlanes(ShadowHandle_t shadow) = 0;
	virtual void AddExtraClipPlane(ShadowHandle_t shadow, const Vector& normal, float dist) = 0;

	virtual void EnableShadow(ShadowHandle_t shadow, bool bEnable) = 0;

	virtual void SetFalloffBias(ShadowHandle_t shadow, unsigned char ucBias) = 0;

	virtual void UpdateFlashlightState(ShadowHandle_t shadowHandle, const FlashlightState_t& lightState) = 0;

	virtual void DrawFlashlightDepthTexture() = 0;

	virtual void AddFlashlightRenderable(ShadowHandle_t shadow, IClientRenderable* pRenderable) = 0;
	virtual ShadowHandle_t CreateShadowEx(IMaterial* pMaterial, IMaterial* pModelMaterial, void* pBindProxy, int creationFlags) = 0;

	virtual void SetFlashlightDepthTexture(ShadowHandle_t shadowHandle, ITexture* pFlashlightDepthTexture, unsigned char ucShadowStencilBit) = 0;

	virtual const FlashlightState_t& GetFlashlightState(ShadowHandle_t handle) = 0;

	virtual void SetFlashlightRenderState(ShadowHandle_t handle) = 0;
};


#endif