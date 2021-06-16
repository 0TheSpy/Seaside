#ifndef SEQUENCE_TRANSITIONER_H
#define SEQUENCE_TRANSITIONER_H
#ifdef _WIN32
#pragma once
#endif


class CSequenceTransitioner
{
public:
	void CheckForSequenceChange(
		CStudioHdr* hdr,
		int nCurSequence,

		bool bForceNewSequence,

		bool bInterpolate
	);

	void UpdateCurrent(
		CStudioHdr* hdr,
		int nCurSequence,
		float flCurCycle,
		float flCurPlaybackRate,
		float flCurTime
	);

	void RemoveAll(void) { m_animationQueue.RemoveAll(); };

public:
	CUtlVector< CAnimationLayer >	m_animationQueue;
};

#endif  