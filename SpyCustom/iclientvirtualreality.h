#ifndef ICLIENTVIRTUALREALITY_H
#define ICLIENTVIRTUALREALITY_H

#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "refcount.h"
#include "IAppSystem.h"

#define CLIENTVIRTUALREALITY_INTERFACE_VERSION "ClientVirtualReality001"



abstract_class IClientVirtualReality : public IAppSystem
{
public:
	virtual ~IClientVirtualReality() {}

	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;

	virtual void DrawMainMenu() = 0;
};



extern IClientVirtualReality* g_pClientVR;


#endif  
