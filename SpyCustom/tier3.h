
#ifndef TIER3_H
#define TIER3_H

#if defined( _WIN32 )
#pragma once
#endif

#include "tier2.h"


void ConnectTier3Libraries(CreateInterfaceFn* pFactoryList, int nFactoryCount);
void DisconnectTier3Libraries();


template< class IInterface, int ConVarFlag = 0 >
class CTier3AppSystem : public CTier2AppSystem< IInterface, ConVarFlag >
{
	typedef CTier2AppSystem< IInterface, ConVarFlag > BaseClass;

public:
	virtual bool Connect(CreateInterfaceFn factory)
	{
		if (!BaseClass::Connect(factory))
			return false;

		ConnectTier3Libraries(&factory, 1);
		return true;
	}

	virtual void Disconnect()
	{
		DisconnectTier3Libraries();
		BaseClass::Disconnect();
	}

	
	virtual AppSystemTier_t GetTier()
	{
		return APP_SYSTEM_TIER3;
	}
};


#endif  