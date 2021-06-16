#ifndef CDLL_INT_H
#define CDLL_INT_H
#ifdef _WIN32
#pragma once
#endif

#include "basetypes.h"
#include "interface.h"
#include "mathlib.h"
#include "const.h"
#include "checksum_crc.h"
#include "datamap.h"
#include "bitbuf.h"
#include "ButtonCode.h"
#include "modes.h"

#if !defined( _X360 )
#include "xboxstubs.h"
#endif

#include "jobthread.h" 
#include "VGUI.h"

struct SpatializationInfo_t;
class IClientEntity;

class ClientClass;
struct model_t;
class CSentence;
struct vrect_t;
struct cmodel_t;
class IMaterial;
class CAudioSource;
class CMeasureSection;
class SurfInfo;
class ISpatialQuery;
struct cache_user_t;
class IMaterialSystem;
class VMatrix;
struct ScreenFade_t;
struct ScreenShake_t;
class CViewSetup;
class CEngineSprite;
class CGlobalVarsBase;
class CPhysCollide;
class CSaveRestoreData;
class INetChannelInfo;
struct datamap_t;
struct typedescription_t;
class CStandardRecvProxies;
struct client_textmessage_t;
class IAchievementMgr;
class CGamestatsData;
class KeyValues;
class IFileList;
class CRenamedRecvTableInfo;
class CMouthInfo;
class IConVar;

struct player_info_t
{
public:
	char __pad0[0x8];
	union {
		long long xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

struct AudioState_t
{
	Vector m_Origin;
	QAngle m_Angles;
	bool m_bIsUnderwater;
};


enum SkyboxVisibility_t
{
	SKYBOX_NOT_VISIBLE = 0,
	SKYBOX_3DSKYBOX_VISIBLE,
	SKYBOX_2DSKYBOX_VISIBLE,
};

struct SkyBoxMaterials_t
{
	IMaterial* material[6];
};

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			     
	FRAME_START,

	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,

	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

DECLARE_POINTER_HANDLE(LightCacheHandle_t);


struct OcclusionParams_t
{
	float	m_flMaxOccludeeArea;
	float	m_flMinOccluderArea;
};


#define VENGINE_CLIENT_RANDOM_INTERFACE_VERSION	"VEngineRandom001"

#define VENGINE_CLIENT_INTERFACE_VERSION		"VEngineClient014"
#define VENGINE_CLIENT_INTERFACE_VERSION_13		"VEngineClient013"

abstract_class IVEngineClient013
{
public:
	virtual int					GetIntersectingSurfaces(
									const model_t * model,
									const Vector & vCenter,
									const float radius,
									const bool bOnlyVisibleSurfaces,	      
									SurfInfo * pInfos,
									const int nMaxInfos) = 0;

	virtual Vector				GetLightForPoint(const Vector& pos, bool bClamp) = 0;

	virtual IMaterial* TraceLineMaterialAndLighting(const Vector& start, const Vector& end,
									Vector& diffuseLightColor, Vector& baseColor) = 0;

	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool				CopyLocalFile(const char* source, const char* destination) = 0;

	virtual void				GetScreenSize(int& width, int& height) = 0;

	virtual void				ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
	virtual void				ClientCmd(const char* szCmdString) = 0;

	virtual bool				GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;

	virtual int					GetPlayerForUserID(int userID) = 0;

	virtual client_textmessage_t* TextMessageGet(const char* pName) = 0;

	virtual bool				Con_IsVisible(void) = 0;

	virtual int					GetLocalPlayer(void) = 0;

	virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;

	virtual float				GetLastTimeStamp(void) = 0;

	virtual CSentence* GetSentence(CAudioSource* pAudioSource) = 0;
	virtual float				GetSentenceLength(CAudioSource* pAudioSource) = 0;
	virtual bool				IsStreaming(CAudioSource* pAudioSource) const = 0;

	virtual void				GetViewAngles(Vector& va) = 0;
	virtual void				SetViewAngles(QAngle& va) = 0;

	virtual int					GetMaxClients(void) = 0;

	virtual	const char* Key_LookupBinding(const char* pBinding) = 0;

	virtual const char* Key_BindingForKey(ButtonCode_t code) = 0;

	virtual	void				Key_SetBinding(ButtonCode_t code, const char* pBinding) = 0; 

	virtual void				StartKeyTrapMode(void) = 0;
	virtual bool				CheckDoneKeyTrapping(ButtonCode_t& code) = 0;

	virtual bool				IsInGame(void) = 0;
	virtual bool				IsConnected(void) = 0;
	virtual bool				IsDrawingLoadingImage(void) = 0;

	virtual void				HideLoadingPlaque(void) = 0; 

	virtual void				Con_NPrintf(int pos, PRINTF_FORMAT_STRING const char* fmt, ...) = 0;
	virtual void				Con_NXPrintf(const struct con_nprint_s* info, PRINTF_FORMAT_STRING const char* fmt, ...) = 0;

	virtual int					IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;

	virtual int					IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;

	virtual bool				CullBox(const Vector& mins, const Vector& maxs) = 0;

	virtual void				Sound_ExtraUpdate(void) = 0;

#if defined(_PS3)
	virtual void				Sound_ServerUpdateSoundsPS3(void) = 0; 
#endif

	virtual const char* GetGameDirectory(void) = 0;

	virtual const VMatrix& WorldToScreenMatrix() = 0;

	virtual const VMatrix& WorldToViewMatrix() = 0;

	virtual int					GameLumpVersion(int lumpId) const = 0;
	virtual int					GameLumpSize(int lumpId) const = 0;
	virtual bool				LoadGameLump(int lumpId, void* pBuffer, int size) = 0;

	virtual int					LevelLeafCount() const = 0;

	virtual ISpatialQuery* GetBSPTreeQuery() = 0;

	virtual void		LinearToGamma(float* linear, float* gamma) = 0;

	virtual float		LightStyleValue(int style) = 0;

	virtual void		ComputeDynamicLighting(const Vector& pt, const Vector* pNormal, Vector& color) = 0;

	virtual void		GetAmbientLightColor(Vector& color) = 0;

	virtual int			GetDXSupportLevel() = 0;

	virtual bool        SupportsHDR() = 0;

	virtual void		Mat_Stub(IMaterialSystem* pMatSys) = 0;

	virtual void GetChapterName(char* pchBuff, int iMaxLength) = 0;
	virtual char const* GetLevelName(void) = 0;
	virtual char const* GetLevelNameShort(void) = 0; 
	virtual char const* GetMapGroupName(void) = 0; 
#if !defined( NO_VOICE )
	virtual struct IVoiceTweak_s* GetVoiceTweakAPI(void) = 0;
	virtual void SetVoiceCasterID(uint32 casterID) = 0; 
#endif
	virtual void		EngineStats_BeginFrame(void) = 0;
	virtual void		EngineStats_EndFrame(void) = 0;

	virtual void		FireEvents() = 0;

	virtual int			GetLeavesArea(int* pLeaves, int nLeaves) = 0;

	virtual bool		DoesBoxTouchAreaFrustum(const Vector& mins, const Vector& maxs, int iArea) = 0;
	virtual int			GetFrustumList(Frustum_t** pList, int listMax) = 0; 
	virtual bool		ShouldUseAreaFrustum(int area) = 0; 

	virtual void		SetAudioState(const AudioState_t& state) = 0;

	virtual int			SentenceGroupPick(int groupIndex, char* name, int nameBufLen) = 0;
	virtual int			SentenceGroupPickSequential(int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset) = 0;
	virtual int			SentenceIndexFromName(const char* pSentenceName) = 0;
	virtual const char* SentenceNameFromIndex(int sentenceIndex) = 0;
	virtual int			SentenceGroupIndexFromName(const char* pGroupName) = 0;
	virtual const char* SentenceGroupNameFromIndex(int groupIndex) = 0;
	virtual float		SentenceLength(int sentenceIndex) = 0;

	virtual void		ComputeLighting(const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector* pBoxColors = NULL) = 0;

	virtual void		ActivateOccluder(int nOccluderIndex, bool bActive) = 0;
	virtual bool		IsOccluded(const Vector& vecAbsMins, const Vector& vecAbsMaxs) = 0;
	virtual int			GetOcclusionViewId() const = 0; 

	virtual void* SaveAllocMemory(size_t num, size_t size) = 0;
	virtual void		SaveFreeMemory(void* pSaveMem) = 0;

	virtual INetChannelInfo* GetNetChannelInfo(void) = 0;

	virtual void		DebugDrawPhysCollide(const CPhysCollide* pCollide, IMaterial* pMaterial, matrix3x4_t& transform, const color32& color) = 0;
	virtual void		CheckPoint(const char* pName) = 0;
	virtual void		DrawPortals() = 0;
	virtual bool		IsPlayingDemo(void) = 0;
	virtual bool		IsRecordingDemo(void) = 0;
	virtual bool		IsPlayingTimeDemo(void) = 0;
	virtual int			GetDemoRecordingTick(void) = 0;
	virtual int			GetDemoPlaybackTick(void) = 0;
	virtual int			GetDemoPlaybackStartTick(void) = 0;
	virtual float		GetDemoPlaybackTimeScale(void) = 0;
	virtual int			GetDemoPlaybackTotalTicks(void) = 0;
	virtual bool		IsPaused(void) = 0;
	virtual float GetTimescale(void) const = 0; 
	virtual bool		IsTakingScreenshot(void) = 0;
	virtual bool		IsHLTV(void) = 0;
	virtual bool		IsLevelMainMenuBackground(void) = 0;
	virtual void		GetMainMenuBackgroundName(char* dest, int destlen) = 0;

	virtual void		SetOcclusionParameters(const OcclusionParams_t& params) = 0;

	virtual void		GetUILanguage(char* dest, int destlen) = 0;

	virtual SkyboxVisibility_t IsSkyboxVisibleFromPoint(const Vector& vecPoint) = 0;

	virtual const char* GetMapEntitiesString() = 0;

	virtual bool		IsInEditMode(void) = 0;

	virtual float		GetScreenAspectRatio() = 0;

	virtual bool		REMOVED_SteamRefreshLogin(const char* password, bool isSecure) = 0;
	virtual bool		REMOVED_SteamProcessCall(bool& finished) = 0;

	virtual unsigned int	GetEngineBuildNumber() = 0;   
	virtual const char* GetProductVersionString() = 0;     

	virtual void			GrabPreColorCorrectedFrame(int x, int y, int width, int height) = 0;

	virtual bool			IsHammerRunning() const = 0;

	virtual void			ExecuteClientCmd(const char* szCmdString) = 0;

	virtual bool MapHasHDRLighting(void) = 0;
	virtual bool MapHasLightMapAlphaData(void) = 0; 

	virtual int	GetAppID() = 0;

	virtual Vector			GetLightForPointFast(const Vector& pos, bool bClamp) = 0;

	virtual void			ClientCmd_Unrestricted(const char* szCmdString, bool fromConsoleOrKeybind = false) = 0;
	virtual void			ClientCmd_Unrestricted(const char* szCmdString, bool fromConsoleOrKeybind, int nUserSlot, bool bCheckValidSlot = true) = 0; 
	 
	virtual void			SetRestrictServerCommands(bool bRestrict) = 0;

	virtual void			SetRestrictClientCommands(bool bRestrict) = 0;

	virtual void			SetOverlayBindProxy(int iOverlayID, void* pBindProxy) = 0;

	virtual bool			CopyFrameBufferToMaterial(const char* pMaterialName) = 0;

	virtual void			ChangeTeam(const char* pTeamName) = 0;

	virtual void			ReadConfiguration(const bool readDefault = false) = 0;

	virtual void SetAchievementMgr(IAchievementMgr* pAchievementMgr) = 0;
	virtual IAchievementMgr* GetAchievementMgr() = 0;

	virtual bool			MapLoadFailed(void) = 0;
	virtual void			SetMapLoadFailed(bool bState) = 0;

	virtual bool			IsLowViolence() = 0;
	virtual const char* GetMostRecentSaveGame(void) = 0;
	virtual void			SetMostRecentSaveGame(const char* lpszFilename) = 0;

	virtual void			StartXboxExitingProcess() = 0;
	virtual bool			IsSaveInProgress() = 0;
	virtual uint			OnStorageDeviceAttached(void) = 0;
	virtual void			OnStorageDeviceDetached(void) = 0;

	virtual void			ResetDemoInterpolation(void) = 0;

	virtual void SetGamestatsData(CGamestatsData* pGamestatsData) = 0;
	virtual CGamestatsData* GetGamestatsData() = 0;

#if defined( USE_SDL )
	virtual void GetMouseDelta(int& x, int& y, bool bIgnoreNextMouseDelta = false) = 0;
#endif

	virtual void ServerCmdKeyValues(KeyValues* pKeyValues) = 0;

	virtual bool IsSkippingPlayback(void) = 0;
	virtual bool IsLoadingDemo(void) = 0;

	virtual bool IsPlayingDemoALocallyRecordedDemo() = 0;

	virtual	const char* Key_LookupBindingExact(const char* pBinding) = 0;

	virtual void				AddPhonemeFile(const char* pszPhonemeFile) = 0;

};

abstract_class IVEngineClient : public IVEngineClient013
{
public:
	virtual uint GetProtocolVersion() = 0;
	virtual bool IsWindowedMode() = 0;

	virtual void	FlashWindow() = 0;

	virtual int GetClientVersion() const = 0;   

	virtual bool IsActiveApp() = 0;

	virtual void DisconnectInternal() = 0;

	virtual int GetInstancesRunningCount() = 0;
};


abstract_class IBaseClientDLL
{
public:
	virtual int				Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase * pGlobals) = 0;
	virtual void            Disconnect() = 0;

	virtual int				Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;

	virtual void			PostInit() = 0;

	virtual void			Shutdown(void) = 0;

	virtual void			LevelInitPreEntity(char const* pMapName) = 0;
	virtual void			LevelInitPostEntity() = 0;
	virtual void			LevelShutdown(void) = 0;

	virtual ClientClass* GetAllClasses(void) = 0;

	virtual int				HudVidInit(void) = 0;
	virtual void			HudProcessInput(bool bActive) = 0;
	virtual void			HudUpdate(bool bActive) = 0;
	virtual void			HudReset(void) = 0;
	virtual void			HudText(const char* message) = 0;

	virtual bool			ShouldDrawDropdownConsole() = 0;

	virtual void			IN_ActivateMouse(void) = 0;
	virtual void			IN_DeactivateMouse(void) = 0;
	virtual void			IN_Accumulate(void) = 0;
	virtual void			IN_ClearStates(void) = 0;
	virtual bool			IN_IsKeyDown(const char* name, bool& isdown) = 0;
	virtual int				IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;

	virtual void			CreateMove(
								int sequence_number,			    
								float input_sample_frametime,	     
								bool active) = 0;				        

	virtual void			ExtraMouseSample(float frametime, bool active) = 0;

	virtual bool			WriteUsercmdDeltaToBuffer(int nSlot, bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual void			EncodeUserCmdToBuffer(int nSlot, bf_write& buf, int slot) = 0;
	virtual void			DecodeUserCmdFromBuffer(int nSlot, bf_read& buf, int slot) = 0;

	virtual void			View_Render(vrect_t* rect) = 0;

	virtual void			RenderView(const CViewSetup& view, int nClearFlags, int whatToDraw) = 0;

	virtual void			View_Fade(ScreenFade_t* pSF) = 0;

	virtual void			SetCrosshairAngle(const QAngle& angle) = 0;

	virtual void			InitSprite(CEngineSprite* pSprite, const char* loadname) = 0;
	virtual void			ShutdownSprite(CEngineSprite* pSprite) = 0;
	virtual int				GetSpriteSize(void) const = 0;

	virtual void			VoiceStatus(int entindex, int iSsSlot, qboolean bTalking) = 0;

	virtual bool			PlayerAudible(int iPlayerIndex) = 0;

	virtual void			InstallStringTableCallback(char const* tableName) = 0;

	virtual void			FrameStageNotify(ClientFrameStage_t curStage) = 0; 

	virtual bool			DispatchUserMessage(int msg_type, int32 nFlags, int size, const void* msg) = 0;	   

	virtual CSaveRestoreData* SaveInit(int size) = 0;
	virtual void			SaveWriteFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			SaveReadFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			PreSave(CSaveRestoreData*) = 0;
	virtual void			Save(CSaveRestoreData*) = 0;
	virtual void			WriteSaveHeaders(CSaveRestoreData*) = 0;
	virtual void			ReadRestoreHeaders(CSaveRestoreData*) = 0;
	virtual void			Restore(CSaveRestoreData*, bool) = 0;
	virtual void			DispatchOnRestore() = 0;

	virtual CStandardRecvProxies* GetStandardRecvProxies() = 0;

	virtual void			WriteSaveGameScreenshot(const char* pFilename) = 0;

	virtual void			EmitSentenceCloseCaption(char const* tokenstream) = 0;
	virtual void			EmitCloseCaption(char const* captionname, float duration) = 0;

	virtual bool			CanRecordDemo(char* errorMsg, int length) const = 0;

	virtual void			OnDemoRecordStart(char const* pDemoBaseName) = 0;
	virtual void			OnDemoRecordStop() = 0;
	virtual void			OnDemoPlaybackStart(char const* pDemoBaseName) = 0;
	virtual void			OnDemoPlaybackRestart() = 0;
	virtual void			OnDemoPlaybackStop() = 0;
	virtual void			SetDemoPlaybackHighlightXuid(uint64 xuid, bool bLowlights) = 0;
	virtual void			ShowHighlightSkippingMessage(bool bState, int nCurrentTick = 0, int nTickStart = 0, int nTickStop = 0) = 0;

	virtual void			RecordDemoPolishUserInput(int nCmdIndex) = 0;

	virtual bool			CacheReplayRagdolls(const char* pFilename, int nStartTick) = 0;

	virtual void			ReplayUI_SendMessage(KeyValues* pMsg) = 0;

	virtual void* GetReplayFactory() = 0;

	virtual void			ClearLocalPlayerReplayPtr() = 0;

	virtual void unk();

	virtual int				GetScreenWidth() = 0;
	virtual int				GetScreenHeight() = 0;

	virtual void			WriteSaveGameScreenshotOfSize(const char* pFilename, int width, int height, bool bCreatePowerOf2Padded = false, bool bWriteVTF = false) = 0;

	virtual void			WriteReplayScreenshot(void* params) = 0;

	virtual void			UpdateReplayScreenshotCache() = 0;
	  
	virtual bool			GetPlayerView(CViewSetup& playerView) = 0;

	virtual bool			ShouldHideLoadingPlaque(void) = 0;

	virtual void			InvalidateMdlCache() = 0;

	virtual void			IN_SetSampleTime(float frametime) = 0;

	virtual void			OnActiveSplitscreenPlayerChanged(int nNewSlot) = 0;
	virtual void			OnSplitScreenStateChanged() = 0;

	virtual int				GetSpectatorTarget(void* pObserverMode) = 0;

	virtual void			CenterStringOff() = 0;

	virtual void			OnScreenSizeChanged(int nOldWidth, int nOldHeight) = 0;

	virtual void* InstantiateMaterialProxy(const char* proxyName) = 0;

	virtual vgui::VPANEL	GetFullscreenClientDLLVPanel(void) = 0;

	virtual void			MarkEntitiesAsTouching(IClientEntity* e1, IClientEntity* e2) = 0;

	virtual void			OnKeyBindingChanged(ButtonCode_t buttonCode, char const* pchKeyName, char const* pchNewBinding) = 0;

	virtual void			SetBlurFade(float scale) = 0;

	virtual void			ResetHudCloseCaption() = 0;

	virtual bool			HandleGameUIEvent(const InputEvent_t& event) = 0;

	virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;

	virtual void			Hud_SaveStarted() = 0;

	virtual void			ShutdownMovies() = 0;

	virtual void			GetStatus(char* buffer, int bufsize) = 0;

#if defined ( CSTRIKE15 )
	virtual bool			IsChatRaised(void) = 0;
	virtual bool			IsRadioPanelRaised(void) = 0;
	virtual bool			IsBindMenuRaised(void) = 0;
	virtual bool			IsTeamMenuRaised(void) = 0;
	virtual bool			IsLoadingScreenRaised(void) = 0;

#endif

#if defined(_PS3)
	virtual int				GetDrawFlags(void) = 0;
	virtual int				GetBuildViewID(void) = 0;
	virtual bool			IsSPUBuildWRJobsOn(void) = 0;
	virtual void			CacheFrustumData(Frustum_t* pFrustum, Frustum_t* pAreaFrustum, void* pRenderAreaBits, int numArea, bool bViewerInSolidSpace) = 0;
	virtual void* GetBuildViewVolumeCuller(void) = 0;
	virtual Frustum_t* GetBuildViewFrustum(void) = 0;
	virtual Frustum_t* GetBuildViewAreaFrustum(void) = 0;
	virtual unsigned char* GetBuildViewRenderAreaBits(void) = 0;
#else
	virtual bool			IsBuildWRThreaded(void) = 0;
	virtual void			QueueBuildWorldListJob(CJob* pJob) = 0;
	virtual void			CacheFrustumData(const Frustum_t& frustum, const CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t,16 > >& aeraFrustums) = 0;
	virtual const Frustum_t* GetBuildViewFrustum(void) const = 0;
	virtual const CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t,16 > >* GetBuildViewAeraFrustums(void) const = 0;
#endif

	virtual bool IsSubscribedMap(const char* pchMapName, bool bOnlyOnDisk) = 0;
	virtual bool IsFeaturedMap(const char* pchMapName, bool bOnlyOnDisk) = 0;

	virtual void DownloadCommunityMapFile(int id) = 0;

	virtual float GetUGCFileDownloadProgress(int id) = 0;

	virtual void RecordUIEvent(const char* szEvent) = 0;

	virtual float FindInventoryItemWithMaxAttributeValue(char const* szItemType, char const* szAttrClass) = 0;

	virtual void DetermineSubscriptionKvToAdvertise(KeyValues* kvLocalPlayer) = 0;

	virtual bool ValidateSignedEvidenceHeader(char const* szKey, void const* pvHeader, void* pPlaybackParameters) = 0;
	virtual void PrepareSignedEvidenceData(void* pvData, int numBytes, int const* pPlaybackParameters) = 0;
	virtual bool ShouldSkipEvidencePlayback(int const* pPlaybackParameters) = 0;

	virtual void* GetScaleformSlotInitController() = 0;

	virtual bool IsConnectedUserInfoChangeAllowed(IConVar* pCvar) = 0;

	virtual char const* GetRichPresenceStatusString() = 0;

	virtual bool			CanStopRecordDemo(char* errorMsg, int length) const = 0;

	virtual void OnHltvReplay(const int& msg) = 0;
	virtual void OnHltvReplayTick() = 0;
	virtual int GetHltvReplayDelay() = 0;

	virtual void OnDemoPlaybackTimeJump() = 0;
	virtual void OnCommandDuringPlayback(char const* cmd) = 0;

	virtual void RetireAllPlayerDecals(bool bRenderContextValid) = 0;
	virtual void EngineGotvSyncPacket(const void* pPkt) = 0;        
	virtual int GetInEyeEntity() const = 0;
	virtual void OnTickPre(int tickcount) = 0;
};

#define CLIENT_DLL_INTERFACE_VERSION		"VClient018"

abstract_class IClientDLLSharedAppSystems
{
public:
	virtual int	Count() = 0;
	virtual char const* GetDllName(int idx) = 0;
	virtual char const* GetInterfaceName(int idx) = 0;
};

#define CLIENT_DLL_SHARED_APPSYSTEMS		"VClientDllSharedAppSystems001"

#endif  