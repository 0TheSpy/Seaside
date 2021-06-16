#if !defined( IEFX_H )
#define IEFX_H
#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "vector.h"
#include "dlight.h" 

struct model_t;
struct dlight_t;
class IMaterial;

#define	MAX_DLIGHTS		32

abstract_class IVEfx
{
public:
	virtual	int				Draw_DecalIndexFromName(char* name) = 0;

	virtual	void			DecalShoot(int textureIndex, int entity,
		const model_t* model, const Vector& model_origin, const QAngle& model_angles,
		const Vector& position, const Vector* saxis, int flags) = 0;

	virtual	void			DecalColorShoot(int textureIndex, int entity,
		const model_t* model, const Vector& model_origin, const QAngle& model_angles,
		const Vector& position, const Vector* saxis, int flags, const color32& rgbaColor) = 0;

	virtual void			PlayerDecalShoot(IMaterial* material, void* userdata, int entity, const model_t* model,
		const Vector& model_origin, const Vector& model_angles,
		const Vector& position, const Vector* saxis, int flags, const color32& rgbaColor) = 0;

	virtual	dlight_t* CL_AllocDlight(int key) = 0;

	virtual	dlight_t* CL_AllocElight(int key) = 0;

	virtual int CL_GetActiveDLights(dlight_t* pList[MAX_DLIGHTS]) = 0;

	virtual	const char* Draw_DecalNameFromIndex(int nIndex) = 0;

	virtual dlight_t* GetElightByKey(int key) = 0;
};

#define VENGINE_EFFECTS_INTERFACE_VERSION "VEngineEffects001"

extern IVEfx* effects;

#endif  