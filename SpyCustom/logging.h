#ifndef LOGGING_H
#define LOGGING_H

#if !defined(__SPU__)

#if defined( COMPILER_MSVC )
#pragma once
#endif

#include "color.h"
#include "icommandline.h"
#include <stdio.h>

#if defined( _X360 )
#include "xbox/xbox_console.h"
#endif

#if defined( _WIN32 ) || (defined(POSIX) && !defined(_GAMECONSOLE))
#include "win32consoleio.h"
#endif

class CLoggingSystem;
class CThreadFastMutex;

const int MAX_LOGGING_MESSAGE_LENGTH = 2048;

const int MAX_LOGGING_IDENTIFIER_LENGTH = 32;

const int MAX_LOGGING_CHANNEL_COUNT = 256;

const int MAX_LOGGING_TAG_COUNT = 1024;

const int MAX_LOGGING_TAG_CHARACTER_COUNT = 8192;

const int MAX_LOGGING_LISTENER_COUNT = 16;

const Color UNSPECIFIED_LOGGING_COLOR(0, 0, 0, 0);

typedef int LoggingChannelID_t;

const LoggingChannelID_t INVALID_LOGGING_CHANNEL_ID = -1;

enum LoggingSeverity_t
{
	LS_MESSAGE = 0,

	LS_WARNING = 1,

	LS_ASSERT = 2,

	LS_ERROR = 3,

	LS_HIGHEST_SEVERITY = 4,
};

enum LoggingResponse_t
{
	LR_CONTINUE,
	LR_DEBUGGER,
	LR_ABORT,
};

enum LoggingChannelFlags_t
{
	LCF_CONSOLE_ONLY = 0x00000001,

	LCF_DO_NOT_ECHO = 0x00000002,
};

typedef void (*RegisterTagsFunc)();

struct LoggingContext_t
{
	LoggingChannelID_t m_ChannelID;
	LoggingChannelFlags_t m_Flags;
	LoggingSeverity_t m_Severity;
	Color m_Color;
};

class ILoggingListener
{
public:
	virtual void Log(const LoggingContext_t* pContext, const tchar* pMessage) = 0;
};

class ILoggingResponsePolicy
{
public:
	virtual LoggingResponse_t OnLog(const LoggingContext_t* pContext) = 0;
};

class CSimpleLoggingListener : public ILoggingListener
{
public:
	CSimpleLoggingListener(bool bQuietPrintf = false, bool bQuietDebugger = false) :
		m_bQuietPrintf(bQuietPrintf),
		m_bQuietDebugger(bQuietDebugger)
	{
	}

	virtual void Log(const LoggingContext_t* pContext, const tchar* pMessage)
	{
#ifdef _X360
		if (!m_bQuietDebugger && XBX_IsConsoleConnected())
		{
			XBX_DebugString(XMAKECOLOR(0, 0, 0), pMessage);
		}
		else
#endif
		{
#if !defined( _CERT ) && !defined( DBGFLAG_STRINGS_STRIP )
			if (!m_bQuietPrintf)
			{
				_tprintf(_T("%s"), pMessage);
			}
#endif

#ifdef _WIN32
			if (!m_bQuietDebugger && Plat_IsInDebugSession())
			{
				Plat_DebugString(pMessage);
			}
#endif
		}
	}

	bool m_bQuietPrintf;
	bool m_bQuietDebugger;
};

class CSimpleWindowsLoggingListener : public ILoggingListener
{
public:
	virtual void Log(const LoggingContext_t* pContext, const tchar* pMessage)
	{
		if (Plat_IsInDebugSession())
		{
			Plat_DebugString(pMessage);
		}
		if (pContext->m_Severity == LS_ERROR)
		{
			if (Plat_IsInDebugSession())
				DebuggerBreak();

			Plat_MessageBox("Error", pMessage);
		}
	}
};


#if !defined(_GAMECONSOLE)
class CColorizedLoggingListener : public CSimpleLoggingListener
{
public:
	CColorizedLoggingListener(bool bQuietPrintf = false, bool bQuietDebugger = false) : CSimpleLoggingListener(bQuietPrintf, bQuietDebugger)
	{
		InitWin32ConsoleColorContext(&m_ColorContext);
	}

	virtual void Log(const LoggingContext_t* pContext, const tchar* pMessage)
	{
		if (!m_bQuietPrintf)
		{
			int nPrevColor = -1;

			if (pContext->m_Color != UNSPECIFIED_LOGGING_COLOR)
			{
				nPrevColor = SetWin32ConsoleColor(&m_ColorContext,
					pContext->m_Color.r(), pContext->m_Color.g(), pContext->m_Color.b(),
					MAX(MAX(pContext->m_Color.r(), pContext->m_Color.g()), pContext->m_Color.b()) > 128);
			}

			_tprintf(_T("%s"), pMessage);

			if (nPrevColor >= 0)
			{
				RestoreWin32ConsoleColor(&m_ColorContext, nPrevColor);
			}
		}

#ifdef _WIN32
		if (!m_bQuietDebugger && Plat_IsInDebugSession())
		{
			Plat_DebugString(pMessage);
		}
#endif
	}

	Win32ConsoleColorContext_t m_ColorContext;
};
#endif  


class CDefaultLoggingResponsePolicy : public ILoggingResponsePolicy
{
public:
	virtual LoggingResponse_t OnLog(const LoggingContext_t* pContext)
	{
		if (pContext->m_Severity == LS_ASSERT && !CommandLine()->FindParm("-noassert"))
		{
			return LR_DEBUGGER;
		}
		else if (pContext->m_Severity == LS_ERROR)
		{
			return LR_ABORT;
		}
		else
		{
			return LR_CONTINUE;
		}
	}
};

class CNonFatalLoggingResponsePolicy : public ILoggingResponsePolicy
{
public:
	virtual LoggingResponse_t OnLog(const LoggingContext_t* pContext)
	{
		if ((pContext->m_Severity == LS_ASSERT && !CommandLine()->FindParm("-noassert")) || pContext->m_Severity == LS_ERROR)
		{
			return LR_DEBUGGER;
		}
		else
		{
			return LR_CONTINUE;
		}
	}
};

class CLoggingSystem
{
public:
	struct LoggingChannel_t;

	CLoggingSystem();
	~CLoggingSystem();

	LoggingChannelID_t RegisterLoggingChannel(const char* pChannelName, RegisterTagsFunc registerTagsFunc, int flags = 0, LoggingSeverity_t minimumSeverity = LS_MESSAGE, Color spewColor = UNSPECIFIED_LOGGING_COLOR);

	LoggingChannelID_t FindChannel(const char* pChannelName) const;

	int GetChannelCount() const { return m_nChannelCount; }

	LoggingChannel_t* GetChannel(LoggingChannelID_t channelID);
	const LoggingChannel_t* GetChannel(LoggingChannelID_t channelID) const;

	bool HasTag(LoggingChannelID_t channelID, const char* pTag) const { return GetChannel(channelID)->HasTag(pTag); }

	bool IsValidChannelID(LoggingChannelID_t channelID) const { return (channelID >= 0) && (channelID < m_nChannelCount); }

	bool IsChannelEnabled(LoggingChannelID_t channelID, LoggingSeverity_t severity) const { return IsValidChannelID(channelID) && GetChannel(channelID)->IsEnabled(severity); }

	void SetChannelSpewLevel(LoggingChannelID_t channelID, LoggingSeverity_t minimumSeverity);
	void SetChannelSpewLevelByName(const char* pName, LoggingSeverity_t minimumSeverity);
	void SetChannelSpewLevelByTag(const char* pTag, LoggingSeverity_t minimumSeverity);
	void SetGlobalSpewLevel(LoggingSeverity_t minimumSeverity);

	Color GetChannelColor(LoggingChannelID_t channelID) const { return GetChannel(channelID)->m_SpewColor; }
	void SetChannelColor(LoggingChannelID_t channelID, Color color) { GetChannel(channelID)->m_SpewColor = color; }

	LoggingChannelFlags_t GetChannelFlags(LoggingChannelID_t channelID) const { return GetChannel(channelID)->m_Flags; }
	void SetChannelFlags(LoggingChannelID_t channelID, LoggingChannelFlags_t flags) { GetChannel(channelID)->m_Flags = flags; }

	void AddTagToCurrentChannel(const char* pTagName);

	void PushLoggingState(bool bThreadLocal = false, bool bClearState = true);
	void PopLoggingState(bool bThreadLocal = false);

	void RegisterLoggingListener(ILoggingListener* pListener);

	bool IsListenerRegistered(ILoggingListener* pListener);

	void ResetCurrentLoggingState();

	void SetLoggingResponsePolicy(ILoggingResponsePolicy* pLoggingResponse);

	LoggingResponse_t LogDirect(LoggingChannelID_t channelID, LoggingSeverity_t severity, Color color, const tchar* pMessage);

	struct LoggingTag_t
	{
		const char* m_pTagName;
		LoggingTag_t* m_pNextTag;
	};

	struct LoggingChannel_t
	{
		bool HasTag(const char* pTag) const
		{
			LoggingTag_t* pCurrentTag = m_pFirstTag;
			while (pCurrentTag != NULL)
			{
				if (stricmp(pCurrentTag->m_pTagName, pTag) == 0)
				{
					return true;
				}
				pCurrentTag = pCurrentTag->m_pNextTag;
			}
			return false;
		}
		bool IsEnabled(LoggingSeverity_t severity) const { return severity >= m_MinimumSeverity; }
		void SetSpewLevel(LoggingSeverity_t severity) { m_MinimumSeverity = severity; }

		LoggingChannelID_t m_ID;
		LoggingChannelFlags_t m_Flags;      
		LoggingSeverity_t m_MinimumSeverity;          
		Color m_SpewColor;
		char m_Name[MAX_LOGGING_IDENTIFIER_LENGTH];
		LoggingTag_t* m_pFirstTag;
	};

private:
	struct LoggingState_t
	{
		int m_nPreviousStackEntry;

		int m_nListenerCount;
		ILoggingListener* m_RegisteredListeners[MAX_LOGGING_LISTENER_COUNT];

		ILoggingResponsePolicy* m_pLoggingResponse;
	};

	LoggingState_t* GetCurrentState();
	const LoggingState_t* GetCurrentState() const;

	int FindUnusedStateIndex();
	LoggingTag_t* AllocTag(const char* pTagName);

	int m_nChannelCount;
	LoggingChannel_t m_RegisteredChannels[MAX_LOGGING_CHANNEL_COUNT];

	int m_nChannelTagCount;
	LoggingTag_t m_ChannelTags[MAX_LOGGING_TAG_COUNT];

	int m_nTagNamePoolIndex;
	char m_TagNamePool[MAX_LOGGING_TAG_CHARACTER_COUNT];

	CThreadFastMutex* m_pStateMutex;

	int m_nGlobalStateIndex;

	static const int MAX_LOGGING_STATE_COUNT = 16;
	LoggingState_t m_LoggingStates[MAX_LOGGING_STATE_COUNT];

	CDefaultLoggingResponsePolicy m_DefaultLoggingResponse;

	CSimpleLoggingListener m_DefaultLoggingListener;

};

#ifdef DBGFLAG_STRINGS_STRIP
#define InternalMsg( Channel, Severity,     ... ) do { if ( Severity == LS_ERROR && LoggingSystem_IsChannelEnabled( Channel, Severity ) ) LoggingSystem_Log( Channel, Severity,     ##__VA_ARGS__ ); } while( 0 )
#else
#define InternalMsg( Channel, Severity,     ... ) do { if ( LoggingSystem_IsChannelEnabled( Channel, Severity ) ) LoggingSystem_Log( Channel, Severity,     ##__VA_ARGS__ ); } while( 0 )
#endif

#define Log_Msg( Channel,     ... ) InternalMsg( Channel, LS_MESSAGE,     ##__VA_ARGS__ )
#define Log_Warning( Channel,     ... ) InternalMsg( Channel, LS_WARNING,     ##__VA_ARGS__ )
#define Log_Error( Channel,     ... ) InternalMsg( Channel, LS_ERROR,     ##__VA_ARGS__ )
#ifdef DBGFLAG_STRINGS_STRIP
#define Log_Assert( ... ) LR_CONTINUE
#else
#define Log_Assert( Message, ... ) LoggingSystem_LogAssert( Message, ##__VA_ARGS__ )
#endif


#define DECLARE_LOGGING_CHANNEL( Channel ) extern LoggingChannelID_t Channel

#define DEFINE_LOGGING_CHANNEL_NO_TAGS( Channel, ChannelName,      ... ) \
	LoggingChannelID_t Channel = LoggingSystem_RegisterLoggingChannel( ChannelName, NULL, ##__VA_ARGS__ )

#define BEGIN_DEFINE_LOGGING_CHANNEL( Channel, ChannelName,      ... ) \
	static void Register_##Channel##_Tags(); \
	LoggingChannelID_t Channel = LoggingSystem_RegisterLoggingChannel( ChannelName, Register_##Channel##_Tags, ##__VA_ARGS__ ); \
	void Register_##Channel##_Tags() \
	{

#define ADD_LOGGING_CHANNEL_TAG( Tag ) LoggingSystem_AddTagToCurrentChannel( Tag )

#define END_DEFINE_LOGGING_CHANNEL() \
	}

PLATFORM_INTERFACE LoggingChannelID_t LoggingSystem_RegisterLoggingChannel(const char* pName, RegisterTagsFunc registerTagsFunc, int flags = 0, LoggingSeverity_t severity = LS_MESSAGE, Color color = UNSPECIFIED_LOGGING_COLOR);

PLATFORM_INTERFACE void LoggingSystem_RegisterLoggingListener(ILoggingListener* pListener);
PLATFORM_INTERFACE void LoggingSystem_UnregisterLoggingListener(ILoggingListener* pListener);
PLATFORM_INTERFACE void LoggingSystem_ResetCurrentLoggingState();
PLATFORM_INTERFACE void LoggingSystem_SetLoggingResponsePolicy(ILoggingResponsePolicy* pResponsePolicy);
PLATFORM_INTERFACE void LoggingSystem_PushLoggingState(bool bThreadLocal = false, bool bClearState = true);
PLATFORM_INTERFACE void LoggingSystem_PopLoggingState(bool bThreadLocal = false);

PLATFORM_INTERFACE void LoggingSystem_AddTagToCurrentChannel(const char* pTagName);

PLATFORM_INTERFACE LoggingChannelID_t LoggingSystem_FindChannel(const char* pChannelName);
PLATFORM_INTERFACE int LoggingSystem_GetChannelCount();
PLATFORM_INTERFACE LoggingChannelID_t LoggingSystem_GetFirstChannelID();
PLATFORM_INTERFACE LoggingChannelID_t LoggingSystem_GetNextChannelID(LoggingChannelID_t channelID);
PLATFORM_INTERFACE const CLoggingSystem::LoggingChannel_t* LoggingSystem_GetChannel(LoggingChannelID_t channelID);

PLATFORM_INTERFACE bool LoggingSystem_HasTag(LoggingChannelID_t channelID, const char* pTag);

PLATFORM_INTERFACE bool LoggingSystem_IsChannelEnabled(LoggingChannelID_t channelID, LoggingSeverity_t severity);
PLATFORM_INTERFACE void LoggingSystem_SetChannelSpewLevel(LoggingChannelID_t channelID, LoggingSeverity_t minimumSeverity);
PLATFORM_INTERFACE void LoggingSystem_SetChannelSpewLevelByName(const char* pName, LoggingSeverity_t minimumSeverity);
PLATFORM_INTERFACE void LoggingSystem_SetChannelSpewLevelByTag(const char* pTag, LoggingSeverity_t minimumSeverity);
PLATFORM_INTERFACE void LoggingSystem_SetGlobalSpewLevel(LoggingSeverity_t minimumSeverity);

PLATFORM_INTERFACE int32 LoggingSystem_GetChannelColor(LoggingChannelID_t channelID);
PLATFORM_INTERFACE void LoggingSystem_SetChannelColor(LoggingChannelID_t channelID, int color);

PLATFORM_INTERFACE LoggingChannelFlags_t LoggingSystem_GetChannelFlags(LoggingChannelID_t channelID);
PLATFORM_INTERFACE void LoggingSystem_SetChannelFlags(LoggingChannelID_t channelID, LoggingChannelFlags_t flags);

PLATFORM_INTERFACE LoggingResponse_t LoggingSystem_Log(LoggingChannelID_t channelID, LoggingSeverity_t severity, PRINTF_FORMAT_STRING const char* pMessageFormat, ...) FMTFUNCTION(3, 4);
PLATFORM_OVERLOAD LoggingResponse_t LoggingSystem_Log(LoggingChannelID_t channelID, LoggingSeverity_t severity, Color spewColor, PRINTF_FORMAT_STRING const char* pMessageFormat, ...) FMTFUNCTION(4, 5);

PLATFORM_INTERFACE LoggingResponse_t LoggingSystem_LogDirect(LoggingChannelID_t channelID, LoggingSeverity_t severity, Color spewColor, const char* pMessage);
PLATFORM_INTERFACE LoggingResponse_t LoggingSystem_LogAssert(PRINTF_FORMAT_STRING const char* pMessageFormat, ...) FMTFUNCTION(1, 2);

#endif  

#endif  