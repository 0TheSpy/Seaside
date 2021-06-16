#ifndef MINIDUMP_H
#define MINIDUMP_H
#ifdef _WIN32
#pragma once
#endif

#include "platform.h"

PLATFORM_INTERFACE void SetMinidumpFilenamePrefix(const char* pszPrefix);

PLATFORM_INTERFACE void SetMinidumpComment(const char* pszComment);

PLATFORM_INTERFACE void WriteMiniDump(const char* pszFilenameSuffix = NULL);

typedef void (*FnWMain)(int, tchar* []);
typedef void (*FnVoidPtrFn)(void*);

#if defined(_WIN32) && !defined(_X360)

typedef void (*FnWMain)(int, tchar* []);
typedef int (*FnWMainIntRet)(int, tchar* []);
typedef void (*FnVoidPtrFn)(void*);

enum ECatchAndWriteMinidumpAction
{
	k_ECatchAndWriteMiniDumpAbort = 0,
	k_ECatchAndWriteMiniDumpReThrow = 1,
	k_ECatchAndWriteMiniDumpIgnore = 2,
};

PLATFORM_INTERFACE void CatchAndWriteMiniDump(FnWMain pfn, int argc, tchar* argv[]);    
PLATFORM_INTERFACE void CatchAndWriteMiniDumpForVoidPtrFn(FnVoidPtrFn pfn, void* pv, bool bExitQuietly);        

PLATFORM_INTERFACE void CatchAndWriteMiniDumpEx(FnWMain pfn, int argc, tchar* argv[], ECatchAndWriteMinidumpAction eAction);
PLATFORM_INTERFACE int CatchAndWriteMiniDumpExReturnsInt(FnWMainIntRet pfn, int argc, tchar* argv[], ECatchAndWriteMinidumpAction eAction);
PLATFORM_INTERFACE void CatchAndWriteMiniDumpExForVoidPtrFn(FnVoidPtrFn pfn, void* pv, ECatchAndWriteMinidumpAction eAction);

struct _EXCEPTION_POINTERS;

typedef void (*FnMiniDump)(unsigned int uStructuredExceptionCode, _EXCEPTION_POINTERS* pExceptionInfo, const char* pszFilenameSuffix);
PLATFORM_INTERFACE FnMiniDump SetMiniDumpFunction(FnMiniDump pfn);

PLATFORM_INTERFACE bool WriteMiniDumpUsingExceptionInfo(
	unsigned int uStructuredExceptionCode,
	_EXCEPTION_POINTERS* pExceptionInfo,
	int    minidumpType,
	const char* pszFilenameSuffix = NULL,
	tchar* ptchMinidumpFileNameBuffer = NULL
);

PLATFORM_INTERFACE void MinidumpSetUnhandledExceptionFunction(FnMiniDump pfn);

PLATFORM_INTERFACE void EnableCrashingOnCrashes();

#endif

#endif  