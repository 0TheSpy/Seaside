#ifndef EIFACE_H
#define EIFACE_H

#ifdef _WIN32
#pragma once
#endif

#include "convar.h"
#include "icvar.h"
#include "edict.h"
#include "vplane.h"
#include "iserverentity.h"
#include "ivmodelinfo.h"
#include "soundflags.h"
#include "bitvec.h"
#include "iserverplugin.h"
#include "bitbuf.h"
#include "checksum_crc.h"
#include "iclient.h"

class	SendTable;
class	ServerClass;
class	IMoveHelper;
struct  Ray_t;
class	CGameTrace;
typedef	CGameTrace trace_t;
struct	typedescription_t;
class	CSaveRestoreData;
struct	datamap_t;
class	SendTable;
class	ServerClass;
class	IMoveHelper;
struct  Ray_t;
struct	studiohdr_t;
class	CBaseEntity;
class	CRestore;
class	CSave;
class	variant_t;
struct	vcollide_t;
class	IRecipientFilter;
class	CBaseEntity;
class	ITraceFilter;
struct	client_textmessage_t;
class	INetChannelInfo;
class	ISpatialPartition;
class IScratchPad3D;
class CStandardSendProxies;
class IAchievementMgr;
class CGamestatsData;
class CSteamID;
class ISPSharedMemory;
class CGamestatsData;
class CEngineHltvInfo_t;
class INetMessage;
class HltvReplayParams_t;

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

typedef struct player_info_s player_info_t;

typedef uint64 PublishedFileId_t;

#ifdef _WIN32
#define DLLEXPORT __stdcall
#else
#define DLLEXPORT  
#endif

#define INTERFACEVERSION_VENGINESERVER	"VEngineServer023"

struct bbox_t
{
	Vector mins;
	Vector maxs;
};

abstract_class IVEngineServer
{
public:
	virtual void		ChangeLevel(const char* s1, const char* s2) = 0;

	virtual int			IsMapValid(const char* filename) = 0;

	virtual bool		IsDedicatedServer(void) = 0;

	virtual int			IsInEditMode(void) = 0;

	virtual KeyValues* GetLaunchOptions(void) = 0;

	virtual int			PrecacheModel(const char* s, bool preload = false) = 0;
	virtual int			PrecacheSentenceFile(const char* s, bool preload = false) = 0;
	virtual int			PrecacheDecal(const char* name, bool preload = false) = 0;
	virtual int			PrecacheGeneric(const char* s, bool preload = false) = 0;

	virtual bool		IsModelPrecached(char const* s) const = 0;
	virtual bool		IsDecalPrecached(char const* s) const = 0;
	virtual bool		IsGenericPrecached(char const* s) const = 0;

	virtual int			GetClusterForOrigin(const Vector& org) = 0;
	virtual int			GetPVSForCluster(int cluster, int outputpvslength, unsigned char* outputpvs) = 0;
	virtual bool		CheckOriginInPVS(const Vector& org, const unsigned char* checkpvs, int checkpvssize) = 0;
	virtual bool		CheckBoxInPVS(const Vector& mins, const Vector& maxs, const unsigned char* checkpvs, int checkpvssize) = 0;

	virtual int			GetPlayerUserId(const edict_t* e) = 0;
	virtual const char* GetPlayerNetworkIDString(const edict_t* e) = 0;
	virtual bool		IsUserIDInUse(int userID) = 0;	    
	virtual int			GetLoadingProgressForUserID(int userID) = 0;	    

	virtual int			GetEntityCount(void) = 0;

	virtual INetChannelInfo* GetPlayerNetInfo(int playerIndex) = 0;

	virtual edict_t* CreateEdict(int iForceEdictIndex = -1) = 0;
	virtual void		RemoveEdict(edict_t* e) = 0;

	virtual void* PvAllocEntPrivateData(long cb) = 0;
	virtual void		FreeEntPrivateData(void* pEntity) = 0;

	virtual void* SaveAllocMemory(size_t num, size_t size) = 0;
	virtual void		SaveFreeMemory(void* pSaveMem) = 0;

	virtual void		EmitAmbientSound(int entindex, const Vector& pos, const char* samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch, float delay = 0.0f) = 0;

	virtual void        FadeClientVolume(const edict_t* pEdict, float fadePercent, float fadeOutSeconds, float holdTime, float fadeInSeconds) = 0;

	virtual int			SentenceGroupPick(int groupIndex, char* name, int nameBufLen) = 0;
	virtual int			SentenceGroupPickSequential(int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset) = 0;
	virtual int			SentenceIndexFromName(const char* pSentenceName) = 0;
	virtual const char* SentenceNameFromIndex(int sentenceIndex) = 0;
	virtual int			SentenceGroupIndexFromName(const char* pGroupName) = 0;
	virtual const char* SentenceGroupNameFromIndex(int groupIndex) = 0;
	virtual float		SentenceLength(int sentenceIndex) = 0;

	virtual void		ServerCommand(const char* str) = 0;
	virtual void		ServerExecute(void) = 0;
	virtual void		ClientCommand(edict_t* pEdict, const char* szFmt, ...) FMTFUNCTION(3, 4) = 0;

	virtual void		LightStyle(int style, const char* val) = 0;

	virtual void		StaticDecal(const Vector& originInEntitySpace, int decalIndex, int entityIndex, int modelIndex, bool lowpriority) = 0;

	virtual void		Message_DetermineMulticastRecipients(bool usepas, const Vector& origin, CPlayerBitVec& playerbits) = 0;

	virtual bf_write* EntityMessageBegin(int ent_index, ServerClass* ent_class, bool reliable) = 0;

	virtual void		MessageEnd(void) = 0;

	virtual void		SendUserMessage(IRecipientFilter& filter, int message, const google::protobuf::Message& msg) = 0;

	virtual void		ClientPrintf(edict_t* pEdict, const char* szMsg) = 0;

	virtual void		Con_NPrintf(int pos, const char* fmt, ...) = 0;
	virtual void		Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;

	virtual void		SetView(const edict_t* pClient, const edict_t* pViewent) = 0;

	virtual void		CrosshairAngle(const edict_t* pClient, float pitch, float yaw) = 0;

	virtual void        GetGameDir(char* szGetGameDir, int maxlength) = 0;

	virtual int 		CompareFileTime(const char* filename1, const char* filename2, int* iCompare) = 0;

	virtual bool		LockNetworkStringTables(bool lock) = 0;

	virtual edict_t* CreateFakeClient(const char* netname) = 0;

	virtual const char* GetClientConVarValue(int clientIndex, const char* name) = 0;

	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool		CopyFile(const char* source, const char* destination) = 0;

	virtual void		ResetPVS(byte* pvs, int pvssize) = 0;
	virtual void		AddOriginToPVS(const Vector& origin) = 0;

	virtual void		SetAreaPortalState(int portalNumber, int isOpen) = 0;

	virtual void		PlaybackTempEntity(IRecipientFilter& filter, float delay, const void* pSender, const SendTable* pST, int classID) = 0;
	virtual int			CheckHeadnodeVisible(int nodenum, const byte* pvs, int vissize) = 0;
	virtual int			CheckAreasConnected(int area1, int area2) = 0;
	virtual int			GetArea(const Vector& origin) = 0;
	virtual void		GetAreaBits(int area, unsigned char* bits, int buflen) = 0;
	virtual bool		GetAreaPortalPlane(Vector const& vViewOrigin, int portalKey, VPlane* pPlane) = 0;

	virtual bool		LoadGameState(char const* pMapName, bool createPlayers) = 0;
	virtual void		LoadAdjacentEnts(const char* pOldLevel, const char* pLandmarkName) = 0;
	virtual void		ClearSaveDir() = 0;

	virtual const char* GetMapEntitiesString() = 0;

	virtual client_textmessage_t* TextMessageGet(const char* pName) = 0;

	virtual void		LogPrint(const char* msg) = 0;
	virtual bool		IsLogEnabled() = 0;
	virtual void		BuildEntityClusterList(edict_t* pEdict, PVSInfo_t* pPVSInfo) = 0;

	virtual void SolidMoved(edict_t* pSolidEnt, ICollideable* pSolidCollide, const Vector* pPrevAbsOrigin, bool testSurroundingBoundsOnly) = 0;
	virtual void TriggerMoved(edict_t* pTriggerEnt, bool testSurroundingBoundsOnly) = 0;

	virtual ISpatialPartition* CreateSpatialPartition(const Vector& worldmin, const Vector& worldmax) = 0;
	virtual void 		DestroySpatialPartition(ISpatialPartition*) = 0;

	virtual void		DrawMapToScratchPad(IScratchPad3D* pPad, unsigned long iFlags) = 0;

	virtual const CBitVec<MAX_EDICTS>* GetEntityTransmitBitsForClient(int iClientIndex) = 0;

	virtual bool		IsPaused() = 0;

	virtual float GetTimescale(void) const = 0;

	virtual void		ForceExactFile(const char* s) = 0;
	virtual void		ForceModelBounds(const char* s, const Vector& mins, const Vector& maxs) = 0;
	virtual void		ClearSaveDirAfterClientLoad() = 0;

	virtual void		SetFakeClientConVarValue(edict_t* pEntity, const char* cvar, const char* value) = 0;

	virtual void		ForceSimpleMaterial(const char* s) = 0;

	virtual int			IsInCommentaryMode(void) = 0;

	virtual bool		IsLevelMainMenuBackground(void) = 0;

	virtual void		SetAreaPortalStates(const int* portalNumbers, const int* isOpen, int nPortals) = 0;

	virtual void		NotifyEdictFlagsChange(int iEdict) = 0;

	virtual const CCheckTransmitInfo* GetPrevCheckTransmitInfo(edict_t* pPlayerEdict) = 0;

	virtual CSharedEdictChangeInfo* GetSharedEdictChangeInfo() = 0;

	virtual void			AllowImmediateEdictReuse() = 0;

	virtual bool		IsInternalBuild(void) = 0;

	virtual IChangeInfoAccessor* GetChangeAccessor(const edict_t* pEdict) = 0;

	virtual char const* GetMostRecentlyLoadedFileName() = 0;
	virtual char const* GetSaveFileName() = 0;

	virtual void CleanUpEntityClusterList(PVSInfo_t* pPVSInfo) = 0;

	virtual void SetAchievementMgr(IAchievementMgr*) = 0;
	virtual IAchievementMgr* GetAchievementMgr() = 0;

	virtual int	GetAppID() = 0;

	virtual bool IsLowViolence() = 0;

	virtual bool IsAnyClientLowViolence() = 0;

	virtual QueryCvarCookie_t StartQueryCvarValue(edict_t* pPlayerEntity, const char* pName) = 0;

	virtual void InsertServerCommand(const char* str) = 0;

	virtual bool GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;

	virtual bool IsClientFullyAuthenticated(edict_t* pEdict) = 0;

	virtual void SetDedicatedServerBenchmarkMode(bool bBenchmarkMode) = 0;

	virtual bool IsSplitScreenPlayer(int ent_num) = 0;
	virtual edict_t* GetSplitScreenPlayerAttachToEdict(int ent_num) = 0;
	virtual int	GetNumSplitScreenUsersAttachedToEdict(int ent_num) = 0;
	virtual edict_t* GetSplitScreenPlayerForEdict(int ent_num, int nSlot) = 0;

	virtual bool IsOverrideLoadGameEntsOn() = 0;

	virtual void ForceFlushEntity(int iEntity) = 0;

	virtual ISPSharedMemory* GetSinglePlayerSharedMemorySpace(const char* szName, int ent_num = MAX_EDICTS) = 0;

	virtual void* AllocLevelStaticData(size_t bytes) = 0;

	virtual int GetClusterCount() = 0;
	virtual int GetAllClusterBounds(bbox_t* pBBoxList, int maxBBox) = 0;

	virtual bool IsCreatingReslist() = 0;
	virtual bool IsCreatingXboxReslist() = 0;
	virtual bool IsDedicatedServerForXbox() = 0;
	virtual bool IsDedicatedServerForPS3() = 0;

	virtual void Pause(bool bPause, bool bForce = false) = 0;

	virtual void SetTimescale(float flTimescale) = 0;

	virtual void SetGamestatsData(CGamestatsData* pGamestatsData) = 0;
	virtual CGamestatsData* GetGamestatsData() = 0;

	virtual const CSteamID* GetClientSteamID(const edict_t* pPlayerEdict, bool bValidatedIDOnly = false) = 0;

	virtual const CSteamID* GetGameServerSteamID() = 0;

	virtual void HostValidateSession() = 0;

	virtual void RefreshScreenIfNecessary() = 0;

	virtual bool HasPaintmap() = 0;

	virtual bool SpherePaintSurface(const model_t* pModel, const Vector&, unsigned char, float, float) = 0;

	virtual void SphereTracePaintSurface(const model_t* pModel, const Vector&, const Vector&, float, CUtlVector<unsigned char>&) = 0;

	virtual void RemoveAllPaint() = 0;

	virtual void PaintAllSurfaces(unsigned char) = 0;
	virtual void RemovePaint(const model_t* pModel) = 0;

	virtual void ClientCommandKeyValues(edict_t* pEdict, KeyValues* pCommand) = 0;

	virtual uint64 GetClientXUID(edict_t* pPlayerEdict) = 0;
	virtual bool IsActiveApp() = 0;

	virtual void SetNoClipEnabled(bool bEnabled) = 0;

	virtual void GetPaintmapDataRLE(CUtlVector<unsigned int>& mapdata) = 0;
	virtual void LoadPaintmapDataRLE(CUtlVector<unsigned int>& mapdata) = 0;
	virtual void SendPaintmapDataToClient(edict_t* pEdict) = 0;

	virtual float GetLatencyForChoreoSounds() = 0;

	virtual CrossPlayPlatform_t GetClientCrossPlayPlatform(int ent_num) = 0;

	virtual void EnsureInstanceBaseline(int ent_num) = 0;

	virtual bool ReserveServerForQueuedGame(const char* szReservationPayload) = 0;

	virtual bool GetEngineHltvInfo(CEngineHltvInfo_t& out) = 0;

	virtual void AddHltvRelayProxyWhitelist(uint32 octet1, uint32 octet2, uint32 octet3, uint32 octet4, uint32 prefix) = 0;

	virtual int GetServerVersion() const = 0;

	virtual void UpdateHltvExternalViewers(uint32 totalSpectators, uint32 spectatorsLinkedToSteam) = 0;

	virtual bool WasShutDownRequested(void) const = 0;
	virtual void* StartClientHltvReplay(int client , const HltvReplayParams_t&) = 0;
	virtual void* StopClientHltvReplay(int client) = 0;
	virtual int GetClientHltvReplayDelay(int client) = 0;
	virtual bool HasHltvReplay(void) = 0;
	virtual bool ClientCanStartHltvReplay(int client) = 0;
	virtual int ClientResetReplayRequestTime(int client) = 0;
	virtual bool AnyClientsInHltvReplayMode(void) = 0;
};

#define INTERFACEVERSION_SERVERGAMEDLL				"ServerGameDLL005"

abstract_class IServerGameDLL
{
public:
	virtual bool			DLLInit(CreateInterfaceFn engineFactory,
										CreateInterfaceFn physicsFactory,
										CreateInterfaceFn fileSystemFactory,
										CGlobalVars * pGlobals) = 0;

	virtual bool			GameInit(void) = 0;

	virtual bool			LevelInit(char const* pMapName,
									char const* pMapEntities, char const* pOldLevel,
									char const* pLandmarkName, bool loadGame, bool background) = 0;

	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) = 0;

	virtual void			GameFrame(bool simulating) = 0;

	virtual void			PreClientUpdate(bool simulating) = 0;

	virtual void			LevelShutdown(void) = 0;
	virtual void			GameShutdown(void) = 0;

	virtual void			DLLShutdown(void) = 0;

	virtual float			GetTickInterval(void) const = 0;

	virtual ServerClass* GetAllServerClasses(void) = 0;

	virtual const char* GetGameDescription(void) = 0;

	virtual void			CreateNetworkStringTables(void) = 0;

	virtual CSaveRestoreData* SaveInit(int size) = 0;
	virtual void			SaveWriteFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			SaveReadFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			SaveGlobalState(CSaveRestoreData*) = 0;
	virtual void			RestoreGlobalState(CSaveRestoreData*) = 0;
	virtual void			PreSave(CSaveRestoreData*) = 0;
	virtual void			Save(CSaveRestoreData*) = 0;
	virtual void			GetSaveComment(char* comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime = false) = 0;
	virtual void			WriteSaveHeaders(CSaveRestoreData*) = 0;
	virtual void			ReadRestoreHeaders(CSaveRestoreData*) = 0;
	virtual void			Restore(CSaveRestoreData*, bool) = 0;
	virtual bool			IsRestoring() = 0;
	virtual bool			SupportsSaveRestore() = 0;

	virtual int				CreateEntityTransitionList(CSaveRestoreData*, int) = 0;
	virtual void			BuildAdjacentMapList(void) = 0;

	virtual CStandardSendProxies* GetStandardSendProxies() = 0;

	virtual void			PostInit() = 0;
	virtual void			Think(bool finalTick) = 0;

#ifdef _XBOX
	virtual void			GetTitleName(const char* pMapName, char* pTitleBuff, int titleBuffSize) = 0;
#endif

	virtual void			PreSaveGameLoaded(char const* pSaveName, bool bCurrentlyInGame) = 0;

	virtual bool			ShouldHideServer(void) = 0;

	virtual void			InvalidateMdlCache() = 0;

	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) = 0;

	virtual void			PostToolsInit() = 0;

	virtual void			GameServerSteamAPIActivated(bool bActivated) = 0;

	virtual void			ApplyGameSettings(KeyValues* pKV) = 0;

	virtual void			GetMatchmakingTags(char* buf, size_t bufSize) = 0;

	virtual void			ServerHibernationUpdate(bool bHibernating) = 0;

	virtual bool			ShouldPreferSteamAuth() = 0;

	virtual bool			ShouldAllowDirectConnect() = 0;
	virtual bool			FriendsReqdForDirectConnect() = 0;
	virtual bool			IsLoadTestServer() = 0;
	virtual bool			IsValveDS() = 0;
	virtual KeyValues* GetExtendedServerInfoForNewClient() = 0;
	virtual void 			UpdateGCInformation() = 0;
	virtual void 			ReportGCQueuedMatchStart(int32 iReservationStage, uint32* puiConfirmedAccounts, int numConfirmedAccounts) = 0;

	virtual PublishedFileId_t	GetUGCMapFileID(const char* mapName) = 0;
	virtual void			GetMatchmakingGameData(char* buf, size_t bufSize) = 0;
	virtual bool			HasPendingMapDownloads() const = 0;
	virtual void			UpdateUGCMap(PublishedFileId_t file) = 0;
	virtual int				GetMessageEncryptionKey(INetMessage* msg) = 0;
	virtual bool			ShouldHoldGameServerReservation(float flTime) = 0;
	virtual bool			OnPureServerFileValidationFailure(edict_t* pPlayer, const char* pszPathID, const char* pszFileName,
								CRC32_t crcIOSequence, int eFileHashType, int cbFileLen, int nPackFileNumber, int nPackFileID) = 0;
	virtual void			PrecacheParticleSystemFile(const char* pszFilename) = 0;
	virtual void			ClientConnectionValidatePreNetChan(bool, const char*, int, unsigned long long) = 0;
	virtual void			OnEngineClientNetworkEvent(edict_t*, unsigned long long, int, void*) = 0;
	virtual void			GetNewestSubscribedFiles() = 0;
	virtual bool			ValidateAndAddActiveCaster(const CSteamID&) = 0;
};

#define VENGINE_SERVER_RANDOM_INTERFACE_VERSION	"VEngineRandom001"

#define INTERFACEVERSION_SERVERGAMEENTS			"ServerGameEnts001"
abstract_class IServerGameEnts
{
public:
	virtual					~IServerGameEnts() {}

	virtual void			MarkEntitiesAsTouching(edict_t * e1, edict_t * e2) = 0;

	virtual void			FreeContainingEntity(edict_t*) = 0;

	virtual edict_t* BaseEntityToEdict(CBaseEntity* pEnt) = 0;
	virtual CBaseEntity* EdictToBaseEntity(edict_t* pEdict) = 0;

	virtual void			CheckTransmit(CCheckTransmitInfo* pInfo, const unsigned short* pEdictIndices, int nEdicts) = 0;

	virtual void			PrepareForFullUpdate(edict_t* pEdict) = 0;
};

#define INTERFACEVERSION_SERVERGAMECLIENTS		"ServerGameClients004"

abstract_class IServerGameClients
{
public:
	virtual void			GetPlayerLimits(int& minplayers, int& maxplayers, int& defaultMaxPlayers) const = 0;

	virtual bool			ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;

	virtual void			ClientActive(edict_t* pEntity, bool bLoadGame) = 0;

	virtual void			ClientFullyConnect(edict_t* pEntity) = 0;

	virtual void			ClientDisconnect(edict_t* pEntity) = 0;

	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) = 0;

	virtual void			ClientCommand(edict_t* pEntity, const CCommand& args) = 0;

	virtual void			SetCommandClient(int index) = 0;

	virtual void			ClientSettingsChanged(edict_t* pEdict) = 0;

	virtual void			ClientSetupVisibility(edict_t* pViewEntity, edict_t* pClient, unsigned char* pvs, int pvssize) = 0;

	virtual float			ProcessUsercmds(edict_t* player, bf_read* buf, int numcmds, int totalcmds,
								int dropped_packets, bool ignore, bool paused) = 0;

	virtual void			PostClientMessagesSent(void) = 0;

	virtual CPlayerState* GetPlayerState(edict_t* player) = 0;

	virtual void			ClientEarPosition(edict_t* pEntity, Vector* pEarOrigin) = 0;

	virtual bool			ClientReplayEvent(edict_t* player, void* event    ) = 0;

	virtual int				GetReplayDelay(edict_t* player, int& entity) = 0;

	virtual void			GetBugReportInfo(char* buf, int buflen) = 0;

	virtual void			ClientVoice(edict_t* pEdict) = 0;

	virtual void			NetworkIDValidated(const char* pszUserName, const char* pszNetworkID, CSteamID steamID) = 0;

	virtual int				GetMaxSplitscreenPlayers() = 0;

	virtual int				GetMaxHumanPlayers() = 0;

	virtual void			ClientCommandKeyValues(edict_t* pEntity, KeyValues* pKeyValues) = 0;

	virtual void			ClientNameHandler(unsigned long long steamid, const char* name) = 0;
};

#define INTERFACEVERSION_UPLOADGAMESTATS		"ServerUploadGameStats001"

abstract_class IUploadGameStats
{
public:
	virtual bool UploadGameStats(
		char const* mapname,				   
		unsigned int blobversion,			      
		unsigned int blobsize,				      
		const void* pvBlobData) = 0;		     

	virtual void InitConnection(void) = 0;

	virtual void UpdateConnection(void) = 0;

	virtual bool IsGameStatsLoggingEnabled() = 0;

	virtual void GetPseudoUniqueId(char* buf, size_t bufsize) = 0;

	virtual bool IsCyberCafeUser(void) = 0;

	virtual bool IsHDREnabled(void) = 0;
};

#define INTERFACEVERSION_PLUGINHELPERSCHECK		"PluginHelpersCheck001"

abstract_class IPluginHelpersCheck
{
public:
	virtual bool CreateMessage(const char* plugin, edict_t * pEntity, DIALOG_TYPE type, KeyValues * data) = 0;
};

abstract_class IServerDLLSharedAppSystems
{
public:
	virtual int	Count() = 0;
	virtual char const* GetDllName(int idx) = 0;
	virtual char const* GetInterfaceName(int idx) = 0;
};

#define SERVER_DLL_SHARED_APPSYSTEMS		"VServerDllSharedAppSystems001"

#define INTERFACEVERSION_SERVERGAMETAGS		"ServerGameTags001"

abstract_class IServerGameTags
{
public:
	virtual void			GetTaggedConVarList(KeyValues * pCvarTagList) = 0;
};

extern IVEngineServer* g_pEngineServer;
extern IServerGameDLL* g_pServerGameDLL;

#endif  