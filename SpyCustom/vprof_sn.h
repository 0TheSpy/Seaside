#ifndef TIER_V0PROF_SN_HDR
#define TIER_V0PROF_SN_HDR

#if defined( SN_TARGET_PS3 ) && !defined(_CERT)
extern "C" void(*g_pfnPushMarker)(const char* pName);
extern "C" void(*g_pfnPopMarker)();

class CVProfSnMarkerScope
{
public:
	CVProfSnMarkerScope(const char* pszName)
	{
		g_pfnPushMarker(pszName);
	}
	~CVProfSnMarkerScope()
	{
		g_pfnPopMarker();
	}
};


#define SNPROF(name) ((void)0) 
#define SNPROF_ANIM(name) ((void)0)

#else

class CVProfSnMarkerScope { public: CVProfSnMarkerScope(const char*) {} };

#define SNPROF(name) TM_ZONE( TELEMETRY_LEVEL1, TMZF_NONE, "%s", name );
#define SNPROF_ANIM(name) TM_ZONE( TELEMETRY_LEVEL1, TMZF_NONE, "anim %s", name );

#endif

#endif