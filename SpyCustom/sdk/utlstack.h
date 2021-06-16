#ifndef UTLSTACK_H
#define UTLSTACK_H

#include <assert.h>
#include <string.h>
#include "utlmemory.h"


template< class T, class M = CUtlMemory< T > >
class CUtlStack
{
public:
	CUtlStack(int growSize = 0, int initSize = 0);
	~CUtlStack();

	void CopyFrom(const CUtlStack<T, M>& from);

	T& operator[](int i);
	T const& operator[](int i) const;
	T& Element(int i);
	T const& Element(int i) const;

	T* Base();
	T const* Base() const;

	T& Top();
	T const& Top() const;

	int Count() const;

	bool IsIdxValid(int i) const;

	int Push();

	int Push(T const& src);

	void Pop();
	void Pop(T& oldTop);
	void PopMultiple(int num);

	void EnsureCapacity(int num);

	void Clear();

	void Purge();

private:
	void GrowStack();

	void ResetDbgInfo();

	M m_Memory;
	int m_Size;

	T* m_pElements;
};


template< class T, class M >
inline void CUtlStack<T, M>::ResetDbgInfo()
{
	m_pElements = m_Memory.Base();
}

template< class T, class M >
CUtlStack<T, M>::CUtlStack(int growSize, int initSize) :
	m_Memory(growSize, initSize), m_Size(0)
{
	ResetDbgInfo();
}

template< class T, class M >
CUtlStack<T, M>::~CUtlStack()
{
	Purge();
}


template< class T, class M >
void CUtlStack<T, M>::CopyFrom(const CUtlStack<T, M>& from)
{
	Purge();
	EnsureCapacity(from.Count());
	for (int i = 0; i < from.Count(); i++)
	{
		Push(from[i]);
	}
}

template< class T, class M >
inline T& CUtlStack<T, M>::operator[](int i)
{
	assert(IsIdxValid(i));
	return m_Memory[i];
}

template< class T, class M >
inline T const& CUtlStack<T, M>::operator[](int i) const
{
	assert(IsIdxValid(i));
	return m_Memory[i];
}

template< class T, class M >
inline T& CUtlStack<T, M>::Element(int i)
{
	assert(IsIdxValid(i));
	return m_Memory[i];
}

template< class T, class M >
inline T const& CUtlStack<T, M>::Element(int i) const
{
	assert(IsIdxValid(i));
	return m_Memory[i];
}


template< class T, class M >
inline T* CUtlStack<T, M>::Base()
{
	return m_Memory.Base();
}

template< class T, class M >
inline T const* CUtlStack<T, M>::Base() const
{
	return m_Memory.Base();
}

template< class T, class M >
inline T& CUtlStack<T, M>::Top()
{
	assert(m_Size > 0);
	return Element(m_Size - 1);
}

template< class T, class M >
inline T const& CUtlStack<T, M>::Top() const
{
	assert(m_Size > 0);
	return Element(m_Size - 1);
}

template< class T, class M >
inline int CUtlStack<T, M>::Count() const
{
	return m_Size;
}


template< class T, class M >
inline bool CUtlStack<T, M>::IsIdxValid(int i) const
{
	return (i >= 0) && (i < m_Size);
}

template< class T, class M >
void CUtlStack<T, M>::GrowStack()
{
	if (m_Size >= m_Memory.NumAllocated())
		m_Memory.Grow();

	++m_Size;

	ResetDbgInfo();
}

template< class T, class M >
void CUtlStack<T, M>::EnsureCapacity(int num)
{
	m_Memory.EnsureCapacity(num);
	ResetDbgInfo();
}


template< class T, class M >
int CUtlStack<T, M>::Push()
{
	GrowStack();
	Construct(&Element(m_Size - 1));
	return m_Size - 1;
}

template< class T, class M >
int CUtlStack<T, M>::Push(T const& src)
{
	GrowStack();
	CopyConstruct(&Element(m_Size - 1), src);
	return m_Size - 1;
}


template< class T, class M >
void CUtlStack<T, M>::Pop()
{
	assert(m_Size > 0);
	Destruct(&Element(m_Size - 1));
	--m_Size;
}

template< class T, class M >
void CUtlStack<T, M>::Pop(T& oldTop)
{
	assert(m_Size > 0);
	oldTop = Top();
	Pop();
}

template< class T, class M >
void CUtlStack<T, M>::PopMultiple(int num)
{
	assert(m_Size >= num);
	for (int i = 0; i < num; ++i)
		Destruct(&Element(m_Size - i - 1));
	m_Size -= num;
}


template< class T, class M >
void CUtlStack<T, M>::Clear()
{
	for (int i = m_Size; --i >= 0; )
		Destruct(&Element(i));

	m_Size = 0;
}


template< class T, class M >
void CUtlStack<T, M>::Purge()
{
	Clear();
	m_Memory.Purge();
	ResetDbgInfo();
}

#endif  