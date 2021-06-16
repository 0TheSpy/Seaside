#ifndef UTLTSHASH_H
#define UTLTSHASH_H

#ifdef _WIN32
#pragma once
#endif

#include <limits.h>
#include "threadtools.h"
#include "mempool.h"
#include "generichash.h"


typedef intp UtlTSHashHandle_t;

template < class T >
abstract_class ITSHashConstructor
{
public:
	virtual void Construct(T * pElement) = 0;
};

template < class T >
class CDefaultTSHashConstructor : public ITSHashConstructor< T >
{
public:
	virtual void Construct(T* pElement)
	{
		::Construct(pElement);
	}
};

template < int BUCKET_COUNT, class KEYTYPE = intp >
class CUtlTSHashGenericHash
{
public:
	static int Hash(const KEYTYPE& key, int nBucketMask)
	{
		int nHash = HashIntConventional((intp)key);
		if (BUCKET_COUNT <= USHRT_MAX)
		{
			nHash ^= (nHash >> 16);
		}
		if (BUCKET_COUNT <= UCHAR_MAX)
		{
			nHash ^= (nHash >> 8);
		}
		return (nHash & nBucketMask);
	}

	static bool Compare(const KEYTYPE& lhs, const KEYTYPE& rhs)
	{
		return lhs == rhs;
	}
};

template < int BUCKET_COUNT, class KEYTYPE >
class CUtlTSHashUseKeyHashMethod
{
public:
	static int Hash(const KEYTYPE& key, int nBucketMask)
	{
		uint32 nHash = key.HashValue();
		return (nHash & nBucketMask);
	}

	static bool Compare(const KEYTYPE& lhs, const KEYTYPE& rhs)
	{
		return lhs == rhs;
	}
};

template< class T, int BUCKET_COUNT, class KEYTYPE = intp, class HashFuncs = CUtlTSHashGenericHash< BUCKET_COUNT, KEYTYPE >, int nAlignment = 0 >
class CUtlTSHash
{
public:
	CUtlTSHash(int nAllocationCount);
	~CUtlTSHash();

	static UtlTSHashHandle_t InvalidHandle(void) { return (UtlTSHashHandle_t)0; }

	UtlTSHashHandle_t Find(KEYTYPE uiKey);

	UtlTSHashHandle_t Insert(KEYTYPE uiKey, const T& data, bool* pDidInsert = NULL);
	UtlTSHashHandle_t Insert(KEYTYPE uiKey, ITSHashConstructor<T>* pConstructor, bool* pDidInsert = NULL);

	UtlTSHashHandle_t FastInsert(KEYTYPE uiKey, const T& data);
	UtlTSHashHandle_t FastInsert(KEYTYPE uiKey, ITSHashConstructor<T>* pConstructor);

	void Commit();

	void FindAndRemove(KEYTYPE uiKey);
	void Remove(UtlTSHashHandle_t hHash) { FindAndRemove(GetID(hHash)); }
	void RemoveAll(void);
	void Purge(void);

	int Count() const;

	int GetElements(int nFirstElement, int nCount, UtlTSHashHandle_t* pHandles) const;

	T& Element(UtlTSHashHandle_t hHash);
	T const& Element(UtlTSHashHandle_t hHash) const;
	T& operator[](UtlTSHashHandle_t hHash);
	T const& operator[](UtlTSHashHandle_t hHash) const;
	KEYTYPE GetID(UtlTSHashHandle_t hHash) const;

	UtlTSHashHandle_t ElementPtrToHandle(T* pElement) const;

private:
	template < typename Data_t >
	struct HashFixedDataInternal_t
	{
		KEYTYPE	m_uiKey;
		HashFixedDataInternal_t< Data_t >* m_pNext;
		Data_t	m_Data;
	};

	typedef HashFixedDataInternal_t<T> HashFixedData_t;

	enum
	{
		BUCKET_MASK = BUCKET_COUNT - 1
	};

	struct HashBucket_t
	{
		HashFixedData_t* m_pFirst;
		HashFixedData_t* m_pFirstUncommitted;
		CThreadSpinRWLock m_AddLock;
	};

	UtlTSHashHandle_t Find(KEYTYPE uiKey, HashFixedData_t* pFirstElement, HashFixedData_t* pLastElement);
	UtlTSHashHandle_t InsertUncommitted(KEYTYPE uiKey, HashBucket_t& bucket);
	CMemoryPoolMT m_EntryMemory;
	HashBucket_t m_aBuckets[BUCKET_COUNT];
	bool m_bNeedsCommit;

#ifdef _DEBUG
	CInterlockedInt m_ContentionCheck;
#endif
};


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::CUtlTSHash(int nAllocationCount) :
	m_EntryMemory(sizeof(HashFixedData_t), nAllocationCount, CUtlMemoryPool::GROW_SLOW, MEM_ALLOC_CLASSNAME(HashFixedData_t), nAlignment)
{
#ifdef _DEBUG
	m_ContentionCheck = 0;
#endif
	m_bNeedsCommit = false;
	for (int i = 0; i < BUCKET_COUNT; i++)
	{
		HashBucket_t& bucket = m_aBuckets[i];
		bucket.m_pFirst = NULL;
		bucket.m_pFirstUncommitted = NULL;
	}
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::~CUtlTSHash()
{
#ifdef _DEBUG
	if (m_ContentionCheck != 0)
	{
		DebuggerBreak();
	}
#endif
	Purge();
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline void CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Purge(void)
{
	RemoveAll();
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline int CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Count() const
{
	return m_EntryMemory.Count();
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
int CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::GetElements(int nFirstElement, int nCount, UtlTSHashHandle_t* pHandles) const
{
	int nIndex = 0;
	for (int i = 0; i < BUCKET_COUNT; i++)
	{
		const HashBucket_t& bucket = m_aBuckets[i];
		bucket.m_AddLock.LockForRead();
		for (HashFixedData_t* pElement = bucket.m_pFirstUncommitted; pElement; pElement = pElement->m_pNext)
		{
			if (--nFirstElement >= 0)
				continue;

			pHandles[nIndex++] = (UtlTSHashHandle_t)pElement;
			if (nIndex >= nCount)
			{
				bucket.m_AddLock.UnlockRead();
				return nIndex;
			}
		}
		bucket.m_AddLock.UnlockRead();
	}
	return nIndex;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::InsertUncommitted(KEYTYPE uiKey, HashBucket_t& bucket)
{
	m_bNeedsCommit = true;
	HashFixedData_t* pNewElement = static_cast<HashFixedData_t*>(m_EntryMemory.Alloc());
	pNewElement->m_pNext = bucket.m_pFirstUncommitted;
	bucket.m_pFirstUncommitted = pNewElement;
	pNewElement->m_uiKey = uiKey;
	return (UtlTSHashHandle_t)pNewElement;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Insert(KEYTYPE uiKey, const T& data, bool* pDidInsert)
{
#ifdef _DEBUG
	if (m_ContentionCheck != 0)
	{
		DebuggerBreak();
	}
#endif

	if (pDidInsert)
	{
		*pDidInsert = false;
	}

	int iBucket = HashFuncs::Hash(uiKey, BUCKET_MASK);
	HashBucket_t& bucket = m_aBuckets[iBucket];

	UtlTSHashHandle_t h = Find(uiKey);
	if (h != InvalidHandle())
		return h;

	bucket.m_AddLock.LockForWrite();

	h = Find(uiKey, bucket.m_pFirstUncommitted, bucket.m_pFirst);
	if (h == InvalidHandle())
	{
		h = InsertUncommitted(uiKey, bucket);
		CopyConstruct(&Element(h), data);
		if (pDidInsert)
		{
			*pDidInsert = true;
		}
	}

	bucket.m_AddLock.UnlockWrite();
	return h;
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Insert(KEYTYPE uiKey, ITSHashConstructor<T>* pConstructor, bool* pDidInsert)
{
#ifdef _DEBUG
	if (m_ContentionCheck != 0)
	{
		DebuggerBreak();
	}
#endif

	if (pDidInsert)
	{
		*pDidInsert = false;
	}

	UtlTSHashHandle_t h = Find(uiKey);
	if (h != InvalidHandle())
		return h;

	int iBucket = HashFuncs::Hash(uiKey, BUCKET_MASK);
	HashBucket_t& bucket = m_aBuckets[iBucket];
	bucket.m_AddLock.LockForWrite();

	h = Find(uiKey, bucket.m_pFirstUncommitted, bucket.m_pFirst);
	if (h == InvalidHandle())
	{
		h = InsertUncommitted(uiKey, bucket);
		pConstructor->Construct(&Element(h));
		if (pDidInsert)
		{
			*pDidInsert = true;
		}
	}

	bucket.m_AddLock.UnlockWrite();
	return h;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::FastInsert(KEYTYPE uiKey, const T& data)
{
#ifdef _DEBUG
	if (m_ContentionCheck != 0)
	{
		DebuggerBreak();
	}
#endif
	int iBucket = HashFuncs::Hash(uiKey, BUCKET_MASK);
	HashBucket_t& bucket = m_aBuckets[iBucket];
	bucket.m_AddLock.LockForWrite();
	UtlTSHashHandle_t h = InsertUncommitted(uiKey, bucket);
	CopyConstruct(&Element(h), data);
	bucket.m_AddLock.UnlockWrite();
	return h;
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::FastInsert(KEYTYPE uiKey, ITSHashConstructor<T>* pConstructor)
{
#ifdef _DEBUG
	if (m_ContentionCheck != 0)
	{
		DebuggerBreak();
	}
#endif
	int iBucket = HashFuncs::Hash(uiKey, BUCKET_MASK);
	HashBucket_t& bucket = m_aBuckets[iBucket];
	bucket.m_AddLock.LockForWrite();
	UtlTSHashHandle_t h = InsertUncommitted(uiKey, bucket);
	pConstructor->Construct(&Element(h));
	bucket.m_AddLock.UnlockWrite();
	return h;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline void CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Commit()
{
	if (!m_bNeedsCommit)
		return;

#ifdef _DEBUG
	m_ContentionCheck++;
#endif

	for (int i = 0; i < BUCKET_COUNT; i++)
	{
		HashBucket_t& bucket = m_aBuckets[i];
		bucket.m_AddLock.LockForRead();
		bucket.m_pFirst = bucket.m_pFirstUncommitted;
		bucket.m_AddLock.UnlockRead();
	}

	m_bNeedsCommit = false;

#ifdef _DEBUG
	m_ContentionCheck--;
#endif
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline void CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::FindAndRemove(KEYTYPE uiKey)
{
	if (m_EntryMemory.Count() == 0)
		return;

#ifdef _DEBUG
	m_ContentionCheck++;
#endif

	int iBucket = HashFuncs::Hash(uiKey, BUCKET_MASK);
	HashBucket_t& bucket = m_aBuckets[iBucket];
	bucket.m_AddLock.LockForWrite();

	HashFixedData_t* pPrev = NULL;
	for (HashFixedData_t* pElement = bucket.m_pFirstUncommitted; pElement; pPrev = pElement, pElement = pElement->m_pNext)
	{
		if (!HashFuncs::Compare(pElement->m_uiKey, uiKey))
			continue;

		if (pPrev)
		{
			pPrev->m_pNext = pElement->m_pNext;
		}
		else
		{
			bucket.m_pFirstUncommitted = pElement->m_pNext;
		}

		if (bucket.m_pFirst == pElement)
		{
			bucket.m_pFirst = bucket.m_pFirst->m_pNext;
		}

		Destruct(&pElement->m_Data);

#ifdef _DEBUG
		memset(pElement, 0xDD, sizeof(HashFixedData_t));
#endif

		m_EntryMemory.Free(pElement);

		break;
	}

	bucket.m_AddLock.UnlockWrite();

#ifdef _DEBUG
	m_ContentionCheck--;
#endif
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline void CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::RemoveAll(void)
{
	m_bNeedsCommit = false;
	if (m_EntryMemory.Count() == 0)
		return;

#ifdef _DEBUG
	m_ContentionCheck++;
#endif

	for (int i = 0; i < BUCKET_COUNT; i++)
	{
		HashBucket_t& bucket = m_aBuckets[i];

		bucket.m_AddLock.LockForWrite();

		for (HashFixedData_t* pElement = bucket.m_pFirstUncommitted; pElement; pElement = pElement->m_pNext)
		{
			Destruct(&pElement->m_Data);
		}

		bucket.m_pFirst = NULL;
		bucket.m_pFirstUncommitted = NULL;
		bucket.m_AddLock.UnlockWrite();
	}

	m_EntryMemory.Clear();

#ifdef _DEBUG
	m_ContentionCheck--;
#endif
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Find(KEYTYPE uiKey, HashFixedData_t* pFirstElement, HashFixedData_t* pLastElement)
{
#ifdef _DEBUG
	if (m_ContentionCheck != 0)
	{
		DebuggerBreak();
	}
#endif

	for (HashFixedData_t* pElement = pFirstElement; pElement != pLastElement; pElement = pElement->m_pNext)
	{
		if (HashFuncs::Compare(pElement->m_uiKey, uiKey))
			return (UtlTSHashHandle_t)pElement;
	}
	return InvalidHandle();
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Find(KEYTYPE uiKey)
{
	int iBucket = HashFuncs::Hash(uiKey, BUCKET_MASK);
	const HashBucket_t& bucket = m_aBuckets[iBucket];
	UtlTSHashHandle_t h = Find(uiKey, bucket.m_pFirst, NULL);
	if (h != InvalidHandle())
		return h;

	bucket.m_AddLock.LockForRead();
	h = Find(uiKey, bucket.m_pFirstUncommitted, bucket.m_pFirst);
	bucket.m_AddLock.UnlockRead();

	return h;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline T& CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Element(UtlTSHashHandle_t hHash)
{
	return ((HashFixedData_t*)hHash)->m_Data;
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline T const& CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::Element(UtlTSHashHandle_t hHash) const
{
	return ((HashFixedData_t*)hHash)->m_Data;
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline T& CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::operator[](UtlTSHashHandle_t hHash)
{
	return ((HashFixedData_t*)hHash)->m_Data;
}

template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline T const& CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::operator[](UtlTSHashHandle_t hHash) const
{
	return ((HashFixedData_t*)hHash)->m_Data;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline KEYTYPE CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::GetID(UtlTSHashHandle_t hHash) const
{
	return ((HashFixedData_t*)hHash)->m_uiKey;
}


template<class T, int BUCKET_COUNT, class KEYTYPE, class HashFuncs, int nAlignment>
inline UtlTSHashHandle_t CUtlTSHash<T, BUCKET_COUNT, KEYTYPE, HashFuncs, nAlignment>::ElementPtrToHandle(T* pElement) const
{
	Assert(pElement);
	HashFixedData_t* pFixedData = (HashFixedData_t*)((uint8*)pElement - offsetof(HashFixedData_t, m_Data));
	Assert(m_EntryMemory.IsAllocationWithinPool(pFixedData));
	return (UtlTSHashHandle_t)pFixedData;
}


#endif  