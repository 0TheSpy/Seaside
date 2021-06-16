#ifndef UTLSYMBOLLARGE_H
#define UTLSYMBOLLARGE_H

#ifdef _WIN32
#pragma once
#endif

#include "threadtools.h"
#include "utltshash.h"
#include "stringpool.h"
#include "vprof.h"
#include "utltshash.h"

typedef intp UtlSymLargeId_t;

#define UTL_INVAL_SYMBOL_LARGE  ((UtlSymLargeId_t)~0)

class CUtlSymbolLarge
{
public:
	CUtlSymbolLarge()
	{
		u.m_Id = UTL_INVAL_SYMBOL_LARGE;
	}

	CUtlSymbolLarge(UtlSymLargeId_t id)
	{
		u.m_Id = id;
	}
	CUtlSymbolLarge(CUtlSymbolLarge const& sym)
	{
		u.m_Id = sym.u.m_Id;
	}

	CUtlSymbolLarge& operator=(CUtlSymbolLarge const& src)
	{
		u.m_Id = src.u.m_Id;
		return *this;
	}

	bool operator==(CUtlSymbolLarge const& src) const
	{
		return u.m_Id == src.u.m_Id;
	}

	bool operator==(UtlSymLargeId_t const& src) const
	{
		return u.m_Id == src;
	}

	bool operator!=(CUtlSymbolLarge const& src) const
	{
		return u.m_Id != src.u.m_Id;
	}

	bool operator!=(UtlSymLargeId_t const& src) const
	{
		return u.m_Id != src;
	}

	operator UtlSymLargeId_t const() const
	{
		return u.m_Id;
	}

	inline const char* String() const
	{
		if (u.m_Id == UTL_INVAL_SYMBOL_LARGE)
			return "";
		return u.m_pAsString;
	}

	inline bool IsValid() const
	{
		return u.m_Id != UTL_INVAL_SYMBOL_LARGE ? true : false;
	}

private:
	CUtlSymbolLarge(const char* pStr);                  
	bool operator==(const char* pStr) const;                     

	union
	{
		UtlSymLargeId_t m_Id;
		char const* m_pAsString;
	} u;
};

#define MIN_STRING_POOL_SIZE	2048

inline uint32 CUtlSymbolLarge_Hash(bool CASEINSENSITIVE, const char* pString, int len)
{
	return (CASEINSENSITIVE ? HashStringCaseless(pString) : HashString(pString));
}

typedef uint32 LargeSymbolTableHashDecoration_t;

struct CUtlSymbolTableLargeBaseTreeEntry_t
{
	LargeSymbolTableHashDecoration_t	m_Hash;
	char								m_String[1];

	bool IsEmpty() const
	{
		return ((m_Hash == 0) && (0 == m_String[0]));
	}

	char const* String() const
	{
		return (const char*)&m_String[0];
	}

	CUtlSymbolLarge ToSymbol() const
	{
		return reinterpret_cast<UtlSymLargeId_t>(String());
	}

	LargeSymbolTableHashDecoration_t HashValue() const
	{
		return m_Hash;
	}
};

template< class TreeType, bool CASEINSENSITIVE >
class CTreeEntryLess
{
public:
	CTreeEntryLess(int ignored = 0) {}        
	bool operator!() const { return false; }
	bool operator()(CUtlSymbolTableLargeBaseTreeEntry_t* const& left, CUtlSymbolTableLargeBaseTreeEntry_t* const& right) const
	{
		if (left->m_Hash == right->m_Hash)
		{
			if (!CASEINSENSITIVE)
				return strcmp(left->String(), right->String()) < 0;
			else
				return V_stricmp(left->String(), right->String()) < 0;
		}
		else
		{
			return left->m_Hash < right->m_Hash;
		}
	}
};

template< bool CASEINSENSITIVE >
class CNonThreadsafeTree : public CUtlRBTree<CUtlSymbolTableLargeBaseTreeEntry_t*, intp, CTreeEntryLess< CNonThreadsafeTree< CASEINSENSITIVE >, CASEINSENSITIVE > >
{
public:
	typedef CUtlRBTree<CUtlSymbolTableLargeBaseTreeEntry_t*, intp, CTreeEntryLess< CNonThreadsafeTree, CASEINSENSITIVE > > CNonThreadsafeTreeType;

	CNonThreadsafeTree() :
		CNonThreadsafeTreeType(0, 16)
	{
	}
	inline void Commit()
	{
	}
	inline intp Insert(CUtlSymbolTableLargeBaseTreeEntry_t* entry)
	{
		return CNonThreadsafeTreeType::Insert(entry);
	}
	inline intp Find(CUtlSymbolTableLargeBaseTreeEntry_t* entry) const
	{
		return CNonThreadsafeTreeType::Find(entry);
	}
	inline intp InvalidIndex() const
	{
		return CNonThreadsafeTreeType::InvalidIndex();
	}
	inline int GetElements(int nFirstElement, int nCount, CUtlSymbolLarge* pElements) const
	{
		CUtlVector< CUtlSymbolTableLargeBaseTreeEntry_t* > list;
		list.EnsureCount(nCount);
		for (int i = 0; i < nCount; ++i)
		{
			pElements[i] = CNonThreadsafeTreeType::Element(i)->ToSymbol();
		}

		return nCount;
	}
};

template < int BUCKET_COUNT, class KEYTYPE, bool CASEINSENSITIVE >
class CCThreadsafeTreeHashMethod
{
public:
	static int Hash(const KEYTYPE& key, int nBucketMask)
	{
		uint32 nHash = key->HashValue();
		return (nHash & nBucketMask);
	}

	static bool Compare(CUtlSymbolTableLargeBaseTreeEntry_t* const& lhs, CUtlSymbolTableLargeBaseTreeEntry_t* const& rhs)
	{
		if (lhs->m_Hash != rhs->m_Hash)
			return false;
		if (!CASEINSENSITIVE)
		{
			return (!Q_strcmp(lhs->String(), rhs->String()) ? true : false);
		}

		return (!Q_stricmp(lhs->String(), rhs->String()) ? true : false);
	}
};

template < bool CASEINSENSITIVE >
class CThreadsafeTree : public CUtlTSHash< CUtlSymbolTableLargeBaseTreeEntry_t*, 2048, CUtlSymbolTableLargeBaseTreeEntry_t*, CCThreadsafeTreeHashMethod< 2048, CUtlSymbolTableLargeBaseTreeEntry_t*, CASEINSENSITIVE > >
{
public:
	typedef CUtlTSHash< CUtlSymbolTableLargeBaseTreeEntry_t*, 2048, CUtlSymbolTableLargeBaseTreeEntry_t*, CCThreadsafeTreeHashMethod< 2048, CUtlSymbolTableLargeBaseTreeEntry_t*, CASEINSENSITIVE > > CThreadsafeTreeType;

	CThreadsafeTree() :
		CThreadsafeTreeType(32)
	{
	}
	inline void Commit()
	{
		CThreadsafeTreeType::Commit();
	}
	inline UtlTSHashHandle_t Insert(CUtlSymbolTableLargeBaseTreeEntry_t* entry)
	{
		return CThreadsafeTreeType::Insert(entry, entry);
	}
	inline UtlTSHashHandle_t Find(CUtlSymbolTableLargeBaseTreeEntry_t* entry)
	{
		return CThreadsafeTreeType::Find(entry);
	}
	inline UtlTSHashHandle_t InvalidIndex() const
	{
		return CThreadsafeTreeType::InvalidHandle();
	}
	inline int GetElements(UtlTSHashHandle_t nFirstElement, int nCount, CUtlSymbolLarge* pElements) const
	{
		CUtlVector< UtlTSHashHandle_t > list;
		list.EnsureCount(nCount);
		int c = CThreadsafeTreeType::GetElements(nFirstElement, nCount, list.Base());
		for (int i = 0; i < c; ++i)
		{
			pElements[i] = CThreadsafeTreeType::Element(list[i])->ToSymbol();
		}

		return c;
	}
};

template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE = MIN_STRING_POOL_SIZE >
class CUtlSymbolTableLargeBase
{
public:
	CUtlSymbolTableLargeBase();
	~CUtlSymbolTableLargeBase();

	CUtlSymbolLarge AddString(const char* pString);

	CUtlSymbolLarge Find(const char* pString) const;

	void  RemoveAll();

	int GetNumStrings(void) const
	{
		return m_Lookup.Count();
	}

	void Commit()
	{
		m_Lookup.Commit();
	}

	int GetElements(int nFirstElement, int nCount, CUtlSymbolLarge* pElements) const
	{
		return m_Lookup.GetElements(nFirstElement, nCount, pElements);
	}

	const char* GetElementString(int nElement) const
	{
		return m_Lookup.Element(nElement)->String();
	}

	uint64 GetMemoryUsage() const
	{
		uint64 unBytesUsed = 0u;

		for (int i = 0; i < m_StringPools.Count(); i++)
		{
			StringPool_t* pPool = m_StringPools[i];

			unBytesUsed += (uint64)pPool->m_TotalLen;
		}
		return unBytesUsed;
	}


protected:

	struct StringPool_t
	{
		int m_TotalLen;		    
		int m_SpaceUsed;
		char m_Data[1];
	};

	TreeType m_Lookup;

	CUtlVector< StringPool_t* > m_StringPools;

private:
	int FindPoolWithSpace(int len) const;
};

template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE >
inline CUtlSymbolTableLargeBase<TreeType, CASEINSENSITIVE, POOL_SIZE >::CUtlSymbolTableLargeBase() :
	m_StringPools(8)
{
}

template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE >
inline CUtlSymbolTableLargeBase<TreeType, CASEINSENSITIVE, POOL_SIZE>::~CUtlSymbolTableLargeBase()
{
	RemoveAll();
}

template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE >
inline CUtlSymbolLarge CUtlSymbolTableLargeBase<TreeType, CASEINSENSITIVE, POOL_SIZE>::Find(const char* pString) const
{
	VPROF("CUtlSymbolLarge::Find");
	if (!pString)
		return CUtlSymbolLarge();

	int len = Q_strlen(pString) + 1;

	CUtlSymbolTableLargeBaseTreeEntry_t* search = (CUtlSymbolTableLargeBaseTreeEntry_t*)_alloca(len + sizeof(LargeSymbolTableHashDecoration_t));
	search->m_Hash = CUtlSymbolLarge_Hash(CASEINSENSITIVE, pString, len);
	Q_memcpy((char*)&search->m_String[0], pString, len);

	intp idx = const_cast<TreeType&>(m_Lookup).Find(search);

	if (idx == m_Lookup.InvalidIndex())
		return UTL_INVAL_SYMBOL_LARGE;

	const CUtlSymbolTableLargeBaseTreeEntry_t* entry = m_Lookup[idx];
	return entry->ToSymbol();
}

template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE >
inline int CUtlSymbolTableLargeBase<TreeType, CASEINSENSITIVE, POOL_SIZE>::FindPoolWithSpace(int len)	const
{
	for (int i = 0; i < m_StringPools.Count(); i++)
	{
		StringPool_t* pPool = m_StringPools[i];

		if ((pPool->m_TotalLen - pPool->m_SpaceUsed) >= len)
		{
			return i;
		}
	}

	return -1;
}

template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE >
inline CUtlSymbolLarge CUtlSymbolTableLargeBase<TreeType, CASEINSENSITIVE, POOL_SIZE>::AddString(const char* pString)
{
	VPROF("CUtlSymbolLarge::AddString");
	if (!pString)
		return UTL_INVAL_SYMBOL_LARGE;

	CUtlSymbolLarge id = Find(pString);
	if (id != UTL_INVAL_SYMBOL_LARGE)
		return id;

	int lenString = Q_strlen(pString) + 1;      
	int lenDecorated = lenString + sizeof(LargeSymbolTableHashDecoration_t);      
	lenDecorated = ALIGN_VALUE(lenDecorated, sizeof(LargeSymbolTableHashDecoration_t));

	int iPool = FindPoolWithSpace(lenDecorated);
	if (iPool == -1)
	{
		int newPoolSize = MAX(lenDecorated + sizeof(StringPool_t), POOL_SIZE);
		StringPool_t* pPool = (StringPool_t*)malloc(newPoolSize);

		pPool->m_TotalLen = newPoolSize - sizeof(StringPool_t);
		pPool->m_SpaceUsed = 0;
		iPool = m_StringPools.AddToTail(pPool);
	}

	LargeSymbolTableHashDecoration_t hash = CUtlSymbolLarge_Hash(CASEINSENSITIVE, pString, lenString);

	StringPool_t* pPool = m_StringPools[iPool];
	CUtlSymbolTableLargeBaseTreeEntry_t* entry = (CUtlSymbolTableLargeBaseTreeEntry_t*)&pPool->m_Data[pPool->m_SpaceUsed];

	pPool->m_SpaceUsed += lenDecorated;

	entry->m_Hash = hash;
	char* pText = (char*)&entry->m_String[0];
	Q_memcpy(pText, pString, lenString);

	MEM_ALLOC_CREDIT();
	return m_Lookup.Element(m_Lookup.Insert(entry))->ToSymbol();
}

#ifdef ANALYZE_SUPPRESS     
ANALYZE_SUPPRESS(6001);        
#endif
template < class TreeType, bool CASEINSENSITIVE, size_t POOL_SIZE >
inline void CUtlSymbolTableLargeBase<TreeType, CASEINSENSITIVE, POOL_SIZE>::RemoveAll()
{
	m_Lookup.Purge();

	for (int i = 0; i < m_StringPools.Count(); i++)
		free(m_StringPools[i]);

	m_StringPools.RemoveAll();
}
#ifdef ANALYZE_UNSUPPRESS     
ANALYZE_UNSUPPRESS();        
#endif

typedef CUtlSymbolTableLargeBase< CNonThreadsafeTree< false >, false > CUtlSymbolTableLarge;
typedef CUtlSymbolTableLargeBase< CNonThreadsafeTree< true >, true > CUtlSymbolTableLarge_CI;
typedef CUtlSymbolTableLargeBase< CThreadsafeTree< false >, false > CUtlSymbolTableLargeMT;
typedef CUtlSymbolTableLargeBase< CThreadsafeTree< true >, true > CUtlSymbolTableLargeMT_CI;

#endif  