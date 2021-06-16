#ifndef C_BASEPLAYER_H
#define C_BASEPLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_playerlocaldata.h"
#include "c_basecombatcharacter.h"

#include "usercmd.h"
#include "shareddefs.h"
#include "timedevent.h"
#include "smartptr.h"

#include "igameevents.h"
#include "GameEventListener.h"

#if defined USES_ECON_ITEMS
#include "econ_item.h"
#include "game_item_schema.h"
#include "econ_item_view.h"
#endif

class C_BaseCombatWeapon;
class C_BaseViewModel;
class C_FuncLadder;
class CFlashlightEffect;
class C_EconWearable;

extern int g_nKillCamMode;
extern int g_nKillCamTarget1;
extern int g_nKillCamTarget2;

class C_CommandContext
{
public:
	bool			needsprocessing;

	CUserCmd		cmd;
	int				command_number;
};

class C_PredictionError
{
public:
	float	time;
	Vector	error;
};

#define CHASE_CAM_DISTANCE_MIN	16.0f
#define CHASE_CAM_DISTANCE_MAX	96.0f
#define WALL_OFFSET				6.0f


bool IsInFreezeCam(void);

class C_BasePlayer : public C_BaseCombatCharacter, public CGameEventListener
{
public:
	DECLARE_CLASS(C_BasePlayer, C_BaseCombatCharacter);
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	C_BasePlayer();
	virtual			~C_BasePlayer();

	virtual void	Spawn(void);
	virtual void	SharedSpawn();      
	virtual bool	GetSteamID(CSteamID* pID);

	virtual void	OnPreDataChanged(DataUpdateType_t updateType);
	virtual void	OnDataChanged(DataUpdateType_t updateType);

	virtual void	PreDataUpdate(DataUpdateType_t updateType);
	virtual void	PostDataUpdate(DataUpdateType_t updateType);

	virtual void	ReceiveMessage(int classID, bf_read& msg);

	virtual void	OnRestore();

	virtual void	AddEntity(void);

	virtual void	MakeTracer(const Vector& vecTracerSrc, const trace_t& tr, int iTracerType);

	virtual void	GetToolRecordingState(KeyValues* msg);

	virtual float GetPlayerMaxSpeed();

	void	SetAnimationExtension(const char* pExtension);

	C_BaseViewModel* GetViewModel(int viewmodelindex = 0, bool bObserverOK = true);
	C_BaseCombatWeapon* GetActiveWeapon(void) const;
	const char* GetTracerType(void);

	virtual void		CalcView(Vector& eyeOrigin, QAngle& eyeAngles, float& zNear, float& zFar, float& fov);
	virtual void		CalcViewModelView(const Vector& eyeOrigin, const QAngle& eyeAngles);


	void				SmoothViewOnStairs(Vector& eyeOrigin);
	virtual float		CalcRoll(const QAngle& angles, const Vector& velocity, float rollangle, float rollspeed);
	void				CalcViewRoll(QAngle& eyeAngles);
	void				CreateWaterEffects(void);

	virtual void			SetPlayerUnderwater(bool state);
	void					UpdateUnderwaterState(void);
	bool					IsPlayerUnderwater(void) { return m_bPlayerUnderwater; }

	virtual Vector			Weapon_ShootPosition();
	virtual void			Weapon_DropPrimary(void) {}

	virtual Vector			GetAutoaimVector(float flScale);
	void					SetSuitUpdate(const char* name, int fgroup, int iNoRepeat);

	virtual bool	CreateMove(float flInputSampleTime, CUserCmd* pCmd);
	virtual void	AvoidPhysicsProps(CUserCmd* pCmd);

	virtual void	PlayerUse(void);
	CBaseEntity* FindUseEntity(void);
	virtual bool	IsUseableEntity(CBaseEntity* pEntity, unsigned int requiredCaps);

	virtual bool	IsPlayer(void) const { return true; }
	virtual int		GetHealth() const { return m_iHealth; }

	int		GetBonusProgress() const { return m_iBonusProgress; }
	int		GetBonusChallenge() const { return m_iBonusChallenge; }

	virtual int			GetObserverMode() const;
	void				SetObserverMode(int iNewMode);
	virtual CBaseEntity* GetObserverTarget() const;
	void			SetObserverTarget(EHANDLE hObserverTarget);

	bool			AudioStateIsUnderwater(Vector vecMainViewOrigin);

	bool IsObserver() const;
	bool IsHLTV() const;
	bool IsReplay() const;
	void ResetObserverMode();
	bool IsBot(void) const { return false; }

	virtual Vector		 EyePosition();
	virtual const QAngle& EyeAngles();		   
	void				 EyePositionAndVectors(Vector* pPosition, Vector* pForward, Vector* pRight, Vector* pUp);
	virtual const QAngle& LocalEyeAngles();		   

	virtual IRagdoll* GetRepresentativeRagdoll() const;

	virtual void GetRagdollInitBoneArrays(matrix3x4_t* pDeltaBones0, matrix3x4_t* pDeltaBones1, matrix3x4_t* pCurrentBones, float boneDt);

	void			EyeVectors(Vector* pForward, Vector* pRight = NULL, Vector* pUp = NULL);
	void			CacheVehicleView(void);	           


	bool			IsSuitEquipped(void) { return m_Local.m_bWearingSuit; };

	virtual void	TeamChange(int iNewTeam);

	void	Flashlight(void);
	void	UpdateFlashlight(void);

	virtual bool				IsAllowedToSwitchWeapons(void) { return !IsObserver(); }
	virtual C_BaseCombatWeapon* GetActiveWeaponForSelection(void);

	virtual C_BaseAnimating* GetRenderedWeaponModel();

	virtual bool				IsOverridingViewmodel(void) { return false; };
	virtual int					DrawOverriddenViewmodel(C_BaseViewModel* pViewmodel, int flags) { return 0; };

	virtual float				GetDefaultAnimSpeed(void) { return 1.0; }

	void						SetMaxSpeed(float flMaxSpeed) { m_flMaxspeed = flMaxSpeed; }
	float						MaxSpeed() const { return m_flMaxspeed; }

	virtual ShadowType_t		ShadowCastType() { return SHADOWS_NONE; }

	virtual bool				ShouldReceiveProjectedTextures(int flags)
	{
		return false;
	}


	bool						IsLocalPlayer(void) const;

	virtual void				ThirdPersonSwitch(bool bThirdperson);
	static bool					LocalPlayerInFirstPersonView();
	static bool					ShouldDrawLocalPlayer();
	static C_BasePlayer* GetLocalPlayer(void);
	int							GetUserID(void);
	virtual bool				CanSetSoundMixer(void);
	virtual int					GetVisionFilterFlags(bool bWeaponsCheck = false) { return 0x00; }
	bool						HasVisionFilterFlags(int nFlags, bool bWeaponsCheck = false) { return (GetVisionFilterFlags(bWeaponsCheck) & nFlags) == nFlags; }
	virtual void				CalculateVisionUsingCurrentFlags(void) {}
	void						BuildFirstPersonMeathookTransformations(CStudioHdr* hdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed, const char* pchHeadBoneName);

	bool						InFirstPersonView();
	bool						ShouldDrawThisPlayer();

	virtual bool				ViewModel_IsTransparent(void);
	virtual bool				ViewModel_IsUsingFBTexture(void);

#if !defined( NO_ENTITY_PREDICTION )
	void						AddToPlayerSimulationList(C_BaseEntity* other);
	void						SimulatePlayerSimulatedEntities(void);
	void						RemoveFromPlayerSimulationList(C_BaseEntity* ent);
	void						ClearPlayerSimulationList(void);
#endif

	virtual void				PhysicsSimulate(void);
	virtual unsigned int	PhysicsSolidMaskForEntity(void) const { return MASK_PLAYERSOLID; }

	virtual bool				ShouldPredict(void);

	virtual void				PreThink(void);
	virtual void				PostThink(void);

	virtual void				ItemPreFrame(void);
	virtual void				ItemPostFrame(void);
	virtual void				AbortReload(void);

	virtual void				SelectLastItem(void);
	virtual void				Weapon_SetLast(C_BaseCombatWeapon* pWeapon);
	virtual bool				Weapon_ShouldSetLast(C_BaseCombatWeapon* pOldWeapon, C_BaseCombatWeapon* pNewWeapon) { return true; }
	virtual bool				Weapon_ShouldSelectItem(C_BaseCombatWeapon* pWeapon);
	virtual	bool				Weapon_Switch(C_BaseCombatWeapon* pWeapon, int viewmodelindex = 0);		          
	virtual C_BaseCombatWeapon* GetLastWeapon(void) { return m_hLastWeapon.Get(); }
	void						ResetAutoaim(void);
	virtual void 				SelectItem(const char* pstr, int iSubType = 0);

	virtual void				UpdateClientData(void);

	virtual float				GetFOV(void);
	int							GetDefaultFOV(void) const;
	virtual bool				IsZoomed(void) { return false; }
	bool						SetFOV(CBaseEntity* pRequester, int FOV, float zoomRate = 0.0f, int iZoomStart = 0);
	void						ClearZoomOwner(void);

	float						GetFOVDistanceAdjustFactor();

	virtual void				ViewPunch(const QAngle& angleOffset);
	void						ViewPunchReset(float tolerance = 0);

	void						UpdateButtonState(int nUserCmdButtonMask);
	int							GetImpulse(void) const;

	virtual void				Simulate();

	virtual bool				ShouldInterpolate();

	virtual bool				ShouldDraw();
	virtual int					DrawModel(int flags);

	virtual void				OverrideView(CViewSetup* pSetup);

	const char* GetPlayerName();
	virtual const Vector		GetPlayerMins(void) const;    
	virtual const Vector		GetPlayerMaxs(void) const;    

	bool				IsPlayerDead();
	bool				IsPoisoned(void) { return m_Local.m_bPoisoned; }

	C_BaseEntity* GetUseEntity();

	IClientVehicle* GetVehicle();

	bool			IsInAVehicle() const { return (NULL != m_hVehicle.Get()) ? true : false; }
	virtual void	SetVehicleRole(int nRole);
	void					LeaveVehicle(void);

	bool					UsingStandardWeaponsInVehicle(void);

	virtual void			SetAnimation(PLAYER_ANIM playerAnim);

	float					GetTimeBase(void) const;
	float					GetFinalPredictedTime() const;

	bool					IsInVGuiInputMode() const;
	bool					IsInViewModelVGuiInputMode() const;

	C_CommandContext* GetCommandContext();

	int CurrentCommandNumber() const;
	const CUserCmd* GetCurrentUserCommand() const;

	const QAngle& GetPunchAngle();
	void SetPunchAngle(const QAngle& angle);

	float					GetWaterJumpTime() const;
	void					SetWaterJumpTime(float flWaterJumpTime);
	float					GetSwimSoundTime(void) const;
	void					SetSwimSoundTime(float flSwimSoundTime);

	float					GetDeathTime(void) { return m_flDeathTime; }

	void		SetPreviouslyPredictedOrigin(const Vector& vecAbsOrigin);
	const Vector& GetPreviouslyPredictedOrigin() const;

	virtual float GetMinFOV() const;

	virtual void DoMuzzleFlash();
	virtual void PlayPlayerJingle();

	virtual void UpdateStepSound(surfacedata_t* psurface, const Vector& vecOrigin, const Vector& vecVelocity);
	virtual void PlayStepSound(Vector& vecOrigin, surfacedata_t* psurface, float fvol, bool force);
	virtual surfacedata_t* GetFootstepSurface(const Vector& origin, const char* surfaceName);
	virtual void GetStepSoundVelocities(float* velwalk, float* velrun);
	virtual void SetStepSoundTime(stepsoundtimes_t iStepSoundTime, bool bWalking);
	virtual const char* GetOverrideStepSound(const char* pszBaseStepSoundName) { return pszBaseStepSoundName; }

	virtual void OnEmitFootstepSound(const CSoundParameters& params, const Vector& vecOrigin, float fVolume) {}

	void NotePredictionError(const Vector& vDelta);

	void GetPredictionErrorSmoothingVector(Vector& vOffset);

	virtual void ExitLadder() {}
	surfacedata_t* GetLadderSurface(const Vector& origin);

	surfacedata_t* GetSurfaceData(void) { return m_pSurfaceData; }

	void SetLadderNormal(Vector vecLadderNormal) { m_vecLadderNormal = vecLadderNormal; }

	virtual CHintSystem* Hints(void) { return NULL; }
	bool					ShouldShowHints(void) { return Hints() ? Hints()->ShouldShowHints() : false; }
	bool 					HintMessage(int hint, bool bForce = false, bool bOnlyIfClear = false) { return Hints() ? Hints()->HintMessage(hint, bForce, bOnlyIfClear) : false; }
	void 					HintMessage(const char* pMessage) { if (Hints()) Hints()->HintMessage(pMessage); }

	virtual	IMaterial* GetHeadLabelMaterial(void);

	fogparams_t* GetFogParams(void) { return &m_CurrentFog; }
	void					FogControllerChanged(bool bSnap);
	void					UpdateFogController(void);
	void					UpdateFogBlend(void);

	float					GetFOVTime(void) { return m_flFOVTime; }

	virtual void			OnAchievementAchieved(int iAchievement) {}

	bool					ShouldAnnounceAchievement(void) { return m_flNextAchievementAnnounceTime < gpGlobals->curtime; }
	void					SetNextAchievementAnnounceTime(float flTime) { m_flNextAchievementAnnounceTime = flTime; }

#if defined USES_ECON_ITEMS
	void					UpdateWearables();
	C_EconWearable* GetWearable(int i) { return m_hMyWearables[i]; }
	int						GetNumWearables(void) { return m_hMyWearables.Count(); }
#endif

	bool					HasFiredWeapon(void) { return m_bFiredWeapon; }
	void					SetFiredWeapon(bool bFlag) { m_bFiredWeapon = bFlag; }

	virtual bool			CanUseFirstPersonCommand(void) { return true; }

protected:
	fogparams_t				m_CurrentFog;
	EHANDLE					m_hOldFogController;

public:
	int m_StuckLast;

	CNetworkVarEmbedded(CPlayerLocalData, m_Local);

#if defined USES_ECON_ITEMS
	CNetworkVarEmbedded(CAttributeList, m_AttributeList);
#endif

	CPlayerState			pl;

	int						m_iFOV;				   
	int						m_iFOVStart;		          
	float					m_flFOVTime;		      
	int						m_iDefaultFOV;		        
	EHANDLE					m_hZoomOwner;		            
	bool			m_fOnTarget;		     

	char			m_szAnimExtension[32];

	int				m_afButtonLast;
	int				m_afButtonPressed;
	int				m_afButtonReleased;

	int				m_nButtons;

	CUserCmd* m_pCurrentCommand;

	EHANDLE			m_hConstraintEntity;
	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;

protected:

	void				CalcPlayerView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	void				CalcVehicleView(IClientVehicle* pVehicle, Vector& eyeOrigin, QAngle& eyeAngles,
		float& zNear, float& zFar, float& fov);
	virtual void		CalcObserverView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	virtual Vector		GetChaseCamViewOffset(CBaseEntity* target);
	void				CalcChaseCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	virtual void		CalcInEyeCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);

	virtual float		GetDeathCamInterpolationTime();

	virtual void		CalcDeathCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	void				CalcRoamingView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	virtual void		CalcFreezeCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);

	void DetermineVguiInputMode(CUserCmd* pCmd);

	virtual void SetLocalViewAngles(const QAngle& viewAngles);
	virtual void SetViewAngles(const QAngle& ang);

	surfacedata_t* GetGroundSurface();

	virtual void	FireGameEvent(IGameEvent* event);

protected:
	bool			JustEnteredVehicle();

	int				m_iObserverMode;	      
	EHANDLE			m_hObserverTarget;	   
	float			m_flObserverChaseDistance;      
	Vector			m_vecFreezeFrameStart;
	float			m_flFreezeFrameStartTime;	         
	float			m_flFreezeFrameDistance;
	bool			m_bWasFreezeFraming;
	float			m_flDeathTime;		    

	float			m_flStepSoundTime;
	bool			m_IsFootprintOnLeft;

private:
	C_BasePlayer& operator=(const C_BasePlayer& src);
	C_BasePlayer(const C_BasePlayer&);     

	EHANDLE			m_hVehicle;
	EHANDLE			m_hOldVehicle;
	EHANDLE			m_hUseEntity;

	float			m_flMaxspeed;

	int				m_iBonusProgress;
	int				m_iBonusChallenge;

	CInterpolatedVar< Vector >	m_iv_vecViewOffset;

	Vector			m_vecWaterJumpVel;
	float			m_flWaterJumpTime;       
	int				m_nImpulse;

	float			m_flSwimSoundTime;
	Vector			m_vecLadderNormal;

	QAngle			m_vecOldViewAngles;

	bool			m_bWasFrozen;
	int				m_flPhysics;

	int				m_nTickBase;
	int				m_nFinalPredictedTick;

	EHANDLE			m_pCurrentVguiScreen;

	bool			m_bFiredWeapon;


	CFlashlightEffect* m_pFlashlight;

	typedef CHandle<C_BaseCombatWeapon> CBaseCombatWeaponHandle;
	CNetworkVar(CBaseCombatWeaponHandle, m_hLastWeapon);

#if !defined( NO_ENTITY_PREDICTION )
	CUtlVector< CHandle< C_BaseEntity > > m_SimulatedByThisPlayer;
#endif

	CHandle< C_BaseViewModel >	m_hViewModel[MAX_VIEWMODELS];

	float					m_flOldPlayerZ;
	float					m_flOldPlayerViewOffsetZ;

	Vector	m_vecVehicleViewOrigin;		              
	QAngle	m_vecVehicleViewAngles;		  
	float	m_flVehicleViewFOV;
	int		m_nVehicleViewSavedFrame;	              

	int				m_iOldAmmo[MAX_AMMO_TYPES];

	C_CommandContext		m_CommandContext;

	float							m_flWaterSurfaceZ;
	bool							m_bResampleWaterSurface;
	TimedEvent						m_tWaterParticleTimer;
	CSmartPtr<WaterDebrisEffect>	m_pWaterEmitter;

	bool							m_bPlayerUnderwater;

	friend class CPrediction;

	friend class CTFGameMovementRecon;
	friend class CGameMovement;
	friend class CTFGameMovement;
	friend class CHL1GameMovement;
	friend class CCSGameMovement;
	friend class CHL2GameMovement;
	friend class CDODGameMovement;
	friend class CPortalGameMovement;

	float GetStepSize(void) const { return m_Local.m_flStepSize; }

	float m_flNextAvoidanceTime;
	float m_flAvoidanceRight;
	float m_flAvoidanceForward;
	float m_flAvoidanceDotForward;
	float m_flAvoidanceDotRight;

protected:
	virtual bool IsDucked(void) const { return m_Local.m_bDucked; }
	virtual bool IsDucking(void) const { return m_Local.m_bDucking; }
	virtual float GetFallVelocity(void) { return m_Local.m_flFallVelocity; }
	void ForceSetupBonesAtTimeFakeInterpolation(matrix3x4_t* pBonesOut, float curtimeOffset);

	float m_flLaggedMovementValue;

	Vector m_vecPredictionError;
	float m_flPredictionErrorTime;

	Vector m_vecPreviouslyPredictedOrigin;               

	char m_szLastPlaceName[MAX_PLACE_NAME_LENGTH];	    

	int				m_surfaceProps;
	surfacedata_t* m_pSurfaceData;
	float			m_surfaceFriction;
	char			m_chTextureType;

	bool			m_bSentFreezeFrame;
	float			m_flFreezeZOffset;

	float			m_flNextAchievementAnnounceTime;

	int				m_nForceVisionFilterFlags;         

#if defined USES_ECON_ITEMS
	CUtlVector<CHandle<C_EconWearable > >	m_hMyWearables;
#endif

private:

	struct StepSoundCache_t
	{
		StepSoundCache_t() : m_usSoundNameIndex(0) {}
		CSoundParameters	m_SoundParameters;
		unsigned short		m_usSoundNameIndex;
	};
	StepSoundCache_t		m_StepSoundCache[2];

public:

	const char* GetLastKnownPlaceName(void) const { return m_szLastPlaceName; }	         

	float GetLaggedMovementValue(void) { return m_flLaggedMovementValue; }
	bool  ShouldGoSouth(Vector vNPCForward, Vector vNPCRight);    

	void SetOldPlayerZ(float flOld) { m_flOldPlayerZ = flOld; }
};

EXTERN_RECV_TABLE(DT_BasePlayer);

inline C_BasePlayer* ToBasePlayer(C_BaseEntity* pEntity)
{
	if (!pEntity || !pEntity->IsPlayer())
		return NULL;

#if _DEBUG
	Assert(dynamic_cast<C_BasePlayer*>(pEntity) != NULL);
#endif

	return static_cast<C_BasePlayer*>(pEntity);
}

inline C_BaseEntity* C_BasePlayer::GetUseEntity()
{
	return m_hUseEntity;
}


inline IClientVehicle* C_BasePlayer::GetVehicle()
{
	C_BaseEntity* pVehicleEnt = m_hVehicle.Get();
	return pVehicleEnt ? pVehicleEnt->GetClientVehicle() : NULL;
}

inline bool C_BasePlayer::IsObserver() const
{
	return (GetObserverMode() != OBS_MODE_NONE);
}

inline int C_BasePlayer::GetImpulse(void) const
{
	return m_nImpulse;
}


inline C_CommandContext* C_BasePlayer::GetCommandContext()
{
	return &m_CommandContext;
}

inline int CBasePlayer::CurrentCommandNumber() const
{
	Assert(m_pCurrentCommand);
	return m_pCurrentCommand->command_number;
}

inline const CUserCmd* CBasePlayer::GetCurrentUserCommand() const
{
	Assert(m_pCurrentCommand);
	return m_pCurrentCommand;
}

#endif  