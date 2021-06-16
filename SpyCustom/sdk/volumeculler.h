#ifndef VOLUME_CULLER_H
#define VOLUME_CULLER_H

#ifdef _WIN32
#pragma once
#endif

#include "vector.h"
#include "vplane.h"
#include "ssemath.h"
#include "memalloc.h" 

class CVolumeCuller : public CAlignedNewDelete< 16 >
{
public:
	inline CVolumeCuller() { Clear(); }

	inline void Clear() { m_nNumInclusionVolumePlanes = 0; m_bHasExclusionFrustum = false; m_bHasBaseFrustum = false; m_bCullSmallObjects = false; ClearCullCheckStats(); }

	inline bool IsValid() const { return m_bHasExclusionFrustum || (m_nNumInclusionVolumePlanes != 0); }

	bool CheckBox(const VectorAligned& mins, const VectorAligned& maxs) const;
	bool CheckBox(const Vector& mins, const Vector& maxs) const;
	bool CheckBoxCenterHalfDiagonal(const VectorAligned& center, const VectorAligned& halfDiagonal) const;

	enum
	{
		cNumBaseFrustumPlanes = 6,

		cNumExclusionFrustumPlanes = 6,

		cMaxInclusionVolumePlanes = 12
	};

	inline bool HasBaseFrustum() const { return m_bHasBaseFrustum; }
	void SetBaseFrustumPlanes(const VPlane* pPlanes);

	void GetBaseFrustumPlanes(VPlane* pBasePlanes) const;
	int GetNumBaseFrustumPlanes() const { return cNumBaseFrustumPlanes; }

	inline bool HasExclusionFrustum() const { return m_bHasExclusionFrustum; }
	void SetExclusionFrustumPlanes(const VPlane* pPlanes);

	int GetNumExclusionFrustumPlanes() const { return cNumExclusionFrustumPlanes; }
	const fltx4* GetExclusionFrustumPlanes() const { return m_ExclusionFrustumPlanes; }

	inline bool HasInclusionVolume() const { return m_nNumInclusionVolumePlanes != 0; }
	void SetInclusionVolumePlanes(const VPlane* pPlanes, uint nNumPlanes);

	int GetNumInclusionVolumePlanes() const { return m_nNumInclusionVolumePlanes; }
	const fltx4* GetInclusionVolumePlanes() const { return m_InclusionVolumePlanes; }

	bool GetCullSmallObjects() const { return m_bCullSmallObjects; }
	float GetSmallObjectCullVolumeThreshold() const { return m_flSmallObjectCullVolumeThreshold; }

	void SetCullSmallObjects(bool bCullSmallObjects, float flCullVolumeThreshold) { m_bCullSmallObjects = bCullSmallObjects; m_flSmallObjectCullVolumeThreshold = flCullVolumeThreshold; }

	struct CullCheckStats_t
	{
		uint m_nTotalAABB;
		uint m_nTotalAABBPassed;

		uint m_nTotalCenterHalfDiagonal;
		uint m_nTotalCenterHalfDiagonalPassed;
	};

	inline void ClearCullCheckStats() { memset(&m_Stats, 0, sizeof(m_Stats)); }
	inline CullCheckStats_t& GetStats() const { return m_Stats; }

private:
	fourplanes_t	m_baseplanes[2];

	fltx4 m_ExclusionFrustumPlanes[cNumExclusionFrustumPlanes];

	fltx4 m_InclusionVolumePlanes[cMaxInclusionVolumePlanes];

	uint m_nNumInclusionVolumePlanes;

	bool m_bHasBaseFrustum : 1;
	bool m_bHasExclusionFrustum : 1;

	bool m_bCullSmallObjects : 1;
	float m_flSmallObjectCullVolumeThreshold;

	mutable CullCheckStats_t m_Stats;
};

#endif  