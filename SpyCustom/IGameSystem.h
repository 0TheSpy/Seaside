#ifndef IGAMESYSTEM_H
#define IGAMESYSTEM_H
#ifdef _WIN32
#pragma once
#endif


class IGameSystem
{
public:
	virtual char const* Name() = 0;

	virtual bool Init() = 0;
	virtual void PostInit() = 0;
	virtual void Shutdown() = 0;

	virtual void LevelInitPreEntity() = 0;
	virtual void LevelInitPostEntity() = 0;

	virtual void LevelShutdownPreClearSteamAPIContext() {};
	virtual void LevelShutdownPreEntity() = 0;
	virtual void LevelShutdownPostEntity() = 0;
	virtual void OnSave() = 0;

	virtual void OnRestore() = 0;

	virtual void SafeRemoveIfDesired() = 0;

	virtual bool	IsPerFrame() = 0;

	virtual ~IGameSystem();

	static char const* MapName();

	static void Add(IGameSystem* pSys);
	static void Remove(IGameSystem* pSys);
	static void RemoveAll();

	static bool InitAllSystems();
	static void PostInitAllSystems();
	static void ShutdownAllSystems();
	static void LevelInitPreEntityAllSystems(char const* pMapName);
	static void LevelInitPostEntityAllSystems();
	static void LevelShutdownPreClearSteamAPIContextAllSystems();     
	static void LevelShutdownPreEntityAllSystems();
	static void LevelShutdownPostEntityAllSystems();

	static void OnSaveAllSystems();
	static void OnRestoreAllSystems();

	static void SafeRemoveIfDesiredAllSystems();

#ifdef CLIENT_DLL
	static void PreRenderAllSystems();
	static void UpdateAllSystems(float frametime);
	static void PostRenderAllSystems();
#else
	static void FrameUpdatePreEntityThinkAllSystems();
	static void FrameUpdatePostEntityThinkAllSystems();
	static void PreClientUpdateAllSystems();

	static CBasePlayer* RunCommandPlayer();
	static CUserCmd* RunCommandUserCmd();
#endif
};

class IGameSystemPerFrame : public IGameSystem
{
public:
	virtual ~IGameSystemPerFrame();

#ifdef CLIENT_DLL
	virtual void PreRender() = 0;

	virtual void Update(float frametime) = 0;

	virtual void PostRender() = 0;
#else
	virtual void FrameUpdatePreEntityThink() = 0;
	virtual void FrameUpdatePostEntityThink() = 0;
	virtual void PreClientUpdate() = 0;
#endif
};

class CBaseGameSystem : public IGameSystem
{
public:

	virtual char const* Name() { return "unnamed"; }

	virtual bool Init() { return true; }
	virtual void PostInit() {}
	virtual void Shutdown() {}

	virtual void LevelInitPreEntity() {}
	virtual void LevelInitPostEntity() {}
	virtual void LevelShutdownPreClearSteamAPIContext() {}
	virtual void LevelShutdownPreEntity() {}
	virtual void LevelShutdownPostEntity() {}

	virtual void OnSave() {}
	virtual void OnRestore() {}
	virtual void SafeRemoveIfDesired() {}

	virtual bool	IsPerFrame() { return false; }
private:

#ifdef CLIENT_DLL
	virtual void PreRender() {}

	virtual void Update(float frametime) {}

	virtual void PostRender() {}
#else
	virtual void FrameUpdatePreEntityThink() {}
	virtual void FrameUpdatePostEntityThink() {}
	virtual void PreClientUpdate() {}
#endif
};

class CBaseGameSystemPerFrame : public IGameSystemPerFrame
{
public:
	virtual char const* Name() { return "unnamed"; }

	virtual bool Init() { return true; }
	virtual void PostInit() {}
	virtual void Shutdown() {}

	virtual void LevelInitPreEntity() {}
	virtual void LevelInitPostEntity() {}
	virtual void LevelShutdownPreClearSteamAPIContext() {}
	virtual void LevelShutdownPreEntity() {}
	virtual void LevelShutdownPostEntity() {}

	virtual void OnSave() {}
	virtual void OnRestore() {}
	virtual void SafeRemoveIfDesired() {}

	virtual bool	IsPerFrame() { return true; }

#ifdef CLIENT_DLL
	virtual void PreRender() { }

	virtual void Update(float frametime) { }

	virtual void PostRender() { }
#else
	virtual void FrameUpdatePreEntityThink() { }
	virtual void FrameUpdatePostEntityThink() { }
	virtual void PreClientUpdate() { }
#endif
};

class CAutoGameSystem : public CBaseGameSystem
{
public:
	CAutoGameSystem(char const* name = NULL);	         
	CAutoGameSystem* m_pNext;

	virtual char const* Name() { return m_pszName ? m_pszName : "unnamed"; }

private:
	char const* m_pszName;
};

class CAutoGameSystemPerFrame : public CBaseGameSystemPerFrame
{
public:
	CAutoGameSystemPerFrame(char const* name = NULL);
	CAutoGameSystemPerFrame* m_pNext;

	virtual char const* Name() { return m_pszName ? m_pszName : "unnamed"; }

private:
	char const* m_pszName;
};


class IToolFrameworkServer
{
public:
	virtual void PreSetupVisibility() = 0;
};

#endif  