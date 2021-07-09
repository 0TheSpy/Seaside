#ifndef VIEW_SHARED_H
#define VIEW_SHARED_H

#ifdef _WIN32
#pragma once
#endif

#include "convar.h"
#include "vector.h"
#include "MaterialSystemUtil.h"


enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10,           
	VIEW_CLEAR_STENCIL = 0x20,
};

enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3,
};


class CViewSetup
{
public:
	

	float ComputeViewMatrices(VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection) const;

	int			x;
	int			m_nUnscaledX;
	int			y;
	int			m_nUnscaledY;
	int			width;
	int			m_nUnscaledWidth;
	int			height;
	int			m_nUnscaledHeight;
	  
	bool		m_bOrtho;
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	bool		m_bCustomViewMatrix;
	matrix3x4_t	m_matCustomViewMatrix;

	bool		m_bCustomProjMatrix;
	VMatrix		m_matCustomProjMatrix;

	const CVolumeCuller* m_pCSMVolumeCuller;

	float		fov;
	float		fovViewmodel;

	Vector		origin;

	Vector		angles;
	float		zNear;
	float		zFar;

	float		zNearViewmodel;
	float		zFarViewmodel;

	float		m_flAspectRatio;

	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	int			m_nDoFQuality;

	void*	m_nMotionBlurMode;
	float	m_flShutterTime;				  
	Vector	m_vShutterOpenPosition;			      
	QAngle	m_shutterOpenAngles;			
	Vector	m_vShutterClosePosition;		      
	QAngle	m_shutterCloseAngles;			 

	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	bool		m_bOffCenter : 1;

	bool		m_bRenderToSubrectOfLargerScreen : 1;

	bool		m_bDoBloomAndToneMapping : 1;
	bool		m_bDoDepthOfField : 1;
	bool		m_bHDRTarget : 1;
	bool		m_bDrawWorldNormal : 1;
	bool		m_bCullFrontFaces : 1;

	bool		m_bCacheFullSceneState : 1;

	bool		m_bCSMView : 1;
};



#endif  
