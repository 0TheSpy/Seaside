#ifndef UTLMAP_H
#define UTLMAP_H

#ifdef _WIN32
#pragma once
#endif

#include "dbg.h"
#include "utlrbtree.h"

#define FOR_EACH_MAP( mapName, iteratorName ) \
	for ( int iteratorName = (mapName).FirstInorder(); (mapName).IsUtlMap && iteratorName != (mapName).InvalidIndex(); iteratorName = (mapName).NextInorder( iteratorName ) )

#define FOR_EACH_MAP_FAST( mapName, iteratorName ) \
	for ( int iteratorName = 0; (mapName).IsUtlMap && iteratorName < (mapName).MaxElement(); ++iteratorName ) if ( !(mapName).IsValidIndex( iteratorName ) ) continue; else


struct base_utlmap_t
{
public:
	enum { IsUtlMap = true };
};

template <typename K, typename T, typename I = unsigned short, typename LessFunc_t = bool (*)(const K&, const K&)>
class CUtlMap : public base_utlmap_t
{
public:
	typedef K KeyType_t;
	typedef T ElemType_t;
	typedef I IndexType_t;

	CUtlMap(int growSize = 0, int initSize = 0, const LessFunc_t& lessfunc = 0)
		: m_Tree(growSize, initSize, CKeyLess(lessfunc))
	{
	}

	CUtlMap(LessFunc_t lessfunc)
		: m_Tree(CKeyLess(lessfunc))
	{
	}

	void EnsureCapacity(int num) { m_Tree.EnsureCapacity(num); }

	ElemType_t& Element(IndexType_t i) { return m_Tree.Element(i).elem; }
	const ElemType_t& Element(IndexType_t i) const { return m_Tree.Element(i).elem; }
	ElemType_t& operator[](IndexType_t i) { return m_Tree.Element(i).elem; }
	const ElemType_t& operator[](IndexType_t i) const { return m_Tree.Element(i).elem; }
	KeyType_t& Key(IndexType_t i) { return m_Tree.Element(i).key; }
	const KeyType_t& Key(IndexType_t i) const { return m_Tree.Element(i).key; }


	unsigned int Count() const { return m_Tree.Count(); }

	IndexType_t  MaxElement() const { return m_Tree.MaxElement(); }

	bool  IsValidIndex(IndexType_t i) const { return m_Tree.IsValidIndex(i); }

	bool  IsValid() const { return m_Tree.IsValid(); }

	static IndexType_t InvalidIndex() { return CTree::InvalidIndex(); }

	void SetLessFunc(LessFunc_t func)
	{
		m_Tree.SetLessFunc(CKeyLess(func));
	}

	IndexType_t  Insert(const KeyType_t& key, const ElemType_t& insert)
	{
		Node_t node;
		node.key = key;
		node.elem = insert;
		return m_Tree.Insert(node);
	}

	IndexType_t  Insert(const KeyType_t& key)
	{
		Node_t node;
		node.key = key;
		return m_Tree.Insert(node);
	}

	IndexType_t  InsertWithDupes(const KeyType_t& key, const ElemType_t& insert)
	{
		Node_t node;
		node.key = key;
		node.elem = insert;
		return m_Tree.Insert(node);
	}

	IndexType_t  InsertWithDupes(const KeyType_t& key)
	{
		Node_t node;
		node.key = key;
		return m_Tree.Insert(node);
	}


	bool HasElement(const KeyType_t& key) const
	{
		Node_t dummyNode;
		dummyNode.key = key;
		return m_Tree.HasElement(dummyNode);
	}


	IndexType_t  Find(const KeyType_t& key) const
	{
		Node_t dummyNode;
		dummyNode.key = key;
		return m_Tree.Find(dummyNode);
	}

	IndexType_t  FindFirst(const KeyType_t& key) const
	{
		Node_t dummyNode;
		dummyNode.key = key;
		return m_Tree.FindFirst(dummyNode);
	}


	const ElemType_t& FindElement(const KeyType_t& key, const ElemType_t& defaultValue) const
	{
		IndexType_t i = Find(key);
		if (i == InvalidIndex())
			return defaultValue;
		return Element(i);
	}


	IndexType_t  FindClosest(const KeyType_t& key, CompareOperands_t eFindCriteria) const
	{
		Node_t dummyNode;
		dummyNode.key = key;
		return m_Tree.FindClosest(dummyNode, eFindCriteria);
	}

	void     RemoveAt(IndexType_t i) { m_Tree.RemoveAt(i); }
	bool     Remove(const KeyType_t& key)
	{
		Node_t dummyNode;
		dummyNode.key = key;
		return m_Tree.Remove(dummyNode);
	}

	void     RemoveAll() { m_Tree.RemoveAll(); }
	void     Purge() { m_Tree.Purge(); }

	void PurgeAndDeleteElements();

	IndexType_t  FirstInorder() const { return m_Tree.FirstInorder(); }
	IndexType_t  NextInorder(IndexType_t i) const { return m_Tree.NextInorder(i); }
	IndexType_t  PrevInorder(IndexType_t i) const { return m_Tree.PrevInorder(i); }
	IndexType_t  LastInorder() const { return m_Tree.LastInorder(); }

	IndexType_t  NextInorderSameKey(IndexType_t i) const
	{
		IndexType_t iNext = NextInorder(i);
		if (!IsValidIndex(iNext))
			return InvalidIndex();
		if (Key(iNext) != Key(i))
			return InvalidIndex();
		return iNext;
	}

	void	Reinsert(const KeyType_t& key, IndexType_t i)
	{
		m_Tree[i].key = key;
		m_Tree.Reinsert(i);
	}

	IndexType_t InsertOrReplace(const KeyType_t& key, const ElemType_t& insert)
	{
		IndexType_t i = Find(key);
		if (i != InvalidIndex())
		{
			Element(i) = insert;
			return i;
		}

		return Insert(key, insert);
	}

	void Swap(CUtlMap< K, T, I >& that)
	{
		m_Tree.Swap(that.m_Tree);
	}


	struct Node_t
	{
		Node_t()
		{
		}

		Node_t(const Node_t& from)
			: key(from.key),
			elem(from.elem)
		{
		}

		KeyType_t	key;
		ElemType_t	elem;
	};

	class CKeyLess
	{
	public:
		CKeyLess(const LessFunc_t& lessFunc) : m_LessFunc(lessFunc) {}

		bool operator!() const
		{
			return !m_LessFunc;
		}

		bool operator()(const Node_t& left, const Node_t& right) const
		{
			return m_LessFunc(left.key, right.key);
		}

		LessFunc_t m_LessFunc;
	};

	typedef CUtlRBTree<Node_t, I, CKeyLess> CTree;

	CTree* AccessTree() { return &m_Tree; }

protected:
	CTree 	   m_Tree;
};

template< typename K, typename T, typename I, typename LessFunc_t >
inline void CUtlMap<K, T, I, LessFunc_t>::PurgeAndDeleteElements()
{
	for (I i = 0; i < MaxElement(); ++i)
	{
		if (!IsValidIndex(i))
			continue;

		delete Element(i);
	}

	Purge();
}

#endif  
