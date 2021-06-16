#ifndef IGAMEUI_H
#define IGAMEUI_H
#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "IPanel.h"

#if !defined( _X360 )
#include "xboxstubs.h"
#endif

enum ESteamLoginFailure
{
	STEAMLOGINFAILURE_NONE,
	STEAMLOGINFAILURE_BADTICKET,
	STEAMLOGINFAILURE_NOSTEAMLOGIN,
	STEAMLOGINFAILURE_VACBANNED,
	STEAMLOGINFAILURE_LOGGED_IN_ELSEWHERE
};

enum ESystemNotify
{
	SYSTEMNOTIFY_STORAGEDEVICES_CHANGED,
	SYSTEMNOTIFY_USER_SIGNEDIN,
	SYSTEMNOTIFY_USER_SIGNEDOUT,
	SYSTEMNOTIFY_XUIOPENING,
	SYSTEMNOTIFY_XUICLOSED,
	SYSTEMNOTIFY_INVITE_SHUTDOWN,	       
};

abstract_class IGameUI
{
public:
	virtual void Initialize(CreateInterfaceFn appFactory) = 0;
	virtual void PostInit() = 0;

	virtual void Connect(CreateInterfaceFn gameFactory) = 0;

	virtual void Start() = 0;
	virtual void Shutdown() = 0;
	virtual void RunFrame() = 0;

	virtual void OnGameUIActivated() = 0;
	virtual void OnGameUIHidden() = 0;

	virtual void OLD_OnConnectToServer(const char* game, int IP, int port) = 0;

	virtual void OnDisconnectFromServer_OLD(uint8 eSteamLoginFailure, const char* username) = 0;
	virtual void OnLevelLoadingStarted(bool bShowProgressDialog) = 0;
	virtual void OnLevelLoadingFinished(bool bError, const char* failureReason, const char* extendedReason) = 0;

	virtual bool UpdateProgressBar(float progress, const char* statusText) = 0;
	virtual bool SetShowProgressText(bool show) = 0;

	virtual void ShowNewGameDialog(int chapter) = 0;

	virtual void SessionNotification(const int notification, const int param = 0) = 0;
	virtual void SystemNotification(const int notification) = 0;
	virtual void ShowMessageDialog(const uint nType, vgui::Panel* pOwner) = 0;
	virtual void UpdatePlayerInfo(uint64 nPlayerId, const char* pName, int nTeam, byte cVoiceState, int nPlayersNeeded, bool bHost) = 0;
	virtual void SessionSearchResult(int searchIdx, void* pHostData, XSESSION_SEARCHRESULT* pResult, int ping) = 0;
	virtual void OnCreditsFinished(void) = 0;

	virtual void SetLoadingBackgroundDialog(vgui::VPANEL panel) = 0;

	virtual void BonusMapUnlock(const char* pchFileName = NULL, const char* pchMapName = NULL) = 0;
	virtual void BonusMapComplete(const char* pchFileName = NULL, const char* pchMapName = NULL) = 0;
	virtual void BonusMapChallengeUpdate(const char* pchFileName, const char* pchMapName, const char* pchChallengeName, int iBest) = 0;
	virtual void BonusMapChallengeNames(char* pchFileName, char* pchMapName, char* pchChallengeName) = 0;
	virtual void BonusMapChallengeObjectives(int& iBronze, int& iSilver, int& iGold) = 0;
	virtual void BonusMapDatabaseSave(void) = 0;
	virtual int BonusMapNumAdvancedCompleted(void) = 0;
	virtual void BonusMapNumMedals(int piNumMedals[3]) = 0;

	virtual void OnConnectToServer2(const char* game, int IP, int connectionPort, int queryPort) = 0;

	virtual bool ValidateStorageDevice(int* pStorageDeviceValidated) = 0;

	virtual void SetProgressOnStart() = 0;
	virtual void OnDisconnectFromServer(uint8 eSteamLoginFailure) = 0;

	virtual void OnConfirmQuit(void) = 0;

	virtual bool IsMainMenuVisible(void) = 0;

	virtual void SetMainMenuOverride(vgui::VPANEL panel) = 0;
	virtual void SendMainMenuCommand(const char* pszCommand) = 0;
};

#define GAMEUI_INTERFACE_VERSION "GameUI011"

#endif  