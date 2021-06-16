#ifndef UTLVECTOR_H
#define UTLVECTOR_H

#ifdef _WIN32
#pragma once
#endif


#include <string.h>
#include "platform.h"
#include "dbg.h"
#include "threadtools.h"
#include "utlmemory.h"
#include "utlblockmemory.h"
#include "strtools.h"
#include "random.h"

#define FOR_EACH_VEC( vecName, iteratorName ) \
	for ( int iteratorName = 0; iteratorName < (vecName).Count(); iteratorName++ )
#define FOR_EACH_VEC_BACK( vecName, iteratorName ) \
	for ( int iteratorName = (vecName).Count()-1; iteratorName >= 0; iteratorName-- )


template< class T, class A = CUtlMemory<T> >
class CUtlVector
{
	typedef A CAllocator;
public:
	typedef T ElemType_t;

	CUtlVector(int growSize = 0, int initSize = 0);
	CUtlVector(T* pMemory, int allocationCount, int numElements = 0);
	~CUtlVector();

	CUtlVector<T, A>& operator=(const CUtlVector<T, A>& other);

	T& operator[](int i);
	const T& operator[](int i) const;
	T& Element(int i);
	const T& Element(int i) const;
	T& Head();
	const T& Head() const;
	T& Tail();
	const T& Tail() const;

	T* Base() { return m_Memory.Base(); }
	const T* Base() const { return m_Memory.Base(); }
	int Count() const;
	bool IsValidIndex(int i) const;
	static int InvalidIndex();
	int AddToHead();
	int AddToTail();
	int InsertBefore(int elem);
	int InsertAfter(int elem);
	int AddToHead(const T& src);
	int AddToTail(const T& src);
	int InsertBefore(int elem, const T& src);
	int InsertAfter(int elem, const T& src);
	int AddMultipleToHead(int num);
	int AddMultipleToTail(int num);
	int AddMultipleToTail(int num, const T* pToCopy);
	int InsertMultipleBefore(int elem, int num);
	int InsertMultipleBefore(int elem, int num, const T* pToCopy);
	int InsertMultipleAfter(int elem, int num);
	void SetSize(int size);
	void SetCount(int count);
	void SetCountNonDestructively(int count);                   
	void CopyArray(const T* pArray, int size);      
	void Swap(CUtlVector< T, A >& vec);
	int AddVectorToTail(CUtlVector<T, A> const& src);
	int GetOffset(const T& src) const;
	void FillWithValue(const T& src);
	bool HasElement(const T& src) const;
	void EnsureCapacity(int num);
	void EnsureCount(int num);
	void FastRemove(int elem);       
	void Remove(int elem);            
	bool FindAndRemove(const T& src);             
	bool FindAndFastRemove(const T& src);            
	void RemoveMultiple(int elem, int num);        
	void RemoveMultipleFromHead(int num);      
	void RemoveMultipleFromTail(int num);      
	void RemoveAll();                   
	void Purge();   
	void PurgeAndDeleteElements();
	void Compact();
	void SetGrowSize(int size) { m_Memory.SetGrowSize(size); }
	int NumAllocated() const;              
	void Sort(int(__cdecl* pfnCompare)(const T*, const T*));

protected:
	CUtlVector(CUtlVector const& vec) { assert(0); }

	void GrowVector(int num = 1);

	void ShiftElementsRight(int elem, int num = 1);
	void ShiftElementsLeft(int elem, int num = 1);

public:
	CAllocator m_Memory;
	int m_Size;

	T* m_pElements;

	inline void ResetDbgInfo()
	{
		m_pElements = Base();
	}
};
template < class T >
class CUtlBlockVector : public CUtlVector< T, CUtlBlockMemory< T, int > >
{
public:
	explicit CUtlBlockVector(int growSize = 0, int initSize = 0)
		: CUtlVector< T, CUtlBlockMemory< T, int > >(growSize, initSize) {}

private:
	typedef CUtlVector< T, CUtlBlockMemory< T, int > > Base;
	typename Base::iterator begin();
	typename Base::const_iterator begin() const;
	typename Base::iterator end();
	typename Base::const_iterator end() const;
};

template< class BASE_UTLVECTOR, class MUTEX_TYPE = CThreadFastMutex >
class CUtlVectorMT : public BASE_UTLVECTOR, public MUTEX_TYPE
{
	typedef BASE_UTLVECTOR BaseClass;
public:
	MUTEX_TYPE Mutex_t;

	explicit CUtlVectorMT(int growSize = 0, int initSize = 0) : BaseClass(growSize, initSize) {}
	explicit CUtlVectorMT(typename BaseClass::ElemType_t* pMemory, int numElements) : BaseClass(pMemory, numElements) {}
};


template< class T, size_t MAX_SIZE >
class CUtlVectorFixed : public CUtlVector< T, CUtlMemoryFixed<T, MAX_SIZE > >
{
	typedef CUtlVector< T, CUtlMemoryFixed<T, MAX_SIZE > > BaseClass;
public:

	explicit CUtlVectorFixed(int growSize = 0, int initSize = 0) : BaseClass(growSize, initSize) {}
	explicit CUtlVectorFixed(T* pMemory, int numElements) : BaseClass(pMemory, numElements) {}
};


template< class T, size_t MAX_SIZE >
class CUtlVectorFixedGrowable : public CUtlVector< T, CUtlMemoryFixedGrowable<T, MAX_SIZE > >
{
	typedef CUtlVector< T, CUtlMemoryFixedGrowable<T, MAX_SIZE > > BaseClass;

public:
	explicit CUtlVectorFixedGrowable(int growSize = 0) : BaseClass(growSize, MAX_SIZE) {}
};


template< class T >
class CUtlVectorConservative : public CUtlVector< T, CUtlMemoryConservative<T> >
{
	typedef CUtlVector< T, CUtlMemoryConservative<T> > BaseClass;
public:

	explicit CUtlVectorConservative(int growSize = 0, int initSize = 0) : BaseClass(growSize, initSize) {}
	explicit CUtlVectorConservative(T* pMemory, int numElements) : BaseClass(pMemory, numElements) {}
};


#pragma warning(push)
#pragma warning(disable : 4200)           
#pragma warning(disable : 4815 )              

class CUtlVectorUltraConservativeAllocator
{
public:
	static void* Alloc(size_t nSize)
	{
		return malloc(nSize);
	}

	static void* Realloc(void* pMem, size_t nSize)
	{
		return realloc(pMem, nSize);
	}

	static void Free(void* pMem)
	{
		free(pMem);
	}

	static size_t GetSize(void* pMem)
	{
		return mallocsize(pMem);
	}

};

template <typename T, typename A = CUtlVectorUltraConservativeAllocator >
class CUtlVectorUltraConservative : private A
{
public:
	CUtlVectorUltraConservative()
	{
		m_pData = StaticData();
	}

	~CUtlVectorUltraConservative()
	{
		RemoveAll();
	}

	int Count() const
	{
		return m_pData->m_Size;
	}

	static int InvalidIndex()
	{
		return -1;
	}

	inline bool IsValidIndex(int i) const
	{
		return (i >= 0) && (i < Count());
	}

	T& operator[](int i)
	{
		Assert(IsValidIndex(i));
		return m_pData->m_Elements[i];
	}

	const T& operator[](int i) const
	{
		Assert(IsValidIndex(i));
		return m_pData->m_Elements[i];
	}

	T& Element(int i)
	{
		Assert(IsValidIndex(i));
		return m_pData->m_Elements[i];
	}

	const T& Element(int i) const
	{
		Assert(IsValidIndex(i));
		return m_pData->m_Elements[i];
	}

	void EnsureCapacity(int num)
	{
		int nCurCount = Count();
		if (num <= nCurCount)
		{
			return;
		}
		if (m_pData == StaticData())
		{
			m_pData = (Data_t*)A::Alloc(sizeof(int) + (num * sizeof(T)));
			m_pData->m_Size = 0;
		}
		else
		{
			int nNeeded = sizeof(int) + (num * sizeof(T));
			int nHave = A::GetSize(m_pData);
			if (nNeeded > nHave)
			{
				m_pData = (Data_t*)A::Realloc(m_pData, nNeeded);
			}
		}
	}

	int AddToTail(const T& src)
	{
		int iNew = Count();
		EnsureCapacity(Count() + 1);
		m_pData->m_Elements[iNew] = src;
		m_pData->m_Size++;
		return iNew;
	}

	void RemoveAll()
	{
		if (Count())
		{
			for (int i = m_pData->m_Size; --i >= 0; )
			{
				Destruct(&m_pData->m_Elements[i]);
			}
		}
		if (m_pData != StaticData())
		{
			A::Free(m_pData);
			m_pData = StaticData();

		}
	}

	void PurgeAndDeleteElements()
	{
		if (m_pData != StaticData())
		{
			for (int i = 0; i < m_pData->m_Size; i++)
			{
				delete Element(i);
			}
			RemoveAll();
		}
	}

	void FastRemove(int elem)
	{
		Assert(IsValidIndex(elem));

		Destruct(&Element(elem));
		if (Count() > 0)
		{
			if (elem != m_pData->m_Size - 1)
				memcpy(&Element(elem), &Element(m_pData->m_Size - 1), sizeof(T));
			--m_pData->m_Size;
		}
		if (!m_pData->m_Size)
		{
			A::Free(m_pData);
			m_pData = StaticData();
		}
	}

	void Remove(int elem)
	{
		Destruct(&Element(elem));
		ShiftElementsLeft(elem);
		--m_pData->m_Size;
		if (!m_pData->m_Size)
		{
			A::Free(m_pData);
			m_pData = StaticData();
		}
	}

	int Find(const T& src) const
	{
		int nCount = Count();
		for (int i = 0; i < nCount; ++i)
		{
			if (Element(i) == src)
				return i;
		}
		return -1;
	}

	bool FindAndRemove(const T& src)
	{
		int elem = Find(src);
		if (elem != -1)
		{
			Remove(elem);
			return true;
		}
		return false;
	}


	bool FindAndFastRemove(const T& src)
	{
		int elem = Find(src);
		if (elem != -1)
		{
			FastRemove(elem);
			return true;
		}
		return false;
	}

	struct Data_t
	{
		int m_Size;
		T m_Elements[0];
	};

	Data_t* m_pData;
private:
	void ShiftElementsLeft(int elem, int num = 1)
	{
		int Size = Count();
		Assert(IsValidIndex(elem) || (Size == 0) || (num == 0));
		int numToMove = Size - elem - num;
		if ((numToMove > 0) && (num > 0))
		{
			Q_memmove(&Element(elem), &Element(elem + num), numToMove * sizeof(T));

#ifdef _DEBUG
			Q_memset(&Element(Size - num), 0xDD, num * sizeof(T));
#endif
		}
	}



	static Data_t* StaticData()
	{
		static Data_t staticData;
		Assert(staticData.m_Size == 0);
		return &staticData;
	}
};

#pragma warning(pop)


template< typename T, typename A = CUtlMemory<T> >
class CCopyableUtlVector : public CUtlVector< T, A >
{
	typedef CUtlVector< T, A > BaseClass;
public:
	explicit CCopyableUtlVector(int growSize = 0, int initSize = 0) : BaseClass(growSize, initSize) {}
	explicit CCopyableUtlVector(T* pMemory, int numElements) : BaseClass(pMemory, numElements) {}
	virtual ~CCopyableUtlVector() {}
	CCopyableUtlVector(CCopyableUtlVector const& vec) { this->CopyArray(vec.Base(), vec.Count()); }
};

template< typename T, class A >
inline CUtlVector<T, A>::CUtlVector(int growSize, int initSize) :
	m_Memory(growSize, initSize), m_Size(0)
{
	ResetDbgInfo();
}

template< typename T, class A >
inline CUtlVector<T, A>::CUtlVector(T* pMemory, int allocationCount, int numElements) :
	m_Memory(pMemory, allocationCount), m_Size(numElements)
{
	ResetDbgInfo();
}

template< typename T, class A >
inline CUtlVector<T, A>::~CUtlVector()
{
	Purge();
}

template< typename T, class A >
inline CUtlVector<T, A>& CUtlVector<T, A>::operator=(const CUtlVector<T, A>& other)
{
	int nCount = other.Count();
	SetSize(nCount);
	for (int i = 0; i < nCount; i++)
	{
		(*this)[i] = other[i];
	}
	return *this;
}

#ifdef STAGING_ONLY
inline void StagingUtlVectorBoundsCheck(int i, int size)
{
	if ((unsigned)i >= (unsigned)size)
	{
		Msg("Array access error: %d / %d\n", i, size);
		DebuggerBreak();
	}
}

#else
#define StagingUtlVectorBoundsCheck( _i, _size )
#endif

template< typename T, class A >
inline T& CUtlVector<T, A>::operator[](int i)
{
	Assert((unsigned)i < (unsigned)m_Size);
	StagingUtlVectorBoundsCheck(i, m_Size);
	return m_Memory[i];
}

template< typename T, class A >
inline const T& CUtlVector<T, A>::operator[](int i) const
{
	Assert((unsigned)i < (unsigned)m_Size);
	StagingUtlVectorBoundsCheck(i, m_Size);
	return m_Memory[i];
}

template< typename T, class A >
inline T& CUtlVector<T, A>::Element(int i)
{
	Assert((unsigned)i < (unsigned)m_Size);
	StagingUtlVectorBoundsCheck(i, m_Size);
	return m_Memory[i];
}

template< typename T, class A >
inline const T& CUtlVector<T, A>::Element(int i) const
{
	Assert((unsigned)i < (unsigned)m_Size);
	StagingUtlVectorBoundsCheck(i, m_Size);
	return m_Memory[i];
}

template< typename T, class A >
inline T& CUtlVector<T, A>::Head()
{
	Assert(m_Size > 0);
	StagingUtlVectorBoundsCheck(0, m_Size);
	return m_Memory[0];
}

template< typename T, class A >
inline const T& CUtlVector<T, A>::Head() const
{
	Assert(m_Size > 0);
	StagingUtlVectorBoundsCheck(0, m_Size);
	return m_Memory[0];
}

template< typename T, class A >
inline T& CUtlVector<T, A>::Tail()
{
	Assert(m_Size > 0);
	StagingUtlVectorBoundsCheck(0, m_Size);
	return m_Memory[m_Size - 1];
}

template< typename T, class A >
inline const T& CUtlVector<T, A>::Tail() const
{
	Assert(m_Size > 0);
	StagingUtlVectorBoundsCheck(0, m_Size);
	return m_Memory[m_Size - 1];
}


template< typename T, class A >
inline int CUtlVector<T, A>::Count() const
{
	return m_Size;
}

template< typename T, class A >
inline bool CUtlVector<T, A>::IsValidIndex(int i) const
{
	return (i >= 0) && (i < m_Size);
}


template< typename T, class A >
inline int CUtlVector<T, A>::InvalidIndex()
{
	return -1;
}


template< typename T, class A >
void CUtlVector<T, A>::GrowVector(int num)
{
	if (m_Size + num > m_Memory.NumAllocated())
	{
		MEM_ALLOC_CREDIT_CLASS();
		m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());
	}

	m_Size += num;
	ResetDbgInfo();
}


template< typename T, class A >
void CUtlVector<T, A>::Sort(int(__cdecl* pfnCompare)(const T*, const T*))
{
	typedef int(__cdecl* QSortCompareFunc_t)(const void*, const void*);
	if (Count() <= 1)
		return;

	if (Base())
	{
		qsort(Base(), Count(), sizeof(T), (QSortCompareFunc_t)(pfnCompare));
	}
	else
	{
		Assert(0);
		for (int i = m_Size - 1; i >= 0; --i)
		{
			for (int j = 1; j <= i; ++j)
			{
				if (pfnCompare(&Element(j - 1), &Element(j)) < 0)
				{
					V_swap(Element(j - 1), Element(j));
				}
			}
		}
	}
}

template< typename T, class A >
void CUtlVector<T, A>::EnsureCapacity(int num)
{
	MEM_ALLOC_CREDIT_CLASS();
	m_Memory.EnsureCapacity(num);
	ResetDbgInfo();
}


template< typename T, class A >
void CUtlVector<T, A>::EnsureCount(int num)
{
	if (Count() < num)
		AddMultipleToTail(num - Count());
}


template< typename T, class A >
void CUtlVector<T, A>::ShiftElementsRight(int elem, int num)
{
	Assert(IsValidIndex(elem) || (m_Size == 0) || (num == 0));
	int numToMove = m_Size - elem - num;
	if ((numToMove > 0) && (num > 0))
		Q_memmove(&Element(elem + num), &Element(elem), numToMove * sizeof(T));
}

template< typename T, class A >
void CUtlVector<T, A>::ShiftElementsLeft(int elem, int num)
{
	Assert(IsValidIndex(elem) || (m_Size == 0) || (num == 0));
	int numToMove = m_Size - elem - num;
	if ((numToMove > 0) && (num > 0))
	{
		Q_memmove(&Element(elem), &Element(elem + num), numToMove * sizeof(T));

#ifdef _DEBUG
		Q_memset(&Element(m_Size - num), 0xDD, num * sizeof(T));
#endif
	}
}


template< typename T, class A >
inline int CUtlVector<T, A>::AddToHead()
{
	return InsertBefore(0);
}

template< typename T, class A >
inline int CUtlVector<T, A>::AddToTail()
{
	return InsertBefore(m_Size);
}

template< typename T, class A >
inline int CUtlVector<T, A>::InsertAfter(int elem)
{
	return InsertBefore(elem + 1);
}

template< typename T, class A >
int CUtlVector<T, A>::InsertBefore(int elem)
{
	Assert((elem == Count()) || IsValidIndex(elem));

	GrowVector();
	ShiftElementsRight(elem);
	Construct(&Element(elem));
	return elem;
}


template< typename T, class A >
inline int CUtlVector<T, A>::AddToHead(const T& src)
{
	Assert((Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count())));
	return InsertBefore(0, src);
}

template< typename T, class A >
inline int CUtlVector<T, A>::AddToTail(const T& src)
{
	Assert((Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count())));
	return InsertBefore(m_Size, src);
}

template< typename T, class A >
inline int CUtlVector<T, A>::InsertAfter(int elem, const T& src)
{
	Assert((Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count())));
	return InsertBefore(elem + 1, src);
}

template< typename T, class A >
int CUtlVector<T, A>::InsertBefore(int elem, const T& src)
{
	Assert((Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count())));

	Assert((elem == Count()) || IsValidIndex(elem));

	GrowVector();
	ShiftElementsRight(elem);
	CopyConstruct(&Element(elem), src);
	return elem;
}


template< typename T, class A >
inline int CUtlVector<T, A>::AddMultipleToHead(int num)
{
	return InsertMultipleBefore(0, num);
}

template< typename T, class A >
inline int CUtlVector<T, A>::AddMultipleToTail(int num, const T* pToCopy)
{
	Assert((Base() == NULL) || !pToCopy || (pToCopy + num < Base()) || (pToCopy >= (Base() + Count())));

	return InsertMultipleBefore(m_Size, num, pToCopy);
}

template< typename T, class A >
int CUtlVector<T, A>::InsertMultipleAfter(int elem, int num)
{
	return InsertMultipleBefore(elem + 1, num);
}


template< typename T, class A >
void CUtlVector<T, A>::SetCount(int count)
{
	RemoveAll();
	AddMultipleToTail(count);
}

template< typename T, class A >
inline void CUtlVector<T, A>::SetSize(int size)
{
	SetCount(size);
}

template< typename T, class A >
void CUtlVector<T, A>::SetCountNonDestructively(int count)
{
	int delta = count - m_Size;
	if (delta > 0) AddMultipleToTail(delta);
	else if (delta < 0) RemoveMultipleFromTail(-delta);
}

template< typename T, class A >
void CUtlVector<T, A>::CopyArray(const T* pArray, int size)
{
	Assert((Base() == NULL) || !pArray || (Base() >= (pArray + size)) || (pArray >= (Base() + Count())));

	SetSize(size);
	for (int i = 0; i < size; i++)
	{
		(*this)[i] = pArray[i];
	}
}

template< typename T, class A >
void CUtlVector<T, A>::Swap(CUtlVector< T, A >& vec)
{
	m_Memory.Swap(vec.m_Memory);
	V_swap(m_Size, vec.m_Size);

#ifndef _X360
	V_swap(m_pElements, vec.m_pElements);
#endif
}

template< typename T, class A >
int CUtlVector<T, A>::AddVectorToTail(CUtlVector const& src)
{
	Assert(&src != this);

	int base = Count();

	AddMultipleToTail(src.Count());

	for (int i = 0; i < src.Count(); i++)
	{
		(*this)[base + i] = src[i];
	}

	return base;
}

template< typename T, class A >
inline int CUtlVector<T, A>::InsertMultipleBefore(int elem, int num, const T* pToInsert)
{
	if (num == 0)
		return elem;

	Assert((elem == Count()) || IsValidIndex(elem));

	GrowVector(num);
	ShiftElementsRight(elem, num);

	for (int i = 0; i < num; ++i)
		Construct(&Element(elem + i));

	if (pToInsert)
	{
		for (int i = 0; i < num; i++)
		{
			Element(elem + i) = pToInsert[i];
		}
	}

	return elem;
}


template< typename T, class A >
int CUtlVector<T, A>::GetOffset(const T& src) const
{
	return -1;
}
template< typename T, class A >
bool CUtlVector<T, A>::HasElement(const T& src) const
{
	return (Find(src) >= 0);
}


template< typename T, class A >
void CUtlVector<T, A>::FastRemove(int elem)
{
	Assert(IsValidIndex(elem));

	Destruct(&Element(elem));
	if (m_Size > 0)
	{
		if (elem != m_Size - 1)
			memcpy(reinterpret_cast<void*>(&Element(elem)), reinterpret_cast<void*>(&Element(m_Size - 1)), sizeof(T));
		--m_Size;
	}
}

template< typename T, class A >
void CUtlVector<T, A>::Remove(int elem)
{
	Destruct(&Element(elem));
	ShiftElementsLeft(elem);
	--m_Size;
}

template< typename T, class A >
bool CUtlVector<T, A>::FindAndRemove(const T& src)
{
	int elem = GetOffset(src);
	if (elem != -1)
	{
		Remove(elem);
		return true;
	}
	return false;
}

template< typename T, class A >
bool CUtlVector<T, A>::FindAndFastRemove(const T& src)
{
	int elem = Find(src);
	if (elem != -1)
	{
		FastRemove(elem);
		return true;
	}
	return false;
}

template< typename T, class A >
void CUtlVector<T, A>::RemoveMultiple(int elem, int num)
{
	Assert(elem >= 0);
	Assert(elem + num <= Count());

	for (int i = elem + num; --i >= elem; )
		Destruct(&Element(i));

	ShiftElementsLeft(elem, num);
	m_Size -= num;
}

template< typename T, class A >
void CUtlVector<T, A>::RemoveMultipleFromHead(int num)
{
	Assert(num <= Count());

	for (int i = num; --i >= 0; )
		Destruct(&Element(i));

	ShiftElementsLeft(0, num);
	m_Size -= num;
}

template< typename T, class A >
void CUtlVector<T, A>::RemoveMultipleFromTail(int num)
{
	Assert(num <= Count());

	for (int i = m_Size - num; i < m_Size; i++)
		Destruct(&Element(i));

	m_Size -= num;
}

template< typename T, class A >
void CUtlVector<T, A>::RemoveAll()
{
	for (int i = m_Size; --i >= 0; )
	{
		Destruct(&Element(i));
	}

	m_Size = 0;
}


template< typename T, class A >
inline void CUtlVector<T, A>::Purge()
{
	RemoveAll();
	m_Memory.Purge();
	ResetDbgInfo();
}


template< typename T, class A >
inline void CUtlVector<T, A>::PurgeAndDeleteElements()
{
	for (int i = 0; i < m_Size; i++)
	{
		delete Element(i);
	}
	Purge();
}

template< typename T, class A >
inline void CUtlVector<T, A>::Compact()
{
	m_Memory.Purge(m_Size);
}

template< typename T, class A >
inline int CUtlVector<T, A>::NumAllocated() const
{
	return m_Memory.NumAllocated();
}


#ifdef DBGFLAG_VALIDATE
template< typename T, class A >
void CUtlVector<T, A>::Validate(CValidator& validator, char* pchName)
{
	validator.Push(typeid(*this).name(), this, pchName);

	m_Memory.Validate(validator, "m_Memory");

	validator.Pop();
}
#endif  

template<class T> class CUtlVectorAutoPurge : public CUtlVector< T, CUtlMemory< T, int> >
{
public:
	~CUtlVectorAutoPurge(void)
	{
		this->PurgeAndDeleteElements();
	}

};

class CUtlStringList : public CUtlVectorAutoPurge< char*>
{
public:
	void CopyAndAddToTail(char const* pString)			        
	{
		char* pNewStr = new char[1 + strlen(pString)];
		V_strcpy(pNewStr, pString);
		AddToTail(pNewStr);
	}

	static int __cdecl SortFunc(char* const* sz1, char* const* sz2)
	{
		return strcmp(*sz1, *sz2);
	}

	inline void PurgeAndDeleteElements()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete[] Element(i);
		}
		Purge();
	}

	~CUtlStringList(void)
	{
		this->PurgeAndDeleteElements();
	}
};



class CSplitString : public CUtlVector<char*, CUtlMemory<char*, int> >
{
public:
	CSplitString(const char* pString, const char* pSeparator);
	CSplitString(const char* pString, const char** pSeparators, int nSeparators);
	~CSplitString();
private:
	void Construct(const char* pString, const char** pSeparators, int nSeparators);
	void PurgeAndDeleteElements();
private:
	char* m_szBuffer;           
};


#endif  