#ifndef UTLBLOCKMEMORY_H
#define UTLBLOCKMEMORY_H

#ifdef _WIN32
#pragma once
#endif

#include "dbg.h"
#include "platform.h"
#include "mathlib.h"

#include "memalloc.h"
#include "memdbgon.h"

#pragma warning (disable:4100)
#pragma warning (disable:4514)

#ifdef UTBLOCKLMEMORY_TRACK
#define UTLBLOCKMEMORY_TRACK_ALLOC()		MemAlloc_RegisterAllocation( "Sum of all UtlBlockMemory", 0, NumAllocated() * sizeof(T), NumAllocated() * sizeof(T), 0 )
#define UTLBLOCKMEMORY_TRACK_FREE()		if ( !m_pMemory ) ; else MemAlloc_RegisterDeallocation( "Sum of all UtlBlockMemory", 0, NumAllocated() * sizeof(T), NumAllocated() * sizeof(T), 0 )
#else
#define UTLBLOCKMEMORY_TRACK_ALLOC()		((void)0)
#define UTLBLOCKMEMORY_TRACK_FREE()		((void)0)
#endif


template< class T, class I >
class CUtlBlockMemory
{
public:
	CUtlBlockMemory(int nGrowSize = 0, int nInitSize = 0);
	~CUtlBlockMemory();

	void Init(int nGrowSize = 0, int nInitSize = 0);

	T* Base() { return NULL; }
	const T* Base() const { return NULL; }

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
	static I InvalidIndex() { return (I)-1; }

	void Swap(CUtlBlockMemory< T, I >& mem);

	int NumAllocated() const;
	int Count() const { return NumAllocated(); }

	void Grow(int num = 1);

	void EnsureCapacity(int num);

	void Purge();

	void Purge(int numElements);

protected:
	int Index(int major, int minor) const { return (major << m_nIndexShift) | minor; }
	int MajorIndex(int i) const { return i >> m_nIndexShift; }
	int MinorIndex(int i) const { return i & m_nIndexMask; }
	void ChangeSize(int nBlocks);
	int NumElementsInBlock() const { return m_nIndexMask + 1; }

	T** m_pMemory;
	int m_nBlocks;
	int m_nIndexMask : 27;
	int m_nIndexShift : 5;
};

template< class T, class I >
CUtlBlockMemory<T, I>::CUtlBlockMemory(int nGrowSize, int nInitAllocationCount)
	: m_pMemory(0), m_nBlocks(0), m_nIndexMask(0), m_nIndexShift(0)
{
	Init(nGrowSize, nInitAllocationCount);
}

template< class T, class I >
CUtlBlockMemory<T, I>::~CUtlBlockMemory()
{
	Purge();
}


template< class T, class I >
void CUtlBlockMemory<T, I>::Swap(CUtlBlockMemory< T, I >& mem)
{
	swap(m_pMemory, mem.m_pMemory);
	swap(m_nBlocks, mem.m_nBlocks);
	swap(m_nIndexMask, mem.m_nIndexMask);
	swap(m_nIndexShift, mem.m_nIndexShift);
}


template< class T, class I >
void CUtlBlockMemory<T, I>::Init(int nGrowSize    , int nInitSize    )
{
	Purge();

	if (nGrowSize == 0)
	{
		nGrowSize = (127 + sizeof(T)) / sizeof(T);
	}
	nGrowSize = SmallestPowerOfTwoGreaterOrEqual(nGrowSize);
	m_nIndexMask = nGrowSize - 1;

	m_nIndexShift = 0;
	while (nGrowSize > 1)
	{
		nGrowSize >>= 1;
		++m_nIndexShift;
	}
	Assert(m_nIndexMask + 1 == (1 << m_nIndexShift));

	Grow(nInitSize);
}


template< class T, class I >
inline T& CUtlBlockMemory<T, I>::operator[](I i)
{
	Assert(IsIdxValid(i));
	T* pBlock = m_pMemory[MajorIndex(i)];
	return pBlock[MinorIndex(i)];
}

template< class T, class I >
inline const T& CUtlBlockMemory<T, I>::operator[](I i) const
{
	Assert(IsIdxValid(i));
	const T* pBlock = m_pMemory[MajorIndex(i)];
	return pBlock[MinorIndex(i)];
}

template< class T, class I >
inline T& CUtlBlockMemory<T, I>::Element(I i)
{
	Assert(IsIdxValid(i));
	T* pBlock = m_pMemory[MajorIndex(i)];
	return pBlock[MinorIndex(i)];
}

template< class T, class I >
inline const T& CUtlBlockMemory<T, I>::Element(I i) const
{
	Assert(IsIdxValid(i));
	const T* pBlock = m_pMemory[MajorIndex(i)];
	return pBlock[MinorIndex(i)];
}


template< class T, class I >
inline int CUtlBlockMemory<T, I>::NumAllocated() const
{
	return m_nBlocks * NumElementsInBlock();
}


template< class T, class I >
inline bool CUtlBlockMemory<T, I>::IsIdxValid(I i) const
{
	return (i >= 0) && (MajorIndex(i) < m_nBlocks);
}

template< class T, class I >
void CUtlBlockMemory<T, I>::Grow(int num)
{
	if (num <= 0)
		return;

	int nBlockSize = NumElementsInBlock();
	int nBlocks = (num + nBlockSize - 1) / nBlockSize;

	ChangeSize(m_nBlocks + nBlocks);
}

template< class T, class I >
void CUtlBlockMemory<T, I>::ChangeSize(int nBlocks)
{
	UTLBLOCKMEMORY_TRACK_FREE();                

	int nBlocksOld = m_nBlocks;
	m_nBlocks = nBlocks;

	UTLBLOCKMEMORY_TRACK_ALLOC();                

	if (m_pMemory)
	{
		for (int i = m_nBlocks; i < nBlocksOld; ++i)
		{
			UTLBLOCKMEMORY_TRACK_FREE();
			free((void*)m_pMemory[i]);
		}

		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T**)realloc(m_pMemory, m_nBlocks * sizeof(T*));
		Assert(m_pMemory);
	}
	else
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T**)malloc(m_nBlocks * sizeof(T*));
		Assert(m_pMemory);
	}

	if (!m_pMemory)
	{
		Error("CUtlBlockMemory overflow!\n");
	}

	int nBlockSize = NumElementsInBlock();
	for (int i = nBlocksOld; i < m_nBlocks; ++i)
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory[i] = (T*)malloc(nBlockSize * sizeof(T));
		Assert(m_pMemory[i]);
	}
}


template< class T, class I >
inline void CUtlBlockMemory<T, I>::EnsureCapacity(int num)
{
	Grow(num - NumAllocated());
}


template< class T, class I >
void CUtlBlockMemory<T, I>::Purge()
{
	if (!m_pMemory)
		return;

	for (int i = 0; i < m_nBlocks; ++i)
	{
		UTLBLOCKMEMORY_TRACK_FREE();
		free((void*)m_pMemory[i]);
	}
	m_nBlocks = 0;

	UTLBLOCKMEMORY_TRACK_FREE();
	free((void*)m_pMemory);
	m_pMemory = 0;
}

template< class T, class I >
void CUtlBlockMemory<T, I>::Purge(int numElements)
{
	Assert(numElements >= 0);

	int nAllocated = NumAllocated();
	if (numElements > nAllocated)
	{
		Assert(numElements <= nAllocated);
		return;
	}

	if (numElements <= 0)
	{
		Purge();
		return;
	}

	int nBlockSize = NumElementsInBlock();
	int nBlocksOld = m_nBlocks;
	int nBlocks = (numElements + nBlockSize - 1) / nBlockSize;

	if (nBlocks == m_nBlocks)
		return;

	ChangeSize(nBlocks);
}

#include "memdbgoff.h"

#endif  