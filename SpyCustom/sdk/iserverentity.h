#ifndef ISERVERENTITY_H
#define ISERVERENTITY_H
#ifdef _WIN32
#pragma once
#endif


#include "iserverunknown.h"
#include "string_t.h"



struct Ray_t;
class ServerClass;
class ICollideable;
class IServerNetworkable;
class Vector;
class QAngle;

class IServerEntity : public IServerUnknown
{
public:
	virtual					~IServerEntity() {}

	virtual int				GetModelIndex(void) const = 0;
	virtual string_t		GetModelName(void) const = 0;

	virtual void			SetModelIndex(int index) = 0;
};


#endif  