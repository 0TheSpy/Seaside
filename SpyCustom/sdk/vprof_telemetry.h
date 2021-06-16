#ifndef VPROF_TELEMETRY_H
#define VPROF_TELEMETRY_H

#if !defined( MAKE_VPC )

#if !defined( RAD_TELEMETRY_DISABLED ) && ( defined( IS_WINDOWS_PC ) || defined( _LINUX ) )
#endif


#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#endif    


enum TelemetryZonePlotSlot_t
{
	TELEMETRY_ZONE_PLOT_SLOT_1,
	TELEMETRY_ZONE_PLOT_SLOT_2,
	TELEMETRY_ZONE_PLOT_SLOT_3,
	TELEMETRY_ZONE_PLOT_SLOT_4,
	TELEMETRY_ZONE_PLOT_SLOT_5,
	TELEMETRY_ZONE_PLOT_SLOT_6,
	TELEMETRY_ZONE_PLOT_SLOT_7,
	TELEMETRY_ZONE_PLOT_SLOT_8,
	TELEMETRY_ZONE_PLOT_SLOT_9,
	TELEMETRY_ZONE_PLOT_SLOT_10,
	TELEMETRY_ZONE_PLOT_SLOT_11,
	TELEMETRY_ZONE_PLOT_SLOT_12,
	TELEMETRY_ZONE_PLOT_SLOT_13,
	TELEMETRY_ZONE_PLOT_SLOT_14,
	TELEMETRY_ZONE_PLOT_SLOT_15,
	TELEMETRY_ZONE_PLOT_SLOT_16,

	TELEMETRY_ZONE_PLOT_SLOT_MAX
};


#if !defined( RAD_TELEMETRY_ENABLED )

#define NTELEMETRY		1
#undef RADCOPYRIGHT

inline void TelemetryTick() {}
inline void TelemetrySetLevel(unsigned int Level) {}

#define TELEMETRY_REQUIRED( tmRequiredCode )           
#define TELEMETRY_REQUIRED_REPLACE( tmRequiredCode, replacementCode ) replacementCode               

#else

#include "../../thirdparty/telemetry/include/telemetry.h"
#undef RADCOPYRIGHT

PLATFORM_INTERFACE void TelemetryTick();
PLATFORM_INTERFACE void TelemetrySetLevel(unsigned int Level);

struct TelemetryZonePlotData
{
	const char* m_Name;
	TmU64 m_CurrFrameTime;
};

struct TelemetryData
{
	HTELEMETRY tmContext[32];
	float flRDTSCToMilliSeconds;	      
	uint32 FrameCount;				        
	char ServerAddress[128];		     
	uint32 ZoneFilterVal;			      
	int playbacktick;				            
	float dotatime;					 
	uint32 DemoTickStart;			      
	uint32 DemoTickEnd;				      
	uint32 Level;					       
	TelemetryZonePlotData m_ZonePlot[TELEMETRY_ZONE_PLOT_SLOT_MAX];		          
};
PLATFORM_INTERFACE TelemetryData g_Telemetry;

#define TELEMETRY_REQUIRED( tmRequiredCode ) tmRequiredCode           
#define TELEMETRY_REQUIRED_REPLACE( tmRequiredCode, replacementCode ) tmRequiredCode               

#endif  










#define TELEMETRY_ERROR_BUILD_DISABLED		TELEMETRY_REQUIRED_REPLACE( TMERR_DISABLED, 0x0001 )
#define TELEMETRY_ERROR_DISCONNECTED		TELEMETRY_REQUIRED_REPLACE( TMCS_DISCONNECTED, 0 )


#define TELEMETRY_LEVEL0	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[0], 0 )	   
#define TELEMETRY_LEVEL1	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[1], 0 )	    
#define TELEMETRY_LEVEL2	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[2], 0 )	 
#define TELEMETRY_LEVEL3	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[3], 0 )	 
#define TELEMETRY_LEVEL4	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[4], 0 )	 
#define TELEMETRY_LEVEL5	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[5], 0 )	 
#define TELEMETRY_LEVEL6	TELEMETRY_REQUIRED_REPLACE( g_Telemetry.tmContext[6], 0 )	 

#define TM_FAST_TIME() TELEMETRY_REQUIRED_REPLACE( tmFastTime(), 0 )


#define TM_LOAD_TELEMETRY(kUseCheckedDll) TELEMETRY_REQUIRED_REPLACE( tmLoadTelemetry(kUseCheckedDll), 0 )


#define TM_STARTUP() TELEMETRY_REQUIRED_REPLACE( tmStartup(), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_SHUTDOWN() TELEMETRY_REQUIRED( tmShutdown() )


#define TM_INITIALIZE_CONTEXT( pContext, pArena, kArenaSize ) TELEMETRY_REQUIRED_REPLACE( tmInitializeContext( pContext, pArena, kArenaSize ), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_ZONE_FILTERED( context, kThreshold, kFlags, kpFormat, ... ) TELEMETRY_REQUIRED( tmZoneFiltered( context, kThreshold, kFlags, kpFormat, ##__VA_ARGS__ ) )


#define TM_ZONE( context, kFlags, kpFormat, ... ) TELEMETRY_REQUIRED( tmZone( context, kFlags, kpFormat, ##__VA_ARGS__ ) )

#define TM_ZONE_DEFAULT( context ) TM_ZONE( context, TMZF_NONE, __FUNCTION__ )
#define TM_ZONE_IDLE( context ) TM_ZONE( context, TMZF_IDLE, __FUNCTION__ )
#define TM_ZONE_STALL( context ) TM_ZONE( context, TMZF_STALL, __FUNCTION__ )


#define TM_CHECK_VERSION( context, major, minor, build, cust ) TELEMETRY_REQUIRED_REPLACE( tmCheckVersion( context, major, minor, build, cust ), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_LISTEN_IPC( context, name ) TELEMETRY_REQUIRED_REPLACE( tmListenIPC( context, name ), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_UPDATE_SYMBOL_DATA( context ) TELEMETRY_REQUIRED( tmUpdateSymbolData( context ) )


#define TM_GET_SESSION_NAME( context, dst, kDstSize ) TELEMETRY_REQUIRED_REPLACE( tmGetSessionName( context, dst, kDstSize ), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_UNWIND_TO_DEBUG_ZONE_LEVEL( context, kLevel ) TELEMETRY_REQUIRED( tmUnwindToDebugZoneLevel( context, kLevel ) )


#define TM_SET_DEBUG_ZONE_LEVEL( context, kLevel ) TELEMETRY_REQUIRED( tmSetDebugZoneLevel( context, kLevel ) )


#define TM_CHECK_DEBUG_ZONE_LEVEL( context, kLevel ) TELEMETRY_REQUIRED( tmCheckDebugZoneLevel( context, kLevel ) )


#define TM_GET_CALL_STACK( context, TmCallStack_Ptr ) TELEMETRY_REQUIRED_REPLACE( tmGetCallStack( context, TmCallStack_Ptr ), 0 )


#define TM_SEND_CALL_STACK( context, TmCallStack_Ptr ) TELEMETRY_REQUIRED_REPLACE( tmSendCallStack( context, TmCallStack_Ptr ), 0 )


#define TM_GET_LAST_ERROR( context ) TELEMETRY_REQUIRED_REPLACE( tmGetLastError( context ), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_SHUTDOWN_CONTEXT( context ) TELEMETRY_REQUIRED( tmShutdownContext( context ) )


#define TM_GET_ACCUMULATION_START( context ) TELEMETRY_REQUIRED_REPLACE( tmGetAccumulationStart( context ), 0 )


#define TM_GET_LAST_CONTEXT_SWITCH_TIME( context ) TELEMETRY_REQUIRED_REPLACE( tmGetLastContextSwitchTime( context ), 0 )


#define TM_ENTER_ACCUMULATION_ZONE( context, zone_variable ) TELEMETRY_REQUIRED( tmEnterAccumulationZone( context, zone_variable ) )


#define TM_LEAVE_ACCUMULATION_ZONE( context, zone_variable ) TELEMETRY_REQUIRED( tmLeaveAccumulationZone( context, zone_variable ) )


#define TM_GET_FORMAT_CODE( context, pCode, kpFmt ) TELEMETRY_REQUIRED( tmGetFormatCode( context, pCode, kpFmt ) )


#define TM_DYNAMIC_STRING( context, kpString ) TELEMETRY_REQUIRED_REPLACE( tmDynamicString( context, kpString ), NULL )


#define TM_CLEAR_STATIC_STRING( context, kpString ) TELEMETRY_REQUIRED( tmClearStaticString( context, kpString ) )


#define TM_ENABLE( context, kOption, kValue ) TELEMETRY_REQUIRED( tmEnable( context, kOption, kValue ) )


#define TM_IS_ENABLED( context, kOption ) TELEMETRY_REQUIRED_REPLACE( tmIsEnabled( context, kOption ), 0 )

#define TM_SET_PARAMETER( context, kParameter, kpValue ) TELEMETRY_REQUIRED( tmSetParameter( context, kParameter, kpValue ) )


#define TM_OPEN( context, kpAppName, kpBuildInfo, kpServerAddress, kConnection, kServerPort, kFlags, kTimeoutMS ) TELEMETRY_REQUIRED_REPLACE( tmOpen( context, kpAppName, kpBuildInfo, kpServerAddress, kConnection, kServerPort, kFlags, kTimeoutMS ), TELEMETRY_ERROR_BUILD_DISABLED )


#define TM_CLOSE( context ) TELEMETRY_REQUIRED( tmClose( context ) )


#define TM_TICK( context ) TELEMETRY_REQUIRED( tmTick( context ) )


#define TM_FLUSH( context ) TELEMETRY_REQUIRED( tmFlush( context ) )


#define TM_PAUSE( context, kPause ) TELEMETRY_REQUIRED( tmPause( context, kPause ) )


#define TM_IS_PAUSED( context ) TELEMETRY_REQUIRED_REPLACE( tmIsPaused( context ), 0 )


#define TM_GET_CONNECTION_STATUS( context ) TELEMETRY_REQUIRED_REPLACE( tmGetConnectionStatus( context ), TELEMETRY_ERROR_DISCONNECTED )


#define TM_FREE( context, kpPtr ) TELEMETRY_REQUIRED( tmFree( context, kpPtr ) )


#define TM_GET_STAT_I( context, kStat ) TELEMETRY_REQUIRED_REPLACE( tmGetStati( context, kStat ), 0 )


#define TM_LEAVE( context ) TELEMETRY_REQUIRED( tmLeave( context ) )


#define TM_LEAVE_EX( context, kMatchId, kThreadId, kpFilename, kLine ) TELEMETRY_REQUIRED( tmLeaveEx( context, kMatchId, kThreadId, kpFilename, kLine ) )


#define TM_TRY_LOCK( context, kPtr, kpLockName, ... ) TELEMETRY_REQUIRED( tmTryLock( context, kPtr, kpLockName, ##__VA_ARGS__ ) )


#define TM_TRY_LOCK_EX( context, matcher, kThreshold, kpFileName, kLine, kPtr, kpLockName, ... ) TELEMETRY_REQUIRED( tmTryLockEx( context, matcher, kThreshold, kpFileName, kLine, kPtr, kpLockName, ##__VA_ARGS__ ) )


#define TM_END_TRY_LOCK( context, kPtr, kResult ) TELEMETRY_REQUIRED( tmEndTryLock( context, kPtr, kResult ) )


#define TM_END_TRY_LOCK_EX( context, kMatchId, kpFileName, kLine, kPtr, kResult ) TELEMETRY_REQUIRED( tmEndTryLockEx( context, kMatchId, kpFileName, kLine, kPtr, kResult ) )


#define TM_BEGIN_TIME_SPAN( context, kId, kFlags, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmBeginTimeSpan( context, kId, kFlags, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_END_TIME_SPAN( context, kId, kFlags, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmEndTimeSpan( context, kId, kFlags, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_BEGIN_TIME_SPAN_AT( context, kId, kFlags, kTimeStamp, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmBeginTimeSpanAt( context, kId, kFlags, kTimeStamp, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_END_TIME_SPAN_AT( context, kId, kFlags, kTimeStamp, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmEndTimeSpanAt( context, kId, kFlags, kTimeStamp, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_SIGNAL_LOCK_COUNT( context, kPtr, kCount, kpDescription, ... ) TELEMETRY_REQUIRED( tmSignalLockCount( context, kPtr, kCount, kpDescription, ##__VA_ARGS__ ) )


#define TM_SET_LOCK_STATE( context, kPtr, kState, kpDescription, ... ) TELEMETRY_REQUIRED( tmSetLockState( context, kPtr, kState, kpDescription, ##__VA_ARGS__ ) )


#define TM_SET_LOCK_STATE_EX( context, kpFileName, kLine, kPtr, kState, kpDescription, ... ) TELEMETRY_REQUIRED( tmSetLockStateEx( context, kpFileName, kLine, kPtr, kState, kpDescription, ##__VA_ARGS__ ) )


#define TM_SET_LOCK_STATE_MIN_TIME( context, buf, kPtr, kState, kpDescription, ... ) TELEMETRY_REQUIRED( tmSetLockStateMinTime( context, buf, kPtr, kState, kpDescription, ##__VA_ARGS__ ) )


#define TM_SET_LOCK_STATE_MIN_TIME_EX( context, buf, kpFilename, kLine, kPtr, kState, kpDescription, ... ) TELEMETRY_REQUIRED( tmSetLockStateMinTimeEx( context, buf, kpFilename, kLine, kPtr, kState, kpDescription, ##__VA_ARGS__ ) )


#define TM_THREAD_NAME( context, kThreadID, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmThreadName( context, kThreadID, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_LOCK_NAME( context, kPtr, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmLockName( context, kPtr, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_EMIT_ACCUMULATION_ZONE( context, kZoneFlags, pStart, kCount, kTotal, kpZoneFormat, ... ) TELEMETRY_REQUIRED( tmEmitAccumulationZone( context, kZoneFlags, pStart, kCount, kTotal, kpZoneFormat, ##__VA_ARGS__ ) )


#define TM_SET_VARIABLE( context, kpKey, kpValueFormat, ... ) TELEMETRY_REQUIRED( tmSetVariable( context, kpKey, kpValueFormat, ##__VA_ARGS__ ) )


#define TM_SET_TIMELINE_SECTION_NAME( context, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmSetTimelineSectionName( context, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_ENTER( context, kFlags, kpZoneName, ... ) TELEMETRY_REQUIRED( tmEnter( context, kFlags, kpZoneName, ##__VA_ARGS__ ) )


#define TM_ENTER_EX( context, pMatchId, kThreadId, kThreshold, kpFilename, kLine, kFlags, kpZoneName, ... ) TELEMETRY_REQUIRED( tmEnterEx( context, pMatchId, kThreadId, kThreshold, kpFilename, kLine, kFlags, kpZoneName, ##__VA_ARGS__ ) )


#define TM_ALLOC( context, kPtr, kSize, kpDescription, ... ) TELEMETRY_REQUIRED( tmAlloc( context, kPtr, kSize, kpDescription, ##__VA_ARGS__ ) )


#define TM_ALLOC_EX( context, kpFilename, kLineNumber, kPtr, kSize, kpDescription, ... ) TELEMETRY_REQUIRED( tmAllocEx( context, kpFilename, kLineNumber, kPtr, kSize, kpDescription, ##__VA_ARGS__ ) )


#define TM_MESSAGE( context, kFlags, kpFormatString, ... ) TELEMETRY_REQUIRED( tmMessage( context, kFlags, kpFormatString, ##__VA_ARGS__ ) )
#define TM_LOG( context, kpFormatString, ... ) TM_MESSAGE( context, TMMF_SEVERITY_LOG, kpFormatString, ##__VA_ARGS__ )
#define TM_WARNING( context, kpFormatString, ... ) TM_MESSAGE( context, TMMF_SEVERITY_WARNING, kpFormatString, ##__VA_ARGS__ )
#define TM_ERROR( context, kpFormatString, ... ) TM_MESSAGE( context, TMMF_SEVERITY_ERROR, kpFormatString, ##__VA_ARGS__ )



#define TM_PLOT( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlot( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_PLOT_F32( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlotF32( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_PLOT_F64( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlotF64( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_PLOT_I32( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlotI32( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_PLOT_U32( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlotU32( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_PLOT_I64( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlotI64( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_PLOT_U64( context, kType, kFlags, kValue, kpNameFormat, ... ) TELEMETRY_REQUIRED( tmPlotU64( context, kType, kFlags, kValue, kpNameFormat, ##__VA_ARGS__ ) )


#define TM_BLOB( context, kpData, kDataSize, kpPluginIdentifier, kpBlobName, ...) TELEMETRY_REQUIRED( tmBlob( context, kpData, kDataSize, kpPluginIdentifier, kpBlobName, ##__VA_ARGS__ ) )


#define TM_DISJOINT_BLOB( context, kNumPieces, kpData, kDataSizes, kpPluginIdentifier, kpBlobName, ... ) TELEMETRY_REQUIRED( tmDisjointBlob( context, kNumPieces, kpData, kDataSizes, kpPluginIdentifier, kpBlobName, ##__VA_ARGS__ ) )












#if !defined( RAD_TELEMETRY_ENABLED )

class CTelemetryLock
{
public:
	CTelemetryLock(void* plocation, const char* description) {}
	~CTelemetryLock() {}
	void Locked() {}
	void Unlocked() {}
};

class CTelemetrySpikeDetector
{
public:
	CTelemetrySpikeDetector(const char* msg, unsigned int threshold = 50) {}
	~CTelemetrySpikeDetector() { }
};

class CTelemetryZonePlotScope
{
public:

	CTelemetryZonePlotScope(const char* pName, TelemetryZonePlotSlot_t slot) {}
	~CTelemetryZonePlotScope() {}
};

#define TelemetrySetLockName( _ctx, _location, _description ) 
#define TM_ZONE_PLOT( context, name, slot ) 

#else

#define TelemetrySetLockName( _ctx, _location, _description ) \
	do  													  \
	{   													  \
		static bool s_bNameSet = false; 					  \
		if( _ctx && !s_bNameSet )							  \
		{   												  \
			tmLockName( _ctx, _location, _description ); 	  \
			s_bNameSet = true;  							  \
		}   												  \
	} while( 0 )

class CTelemetryLock
{
public:
	CTelemetryLock(void* plocation, const char* description)
	{
		m_plocation = (const char*)plocation;
		m_description = description;
		TelemetrySetLockName(TELEMETRY_LEVEL1, m_plocation, m_description);
		TM_TRY_LOCK(TELEMETRY_LEVEL1, m_plocation, "%s", m_description);
	}
	~CTelemetryLock()
	{
		Unlocked();
	}
	void Locked()
	{
		TM_END_TRY_LOCK(TELEMETRY_LEVEL1, m_plocation, TMLR_SUCCESS);
		TM_SET_LOCK_STATE(TELEMETRY_LEVEL1, m_plocation, TMLS_LOCKED, "%s Locked", m_description);
	}
	void Unlocked()
	{
		if (m_plocation)
		{
			TM_SET_LOCK_STATE(TELEMETRY_LEVEL1, m_plocation, TMLS_RELEASED, "%s Released", m_description);
			m_plocation = NULL;
		}
	}

public:
	const char* m_plocation;
	const char* m_description;
};

class CTelemetrySpikeDetector
{
public:
	CTelemetrySpikeDetector(const char* msg, float threshold = 5) :
		m_message(msg), m_threshold(threshold), time0(tmFastTime()) {}
	~CTelemetrySpikeDetector()
	{
		float time = (tmFastTime() - time0) * g_Telemetry.flRDTSCToMilliSeconds;
		if (time >= m_threshold)
		{
			TM_MESSAGE(TELEMETRY_LEVEL0, TMMF_ICON_NOTE | TMMF_SEVERITY_WARNING, "(dota/spike)%s %.2fms %t", m_message, time, tmSendCallStack(TELEMETRY_LEVEL0, 0));
		}
	}

private:
	TmU64 time0;
	float m_threshold;
	const char* m_message;
};

#define TM_ZONE_PLOT( context, name, slot ) CTelemetryZonePlotScope _telemetryZonePlot##__LINE__(context, name, slot);

class CTelemetryZonePlotScope
{
public:

	CTelemetryZonePlotScope(HTELEMETRY context, const char* pName, TelemetryZonePlotSlot_t slot)
		:
		m_Context(context),
		m_SlotData(NULL),
		m_StartTime(0)
	{
		if (slot < TELEMETRY_ZONE_PLOT_SLOT_MAX)
		{
			m_SlotData = &g_Telemetry.m_ZonePlot[slot];
			m_SlotData->m_Name = pName;
			m_StartTime = TM_FAST_TIME();
		}
		TM_ENTER(m_Context, TMZF_NONE, "%s", pName);
	}

	~CTelemetryZonePlotScope()
	{
		TM_LEAVE(m_Context);
		if (m_SlotData)
		{
			m_SlotData->m_CurrFrameTime += (TM_FAST_TIME() - m_StartTime);
		}
	}

private:

	HTELEMETRY m_Context;
	TelemetryZonePlotData* m_SlotData;
	TmU64 m_StartTime;
};

#endif  

#endif  