#ifndef IAPPSYSTEM_H
#define IAPPSYSTEM_H

#ifdef COMPILER_MSVC
#pragma once
#endif

#include "interface.h"



struct AppSystemInfo_t
{
	const char* m_pModuleName;
	const char* m_pInterfaceName;
};


enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,

	INIT_LAST_VAL,
};

enum AppSystemTier_t
{
	APP_SYSTEM_TIER0 = 0,
	APP_SYSTEM_TIER1,
	APP_SYSTEM_TIER2,
	APP_SYSTEM_TIER3,

	APP_SYSTEM_TIER_OTHER,
};


abstract_class IAppSystem
{
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;		
	virtual void Disconnect() = 0;								

	virtual void* QueryInterface(const char* pInterfaceName) = 0;

	virtual InitReturnVal_t Init() = 0;								
	virtual void Shutdown() = 0;								

	virtual const AppSystemInfo_t* GetDependencies() { return NULL; }

	virtual AppSystemTier_t GetTier() { return APP_SYSTEM_TIER_OTHER; }								

	virtual void Reconnect(CreateInterfaceFn factory, const char* pInterfaceName) {}								
	virtual bool IsSingleton() { return true; }								
};


template< class IInterface >
class CBaseAppSystem : public IInterface
{
public:
	virtual bool Connect(CreateInterfaceFn factory) { return true; }
	virtual void Disconnect() {}

	virtual void* QueryInterface(const char* pInterfaceName) { return NULL; }

	virtual InitReturnVal_t Init() { return INIT_OK; }
	virtual void Shutdown() {}

	virtual const AppSystemInfo_t* GetDependencies() { return NULL; }
	virtual AppSystemTier_t GetTier() { return APP_SYSTEM_TIER_OTHER; }

	virtual void Reconnect(CreateInterfaceFn factory, const char* pInterfaceName);
};


template< class IInterface >
class CTier0AppSystem : public CBaseAppSystem< IInterface >
{
};


#endif  

