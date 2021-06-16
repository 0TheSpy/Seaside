#ifndef VCRMODE_H
#define VCRMODE_H

#ifdef _WIN32
#include <process.h>
#endif

#ifdef _WIN32
#pragma once
#endif

#include "platform.h"
#define VCRFILE_VERSION		2


typedef enum
{
	VCREvent_Sys_FloatTime = 0,
	VCREvent_recvfrom,
	VCREvent_SyncToken,
	VCREvent_GetCursorPos,
	VCREvent_SetCursorPos,
	VCREvent_ScreenToClient,
	VCREvent_Cmd_Exec,
	VCREvent_CmdLine,
	VCREvent_RegOpenKeyEx,
	VCREvent_RegSetValueEx,
	VCREvent_RegQueryValueEx,
	VCREvent_RegCreateKeyEx,
	VCREvent_RegCloseKey,
	VCREvent_PeekMessage,
	VCREvent_GameMsg,
	VCREvent_GetNumberOfConsoleInputEvents,
	VCREvent_ReadConsoleInput,
	VCREvent_GetKeyState,
	VCREvent_recv,
	VCREvent_send,
	VCREvent_Generic,
	VCREvent_CreateThread,
	VCREvent_WaitForSingleObject,
	VCREvent_EnterCriticalSection,
	VCREvent_Time,
	VCREvent_LocalTime,
	VCREvent_GenericString,
	VCREvent_NUMEVENTS
} VCREvent;

#include "dbg.h"

#ifdef POSIX
DBG_INTERFACE const char* BuildCmdLine(int argc, char** argv, bool fAddSteam = true);
tchar* GetCommandLine();
#endif

#ifdef _X360
#define NO_VCR 1
#endif


#ifndef NO_VCR
#define NOVCR(x)	\
{\
	VCRSetEnabled(0);\
	x;\
	VCRSetEnabled(1);\
}
#else
#define NOVCR(x)	\
{\
	x;\
}
#endif


struct InputEvent_t;


enum VCRMode_t
{
	VCR_Invalid = -1,
	VCR_Disabled = 0,
	VCR_Record,
	VCR_Playback
};


abstract_class IVCRHelpers
{
public:
	virtual void	ErrorMessage(const tchar * pMsg) = 0;
	virtual void* GetMainWindow() = 0;
};


abstract_class IVCRTrace
{
public:
	virtual VCREvent	ReadEvent() = 0;
	virtual void		Read(void* pDest, int size) = 0;
};

typedef struct VCR_s
{
	int			(*Start)(tchar const* pFilename, bool bRecord, IVCRHelpers* pHelpers);
	void		(*End)();

	IVCRTrace* (*GetVCRTraceInterface)();

	VCRMode_t(*GetMode)();

	void		(*SetEnabled)(int bEnabled);

	void		(*SyncToken)(tchar const* pToken);

	double		(*Hook_Sys_FloatTime)(double time);

	int			(*Hook_PeekMessage)(
		struct tagMSG* msg,
		void* hWnd,
		unsigned int wMsgFilterMin,
		unsigned int wMsgFilterMax,
		unsigned int wRemoveMsg
		);

	void		(*Hook_RecordGameMsg)(const InputEvent_t& event);
	void		(*Hook_RecordEndGameMsg)();

	bool		(*Hook_PlaybackGameMsg)(InputEvent_t* pEvent);

	int			(*Hook_recvfrom)(int s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);

	void		(*Hook_GetCursorPos)(struct tagPOINT* pt);
	void		(*Hook_ScreenToClient)(void* hWnd, struct tagPOINT* pt);

	void		(*Hook_Cmd_Exec)(tchar** f);

	tchar* (*Hook_GetCommandLine)();

	long		(*Hook_RegOpenKeyEx)(void* hKey, const tchar* lpSubKey, unsigned long ulOptions, unsigned long samDesired, void* pHKey);
	long		(*Hook_RegSetValueEx)(void* hKey, tchar const* lpValueName, unsigned long Reserved, unsigned long dwType, uint8 const* lpData, unsigned long cbData);
	long		(*Hook_RegQueryValueEx)(void* hKey, tchar const* lpValueName, unsigned long* lpReserved, unsigned long* lpType, uint8* lpData, unsigned long* lpcbData);
	long		(*Hook_RegCreateKeyEx)(void* hKey, tchar const* lpSubKey, unsigned long Reserved, tchar* lpClass, unsigned long dwOptions, unsigned long samDesired, void* lpSecurityAttributes, void* phkResult, unsigned long* lpdwDisposition);
	void		(*Hook_RegCloseKey)(void* hKey);

	int			(*Hook_GetNumberOfConsoleInputEvents)(void* hInput, unsigned long* pNumEvents);

	int			(*Hook_ReadConsoleInput)(void* hInput, void* pRecs, int nMaxRecs, unsigned long* pNumRead);


	void		(*Hook_LocalTime)(struct tm* today);

	short		(*Hook_GetKeyState)(int nVirtKey);

	int			(*Hook_recv)(int s, char* buf, int len, int flags);
	int			(*Hook_send)(int s, const char* buf, int len, int flags);

	void		(*GenericRecord)(const tchar* pEventName, const void* pData, int len);


	int			(*GenericPlayback)(const tchar* pEventName, void* pOutData, int maxLen, bool bForceLenSame);

	void		(*GenericValue)(const tchar* pEventName, void* pData, int maxLen);

	double		(*GetPercentCompleted)();

	void* (*Hook_CreateThread)(
		void* lpThreadAttributes,
		unsigned long dwStackSize,
		void* lpStartAddress,
		void* lpParameter,
		unsigned long dwCreationFlags,
		unsigned long* lpThreadID);

	unsigned long (*Hook_WaitForSingleObject)(
		void* handle,
		unsigned long dwMilliseconds);

	void		(*Hook_EnterCriticalSection)(void* pCS);

	void		(*Hook_Time)(long* pTime);

	void		(*GenericString)(const char* pEventName, const char* pString);

	void		(*GenericValueVerify)(const tchar* pEventName, const void* pData, int maxLen);

	unsigned long (*Hook_WaitForMultipleObjects)(uint32 nHandles, const void** pHandles, int bWaitAll, uint32 timeout);

} VCR_t;

#ifndef NO_VCR

PLATFORM_INTERFACE VCR_t* g_pVCR;

#endif


#ifndef NO_VCR
#define VCRStart								g_pVCR->Start
#define VCREnd									g_pVCR->End
#define VCRGetVCRTraceInterface					g_pVCR->GetVCRTraceInterface
#define VCRGetMode								g_pVCR->GetMode
#define VCRSetEnabled							g_pVCR->SetEnabled
#define VCRSyncToken							g_pVCR->SyncToken
#define VCRGenericString						g_pVCR->GenericString
#define VCRGenericValueVerify					g_pVCR->GenericValueVerify
#define VCRHook_Sys_FloatTime					g_pVCR->Hook_Sys_FloatTime
#define VCRHook_PeekMessage						g_pVCR->Hook_PeekMessage
#define VCRHook_RecordGameMsg					g_pVCR->Hook_RecordGameMsg
#define VCRHook_RecordEndGameMsg				g_pVCR->Hook_RecordEndGameMsg
#define VCRHook_PlaybackGameMsg					g_pVCR->Hook_PlaybackGameMsg
#define VCRHook_recvfrom						g_pVCR->Hook_recvfrom
#define VCRHook_GetCursorPos					g_pVCR->Hook_GetCursorPos
#define VCRHook_ScreenToClient					g_pVCR->Hook_ScreenToClient
#define VCRHook_Cmd_Exec						g_pVCR->Hook_Cmd_Exec
#define VCRHook_GetCommandLine					g_pVCR->Hook_GetCommandLine
#define VCRHook_RegOpenKeyEx					g_pVCR->Hook_RegOpenKeyEx
#define VCRHook_RegSetValueEx					g_pVCR->Hook_RegSetValueEx
#define VCRHook_RegQueryValueEx					g_pVCR->Hook_RegQueryValueEx
#define VCRHook_RegCreateKeyEx					g_pVCR->Hook_RegCreateKeyEx
#define VCRHook_RegCloseKey						g_pVCR->Hook_RegCloseKey
#define VCRHook_GetNumberOfConsoleInputEvents	g_pVCR->Hook_GetNumberOfConsoleInputEvents
#define VCRHook_ReadConsoleInput				g_pVCR->Hook_ReadConsoleInput
#define VCRHook_LocalTime						g_pVCR->Hook_LocalTime
#define VCRHook_GetKeyState						g_pVCR->Hook_GetKeyState
#define VCRHook_recv							g_pVCR->Hook_recv
#define VCRHook_send							g_pVCR->Hook_send
#define VCRGenericRecord						g_pVCR->GenericRecord
#define VCRGenericPlayback						g_pVCR->GenericPlayback
#define VCRGenericValue							g_pVCR->GenericValue
#define VCRGetPercentCompleted					g_pVCR->GetPercentCompleted
#define VCRHook_CreateThread					g_pVCR->Hook_CreateThread
#define VCRHook_WaitForSingleObject				g_pVCR->Hook_WaitForSingleObject
#define VCRHook_EnterCriticalSection			g_pVCR->Hook_EnterCriticalSection
#define VCRHook_Time							g_pVCR->Hook_Time
#define VCRHook_WaitForMultipleObjects( a, b, c, d) g_pVCR->Hook_WaitForMultipleObjects( a, (const void **)b, c, d)
#else
#define VCRStart( a, b, c )						(1)
#define VCREnd									((void)(0))
#define VCRGetVCRTraceInterface					(NULL)
#define VCRGetMode()							(VCR_Disabled)
#define VCRSetEnabled( a )						((void)(0))
#define VCRSyncToken( a )						((void)(0))
#define VCRGenericRecord						MUST_IFDEF_OUT_GenericRecord
#define VCRGenericPlayback						MUST_IFDEF_OUT_GenericPlayback
#define VCRGenericValue							MUST_IFDEF_OUT_GenericValue
#define VCRGenericString						MUST_IFDEF_OUT_GenericString
#define VCRGenericValueVerify					MUST_IFDEF_OUT_GenericValueVerify
#define VCRGetPercentCompleted()				(0.0f)
#define VCRHook_Sys_FloatTime					Sys_FloatTime
#define VCRHook_PeekMessage						PeekMessage
#define VCRHook_RecordGameMsg					RecordGameMsg
#define VCRHook_RecordEndGameMsg				RecordEndGameMsg
#define VCRHook_PlaybackGameMsg					PlaybackGameMsg
#define VCRHook_recvfrom						recvfrom
#define VCRHook_GetCursorPos					GetCursorPos
#define VCRHook_ScreenToClient					ScreenToClient
#define VCRHook_Cmd_Exec( a )					((void)(0))
#define VCRHook_GetCommandLine					GetCommandLine
#define VCRHook_RegOpenKeyEx					RegOpenKeyEx
#define VCRHook_RegSetValueEx					RegSetValueEx
#define VCRHook_RegQueryValueEx					RegQueryValueEx
#define VCRHook_RegCreateKeyEx					RegCreateKeyEx
#define VCRHook_RegCloseKey						RegCloseKey
#define VCRHook_GetNumberOfConsoleInputEvents	GetNumberOfConsoleInputEvents
#define VCRHook_ReadConsoleInput				ReadConsoleInput
#define VCRHook_LocalTime( a )					memset(a, 0, sizeof(*a));
#define VCRHook_GetKeyState						GetKeyState
#define VCRHook_recv							recv
#define VCRHook_send							send
#if defined( _X360 )
#define VCRHook_CreateThread					CreateThread
#else
#define VCRHook_CreateThread					(void*)_beginthreadex
#endif
#define VCRHook_WaitForSingleObject				WaitForSingleObject
#define VCRHook_EnterCriticalSection			EnterCriticalSection
#define VCRHook_WaitForMultipleObjects( a, b, c, d) WaitForMultipleObjects( a, (const HANDLE *)b, c, d)
#define VCRHook_Time							Time
#endif

#endif  