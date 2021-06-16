#ifndef IMDLCACHE_H
#define IMDLCACHE_H

#ifdef _WIN32
#pragma once
#endif

#include "IAppSystem.h"

struct studiohdr_t;
struct studiohwdata_t;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

namespace OptimizedModel
{
	struct FileHeader_t;
}


typedef unsigned short MDLHandle_t;

enum
{
	MDLHANDLE_INVALID = (MDLHandle_t)~0
};


enum MDLCacheDataType_t
{
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,

	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK,
};

abstract_class IMDLCacheNotify
{
public:
	virtual void OnDataLoaded(MDLCacheDataType_t type, MDLHandle_t handle) = 0;

	virtual void OnDataUnloaded(MDLCacheDataType_t type, MDLHandle_t handle) = 0;
};


enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,

	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

#define MDLCACHE_INTERFACE_VERSION "MDLCache004"

abstract_class IMDLCache : public IAppSystem
{
public:
	virtual void SetCacheNotify(IMDLCacheNotify * pNotify) = 0;

	virtual MDLHandle_t FindMDL(const char* pMDLRelativePath) = 0;

	virtual int AddRef(MDLHandle_t handle) = 0;
	virtual int Release(MDLHandle_t handle) = 0;
	virtual int GetRef(MDLHandle_t handle) = 0;

	virtual studiohdr_t* GetStudioHdr(MDLHandle_t handle) = 0;
	virtual studiohwdata_t* GetHardwareData(MDLHandle_t handle) = 0;
	virtual vcollide_t* GetVCollide(MDLHandle_t handle) = 0;
	virtual unsigned char* GetAnimBlock(MDLHandle_t handle, int nBlock) = 0;
	virtual virtualmodel_t* GetVirtualModel(MDLHandle_t handle) = 0;
	virtual int GetAutoplayList(MDLHandle_t handle, unsigned short** pOut) = 0;
	virtual vertexFileHeader_t* GetVertexData(MDLHandle_t handle) = 0;

	virtual void TouchAllData(MDLHandle_t handle) = 0;

	virtual void SetUserData(MDLHandle_t handle, void* pData) = 0;
	virtual void* GetUserData(MDLHandle_t handle) = 0;

	virtual bool IsErrorModel(MDLHandle_t handle) = 0;

	virtual void Flush(MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	virtual void Flush(MDLHandle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	virtual const char* GetModelName(MDLHandle_t handle) = 0;

	virtual virtualmodel_t* GetVirtualModelFast(const studiohdr_t* pStudioHdr, MDLHandle_t handle) = 0;

	virtual void BeginLock() = 0;

	virtual void EndLock() = 0;

	virtual int* GetFrameUnlockCounterPtrOLD() = 0;

	virtual void FinishPendingLoads() = 0;

	virtual vcollide_t* GetVCollideEx(MDLHandle_t handle, bool synchronousLoad = true) = 0;
	virtual bool GetVCollideSize(MDLHandle_t handle, int* pVCollideSize) = 0;

	virtual bool GetAsyncLoad(MDLCacheDataType_t type) = 0;
	virtual bool SetAsyncLoad(MDLCacheDataType_t type, bool bAsync) = 0;

	virtual void BeginMapLoad() = 0;
	virtual void EndMapLoad() = 0;
	virtual void MarkAsLoaded(MDLHandle_t handle) = 0;

	virtual void InitPreloadData(bool rebuild) = 0;
	virtual void ShutdownPreloadData() = 0;

	virtual bool IsDataLoaded(MDLHandle_t handle, MDLCacheDataType_t type) = 0;

	virtual int* GetFrameUnlockCounterPtr(MDLCacheDataType_t type) = 0;

	virtual studiohdr_t* LockStudioHdr(MDLHandle_t handle) = 0;
	virtual void UnlockStudioHdr(MDLHandle_t handle) = 0;

	virtual bool PreloadModel(MDLHandle_t handle) = 0;

	virtual void ResetErrorModelStatus(MDLHandle_t handle) = 0;

	virtual void MarkFrame() = 0;
};


class CMDLCacheCriticalSection
{
public:
	CMDLCacheCriticalSection(IMDLCache* pCache) : m_pCache(pCache)
	{
		m_pCache->BeginLock();
	}

	~CMDLCacheCriticalSection()
	{
		m_pCache->EndLock();
	}

private:
	IMDLCache* m_pCache;
};

#define MDCACHE_FINE_GRAINED 1

#if defined(MDCACHE_FINE_GRAINED)
#define MDLCACHE_CRITICAL_SECTION_( pCache ) CMDLCacheCriticalSection cacheCriticalSection(pCache)
#define MDLCACHE_COARSE_LOCK_( pCache ) ((void)(0))
#elif defined(MDLCACHE_LEVEL_LOCKED)
#define MDLCACHE_CRITICAL_SECTION_( pCache )  ((void)(0))
#define MDLCACHE_COARSE_LOCK_( pCache ) ((void)(0))
#else
#define MDLCACHE_CRITICAL_SECTION_( pCache ) ((void)(0))
#define MDLCACHE_COARSE_LOCK_( pCache ) CMDLCacheCriticalSection cacheCriticalSection(pCache)
#endif
#define MDLCACHE_CRITICAL_SECTION() MDLCACHE_CRITICAL_SECTION_(mdlcache)
#define MDLCACHE_COARSE_LOCK() MDLCACHE_COARSE_LOCK_(mdlcache)

#endif  