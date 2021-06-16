#if !defined( ICLIENTENTITYLIST_H )
#define ICLIENTENTITYLIST_H

#ifdef _WIN32
#pragma once
#endif

#include "interface.h"

class IClientEntity;
class ClientClass;
class IClientNetworkable;
class CBaseHandle;
class IClientUnknown;


class IClientEntityList
{
public:
	virtual IClientNetworkable * GetClientNetworkable(int entnum) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;

	virtual IClientEntity* GetClientEntity(int entnum) = 0;
	virtual IClientEntity* GetClientEntityFromHandle(CBaseHandle hEnt) = 0;

	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

	virtual int					GetHighestEntityIndex(void) = 0;

	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};

#define VCLIENTENTITYLIST_INTERFACE_VERSION	"VClientEntityList003"

#endif  