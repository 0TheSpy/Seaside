#ifndef IVMODELINFO_H
#define IVMODELINFO_H

#ifdef _WIN32
#pragma once
#endif

#include "platform.h"
#include "dbg.h"
#include "gametrace.h"

class IMaterial;
class KeyValues;
struct vcollide_t;
struct model_t;
class Vector;
class QAngle;
//class CGameTrace;
struct cplane_t;
typedef CGameTrace trace_t;
struct studiohdr_t;
struct virtualmodel_t;
typedef unsigned char byte;
struct virtualterrainparams_t;
class CPhysCollide;
typedef unsigned short MDLHandle_t;
class CUtlBuffer;
class IClientRenderable;

class CStudioHdr;
enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,          
};

abstract_class IModelLoadCallback
{
public:
	virtual void OnModelLoadComplete(const model_t * pModel) = 0;

protected:
	~IModelLoadCallback();
};


class CRefCountedModelIndex
{
private:
	int m_nIndex;
public:
	CRefCountedModelIndex() : m_nIndex(-1) { }
	~CRefCountedModelIndex() { Set(-1); }

	CRefCountedModelIndex(const CRefCountedModelIndex& src) : m_nIndex(-1) { Set(src.m_nIndex); }
	CRefCountedModelIndex& operator=(const CRefCountedModelIndex& src) { Set(src.m_nIndex); return *this; }

	explicit CRefCountedModelIndex(int i) : m_nIndex(-1) { Set(i); }
	CRefCountedModelIndex& operator=(int i) { Set(i); return *this; }

	int Get() const { return m_nIndex; }
	void Set(int i);
	void Clear() { Set(-1); }

	operator int() const { return m_nIndex; }
};


#define VMODELINFO_CLIENT_INTERFACE_VERSION		"VModelInfoClient006"
#define VMODELINFO_SERVER_INTERFACE_VERSION_3	"VModelInfoServer003"
#define VMODELINFO_SERVER_INTERFACE_VERSION		"VModelInfoServer004"

inline bool IsDynamicModelIndex(int modelindex) { return modelindex < -1; }
inline bool IsClientOnlyModelIndex(int modelindex) { return modelindex < -1 && (modelindex & 1); }


class IVModelInfo
{
public:
	virtual                                 ~IVModelInfo(void) {}
	virtual const model_t* GetModel(int modelindex) const = 0;
	virtual int                             GetModelIndex(const char* name) const = 0;
	virtual const char* GetModelName(const model_t* model) const = 0;
	virtual vcollide_t* GetVCollide(const model_t* model) const = 0;
	virtual vcollide_t* GetVCollide(int modelindex) const = 0;
	virtual void                            GetModelBounds(const model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual void                            GetModelRenderBounds(const model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual int                             GetModelFrameCount(const model_t* model) const = 0;
	virtual int                             GetModelType(const model_t* model) const = 0;
	virtual void* GetModelExtraData(const model_t* model) = 0;
	virtual bool                            ModelHasMaterialProxy(const model_t* model) const = 0;
	virtual bool                            IsTranslucent(model_t const* model) const = 0;
	virtual bool                            IsTranslucentTwoPass(const model_t* model) const = 0;
	virtual void                            Unused0() {};
	virtual void UNUSED() = 0;
	virtual void UNUSE11D() = 0;
	virtual RenderableTranslucencyType_t    ComputeTranslucencyType(const model_t* model, int nSkin, int nBody) = 0;
	virtual int                             GetModelMaterialCount(const model_t* model) const = 0;
	virtual void                            GetModelMaterials(const model_t* model, int count, IMaterial** ppMaterial) = 0;
	virtual bool                            IsModelVertexLit(const model_t* model) const = 0;
	virtual const char* GetModelKeyValueText(const model_t* model) = 0;
	virtual bool                            GetModelKeyValue(const model_t* model, CUtlBuffer& buf) = 0;
	virtual float                           GetModelRadius(const model_t* model) = 0;
	virtual CStudioHdr* GetStudioHdr(MDLHandle_t handle) = 0;
	virtual const studiohdr_t* FindModel(const studiohdr_t* pStudioHdr, void** cache, const char* modelname) const = 0;
	virtual const studiohdr_t* FindModel(void* cache) const = 0;
	virtual virtualmodel_t* GetVirtualModel(const studiohdr_t* pStudioHdr) const = 0;
	virtual uint8_t* GetAnimBlock(const studiohdr_t* pStudioHdr, int iBlock) const = 0;
	virtual void                            GetModelMaterialColorAndLighting(const model_t* model, Vector const& origin, QAngle const& angles, trace_t* pTrace, Vector& lighting, Vector& matColor) = 0;
	virtual void                            GetIlluminationPoint(const model_t* model, IClientRenderable* pRenderable, Vector const& origin, QAngle const& angles, Vector* pLightingCenter) = 0;
	virtual int                             GetModelContents(int modelIndex) const = 0;
	virtual studiohdr_t* GetStudiomodel(const model_t* mod) = 0;
	virtual int                             GetModelSpriteWidth(const model_t* model) const = 0;
	virtual int                             GetModelSpriteHeight(const model_t* model) const = 0;
	virtual void                            SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void                            GetLevelScreenFadeRange(float* pMinArea, float* pMaxArea) const = 0;
	virtual void                            SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual unsigned char                   ComputeLevelScreenFade(const Vector& vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char                   ComputeViewScreenFade(const Vector& vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual int                             GetAutoplayList(const studiohdr_t* pStudioHdr, unsigned short** pAutoplayList) const = 0;
	virtual CPhysCollide* GetCollideForVirtualTerrain(int index) = 0;
	virtual bool                            IsUsingFBTexture(const model_t* model, int nSkin, int nBody, IClientRenderable** pClientRenderable) const = 0;
	virtual const model_t* FindOrLoadModel(const char* name) const = 0;
	virtual MDLHandle_t                     GetCacheHandle(const model_t* model) const = 0;
	virtual int                             GetBrushModelPlaneCount(const model_t* model) const = 0;
	virtual void                            GetBrushModelPlane(const model_t* model, int nIndex, cplane_t& plane, Vector* pOrigin) const = 0;
	virtual int                             GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool                            UsesEnvCubemap(const model_t* model) const = 0;
	virtual bool                            UsesStaticLighting(const model_t* model) const = 0;
};

typedef IVModelInfo IVModelInfo003;


abstract_class IVModelInfoClient : public IVModelInfo
{
public:
	virtual void OnDynamicModelsStringTableChange(int nStringIndex, const char* pString, const void* pData) = 0;

	virtual const model_t* FindOrLoadModel(const char* name) = 0;
};


struct virtualterrainparams_t
{
	int index;
};




#endif  