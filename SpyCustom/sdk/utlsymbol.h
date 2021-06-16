#ifndef UTLSYMBOL_H
#define UTLSYMBOL_H

#ifdef _WIN32
#pragma once
#endif

#include "threadtools.h"
#include "utlrbtree.h"
#include "utlvector.h"


class CUtlSymbolTable;
class CUtlSymbolTableMT;


typedef unsigned short UtlSymId_t;

#define UTL_INVAL_SYMBOL  ((UtlSymId_t)~0)

class CUtlSymbol
{
public:
	CUtlSymbol() : m_Id(UTL_INVAL_SYMBOL) {}
	CUtlSymbol(UtlSymId_t id) : m_Id(id) {}
	CUtlSymbol(const char* pStr);
	CUtlSymbol(CUtlSymbol const& sym) : m_Id(sym.m_Id) {}

	CUtlSymbol& operator=(CUtlSymbol const& src) { m_Id = src.m_Id; return *this; }

	bool operator==(CUtlSymbol const& src) const { return m_Id == src.m_Id; }
	bool operator==(const char* pStr) const;

	bool IsValid() const { return m_Id != UTL_INVAL_SYMBOL; }

	operator UtlSymId_t const() const { return m_Id; }

	const char* String() const;

	static void DisableStaticSymbolTable();

protected:
	UtlSymId_t   m_Id;

	static void Initialize();

	static CUtlSymbolTableMT* CurrTable();

	static CUtlSymbolTableMT* s_pSymbolTable;

	static bool s_bAllowStaticSymbolTable;

	friend class CCleanupUtlSymbolTable;
};


class CUtlSymbolTable
{
public:
	CUtlSymbolTable(int growSize = 0, int initSize = 32, bool caseInsensitive = false);
	~CUtlSymbolTable();

	CUtlSymbol AddString(const char* pString);

	CUtlSymbol Find(const char* pString) const;

	const char* String(CUtlSymbol id) const;

	void  RemoveAll();

	int GetNumStrings(void) const
	{
		return m_Lookup.Count();
	}

protected:
	class CStringPoolIndex
	{
	public:
		inline CStringPoolIndex()
		{
		}

		inline CStringPoolIndex(unsigned short iPool, unsigned short iOffset)
		{
			m_iPool = iPool;
			m_iOffset = iOffset;
		}

		inline bool operator==(const CStringPoolIndex& other)	const
		{
			return m_iPool == other.m_iPool && m_iOffset == other.m_iOffset;
		}

		unsigned short m_iPool;		   
		unsigned short m_iOffset;	     
	};

	class CLess
	{
	public:
		CLess(int ignored = 0) {}        
		bool operator!() const { return false; }
		bool operator()(const CStringPoolIndex& left, const CStringPoolIndex& right) const;
	};

	class CTree : public CUtlRBTree<CStringPoolIndex, unsigned short, CLess>
	{
	public:
		CTree(int growSize, int initSize) : CUtlRBTree<CStringPoolIndex, unsigned short, CLess>(growSize, initSize) {}
		friend class CUtlSymbolTable::CLess;           
	};

	struct StringPool_t
	{
		int m_TotalLen;		    
		int m_SpaceUsed;
		char m_Data[1];
	};

	CTree m_Lookup;
	bool m_bInsensitive;
	mutable const char* m_pUserSearchString;

	CUtlVector<StringPool_t*> m_StringPools;

private:
	int FindPoolWithSpace(int len) const;
	const char* StringFromIndex(const CStringPoolIndex& index) const;

	friend class CLess;
};

class CUtlSymbolTableMT : private CUtlSymbolTable
{
public:
	CUtlSymbolTableMT(int growSize = 0, int initSize = 32, bool caseInsensitive = false)
		: CUtlSymbolTable(growSize, initSize, caseInsensitive)
	{
	}

	CUtlSymbol AddString(const char* pString)
	{
		m_lock.LockForWrite();
		CUtlSymbol result = CUtlSymbolTable::AddString(pString);
		m_lock.UnlockWrite();
		return result;
	}

	CUtlSymbol Find(const char* pString) const
	{
		m_lock.LockForRead();
		CUtlSymbol result = CUtlSymbolTable::Find(pString);
		m_lock.UnlockRead();
		return result;
	}

	const char* String(CUtlSymbol id) const
	{
		m_lock.LockForRead();
		const char* pszResult = CUtlSymbolTable::String(id);
		m_lock.UnlockRead();
		return pszResult;
	}

private:
#if defined(WIN32) || defined(_WIN32)
	mutable CThreadSpinRWLock m_lock;
#else
	mutable CThreadRWLock m_lock;
#endif
};



typedef void* FileNameHandle_t;
#define FILENAMEHANDLE_INVALID 0

class CUtlFilenameSymbolTable
{
	struct FileNameHandleInternal_t
	{
		FileNameHandleInternal_t()
		{
			path = 0;
			file = 0;
		}

		unsigned short path;
		unsigned short file;
	};

	class HashTable;

public:
	CUtlFilenameSymbolTable();
	~CUtlFilenameSymbolTable();
	FileNameHandle_t	FindOrAddFileName(const char* pFileName);
	FileNameHandle_t	FindFileName(const char* pFileName);
	int					PathIndex(const FileNameHandle_t& handle) { return ((const FileNameHandleInternal_t*)&handle)->path; }
	bool				String(const FileNameHandle_t& handle, char* buf, int buflen);
	void				RemoveAll();

private:
	HashTable* m_Strings;
	mutable CThreadSpinRWLock m_lock;
};


#endif  