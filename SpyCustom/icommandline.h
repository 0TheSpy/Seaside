#ifndef TIER0_ICOMMANDLINE_H
#define TIER0_ICOMMANDLINE_H
#ifdef _WIN32
#pragma once
#endif

#include "platform.h"


abstract_class ICommandLine
{
public:
	virtual void		CreateCmdLine(const char* commandline) = 0;
	virtual void		CreateCmdLine(int argc, char** argv) = 0;
	virtual const char* GetCmdLine(void) const = 0;

	virtual	const char* CheckParm(const char* psz, const char** ppszValue = 0) const = 0;
	virtual bool		HasParm(const char* psz) const = 0;
	virtual void		RemoveParm(const char* parm) = 0;
	virtual void		AppendParm(const char* pszParm, const char* pszValues) = 0;

	virtual const char* ParmValue(const char* psz, const char* pDefaultVal = 0) const = 0;
	virtual int			ParmValue(const char* psz, int nDefaultVal) const = 0;
	virtual float		ParmValue(const char* psz, float flDefaultVal) const = 0;

	virtual int			ParmCount() const = 0;
	virtual int			FindParm(const char* psz) const = 0;	     
	virtual const char* GetParm(int nIndex) const = 0;

	virtual void SetParm(int nIndex, char const* pNewParm) = 0;

	virtual const char** GetParms() const = 0;
};

PLATFORM_INTERFACE ICommandLine* CommandLine();


PLATFORM_INTERFACE const tchar* Plat_GetCommandLine();
#ifndef _WIN32
PLATFORM_INTERFACE void Plat_SetCommandLine(const char* cmdLine);
#endif
PLATFORM_INTERFACE const char* Plat_GetCommandLineA();


#endif  
