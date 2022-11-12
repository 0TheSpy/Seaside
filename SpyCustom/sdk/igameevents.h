#if !defined( IGAMEEVENTS_H )
#define IGAMEEVENTS_H
#ifdef _WIN32
#pragma once
#endif

#include "interface.h"


#define INTERFACEVERSION_GAMEEVENTSMANAGER	"GAMEEVENTSMANAGER001"	       
#define INTERFACEVERSION_GAMEEVENTSMANAGER2	"GAMEEVENTSMANAGER002"	    

#include "bitbuf.h"
#define NULL 0x0
#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13


#define MAX_EVENT_NAME_LENGTH	32		     
#define MAX_EVENT_BITS			9		       
#define MAX_EVENT_NUMBER		(1<<MAX_EVENT_BITS)		     
#define MAX_EVENT_BYTES			1024	        

class KeyValues;
class CGameEvent;

class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char* GetName() const = 0;	   

	virtual bool  IsReliable() const = 0;     
	virtual bool  IsLocal() const = 0;      
	virtual bool  IsEmpty(const char* keyName = NULL) = 0;      

	virtual bool  GetBool(const char* keyName = NULL, bool defaultValue = false) = 0;
	virtual int   GetInt(const char* keyName = NULL, int defaultValue = 0) = 0;
	virtual uint64_t        GetUint64(const char* keyName = nullptr, unsigned long defaultValue = 0) = 0; 
	virtual float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(const char* keyName = NULL, const char* defaultValue = "") = 0;
	virtual const wchar_t* GetWString(char const* keyName = NULL, const wchar_t* defaultValue = L"") = 0; 

	virtual void SetBool(const char* keyName, bool value) = 0;
	virtual void SetInt(const char* keyName, int value) = 0;
	virtual void            SetUint64(const char* keyName, unsigned long value) = 0; 
	virtual void SetFloat(const char* keyName, float value) = 0;
	virtual void test() = 0;
	virtual void SetString(const char* keyName, const char* value) = 0;
	virtual void            SetWString(const char* keyName, const wchar_t* value) = 0; 
};


class IGameEventListener2
{
public:
	virtual	~IGameEventListener2(void) {};

	virtual void FireGameEvent(IGameEvent* event) = 0;
	virtual int GetEventDebugID(void) = 0;
};

class IGameEventManager2 : public IBaseInterface
{
public:
	virtual	~IGameEventManager2(void) {};

	virtual int LoadEventsFromFile(const char* filename) = 0;

	virtual void  Reset() = 0;

	virtual bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide) = 0;

	virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;

	virtual void RemoveListener(IGameEventListener2* listener) = 0;

	// add a listener that listens to all events. 
	virtual bool AddListenerGlobal(IGameEventListener2* listener, bool bServerSide) = 0;

	virtual IGameEvent* CreateEvent(const char* name, bool bForce = false) = 0;

	virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;

	virtual bool FireEventClientSide(IGameEvent* event) = 0;

	virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;

	virtual void FreeEvent(IGameEvent* event) = 0;

	virtual bool SerializeEvent(IGameEvent* event, bf_write* buf) = 0;
	virtual IGameEvent* UnserializeEvent(bf_read* buf) = 0;       

	//virtual KeyValues* GetEventDataTypes(IGameEvent* event) = 0;
};

class IGameEventListener
{
public:
	virtual	~IGameEventListener(void) {};

	virtual void FireGameEvent(KeyValues* event) = 0;
};

class IGameEventManager : public IBaseInterface
{
public:
	virtual	~IGameEventManager(void) {};

	virtual int LoadEventsFromFile(const char* filename) = 0;

	virtual void  Reset() = 0;

	virtual KeyValues* GetEvent(const char* name) = 0;     

	virtual bool AddListener(IGameEventListener* listener, const char* event, bool bIsServerSide) = 0;
	virtual bool AddListener(IGameEventListener* listener, bool bIsServerSide) = 0;

	virtual void RemoveListener(IGameEventListener* listener) = 0;

	virtual bool FireEvent(KeyValues* event) = 0;

	virtual bool FireEventServerOnly(KeyValues* event) = 0;

	virtual bool FireEventClientOnly(KeyValues* event) = 0;

	virtual bool SerializeKeyValues(KeyValues* event, bf_write* buf, CGameEvent* eventtype = NULL) = 0;
	virtual KeyValues* UnserializeKeyValue(bf_read* msg) = 0;       
};




#endif  