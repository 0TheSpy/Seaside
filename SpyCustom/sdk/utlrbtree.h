#ifndef UTLRBTREE_H
#define UTLRBTREE_H

#include "utlmemory.h"
#include "utlfixedmemory.h"
#include "utlblockmemory.h"


#define FOR_EACH_UTLRBTREE( treeName, iteratorName ) \
	for ( int iteratorName = treeName.FirstInorder(); (treeName).IsUtlRBTree && iteratorName != treeName.InvalidIndex(); iteratorName = treeName.NextInorder( iteratorName ) )


template <typename T >
class CDefOps
{
public:
	static bool LessFunc(const T& lhs, const T& rhs) { return (lhs < rhs); }
};

#define DefLessFunc( type ) CDefOps< type >::LessFunc

template <typename T>
class CDefLess
{
public:
	CDefLess() {}
	CDefLess(int i) {}
	inline bool operator()(const T& lhs, const T& rhs) const { return (lhs < rhs); }
	inline bool operator!() const { return false; }
};


inline bool StringLessThan(const char* const& lhs, const char* const& rhs) {
	if (!lhs) return false;
	if (!rhs) return true;
	return (strcmp(lhs, rhs) < 0);
}

inline bool CaselessStringLessThan(const char* const& lhs, const char* const& rhs) {
	if (!lhs) return false;
	if (!rhs) return true;
	return (stricmp(lhs, rhs) < 0);
}


inline bool CaselessStringLessThanIgnoreSlashes(const char* const& lhs, const char* const& rhs)
{
	const char* pa = lhs;
	const char* pb = rhs;
	while (*pa && *pb)
	{
		char a = *pa;
		char b = *pb;

		if (a == '/' || a == '\\')
		{
			if (b != '/' && b != '\\')
				return ('/' < b);
		}
		else
		{
			if (a >= 'a' && a <= 'z')
				a = 'A' + (a - 'a');

			if (b >= 'a' && b <= 'z')
				b = 'A' + (b - 'a');

			if (a > b)
				return false;
			else if (a < b)
				return true;
		}
		++pa;
		++pb;
	}

	if (*pa != *pb)
	{
		return (!*pa);
	}

	return false;
}

template <> inline bool CDefOps<const char*>::LessFunc(const char* const& lhs, const char* const& rhs) { return StringLessThan(lhs, rhs); }
template <> inline bool CDefOps<char*>::LessFunc(char* const& lhs, char* const& rhs) { return StringLessThan(lhs, rhs); }

template <typename RBTREE_T>
void SetDefLessFunc(RBTREE_T& RBTree)
{
	RBTree.SetLessFunc(DefLessFunc(typename RBTREE_T::KeyType_t));
}

enum CompareOperands_t
{
	k_EEqual = 0x1,
	k_EGreaterThan = 0x2,
	k_ELessThan = 0x4,
	k_EGreaterThanOrEqualTo = k_EGreaterThan | k_EEqual,
	k_ELessThanOrEqualTo = k_ELessThan | k_EEqual,
};

template < class I >
struct UtlRBTreeLinks_t
{
	I  m_Left;
	I  m_Right;
	I  m_Parent;
	I  m_Tag;
};

template < class T, class I >
struct UtlRBTreeNode_t : public UtlRBTreeLinks_t< I >
{
	T  m_Data;
};

template < class T, class I = unsigned short, typename L = bool (*)(const T&, const T&), class M = CUtlMemory< UtlRBTreeNode_t< T, I >, I > >
class CUtlRBTree
{
public:

	typedef T KeyType_t;
	typedef T ElemType_t;
	typedef I IndexType_t;
	enum { IsUtlRBTree = true };        

	typedef L LessFunc_t;

	explicit CUtlRBTree(int growSize = 0, int initSize = 0, const LessFunc_t& lessfunc = 0);
	explicit CUtlRBTree(const LessFunc_t& lessfunc);
	~CUtlRBTree();

	void EnsureCapacity(int num);

	void CopyFrom(const CUtlRBTree<T, I, L, M>& other);

	T& Element(I i);
	T const& Element(I i) const;
	T& operator[](I i);
	T const& operator[](I i) const;

	I  Root() const;

	unsigned int Count() const;

	I  MaxElement() const;

	I  Parent(I i) const;
	I  LeftChild(I i) const;
	I  RightChild(I i) const;

	bool  IsLeftChild(I i) const;
	bool  IsRightChild(I i) const;

	bool  IsRoot(I i) const;
	bool  IsLeaf(I i) const;

	bool  IsValidIndex(I i) const;

	bool  IsValid() const;

	static I InvalidIndex();

	int   Depth(I node) const;
	int   Depth() const;

	void SetLessFunc(const LessFunc_t& func);

	I  NewNode();

	I  Insert(T const& insert);
	void Insert(const T* pArray, int nItems);
	I  InsertIfNotFound(T const& insert);

	I  Find(T const& search) const;

	I  FindFirst(T const& search) const;

	I  FindClosest(T const& search, CompareOperands_t eFindCriteria) const;

	void     RemoveAt(I i);
	bool     Remove(T const& remove);
	void     RemoveAll();
	void	 Purge();

	void  FreeNode(I i);

	I  FirstInorder() const;
	I  NextInorder(I i) const;
	I  PrevInorder(I i) const;
	I  LastInorder() const;

	I  FirstPreorder() const;
	I  NextPreorder(I i) const;
	I  PrevPreorder(I i) const;
	I  LastPreorder() const;

	I  FirstPostorder() const;
	I  NextPostorder(I i) const;

	void	Reinsert(I elem);

	void Swap(CUtlRBTree< T, I, L >& that);

private:
	CUtlRBTree<T, I, L, M>& operator=(const CUtlRBTree<T, I, L, M>& other);

protected:
	enum NodeColor_t
	{
		RED = 0,
		BLACK
	};

	typedef UtlRBTreeNode_t< T, I > Node_t;
	typedef UtlRBTreeLinks_t< I > Links_t;

	void  SetParent(I i, I parent);
	void  SetLeftChild(I i, I child);
	void  SetRightChild(I i, I child);
	void  LinkToParent(I i, I parent, bool isLeft);

	Links_t const& Links(I i) const;
	Links_t& Links(I i);

	bool IsRed(I i) const;
	bool IsBlack(I i) const;

	NodeColor_t Color(I i) const;
	void        SetColor(I i, NodeColor_t c);

	void RotateLeft(I i);
	void RotateRight(I i);
	void InsertRebalance(I i);
	void RemoveRebalance(I i);

	I  InsertAt(I parent, bool leftchild);

	CUtlRBTree(CUtlRBTree<T, I, L, M> const& tree);

	void FindInsertionPosition(T const& insert, I& parent, bool& leftchild);

	void	Unlink(I elem);
	void	Link(I elem);

	LessFunc_t m_LessFunc;

	M m_Elements;
	I m_Root;
	I m_NumElements;
	I m_FirstFree;
	typename M::Iterator_t m_LastAlloc;     

	Node_t* m_pElements;

	FORCEINLINE M const& Elements(void) const
	{
		return m_Elements;
	}


	void ResetDbgInfo()
	{
		m_pElements = (Node_t*)m_Elements.Base();
	}
};

template < class T, class I = int, typename L = bool (*)(const T&, const T&)  >
class CUtlFixedRBTree : public CUtlRBTree< T, I, L, CUtlFixedMemory< UtlRBTreeNode_t< T, I > > >
{
public:

	typedef L LessFunc_t;

	CUtlFixedRBTree(int growSize = 0, int initSize = 0, const LessFunc_t& lessfunc = 0)
		: CUtlRBTree< T, I, L, CUtlFixedMemory< UtlRBTreeNode_t< T, I > > >(growSize, initSize, lessfunc) {}
	CUtlFixedRBTree(const LessFunc_t& lessfunc)
		: CUtlRBTree< T, I, L, CUtlFixedMemory< UtlRBTreeNode_t< T, I > > >(lessfunc) {}

	typedef CUtlRBTree< T, I, L, CUtlFixedMemory< UtlRBTreeNode_t< T, I > > > BaseClass;
	bool IsValidIndex(I i) const
	{
		if (!BaseClass::Elements().IsIdxValid(i))
			return false;

#ifdef _DEBUG                    
		if (BaseClass::Elements().IsIdxAfter(i, this->m_LastAlloc))
		{
			Assert(0);
			return false;           
		}
#endif

		return LeftChild(i) != i;
	}

protected:
	void ResetDbgInfo() {}

private:
	I  MaxElement() const;
};

template < class T, class I = unsigned short, typename L = bool (*)(const T&, const T&)  >
class CUtlBlockRBTree : public CUtlRBTree< T, I, L, CUtlBlockMemory< UtlRBTreeNode_t< T, I >, I > >
{
public:
	typedef L LessFunc_t;
	CUtlBlockRBTree(int growSize = 0, int initSize = 0, const LessFunc_t& lessfunc = 0)
		: CUtlRBTree< T, I, L, CUtlBlockMemory< UtlRBTreeNode_t< T, I >, I > >(growSize, initSize, lessfunc) {}
	CUtlBlockRBTree(const LessFunc_t& lessfunc)
		: CUtlRBTree< T, I, L, CUtlBlockMemory< UtlRBTreeNode_t< T, I >, I > >(lessfunc) {}
protected:
	void ResetDbgInfo() {}
};


template < class T, class I, typename L, class M >
inline CUtlRBTree<T, I, L, M>::CUtlRBTree(int growSize, int initSize, const LessFunc_t& lessfunc) :
	m_LessFunc(lessfunc),
	m_Elements(growSize, initSize),
	m_Root(InvalidIndex()),
	m_NumElements(0),
	m_FirstFree(InvalidIndex()),
	m_LastAlloc(m_Elements.InvalidIterator())
{
	ResetDbgInfo();
}

template < class T, class I, typename L, class M >
inline CUtlRBTree<T, I, L, M>::CUtlRBTree(const LessFunc_t& lessfunc) :
	m_Elements(0, 0),
	m_LessFunc(lessfunc),
	m_Root(InvalidIndex()),
	m_NumElements(0),
	m_FirstFree(InvalidIndex()),
	m_LastAlloc(m_Elements.InvalidIterator())
{
	ResetDbgInfo();
}

template < class T, class I, typename L, class M >
inline CUtlRBTree<T, I, L, M>::~CUtlRBTree()
{
	Purge();
}

template < class T, class I, typename L, class M >
inline void CUtlRBTree<T, I, L, M>::EnsureCapacity(int num)
{
	m_Elements.EnsureCapacity(num);
}

template < class T, class I, typename L, class M >
inline void CUtlRBTree<T, I, L, M>::CopyFrom(const CUtlRBTree<T, I, L, M>& other)
{
	Purge();
	m_Elements.EnsureCapacity(other.m_Elements.Count());
	memcpy(m_Elements.Base(), other.m_Elements.Base(), other.m_Elements.Count() * sizeof(UtlRBTreeNode_t< T, I >));
	m_LessFunc = other.m_LessFunc;
	m_Root = other.m_Root;
	m_NumElements = other.m_NumElements;
	m_FirstFree = other.m_FirstFree;
	m_LastAlloc = other.m_LastAlloc;
	ResetDbgInfo();
}

template < class T, class I, typename L, class M >
inline T& CUtlRBTree<T, I, L, M>::Element(I i)
{
	Assert(IsValidIndex(i));
	return m_Elements[i].m_Data;
}

template < class T, class I, typename L, class M >
inline T const& CUtlRBTree<T, I, L, M>::Element(I i) const
{
	Assert(IsValidIndex(i));
	return m_Elements[i].m_Data;
}

template < class T, class I, typename L, class M >
inline T& CUtlRBTree<T, I, L, M>::operator[](I i)
{
	return Element(i);
}

template < class T, class I, typename L, class M >
inline T const& CUtlRBTree<T, I, L, M>::operator[](I i) const
{
	return Element(i);
}

template < class T, class I, typename L, class M >
inline	I  CUtlRBTree<T, I, L, M>::Root() const
{
	return m_Root;
}

template < class T, class I, typename L, class M >
inline	unsigned int CUtlRBTree<T, I, L, M>::Count() const
{
	return (unsigned int)m_NumElements;
}

template < class T, class I, typename L, class M >
inline	I  CUtlRBTree<T, I, L, M>::MaxElement() const
{
	return (I)m_Elements.NumAllocated();
}


template < class T, class I, typename L, class M >
inline	I CUtlRBTree<T, I, L, M>::Parent(I i) const
{
	return i != InvalidIndex() ? m_Elements[i].m_Parent : InvalidIndex();
}

template < class T, class I, typename L, class M >
inline	I CUtlRBTree<T, I, L, M>::LeftChild(I i) const
{
	return i != InvalidIndex() ? m_Elements[i].m_Left : InvalidIndex();
}

template < class T, class I, typename L, class M >
inline	I CUtlRBTree<T, I, L, M>::RightChild(I i) const
{
	return i != InvalidIndex() ? m_Elements[i].m_Right : InvalidIndex();
}

template < class T, class I, typename L, class M >
inline	bool CUtlRBTree<T, I, L, M>::IsLeftChild(I i) const
{
	return LeftChild(Parent(i)) == i;
}

template < class T, class I, typename L, class M >
inline	bool CUtlRBTree<T, I, L, M>::IsRightChild(I i) const
{
	return RightChild(Parent(i)) == i;
}


template < class T, class I, typename L, class M >
inline	bool CUtlRBTree<T, I, L, M>::IsRoot(I i) const
{
	return i == m_Root;
}

template < class T, class I, typename L, class M >
inline	bool CUtlRBTree<T, I, L, M>::IsLeaf(I i) const
{
	return (LeftChild(i) == InvalidIndex()) && (RightChild(i) == InvalidIndex());
}


template < class T, class I, typename L, class M >
inline	bool CUtlRBTree<T, I, L, M>::IsValidIndex(I i) const
{
	if (!m_Elements.IsIdxValid(i))
		return false;

	if (m_Elements.IsIdxAfter(i, m_LastAlloc))
		return false;           

	return LeftChild(i) != i;
}


template < class T, class I, typename L, class M >
inline I CUtlRBTree<T, I, L, M>::InvalidIndex()
{
	return (I)M::InvalidIndex();
}


template < class T, class I, typename L, class M >
inline int CUtlRBTree<T, I, L, M>::Depth() const
{
	return Depth(Root());
}

template < class T, class I, typename L, class M >
inline void  CUtlRBTree<T, I, L, M>::SetParent(I i, I parent)
{
	Links(i).m_Parent = parent;
}

template < class T, class I, typename L, class M >
inline void  CUtlRBTree<T, I, L, M>::SetLeftChild(I i, I child)
{
	Links(i).m_Left = child;
}

template < class T, class I, typename L, class M >
inline void  CUtlRBTree<T, I, L, M>::SetRightChild(I i, I child)
{
	Links(i).m_Right = child;
}

template < class T, class I, typename L, class M >
inline typename CUtlRBTree<T, I, L, M>::Links_t const& CUtlRBTree<T, I, L, M>::Links(I i) const
{
	static const Links_t s_Sentinel =
	{
		M::INVALID_INDEX, M::INVALID_INDEX, M::INVALID_INDEX, CUtlRBTree<T, I, L, M>::BLACK
	};

	return (i != InvalidIndex()) ? m_Elements[i] : s_Sentinel;
}

template < class T, class I, typename L, class M >
inline typename CUtlRBTree<T, I, L, M>::Links_t& CUtlRBTree<T, I, L, M>::Links(I i)
{
	Assert(i != InvalidIndex());
	return m_Elements[i];
}

template < class T, class I, typename L, class M >
inline bool CUtlRBTree<T, I, L, M>::IsRed(I i) const
{
	return Color(i) == RED;
}

template < class T, class I, typename L, class M >
inline bool CUtlRBTree<T, I, L, M>::IsBlack(I i) const
{
	return Color(i) == BLACK;
}


template < class T, class I, typename L, class M >
inline typename CUtlRBTree<T, I, L, M>::NodeColor_t  CUtlRBTree<T, I, L, M>::Color(I i) const
{
	return (NodeColor_t)(i != InvalidIndex() ? m_Elements[i].m_Tag : BLACK);
}

template < class T, class I, typename L, class M >
inline void CUtlRBTree<T, I, L, M>::SetColor(I i, typename CUtlRBTree<T, I, L, M>::NodeColor_t c)
{
	Links(i).m_Tag = (I)c;
}

#pragma warning(push)
#pragma warning(disable:4389)     
template < class T, class I, typename L, class M >
I  CUtlRBTree<T, I, L, M>::NewNode()
{
	I elem;

	if (m_FirstFree == InvalidIndex())
	{
		Assert(m_Elements.IsValidIterator(m_LastAlloc) || m_NumElements == 0);
		typename M::Iterator_t it = m_Elements.IsValidIterator(m_LastAlloc) ? m_Elements.Next(m_LastAlloc) : m_Elements.First();
		if (!m_Elements.IsValidIterator(it))
		{
			MEM_ALLOC_CREDIT_CLASS();
			m_Elements.Grow();

			it = m_Elements.IsValidIterator(m_LastAlloc) ? m_Elements.Next(m_LastAlloc) : m_Elements.First();

			Assert(m_Elements.IsValidIterator(it));
			if (!m_Elements.IsValidIterator(it))
			{
				Error("CUtlRBTree overflow!\n");
			}
		}
		m_LastAlloc = it;
		elem = m_Elements.GetIndex(m_LastAlloc);
		Assert(m_Elements.IsValidIterator(m_LastAlloc));
	}
	else
	{
		elem = m_FirstFree;
		m_FirstFree = RightChild(m_FirstFree);
	}

#ifdef _DEBUG
	Links_t& node = Links(elem);
	node.m_Left = node.m_Right = node.m_Parent = InvalidIndex();
#endif

	Construct(&Element(elem));
	ResetDbgInfo();

	return elem;
}
#pragma warning(pop)

template < class T, class I, typename L, class M >
void  CUtlRBTree<T, I, L, M>::FreeNode(I i)
{
	Assert(IsValidIndex(i) && (i != InvalidIndex()));
	Destruct(&Element(i));
	SetLeftChild(i, i);        
	SetRightChild(i, m_FirstFree);
	m_FirstFree = i;
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::RotateLeft(I elem)
{
	I rightchild = RightChild(elem);
	SetRightChild(elem, LeftChild(rightchild));
	if (LeftChild(rightchild) != InvalidIndex())
		SetParent(LeftChild(rightchild), elem);

	if (rightchild != InvalidIndex())
		SetParent(rightchild, Parent(elem));
	if (!IsRoot(elem))
	{
		if (IsLeftChild(elem))
			SetLeftChild(Parent(elem), rightchild);
		else
			SetRightChild(Parent(elem), rightchild);
	}
	else
		m_Root = rightchild;

	SetLeftChild(rightchild, elem);
	if (elem != InvalidIndex())
		SetParent(elem, rightchild);
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::RotateRight(I elem)
{
	I leftchild = LeftChild(elem);
	SetLeftChild(elem, RightChild(leftchild));
	if (RightChild(leftchild) != InvalidIndex())
		SetParent(RightChild(leftchild), elem);

	if (leftchild != InvalidIndex())
		SetParent(leftchild, Parent(elem));
	if (!IsRoot(elem))
	{
		if (IsRightChild(elem))
			SetRightChild(Parent(elem), leftchild);
		else
			SetLeftChild(Parent(elem), leftchild);
	}
	else
		m_Root = leftchild;

	SetRightChild(leftchild, elem);
	if (elem != InvalidIndex())
		SetParent(elem, leftchild);
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::InsertRebalance(I elem)
{
	while (!IsRoot(elem) && (Color(Parent(elem)) == RED))
	{
		I parent = Parent(elem);
		I grandparent = Parent(parent);

		if (IsLeftChild(parent))
		{
			I uncle = RightChild(grandparent);
			if (IsRed(uncle))
			{
				SetColor(parent, BLACK);
				SetColor(uncle, BLACK);
				SetColor(grandparent, RED);
				elem = grandparent;
			}
			else
			{
				if (IsRightChild(elem))
				{
					elem = parent;
					RotateLeft(elem);
					parent = Parent(elem);
					grandparent = Parent(parent);
				}
				SetColor(parent, BLACK);
				SetColor(grandparent, RED);
				RotateRight(grandparent);
			}
		}
		else
		{
			I uncle = LeftChild(grandparent);
			if (IsRed(uncle))
			{
				SetColor(parent, BLACK);
				SetColor(uncle, BLACK);
				SetColor(grandparent, RED);
				elem = grandparent;
			}
			else
			{
				if (IsLeftChild(elem))
				{
					elem = parent;
					RotateRight(parent);
					parent = Parent(elem);
					grandparent = Parent(parent);
				}
				SetColor(parent, BLACK);
				SetColor(grandparent, RED);
				RotateLeft(grandparent);
			}
		}
	}
	SetColor(m_Root, BLACK);
}


template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::InsertAt(I parent, bool leftchild)
{
	I i = NewNode();
	LinkToParent(i, parent, leftchild);
	++m_NumElements;

	Assert(IsValid());

	return i;
}

template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::LinkToParent(I i, I parent, bool isLeft)
{
	Links_t& elem = Links(i);
	elem.m_Parent = parent;
	elem.m_Left = elem.m_Right = InvalidIndex();
	elem.m_Tag = RED;

	if (parent != InvalidIndex())
	{
		if (isLeft)
			Links(parent).m_Left = i;
		else
			Links(parent).m_Right = i;
	}
	else
	{
		m_Root = i;
	}

	InsertRebalance(i);
}

template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::RemoveRebalance(I elem)
{
	while (elem != m_Root && IsBlack(elem))
	{
		I parent = Parent(elem);

		if (elem == LeftChild(parent))
		{
			I sibling = RightChild(parent);
			if (IsRed(sibling))
			{
				SetColor(sibling, BLACK);
				SetColor(parent, RED);
				RotateLeft(parent);

				parent = Parent(elem);
				sibling = RightChild(parent);
			}
			if ((IsBlack(LeftChild(sibling))) && (IsBlack(RightChild(sibling))))
			{
				if (sibling != InvalidIndex())
					SetColor(sibling, RED);
				elem = parent;
			}
			else
			{
				if (IsBlack(RightChild(sibling)))
				{
					SetColor(LeftChild(sibling), BLACK);
					SetColor(sibling, RED);
					RotateRight(sibling);

					parent = Parent(elem);
					sibling = RightChild(parent);
				}
				SetColor(sibling, Color(parent));
				SetColor(parent, BLACK);
				SetColor(RightChild(sibling), BLACK);
				RotateLeft(parent);
				elem = m_Root;
			}
		}
		else
		{
			I sibling = LeftChild(parent);
			if (IsRed(sibling))
			{
				SetColor(sibling, BLACK);
				SetColor(parent, RED);
				RotateRight(parent);

				parent = Parent(elem);
				sibling = LeftChild(parent);
			}
			if ((IsBlack(RightChild(sibling))) && (IsBlack(LeftChild(sibling))))
			{
				if (sibling != InvalidIndex())
					SetColor(sibling, RED);
				elem = parent;
			}
			else
			{
				if (IsBlack(LeftChild(sibling)))
				{
					SetColor(RightChild(sibling), BLACK);
					SetColor(sibling, RED);
					RotateLeft(sibling);

					parent = Parent(elem);
					sibling = LeftChild(parent);
				}
				SetColor(sibling, Color(parent));
				SetColor(parent, BLACK);
				SetColor(LeftChild(sibling), BLACK);
				RotateRight(parent);
				elem = m_Root;
			}
		}
	}
	SetColor(elem, BLACK);
}

template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::Unlink(I elem)
{
	if (elem != InvalidIndex())
	{
		I x, y;

		if ((LeftChild(elem) == InvalidIndex()) ||
			(RightChild(elem) == InvalidIndex()))
		{
			y = elem;
		}
		else
		{
			y = RightChild(elem);
			while (LeftChild(y) != InvalidIndex())
				y = LeftChild(y);
		}

		if (LeftChild(y) != InvalidIndex())
			x = LeftChild(y);
		else
			x = RightChild(y);

		if (x != InvalidIndex())
			SetParent(x, Parent(y));
		if (!IsRoot(y))
		{
			if (IsLeftChild(y))
				SetLeftChild(Parent(y), x);
			else
				SetRightChild(Parent(y), x);
		}
		else
			m_Root = x;

		NodeColor_t ycolor = Color(y);
		if (y != elem)
		{
			SetParent(y, Parent(elem));
			SetRightChild(y, RightChild(elem));
			SetLeftChild(y, LeftChild(elem));

			if (!IsRoot(elem))
				if (IsLeftChild(elem))
					SetLeftChild(Parent(elem), y);
				else
					SetRightChild(Parent(elem), y);
			else
				m_Root = y;

			if (LeftChild(y) != InvalidIndex())
				SetParent(LeftChild(y), y);
			if (RightChild(y) != InvalidIndex())
				SetParent(RightChild(y), y);

			SetColor(y, Color(elem));
		}

		if ((x != InvalidIndex()) && (ycolor == BLACK))
			RemoveRebalance(x);
	}
}

template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::Link(I elem)
{
	if (elem != InvalidIndex())
	{
		I parent = InvalidIndex();
		bool leftchild = false;

		FindInsertionPosition(Element(elem), parent, leftchild);

		LinkToParent(elem, parent, leftchild);

		Assert(IsValid());
	}
}

template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::RemoveAt(I elem)
{
	if (elem != InvalidIndex())
	{
		Unlink(elem);

		FreeNode(elem);
		--m_NumElements;

		Assert(IsValid());
	}
}


template < class T, class I, typename L, class M > bool CUtlRBTree<T, I, L, M>::Remove(T const& search)
{
	I node = Find(search);
	if (node != InvalidIndex())
	{
		RemoveAt(node);
		return true;
	}
	return false;
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::RemoveAll()
{
	if (m_LastAlloc == m_Elements.InvalidIterator())
	{
		Assert(m_Root == InvalidIndex());
		Assert(m_FirstFree == InvalidIndex());
		Assert(m_NumElements == 0);
		return;
	}

	for (typename M::Iterator_t it = m_Elements.First(); it != m_Elements.InvalidIterator(); it = m_Elements.Next(it))
	{
		I i = m_Elements.GetIndex(it);
		if (IsValidIndex(i))       
		{
			Destruct(&Element(i));
			SetRightChild(i, m_FirstFree);
			SetLeftChild(i, i);
			m_FirstFree = i;
		}

		if (it == m_LastAlloc)
			break;         
	}

	m_Root = InvalidIndex();
	m_NumElements = 0;

	Assert(IsValid());
}

template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::Purge()
{
	RemoveAll();
	m_FirstFree = InvalidIndex();
	m_Elements.Purge();
	m_LastAlloc = m_Elements.InvalidIterator();
}


template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::FirstInorder() const
{
	I i = m_Root;
	I left;
	while ((left = LeftChild(i)) != InvalidIndex())
		i = left;
	return i;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::NextInorder(I i) const
{
	Assert(IsValidIndex(i));

	I right;
	if ((right = RightChild(i)) != InvalidIndex())
	{
		i = right;
		I left;
		while ((left = LeftChild(i)) != InvalidIndex())
			i = left;
		return i;
	}

	I parent = Parent(i);
	while (IsRightChild(i))
	{
		i = parent;
		if (i == InvalidIndex()) break;
		parent = Parent(i);
	}
	return parent;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::PrevInorder(I i) const
{
	Assert(IsValidIndex(i));

	I left, right;
	if ((left = LeftChild(i)) != InvalidIndex())
	{
		i = left;
		while ((right = RightChild(i)) != InvalidIndex())
			i = right;
		return i;
	}

	I parent = Parent(i);
	while (IsLeftChild(i))
	{
		i = parent;
		if (i == InvalidIndex()) break;
		parent = Parent(i);
	}
	return parent;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::LastInorder() const
{
	I i = m_Root;
	I right;
	while ((right = RightChild(i)) != InvalidIndex())
		i = right;
	return i;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::FirstPreorder() const
{
	return m_Root;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::NextPreorder(I i) const
{
	I left, right;
	if ((left = LeftChild(i)) != InvalidIndex())
		return left;

	if ((right = RightChild(i)) != InvalidIndex())
		return right;

	I parent = Parent(i);
	while (parent != InvalidIndex())
	{
		if (IsLeftChild(i) && (RightChild(parent) != InvalidIndex()))
			return RightChild(parent);
		i = parent;
		parent = Parent(parent);
	}
	return InvalidIndex();
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::PrevPreorder(I i) const
{
	Assert(0);     
	return InvalidIndex();
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::LastPreorder() const
{
	I i = m_Root;
	while (1)
	{
		I left, right;
		while ((right = RightChild(i)) != InvalidIndex())
			i = right;

		if ((left = LeftChild(i)) != InvalidIndex())
			i = left;
		else
			break;
	}
	return i;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::FirstPostorder() const
{
	I i = m_Root;
	while (!IsLeaf(i))
	{
		I left;
		if ((left = LeftChild(i)) != InvalidIndex())
			i = left;
		else
			i = RightChild(i);
	}
	return i;
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::NextPostorder(I i) const
{
	I parent = Parent(i);
	if (parent == InvalidIndex())
		return InvalidIndex();

	if (IsRightChild(i))
		return parent;

	if (RightChild(parent) == InvalidIndex())
		return parent;

	i = RightChild(parent);
	while (!IsLeaf(i))
	{
		I left;
		if ((left = LeftChild(i)) != InvalidIndex())
			i = left;
		else
			i = RightChild(i);
	}
	return i;
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::Reinsert(I elem)
{
	Unlink(elem);
	Link(elem);
}


template < class T, class I, typename L, class M >
int CUtlRBTree<T, I, L, M>::Depth(I node) const
{
	if (node == InvalidIndex())
		return 0;

	int depthright = Depth(RightChild(node));
	int depthleft = Depth(LeftChild(node));
	return MAX(depthright, depthleft) + 1;
}


template < class T, class I, typename L, class M >
bool CUtlRBTree<T, I, L, M>::IsValid() const
{
	if (!Count())
		return true;

	if (m_LastAlloc == m_Elements.InvalidIterator())
		return false;

	if (!m_Elements.IsIdxValid(Root()))
		return false;

	if (Parent(Root()) != InvalidIndex())
		return false;

#ifdef UTLTREE_PARANOID

	int numFree = 0;
	for (int i = m_FirstFree; i != InvalidIndex(); i = RightChild(i))
	{
		++numFree;
		if (!m_Elements.IsIdxValid(i))
			return false;
	}

	int nElements = 0;
	int numFree2 = 0;
	for (M::Iterator_t it = m_Elements.First(); it != m_Elements.InvalidIterator(); it = m_Elements.Next(it))
	{
		I i = m_Elements.GetIndex(it);
		if (!IsValidIndex(i))
		{
			++numFree2;
		}
		else
		{
			++nElements;

			int right = RightChild(i);
			int left = LeftChild(i);
			if ((right == left) && (right != InvalidIndex()))
				return false;

			if (right != InvalidIndex())
			{
				if (!IsValidIndex(right))
					return false;
				if (Parent(right) != i)
					return false;
				if (IsRed(i) && IsRed(right))
					return false;
			}

			if (left != InvalidIndex())
			{
				if (!IsValidIndex(left))
					return false;
				if (Parent(left) != i)
					return false;
				if (IsRed(i) && IsRed(left))
					return false;
			}
		}

		if (it == m_LastAlloc)
			break;
	}
	if (numFree2 != numFree)
		return false;

	if (nElements != m_NumElements)
		return false;

#endif  

	return true;
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::SetLessFunc(const typename CUtlRBTree<T, I, L, M>::LessFunc_t& func)
{
	if (!m_LessFunc)
	{
		m_LessFunc = func;
	}
	else if (Count() > 0)
	{
		Assert(0);
	}
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::FindInsertionPosition(T const& insert, I& parent, bool& leftchild)
{
	Assert(!!m_LessFunc);

	I current = m_Root;
	parent = InvalidIndex();
	leftchild = false;
	while (current != InvalidIndex())
	{
		parent = current;
		if (m_LessFunc(insert, Element(current)))
		{
			leftchild = true; current = LeftChild(current);
		}
		else
		{
			leftchild = false; current = RightChild(current);
		}
	}
}

template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::Insert(T const& insert)
{
	I parent = InvalidIndex();
	bool leftchild = false;
	FindInsertionPosition(insert, parent, leftchild);
	I newNode = InsertAt(parent, leftchild);
	CopyConstruct(&Element(newNode), insert);
	return newNode;
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::Insert(const T* pArray, int nItems)
{
	while (nItems--)
	{
		Insert(*pArray++);
	}
}


template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::InsertIfNotFound(T const& insert)
{
	I parent;
	bool leftchild;

	I current = m_Root;
	parent = InvalidIndex();
	leftchild = false;
	while (current != InvalidIndex())
	{
		parent = current;
		if (m_LessFunc(insert, Element(current)))
		{
			leftchild = true; current = LeftChild(current);
		}
		else if (m_LessFunc(Element(current), insert))
		{
			leftchild = false; current = RightChild(current);
		}
		else
			return InvalidIndex();
	}

	I newNode = InsertAt(parent, leftchild);
	CopyConstruct(&Element(newNode), insert);
	return newNode;
}


template < class T, class I, typename L, class M >
I CUtlRBTree<T, I, L, M>::Find(T const& search) const
{
	Assert(!!m_LessFunc);

	I current = m_Root;
	while (current != InvalidIndex())
	{
		if (m_LessFunc(search, Element(current)))
			current = LeftChild(current);
		else if (m_LessFunc(Element(current), search))
			current = RightChild(current);
		else
			break;
	}
	return current;
}


template <class T, class I, typename L, class E>
I CUtlRBTree<T, I, L, E>::FindFirst(T const& search) const
{
	Assert(!!m_LessFunc);

	I current = m_Root;
	I best = InvalidIndex();
	while (current != InvalidIndex())
	{
		if (m_LessFunc(search, Element(current)))
			current = LeftChild(current);
		else if (m_LessFunc(Element(current), search))
			current = RightChild(current);
		else
		{
			best = current;
			current = LeftChild(current);
		}
	}
	return best;
}


template <class T, class I, typename L, class E>
I CUtlRBTree<T, I, L, E>::FindClosest(T const& search, CompareOperands_t eFindCriteria) const
{
	Assert(!!m_LessFunc);
	Assert((eFindCriteria & (k_EGreaterThan | k_ELessThan)) ^ (k_EGreaterThan | k_ELessThan));

	I current = m_Root;
	I best = InvalidIndex();

	while (current != InvalidIndex())
	{
		if (m_LessFunc(search, Element(current)))
		{
			if (eFindCriteria & k_EGreaterThan)
				best = current;
			current = LeftChild(current);
		}
		else if (m_LessFunc(Element(current), search))
		{
			if (eFindCriteria & k_ELessThan)
				best = current;
			current = RightChild(current);
		}
		else
		{
			if (eFindCriteria & k_EEqual)
			{
				best = current;
				break;
			}
			else if (eFindCriteria & k_EGreaterThan)
			{
				current = RightChild(current);
			}
			else if (eFindCriteria & k_ELessThan)
			{
				current = LeftChild(current);
			}
		}
	}
	return best;
}


template < class T, class I, typename L, class M >
void CUtlRBTree<T, I, L, M>::Swap(CUtlRBTree< T, I, L >& that)
{
	m_Elements.Swap(that.m_Elements);
	V_swap(m_LessFunc, that.m_LessFunc);
	V_swap(m_Root, that.m_Root);
	V_swap(m_NumElements, that.m_NumElements);
	V_swap(m_FirstFree, that.m_FirstFree);
	V_swap(m_pElements, that.m_pElements);
	V_swap(m_LastAlloc, that.m_LastAlloc);
	Assert(IsValid());
	Assert(m_Elements.IsValidIterator(m_LastAlloc) || (m_NumElements == 0 && m_FirstFree == InvalidIndex()));
}


#endif  