#ifndef UTLPRIORITYQUEUE_H
#define UTLPRIORITYQUEUE_H
#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"

template< class T >
class CUtlPriorityQueue
{
public:
	typedef bool (*LessFunc_t)(T const&, T const&);

	typedef T ElemType_t;

	CUtlPriorityQueue(int growSize = 0, int initSize = 0, LessFunc_t lessfunc = 0);
	CUtlPriorityQueue(T* pMemory, int numElements, LessFunc_t lessfunc = 0);

	inline T const& ElementAtHead() const { return m_heap.Element(0); }

	inline bool IsValidIndex(int index) { return m_heap.IsValidIndex(index); }

	void		RemoveAtHead();
	void		RemoveAt(int index);

	void		Insert(T const& element);
	void		SetLessFunc(LessFunc_t func);

	inline int	Count() const { return m_heap.Count(); }

	void		RemoveAll() { m_heap.RemoveAll(); }

	void		Purge() { m_heap.Purge(); }

	inline const T& Element(int index) const { return m_heap.Element(index); }

protected:
	CUtlVector<T>	m_heap;

	void		Swap(int index1, int index2);

	LessFunc_t m_LessFunc;
};

template< class T >
inline CUtlPriorityQueue<T>::CUtlPriorityQueue(int growSize, int initSize, LessFunc_t lessfunc) :
	m_heap(growSize, initSize), m_LessFunc(lessfunc)
{
}

template< class T >
inline CUtlPriorityQueue<T>::CUtlPriorityQueue(T* pMemory, int numElements, LessFunc_t lessfunc) :
	m_heap(pMemory, numElements), m_LessFunc(lessfunc)
{
}

template <class T>
void CUtlPriorityQueue<T>::RemoveAtHead()
{
	m_heap.FastRemove(0);
	int index = 0;

	int count = Count();
	if (!count)
		return;

	int half = count / 2;
	int larger = index;
	while (index < half)
	{
		int child = ((index + 1) * 2) - 1;	              
		if (child < count)
		{
			if (m_LessFunc(m_heap[index], m_heap[child]))
			{
				larger = child;
			}
		}
		child++;
		if (child < count)
		{
			if (m_LessFunc(m_heap[larger], m_heap[child]))
				larger = child;
		}

		if (larger == index)
			break;

		Swap(index, larger);
		index = larger;
	}
}


template <class T>
void CUtlPriorityQueue<T>::RemoveAt(int index)
{
	Assert(m_heap.IsValidIndex(index));
	m_heap.FastRemove(index);

	int count = Count();
	if (!count)
		return;

	int half = count / 2;
	int larger = index;
	while (index < half)
	{
		int child = ((index + 1) * 2) - 1;	              
		if (child < count)
		{
			if (m_LessFunc(m_heap[index], m_heap[child]))
			{
				larger = child;
			}
		}
		child++;
		if (child < count)
		{
			if (m_LessFunc(m_heap[larger], m_heap[child]))
				larger = child;
		}

		if (larger == index)
			break;

		Swap(index, larger);
		index = larger;
	}
}

template <class T>
void CUtlPriorityQueue<T>::Insert(T const& element)
{
	int index = m_heap.AddToTail();
	m_heap[index] = element;

	while (index != 0)
	{
		int parent = ((index + 1) / 2) - 1;
		if (m_LessFunc(m_heap[index], m_heap[parent]))
			break;

		Swap(parent, index);
		index = parent;
	}
}

template <class T>
void CUtlPriorityQueue<T>::Swap(int index1, int index2)
{
	T tmp = m_heap[index1];
	m_heap[index1] = m_heap[index2];
	m_heap[index2] = tmp;
}

template <class T>
void CUtlPriorityQueue<T>::SetLessFunc(LessFunc_t lessfunc)
{
	m_LessFunc = lessfunc;
}

#endif  