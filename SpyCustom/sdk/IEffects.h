#ifndef IEFFECTS_H
#define IEFFECTS_H

#ifdef _WIN32
#pragma once
#endif

#include "basetypes.h"
#include "vector.h"
#include "interface.h"
#include "ipredictionsystem.h"

enum ShakeCommand_t;
class Vector;
class CGameTrace;
typedef CGameTrace trace_t;


#define IEFFECTS_INTERFACE_VERSION	"IEffects001"
abstract_class IEffects : public IPredictionSystem
{
public:
	virtual ~IEffects() {};
	virtual void Beam(const Vector & Start, const Vector & End, int nModelIndex,
		int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
		float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
		unsigned char noise, unsigned char red, unsigned char green,
		unsigned char blue, unsigned char brightness, unsigned char speed) = 0;

	virtual void Smoke(const Vector& origin, int modelIndex, float scale, float framerate) = 0;

	virtual void Sparks(const Vector& position, int nMagnitude = 1, int nTrailLength = 1, const Vector* pvecDir = NULL) = 0;

	virtual void Dust(const Vector& pos, const Vector& dir, float size, float speed) = 0;

	virtual void MuzzleFlash(const Vector& vecOrigin, const Vector& vecAngles, float flScale, int iType) = 0; 

	virtual void MetalSparks(const Vector& position, const Vector& direction) = 0;

	virtual void EnergySplash(const Vector& position, const Vector& direction, bool bExplosive = false) = 0;

	virtual void Ricochet(const Vector& position, const Vector& direction) = 0;

	virtual float Time() = 0;
	virtual bool IsServer() = 0;

	virtual void SuppressEffectsSounds(bool bSuppress) = 0;
};



#endif  