#ifndef PATTERNSCAN
#define PATTERNSCAN
#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <string> 
#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

DWORD WaitOnModuleHandle(std::string moduleName);
DWORD FindPatternV2(std::string moduleName, std::string pattern);
bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask);
DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);

#include "Interfaces.hpp"

#endif

