#ifndef UTILINTRUSIVELIST_H
#define UTILINTRUSIVELIST_H

#ifdef _WIN32
#pragma once
#endif

#include "basetypes.h"
#include "utlmemory.h"
#include "dbg.h"



namespace IntrusiveList
{
#ifdef SUPERSLOW_DEBUG_VERSION                              
	template<class T> inline void ValidateDList(T* head)
	{
		if (head)
		{
			Assert(head->m_pPrev == 0);
		}
		while (head)
		{
			if (head->m_pNext)
			{
				Assert(head->m_pNext->m_pPrev == head);
			}
			if (head->m_pPrev)
			{
				Assert(head->m_pPrev->m_pNext == head);
			}
			head = head->m_pNext;
		}
	}
#else
	template<class T> inline void ValidateDList(T* )
	{
	}
#endif



	template <class T> inline void MoveDNodeBackwards(T* which, T*& head)
	{
		if (which->m_pPrev)
		{
			T* p = which->m_pPrev;
			T* pp = p->m_pPrev;
			T* n = which->m_pNext;
			Assert(p->m_pNext == which);
			if (n)
			{
				Assert(n->m_pPrev == which);
				n->m_pPrev = p;
			}
			if (pp)
			{
				Assert(pp->m_pNext == p);
				pp->m_pNext = which;
			}
			else
			{
				head = which;                           
			}
			which->m_pNext = p;
			which->m_pPrev = pp;
			p->m_pNext = n;
			p->m_pPrev = which;
		}
		ValidateDList(head);
	}



	template<class T> inline void RemoveFromDList(T*& head, T* which)
	{
		if (which->m_pPrev)
		{
			Assert(which->m_pPrev->m_pNext == which);
			which->m_pPrev->m_pNext = which->m_pNext;
			if (which->m_pNext)
			{
				Assert(which->m_pNext->m_pPrev == which);
				which->m_pNext->m_pPrev = which->m_pPrev;
			}
		}
		else
		{
			if (head == which)
			{
				head = which->m_pNext;
				if (head)
				{
					Assert(head->m_pPrev == which);
					head->m_pPrev = 0;
				}
			}
		}
		which->m_pNext = which->m_pPrev = 0;
		ValidateDList(head);

	}

	template<class T> bool OnDList(T const* head, T const* which)
	{
		return (head == which) || (which->m_pNext != 0) || (which->m_pPrev != 0);
	}

	template<class T> void AddToDTail(T*& head, T* node)
	{
		node->m_pNext = 0;
		if (!head)
		{
			head = node;
		}
		else
		{
			T* ptr = head;
			while (ptr->m_pNext)
			{
				ptr = ptr->m_pNext;
			}
			ptr->m_pNext = node;
			node->m_pPrev = ptr;   
		}
	}

	template<class T> inline void AddToDHead(T*& head, T* which)
	{
		which->m_pNext = head;
		if (head)
		{
			head->m_pPrev = which;
		}
		which->m_pPrev = 0;
		head = which;
		ValidateDList(head);
	}

	template<class T> inline void AddToDHeadWithTailPtr(T*& head, T* which, T*& tailptr)
	{
		which->m_pNext = head;
		if (head)
		{
			head->m_pPrev = which;
		}
		else
		{
			tailptr = which;
		}
		which->m_pPrev = 0;
		head = which;
		ValidateDList(head);
	}

	template<class T> inline void AddToDTailWithTailPtr(T*& head, T* which, T*& tailptr)
	{
		if (!tailptr)
		{
			Assert(!head);
			which->m_pPrev = which->m_pNext = 0;
			tailptr = head = which;
		}
		else
		{
			which->m_pNext = 0;
			which->m_pPrev = tailptr;
			tailptr->m_pNext = which;
			tailptr = which;
		}
		ValidateDList(head);
	}

	template<class T> inline void RemoveFromDListWithTailPtr(T*& head, T* which, T*& tailptr)
	{
		if (which == tailptr)
		{
			tailptr = which->m_pPrev;
		}
		if (which->m_pPrev)
		{
			Assert(which->m_pPrev->m_pNext == which);
			which->m_pPrev->m_pNext = which->m_pNext;
			if (which->m_pNext)
			{
				Assert(which->m_pNext->m_pPrev == which);
				which->m_pNext->m_pPrev = which->m_pPrev;
			}
		}
		else
		{
			if (head == which)
			{
				head = which->m_pNext;
				if (head)
				{
					Assert(head->m_pPrev == which);
					head->m_pPrev = 0;
				}
			}
		}
		which->m_pNext = which->m_pPrev = 0;
		ValidateDList(head);

	}

	template<class T> inline void DeleteFromDListWithTailPtr(T*& head, T* which, T*& tailptr)
	{
		T* tmp = which;
		if (which == tailptr)
		{
			tailptr = which->m_pPrev;
		}
		if (which->m_pPrev)
		{
			Assert(which->m_pPrev->m_pNext == which);
			which->m_pPrev->m_pNext = which->m_pNext;
			if (which->m_pNext)
			{
				Assert(which->m_pNext->m_pPrev == which);
				which->m_pNext->m_pPrev = which->m_pPrev;
			}
		}
		else
		{
			if (head == which)
			{
				head = which->m_pNext;
				if (head)
				{
					Assert(head->m_pPrev == which);
					head->m_pPrev = 0;
				}
			}
		}
		which->m_pNext = which->m_pPrev = 0;
		delete tmp;
		ValidateDList(head);
	}

	template<class T> inline void AddToDPriority(T*& head, T* which)
	{
		T* prevnode = 0;
		for (T* curnode = head; curnode; curnode = curnode->m_pNext)
		{
			if (which->m_Priority >= curnode->m_Priority)
				break;
			prevnode = curnode;
		}
		if (!prevnode)
		{
			AddToDHead(head, which);
		}
		else
		{
			which->m_pNext = prevnode->m_pNext;
			prevnode->m_pNext = which;
			which->m_pPrev = prevnode;
			if (which->m_pNext)
				which->m_pNext->m_pPrev = which;
		}
	}

	template<class T> inline void AddToDPriorityLowestFirst(T*& head, T* which)
	{
		T* prevnode = 0;
		for (T* curnode = head; curnode; curnode = curnode->m_pNext)
		{
			if (which->m_Priority <= curnode->m_Priority)
				break;
			prevnode = curnode;
		}
		if (!prevnode)
		{
			AddToDHead(head, which);
		}
		else
		{
			which->m_pNext = prevnode->m_pNext;
			prevnode->m_pNext = which;
			which->m_pPrev = prevnode;
			if (which->m_pNext)
				which->m_pNext->m_pPrev = which;
		}
	}


	template<class T> T* LastNode(T* head)
	{
		if (head)
		{
			while (head->m_pNext)
			{
				head = head->m_pNext;
			}
		}
		return head;
	}


	template<class T, class V> void RemoveFromList(T*& head, V* which)
	{
		if (head == which)
		{
			head = which->m_pNext;
		}
		else
		{
			for (T* i = head; i; i = i->m_pNext)
			{
				if (i->m_pNext == which)
				{
					i->m_pNext = which->m_pNext;
					return;
				}
			}
		}
	}

	template<class T, class V> void DeleteFromList(T*& head, V* which)
	{
		T* tmp;
		if (head == which)
		{
			tmp = which->m_pNext;
			delete(head);
			head = tmp;
		}
		else
		{
			for (T* i = head; i; i = i->m_pNext)
			{
				if (i->m_pNext == which)
				{
					tmp = which->m_pNext;
					delete(which);
					i->m_pNext = tmp;
					return;
				}
			}
		}
	}

	template<class T, class V> int PositionInList(T* head, V* node)
	{
		int pos = 0;
		while (head)
		{
			if (head == node) return pos;
			head = head->m_pNext;
			pos++;
		}
		return -1;
	}

	template<class T> T* NthNode(T* head, int idx)
	{
		while (idx && head)
		{
			idx--;
			head = head->m_pNext;
		}
		return head;
	}

	template<class T, class V> static inline void AddToHead(T*& head, V* node)
	{
		node->m_pNext = head;
		head = node;
	}

	template<class T, class V> static inline void AddToTail(T*& head, V* node)
	{
		node->m_pNext = NULL;
		if (!head)
			head = node;
		else
		{
			T* pLastNode = head;
			while (pLastNode->m_pNext)
				pLastNode = pLastNode->m_pNext;
			pLastNode->m_pNext = node;
		}
	}

	template<class T, class V> static inline void AddToHead(T*& head, T*& tail, V* node)
	{
		if (!head)
		{
			tail = node;
		}
		node->m_pNext = head;
		head = node;
	}



	template<class T> static inline T* PrevNode(T* head, T* node)
	{
		T* i;
		for (i = head; i; i = i->m_pNext)
		{
			if (i->m_pNext == node)
				break;
		}
		return i;
	}


	template<class T, class V> void AddToEnd(T*& head, V* node)
	{
		node->m_pNext = 0;
		if (!head)
		{
			head = node;
		}
		else
		{
			T* ptr = head;
			while (ptr->m_pNext)
			{
				ptr = ptr->m_pNext;
			}
			ptr->m_pNext = node;
		}
	}

	template<class T, class V> void AddToEndWithTail(T*& head, T*& tail, V* node)
	{
		Assert((head && tail) || ((!head) && (!tail)));
		node->m_pNext = 0;
		if (!head)
		{
			head = tail = node;
		}
		else
		{
			tail->m_pNext = node;
			tail = node;
		}
	}

	template<class T> void AddSortedByName(T*& head, T* node)
	{
		if ((!head) ||                                            
			(stricmp(node->m_Name, head->m_Name) == -1))                     
		{
			node->m_pNext = head;                                           
			head = node;
		}
		else
		{
			T* t;
			for (t = head; t->m_pNext; t = t->m_pNext)                               
				if (stricmp(t->m_pNext->m_Name, node->m_Name) >= 0)
					break;
			node->m_pNext = t->m_pNext;
			t->m_pNext = node;
		}
	}

	template<class T> int ListLength(T* head)
	{
		int len = 0;
		while (head)
		{
			len++;
			head = head->m_pNext;
		}
		return len;
	}

	template<class T> void KillList(T*& head)
	{
		while (head)
		{
			delete head;
		}
	}


	template<class T> void DeleteList(T*& head)
	{
		while (head)
		{
			T* tmp = head->m_pNext;
			delete head;
			head = tmp;
		}
	}

	template <class T> static inline T* FindNamedNode(T* head, char const* name)
	{
		for (; head && stricmp(head->m_Name, name); head = head->m_pNext)
		{
		}
		return head;
	}

	template <class T> static inline T* FindNamedNodeCaseSensitive(T* head, char const* name)
	{
		for (; head && strcmp(head->m_Name, name); head = head->m_pNext)
		{
		}
		return head;
	}

	template <class T, class U, class V> static inline T* FindNodeByField(T* head, U data, U V::* field)
	{
		while (head)
		{
			if (data == (*head).*field)
				return head;
			head = head->m_pNext;
		}
		return 0;
	}

	template <class T, class U, class V> static inline T* FindNodeByFieldWithPrev(T* head, U data, U V::* field, T*& prev)
	{
		prev = 0;
		for (T* i = head; i; i = i->m_pNext)
		{
			if (data == (*i).*field)
				return i;
			prev = i;
		}
		prev = 0;
		return 0;
	}


	template<class T> void SortList(T*& head, int (*comparefn)(T* a, T* b))
	{
		int didswap = 1;
		while (didswap)
		{
			didswap = 0;
			T* prev = 0;
			for (T* i = head; i && i->m_pNext; i = i->m_pNext)
			{
				int rslt = (*comparefn)(i, i->m_pNext);
				if (rslt == -1)
				{
					didswap = 1;
					T* newfirst = i->m_pNext;
					if (prev)
					{
						prev->m_pNext = newfirst;
						i->m_pNext = newfirst->m_pNext;
						newfirst->m_pNext = i;
					}
					else
					{
						head = i->m_pNext;
						i->m_pNext = newfirst->m_pNext;
						newfirst->m_pNext = i;
					}
					i = newfirst;
				}
				prev = i;
			}
		}
	}

	template <class T> void SortDList(T*& head, int (*comparefn)(T* a, T* b))
	{
		SortList(head, comparefn);
		T* prev = 0;
		for (T* i = head; i; i = i->m_pNext)
		{
			i->m_pPrev = prev;
			prev = i;
		}
	}

	template <class T> T* ReversedList(T* head)
	{
		T* pNewHead = NULL;
		while (head)
		{
			T* pNext = head->m_pNext;
#ifdef INTERVIEW_QUESTION
			head->m_pNext = pNewHead;
			pNewHead = head;
#else
			AddToHead(pNewHead, head);
#endif
			head = pNext;
		}
		return pNewHead;
	}
};

template<class T> class CUtlIntrusiveList
{
public:
	T* m_pHead;

	FORCEINLINE T* Head(void) const
	{
		return m_pHead;
	}

	FORCEINLINE CUtlIntrusiveList(void)
	{
		m_pHead = NULL;
	}


	FORCEINLINE void RemoveAll(void)
	{
		m_pHead = NULL;
	}
	FORCEINLINE void AddToHead(T* node)
	{
		IntrusiveList::AddToHead(m_pHead, node);
	}

	FORCEINLINE void AddToTail(T* node)
	{
		IntrusiveList::AddToTail(m_pHead, node);
	}

	void RemoveNode(T* which)
	{
		IntrusiveList::RemoveFromList(m_pHead, which);
	}

	void KillList(void)
	{
		while (m_pHead)
		{
			delete m_pHead;
		}
	}


	T* PrevNode(T* node)
	{
		return IntrusiveList::PrevNode(m_pHead, node);
	}

	int NthNode(int n)
	{
		return NthNode(m_pHead, n);
	}

	void Purge(void)
	{
		while (m_pHead)
		{
			T* tmp = m_pHead->m_pNext;
			delete m_pHead;
			m_pHead = tmp;
		}
	}

	int Count(void) const
	{
		return IntrusiveList::ListLength(m_pHead);
	}

	FORCEINLINE T* FindNamedNodeCaseSensitive(char const* pName) const
	{
		return IntrusiveList::FindNamedNodeCaseSensitive(m_pHead, pName);

	}

	T* RemoveHead(void)
	{
		if (m_pHead)
		{
			T* pRet = m_pHead;
			m_pHead = pRet->m_pNext;
			return pRet;
		}
		else
			return NULL;
	}
};

template<class T> class CUtlIntrusiveDList : public CUtlIntrusiveList<T>
{
public:

	FORCEINLINE void AddToHead(T* node)
	{
		IntrusiveList::AddToDHead(CUtlIntrusiveList<T>::m_pHead, node);
	}
	FORCEINLINE void AddToTail(T* node)
	{
		IntrusiveList::AddToDTail(CUtlIntrusiveList<T>::m_pHead, node);
	}

	void RemoveNode(T* which)
	{
		IntrusiveList::RemoveFromDList(CUtlIntrusiveList<T>::m_pHead, which);
	}

	T* RemoveHead(void)
	{
		if (CUtlIntrusiveList<T>::m_pHead)
		{
			T* pRet = CUtlIntrusiveList<T>::m_pHead;
			CUtlIntrusiveList<T>::m_pHead = CUtlIntrusiveList<T>::m_pHead->m_pNext;
			if (CUtlIntrusiveList<T>::m_pHead)
				CUtlIntrusiveList<T>::m_pHead->m_pPrev = NULL;
			return pRet;
		}
		else
			return NULL;
	}

	T* PrevNode(T* node)
	{
		return (node) ? node->m_Prev : NULL;
	}

};

template<class T> class CUtlIntrusiveDListWithTailPtr : public CUtlIntrusiveDList<T>
{
public:

	T* m_pTailPtr;

	FORCEINLINE CUtlIntrusiveDListWithTailPtr(void) : CUtlIntrusiveDList<T>()
	{
		m_pTailPtr = NULL;
	}

	FORCEINLINE void AddToHead(T* node)
	{
		IntrusiveList::AddToDHeadWithTailPtr(CUtlIntrusiveList<T>::m_pHead, node, m_pTailPtr);
	}
	FORCEINLINE void AddToTail(T* node)
	{
		IntrusiveList::AddToDTailWithTailPtr(CUtlIntrusiveList<T>::m_pHead, node, m_pTailPtr);
	}

	void RemoveNode(T* pWhich)
	{
		IntrusiveList::RemoveFromDListWithTailPtr(CUtlIntrusiveList<T>::m_pHead, pWhich, m_pTailPtr);
	}

	void Purge(void)
	{
		CUtlIntrusiveList<T>::Purge();
		m_pTailPtr = NULL;
	}

	void Kill(void)
	{
		CUtlIntrusiveList<T>::Purge();
		m_pTailPtr = NULL;
	}

	T* RemoveHead(void)
	{
		if (CUtlIntrusiveDList<T>::m_pHead)
		{
			T* pRet = CUtlIntrusiveDList<T>::m_pHead;
			CUtlIntrusiveDList<T>::m_pHead = CUtlIntrusiveDList<T>::m_pHead->m_pNext;
			if (CUtlIntrusiveDList<T>::m_pHead)
				CUtlIntrusiveDList<T>::m_pHead->m_pPrev = NULL;
			if (!CUtlIntrusiveDList<T>::m_pHead)
				m_pTailPtr = NULL;
			ValidateDList(CUtlIntrusiveDList<T>::m_pHead);
			return pRet;
		}
		else
			return NULL;
	}

	T* PrevNode(T* node)
	{
		return (node) ? node->m_Prev : NULL;
	}

};

template<class T> void PrependDListWithTailToDList(CUtlIntrusiveDListWithTailPtr<T>& src,
	CUtlIntrusiveDList<T>& dest)
{
	if (src.m_pHead)
	{
		src.m_pTailPtr->m_pNext = dest.m_pHead;
		if (dest.m_pHead)
			dest.m_pHead->m_pPrev = src.m_pTailPtr;
		dest.m_pHead = src.m_pHead;
		IntrusiveList::ValidateDList(dest.m_pHead);
	}
}

#endif