#ifndef ISERVERPLUGIN_H
#define ISERVERPLUGIN_H

#ifdef _WIN32
#pragma once
#endif

#include "edict.h"
#include "interface.h"
#include "KeyValues.h"

class CCommand;

typedef enum
{
	PLUGIN_CONTINUE = 0,   
	PLUGIN_OVERRIDE,            
	PLUGIN_STOP,         
} PLUGIN_RESULT;


typedef enum
{
	eQueryCvarValueStatus_ValueIntact = 0,	     
	eQueryCvarValueStatus_CvarNotFound = 1,
	eQueryCvarValueStatus_NotACvar = 2,		        
	eQueryCvarValueStatus_CvarProtected = 3	                
} EQueryCvarValueStatus;


typedef int QueryCvarCookie_t;
#define InvalidQueryCvarCookie -1


#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_1	"ISERVERPLUGINCALLBACKS001"
#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_2	"ISERVERPLUGINCALLBACKS002"
#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS				"ISERVERPLUGINCALLBACKS003"

abstract_class IServerPluginCallbacks
{
public:
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;

	virtual void			Unload(void) = 0;

	virtual void			Pause(void) = 0;

	virtual void			UnPause(void) = 0;

	virtual const char* GetPluginDescription(void) = 0;

	virtual void			LevelInit(char const* pMapName) = 0;

	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) = 0;

	virtual void			GameFrame(bool simulating) = 0;

	virtual void			LevelShutdown(void) = 0;

	virtual void			ClientActive(edict_t* pEntity) = 0;

	virtual void			ClientDisconnect(edict_t* pEntity) = 0;

	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) = 0;

	virtual void			SetCommandClient(int index) = 0;

	virtual void			ClientSettingsChanged(edict_t* pEdict) = 0;

	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;

	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args) = 0;

	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;

	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) = 0;

	virtual void			OnEdictAllocated(edict_t* edict) = 0;
	virtual void			OnEdictFreed(const edict_t* edict) = 0;
};

#define INTERFACEVERSION_ISERVERPLUGINHELPERS			"ISERVERPLUGINHELPERS001"


typedef enum
{
	DIALOG_MSG = 0,		     
	DIALOG_MENU,		   
	DIALOG_TEXT,		   
	DIALOG_ENTRY,		   
	DIALOG_ASKCONNECT	                  
} DIALOG_TYPE;

abstract_class IServerPluginHelpers
{
public:
	virtual void CreateMessage(edict_t * pEntity, DIALOG_TYPE type, KeyValues * data, IServerPluginCallbacks * plugin) = 0;
	virtual void ClientCommand(edict_t* pEntity, const char* cmd) = 0;

	virtual QueryCvarCookie_t StartQueryCvarValue(edict_t* pEntity, const char* pName) = 0;
};

#endif 