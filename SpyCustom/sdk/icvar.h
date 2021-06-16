#ifndef ICVAR_H
#define ICVAR_H
#ifdef _WIN32
#pragma once
#endif

#include "IAppSystem.h"
#include "iconvar.h"

class ConCommandBase;
class ConCommand;
class ConVar;
class Color;


typedef int CVarDLLIdentifier_t;


abstract_class IConsoleDisplayFunc
{
public:
	virtual void ColorPrint(const Color & clr, const char* pMessage) = 0;
	virtual void Print(const char* pMessage) = 0;
	virtual void DPrint(const char* pMessage) = 0;
};


#define CVAR_QUERY_INTERFACE_VERSION "VCvarQuery001"
abstract_class ICvarQuery : public IAppSystem
{
public:
	virtual bool AreConVarsLinkable(const ConVar * child, const ConVar * parent) = 0;
};


class ICvar : public IAppSystem
{
public:
	virtual CVarDLLIdentifier_t        AllocateDLLIdentifier() = 0;  
	virtual void                       RegisterConCommand(ConCommandBase* pCommandBase) = 0; 
	virtual void                       UnregisterConCommand(ConCommandBase* pCommandBase) = 0;
	virtual void                       UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
	virtual const char* GetCommandLineValue(const char* pVariableName) = 0;
	virtual ConCommandBase* FindCommandBase(const char* name) = 0;
	virtual const ConCommandBase* FindCommandBase(const char* name) const = 0;
	virtual ConVar* FindVar(const char* var_name) = 0; 
	virtual const ConVar* FindVar(const char* var_name) const = 0;
	virtual ConCommand* FindCommand(const char* name) = 0;
	virtual const ConCommand* FindCommand(const char* name) const = 0;
	virtual void                       InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void                       RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void                       CallGlobalChangeCallbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
	virtual void                       InstallConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void                       RemoveConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void                       ConsoleColorPrintf(const Color& clr, const char* pFormat, ...) const = 0;
	virtual void                       ConsolePrintf(const char* pFormat, ...) const = 0;
	virtual void                       ConsoleDPrintf(const char* pFormat, ...) const = 0;
	virtual void                       RevertFlaggedConVars(int nFlag) = 0;
};

#define CVAR_INTERFACE_VERSION "VEngineCvar004"


extern ICvar* g_pCVar;


#endif  