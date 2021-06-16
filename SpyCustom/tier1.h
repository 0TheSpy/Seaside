
#ifndef TIER1_H
#define TIER1_H

#if defined( _WIN32 )
#pragma once
#endif

#include "iappsystem.h"
#include "convar.h"


void ConnectTier1Libraries(CreateInterfaceFn* pFactoryList, int nFactoryCount);
void DisconnectTier1Libraries();


template< class IInterface, int ConVarFlag = 0 >
class CTier1AppSystem : public CTier0AppSystem< IInterface >
{
	typedef CTier0AppSystem< IInterface > BaseClass;

public:
	virtual bool Connect(CreateInterfaceFn factory)
	{
		if (!BaseClass::Connect(factory))
			return false;

		ConnectTier1Libraries(&factory, 1);
		return true;
	}

	virtual void Disconnect()
	{
		DisconnectTier1Libraries();
		BaseClass::Disconnect();
	}

	virtual InitReturnVal_t Init()
	{
		InitReturnVal_t nRetVal = BaseClass::Init();
		if (nRetVal != INIT_OK)
			return nRetVal;

		if (g_pCVar)
		{
			ConVar_Register(ConVarFlag);
		}
		return INIT_OK;
	}

	virtual void Shutdown()
	{
		if (g_pCVar)
		{
			ConVar_Unregister();
		}
		BaseClass::Shutdown();
	}

	
	virtual AppSystemTier_t GetTier()
	{
		return APP_SYSTEM_TIER1;
	}
};


#endif  