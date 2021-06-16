#ifndef UTLMEMORY_H
#define UTLMEMORY_H

#ifdef _WIN32
#pragma once
#endif

#include "dbg.h"
#include <string.h>
#include "platform.h"

#include "memalloc.h"
#include "mathlib.h"
#include "memdbgon.h"

#pragma warning (disable:4100)
#pragma warning (disable:4514)



#ifdef UTLMEMORY_TRACK
#define UTLMEMORY_TRACK_ALLOC()		MemAlloc_RegisterAllocation( "||Sum of all UtlMemory||", 0, m_nAllocationCount * sizeof(T), m_nAllocationCount * sizeof(T), 0 )
#define UTLMEMORY_TRACK_FREE()		if ( !m_pMemory ) ; else MemAlloc_RegisterDeallocation( "||Sum of all UtlMemory||", 0, m_nAllocationCount * sizeof(T), m_nAllocationCount * sizeof(T), 0 )
#else
#define UTLMEMORY_TRACK_ALLOC()		((void)0)
#define UTLMEMORY_TRACK_FREE()		((void)0)
#endif


template< class T, class I = int >
class CUtlMemory
{
	template< class A, class B> friend class CUtlVector;
	template< class A, size_t B> friend class CUtlVectorFixedGrowableCompat;
public:
	CUtlMemory(int nGrowSize = 0, int nInitSize = 0);
	CUtlMemory(T* pMemory, int numElements);
	CUtlMemory(const T* pMemory, int numElements);
	~CUtlMemory();

	CUtlMemory(const CUtlMemory&) = delete;
	CUtlMemory& operator=(const CUtlMemory&) = delete;

	CUtlMemory(CUtlMemory&& moveFrom);
	CUtlMemory& operator=(CUtlMemory&& moveFrom);

	void Init(int nGrowSize = 0, int nInitSize = 0);

	class Iterator_t
	{
	public:
		Iterator_t(I i) : index(i) {}
		I index;

		bool operator==(const Iterator_t it) const { return index == it.index; }
		bool operator!=(const Iterator_t it) const { return index != it.index; }
	};
	Iterator_t First() const { return Iterator_t(IsIdxValid(0) ? 0 : InvalidIndex()); }
	Iterator_t Next(const Iterator_t& it) const { return Iterator_t(IsIdxValid(it.index + 1) ? it.index + 1 : InvalidIndex()); }
	I GetIndex(const Iterator_t& it) const { return it.index; }
	bool IsIdxAfter(I i, const Iterator_t& it) const { return i > it.index; }
	bool IsValidIterator(const Iterator_t& it) const { return IsIdxValid(it.index); }
	Iterator_t InvalidIterator() const { return Iterator_t(InvalidIndex()); }

	T& operator[](I i);
	const T& operator[](I i) const;
	T& Element(I i);
	const T& Element(I i) const;

	bool IsIdxValid(I i) const;

	static const I INVALID_INDEX = (I)-1;     
	static I InvalidIndex() { return INVALID_INDEX; }

	T* Base();
	const T* Base() const;

	void SetExternalBuffer(T* pMemory, int numElements);
	void SetExternalBuffer(const T* pMemory, int numElements);
	void AssumeMemory(T* pMemory, int nSize);
	T* Detach();
	void* DetachMemory();

	void Swap(CUtlMemory< T, I >& mem);

	void ConvertToGrowableMemory(int nGrowSize);

	int NumAllocated() const;
	int Count() const;

	void Grow(int num = 1);

	void EnsureCapacity(int num);

	void Purge();

	void Purge(int numElements);

	bool IsExternallyAllocated() const;

	bool IsReadOnly() const;

	void SetGrowSize(int size);

protected:
	void ValidateGrowSize()
	{
#ifdef _X360
		if (m_nGrowSize && m_nGrowSize != EXTERNAL_BUFFER_MARKER)
		{
			const int MAX_GROW = 128;
			if (m_nGrowSize * sizeof(T) > MAX_GROW)
			{
				m_nGrowSize = max(1, MAX_GROW / sizeof(T));
			}
		}
#endif
	}

	enum
	{
		EXTERNAL_BUFFER_MARKER = -1,
		EXTERNAL_CONST_BUFFER_MARKER = -2,
	};

	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};


template< class T, size_t SIZE, class I = int >
class CUtlMemoryFixedGrowable : public CUtlMemory< T, I >
{
	typedef CUtlMemory< T, I > BaseClass;

public:
	CUtlMemoryFixedGrowable(int nGrowSize = 0, int nInitSize = SIZE) : BaseClass(m_pFixedMemory, SIZE)
	{
		Assert(nInitSize == 0 || nInitSize == SIZE);
		m_nMallocGrowSize = nGrowSize;
	}

	void Grow(int nCount = 1)
	{
		if (this->IsExternallyAllocated())
		{
			this->ConvertToGrowableMemory(m_nMallocGrowSize);
		}
		BaseClass::Grow(nCount);
	}

	void EnsureCapacity(int num)
	{
		if (CUtlMemory<T>::m_nAllocationCount >= num)
			return;

		if (this->IsExternallyAllocated())
		{
			this->ConvertToGrowableMemory(m_nMallocGrowSize);
		}

		BaseClass::EnsureCapacity(num);
	}

private:
	int m_nMallocGrowSize;
	T m_pFixedMemory[SIZE];
};

template< typename T, size_t SIZE, int nAlignment = 0 >
class CUtlMemoryFixed
{
public:
	CUtlMemoryFixed(int nGrowSize = 0, int nInitSize = 0) { Assert(nInitSize == 0 || nInitSize == SIZE); }
	CUtlMemoryFixed(T* pMemory, int numElements) { Assert(0); }

	bool IsIdxValid(int i) const { return (i >= 0) && (i < SIZE); }

	static const int INVALID_INDEX = -1;     
	static int InvalidIndex() { return INVALID_INDEX; }

	T* Base() { if (nAlignment == 0) return (T*)(&m_Memory[0]); else return (T*)AlignValue(&m_Memory[0], nAlignment); }
	const T* Base() const { if (nAlignment == 0) return (T*)(&m_Memory[0]); else return (T*)AlignValue(&m_Memory[0], nAlignment); }

	T& operator[](int i) { Assert(IsIdxValid(i)); return Base()[i]; }
	const T& operator[](int i) const { Assert(IsIdxValid(i)); return Base()[i]; }
	T& Element(int i) { Assert(IsIdxValid(i)); return Base()[i]; }
	const T& Element(int i) const { Assert(IsIdxValid(i)); return Base()[i]; }

	void SetExternalBuffer(T* pMemory, int numElements) { Assert(0); }

	int NumAllocated() const { return SIZE; }
	int Count() const { return SIZE; }

	void Grow(int num = 1) { Assert(0); }

	void EnsureCapacity(int num) { Assert(num <= SIZE); }

	void Purge() {}

	void Purge(int numElements) { Assert(0); }

	bool IsExternallyAllocated() const { return false; }

	void SetGrowSize(int size) {}

	class Iterator_t
	{
	public:
		Iterator_t(int i) : index(i) {}
		int index;
		bool operator==(const Iterator_t it) const { return index == it.index; }
		bool operator!=(const Iterator_t it) const { return index != it.index; }
	};
	Iterator_t First() const { return Iterator_t(IsIdxValid(0) ? 0 : InvalidIndex()); }
	Iterator_t Next(const Iterator_t& it) const { return Iterator_t(IsIdxValid(it.index + 1) ? it.index + 1 : InvalidIndex()); }
	int GetIndex(const Iterator_t& it) const { return it.index; }
	bool IsIdxAfter(int i, const Iterator_t& it) const { return i > it.index; }
	bool IsValidIterator(const Iterator_t& it) const { return IsIdxValid(it.index); }
	Iterator_t InvalidIterator() const { return Iterator_t(InvalidIndex()); }

private:
	char m_Memory[SIZE * sizeof(T) + nAlignment];
};

#ifdef _LINUX
#define REMEMBER_ALLOC_SIZE_FOR_VALGRIND 1
#endif

template< typename T >
class CUtlMemoryConservative
{

public:
	CUtlMemoryConservative(int nGrowSize = 0, int nInitSize = 0) : m_pMemory(NULL)
	{
#ifdef REMEMBER_ALLOC_SIZE_FOR_VALGRIND
		m_nCurAllocSize = 0;
#endif

	}
	CUtlMemoryConservative(T* pMemory, int numElements) { Assert(0); }
	~CUtlMemoryConservative() { if (m_pMemory) free(m_pMemory); }

	bool IsIdxValid(int i) const { return (IsDebug()) ? (i >= 0 && i < NumAllocated()) : (i >= 0); }
	static int InvalidIndex() { return -1; }

	T* Base() { return m_pMemory; }
	const T* Base() const { return m_pMemory; }

	T& operator[](int i) { Assert(IsIdxValid(i)); return Base()[i]; }
	const T& operator[](int i) const { Assert(IsIdxValid(i)); return Base()[i]; }
	T& Element(int i) { Assert(IsIdxValid(i)); return Base()[i]; }
	const T& Element(int i) const { Assert(IsIdxValid(i)); return Base()[i]; }

	void SetExternalBuffer(T* pMemory, int numElements) { Assert(0); }

	FORCEINLINE void RememberAllocSize(size_t sz)
	{
#ifdef REMEMBER_ALLOC_SIZE_FOR_VALGRIND
		m_nCurAllocSize = sz;
#endif
	}

	size_t AllocSize(void) const
	{
#ifdef REMEMBER_ALLOC_SIZE_FOR_VALGRIND
		return m_nCurAllocSize;
#else
		return (m_pMemory) ? g_pMemAlloc->GetSize(m_pMemory) : 0;
#endif
	}

	int NumAllocated() const
	{
		return AllocSize() / sizeof(T);
	}
	int Count() const
	{
		return NumAllocated();
	}

	FORCEINLINE void ReAlloc(size_t sz)
	{
		m_pMemory = (T*)realloc(m_pMemory, sz);
		RememberAllocSize(sz);
	}
	void Grow(int num = 1)
	{
		int nCurN = NumAllocated();
		ReAlloc((nCurN + num) * sizeof(T));
	}

	void EnsureCapacity(int num)
	{
		size_t nSize = sizeof(T) * MAX(num, Count());
		ReAlloc(nSize);
	}

	void Purge()
	{
		free(m_pMemory);
		RememberAllocSize(0);
		m_pMemory = NULL;
	}

	void Purge(int numElements) { ReAlloc(numElements * sizeof(T)); }

	bool IsExternallyAllocated() const { return false; }

	void SetGrowSize(int size) {}

	class Iterator_t
	{
	public:
		Iterator_t(int i, int _limit) : index(i), limit(_limit) {}
		int index;
		int limit;
		bool operator==(const Iterator_t it) const { return index == it.index; }
		bool operator!=(const Iterator_t it) const { return index != it.index; }
	};
	Iterator_t First() const { int limit = NumAllocated(); return Iterator_t(limit ? 0 : InvalidIndex(), limit); }
	Iterator_t Next(const Iterator_t& it) const { return Iterator_t((it.index + 1 < it.limit) ? it.index + 1 : InvalidIndex(), it.limit); }
	int GetIndex(const Iterator_t& it) const { return it.index; }
	bool IsIdxAfter(int i, const Iterator_t& it) const { return i > it.index; }
	bool IsValidIterator(const Iterator_t& it) const { return IsIdxValid(it.index) && (it.index < it.limit); }
	Iterator_t InvalidIterator() const { return Iterator_t(InvalidIndex(), 0); }

private:
	T* m_pMemory;
#ifdef REMEMBER_ALLOC_SIZE_FOR_VALGRIND
	size_t m_nCurAllocSize;
#endif

};


template< class T, class I >
CUtlMemory<T, I>::CUtlMemory(int nGrowSize, int nInitAllocationCount) : m_pMemory(0),
m_nAllocationCount(nInitAllocationCount), m_nGrowSize(nGrowSize)
{
	ValidateGrowSize();
	Assert(nGrowSize >= 0);
	if (m_nAllocationCount)
	{
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}
}

template< class T, class I >
CUtlMemory<T, I>::CUtlMemory(T* pMemory, int numElements) : m_pMemory(pMemory),
m_nAllocationCount(numElements)
{
	m_nGrowSize = EXTERNAL_BUFFER_MARKER;
}

template< class T, class I >
CUtlMemory<T, I>::CUtlMemory(const T* pMemory, int numElements) : m_pMemory((T*)pMemory),
m_nAllocationCount(numElements)
{
	m_nGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template< class T, class I >
CUtlMemory<T, I>::~CUtlMemory()
{
	Purge();

#ifdef _DEBUG
	m_pMemory = reinterpret_cast<T*>(0xFEFEBAAD);
	m_nAllocationCount = 0x7BADF00D;
#endif
}

template< class T, class I >
CUtlMemory<T, I>::CUtlMemory(CUtlMemory&& moveFrom)
	: m_pMemory(moveFrom.m_pMemory)
	, m_nAllocationCount(moveFrom.m_nAllocationCount)
	, m_nGrowSize(moveFrom.m_nGrowSize)
{
	moveFrom.m_pMemory = nullptr;
	moveFrom.m_nAllocationCount = 0;
	moveFrom.m_nGrowSize = 0;
}

template< class T, class I >
CUtlMemory<T, I>& CUtlMemory<T, I>::operator=(CUtlMemory&& moveFrom)
{
	T* pMemory = moveFrom.m_pMemory;
	int nAllocationCount = moveFrom.m_nAllocationCount;
	int nGrowSize = moveFrom.m_nGrowSize;

	moveFrom.m_pMemory = nullptr;
	moveFrom.m_nAllocationCount = 0;
	moveFrom.m_nGrowSize = 0;

	Purge();

	m_pMemory = pMemory;
	m_nAllocationCount = nAllocationCount;
	m_nGrowSize = nGrowSize;

	return *this;
}

template< class T, class I >
void CUtlMemory<T, I>::Init(int nGrowSize  , int nInitSize  )
{
	Purge();

	m_nGrowSize = nGrowSize;
	m_nAllocationCount = nInitSize;
	ValidateGrowSize();
	Assert(nGrowSize >= 0);
	if (m_nAllocationCount)
	{
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}
}

template< class T, class I >
void CUtlMemory<T, I>::Swap(CUtlMemory<T, I>& mem)
{
	V_swap(m_nGrowSize, mem.m_nGrowSize);
	V_swap(m_pMemory, mem.m_pMemory);
	V_swap(m_nAllocationCount, mem.m_nAllocationCount);
}


template< class T, class I >
void CUtlMemory<T, I>::ConvertToGrowableMemory(int nGrowSize)
{
	if (!IsExternallyAllocated())
		return;

	m_nGrowSize = nGrowSize;
	if (m_nAllocationCount)
	{
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();

		int nNumBytes = m_nAllocationCount * sizeof(T);
		T* pMemory = (T*)malloc(nNumBytes);
		memcpy(pMemory, m_pMemory, nNumBytes);
		m_pMemory = pMemory;
	}
	else
	{
		m_pMemory = NULL;
	}
}


template< class T, class I >
void CUtlMemory<T, I>::SetExternalBuffer(T* pMemory, int numElements)
{
	Purge();

	m_pMemory = pMemory;
	m_nAllocationCount = numElements;

	m_nGrowSize = EXTERNAL_BUFFER_MARKER;
}

template< class T, class I >
void CUtlMemory<T, I>::SetExternalBuffer(const T* pMemory, int numElements)
{
	Purge();

	m_pMemory = const_cast<T*>(pMemory);
	m_nAllocationCount = numElements;

	m_nGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template< class T, class I >
void CUtlMemory<T, I>::AssumeMemory(T* pMemory, int numElements)
{
	Purge();

	m_pMemory = pMemory;
	m_nAllocationCount = numElements;
}

template< class T, class I >
void* CUtlMemory<T, I>::DetachMemory()
{
	if (IsExternallyAllocated())
		return NULL;

	void* pMemory = m_pMemory;
	m_pMemory = 0;
	m_nAllocationCount = 0;
	return pMemory;
}

template< class T, class I >
inline T* CUtlMemory<T, I>::Detach()
{
	return (T*)DetachMemory();
}


template< class T, class I >
inline T& CUtlMemory<T, I>::operator[](I i)
{
	Assert(!IsReadOnly());
	Assert(IsIdxValid(i));
	return m_pMemory[i];
}

template< class T, class I >
inline const T& CUtlMemory<T, I>::operator[](I i) const
{
	Assert(IsIdxValid(i));
	return m_pMemory[i];
}

template< class T, class I >
inline T& CUtlMemory<T, I>::Element(I i)
{
	Assert(!IsReadOnly());
	Assert(IsIdxValid(i));
	return m_pMemory[i];
}

template< class T, class I >
inline const T& CUtlMemory<T, I>::Element(I i) const
{
	Assert(IsIdxValid(i));
	return m_pMemory[i];
}


template< class T, class I >
bool CUtlMemory<T, I>::IsExternallyAllocated() const
{
	return (m_nGrowSize < 0);
}


template< class T, class I >
bool CUtlMemory<T, I>::IsReadOnly() const
{
	return (m_nGrowSize == EXTERNAL_CONST_BUFFER_MARKER);
}


template< class T, class I >
void CUtlMemory<T, I>::SetGrowSize(int nSize)
{
	Assert(!IsExternallyAllocated());
	Assert(nSize >= 0);
	m_nGrowSize = nSize;
	ValidateGrowSize();
}


template< class T, class I >
inline T* CUtlMemory<T, I>::Base()
{
	Assert(!IsReadOnly());
	return m_pMemory;
}

template< class T, class I >
inline const T* CUtlMemory<T, I>::Base() const
{
	return m_pMemory;
}


template< class T, class I >
inline int CUtlMemory<T, I>::NumAllocated() const
{
	return m_nAllocationCount;
}

template< class T, class I >
inline int CUtlMemory<T, I>::Count() const
{
	return m_nAllocationCount;
}


template< class T, class I >
inline bool CUtlMemory<T, I>::IsIdxValid(I i) const
{
	long x = i;
	return (x >= 0) && (x < m_nAllocationCount);
}

inline int UtlMemory_CalcNewAllocationCount(int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem)
{
	if (nGrowSize)
	{
		nAllocationCount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
	}
	else
	{
		if (!nAllocationCount)
		{
			nAllocationCount = (31 + nBytesItem) / nBytesItem;
			if (nAllocationCount < nNewSize)
				nAllocationCount = nNewSize;
		}

		while (nAllocationCount < nNewSize)
		{
#ifndef _X360
			nAllocationCount *= 2;
#else
			int nNewAllocationCount = (nAllocationCount * 9) / 8;   
			if (nNewAllocationCount > nAllocationCount)
				nAllocationCount = nNewAllocationCount;
			else
				nAllocationCount *= 2;
#endif
		}
	}

	return nAllocationCount;
}

template< class T, class I >
void CUtlMemory<T, I>::Grow(int num)
{
	Assert(num > 0);

	if (IsExternallyAllocated())
	{
		Assert(0);
		return;
	}

	int nAllocationRequested = m_nAllocationCount + num;

	UTLMEMORY_TRACK_FREE();

	int nNewAllocationCount = UtlMemory_CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

	if ((int)(I)nNewAllocationCount < nAllocationRequested)
	{
		if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested)
		{
			--nNewAllocationCount;            
		}
		else
		{
			if ((int)(I)nAllocationRequested != nAllocationRequested)
			{
				Assert(0);
				return;
			}
			while ((int)(I)nNewAllocationCount < nAllocationRequested)
			{
				nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
			}
		}
	}

	m_nAllocationCount = nNewAllocationCount;

	UTLMEMORY_TRACK_ALLOC();

	if (m_pMemory)
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
		Assert(m_pMemory);
	}
	else
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
		Assert(m_pMemory);
	}
}


template< class T, class I >
inline void CUtlMemory<T, I>::EnsureCapacity(int num)
{
	if (m_nAllocationCount >= num)
		return;

	if (IsExternallyAllocated())
	{
		Assert(0);
		return;
	}

	UTLMEMORY_TRACK_FREE();

	m_nAllocationCount = num;

	UTLMEMORY_TRACK_ALLOC();

	if (m_pMemory)
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
	}
	else
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}
}


template< class T, class I >
void CUtlMemory<T, I>::Purge()
{
	if (!IsExternallyAllocated())
	{
		if (m_pMemory)
		{
			UTLMEMORY_TRACK_FREE();
			free((void*)m_pMemory);
			m_pMemory = 0;
		}
		m_nAllocationCount = 0;
	}
}

template< class T, class I >
void CUtlMemory<T, I>::Purge(int numElements)
{
	Assert(numElements >= 0);

	if (numElements > m_nAllocationCount)
	{
		Assert(numElements <= m_nAllocationCount);
		return;
	}

	if (numElements == 0)
	{
		Purge();
		return;
	}

	if (IsExternallyAllocated())
	{
		return;
	}

	if (numElements == m_nAllocationCount)
	{
		return;
	}


	if (!m_pMemory)
	{
		Assert(m_pMemory);
		return;
	}

	UTLMEMORY_TRACK_FREE();

	m_nAllocationCount = numElements;

	UTLMEMORY_TRACK_ALLOC();

	MEM_ALLOC_CREDIT_CLASS();
	m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
}

template< class T, int nAlignment >
class CUtlMemoryAligned : public CUtlMemory<T>
{
public:
	CUtlMemoryAligned(int nGrowSize = 0, int nInitSize = 0);
	CUtlMemoryAligned(T* pMemory, int numElements);
	CUtlMemoryAligned(const T* pMemory, int numElements);
	~CUtlMemoryAligned();

	void SetExternalBuffer(T* pMemory, int numElements);
	void SetExternalBuffer(const T* pMemory, int numElements);

	void Grow(int num = 1);

	void EnsureCapacity(int num);

	void Purge();

	void Purge(int numElements) { Assert(0); }

private:
	void* Align(const void* pAddr);
};


template< class T, int nAlignment >
void* CUtlMemoryAligned<T, nAlignment>::Align(const void* pAddr)
{
	size_t nAlignmentMask = nAlignment - 1;
	return (void*)(((size_t)pAddr + nAlignmentMask) & (~nAlignmentMask));
}


template< class T, int nAlignment >
CUtlMemoryAligned<T, nAlignment>::CUtlMemoryAligned(int nGrowSize, int nInitAllocationCount)
{
	CUtlMemory<T>::m_pMemory = 0;
	CUtlMemory<T>::m_nAllocationCount = nInitAllocationCount;
	CUtlMemory<T>::m_nGrowSize = nGrowSize;
	this->ValidateGrowSize();

	COMPILE_TIME_ASSERT((nAlignment & (nAlignment - 1)) == 0);
	Assert((nGrowSize >= 0) && (nGrowSize != CUtlMemory<T>::EXTERNAL_BUFFER_MARKER));
	if (CUtlMemory<T>::m_nAllocationCount)
	{
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();
		CUtlMemory<T>::m_pMemory = (T*)_aligned_malloc(nInitAllocationCount * sizeof(T), nAlignment);
	}
}

template< class T, int nAlignment >
CUtlMemoryAligned<T, nAlignment>::CUtlMemoryAligned(T* pMemory, int numElements)
{
	CUtlMemory<T>::m_nGrowSize = CUtlMemory<T>::EXTERNAL_BUFFER_MARKER;

	CUtlMemory<T>::m_pMemory = (T*)Align(pMemory);
	CUtlMemory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)CUtlMemory<T>::m_pMemory) / sizeof(T);
}

template< class T, int nAlignment >
CUtlMemoryAligned<T, nAlignment>::CUtlMemoryAligned(const T* pMemory, int numElements)
{
	CUtlMemory<T>::m_nGrowSize = CUtlMemory<T>::EXTERNAL_CONST_BUFFER_MARKER;

	CUtlMemory<T>::m_pMemory = (T*)Align(pMemory);
	CUtlMemory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)CUtlMemory<T>::m_pMemory) / sizeof(T);
}

template< class T, int nAlignment >
CUtlMemoryAligned<T, nAlignment>::~CUtlMemoryAligned()
{
	Purge();
}


template< class T, int nAlignment >
void CUtlMemoryAligned<T, nAlignment>::SetExternalBuffer(T* pMemory, int numElements)
{
	Purge();

	CUtlMemory<T>::m_pMemory = (T*)Align(pMemory);
	CUtlMemory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)CUtlMemory<T>::m_pMemory) / sizeof(T);

	CUtlMemory<T>::m_nGrowSize = CUtlMemory<T>::EXTERNAL_BUFFER_MARKER;
}

template< class T, int nAlignment >
void CUtlMemoryAligned<T, nAlignment>::SetExternalBuffer(const T* pMemory, int numElements)
{
	Purge();

	CUtlMemory<T>::m_pMemory = (T*)Align(pMemory);
	CUtlMemory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)CUtlMemory<T>::m_pMemory) / sizeof(T);

	CUtlMemory<T>::m_nGrowSize = CUtlMemory<T>::EXTERNAL_CONST_BUFFER_MARKER;
}


template< class T, int nAlignment >
void CUtlMemoryAligned<T, nAlignment>::Grow(int num)
{
	Assert(num > 0);

	if (this->IsExternallyAllocated())
	{
		Assert(0);
		return;
	}

	UTLMEMORY_TRACK_FREE();

	int nAllocationRequested = CUtlMemory<T>::m_nAllocationCount + num;

	CUtlMemory<T>::m_nAllocationCount = UtlMemory_CalcNewAllocationCount(CUtlMemory<T>::m_nAllocationCount, CUtlMemory<T>::m_nGrowSize, nAllocationRequested, sizeof(T));

	UTLMEMORY_TRACK_ALLOC();

	if (CUtlMemory<T>::m_pMemory)
	{
		MEM_ALLOC_CREDIT_CLASS();
		CUtlMemory<T>::m_pMemory = (T*)MemAlloc_ReallocAligned(CUtlMemory<T>::m_pMemory, CUtlMemory<T>::m_nAllocationCount * sizeof(T), nAlignment);
		Assert(CUtlMemory<T>::m_pMemory);
	}
	else
	{
		MEM_ALLOC_CREDIT_CLASS();
		CUtlMemory<T>::m_pMemory = (T*)MemAlloc_AllocAligned(CUtlMemory<T>::m_nAllocationCount * sizeof(T), nAlignment);
		Assert(CUtlMemory<T>::m_pMemory);
	}
}


template< class T, int nAlignment >
inline void CUtlMemoryAligned<T, nAlignment>::EnsureCapacity(int num)
{
	if (CUtlMemory<T>::m_nAllocationCount >= num)
		return;

	if (this->IsExternallyAllocated())
	{
		Assert(0);
		return;
	}

	UTLMEMORY_TRACK_FREE();

	CUtlMemory<T>::m_nAllocationCount = num;

	UTLMEMORY_TRACK_ALLOC();

	if (CUtlMemory<T>::m_pMemory)
	{
		MEM_ALLOC_CREDIT_CLASS();
		CUtlMemory<T>::m_pMemory = (T*)MemAlloc_ReallocAligned(CUtlMemory<T>::m_pMemory, CUtlMemory<T>::m_nAllocationCount * sizeof(T), nAlignment);
	}
	else
	{
		MEM_ALLOC_CREDIT_CLASS();
		CUtlMemory<T>::m_pMemory = (T*)MemAlloc_AllocAligned(CUtlMemory<T>::m_nAllocationCount * sizeof(T), nAlignment);
	}
}


template< class T, int nAlignment >
void CUtlMemoryAligned<T, nAlignment>::Purge()
{
	if (!this->IsExternallyAllocated())
	{
		if (CUtlMemory<T>::m_pMemory)
		{
			UTLMEMORY_TRACK_FREE();
			MemAlloc_FreeAligned(CUtlMemory<T>::m_pMemory);
			CUtlMemory<T>::m_pMemory = 0;
		}
		CUtlMemory<T>::m_nAllocationCount = 0;
	}
}

#include "memdbgoff.h"

#endif  