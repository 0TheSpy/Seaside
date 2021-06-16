
#ifndef WCHARTYPES_H
#define WCHARTYPES_H
#ifdef _WIN32
#pragma once
#endif

#ifdef _INC_TCHAR
#error ("Must include tier0 type headers before tchar.h")
#endif

#if !defined(_WCHAR_T_DEFINED) && !defined(GNUC)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

typedef char char8;

typedef unsigned char uint8;
typedef unsigned char BYTE;
typedef unsigned char byte;

typedef wchar_t wchar;
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

#ifdef STEAM
#ifndef _UNICODE
#define FORCED_UNICODE
#endif
#define _UNICODE
#endif

#ifdef _WIN32
#include <tchar.h>
#else
#define _tcsstr strstr
#define _tcsicmp stricmp
#define _tcscmp strcmp
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcsrchr strrchr
#define _tcslen strlen
#define _tfopen fopen
#define _stprintf sprintf 
#define _ftprintf fprintf
#define _vsntprintf _vsnprintf
#define _tprintf printf
#define _sntprintf _snprintf
#define _T(s) s
#endif

#if defined(_UNICODE)
typedef wchar tchar;
#define tstring wstring
#define __TFILE__ __WFILE__
#define TCHAR_IS_WCHAR
#else
typedef char tchar;
#define tstring string
#define __TFILE__ __FILE__
#define TCHAR_IS_CHAR
#endif

#ifdef FORCED_UNICODE
#undef _UNICODE
#endif


#endif  
