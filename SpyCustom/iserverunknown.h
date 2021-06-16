#ifndef ISERVERUNKNOWN_H
#define ISERVERUNKNOWN_H

#ifdef _WIN32
#pragma once
#endif


#include "ihandleentity.h"

class ICollideable;
class IServerNetworkable;
class CBaseEntity;


class IServerUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IServerNetworkable* GetNetworkable() = 0;
	virtual CBaseEntity* GetBaseEntity() = 0;
};


#endif  