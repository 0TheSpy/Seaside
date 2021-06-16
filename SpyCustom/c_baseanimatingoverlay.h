#ifndef C_BASEANIMATINGOVERLAY_H
#define C_BASEANIMATINGOVERLAY_H
#pragma once

#include "c_baseanimating.h"

#define CBaseAnimatingOverlay C_BaseAnimatingOverlay


class C_BaseAnimatingOverlay : public C_BaseAnimating
{
public:
	DECLARE_CLASS(C_BaseAnimatingOverlay, C_BaseAnimating);
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();


	C_BaseAnimatingOverlay();

	virtual CStudioHdr* OnNewModel();

	C_AnimationLayer* GetAnimOverlay(int i);
	void SetNumAnimOverlays(int num);	           
	int GetNumAnimOverlays() const;

	virtual void	GetRenderBounds(Vector& theMins, Vector& theMaxs);

	void			CheckForLayerChanges(CStudioHdr* hdr, float currentTime);

	virtual void	AccumulateLayers(IBoneSetup& boneSetup, Vector pos[], Quaternion q[], float currentTime);
	virtual void DoAnimationEvents(CStudioHdr* pStudioHdr);

	enum
	{
		MAX_OVERLAYS = 15,
	};

	CUtlVector < C_AnimationLayer >	m_AnimOverlay;

	CUtlVector < CInterpolatedVar< C_AnimationLayer > >	m_iv_AnimOverlay;

	float m_flOverlayPrevEventCycle[MAX_OVERLAYS];

private:
	C_BaseAnimatingOverlay(const C_BaseAnimatingOverlay&);     
};


EXTERN_RECV_TABLE(DT_BaseAnimatingOverlay);


#endif  