#ifndef UTLQUEUE_H
#define UTLQUEUE_H
#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"

template< class T >
class CUtlQueue
{
public:

	CUtlQueue(int growSize = 0, int initSize = 0);
	CUtlQueue(T* pMemory, int numElements);

	T const& RemoveAtHead();
	T const& RemoveAtTail();

	T const& Head();
	T const& Tail();

	void	Insert(T const& element);

	bool		Check(T const element);

	int			Count() const { return m_heap.Count(); }

	void		RemoveAll() { m_heap.RemoveAll(); }

	void		Purge() { m_heap.Purge(); }

protected:
	CUtlVector<T>	m_heap;
	T				m_current;
};

template< class T >
inline CUtlQueue<T>::CUtlQueue(int growSize, int initSize) :
	m_heap(growSize, initSize)
{
}

template< class T >
inline CUtlQueue<T>::CUtlQueue(T* pMemory, int numElements) :
	m_heap(pMemory, numElements)
{
}

template <class T>
inline T const& CUtlQueue<T>::RemoveAtHead()
{
	m_current = m_heap[0];
	m_heap.Remove((int)0);
	return m_current;
}

template <class T>
inline T const& CUtlQueue<T>::RemoveAtTail()
{
	m_current = m_heap[m_heap.Count() - 1];
	m_heap.Remove((int)(m_heap.Count() - 1));
	return m_current;
}

template <class T>
inline T const& CUtlQueue<T>::Head()
{
	m_current = m_heap[0];
	return m_current;
}

template <class T>
inline T const& CUtlQueue<T>::Tail()
{
	m_current = m_heap[m_heap.Count() - 1];
	return m_current;
}

template <class T>
void CUtlQueue<T>::Insert(T const& element)
{
	int index = m_heap.AddToTail();
	m_heap[index] = element;
}

template <class T>
bool CUtlQueue<T>::Check(T const element)
{
	int index = m_heap.Find(element);
	return (index != -1);
}


#endif  