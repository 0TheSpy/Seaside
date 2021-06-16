#if !defined ( GAMEEVENTMANAGER_H )
#define GAMEEVENTMANAGER_H

#ifdef _WIN32
#pragma once
#endif 

#include "igameevents.h"
#include "utlvector.h"
#include "KeyValues.h"
#include "networkstringtabledefs.h"
#include "utlsymbol.h"

#include "bitbuf.h" 

class SVC_GameEventList;
class CLC_ListenEvents;

class CGameEventCallback
{
public:
	void* m_pCallback;		  
	int					m_nListenerType;	     
};

class CGameEventDescriptor
{
public:
	CGameEventDescriptor()
	{
		name[0] = 0;
		eventid = -1;
		keys = NULL;
		local = false;
		reliable = true;
	}

public:
	char		name[MAX_EVENT_NAME_LENGTH];	    
	int			eventid;	       
	KeyValues* keys;		         
	bool		local;		       
	bool		reliable;	      
	CUtlVector<CGameEventCallback*>	listeners;	  
};

class CGameEvent : public IGameEvent
{
public:

	CGameEvent(CGameEventDescriptor* descriptor);
	virtual ~CGameEvent();

	const char* GetName() const;
	bool  IsEmpty(const char* keyName = NULL);
	bool  IsLocal() const;
	bool  IsReliable() const;

	bool  GetBool(const char* keyName = NULL, bool defaultValue = false);
	int   GetInt(const char* keyName = NULL, int defaultValue = 0);
	float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f);
	const char* GetString(const char* keyName = NULL, const char* defaultValue = "");

	void SetBool(const char* keyName, bool value);
	void SetInt(const char* keyName, int value);
	void SetFloat(const char* keyName, float value);
	void SetString(const char* keyName, const char* value);

	CGameEventDescriptor* m_pDescriptor;
	KeyValues* m_pDataKeys;
};

class CGameEventManager : public IGameEventManager2
{
	friend class CGameEventManagerOld;

public:	  

	enum
	{
		SERVERSIDE = 0,		         
		CLIENTSIDE,			         
		CLIENTSTUB,			              
		SERVERSIDE_OLD,		       
		CLIENTSIDE_OLD,		       
	};

	enum
	{
		TYPE_LOCAL = 0,	  
		TYPE_STRING,	    
		TYPE_FLOAT,		   
		TYPE_LONG,		    
		TYPE_SHORT,		    
		TYPE_BYTE,		    
		TYPE_BOOL		    
	};

	CGameEventManager();
	virtual ~CGameEventManager();

	int	 LoadEventsFromFile(const char* filename);
	void Reset();

	bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide);
	bool FindListener(IGameEventListener2* listener, const char* name);
	void RemoveListener(IGameEventListener2* listener);

	IGameEvent* CreateEvent(const char* name, bool bForce = false);
	IGameEvent* DuplicateEvent(IGameEvent* event);
	bool FireEvent(IGameEvent* event, bool bDontBroadcast = false);
	bool FireEventClientSide(IGameEvent* event);
	void FreeEvent(IGameEvent* event);

	bool SerializeEvent(IGameEvent* event, bf_write* buf);
	IGameEvent* UnserializeEvent(bf_read* buf);

public:
	bool Init();
	void Shutdown();
	void ReloadEventDefinitions();	      
	bool AddListener(void* listener, CGameEventDescriptor* descriptor, int nListenerType);

	CGameEventDescriptor* GetEventDescriptor(const char* name);
	CGameEventDescriptor* GetEventDescriptor(IGameEvent* event);
	CGameEventDescriptor* GetEventDescriptor(int eventid);

	void WriteEventList(SVC_GameEventList* msg);
	bool ParseEventList(SVC_GameEventList* msg);

	void WriteListenEventList(CLC_ListenEvents* msg);
	bool HasClientListenersChanged(bool bReset = true);
	void ConPrintEvent(IGameEvent* event);

	bool AddListenerAll(void* listener, int nListenerType);
	void RemoveListenerOld(void* listener);


protected:

	IGameEvent* CreateEvent(CGameEventDescriptor* descriptor);
	bool RegisterEvent(KeyValues* keys);
	void UnregisterEvent(int index);
	bool FireEventIntern(IGameEvent* event, bool bServerSide, bool bClientOnly);
	CGameEventCallback* FindEventListener(void* listener);

	CUtlVector<CGameEventDescriptor>	m_GameEvents;	     
	CUtlVector<CGameEventCallback*>		m_Listeners;	     
	CUtlSymbolTable						m_EventFiles;	      
	CUtlVector<CUtlSymbol>				m_EventFileNames;

	bool	m_bClientListenersChanged;	      
};

extern CGameEventManager& g_GameEventManager;

#endif 