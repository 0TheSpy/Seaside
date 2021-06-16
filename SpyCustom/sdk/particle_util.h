#ifndef PARTICLE_UTIL_H
#define PARTICLE_UTIL_H

#include "IMesh.h"
#include "particledraw.h"
#include "particlemgr.h"
#include "cdll_client_int.h"
#include "timedevent.h"



inline float FLerp(float minVal, float maxVal, float t)
{
	return minVal + (maxVal - minVal) * t;
}

inline Vector VecLerp(const Vector& minVal, const Vector& maxVal, float t)
{
	return minVal + (maxVal - minVal) * t;
}

inline float FRand(float minVal, float maxVal)
{
	return minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

inline void PhysicallySimulate(Vector& pos, Vector& velocity, const Vector& acceleration, const float fTimeDelta)
{
	pos = pos + (velocity + (acceleration * fTimeDelta * 0.5f)) * fTimeDelta;
	velocity = velocity + acceleration * fTimeDelta;
}


inline Vector GetGravityVector()
{
	return Vector(0, 0, -150);
}


inline void RenderParticle_Color255SizeSpecularTCoord3(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size,
	const unsigned char* specular,
	const float tCoord
)
{
	if (alpha < 0.5f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha);

	pBuilder->Position3f(pos.x - size, pos.y - size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord3f(0, pDraw->m_pSubTexture->m_tCoordMins[0], pDraw->m_pSubTexture->m_tCoordMaxs[1], tCoord);
	pBuilder->Specular3ubv(specular);
	pBuilder->AdvanceVertex();

	pBuilder->Position3f(pos.x - size, pos.y + size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord3f(0, pDraw->m_pSubTexture->m_tCoordMins[0], pDraw->m_pSubTexture->m_tCoordMins[1], tCoord);
	pBuilder->Specular3ubv(specular);
	pBuilder->AdvanceVertex();

	pBuilder->Position3f(pos.x + size, pos.y + size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord3f(0, pDraw->m_pSubTexture->m_tCoordMaxs[0], pDraw->m_pSubTexture->m_tCoordMins[1], tCoord);
	pBuilder->Specular3ubv(specular);
	pBuilder->AdvanceVertex();

	pBuilder->Position3f(pos.x + size, pos.y - size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord3f(0, pDraw->m_pSubTexture->m_tCoordMaxs[0], pDraw->m_pSubTexture->m_tCoordMaxs[1], tCoord);
	pBuilder->Specular3ubv(specular);
	pBuilder->AdvanceVertex();
}


inline void RenderParticle_Color255Size(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size)
{
	if (alpha < 0.5f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha);

	pBuilder->Position3f(pos.x - size, pos.y - size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord2f(0, pDraw->m_pSubTexture->m_tCoordMins[0], pDraw->m_pSubTexture->m_tCoordMaxs[1]);
	pBuilder->AdvanceVertex();

	pBuilder->Position3f(pos.x - size, pos.y + size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord2f(0, pDraw->m_pSubTexture->m_tCoordMins[0], pDraw->m_pSubTexture->m_tCoordMins[1]);
	pBuilder->AdvanceVertex();

	pBuilder->Position3f(pos.x + size, pos.y + size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord2f(0, pDraw->m_pSubTexture->m_tCoordMaxs[0], pDraw->m_pSubTexture->m_tCoordMins[1]);
	pBuilder->AdvanceVertex();

	pBuilder->Position3f(pos.x + size, pos.y - size, pos.z);
	pBuilder->Color4ubv(ubColor);
	pBuilder->TexCoord2f(0, pDraw->m_pSubTexture->m_tCoordMaxs[0], pDraw->m_pSubTexture->m_tCoordMaxs[1]);
	pBuilder->AdvanceVertex();
}


inline void RenderParticle_Color255SizeNormal(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size,
	const Vector& vNormal)
{
	if (alpha < 0.5f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha);

}


inline void RenderParticle_Color255SizeNormalAngle(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size,
	const Vector& vNormal,
	const float angle)
{
	if (alpha < 0.5f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha);

	float ca = (float)cos(angle);
	float sa = (float)sin(angle);

}


inline void RenderParticle_ColorSize(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size
)
{
	if (alpha < 0.001f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x * 254.9f);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y * 254.9f);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z * 254.9f);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha * 254.9f);

}


inline void RenderParticle_ColorSizeAngle(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size,
	const float angle)
{
	if (alpha < 0.001f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x * 254.9f);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y * 254.9f);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z * 254.9f);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha * 254.9f);

	float sa, ca;
	SinCos(angle, &sa, &ca);

}

inline void RenderParticle_ColorSizeAngles(
	ParticleDraw* pDraw,
	const Vector& pos,
	const Vector& color,
	const float alpha,
	const float size,
	const QAngle& angles)
{
	if (alpha < 0.001f)
		return;

	CMeshBuilder* pBuilder = pDraw->GetMeshBuilder();
	if (!pBuilder)
		return;

	unsigned char ubColor[4];
	ubColor[0] = (unsigned char)RoundFloatToInt(color.x * 254.9f);
	ubColor[1] = (unsigned char)RoundFloatToInt(color.y * 254.9f);
	ubColor[2] = (unsigned char)RoundFloatToInt(color.z * 254.9f);
	ubColor[3] = (unsigned char)RoundFloatToInt(alpha * 254.9f);

	Vector vNorm, vWidth, vHeight;
	AngleVectors(angles, &vNorm, &vWidth, &vHeight);

	Vector vVertex = pos;
}

inline float GetAlphaDistanceFade(
	const Vector& pos,
	const float fadeNearDist,
	const float fadeFarDist)
{
	if (-pos.z > fadeFarDist)
	{
		return 1;
	}
	else if (-pos.z > fadeNearDist)
	{
		return (-pos.z - fadeNearDist) / (fadeFarDist - fadeNearDist);
	}
	else
	{
		return 0;
	}
}


inline Vector WorldGetLightForPoint(const Vector& vPos, bool bClamp)
{
#if defined(PARTICLEPROTOTYPE_APP)
	return Vector(1, 1, 1);
#else
	return engine->GetLightForPoint(vPos, bClamp);
#endif
}

#endif