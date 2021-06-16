#ifndef CS_VIEW_SCENE_H
#define CS_VIEW_SCENE_H
#ifdef _WIN32
#pragma once
#endif

#include "viewrender.h"

class CCSViewRender : public CViewRender
{
public:
	CCSViewRender();

	virtual void Init(void);

	virtual void GetScreenFadeDistances(float* min, float* max);

	virtual void Render2DEffectsPreHUD(const CViewSetup& view);
	virtual void Render2DEffectsPostHUD(const CViewSetup& view);
	virtual void RenderPlayerSprites(void);

	void PerformFlashbangEffect(const CViewSetup& view);
	void PerformNightVisionEffect(const CViewSetup& view);

	ITexture* m_pFlashTexture;
};

#endif 