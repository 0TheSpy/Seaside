#ifndef CLIENTVIRTUALREALITY_H
#define CLIENTVIRTUALREALITY_H
#if defined( _WIN32 )
#pragma once
#endif

#include "tier3.h"
#include "iclientvirtualreality.h"
#include "view_shared.h"

enum HeadtrackMovementMode_t
{
	HMM_SHOOTFACE_MOVEFACE = 0,		        
	HMM_SHOOTFACE_MOVETORSO,		           
	HMM_SHOOTMOUSE_MOVEFACE,		              
	HMM_SHOOTBOUNDEDMOUSE_LOOKFACE_MOVEFACE,	               
	HMM_SHOOTBOUNDEDMOUSE_LOOKFACE_MOVEMOUSE,	                 

	HMM_SHOOTMOVELOOKMOUSEFACE,		                    
	HMM_SHOOTMOVEMOUSE_LOOKFACE,	              
	HMM_SHOOTMOVELOOKMOUSE,			              

	HMM_LAST,

	HMM_NOOVERRIDE = HMM_LAST		          
};



class CClientVirtualReality : public CTier3AppSystem< IClientVirtualReality >
{
	typedef CTier3AppSystem< IClientVirtualReality > BaseClass;

public:

	CClientVirtualReality();
	~CClientVirtualReality();

	virtual bool							Connect(CreateInterfaceFn factory);
	virtual void							Disconnect();
	virtual void* QueryInterface(const char* pInterfaceName);

	virtual InitReturnVal_t					Init();
	virtual void							Shutdown();

	void StartupComplete();

	virtual void DrawMainMenu() OVERRIDE;


	bool OverrideView(CViewSetup* pViewMiddle, Vector* pViewModelOrigin, QAngle* pViewModelAngles, HeadtrackMovementMode_t hmmMovementOverride);
	bool OverrideStereoView(CViewSetup* pViewMiddle, CViewSetup* pViewLeft, CViewSetup* pViewRight);
	bool OverridePlayerMotion(float flInputSampleFrametime, const QAngle& oldAngles, const QAngle& curAngles, const Vector& curMotion, QAngle* pNewAngles, Vector* pNewMotion);
	bool OverrideWeaponHudAimVectors(Vector* pAimOrigin, Vector* pAimDirection);
	bool CurrentlyZoomed();
	void OverrideTorsoTransform(const Vector& position, const QAngle& angles);
	void CancelTorsoTransformOverride();
	bool CanOverlayHudQuad();
	void GetHUDBounds(Vector* pViewer, Vector* pUL, Vector* pUR, Vector* pLL, Vector* pLR);
	void RenderHUDQuad(bool bBlackout, bool bTranslucent);
	float GetZoomedModeMagnification();
	bool ProcessCurrentTrackingState(float fGameFOV);
	const VMatrix& GetHudProjectionFromWorld();
	void GetTorsoRelativeAim(Vector* pPosition, QAngle* pAngles);
	float GetHUDDistance();
	bool ShouldRenderHUDInWorld();
	const VMatrix& GetWorldFromMidEye() const { return m_WorldFromMidEyeNoDebugCam; }
	void OverrideViewModelTransform(Vector& vmorigin, QAngle& vmangles, bool bUseLargeOverride);
	void AlignTorsoAndViewToWeapon();
	void PostProcessFrame(StereoEye_t eEye);
	void OverlayHUDQuadWithUndistort(const CViewSetup& view, bool bDoUndistort, bool bBlackout, bool bTranslucent);

	void Activate();
	void Deactivate();

private:
	HeadtrackMovementMode_t m_hmmMovementActual;

	VMatrix			m_WorldFromMidEye;

	float			m_fHudHorizontalFov;
	VMatrix			m_WorldFromHud;
	VMatrix			m_HudProjectionFromWorld;
	float			m_fHudHalfWidth;
	float			m_fHudHalfHeight;

	VMatrix			m_TorsoFromMideye;

	VMatrix			m_WorldFromMidEyeNoDebugCam;

	VMatrix			m_WorldFromWeapon;

	QAngle			m_PlayerTorsoAngle;
	Vector			m_PlayerTorsoOrigin;
	Vector			m_PlayerLastMovement;

	QAngle			m_PlayerViewAngle;
	Vector			m_PlayerViewOrigin;

	float			m_WorldZoomScale;

	QAngle			m_OverrideTorsoAngle;
	QAngle			m_OverrideTorsoOffset;
	bool			m_bOverrideTorsoAngle;

	int				m_iAlignTorsoAndViewToWeaponCountdown;

	bool			m_bMotionUpdated;

	RTime32			m_rtLastMotionSample;

	bool m_bNonVRWindowed;
	int m_nNonVRWidth;
	int m_nNonVRHeight;
#if defined( USE_SDL )
	int m_nNonVRSDLDisplayIndex;
#endif
	bool m_bNonVRRawInput;
};

extern CClientVirtualReality g_ClientVirtualReality;

#endif  