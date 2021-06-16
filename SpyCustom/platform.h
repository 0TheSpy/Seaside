#ifndef PLATFORM_H
#define PLATFORM_H

#if defined( _X360 )
#define NO_STEAM
#define NO_VOICE
#define _XBOX
#include <xtl.h>
#include <xaudio2.h>
#include <xbdm.h>
#include <Xgraphics.h>
#include <xui.h>
#include <pmcpbsetup.h>
#include <XMAHardwareAbstraction.h>
#undef _XBOX
#endif

#include "wchartypes.h"
#include "basetypes.h"
#include "valve_off.h"

#ifdef _DEBUG
#if !defined( PLAT_COMPILE_TIME_ASSERT )
#define PLAT_COMPILE_TIME_ASSERT( pred )	switch(0){case 0:case pred:;}
#endif
#else
#if !defined( PLAT_COMPILE_TIME_ASSERT )
#define PLAT_COMPILE_TIME_ASSERT( pred )
#endif
#endif

#ifdef _WIN32
#pragma once
#endif

#define NEW_SOFTWARE_LIGHTING

#ifdef POSIX
#include <alloca.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#endif

#include <malloc.h>
#include <new>


#include <string.h>

#include "valve_minmax_on.h"	        

#ifdef _RETAIL
#define IsRetail() true
#else
#define IsRetail() false
#endif

#ifdef _DEBUG
#define IsRelease() false
#define IsDebug() true
#else
#define IsRelease() true
#define IsDebug() false
#endif

#define IsXbox()	false

#ifdef _WIN32
#define IsLinux() false
#define IsOSX() false
#define IsPosix() false
#define PLATFORM_WINDOWS 1      
#ifndef _X360
#define IsWindows() true
#define IsPC() true
#define IsConsole() false
#define IsX360() false
#define IsPS3() false
#define IS_WINDOWS_PC
#define PLATFORM_WINDOWS_PC 1   
#ifdef _WIN64
#define IsPlatformWindowsPC64() true
#define IsPlatformWindowsPC32() false
#define PLATFORM_WINDOWS_PC64 1
#else
#define IsPlatformWindowsPC64() false
#define IsPlatformWindowsPC32() true
#define PLATFORM_WINDOWS_PC32 1
#endif
#else
#define PLATFORM_X360 1
#ifndef _CONSOLE
#define _CONSOLE
#endif
#define IsWindows() false
#define IsPC() false
#define IsConsole() true
#define IsX360() true
#define IsPS3() false
#endif
#if defined( DX_TO_GL_ABSTRACTION )
#define IsPlatformOpenGL() true
#else
#define IsPlatformOpenGL() false
#endif
#elif defined(POSIX)
#define IsPC() true
#define IsWindows() false
#define IsConsole() false
#define IsX360() false
#define IsPS3() false
#if defined( LINUX )
#define IsLinux() true
#else
#define IsLinux() false
#endif

#if defined( OSX )
#define IsOSX() true
#else
#define IsOSX() false
#endif

#define IsPosix() true
#define IsPlatformOpenGL() true
#else
#error
#endif

typedef unsigned char uint8;
typedef signed char int8;

#if defined(__x86_64__) || defined(_WIN64)
#define X64BITS
#endif  

#if defined( _WIN32 )

typedef __int16					int16;
typedef unsigned __int16		uint16;
typedef __int32					int32;
typedef unsigned __int32		uint32;
typedef __int64					int64;
typedef unsigned __int64		uint64;

#ifdef PLATFORM_64BITS
typedef __int64 intp;				         
typedef unsigned __int64 uintp;		         
#else
typedef __int32 intp;
typedef unsigned __int32 uintp;
#endif

#if defined( _X360 )
#ifdef __m128
#undef __m128
#endif
#define __m128				__vector4
#endif

#define OVERRIDE override

#else  

typedef short					int16;
typedef unsigned short			uint16;
typedef int						int32;
typedef unsigned int			uint32;
typedef long long				int64;
typedef unsigned long long		uint64;
#ifdef X64BITS
typedef long long			intp;
typedef unsigned long long	uintp;
#else
typedef int					intp;
typedef unsigned int		uintp;
#endif
typedef void* HWND;

#undef OVERRIDE
#if defined(__clang__)
#define OVERRIDE override
#pragma GCC diagnostic ignored "-Wc++11-extensions"
#else
#define OVERRIDE
#endif

#endif   

typedef uint32 RTime32;

typedef float				float32;
typedef double				float64;

typedef unsigned int		uint;

#ifdef _MSC_VER
#pragma once
#if _MSC_FULL_VER > 160000000
#if _MSC_FULL_VER < 160040219
#error You must install VS 2010 SP1
#endif
#else
#if _MSC_FULL_VER < 140050727
#error You must install VS 2005 SP1
#endif
#endif
#endif

#ifdef _MSC_VER
#define SINGLE_INHERITANCE __single_inheritance
#define MULTIPLE_INHERITANCE __multiple_inheritance
#else
#define SINGLE_INHERITANCE
#define MULTIPLE_INHERITANCE
#endif

#ifdef _MSC_VER
#define NO_VTABLE __declspec( novtable )
#else
#define NO_VTABLE
#endif

#ifdef _MSC_VER
#define NORETURN				__declspec( noreturn )
#else
#define NORETURN
#endif

#ifndef _X360
#define abstract_class class
#else
#define abstract_class class NO_VTABLE
#endif


#define VALVE_RAND_MAX 0x7fff



#if defined(_WIN32) && !defined(WINDED)

#if defined(_M_IX86)
#define __i386__	1
#endif

#elif POSIX
#if defined( OSX ) && defined( CARBON_WORKAROUND )
#define DWORD unsigned int
#else
typedef unsigned int DWORD;
#endif
typedef unsigned short WORD;
typedef void* HINSTANCE;
#define _MAX_PATH PATH_MAX
#define __cdecl
#define __stdcall
#define __declspec

#endif    


#ifndef MAX_PATH
#define MAX_PATH  260
#endif

#ifdef _WIN32
#define MAX_UNICODE_PATH 32767
#else
#define MAX_UNICODE_PATH MAX_PATH
#endif

#define MAX_UNICODE_PATH_IN_UTF8 MAX_UNICODE_PATH*4

#ifdef GNUC
#undef offsetof
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
#else
#undef offsetof
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
#endif


#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) )       

#if defined( _WIN32 ) && !defined( _X360 )
#define DebuggerBreak()  __debugbreak()
#elif defined( _X360 )
#define DebuggerBreak() DebugBreak()
#else
#ifdef OSX
#define DebuggerBreak()  if ( Plat_IsInDebugSession() ) { __asm ( "int $3" ); } else { raise(SIGTRAP); }
#else
#define DebuggerBreak()  raise(SIGTRAP)
#endif
#endif
#define	DebuggerBreakIfDebugging() if ( !Plat_IsInDebugSession() ) ; else DebuggerBreak()

#ifndef EXPORT
#ifdef _WIN32
#define EXPORT	_declspec( dllexport )
#else
#define EXPORT	 
#endif
#endif

#if defined __i386__ && !defined __linux__
#define id386	1
#else
#define id386	0
#endif   

#ifdef _WIN32
#define DECL_ALIGN(x) __declspec(align(x))

#elif GNUC
#define DECL_ALIGN(x) __attribute__((aligned(x)))
#else
#define DECL_ALIGN(x)  
#endif

#ifdef _MSC_VER
#define ALIGN4 DECL_ALIGN(4)
#define ALIGN8 DECL_ALIGN(8)
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN32 DECL_ALIGN(32)
#define ALIGN128 DECL_ALIGN(128)

#define ALIGN4_POST
#define ALIGN8_POST
#define ALIGN16_POST
#define ALIGN32_POST
#define ALIGN128_POST
#elif defined( GNUC )
#define ALIGN4
#define ALIGN8 
#define ALIGN16
#define ALIGN32
#define ALIGN128

#define ALIGN4_POST DECL_ALIGN(4)
#define ALIGN8_POST DECL_ALIGN(8)
#define ALIGN16_POST DECL_ALIGN(16)
#define ALIGN32_POST DECL_ALIGN(32)
#define ALIGN128_POST DECL_ALIGN(128)
#else
#error
#endif

#ifdef __GNUC__
#define COMPILE_TIME_ASSERT( pred ) typedef int UNIQUE_ID[ (pred) ? 1 : -1 ]
#else
#if _MSC_VER >= 1600
#define COMPILE_TIME_ASSERT( pred ) static_assert( pred, "Compile time assert constraint is not true: " #pred )
#else
#define COMPILE_TIME_ASSERT( pred ) typedef char compile_time_assert_type[(pred) ? 1 : -1];
#endif
#endif
#define ASSERT_INVARIANT( pred )	COMPILE_TIME_ASSERT( pred )

#if defined( PLATFORM_WINDOWS_PC )
 void Plat_MessageBox(const char* pTitle, const tchar * pMessage);
#else
#define Plat_MessageBox( t, m ) ((void)0)
#endif
#define	DebuggerBreakIfDebugging() if ( !Plat_IsInDebugSession() ) ; else DebuggerBreak()
void				Plat_GetModuleFilename(char* pOut, int nMaxBytes);
void				Plat_ExitProcess(int nCode);

void				Plat_ExitProcessWithError(int nCode, bool bGenerateMinidump = false);
#if defined( PLATFORM_X360 ) || defined( _PS3 )
#ifndef _GAMECONSOLE
#define _GAMECONSOLE
#endif
#define IsPC()		0
#define IsGameConsole() 1
#else
#define IsPC()		1
#define IsGameConsole() 0
#endif


inline int64 CastPtrToInt64(const void* p)
{
	return (int64)((uintp)p);
}

#include "annotations.h"

#define INT_TO_POINTER( i ) (void *)( ( i ) + (char *)NULL )
#define POINTER_TO_INT( p ) ( (int)(uintp)( p ) )


#if defined( GNUC )
#define stackalloc( _size )		alloca( ALIGN_VALUE( _size, 16 ) )
#ifdef _LINUX
#define mallocsize( _p )	( malloc_usable_size( _p ) )
#elif defined(OSX)
#define mallocsize( _p )	( malloc_size( _p ) )
#else
#error
#endif
#elif defined ( _WIN32 )
#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )
#define mallocsize( _p )		( _msize( _p ) )
#endif

#define  stackfree( _p )			0

#ifdef POSIX
#define CONSTRUCT_EARLY __attribute__((init_priority(101)))
#else
#define CONSTRUCT_EARLY
#endif

#if defined(_MSC_VER)
#define SELECTANY __declspec(selectany)
#define RESTRICT __restrict
#define RESTRICT_FUNC __declspec(restrict)
#define FMTFUNCTION( a, b )
#elif defined(GNUC)
#define SELECTANY __attribute__((weak))
#if defined(LINUX) && !defined(DEDICATED)
#define RESTRICT
#else
#define RESTRICT __restrict
#endif
#define RESTRICT_FUNC
#define FMTFUNCTION( fmtargnumber, firstvarargnumber ) __attribute__ (( format( __printf__, fmtargnumber, firstvarargnumber )))
#else
#define SELECTANY static
#define RESTRICT
#define RESTRICT_FUNC
#define FMTFUNCTION( a, b )
#endif

#if defined( _WIN32 )

#define DLL_EXPORT				extern "C" __declspec( dllexport )
#define DLL_IMPORT				extern "C" __declspec( dllimport )

#define DLL_CLASS_EXPORT		__declspec( dllexport )
#define DLL_CLASS_IMPORT		__declspec( dllimport )

#define DLL_GLOBAL_EXPORT		extern __declspec( dllexport )
#define DLL_GLOBAL_IMPORT		extern __declspec( dllimport )

#define DLL_LOCAL

#elif defined GNUC
#define  DLL_EXPORT   extern "C" __attribute__ ((visibility("default")))
#define  DLL_IMPORT   extern "C"

#define  DLL_CLASS_EXPORT __attribute__ ((visibility("default")))
#define  DLL_CLASS_IMPORT

#define  DLL_GLOBAL_EXPORT   extern __attribute ((visibility("default")))
#define  DLL_GLOBAL_IMPORT   extern

#define  DLL_LOCAL __attribute__ ((visibility("hidden")))

#else
#error "Unsupported Platform."
#endif

#if defined( _WIN32 ) && !defined( _X360 )
#define  STDCALL				__stdcall
#define  FASTCALL				__fastcall
#define  FORCEINLINE			__forceinline
#define  FORCEINLINE_TEMPLATE		__forceinline
#elif defined( _X360 )
#define  STDCALL				__stdcall
#ifdef FORCEINLINE
#undef FORCEINLINE
#endif 
#define  FORCEINLINE			__forceinline
#define  FORCEINLINE_TEMPLATE		__forceinline
#else
#define  STDCALL
#define  FASTCALL
#ifdef _LINUX_DEBUGGABLE
#define  FORCEINLINE
#else
#define  FORCEINLINE inline __attribute__ ((always_inline))
#endif
#define FORCEINLINE_TEMPLATE	inline
#endif

#define DONT_INLINE(a) (((int)(a)+1)?(a):(a))

#ifdef _MSC_VER
#define HINT(THE_HINT)	__assume((THE_HINT))
#else
#define HINT(THE_HINT)	0
#endif

#define UNREACHABLE() { Assert(0); HINT(0); }

#define NO_DEFAULT default: UNREACHABLE();


#ifdef _WIN32

#pragma warning(disable : 4514)           
#pragma warning(disable : 4100)        
#pragma warning(disable : 4127)       
#pragma warning(disable : 4512)           
#pragma warning(disable : 4611)            
#pragma warning(disable : 4710)       
#pragma warning(disable : 4702)     
#pragma warning(disable : 4505)       
#pragma warning(disable : 4239)               
#pragma warning(disable : 4097)         
#pragma warning(disable : 4324)          
#pragma warning(disable : 4244)    
#pragma warning(disable : 4305)	        
#pragma warning(disable : 4786)	      
#pragma warning(disable : 4250)       
#pragma warning(disable : 4201)       
#pragma warning(disable : 4481)         
#pragma warning(disable : 4748)                     

#if _MSC_VER >= 1300
#pragma warning(disable : 4511)	      
#pragma warning(disable : 4121)	            
#pragma warning(disable : 4530)	                      
#endif

#if _MSC_VER >= 1400
#pragma warning(disable : 4996)	   
#endif


#endif  

#if defined( LINUX ) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#define GCC_DIAG_STR(s) #s
#define GCC_DIAG_JOINSTR(x,y) GCC_DIAG_STR(x ## y)
#define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
#define GCC_DIAG_PRAGMA(x)	GCC_DIAG_DO_PRAGMA(GCC diagnostic x)

#define GCC_DIAG_PUSH_OFF(x)	GCC_DIAG_PRAGMA(push) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#define GCC_DIAG_POP()		GCC_DIAG_PRAGMA(pop)
#else
#define GCC_DIAG_PUSH_OFF(x)
#define GCC_DIAG_POP()
#endif

#ifdef LINUX
#pragma GCC diagnostic ignored "-Wconversion-null"	      
#pragma GCC diagnostic ignored "-Wpointer-arith"	            
#pragma GCC diagnostic ignored "-Wswitch"				      
#endif

#ifdef OSX
#pragma GCC diagnostic ignored "-Wconversion-null"			      
#pragma GCC diagnostic ignored "-Wnull-arithmetic"			            
#pragma GCC diagnostic ignored "-Wswitch-enum"				      
#pragma GCC diagnostic ignored "-Wswitch"					      
#endif


#if !defined( _WIN64 )
#pragma warning( disable : 4267 )	         
#pragma warning( disable : 4311 )	       
#pragma warning( disable : 4312 )	         
#endif


#ifdef POSIX
#define _stricmp stricmp
#define strcmpi stricmp
#define stricmp strcasecmp
#define _vsnprintf vsnprintf
#define _alloca alloca
#ifdef _snprintf
#undef _snprintf
#endif
#define _snprintf snprintf
#define GetProcAddress dlsym
#define _chdir chdir
#define _strnicmp strnicmp
#define strnicmp strncasecmp
#define _getcwd getcwd
#define _snwprintf swprintf
#define swprintf_s swprintf
#define wcsicmp _wcsicmp
#define _wcsicmp wcscmp
#define _finite finite
#define _tempnam tempnam
#define _unlink unlink
#define _access access
#define _mkdir(dir) mkdir( dir, S_IRWXU | S_IRWXG | S_IRWXO )
#define _wtoi(arg) wcstol(arg, NULL, 10)
#define _wtoi64(arg) wcstoll(arg, NULL, 10)

typedef uint32 HMODULE;
typedef void* HANDLE;
#endif

#ifndef _X360

static FORCEINLINE float fsel(float fComparand, float fValGE, float fLT)
{
	return fComparand >= 0 ? fValGE : fLT;
}
static FORCEINLINE double fsel(double fComparand, double fValGE, double fLT)
{
	return fComparand >= 0 ? fValGE : fLT;
}

#else

#define fsel __fsel

#endif


#if !defined( _X360 )
#if defined( _MSC_VER )

#if defined( PLATFORM_WINDOWS_PC64 )
inline void SetupFPUControlWord()
{
}
#else
inline void SetupFPUControlWordForceExceptions()
{
	uint16 tmpCtrlW;
	__asm
	{
		fnclex						     
		fnstcw word ptr[tmpCtrlW]	     
		and [tmpCtrlW], 0FCC0h		       
		or [tmpCtrlW], 0230h		        
		fldcw word ptr[tmpCtrlW]	       
	}
}

#ifdef CHECK_FLOAT_EXCEPTIONS

inline void SetupFPUControlWord()
{
	SetupFPUControlWordForceExceptions();
}

#else

inline void SetupFPUControlWord()
{
	uint16 tmpCtrlW;
	__asm
	{
		fnstcw word ptr[tmpCtrlW]	     
		and [tmpCtrlW], 0FCC0h		       
		or [tmpCtrlW], 023Fh		        
		fldcw word ptr[tmpCtrlW]	       
	}
}

#endif
#endif

#else

inline void SetupFPUControlWord()
{
	__volatile unsigned short int __cw;
	__asm __volatile("fnstcw %0" : "=m" (__cw));
	__cw = __cw & 0x0FCC0;	      
	__cw = __cw | 0x023F;	    
	__asm __volatile("fldcw %0" : : "m" (__cw));
}

#endif  

#else

#ifdef _DEBUG
FORCEINLINE bool IsFPUControlWordSet()
{
	float f = 0.996f;
	union
	{
		double flResult;
		int pResult[2];
	};
	flResult = __fctiw(f);
	return (pResult[1] == 1);
}
#endif

inline void SetupFPUControlWord()
{
	__emit(0xFF80010C);	   

	__vector4  a = { 0.0f, 0.0f, 0.0f, 0.0f };
	a;					  
	__asm
	{
		mtvscr a;	         
	}
}

#endif  

template <typename T>
inline T WordSwapC(T w)
{
	uint16 temp;

	temp = ((*((uint16*)&w) & 0xff00) >> 8);
	temp |= ((*((uint16*)&w) & 0x00ff) << 8);

	return *((T*)&temp);
}

template <typename T>
inline T DWordSwapC(T dw)
{
	uint32 temp;

	temp = *((uint32*)&dw) >> 24;
	temp |= ((*((uint32*)&dw) & 0x00FF0000) >> 8);
	temp |= ((*((uint32*)&dw) & 0x0000FF00) << 8);
	temp |= ((*((uint32*)&dw) & 0x000000FF) << 24);

	return *((T*)&temp);
}

template <typename T>
inline T QWordSwapC(T dw)
{
	PLAT_COMPILE_TIME_ASSERT(sizeof(dw) == sizeof(uint64));

	uint64 temp;

	temp = *((uint64*)&dw) >> 56;
	temp |= ((*((uint64*)&dw) & 0x00FF000000000000ull) >> 40);
	temp |= ((*((uint64*)&dw) & 0x0000FF0000000000ull) >> 24);
	temp |= ((*((uint64*)&dw) & 0x000000FF00000000ull) >> 8);
	temp |= ((*((uint64*)&dw) & 0x00000000FF000000ull) << 8);
	temp |= ((*((uint64*)&dw) & 0x0000000000FF0000ull) << 24);
	temp |= ((*((uint64*)&dw) & 0x000000000000FF00ull) << 40);
	temp |= ((*((uint64*)&dw) & 0x00000000000000FFull) << 56);

	return *((T*)&temp);
}

#if defined( _X360 )

#define WordSwap  WordSwap360Intr
#define DWordSwap DWordSwap360Intr

template <typename T>
inline T WordSwap360Intr(T w)
{
	T output;
	__storeshortbytereverse(w, 0, &output);
	return output;
}

template <typename T>
inline T DWordSwap360Intr(T dw)
{
	T output;
	__storewordbytereverse(dw, 0, &output);
	return output;
}

#elif defined( _MSC_VER ) && !defined( PLATFORM_WINDOWS_PC64 )

#define WordSwap  WordSwapAsm
#define DWordSwap DWordSwapAsm

#pragma warning(push)
#pragma warning (disable:4035)    

template <typename T>
inline T WordSwapAsm(T w)
{
	__asm
	{
		mov ax, w
		xchg al, ah
	}
}

template <typename T>
inline T DWordSwapAsm(T dw)
{
	__asm
	{
		mov eax, dw
		bswap eax
	}
}

#pragma warning(pop)

#else

#define WordSwap  WordSwapC
#define DWordSwap DWordSwapC

#endif

#define QWordSwap QWordSwapC

#if defined(__i386__) && !defined(VALVE_LITTLE_ENDIAN)
#define VALVE_LITTLE_ENDIAN 1
#endif

#if defined( _SGI_SOURCE ) || defined( _X360 )
#define	VALVE_BIG_ENDIAN 1
#endif

#define SafeSwapFloat( pOut, pIn )	(*((uint*)pOut) = DWordSwap( *((uint*)pIn) ))

#if defined(VALVE_LITTLE_ENDIAN)

#define BigShort( val )				WordSwap( val )
#define BigWord( val )				WordSwap( val )
#define BigLong( val )				DWordSwap( val )
#define BigDWord( val )				DWordSwap( val )
#define LittleShort( val )			( val )
#define LittleWord( val )			( val )
#define LittleLong( val )			( val )
#define LittleDWord( val )			( val )
#define LittleQWord( val )			( val )
#define SwapShort( val )			BigShort( val )
#define SwapWord( val )				BigWord( val )
#define SwapLong( val )				BigLong( val )
#define SwapDWord( val )			BigDWord( val )

#define BigFloat( pOut, pIn )		SafeSwapFloat( pOut, pIn )
#define LittleFloat( pOut, pIn )	( *pOut = *pIn )
#define SwapFloat( pOut, pIn )		BigFloat( pOut, pIn )

#elif defined(VALVE_BIG_ENDIAN)

#define BigShort( val )				( val )
#define BigWord( val )				( val )
#define BigLong( val )				( val )
#define BigDWord( val )				( val )
#define LittleShort( val )			WordSwap( val )
#define LittleWord( val )			WordSwap( val )
#define LittleLong( val )			DWordSwap( val )
#define LittleDWord( val )			DWordSwap( val )
#define LittleQWord( val )			QWordSwap( val )
#define SwapShort( val )			LittleShort( val )
#define SwapWord( val )				LittleWord( val )
#define SwapLong( val )				LittleLong( val )
#define SwapDWord( val )			LittleDWord( val )

#define BigFloat( pOut, pIn )		( *pOut = *pIn )
#define LittleFloat( pOut, pIn )	SafeSwapFloat( pOut, pIn )
#define SwapFloat( pOut, pIn )		LittleFloat( pOut, pIn )

#else

inline short BigShort(short val) { int test = 1; return (*(char*)&test == 1) ? WordSwap(val) : val; }
inline uint16 BigWord(uint16 val) { int test = 1; return (*(char*)&test == 1) ? WordSwap(val) : val; }
inline long BigLong(long val) { int test = 1; return (*(char*)&test == 1) ? DWordSwap(val) : val; }
inline uint32 BigDWord(uint32 val) { int test = 1; return (*(char*)&test == 1) ? DWordSwap(val) : val; }
inline short LittleShort(short val) { int test = 1; return (*(char*)&test == 1) ? val : WordSwap(val); }
inline uint16 LittleWord(uint16 val) { int test = 1; return (*(char*)&test == 1) ? val : WordSwap(val); }
inline long LittleLong(long val) { int test = 1; return (*(char*)&test == 1) ? val : DWordSwap(val); }
inline uint32 LittleDWord(uint32 val) { int test = 1; return (*(char*)&test == 1) ? val : DWordSwap(val); }
inline uint64 LittleQWord(uint64 val) { int test = 1; return (*(char*)&test == 1) ? val : QWordSwap(val); }
inline short SwapShort(short val) { return WordSwap(val); }
inline uint16 SwapWord(uint16 val) { return WordSwap(val); }
inline long SwapLong(long val) { return DWordSwap(val); }
inline uint32 SwapDWord(uint32 val) { return DWordSwap(val); }

inline void BigFloat(float* pOut, const float* pIn) { int test = 1; (*(char*)&test == 1) ? SafeSwapFloat(pOut, pIn) : (*pOut = *pIn); }
inline void LittleFloat(float* pOut, const float* pIn) { int test = 1; (*(char*)&test == 1) ? (*pOut = *pIn) : SafeSwapFloat(pOut, pIn); }
inline void SwapFloat(float* pOut, const float* pIn) { SafeSwapFloat(pOut, pIn); }

#endif

#if _X360
FORCEINLINE unsigned long LoadLittleDWord(const unsigned long* base, unsigned int dwordIndex)
{
	return __loadwordbytereverse(dwordIndex << 2, base);
}

FORCEINLINE void StoreLittleDWord(unsigned long* base, unsigned int dwordIndex, unsigned long dword)
{
	__storewordbytereverse(dword, dwordIndex << 2, base);
}
#else
FORCEINLINE unsigned long LoadLittleDWord(const unsigned long* base, unsigned int dwordIndex)
{
	return LittleDWord(base[dwordIndex]);
}

FORCEINLINE void StoreLittleDWord(unsigned long* base, unsigned int dwordIndex, unsigned long dword)
{
	base[dwordIndex] = LittleDWord(dword);
}
#endif


#ifndef STATIC_TIER0

#ifdef TIER0_DLL_EXPORT
#define PLATFORM_INTERFACE	DLL_EXPORT
#define PLATFORM_OVERLOAD	DLL_GLOBAL_EXPORT
#define PLATFORM_CLASS		DLL_CLASS_EXPORT
#else
#define PLATFORM_INTERFACE	DLL_IMPORT
#define PLATFORM_OVERLOAD	DLL_GLOBAL_IMPORT
#define PLATFORM_CLASS		DLL_CLASS_IMPORT
#endif

#else	 

#define PLATFORM_INTERFACE	extern
#define PLATFORM_OVERLOAD
#define PLATFORM_CLASS

#endif	 


PLATFORM_INTERFACE void				Plat_SetBenchmarkMode(bool bBenchmarkMode);
PLATFORM_INTERFACE bool				Plat_IsInBenchmarkMode();


PLATFORM_INTERFACE double			Plat_FloatTime();		         
PLATFORM_INTERFACE unsigned int		Plat_MSTime();			   
PLATFORM_INTERFACE char* Plat_asctime(const struct tm* tm, char* buf);
PLATFORM_INTERFACE char* Plat_ctime(const time_t* timep, char* buf, size_t bufsize);
PLATFORM_INTERFACE struct tm* Plat_gmtime(const time_t* timep, struct tm* result);
PLATFORM_INTERFACE time_t			Plat_timegm(struct tm* timeptr);
PLATFORM_INTERFACE struct tm* Plat_localtime(const time_t* timep, struct tm* result);

#if defined( _WIN32 ) && defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
extern "C" unsigned __int64 __rdtsc();
#pragma intrinsic(__rdtsc)
#endif

inline uint64 Plat_Rdtsc()
{
#if defined( _X360 )
	return (uint64)__mftb32();
#elif defined( _WIN64 )
	return (uint64)__rdtsc();
#elif defined( _WIN32 )
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	return (uint64)__rdtsc();
#else
	__asm rdtsc;
	__asm ret;
#endif
#elif defined( __i386__ )
	uint64 val;
	__asm__ __volatile__("rdtsc" : "=A" (val));
	return val;
#elif defined( __x86_64__ )
	uint32 lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return (((uint64)hi) << 32) | lo;
#else
#error
#endif
}

#define Sys_FloatTime Plat_FloatTime

#define DISALLOW_OPERATOR_EQUAL( _classname )			\
	private:											\
		_classname &operator=( const _classname & );	\
	public:

#define IMPLEMENT_OPERATOR_EQUAL( _classname )			\
	public:												\
		_classname &operator=( const _classname &src )	\
		{												\
			memcpy( this, &src, sizeof(_classname) );	\
			return *this;								\
		}

struct CPUInformation
{
	int	 m_Size;		       

	bool m_bRDTSC : 1,	   
		m_bCMOV : 1,     
		m_bFCMOV : 1,     
		m_bSSE : 1,	   
		m_bSSE2 : 1,	   
		m_b3DNow : 1,	   
		m_bMMX : 1,	   
		m_bHT : 1;	   

	uint8 m_nLogicalProcessors;		    
	uint8 m_nPhysicalProcessors;	    

	bool m_bSSE3 : 1,
		m_bSSSE3 : 1,
		m_bSSE4a : 1,
		m_bSSE41 : 1,
		m_bSSE42 : 1;

	int64 m_Speed;						    

	tchar* m_szProcessorID;				   

	CPUInformation() : m_Size(0) {}
};

PLATFORM_INTERFACE const CPUInformation* GetCPUInformation();

PLATFORM_INTERFACE float GetCPUUsage();

PLATFORM_INTERFACE void GetCurrentDate(int* pDay, int* pMonth, int* pYear);

PLATFORM_INTERFACE void InitPME();
PLATFORM_INTERFACE void ShutdownPME();

PLATFORM_INTERFACE void	Plat_SetHardwareDataBreakpoint(const void* pAddress, int nWatchBytes, bool bBreakOnRead);

PLATFORM_INTERFACE void	Plat_ApplyHardwareDataBreakpointsToNewThread(unsigned long dwThreadID);

PLATFORM_INTERFACE const tchar* Plat_GetCommandLine();
#ifndef _WIN32
PLATFORM_INTERFACE void Plat_SetCommandLine(const char* cmdLine);
#endif
PLATFORM_INTERFACE const char* Plat_GetCommandLineA();

PLATFORM_INTERFACE bool Plat_VerifyHardwareKeyDriver();

PLATFORM_INTERFACE bool Plat_VerifyHardwareKey();

PLATFORM_INTERFACE bool Plat_VerifyHardwareKeyPrompt();

PLATFORM_INTERFACE bool Plat_FastVerifyHardwareKey();

PLATFORM_INTERFACE void* Plat_SimpleLog(const tchar* file, int line);

#if _X360
#define Plat_FastMemset XMemSet
#define Plat_FastMemcpy XMemCpy
#else
#define Plat_FastMemset memset
#define Plat_FastMemcpy memcpy
#endif

#if defined(_WIN32) || defined(LINUX) || defined(OSX)
PLATFORM_INTERFACE bool Plat_IsInDebugSession(bool bForceRecheck = false);
PLATFORM_INTERFACE void Plat_DebugString(const char*);
#else
inline bool Plat_IsInDebugSession(bool bForceRecheck = false) { return false; }
#define Plat_DebugString(s) ((void)0)
#endif

PLATFORM_INTERFACE bool Is64BitOS();


#define XBOX_DVD_SECTORSIZE			2048
#define XBOX_DVD_ECC_SIZE			32768       
#define XBOX_HDD_SECTORSIZE			512

#define WM_XREMOTECOMMAND					(WM_USER + 100)
#define WM_XCONTROLLER_KEY					(WM_USER + 101)
#define WM_SYS_UI							(WM_USER + 102)
#define WM_SYS_SIGNINCHANGED				(WM_USER + 103)
#define WM_SYS_STORAGEDEVICESCHANGED		(WM_USER + 104)
#define WM_SYS_PROFILESETTINGCHANGED		(WM_USER + 105)
#define WM_SYS_MUTELISTCHANGED				(WM_USER + 106)
#define WM_SYS_INPUTDEVICESCHANGED			(WM_USER + 107)
#define WM_SYS_INPUTDEVICECONFIGCHANGED		(WM_USER + 108)
#define WM_LIVE_CONNECTIONCHANGED			(WM_USER + 109)
#define WM_LIVE_INVITE_ACCEPTED				(WM_USER + 110)
#define WM_LIVE_LINK_STATE_CHANGED			(WM_USER + 111)
#define WM_LIVE_CONTENT_INSTALLED			(WM_USER + 112)
#define WM_LIVE_MEMBERSHIP_PURCHASED		(WM_USER + 113)
#define WM_LIVE_VOICECHAT_AWAY				(WM_USER + 114)
#define WM_LIVE_PRESENCE_CHANGED			(WM_USER + 115)
#define WM_FRIENDS_PRESENCE_CHANGED			(WM_USER + 116)
#define WM_FRIENDS_FRIEND_ADDED				(WM_USER + 117)
#define WM_FRIENDS_FRIEND_REMOVED			(WM_USER + 118)
#define WM_CUSTOM_GAMEBANNERPRESSED			(WM_USER + 119)
#define WM_CUSTOM_ACTIONPRESSED				(WM_USER + 120)
#define WM_XMP_STATECHANGED					(WM_USER + 121)
#define WM_XMP_PLAYBACKBEHAVIORCHANGED		(WM_USER + 122)
#define WM_XMP_PLAYBACKCONTROLLERCHANGED	(WM_USER + 123)

inline const char* GetPlatformExt(void)
{
	return IsX360() ? ".360" : "";
}

#define XBOX_PROCESSOR_0			( 1<<0 )
#define XBOX_PROCESSOR_1			( 1<<1 )
#define XBOX_PROCESSOR_2			( 1<<2 )
#define XBOX_PROCESSOR_3			( 1<<3 )
#define XBOX_PROCESSOR_4			( 1<<4 )
#define XBOX_PROCESSOR_5			( 1<<5 )

#define XBOX_CORE_0_HWTHREAD_0		XBOX_PROCESSOR_0
#define XBOX_CORE_0_HWTHREAD_1		XBOX_PROCESSOR_1
#define XBOX_CORE_1_HWTHREAD_0		XBOX_PROCESSOR_2
#define XBOX_CORE_1_HWTHREAD_1		XBOX_PROCESSOR_3
#define XBOX_CORE_2_HWTHREAD_0		XBOX_PROCESSOR_4
#define XBOX_CORE_2_HWTHREAD_1		XBOX_PROCESSOR_5

#include "fasttimer.h"

#if defined( _X360 )
#include "xbox/xbox_core.h"
#endif

template <class T>
inline T* Construct(T* pMemory)
{
	return ::new(pMemory) T;
}

template <class T, typename ARG1>
inline T* Construct(T* pMemory, ARG1 a1)
{
	return ::new(pMemory) T(a1);
}

template <class T, typename ARG1, typename ARG2>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2)
{
	return ::new(pMemory) T(a1, a2);
}

template <class T, typename ARG1, typename ARG2, typename ARG3>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2, ARG3 a3)
{
	return ::new(pMemory) T(a1, a2, a3);
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4)
{
	return ::new(pMemory) T(a1, a2, a3, a4);
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4, ARG5 a5)
{
	return ::new(pMemory) T(a1, a2, a3, a4, a5);
}

template <class T, class P>
inline void ConstructOneArg(T* pMemory, P const& arg)
{
	::new(pMemory) T(arg);
}

template <class T, class P1, class P2 >
inline void ConstructTwoArg(T* pMemory, P1 const& arg1, P2 const& arg2)
{
	::new(pMemory) T(arg1, arg2);
}

template <class T, class P1, class P2, class P3 >
inline void ConstructThreeArg(T* pMemory, P1 const& arg1, P2 const& arg2, P3 const& arg3)
{
	::new(pMemory) T(arg1, arg2, arg3);
}

template <class T>
inline T* CopyConstruct(T* pMemory, T const& src)
{
	return ::new(pMemory) T(src);
}

template <class T>
inline void Destruct(T* pMemory)
{
	pMemory->~T();

#ifdef _DEBUG
	memset(reinterpret_cast<void*>(pMemory), 0xDD, sizeof(T));
#endif
}


#define GET_OUTER( OuterType, OuterMember ) \
   ( ( OuterType * ) ( (uint8 *)this - offsetof( OuterType, OuterMember ) ) )


PLATFORM_INTERFACE bool vtune(bool resume);


#define TEMPLATE_FUNCTION_TABLE(RETURN_TYPE, NAME, ARGS, COUNT)			\
																		\
typedef RETURN_TYPE (FASTCALL *__Type_##NAME) ARGS;						\
																		\
template<const int nArgument>											\
struct __Function_##NAME												\
{																		\
	static RETURN_TYPE FASTCALL Run ARGS;								\
};																		\
																		\
template <const int i>														\
struct __MetaLooper_##NAME : __MetaLooper_##NAME<i-1>					\
{																		\
	__Type_##NAME func;													\
	inline __MetaLooper_##NAME() { func = __Function_##NAME<i>::Run; }	\
};																		\
																		\
template<>																\
struct __MetaLooper_##NAME<0>											\
{																		\
	__Type_##NAME func;													\
	inline __MetaLooper_##NAME() { func = __Function_##NAME<0>::Run; }	\
};																		\
																		\
class NAME																\
{																		\
private:																\
    static const __MetaLooper_##NAME<COUNT> m;							\
public:																	\
	enum { count = COUNT };												\
	static const __Type_##NAME* functions;								\
};																		\
const __MetaLooper_##NAME<COUNT> NAME::m;								\
const __Type_##NAME* NAME::functions = (__Type_##NAME*)&m;				\
template<const int nArgument>													\
RETURN_TYPE FASTCALL __Function_##NAME<nArgument>::Run ARGS


#define LOOP_INTERCHANGE(BOOLEAN, CODE)\
	if( (BOOLEAN) )\
	{\
		CODE;\
	} else\
	{\
		CODE;\
	}

#if 0    

PLATFORM_INTERFACE void* Plat_GetProcAddress(const char* pszModule, const char* pszName);

template <typename FUNCPTR_TYPE>
class CDynamicFunction
{
public:
	CDynamicFunction(const char* pszModule, const char* pszName, FUNCPTR_TYPE pfnFallback = NULL)
	{
		m_pfn = pfnFallback;
		void* pAddr = Plat_GetProcAddress(pszModule, pszName);
		if (pAddr)
		{
			m_pfn = (FUNCPTR_TYPE)pAddr;
		}
	}

	operator bool() { return m_pfn != NULL; }
	bool operator !() { return !m_pfn; }
	operator FUNCPTR_TYPE() { return m_pfn; }

private:
	FUNCPTR_TYPE m_pfn;
};
#endif


#include "valve_on.h"

#if defined(TIER0_DLL_EXPORT)
extern "C" int V_tier0_stricmp(const char* s1, const char* s2);
#undef stricmp
#undef strcmpi
#define stricmp(s1,s2) V_tier0_stricmp( s1, s2 )
#define strcmpi(s1,s2) V_tier0_stricmp( s1, s2 )
#endif


#endif   