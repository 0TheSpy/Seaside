#if !defined( ICLIENTLEAFSYSTEM_H )
#define ICLIENTLEAFSYSTEM_H
#ifdef _WIN32
#pragma once
#endif


#include "platform.h"
#include "client_render_handle.h"
#include "ivmodelinfo.h"


#define CLIENTLEAFSYSTEM_INTERFACE_VERSION	"ClientLeafSystem002"


enum RenderableModelType_t
{
	RENDERABLE_MODEL_UNKNOWN_TYPE = -1,
	RENDERABLE_MODEL_ENTITY = 0,
	RENDERABLE_MODEL_STUDIOMDL,
	RENDERABLE_MODEL_STATIC_PROP,
	RENDERABLE_MODEL_BRUSH,
};

enum RenderGroup_t
{
	RENDER_GROUP_OPAQUE = 0,
	RENDER_GROUP_TRANSLUCENT,
	RENDER_GROUP_TRANSLUCENT_IGNOREZ,
	RENDER_GROUP_COUNT,             
};


abstract_class IClientLeafSystemEngine
{
public:
	virtual void CreateRenderableHandle(IClientRenderable* pRenderable, bool bRenderWithViewModels, RenderableTranslucencyType_t nType, RenderableModelType_t nModelType, UINT32 nSplitscreenEnabled = 0xFFFFFFFF) = 0;      
	virtual void RemoveRenderable(ClientRenderHandle_t handle) = 0;
	virtual void AddRenderableToLeaves(ClientRenderHandle_t renderable, int nLeafCount, unsigned short* pLeaves) = 0;
	virtual void SetTranslucencyType(ClientRenderHandle_t handle, RenderableTranslucencyType_t nType) = 0;

};


#endif	 







