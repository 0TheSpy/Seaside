#ifndef ICLIENTTHINKABLE_H
#define ICLIENTTHINKABLE_H
#ifdef _WIN32
#pragma once
#endif


#include "iclientunknown.h"


class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;


abstract_class IClientThinkable
{
public:
	virtual IClientUnknown * GetIClientUnknown() = 0;

	virtual void				ClientThink() = 0;

	virtual ClientThinkHandle_t	GetThinkHandle() = 0;
	virtual void				SetThinkHandle(ClientThinkHandle_t hThink) = 0;

	virtual void				Release() = 0;
};


#endif  
