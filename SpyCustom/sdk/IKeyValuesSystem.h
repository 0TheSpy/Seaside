#ifndef VSTDLIB_IKEYVALUESSYSTEM_H
#define VSTDLIB_IKEYVALUESSYSTEM_H
#ifdef _WIN32
#pragma once
#endif

#include "vstdlib.h"

typedef int HKeySymbol;
#define INVALID_KEY_SYMBOL (-1)

class IKeyValuesSystem
{
public:
    virtual ~IKeyValuesSystem() = 0;

    virtual void RegisterSizeofKeyValues(int size) = 0;

    virtual void* AllocKeyValuesMemory(int size) = 0;
    virtual void FreeKeyValuesMemory(void* pMem) = 0;

    virtual HKeySymbol GetSymbolForString(const char* name, bool bCreate = true) = 0;
    virtual const char* GetStringForSymbol(HKeySymbol symbol) = 0;

    virtual void AddKeyValuesToMemoryLeakList(void* pMem, HKeySymbol name) = 0;
    virtual void RemoveKeyValuesFromMemoryLeakList(void* pMem) = 0;

    virtual void SetKeyValuesExpressionSymbol(const char* name, bool bValue) = 0;
    virtual bool GetKeyValuesExpressionSymbol(const char* name) = 0;

    virtual HKeySymbol GetSymbolForStringCaseSensitive(HKeySymbol& hCaseInsensitiveSymbol, const char* name, bool bCreate = true) = 0;
};

VSTDLIB_INTERFACE IKeyValuesSystem* KeyValuesSystem();

#endif  
