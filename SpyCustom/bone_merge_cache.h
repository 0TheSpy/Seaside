#ifndef BONE_MERGE_CACHE_H
#define BONE_MERGE_CACHE_H
#ifdef _WIN32
#pragma once
#endif


class C_BaseAnimating;
class CStudioHdr;


#include "vector.h"


class CBoneMergeCache
{
public:

	CBoneMergeCache();

	void Init(C_BaseAnimating* pOwner);

	void UpdateCache();

	void MergeMatchingBones(int boneMask);

	void CopyParentToChild(const Vector parentPos[], const Quaternion parentQ[], Vector childPos[], Quaternion childQ[], int boneMask);
	void CopyChildToParent(const Vector childPos[], const Quaternion childQ[], Vector parentPos[], Quaternion parentQ[], int boneMask);

	int IsBoneMerged(int iBone) const;

	bool GetAimEntOrigin(Vector* pAbsOrigin, QAngle* pAbsAngles);

	bool GetRootBone(matrix3x4_t& rootBone);

private:

	C_BaseAnimating* m_pOwner;

	C_BaseAnimating* m_pFollow;
	CStudioHdr* m_pFollowHdr;
	const studiohdr_t* m_pFollowRenderHdr;
	CStudioHdr* m_pOwnerHdr;

	int				m_nFollowBoneSetupMask;

	class CMergedBone
	{
	public:
		unsigned short m_iMyBone;
		unsigned short m_iParentBone;
	};

	CUtlVector<CMergedBone> m_MergedBones;
	CUtlVector<unsigned char> m_BoneMergeBits;	              
};


inline int CBoneMergeCache::IsBoneMerged(int iBone) const
{
	if (m_pOwnerHdr)
		return m_BoneMergeBits[iBone >> 3] & (1 << (iBone & 7));
	else
		return 0;
}


#endif  