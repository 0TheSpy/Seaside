#ifndef VPROF_H
#define VPROF_H

#if !defined(__SPU__)

#include "dbg.h"
#include "fasttimer.h"
#include "l2cache.h"
#include "threadtools.h"
#include "vprof_sn.h"
#include "vprof_telemetry.h"

#if !( defined( _GAMECONSOLE ) && defined( _CERT ) ) && !defined( _PS3 )
#define VPROF_ENABLED
#endif

#if defined(_X360) && defined(VPROF_ENABLED)

#ifdef PROFILE
#define VPROF_PIX 1
#endif

#include "tier0/pmc360.h"
#ifndef USE_PIX
#define VPROF_UNDO_PIX
#undef _PIX_H_
#undef PIXBeginNamedEvent
#undef PIXEndNamedEvent
#undef PIXSetMarker
#undef PIXNameThread
#define USE_PIX
#include <pix.h>
#undef USE_PIX
#else
#include <pix.h>
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#if defined( _X360 ) && defined( PROFILE )
#define VPROF_PIX 1
#endif

#if defined(_X360) || defined(_PS3)
#define VPROF_VXCONSOLE_EXISTS 1
#endif


#if defined(_X360) && defined(VPROF_PIX)
#pragma comment( lib, "Xapilibi" )
#endif

#define MAXCOUNTERS 256


#ifdef VPROF_ENABLED

#define VPROF_VTUNE_GROUP

#define	VPROF( name )						VPROF_(name, 1, VPROF_BUDGETGROUP_OTHER_UNACCOUNTED, false, 0)
#define	VPROF_ASSERT_ACCOUNTED( name )		VPROF_(name, 1, VPROF_BUDGETGROUP_OTHER_UNACCOUNTED, true, 0)
#define	VPROF_( name, detail, group, bAssertAccounted, budgetFlags )		VPROF_##detail(name,group, bAssertAccounted, budgetFlags)

#define VPROF_BUDGET( name, group )					VPROF_BUDGET_FLAGS(name, group, BUDGETFLAG_OTHER)
#define VPROF_BUDGET_FLAGS( name, group, flags )	VPROF_(name, 0, group, false, flags)

#define VPROF_SCOPE_BEGIN( tag )	do { VPROF( tag )
#define VPROF_SCOPE_END()			} while (0)

#define VPROF_ONLY( expression )	( expression )

#define VPROF_ENTER_SCOPE( name )			g_VProfCurrentProfile.EnterScope( name, 1, VPROF_BUDGETGROUP_OTHER_UNACCOUNTED, false, 0 )
#define VPROF_EXIT_SCOPE()					g_VProfCurrentProfile.ExitScope()

#define VPROF_BUDGET_GROUP_ID_UNACCOUNTED 0


#define BUDGETFLAG_CLIENT	(1<<0)		      
#define BUDGETFLAG_SERVER	(1<<1)		      
#define BUDGETFLAG_OTHER	(1<<2)		          
#define BUDGETFLAG_HIDDEN	(1<<15)
#define BUDGETFLAG_ALL		0xFFFF


#define VPROF_BUDGETGROUP_OTHER_UNACCOUNTED			_T("Unaccounted")
#define VPROF_BUDGETGROUP_WORLD_RENDERING			_T("World Rendering")
#define VPROF_BUDGETGROUP_DISPLACEMENT_RENDERING	_T("Displacement_Rendering")
#define VPROF_BUDGETGROUP_GAME						_T("Game")
#define VPROF_BUDGETGROUP_NPCS						_T("NPCs")
#define VPROF_BUDGETGROUP_SERVER_ANIM				_T("Server Animation")
#define VPROF_BUDGETGROUP_PHYSICS					_T("Physics")
#define VPROF_BUDGETGROUP_STATICPROP_RENDERING		_T("Static_Prop_Rendering")
#define VPROF_BUDGETGROUP_MODEL_RENDERING			_T("Other_Model_Rendering")
#define VPROF_BUDGETGROUP_MODEL_FAST_PATH_RENDERING _T("Fast Path Model Rendering")
#define VPROF_BUDGETGROUP_BRUSH_FAST_PATH_RENDERING _T("Fast Path Brush Rendering")
#define VPROF_BUDGETGROUP_BRUSHMODEL_RENDERING		_T("Brush_Model_Rendering")
#define VPROF_BUDGETGROUP_SHADOW_RENDERING			_T("Shadow_Rendering")
#define VPROF_BUDGETGROUP_DETAILPROP_RENDERING		_T("Detail_Prop_Rendering")
#define VPROF_BUDGETGROUP_PARTICLE_RENDERING		_T("Particle/Effect_Rendering")
#define VPROF_BUDGETGROUP_ROPES						_T("Ropes")
#define VPROF_BUDGETGROUP_DLIGHT_RENDERING			_T("Dynamic_Light_Rendering")
#define VPROF_BUDGETGROUP_OTHER_NETWORKING			_T("Networking")
#define VPROF_BUDGETGROUP_CLIENT_ANIMATION			_T("Client_Animation")
#define VPROF_BUDGETGROUP_OTHER_SOUND				_T("Sound")
#define VPROF_BUDGETGROUP_OTHER_VGUI				_T("VGUI")
#define VPROF_BUDGETGROUP_OTHER_FILESYSTEM			_T("FileSystem")
#define VPROF_BUDGETGROUP_PREDICTION				_T("Prediction")
#define VPROF_BUDGETGROUP_INTERPOLATION				_T("Interpolation")
#define VPROF_BUDGETGROUP_SWAP_BUFFERS				_T("Swap_Buffers")
#define VPROF_BUDGETGROUP_PLAYER					_T("Player")
#define VPROF_BUDGETGROUP_OCCLUSION					_T("Occlusion")
#define VPROF_BUDGETGROUP_OVERLAYS					_T("Overlays")
#define VPROF_BUDGETGROUP_TOOLS						_T("Tools")
#define VPROF_BUDGETGROUP_LIGHTCACHE				_T("Light_Cache")
#define VPROF_BUDGETGROUP_DISP_HULLTRACES			_T("Displacement_Hull_Traces")
#define VPROF_BUDGETGROUP_TEXTURE_CACHE				_T("Texture_Cache")
#define VPROF_BUDGETGROUP_REPLAY					_T("Replay")
#define VPROF_BUDGETGROUP_PARTICLE_SIMULATION		_T("Particle Simulation")
#define VPROF_BUDGETGROUP_SHADOW_DEPTH_TEXTURING	_T("Flashlight Shadows")
#define VPROF_BUDGETGROUP_CLIENT_SIM				_T("Client Simulation")     
#define VPROF_BUDGETGROUP_STEAM						_T("Steam") 
#define VPROF_BUDGETGROUP_CVAR_FIND					_T("Cvar_Find") 
#define VPROF_BUDGETGROUP_CLIENTLEAFSYSTEM			_T("ClientLeafSystem")
#define VPROF_BUDGETGROUP_JOBS_COROUTINES			_T("Jobs/Coroutines")
#define VPROF_BUDGETGROUP_SLEEPING					_T("Sleeping")
#define VPROF_BUDGETGROUP_THREADINGMAIN				_T("ThreadingMain")
#define VPROF_BUDGETGROUP_ENCRYPTION				_T("Encryption")


#define VPROF_BUDGETGROUP_CHROMEHTML				_T("Chromehtml")
#define VPROF_BUDGETGROUP_VGUI						VPROF_BUDGETGROUP_CHROMEHTML
#define VPROF_BUDGETGROUP_TENFOOT					VPROF_BUDGETGROUP_CHROMEHTML
#define VPROF_BUDGETGROUP_STEAMUI					VPROF_BUDGETGROUP_CHROMEHTML

#ifdef VPROF_VXCONSOLE_EXISTS
#define VPROF_UPDATE_BUDGET				0x01	     
#define VPROF_UPDATE_TEXTURE_GLOBAL		0x02	      
#define VPROF_UPDATE_TEXTURE_PERFRAME	0x04	      
#endif

#ifndef VPROF_LEVEL
#endif

#if !defined( VPROF_SN_LEVEL ) && !defined( _CERT )
#endif

#define VPROF_SCOPE_VARIABLE_NAME( prefix, line ) prefix##line
#define VPROF_SCOPE_VARIABLE_DECL( name, level, group, assertAccounted, budgetFlags, line ) CVProfScope VPROF_SCOPE_VARIABLE_NAME( VProf_,line )(name, level, group, assertAccounted, budgetFlags)

#define	VPROF_0(name,group,assertAccounted,budgetFlags)	TM_ZONE( TELEMETRY_LEVEL2, TMZF_NONE, "(%s)%s", group, name ); VPROF_SCOPE_VARIABLE_DECL(name, 0, group, assertAccounted, budgetFlags, __LINE__ );

#if VPROF_LEVEL > 0 
#  define	VPROF_1(name,group,assertAccounted,budgetFlags)	TM_ZONE( TELEMETRY_LEVEL3, TMZF_NONE, "(%s)%s", group, name ); VPROF_SCOPE_VARIABLE_DECL(name, 1, group, assertAccounted, budgetFlags, __LINE__ );
#else
#  if VPROF_SN_LEVEL > 0 && defined( _PS3 )
#	 define	VPROF_1(name,group,assertAccounted,budgetFlags)	CVProfSnMarkerScope VProfSn_( name )
#  else
#    define	VPROF_1(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif
#endif

#if VPROF_LEVEL > 1 
#define	VPROF_2(name,group,assertAccounted,budgetFlags)	TM_ZONE( TELEMETRY_LEVEL4, TMZF_NONE, "(%s)%s", group, name ); VPROF_SCOPE_VARIABLE_DECL(name, 2, group, assertAccounted, budgetFlags, __LINE__);
#else
#  if VPROF_SN_LEVEL > 1 && defined( _PS3 )
#	 define	VPROF_2(name,group,assertAccounted,budgetFlags)	CVProfSnMarkerScope VProfSn_( name )
#  else
#    define	VPROF_2(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif
#endif

#if VPROF_LEVEL > 2 
#define	VPROF_3(name,group,assertAccounted,budgetFlags)	TM_ZONE( TELEMETRY_LEVEL5, TMZF_NONE, "(%s)%s", group, name ); VPROF_SCOPE_VARIABLE_DECL(name, 3, group, assertAccounted, budgetFlags, __LINE__);
#else
#  if VPROF_SN_LEVEL > 2 && defined( _PS3 )
#	 define	VPROF_3(name,group,assertAccounted,budgetFlags)	CVProfSnMarkerScope VProfSn_( name )
#  else
#    define	VPROF_3(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif
#endif

#if VPROF_LEVEL > 3 
#define	VPROF_4(name,group,assertAccounted,budgetFlags)	TM_ZONE( TELEMETRY_LEVEL6, TMZF_NONE, "(%s)%s", group, name ); VPROF_SCOPE_VARIABLE_DECL(name, 4, group, assertAccounted, budgetFlags, __LINE__);
#else
#  if VPROF_SN_LEVEL > 3 && defined( _PS3 )
#	 define	VPROF_4(name,group,assertAccounted,budgetFlags)	CVProfSnMarkerScope VProfSn_( name )
#  else
#    define	VPROF_4(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif
#endif

#ifdef _MSC_VER
#define VProfCode( code ) \
	if ( 0 ) \
		; \
	else \
	{ \
	VPROF( __FUNCTION__ ": " #code ); \
		code; \
	}
#else
#define VProfCode( code ) \
	if ( 0 ) \
		; \
	else \
	{ \
		VPROF( #code ); \
		code; \
	} 
#endif


#define VPROF_INCREMENT_COUNTER(name,amount)			do { static CVProfCounter _counter( name ); _counter.Increment( amount ); } while( 0 )
#define VPROF_INCREMENT_GROUP_COUNTER(name,group,amount)			do { static CVProfCounter _counter( name, group ); _counter.Increment( amount ); } while( 0 )
#define VPROF_SET_COUNTER(name,amount)			do { static CVProfCounter _counter( name ); _counter.Set( amount ); } while( 0 )
#define VPROF_SET_GROUP_COUNTER(name,group,amount)			do { static CVProfCounter _counter( name, group ); _counter.Set( amount ); } while( 0 )

#else

#  if defined( VPROF_SN_LEVEL ) && ( VPROF_SN_LEVEL >= 0 )
#    define	VPROF( name )									CVProfSnMarkerScope VProfSn_( name )
#    define	VPROF_ASSERT_ACCOUNTED( name )					VPROF( name )
#    define	VPROF_( name, detail, group, bAssertAccounted, budgetFlags )	VPROF_##detail( name, group, bAssertAccounted, budgetFlags )
#	 define	VPROF_0(name,group,assertAccounted,budgetFlags)	VPROF( name )
#    define VPROF_BUDGET( name, group )						VPROF( name )
#    define VPROF_BUDGET_FLAGS( name, group, flags )		VPROF( name )

#    define VPROF_SCOPE_BEGIN( tag )	do { VPROF( tag )
#    define VPROF_SCOPE_END()			} while (0)

#    define VPROF_ONLY( expression )	( expression )

#    define VPROF_ENTER_SCOPE( name )   g_pfnPushMarker( name )
#    define VPROF_EXIT_SCOPE()   g_pfnPopMarker()
#  else
#    define	VPROF( name )									((void)0)
#    define	VPROF_ASSERT_ACCOUNTED( name )					((void)0)
#    define	VPROF_( name, detail, group, bAssertAccounted, budgetFlags )	((void)0)
#    define	VPROF_0(name,group,assertAccounted,budgetFlags)	((void)0)
#    define VPROF_BUDGET( name, group )						((void)0)
#    define VPROF_BUDGET_FLAGS( name, group, flags )		((void)0)

#    define VPROF_SCOPE_BEGIN( tag )	do {
#    define VPROF_SCOPE_END()			} while (0)

#    define VPROF_ONLY( expression )	((void)0)

#    define VPROF_ENTER_SCOPE( name )
#    define VPROF_EXIT_SCOPE()
#  endif

#  if defined( VPROF_SN_LEVEL ) && ( VPROF_SN_LEVEL >= 1 )
#	 define	VPROF_1(name,group,assertAccounted,budgetFlags)	VPROF( name )
#  else
#    define	VPROF_1(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif

#  if defined( VPROF_SN_LEVEL ) && ( VPROF_SN_LEVEL >= 2 )
#	 define	VPROF_2(name,group,assertAccounted,budgetFlags)	VPROF( name )
#  else
#    define	VPROF_2(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif

#  if defined( VPROF_SN_LEVEL ) && ( VPROF_SN_LEVEL >= 3 )
#	 define	VPROF_3(name,group,assertAccounted,budgetFlags)	VPROF( name )
#  else
#    define	VPROF_3(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif

#  if defined( VPROF_SN_LEVEL ) && ( VPROF_SN_LEVEL >= 4 )
#	 define	VPROF_4(name,group,assertAccounted,budgetFlags)	VPROF( name )
#  else
#    define	VPROF_4(name,group,assertAccounted,budgetFlags)	((void)0)
#  endif



#define VPROF_INCREMENT_COUNTER(name,amount)			((void)0)
#define VPROF_INCREMENT_GROUP_COUNTER(name,group,amount)	((void)0)
#define VPROF_SET_COUNTER(name,amount)		((void)0)
#define VPROF_SET_GROUP_COUNTER(name,group,amount)	((void)0)

#define VPROF_TEST_SPIKE( msec )	((void)0)

#define VProfCode( code ) code

#endif

#ifdef VPROF_ENABLED

class PLATFORM_CLASS CVProfNode
{
	friend class CVProfRecorder;
	friend class CVProfile;

public:
	CVProfNode(const tchar* pszName, int detailLevel, CVProfNode* pParent, const tchar* pBudgetGroupName, int budgetFlags);
	~CVProfNode();

	CVProfNode* GetSubNode(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, int budgetFlags);
	CVProfNode* GetSubNode(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName);
	CVProfNode* GetParent();
	CVProfNode* GetSibling();
	CVProfNode* GetPrevSibling();
	CVProfNode* GetChild();

	void MarkFrame();
	void ResetPeak();

	void Pause();
	void Resume();
	void Reset();

	void EnterScope();
	bool ExitScope();

	const tchar* GetName();

	int GetBudgetGroupID()
	{
		return m_BudgetGroupID;
	}

	void SetBudgetGroupID(int id)
	{
		m_BudgetGroupID = id;
	}

	int	GetCurCalls();
	double GetCurTime();
	int GetPrevCalls();
	double GetPrevTime();
	int	GetTotalCalls();
	double GetTotalTime();
	double GetPeakTime();

	double GetCurTimeLessChildren();
	double GetPrevTimeLessChildren();
	double GetTotalTimeLessChildren();

	int GetPrevL2CacheMissLessChildren();
	int GetPrevLoadHitStoreLessChildren();

	void ClearPrevTime();

	int GetL2CacheMisses();

	void SetCurFrameTime(unsigned long milliseconds);

	void SetClientData(int iClientData) { m_iClientData = iClientData; }
	int GetClientData() const { return m_iClientData; }

#ifdef DBGFLAG_VALIDATE
	void Validate(CValidator& validator, tchar* pchName);		    
#endif  


private:

	void SetUniqueNodeID(int id)
	{
		m_iUniqueNodeID = id;
	}

	int GetUniqueNodeID() const
	{
		return m_iUniqueNodeID;
	}

	static int s_iCurrentUniqueNodeID;


private:
	const tchar* m_pszName;
	CFastTimer	m_Timer;

	int			m_iPrevL2CacheMiss;
	int			m_iCurL2CacheMiss;
	int			m_iTotalL2CacheMiss;

#ifndef _X360	
	CL2Cache	m_L2Cache;
#else  

	unsigned int m_iBitFlags;      
	CPMCData	m_PMCData;
	int			m_iPrevLoadHitStores;
	int			m_iCurLoadHitStores;
	int			m_iTotalLoadHitStores;

public:
	enum FlagBits
	{
		kRecordL2 = 0x01,
		kCPUTrace = 0x02,        
	};
	inline void EnableL2andLHS(bool enable)
	{
		if (enable)
			m_iBitFlags |= kRecordL2;
		else
			m_iBitFlags &= (~kRecordL2);
	}

	inline bool IsL2andLHSEnabled(void)
	{
		return (m_iBitFlags & kRecordL2) != 0;
	}

	int GetLoadHitStores();

private:

#endif

	int			m_nRecursions;

	unsigned	m_nCurFrameCalls;
	CCycleCount	m_CurFrameTime;

	unsigned	m_nPrevFrameCalls;
	CCycleCount	m_PrevFrameTime;

	unsigned	m_nTotalCalls;
	CCycleCount	m_TotalTime;

	CCycleCount	m_PeakTime;

	CVProfNode* m_pParent;
	CVProfNode* m_pChild;
	CVProfNode* m_pSibling;

	int m_BudgetGroupID;

	int m_iClientData;
	int m_iUniqueNodeID;
};

enum VProfReportType_t
{
	VPRT_SUMMARY = (1 << 0),
	VPRT_HIERARCHY = (1 << 1),
	VPRT_HIERARCHY_TIME_PER_FRAME_AND_COUNT_ONLY = (1 << 2),
	VPRT_LIST_BY_TIME = (1 << 3),
	VPRT_LIST_BY_TIME_LESS_CHILDREN = (1 << 4),
	VPRT_LIST_BY_AVG_TIME = (1 << 5),
	VPRT_LIST_BY_AVG_TIME_LESS_CHILDREN = (1 << 6),
	VPRT_LIST_BY_PEAK_TIME = (1 << 7),
	VPRT_LIST_BY_PEAK_OVER_AVERAGE = (1 << 8),
	VPRT_LIST_TOP_ITEMS_ONLY = (1 << 9),

	VPRT_FULL = (0xffffffff & ~(VPRT_HIERARCHY_TIME_PER_FRAME_AND_COUNT_ONLY | VPRT_LIST_TOP_ITEMS_ONLY)),
};

enum CounterGroup_t
{
	COUNTER_GROUP_DEFAULT = 0,
	COUNTER_GROUP_NO_RESET,				           
										COUNTER_GROUP_TEXTURE_GLOBAL,		           
										COUNTER_GROUP_TEXTURE_PER_FRAME,		    
										COUNTER_GROUP_GRAPHICS_PER_FRAME,	        

										COUNTER_GROUP_TELEMETRY,
};

class PLATFORM_CLASS CVProfile
{
public:
	CVProfile();
	~CVProfile();

	void Term();

	void Start();
	void Stop();

	void SetTargetThreadId(unsigned id) { m_TargetThreadId = id; }
	unsigned GetTargetThreadId() { return m_TargetThreadId; }
	bool InTargetThread() { return (m_TargetThreadId == ThreadGetCurrentId()); }

#ifdef VPROF_VXCONSOLE_EXISTS
	enum VXConsoleReportMode_t
	{
		VXCONSOLE_REPORT_TIME = 0,
		VXCONSOLE_REPORT_L2CACHE_MISSES,
		VXCONSOLE_REPORT_LOAD_HIT_STORE,
		VXCONSOLE_REPORT_COUNT,
	};

	void VXProfileStart();
	void VXProfileUpdate();
	void VXEnableUpdateMode(int event, bool bEnable);
	void VXSendNodes(void);

	void PMCDisableAllNodes(CVProfNode* pStartNode = NULL);          
	bool PMCEnableL2Upon(const tchar* pszNodeName, bool bRecursive = false);          
	bool PMCDisableL2Upon(const tchar* pszNodeName, bool bRecursive = false);          

	void DumpEnabledPMCNodes(void);

	void VXConsoleReportMode(VXConsoleReportMode_t mode);
	void VXConsoleReportScale(VXConsoleReportMode_t mode, float flScale);
#endif

#ifdef _X360


	enum CPUTraceState
	{
		kDisabled,
		kFirstHitNode,						             
		kAllNodesInFrame_WaitingForMark,	                      
		kAllNodesInFrame_Recording,			         

		kAllNodesInFrame_WaitingForMarkMultiFrame,	                      
		kAllNodesInFrame_RecordingMultiFrame,
	};

	inline CPUTraceState GetCPUTraceMode();
	inline void SetCPUTraceEnabled(CPUTraceState enabled, bool bTraceCompleteEvent = false, int nNumFrames = -1);
	inline void IncrementMultiTraceIndex();            
	inline unsigned int GetMultiTraceIndex();    
	void CPUTraceDisableAllNodes(CVProfNode* pStartNode = NULL);          
	CVProfNode* CPUTraceEnableForNode(const tchar* pszNodeName);                
	CVProfNode* CPUTraceGetEnabledNode(CVProfNode* pStartNode = NULL);          
	const char* GetCPUTraceFilename();         
	const char* SetCPUTraceFilename(const char* filename);                
	inline bool TraceCompleteEvent(void);

#ifdef _X360
	void LatchMultiFrame(int64 cycles);
	void SpewWorstMultiFrame();
#endif

#endif

	void EnterScope(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, bool bAssertAccounted);
	void EnterScope(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, bool bAssertAccounted, int budgetFlags);
	void ExitScope();

	void MarkFrame(char const* str);
	void MarkFrame();
	void ResetPeaks();

	void Pause();
	void Resume();
	void Reset();

	bool IsEnabled() const;
	int GetDetailLevel() const;

	bool AtRoot() const;

#ifdef VPROF_VTUNE_GROUP
#	define MAX_GROUP_STACK_DEPTH 1024

	void EnableVTuneGroup(const tchar* pGroupName)
	{
		m_nVTuneGroupID = BudgetGroupNameToBudgetGroupID(pGroupName);
		m_bVTuneGroupEnabled = true;
	}
	void DisableVTuneGroup(void)
	{
		m_bVTuneGroupEnabled = false;
	}

	inline void PushGroup(int nGroupID);
	inline void PopGroup(void);
#endif

	int NumFramesSampled() { return m_nFrames; }
	double GetPeakFrameTime();
	double GetTotalTimeSampled();
	double GetTimeLastFrame();

	CVProfNode* GetRoot();
	CVProfNode* FindNode(CVProfNode* pStartNode, const tchar* pszNode);
	CVProfNode* GetCurrentNode();

	void OutputReport(int type = VPRT_FULL, const tchar* pszStartNode = NULL, int budgetGroupID = -1);

	const tchar* GetBudgetGroupName(int budgetGroupID);
	int GetBudgetGroupFlags(int budgetGroupID) const;	      
	int GetNumBudgetGroups(void);
	void GetBudgetGroupColor(int budgetGroupID, int& r, int& g, int& b, int& a);
	int BudgetGroupNameToBudgetGroupID(const tchar* pBudgetGroupName);
	int BudgetGroupNameToBudgetGroupID(const tchar* pBudgetGroupName, int budgetFlagsToORIn);
	void RegisterNumBudgetGroupsChangedCallBack(void (*pCallBack)(void));

	int BudgetGroupNameToBudgetGroupIDNoCreate(const tchar* pBudgetGroupName) { return FindBudgetGroupName(pBudgetGroupName); }

	void HideBudgetGroup(int budgetGroupID, bool bHide = true);
	void HideBudgetGroup(const tchar* pszName, bool bHide = true) { HideBudgetGroup(BudgetGroupNameToBudgetGroupID(pszName), bHide); }

	int* FindOrCreateCounter(const tchar* pName, CounterGroup_t eCounterGroup = COUNTER_GROUP_DEFAULT);
	void ResetCounters(CounterGroup_t eCounterGroup);

	int GetNumCounters(void) const;

	const tchar* GetCounterName(int index) const;
	int GetCounterValue(int index) const;
	const tchar* GetCounterNameAndValue(int index, int& val) const;
	CounterGroup_t GetCounterGroup(int index) const;

	void PMEInitialized(bool bInit) { m_bPMEInit = bInit; }
	void PMEEnable(bool bEnable) { m_bPMEEnabled = bEnable; }

#ifdef _X360
	bool UsePME(void) { return (CPMCData::IsInitialized() && m_bPMEEnabled); }
#elif defined( _PS3 )
	inline bool UsePME(void) { return false; }
#else
	bool UsePME(void) { return (m_bPMEInit && m_bPMEEnabled); }
#endif

#ifdef DBGFLAG_VALIDATE
	void Validate(CValidator& validator, tchar* pchName);		    
#endif  

protected:

	void FreeNodes_R(CVProfNode* pNode);

#ifdef VPROF_VTUNE_GROUP
	bool VTuneGroupEnabled()
	{
		return m_bVTuneGroupEnabled;
	}
	int VTuneGroupID()
	{
		return m_nVTuneGroupID;
	}
#endif

	void SumTimes(const tchar* pszStartNode, int budgetGroupID);
	void SumTimes(CVProfNode* pNode, int budgetGroupID);
	void DumpNodes(CVProfNode* pNode, int indent, bool bAverageAndCountOnly);
	int FindBudgetGroupName(const tchar* pBudgetGroupName);
	int AddBudgetGroupName(const tchar* pBudgetGroupName, int budgetFlags);

#ifdef VPROF_VTUNE_GROUP
	bool		m_bVTuneGroupEnabled;
	int			m_nVTuneGroupID;
	int			m_GroupIDStack[MAX_GROUP_STACK_DEPTH];
	int			m_GroupIDStackDepth;
#endif
	int 		m_enabled;
	bool		m_fAtRoot;         
	CVProfNode* m_pCurNode;
	CVProfNode	m_Root;
	int			m_nFrames;
	int			m_ProfileDetailLevel;
	int			m_pausedEnabledDepth;

	class CBudgetGroup
	{
	public:
		tchar* m_pName;
		int m_BudgetFlags;
	};

	CBudgetGroup* m_pBudgetGroups;
	int			m_nBudgetGroupNamesAllocated;
	int			m_nBudgetGroupNames;
	void		(*m_pNumBudgetGroupsChangedCallBack)(void);

	bool		m_bPMEInit;
	bool		m_bPMEEnabled;

	int m_Counters[MAXCOUNTERS];
	char m_CounterGroups[MAXCOUNTERS];    
	tchar* m_CounterNames[MAXCOUNTERS];
	int m_NumCounters;

#ifdef VPROF_VXCONSOLE_EXISTS
	int						m_UpdateMode;
	int						m_nFramesRemaining;
	int						m_nFrameCount;
	int64					m_WorstCycles;
	char					m_WorstTraceFilename[128];
	char					m_CPUTraceFilename[128];
	unsigned int			m_iSuccessiveTraceIndex;
	VXConsoleReportMode_t	m_ReportMode;
	float					m_pReportScale[VXCONSOLE_REPORT_COUNT];
	bool					m_bTraceCompleteEvent;
#endif
#ifdef _X360
	CPUTraceState			m_iCPUTraceEnabled;
#endif

	unsigned m_TargetThreadId;
};

PLATFORM_INTERFACE CVProfile g_VProfCurrentProfile;

PLATFORM_INTERFACE bool g_VProfSignalSpike;

class CVProfSpikeDetector
{
public:
	CVProfSpikeDetector(float spike) :
		m_timeLast(GetTimeLast())
	{
		m_spike = spike;
		m_Timer.Start();
	}

	~CVProfSpikeDetector()
	{
		m_Timer.End();
		if (Plat_FloatTime() - m_timeLast > 2.0)
		{
			m_timeLast = Plat_FloatTime();
			if (m_Timer.GetDuration().GetMillisecondsF() > m_spike)
			{
				g_VProfSignalSpike = true;
			}
		}
	}

private:
	static float& GetTimeLast() { static float timeLast = 0; return timeLast; }
	CFastTimer	m_Timer;
	float m_spike;
	float& m_timeLast;
};


#define VPROF_TEST_SPIKE( msec ) CVProfSpikeDetector UNIQUE_ID( msec )

#ifdef VPROF_VTUNE_GROUP
inline void CVProfile::PushGroup(int nGroupID)
{
	Assert(m_GroupIDStackDepth > 0);
	Assert(m_GroupIDStackDepth < MAX_GROUP_STACK_DEPTH);
	m_GroupIDStack[m_GroupIDStackDepth] = nGroupID;
	m_GroupIDStackDepth++;
	if (m_GroupIDStack[m_GroupIDStackDepth - 2] != nGroupID &&
		VTuneGroupEnabled() &&
		nGroupID == VTuneGroupID())
	{
		vtune(true);
	}
}
#endif  

#ifdef VPROF_VTUNE_GROUP
inline void CVProfile::PopGroup(void)
{
	m_GroupIDStackDepth--;
	Assert(m_GroupIDStackDepth > 0);
	if (m_GroupIDStack[m_GroupIDStackDepth] != m_GroupIDStack[m_GroupIDStackDepth + 1] &&
		VTuneGroupEnabled() &&
		m_GroupIDStack[m_GroupIDStackDepth + 1] == VTuneGroupID())
	{
		vtune(false);
	}
}
#endif  

inline CVProfile* GetVProfProfileForCurrentThread()
{
	return NULL;
}

class CVProfScope : public CVProfSnMarkerScope
{
public:
	CVProfScope(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, bool bAssertAccounted, int budgetFlags);
	~CVProfScope();

private:
	bool m_bEnabled;
};

inline CVProfNode::CVProfNode(const tchar* pszName, int detailLevel, CVProfNode* pParent, const tchar* pBudgetGroupName, int budgetFlags)
	: m_pszName(pszName),
	m_nCurFrameCalls(0),
	m_nPrevFrameCalls(0),
	m_nRecursions(0),
	m_pParent(pParent),
	m_pChild(NULL),
	m_pSibling(NULL),
	m_iClientData(-1)
#ifdef _X360
	, m_iBitFlags(0)
#endif
{
	m_iUniqueNodeID = s_iCurrentUniqueNodeID++;

	if (m_iUniqueNodeID > 0)
	{
		m_BudgetGroupID = g_VProfCurrentProfile.BudgetGroupNameToBudgetGroupID(pBudgetGroupName, budgetFlags);
	}
	else
	{
		m_BudgetGroupID = 0;          
	}

	Reset();

	if (m_pParent && (m_BudgetGroupID == VPROF_BUDGET_GROUP_ID_UNACCOUNTED))
	{
		m_BudgetGroupID = m_pParent->GetBudgetGroupID();
	}
}


inline CVProfNode* CVProfNode::GetParent()
{
	Assert(m_pParent);
	return m_pParent;
}

inline CVProfNode* CVProfNode::GetSibling()
{
	return m_pSibling;
}

inline CVProfNode* CVProfNode::GetPrevSibling()
{
	CVProfNode* p = GetParent();

	if (!p)
		return NULL;

	CVProfNode* s;
	for (s = p->GetChild();
		s && (s->GetSibling() != this);
		s = s->GetSibling())
		;

	return s;
}

inline CVProfNode* CVProfNode::GetChild()
{
	return m_pChild;
}

inline const tchar* CVProfNode::GetName()
{
	return m_pszName;
}

inline int	CVProfNode::GetTotalCalls()
{
	return m_nTotalCalls;
}

inline double CVProfNode::GetTotalTime()
{
	return m_TotalTime.GetMillisecondsF();
}

inline int	CVProfNode::GetCurCalls()
{
	return m_nCurFrameCalls;
}

inline double CVProfNode::GetCurTime()
{
	return m_CurFrameTime.GetMillisecondsF();
}

inline int CVProfNode::GetPrevCalls()
{
	return m_nPrevFrameCalls;
}

inline double CVProfNode::GetPrevTime()
{
	return m_PrevFrameTime.GetMillisecondsF();
}

inline double CVProfNode::GetPeakTime()
{
	return m_PeakTime.GetMillisecondsF();
}

inline double CVProfNode::GetTotalTimeLessChildren()
{
	double result = GetTotalTime();
	CVProfNode* pChild = GetChild();
	while (pChild)
	{
		result -= pChild->GetTotalTime();
		pChild = pChild->GetSibling();
	}
	return result;
}

inline double CVProfNode::GetCurTimeLessChildren()
{
	double result = GetCurTime();
	CVProfNode* pChild = GetChild();
	while (pChild)
	{
		result -= pChild->GetCurTime();
		pChild = pChild->GetSibling();
	}
	return result;
}

inline double CVProfNode::GetPrevTimeLessChildren()
{
	double result = GetPrevTime();
	CVProfNode* pChild = GetChild();
	while (pChild)
	{
		result -= pChild->GetPrevTime();
		pChild = pChild->GetSibling();
	}
	return result;
}

inline int CVProfNode::GetPrevL2CacheMissLessChildren()
{
	int result = m_iPrevL2CacheMiss;
	CVProfNode* pChild = GetChild();
	while (pChild)
	{
		result -= pChild->m_iPrevL2CacheMiss;
		pChild = pChild->GetSibling();
	}
	return result;
}

inline int CVProfNode::GetPrevLoadHitStoreLessChildren()
{
#ifndef _X360
	return 0;
#else
	int result = m_iPrevLoadHitStores;
	CVProfNode* pChild = GetChild();
	while (pChild)
	{
		result -= pChild->m_iPrevLoadHitStores;
		pChild = pChild->GetSibling();
	}
	return result;
#endif
}


inline void CVProfNode::ClearPrevTime()
{
	m_PrevFrameTime.Init();
}

inline int CVProfNode::GetL2CacheMisses(void)
{
#ifndef _X360
	return m_L2Cache.GetL2CacheMisses();
#else
	return m_iTotalL2CacheMiss;
#endif
}

#ifdef _X360
inline int CVProfNode::GetLoadHitStores(void)
{
	return m_iTotalLoadHitStores;
}
#endif

inline bool CVProfile::IsEnabled() const
{
	return (m_enabled != 0);
}

inline int CVProfile::GetDetailLevel() const
{
	return m_ProfileDetailLevel;
}


inline bool CVProfile::AtRoot() const
{
	return m_fAtRoot;
}

inline void CVProfile::Start()
{
	if (++m_enabled == 1)
	{
		m_Root.EnterScope();
#ifdef	VPROF_VXCONSOLE_EXISTS 
		VXProfileStart();
#endif
#ifdef _X360
		CPMCData::InitializeOnceProgramWide();
#endif
	}
}

inline void CVProfile::Stop()
{
	if (--m_enabled == 0)
		m_Root.ExitScope();
}

inline void CVProfile::EnterScope(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, bool bAssertAccounted, int budgetFlags)
{
	if ((m_enabled != 0 || !m_fAtRoot) && InTargetThread())            
	{
		if (pszName != m_pCurNode->GetName())
		{
			m_pCurNode = m_pCurNode->GetSubNode(pszName, detailLevel, pBudgetGroupName, budgetFlags);
		}
		m_pBudgetGroups[m_pCurNode->GetBudgetGroupID()].m_BudgetFlags |= budgetFlags;

#if defined( _DEBUG ) && !defined( _X360 )
		if (bAssertAccounted)
		{
			AssertOnce(m_pCurNode->GetBudgetGroupID() != 0);
		}
#endif
		m_pCurNode->EnterScope();
		m_fAtRoot = false;
	}
#if defined(_X360) && defined(VPROF_PIX)
	if (m_pCurNode->GetBudgetGroupID() != VPROF_BUDGET_GROUP_ID_UNACCOUNTED)
		PIXBeginNamedEvent(0, pszName);
#endif
}

inline void CVProfile::EnterScope(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, bool bAssertAccounted)
{
	EnterScope(pszName, detailLevel, pBudgetGroupName, bAssertAccounted, BUDGETFLAG_OTHER);
}

inline void CVProfile::ExitScope()
{
#if defined(_X360) && defined(VPROF_PIX)
#ifndef PIXBeginNamedEvent
#error PIX.h must be included if VPROF_PIX is enabled
#endif
	if (m_pCurNode->GetBudgetGroupID() != VPROF_BUDGET_GROUP_ID_UNACCOUNTED)
		PIXEndNamedEvent();
#endif
	if ((!m_fAtRoot || m_enabled != 0) && InTargetThread())
	{
		if (m_pCurNode->ExitScope())
		{
			m_pCurNode = m_pCurNode->GetParent();
		}
		m_fAtRoot = (m_pCurNode == &m_Root);
	}
}

inline void CVProfile::Pause()
{
	m_pausedEnabledDepth = m_enabled;
	m_enabled = 0;
	if (!AtRoot())
		m_Root.Pause();
}

inline void CVProfile::Resume()
{
	m_enabled = m_pausedEnabledDepth;
	if (!AtRoot())
		m_Root.Resume();
}

inline void CVProfile::Reset()
{
	m_Root.Reset();
	m_nFrames = 0;
}

inline void CVProfile::ResetPeaks()
{
	m_Root.ResetPeak();
}

inline void CVProfile::MarkFrame(char const* str)
{
	MarkFrame();
}

inline void CVProfile::MarkFrame()
{
	if (m_enabled)
	{
		++m_nFrames;
		m_Root.ExitScope();
		m_Root.MarkFrame();
		m_Root.EnterScope();

#ifdef _X360
		switch (GetCPUTraceMode())
		{
		case kAllNodesInFrame_WaitingForMark:
			m_iCPUTraceEnabled = kAllNodesInFrame_Recording;
			break;
		case kAllNodesInFrame_WaitingForMarkMultiFrame:
			m_iCPUTraceEnabled = kAllNodesInFrame_RecordingMultiFrame;
			break;
		case kAllNodesInFrame_Recording:
			m_iCPUTraceEnabled = kDisabled;
			Msg("Frame ended. Recording no more CPU traces\n");

			break;
		case kAllNodesInFrame_RecordingMultiFrame:
			if (--m_nFramesRemaining == 0)
			{
				m_iCPUTraceEnabled = kDisabled;
				Msg("Frames ended. Recording no more CPU traces\n");

				SpewWorstMultiFrame();
			}

			++m_nFrameCount;

			break;
		default:
			break;
		}
#endif
	}
}

inline double CVProfile::GetTotalTimeSampled()
{
	return m_Root.GetTotalTime();
}

inline double CVProfile::GetPeakFrameTime()
{
	return m_Root.GetPeakTime();
}

inline double CVProfile::GetTimeLastFrame()
{
	return m_Root.GetCurTime();
}

inline CVProfNode* CVProfile::GetRoot()
{
	return &m_Root;
}

inline CVProfNode* CVProfile::GetCurrentNode()
{
	return m_pCurNode;
}


inline const tchar* CVProfile::GetBudgetGroupName(int budgetGroupID)
{
	Assert(budgetGroupID >= 0 && budgetGroupID < m_nBudgetGroupNames);
	return m_pBudgetGroups[budgetGroupID].m_pName;
}

inline int CVProfile::GetBudgetGroupFlags(int budgetGroupID) const
{
	Assert(budgetGroupID >= 0 && budgetGroupID < m_nBudgetGroupNames);
	return m_pBudgetGroups[budgetGroupID].m_BudgetFlags;
}

#ifdef _X360

inline CVProfile::CPUTraceState CVProfile::GetCPUTraceMode()
{
	return m_iCPUTraceEnabled;
}

inline void CVProfile::SetCPUTraceEnabled(CPUTraceState enabled, bool bTraceCompleteEvent , int nNumFrames  )
{
	m_iCPUTraceEnabled = enabled;
	m_bTraceCompleteEvent = bTraceCompleteEvent;
	if (nNumFrames != -1)
	{
		m_nFramesRemaining = nNumFrames;
		m_nFrameCount = 0;
		m_WorstCycles = 0;
		m_WorstTraceFilename[0] = 0;
	}
}

inline void CVProfile::IncrementMultiTraceIndex()
{
	++m_iSuccessiveTraceIndex;
}

inline unsigned int CVProfile::GetMultiTraceIndex()
{
	return m_iSuccessiveTraceIndex;
}

#endif


inline CVProfScope::CVProfScope(const tchar* pszName, int detailLevel, const tchar* pBudgetGroupName, bool bAssertAccounted, int budgetFlags) :
	CVProfSnMarkerScope(pszName),
	m_bEnabled(g_VProfCurrentProfile.IsEnabled())
{
	if (m_bEnabled)
	{
		g_VProfCurrentProfile.EnterScope(pszName, detailLevel, pBudgetGroupName, bAssertAccounted, budgetFlags);
	}
}

inline CVProfScope::~CVProfScope()
{
	if (m_bEnabled)
	{
		g_VProfCurrentProfile.ExitScope();
	}
}

class CVProfCounter
{
public:
	CVProfCounter(const tchar* pName, CounterGroup_t group = COUNTER_GROUP_DEFAULT)
	{
		m_pCounter = g_VProfCurrentProfile.FindOrCreateCounter(pName, group);
		Assert(m_pCounter);
	}
	~CVProfCounter()
	{
	}
	void Increment(int val)
	{
		Assert(m_pCounter);
		*m_pCounter += val;
	}
	void Set(int val)
	{
		Assert(m_pCounter);
		*m_pCounter = val;
	}
private:
	int* m_pCounter;
};

#endif

#ifdef _X360

#include "xbox/xbox_console.h"
#include "tracerecording.h"
#include  "tier1/fmtstr.h"
#pragma comment( lib, "tracerecording.lib" )
#pragma comment( lib, "xbdm.lib" )

class CPIXRecorder
{
public:
	CPIXRecorder() : m_bActive(false) {}
	~CPIXRecorder() { Stop(); }

	void Start(const char* pszFilename = "capture")
	{
		if (!m_bActive)
		{
			if (!XTraceStartRecording(CFmtStr("e:\\%s.pix2", pszFilename)))
			{
				Msg("XTraceStartRecording failed, error code %d\n", GetLastError());
			}
			else
			{
				m_bActive = true;
			}
		}
	}

	void Stop()
	{
		if (m_bActive)
		{
			m_bActive = false;
			if (XTraceStopRecording())
			{
				Msg("CPU trace finished.\n");
				XBX_rTraceComplete();
			}
		}
	}

private:
	bool m_bActive;
};

#define VPROF_BEGIN_PIX_BLOCK( convar ) \
	{ \
	bool bRunPix = 0; \
	static CFastTimer PIXTimer; \
	extern ConVar convar; \
	ConVar &PIXConvar = convar; \
	CPIXRecorder PIXRecorder; \
		{ \
		PIXLabel: \
			if ( bRunPix ) \
			{ \
				PIXRecorder.Start(); \
			} \
			else \
			{ \
				if ( PIXConvar.GetBool() ) \
				{ \
					PIXTimer.Start(); \
				} \
			} \
				{


#define VPROF_END_PIX_BLOCK() \
				} \
			\
			if ( !bRunPix ) \
			{ \
				if ( PIXConvar.GetBool() ) \
				{ \
					PIXTimer.End(); \
					if ( PIXTimer.GetDuration().GetMillisecondsF() > PIXConvar.GetFloat() ) \
					{ \
						PIXConvar.SetValue( 0 ); \
						bRunPix = true; \
						goto PIXLabel; \
					} \
				} \
			} \
			else \
			{ \
				PIXRecorder.Stop(); \
			} \
		} \
	}
#else
#define VPROF_BEGIN_PIX_BLOCK( PIXConvar ) {
#define VPROF_END_PIX_BLOCK() }
#endif


#ifdef VPROF_UNDO_PIX
#undef USE_PIX
#undef _PIX_H_
#undef PIXBeginNamedEvent
#undef PIXEndNamedEvent
#undef PIXSetMarker
#undef PIXNameThread
#include <pix.h>
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif   

#endif

