#ifndef UTLLINKEDLIST_H
#define UTLLINKEDLIST_H

#ifdef _WIN32
#pragma once
#endif

#include "basetypes.h"
#include "utlmemory.h"
#include "utlfixedmemory.h"
#include "utlblockmemory.h"
#include "dbg.h"

#define FOR_EACH_LL( listName, iteratorName ) \
	for( int iteratorName=(listName).Head(); (listName).IsUtlLinkedList && iteratorName != (listName).InvalidIndex(); iteratorName = (listName).Next( iteratorName ) )

template <class T, class I>
struct UtlLinkedListElem_t
{
	T  m_Element;
	I  m_Previous;
	I  m_Next;

private:
	UtlLinkedListElem_t(const UtlLinkedListElem_t&);
};


template <class T, class S = unsigned short, bool ML = false, class I = S, class M = CUtlMemory< UtlLinkedListElem_t<T, S>, I > >
class CUtlLinkedList
{
public:
	typedef T ElemType_t;
	typedef S IndexType_t;             
	typedef I IndexLocalType_t;
	typedef M MemoryAllocator_t;
	static const bool IsUtlLinkedList = true;        

	CUtlLinkedList(int growSize = 0, int initSize = 0);
	~CUtlLinkedList();

	T& Element(I i);
	T const& Element(I i) const;
	T& operator[](I i);
	T const& operator[](I i) const;

	void EnsureCapacity(int num);

	void SetGrowSize(int growSize);

	void Purge();

	void PurgeAndDeleteElements();

	I	InsertBefore(I before);
	I	InsertAfter(I after);
	I	AddToHead();
	I	AddToTail();

	I	InsertBefore(I before, T const& src);
	I	InsertAfter(I after, T const& src);
	I	AddToHead(T const& src);
	I	AddToTail(T const& src);

	I		Find(const T& src) const;

	bool	FindAndRemove(const T& src);

	void	Remove(I elem);
	void	RemoveAll();

	I		Alloc(bool multilist = false);
	void	Free(I elem);

	void	LinkBefore(I before, I elem);
	void	LinkAfter(I after, I elem);
	void	Unlink(I elem);
	void	LinkToHead(I elem);
	void	LinkToTail(I elem);

	inline static S  InvalidIndex() { return (S)M::InvalidIndex(); }

	static bool IndexInRange(I index);

	inline static size_t ElementSize() { return sizeof(ListElem_t); }

	int	Count() const;
	I	MaxElementIndex() const;
	I	NumAllocated(void) const { return m_NumAlloced; }

	I  Head() const;
	I  Tail() const;
	I  Previous(I i) const;
	I  Next(I i) const;

	template < typename List_t >
	class _CUtlLinkedList_constiterator_t
	{
	public:
		typedef typename List_t::ElemType_t ElemType_t;
		typedef typename List_t::IndexType_t IndexType_t;

		_CUtlLinkedList_constiterator_t()
			: m_list(0)
			, m_index(List_t::InvalidIndex())
		{
		}
		_CUtlLinkedList_constiterator_t(const List_t& list, IndexType_t index)
			: m_list(&list)
			, m_index(index)
		{
		}

		_CUtlLinkedList_constiterator_t& operator++()
		{
			m_index = m_list->Next(m_index);
			return *this;
		}
		_CUtlLinkedList_constiterator_t operator++(int)
		{
			_CUtlLinkedList_constiterator_t temp = *this;
			++* this;
			return temp;
		}

		_CUtlLinkedList_constiterator_t& operator--()
		{
			Assert(m_index != m_list->Head());
			if (m_index == m_list->InvalidIndex())
			{
				m_index = m_list->Tail();
			}
			else
			{
				m_index = m_list->Previous(m_index);
			}
			return *this;
		}
		_CUtlLinkedList_constiterator_t operator--(int)
		{
			_CUtlLinkedList_constiterator_t temp = *this;
			--* this;
			return temp;
		}

		bool operator==(const _CUtlLinkedList_constiterator_t& other) const
		{
			Assert(m_list == other.m_list);
			return m_index == other.m_index;
		}

		bool operator!=(const _CUtlLinkedList_constiterator_t& other) const
		{
			Assert(m_list == other.m_list);
			return m_index != other.m_index;
		}

		const ElemType_t& operator*() const
		{
			return m_list->Element(m_index);
		}

		const ElemType_t* operator->() const
		{
			return (&**this);
		}

	protected:
		const List_t* m_list;
		IndexType_t m_index;
	};

	template < typename List_t >
	class _CUtlLinkedList_iterator_t : public _CUtlLinkedList_constiterator_t< List_t >
	{
	public:
		typedef typename List_t::ElemType_t ElemType_t;
		typedef typename List_t::IndexType_t IndexType_t;
		typedef _CUtlLinkedList_constiterator_t< List_t > Base;

		_CUtlLinkedList_iterator_t()
		{
		}
		_CUtlLinkedList_iterator_t(const List_t& list, IndexType_t index)
			: _CUtlLinkedList_constiterator_t< List_t >(list, index)
		{
		}

		_CUtlLinkedList_iterator_t& operator++()
		{
			Base::m_index = Base::m_list->Next(Base::m_index);
			return *this;
		}
		_CUtlLinkedList_iterator_t operator++(int)
		{
			_CUtlLinkedList_iterator_t temp = *this;
			++* this;
			return temp;
		}

		_CUtlLinkedList_iterator_t& operator--()
		{
			Assert(Base::m_index != Base::m_list->Head());
			if (Base::m_index == Base::m_list->InvalidIndex())
			{
				Base::m_index = Base::m_list->Tail();
			}
			else
			{
				Base::m_index = Base::m_list->Previous(Base::m_index);
			}
			return *this;
		}
		_CUtlLinkedList_iterator_t operator--(int)
		{
			_CUtlLinkedList_iterator_t temp = *this;
			--* this;
			return temp;
		}

		ElemType_t& operator*() const
		{
			List_t* pMutableList = const_cast<List_t*>(Base::m_list);
			return pMutableList->Element(Base::m_index);
		}

		ElemType_t* operator->() const
		{
			return (&**this);
		}
	};

	typedef _CUtlLinkedList_constiterator_t<CUtlLinkedList<T, S, ML, I, M> > const_iterator;
	typedef _CUtlLinkedList_iterator_t<CUtlLinkedList<T, S, ML, I, M> > iterator;
	const_iterator begin() const
	{
		return const_iterator(*this, Head());
	}
	iterator begin()
	{
		return iterator(*this, Head());
	}

	const_iterator end() const
	{
		return const_iterator(*this, InvalidIndex());
	}
	iterator end()
	{
		return iterator(*this, InvalidIndex());
	}

	bool  IsValidIndex(I i) const;
	bool  IsInList(I i) const;

protected:

	typedef UtlLinkedListElem_t<T, S>  ListElem_t;

	I		AllocInternal(bool multilist = false);
	void ConstructList();

	ListElem_t& InternalElement(I i) { return m_Memory[i]; }
	ListElem_t const& InternalElement(I i) const { return m_Memory[i]; }

	CUtlLinkedList(CUtlLinkedList<T, S, ML, I, M> const& list) { Assert(0); }

	M	m_Memory;
	I	m_Head;
	I	m_Tail;
	I	m_FirstFree;
	I	m_ElementCount;		      
	I	m_NumAlloced;		     
	typename M::Iterator_t	m_LastAlloc;     

	ListElem_t* m_pElements;

	FORCEINLINE M const& Memory(void) const
	{
		return m_Memory;
	}

	void ResetDbgInfo()
	{
		m_pElements = m_Memory.Base();
	}

private:
	I  PrivateNext(I i) const;
};


template < class T >
class CUtlFixedLinkedList : public CUtlLinkedList< T, int, true, int, CUtlFixedMemory< UtlLinkedListElem_t< T, int > > >
{
public:
	CUtlFixedLinkedList(int growSize = 0, int initSize = 0)
		: CUtlLinkedList< T, int, true, int, CUtlFixedMemory< UtlLinkedListElem_t< T, int > > >(growSize, initSize) {}

	typedef CUtlLinkedList< T, int, true, int, CUtlFixedMemory< UtlLinkedListElem_t< T, int > > > BaseClass;
	bool IsValidIndex(int i) const
	{
		if (!BaseClass::Memory().IsIdxValid(i))
			return false;

#ifdef _DEBUG                    
		if (BaseClass::Memory().IsIdxAfter(i, this->m_LastAlloc))
		{
			Assert(0);
			return false;           
		}
#endif

		return (BaseClass::Memory()[i].m_Previous != i) || (BaseClass::Memory()[i].m_Next == i);
	}

private:
	int	MaxElementIndex() const { Assert(0); return BaseClass::InvalidIndex(); }        
	void ResetDbgInfo() {}
};

template < class T, class I = unsigned short >
class CUtlBlockLinkedList : public CUtlLinkedList< T, I, true, I, CUtlBlockMemory< UtlLinkedListElem_t< T, I >, I > >
{
public:
	CUtlBlockLinkedList(int growSize = 0, int initSize = 0)
		: CUtlLinkedList< T, I, true, I, CUtlBlockMemory< UtlLinkedListElem_t< T, I >, I > >(growSize, initSize) {}
protected:
	void ResetDbgInfo() {}
};


template <class T, class S, bool ML, class I, class M>
CUtlLinkedList<T, S, ML, I, M>::CUtlLinkedList(int growSize, int initSize) :
	m_Memory(growSize, initSize), m_LastAlloc(m_Memory.InvalidIterator())
{
	COMPILE_TIME_ASSERT(sizeof(S) == 4 || (((S)-1) > 0));
	ConstructList();
	ResetDbgInfo();
}

template <class T, class S, bool ML, class I, class M>
CUtlLinkedList<T, S, ML, I, M>::~CUtlLinkedList()
{
	RemoveAll();
}

template <class T, class S, bool ML, class I, class M>
void CUtlLinkedList<T, S, ML, I, M>::ConstructList()
{
	m_Head = InvalidIndex();
	m_Tail = InvalidIndex();
	m_FirstFree = InvalidIndex();
	m_ElementCount = 0;
	m_NumAlloced = 0;
}


template <class T, class S, bool ML, class I, class M>
inline T& CUtlLinkedList<T, S, ML, I, M>::Element(I i)
{
	return m_Memory[i].m_Element;
}

template <class T, class S, bool ML, class I, class M>
inline T const& CUtlLinkedList<T, S, ML, I, M>::Element(I i) const
{
	return m_Memory[i].m_Element;
}

template <class T, class S, bool ML, class I, class M>
inline T& CUtlLinkedList<T, S, ML, I, M>::operator[](I i)
{
	return m_Memory[i].m_Element;
}

template <class T, class S, bool ML, class I, class M>
inline T const& CUtlLinkedList<T, S, ML, I, M>::operator[](I i) const
{
	return m_Memory[i].m_Element;
}

template <class T, class S, bool ML, class I, class M>
inline int CUtlLinkedList<T, S, ML, I, M>::Count() const
{
#ifdef MULTILIST_PEDANTIC_ASSERTS
	AssertMsg(!ML, "CUtlLinkedList::Count() is meaningless for linked lists.");
#endif
	return m_ElementCount;
}

template <class T, class S, bool ML, class I, class M>
inline I CUtlLinkedList<T, S, ML, I, M>::MaxElementIndex() const
{
	return m_Memory.NumAllocated();
}


template <class T, class S, bool ML, class I, class M>
inline I  CUtlLinkedList<T, S, ML, I, M>::Head() const
{
	return m_Head;
}

template <class T, class S, bool ML, class I, class M>
inline I  CUtlLinkedList<T, S, ML, I, M>::Tail() const
{
	return m_Tail;
}

template <class T, class S, bool ML, class I, class M>
inline I  CUtlLinkedList<T, S, ML, I, M>::Previous(I i) const
{
	Assert(IsValidIndex(i));
	return InternalElement(i).m_Previous;
}

template <class T, class S, bool ML, class I, class M>
inline I  CUtlLinkedList<T, S, ML, I, M>::Next(I i) const
{
	Assert(IsValidIndex(i));
	return InternalElement(i).m_Next;
}

template <class T, class S, bool ML, class I, class M>
inline I  CUtlLinkedList<T, S, ML, I, M>::PrivateNext(I i) const
{
	return InternalElement(i).m_Next;
}


#pragma warning(push)
#pragma warning( disable: 4310 )    
template <class T, class S, bool ML, class I, class M>
inline bool CUtlLinkedList<T, S, ML, I, M>::IndexInRange(I index)   
{
	COMPILE_TIME_ASSERT(sizeof(I) >= sizeof(S));
	COMPILE_TIME_ASSERT((sizeof(S) > 2) || (((S)-1) > 0));
	COMPILE_TIME_ASSERT((M::INVALID_INDEX == -1) || (M::INVALID_INDEX == (S)M::INVALID_INDEX));

	return (((S)index == index) && ((S)index != InvalidIndex()));
}
#pragma warning(pop)

template <class T, class S, bool ML, class I, class M>
inline bool CUtlLinkedList<T, S, ML, I, M>::IsValidIndex(I i) const
{
	if (!m_Memory.IsIdxValid(i))
		return false;

	if (m_Memory.IsIdxAfter(i, m_LastAlloc))
		return false;           

	return (m_Memory[i].m_Previous != i) || (m_Memory[i].m_Next == i);
}

template <class T, class S, bool ML, class I, class M>
inline bool CUtlLinkedList<T, S, ML, I, M>::IsInList(I i) const
{
	if (!m_Memory.IsIdxValid(i) || m_Memory.IsIdxAfter(i, m_LastAlloc))
		return false;           

	return Previous(i) != i;
}

template< class T, class S, bool ML, class I, class M >
void CUtlLinkedList<T, S, ML, I, M>::EnsureCapacity(int num)
{
	MEM_ALLOC_CREDIT_CLASS();
	m_Memory.EnsureCapacity(num);
	ResetDbgInfo();
}

template< class T, class S, bool ML, class I, class M >
void CUtlLinkedList<T, S, ML, I, M>::SetGrowSize(int growSize)
{
	RemoveAll();
	m_Memory.Init(growSize);
	ResetDbgInfo();
}


template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::Purge()
{
	RemoveAll();

	m_Memory.Purge();
	m_FirstFree = InvalidIndex();
	m_NumAlloced = 0;

	const typename M::Iterator_t scInvalidIterator = m_Memory.InvalidIterator();
	m_LastAlloc = scInvalidIterator;
	ResetDbgInfo();
}


template<class T, class S, bool ML, class I, class M>
void CUtlLinkedList<T, S, ML, I, M>::PurgeAndDeleteElements()
{
	I iNext;
	for (I i = Head(); i != InvalidIndex(); i = iNext)
	{
		iNext = Next(i);
		delete Element(i);
	}

	Purge();
}


template <class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::AllocInternal(bool multilist)
{
	Assert(!multilist || ML);
#ifdef MULTILIST_PEDANTIC_ASSERTS
	Assert(multilist == ML);
#endif
	I elem;
	if (m_FirstFree == InvalidIndex())
	{
		Assert(m_Memory.IsValidIterator(m_LastAlloc) || m_ElementCount == 0);

		typename M::Iterator_t it = m_Memory.IsValidIterator(m_LastAlloc) ? m_Memory.Next(m_LastAlloc) : m_Memory.First();

		if (!m_Memory.IsValidIterator(it))
		{
			MEM_ALLOC_CREDIT_CLASS();
			m_Memory.Grow();
			ResetDbgInfo();

			it = m_Memory.IsValidIterator(m_LastAlloc) ? m_Memory.Next(m_LastAlloc) : m_Memory.First();

			Assert(m_Memory.IsValidIterator(it));
			if (!m_Memory.IsValidIterator(it))
			{
				Error("CUtlLinkedList overflow! (exhausted memory allocator)\n");
				return InvalidIndex();
			}
		}

		if (!IndexInRange(m_Memory.GetIndex(it)))
		{
			Error("CUtlLinkedList overflow! (exhausted index range)\n");
			return InvalidIndex();
		}

		m_LastAlloc = it;
		elem = m_Memory.GetIndex(m_LastAlloc);
		m_NumAlloced++;
	}
	else
	{
		elem = m_FirstFree;
		m_FirstFree = InternalElement(m_FirstFree).m_Next;
	}

	if (!multilist)
	{
		InternalElement(elem).m_Next = elem;
		InternalElement(elem).m_Previous = elem;
	}
	else
	{
		InternalElement(elem).m_Next = InvalidIndex();
		InternalElement(elem).m_Previous = InvalidIndex();
	}

	return elem;
}

template <class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::Alloc(bool multilist)
{
	I elem = AllocInternal(multilist);
	if (elem == InvalidIndex())
		return elem;

	Construct(&Element(elem));

	return elem;
}

template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::Free(I elem)
{
	Assert(IsValidIndex(elem) && IndexInRange(elem));
	Unlink(elem);

	ListElem_t& internalElem = InternalElement(elem);
	Destruct(&internalElem.m_Element);
	internalElem.m_Next = m_FirstFree;
	m_FirstFree = elem;
}

template <class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::InsertBefore(I before)
{
	I   newNode = AllocInternal();
	if (newNode == InvalidIndex())
		return newNode;

	LinkBefore(before, newNode);

	Construct(&Element(newNode));

	return newNode;
}

template <class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::InsertAfter(I after)
{
	I   newNode = AllocInternal();
	if (newNode == InvalidIndex())
		return newNode;

	LinkAfter(after, newNode);

	Construct(&Element(newNode));

	return newNode;
}

template <class T, class S, bool ML, class I, class M>
inline I CUtlLinkedList<T, S, ML, I, M>::AddToHead()
{
	return InsertAfter(InvalidIndex());
}

template <class T, class S, bool ML, class I, class M>
inline I CUtlLinkedList<T, S, ML, I, M>::AddToTail()
{
	return InsertBefore(InvalidIndex());
}


template <class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::InsertBefore(I before, T const& src)
{
	I   newNode = AllocInternal();
	if (newNode == InvalidIndex())
		return newNode;

	LinkBefore(before, newNode);

	CopyConstruct(&Element(newNode), src);

	return newNode;
}

template <class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::InsertAfter(I after, T const& src)
{
	I   newNode = AllocInternal();
	if (newNode == InvalidIndex())
		return newNode;

	LinkAfter(after, newNode);

	CopyConstruct(&Element(newNode), src);

	return newNode;
}

template <class T, class S, bool ML, class I, class M>
inline I CUtlLinkedList<T, S, ML, I, M>::AddToHead(T const& src)
{
	return InsertAfter(InvalidIndex(), src);
}

template <class T, class S, bool ML, class I, class M>
inline I CUtlLinkedList<T, S, ML, I, M>::AddToTail(T const& src)
{
	return InsertBefore(InvalidIndex(), src);
}


template<class T, class S, bool ML, class I, class M>
I CUtlLinkedList<T, S, ML, I, M>::Find(const T& src) const
{
	I invalidIndex = InvalidIndex();
	for (I i = Head(); i != invalidIndex; i = PrivateNext(i))
	{
		if (Element(i) == src)
			return i;
	}
	return InvalidIndex();
}


template<class T, class S, bool ML, class I, class M>
bool CUtlLinkedList<T, S, ML, I, M>::FindAndRemove(const T& src)
{
	I i = Find(src);
	if (i == InvalidIndex())
	{
		return false;
	}
	else
	{
		Remove(i);
		return true;
	}
}


template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::Remove(I elem)
{
	Free(elem);
}

template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::RemoveAll()
{
	if (m_LastAlloc == m_Memory.InvalidIterator())
	{
		Assert(m_Head == InvalidIndex());
		Assert(m_Tail == InvalidIndex());
		Assert(m_FirstFree == InvalidIndex());
		Assert(m_ElementCount == 0);
		return;
	}

	if (ML)
	{
		for (typename M::Iterator_t it = m_Memory.First(); it != m_Memory.InvalidIterator(); it = m_Memory.Next(it))
		{
			I i = m_Memory.GetIndex(it);
			if (IsValidIndex(i))        
			{
				ListElem_t& internalElem = InternalElement(i);
				Destruct(&internalElem.m_Element);
				internalElem.m_Previous = i;
				internalElem.m_Next = m_FirstFree;
				m_FirstFree = i;
			}

			if (it == m_LastAlloc)
				break;         
		}
	}
	else
	{
		I i = Head();
		I next;
		while (i != InvalidIndex())
		{
			next = Next(i);
			ListElem_t& internalElem = InternalElement(i);
			Destruct(&internalElem.m_Element);
			internalElem.m_Previous = i;
			internalElem.m_Next = next == InvalidIndex() ? m_FirstFree : next;
			i = next;
		}
		if (Head() != InvalidIndex())
		{
			m_FirstFree = Head();
		}
	}

	m_Head = InvalidIndex();
	m_Tail = InvalidIndex();
	m_ElementCount = 0;
}


template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::LinkBefore(I before, I elem)
{
	Assert(IsValidIndex(elem));

	Unlink(elem);

	ListElem_t* RESTRICT pNewElem = &InternalElement(elem);

	pNewElem->m_Next = before;

	S newElem_mPrevious;             
	if (before == InvalidIndex())
	{
		newElem_mPrevious = m_Tail;
		pNewElem->m_Previous = m_Tail;
		m_Tail = elem;
	}
	else
	{
		Assert(IsInList(before));
		ListElem_t* RESTRICT beforeElem = &InternalElement(before);
		pNewElem->m_Previous = newElem_mPrevious = beforeElem->m_Previous;
		beforeElem->m_Previous = elem;
	}

	if (newElem_mPrevious == InvalidIndex())
		m_Head = elem;
	else
		InternalElement(newElem_mPrevious).m_Next = elem;

	++m_ElementCount;
}

template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::LinkAfter(I after, I elem)
{
	Assert(IsValidIndex(elem));

	if (IsInList(elem))
		Unlink(elem);

	ListElem_t& newElem = InternalElement(elem);

	newElem.m_Previous = after;
	if (after == InvalidIndex())
	{
		newElem.m_Next = m_Head;
		m_Head = elem;
	}
	else
	{
		Assert(IsInList(after));
		ListElem_t& afterElem = InternalElement(after);
		newElem.m_Next = afterElem.m_Next;
		afterElem.m_Next = elem;
	}

	if (newElem.m_Next == InvalidIndex())
		m_Tail = elem;
	else
		InternalElement(newElem.m_Next).m_Previous = elem;

	++m_ElementCount;
}

template <class T, class S, bool ML, class I, class M>
void  CUtlLinkedList<T, S, ML, I, M>::Unlink(I elem)
{
	Assert(IsValidIndex(elem));
	if (IsInList(elem))
	{
		ListElem_t* RESTRICT pOldElem = &m_Memory[elem];

		if (pOldElem->m_Previous != InvalidIndex())
		{
			m_Memory[pOldElem->m_Previous].m_Next = pOldElem->m_Next;
		}
		else
		{
			m_Head = pOldElem->m_Next;
		}

		if (pOldElem->m_Next != InvalidIndex())
		{
			m_Memory[pOldElem->m_Next].m_Previous = pOldElem->m_Previous;
		}
		else
		{
			m_Tail = pOldElem->m_Previous;
		}

		pOldElem->m_Previous = pOldElem->m_Next = elem;

		--m_ElementCount;
	}
}

template <class T, class S, bool ML, class I, class M>
inline void CUtlLinkedList<T, S, ML, I, M>::LinkToHead(I elem)
{
	LinkAfter(InvalidIndex(), elem);
}

template <class T, class S, bool ML, class I, class M>
inline void CUtlLinkedList<T, S, ML, I, M>::LinkToTail(I elem)
{
	LinkBefore(InvalidIndex(), elem);
}


DECLARE_POINTER_HANDLE(UtlPtrLinkedListIndex_t);     

template < typename T >
class CUtlPtrLinkedList
{
public:
	CUtlPtrLinkedList()
		: m_pFirst(NULL),
		m_nElems(0)
	{
		COMPILE_TIME_ASSERT(sizeof(IndexType_t) == sizeof(Node_t*));
	}

	~CUtlPtrLinkedList()
	{
		RemoveAll();
	}

	typedef UtlPtrLinkedListIndex_t IndexType_t;

	T& operator[](IndexType_t i)
	{
		return ((Node_t*)i)->elem;
	}

	const T& operator[](IndexType_t i) const
	{
		return ((Node_t*)i)->elem;
	}

	IndexType_t	AddToTail()
	{
		return DoInsertBefore((IndexType_t)m_pFirst, NULL);
	}

	IndexType_t	AddToTail(T const& src)
	{
		return DoInsertBefore((IndexType_t)m_pFirst, &src);
	}

	IndexType_t	AddToHead()
	{
		IndexType_t result = DoInsertBefore((IndexType_t)m_pFirst, NULL);
		m_pFirst = ((Node_t*)result);
		return result;
	}

	IndexType_t	AddToHead(T const& src)
	{
		IndexType_t result = DoInsertBefore((IndexType_t)m_pFirst, &src);
		m_pFirst = ((Node_t*)result);
		return result;
	}

	IndexType_t InsertBefore(IndexType_t before)
	{
		return DoInsertBefore(before, NULL);
	}

	IndexType_t InsertAfter(IndexType_t after)
	{
		Node_t* pBefore = ((Node_t*)after)->next;
		return DoInsertBefore(pBefore, NULL);
	}

	IndexType_t InsertBefore(IndexType_t before, T const& src)
	{
		return DoInsertBefore(before, &src);
	}

	IndexType_t InsertAfter(IndexType_t after, T const& src)
	{
		Node_t* pBefore = ((Node_t*)after)->next;
		return DoInsertBefore(pBefore, &src);
	}

	void Remove(IndexType_t elem)
	{
		Node_t* p = (Node_t*)elem;

		if (p->pNext == p)
		{
			m_pFirst = NULL;
		}
		else
		{
			if (m_pFirst == p)
			{
				m_pFirst = p->pNext;
			}
			p->pNext->pPrev = p->pPrev;
			p->pPrev->pNext = p->pNext;
		}

		delete p;
		m_nElems--;
	}

	void RemoveAll()
	{
		Node_t* p = m_pFirst;
		if (p)
		{
			do
			{
				Node_t* pNext = p->pNext;
				delete p;
				p = pNext;
			} while (p != m_pFirst);
		}

		m_pFirst = NULL;
		m_nElems = 0;
	}

	int	Count() const
	{
		return m_nElems;
	}

	IndexType_t Head() const
	{
		return (IndexType_t)m_pFirst;
	}

	IndexType_t Next(IndexType_t i) const
	{
		Node_t* p = ((Node_t*)i)->pNext;
		if (p != m_pFirst)
		{
			return (IndexType_t)p;
		}
		return NULL;
	}

	bool IsValidIndex(IndexType_t i) const
	{
		Node_t* p = ((Node_t*)i);
		return (p && p->pNext && p->pPrev);
	}

	inline static IndexType_t  InvalidIndex()
	{
		return NULL;
	}
private:

	struct Node_t
	{
		Node_t() {}
		Node_t(const T& _elem) : elem(_elem) {}

		T elem;
		Node_t* pPrev, * pNext;
	};

	Node_t* AllocNode(const T* pCopyFrom)
	{
		MEM_ALLOC_CREDIT_CLASS();
		Node_t* p;

		if (!pCopyFrom)
		{
			p = new Node_t;
		}
		else
		{
			p = new Node_t(*pCopyFrom);
		}

		return p;
	}

	IndexType_t DoInsertBefore(IndexType_t before, const T* pCopyFrom)
	{
		Node_t* p = AllocNode(pCopyFrom);
		Node_t* pBefore = (Node_t*)before;
		if (pBefore)
		{
			p->pNext = pBefore;
			p->pPrev = pBefore->pPrev;
			pBefore->pPrev = p;
			p->pPrev->pNext = p;
		}
		else
		{
			Assert(!m_pFirst);
			m_pFirst = p->pNext = p->pPrev = p;
		}

		m_nElems++;
		return (IndexType_t)p;
	}

	Node_t* m_pFirst;
	unsigned m_nElems;
};

#endif  