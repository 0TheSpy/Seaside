#ifndef IDATACACHE_H
#define IDATACACHE_H

#ifdef _WIN32
#pragma once
#endif


#include "dbg.h"
#include "IAppSystem.h"

class IDataCache;

#define DATACACHE_INTERFACE_VERSION		"VDataCache003"

typedef uint32 DataCacheClientID_t;


FORWARD_DECLARE_HANDLE(memhandle_t);
typedef memhandle_t DataCacheHandle_t;
#define DC_INVALID_HANDLE ((DataCacheHandle_t)0)

struct DataCacheLimits_t
{
	DataCacheLimits_t(unsigned _nMaxBytes = (unsigned)-1, unsigned _nMaxItems = (unsigned)-1, unsigned _nMinBytes = 0, unsigned _nMinItems = 0)
		: nMaxBytes(_nMaxBytes),
		nMaxItems(_nMaxItems),
		nMinBytes(_nMinBytes),
		nMinItems(_nMinItems)
	{
	}

	unsigned nMaxBytes;
	unsigned nMaxItems;

	unsigned nMinBytes;
	unsigned nMinItems;
};

struct DataCacheStatus_t
{
	unsigned nBytes;
	unsigned nItems;

	unsigned nBytesLocked;
	unsigned nItemsLocked;

	unsigned nFindRequests;
	unsigned nFindHits;
};

enum DataCacheOptions_t
{
	DC_TRACE_ACTIVITY = (1 << 0),
	DC_FORCE_RELOCATE = (1 << 1),
	DC_ALWAYS_MISS = (1 << 2),
	DC_VALIDATE = (1 << 3),
};


enum DataCacheReportType_t
{
	DC_SUMMARY_REPORT,
	DC_DETAIL_REPORT,
	DC_DETAIL_REPORT_LRU,
};


enum DataCacheNotificationType_t
{
	DC_NONE,

	DC_AGE_DISCARD,

	DC_FLUSH_DISCARD,

	DC_REMOVED,

	DC_RELOCATE,

	DC_PRINT_INF0,
};

struct DataCacheNotification_t
{
	DataCacheNotificationType_t type;
	const char* pszSectionName;
	DataCacheClientID_t			clientId;
	const void* pItemData;
	unsigned					nItemSize;
};

const int DC_MAX_CLIENT_NAME = 15;
const int DC_MAX_ITEM_NAME = 511;

enum DataCacheRemoveResult_t
{
	DC_OK,
	DC_NOT_FOUND,
	DC_LOCKED,
};

enum DataCacheAddFlags_t
{
	DCAF_LOCK = (1 << 0),
	DCAF_DEFAULT = 0,
};



abstract_class IDataCacheSection
{
public:
	virtual IDataCache * GetSharedCache() = 0;
	virtual const char* GetName() = 0;

	virtual void SetLimits(const DataCacheLimits_t& limits) = 0;
	virtual void SetOptions(unsigned options) = 0;


	virtual void GetStatus(DataCacheStatus_t* pStatus, DataCacheLimits_t* pLimits = NULL) = 0;


	virtual void EnsureCapacity(unsigned nBytes, unsigned nItems = 1) = 0;


	virtual bool Add(DataCacheClientID_t clientId, const void* pItemData, unsigned size, DataCacheHandle_t* pHandle) = 0;

	virtual DataCacheHandle_t Find(DataCacheClientID_t clientId) = 0;


	virtual DataCacheRemoveResult_t Remove(DataCacheHandle_t handle, const void** ppItemData, unsigned* pItemSize = NULL, bool bNotify = false) = 0;
	DataCacheRemoveResult_t Remove(DataCacheHandle_t handle, bool bNotify = false) { return Remove(handle, NULL, NULL, bNotify); }


	virtual bool IsPresent(DataCacheHandle_t handle) = 0;


	virtual void* Lock(DataCacheHandle_t handle) = 0;


	virtual int Unlock(DataCacheHandle_t handle) = 0;


	virtual void* Get(DataCacheHandle_t handle, bool bFrameLock = false) = 0;
	virtual void* GetNoTouch(DataCacheHandle_t handle, bool bFrameLock = false) = 0;

	virtual int BeginFrameLocking() = 0;
	virtual bool IsFrameLocking() = 0;
	virtual void* FrameLock(DataCacheHandle_t handle) = 0;
	virtual int EndFrameLocking() = 0;
	virtual int* GetFrameUnlockCounterPtr() = 0;


	virtual int GetLockCount(DataCacheHandle_t handle) = 0;
	virtual int BreakLock(DataCacheHandle_t handle) = 0;


	virtual bool Touch(DataCacheHandle_t handle) = 0;


	virtual bool Age(DataCacheHandle_t handle) = 0;


	virtual unsigned Flush(bool bUnlockedOnly = true, bool bNotify = true) = 0;


	virtual unsigned Purge(unsigned nBytes) = 0;


	virtual void OutputReport(DataCacheReportType_t reportType = DC_SUMMARY_REPORT) = 0;

	virtual void UpdateSize(DataCacheHandle_t handle, unsigned int nNewSize) = 0;


	virtual void LockMutex() = 0;
	virtual void UnlockMutex() = 0;

	virtual bool AddEx(DataCacheClientID_t clientId, const void* pItemData, unsigned size, unsigned flags, DataCacheHandle_t* pHandle) = 0;
};


abstract_class IDataCacheClient
{
public:
	virtual bool HandleCacheNotification(const DataCacheNotification_t & notification) = 0;


	virtual bool GetItemName(DataCacheClientID_t clientId, const void* pItem, char* pDest, unsigned nMaxLen) = 0;
};

class CDefaultDataCacheClient : public IDataCacheClient
{
public:
	virtual bool HandleCacheNotification(const DataCacheNotification_t& notification)
	{
		switch (notification.type)
		{
		case DC_AGE_DISCARD:
		case DC_FLUSH_DISCARD:
		case DC_REMOVED:
		default:
			Assert(0);
			return false;
		}
		return false;
	}

	virtual bool GetItemName(DataCacheClientID_t clientId, const void* pItem, char* pDest, unsigned nMaxLen)
	{
		return false;
	}
};


abstract_class IDataCache : public IAppSystem
{
public:
	virtual void SetSize(int nMaxBytes) = 0;
	virtual void SetOptions(unsigned options) = 0;
	virtual void SetSectionLimits(const char* pszSectionName, const DataCacheLimits_t& limits) = 0;


	virtual void GetStatus(DataCacheStatus_t* pStatus, DataCacheLimits_t* pLimits = NULL) = 0;


	virtual IDataCacheSection* AddSection(IDataCacheClient* pClient, const char* pszSectionName, const DataCacheLimits_t& limits = DataCacheLimits_t(), bool bSupportFastFind = false) = 0;


	virtual void RemoveSection(const char* pszClientName, bool bCallFlush = true) = 0;
	void RemoveSection(IDataCacheSection* pSection, bool bCallFlush = true) { if (pSection) RemoveSection(pSection->GetName()); }


	virtual IDataCacheSection* FindSection(const char* pszClientName) = 0;


	virtual unsigned Purge(unsigned nBytes) = 0;


	virtual unsigned Flush(bool bUnlockedOnly = true, bool bNotify = true) = 0;


	virtual void OutputReport(DataCacheReportType_t reportType = DC_SUMMARY_REPORT, const char* pszSection = NULL) = 0;
};

template< class STORAGE_TYPE, class CREATE_PARAMS, class LOCK_TYPE = STORAGE_TYPE* >
class CManagedDataCacheClient : public CDefaultDataCacheClient
{
public:
	typedef CManagedDataCacheClient<STORAGE_TYPE, CREATE_PARAMS, LOCK_TYPE> CCacheClientBaseClass;

	CManagedDataCacheClient()
		: m_pCache(NULL)
	{
	}

	void Init(IDataCache* pSharedCache, const char* pszSectionName, const DataCacheLimits_t& limits = DataCacheLimits_t(), bool bSupportFastFind = false)
	{
		if (!m_pCache)
		{
			m_pCache = pSharedCache->AddSection(this, pszSectionName, limits, bSupportFastFind);
		}
	}

	void Shutdown()
	{
		if (m_pCache)
		{
			m_pCache->GetSharedCache()->RemoveSection(m_pCache);
			m_pCache = NULL;
		}
	}

	LOCK_TYPE CacheGet(DataCacheHandle_t handle, bool bFrameLock = true)
	{
		return (LOCK_TYPE)(((STORAGE_TYPE*)m_pCache->Get(handle, bFrameLock))->GetData());
	}

	LOCK_TYPE CacheGetNoTouch(DataCacheHandle_t handle)
	{
		return (LOCK_TYPE)(((STORAGE_TYPE*)m_pCache->GetNoTouch(handle))->GetData());
	}

	LOCK_TYPE CacheLock(DataCacheHandle_t handle)
	{
		return (LOCK_TYPE)(((STORAGE_TYPE*)m_pCache->Lock(handle))->GetData());
	}

	int CacheUnlock(DataCacheHandle_t handle)
	{
		return m_pCache->Unlock(handle);
	}

	void CacheTouch(DataCacheHandle_t handle)
	{
		m_pCache->Touch(handle);
	}

	void CacheRemove(DataCacheHandle_t handle, bool bNotify = true)
	{
		m_pCache->Remove(handle, bNotify);
	}

	void CacheFlush()
	{
		m_pCache->Flush();
	}

	DataCacheHandle_t CacheCreate(const CREATE_PARAMS& createParams, unsigned flags = DCAF_DEFAULT)
	{
		m_pCache->EnsureCapacity(STORAGE_TYPE::EstimatedSize(createParams));
		STORAGE_TYPE* pStore = STORAGE_TYPE::CreateResource(createParams);
		DataCacheHandle_t handle;
		m_pCache->AddEx((DataCacheClientID_t)pStore, pStore, pStore->Size(), flags, &handle);
		return handle;
	}

	void CacheLockMutex()
	{
		m_pCache->LockMutex();
	}

	void CacheUnlockMutex()
	{
		m_pCache->UnlockMutex();
	}

	bool HandleCacheNotification(const DataCacheNotification_t& notification)
	{
		switch (notification.type)
		{
		case DC_AGE_DISCARD:
		case DC_FLUSH_DISCARD:
		case DC_REMOVED:
		{
			STORAGE_TYPE* p = (STORAGE_TYPE*)notification.clientId;
			p->DestroyResource();
		}
		return true;
		default:
			return CDefaultDataCacheClient::HandleCacheNotification(notification);
		}
	}


protected:

	~CManagedDataCacheClient()
	{
		Shutdown();
	}

	IDataCacheSection* GetCacheSection()
	{
		return m_pCache;
	}

private:
	IDataCacheSection* m_pCache;

};

#endif  