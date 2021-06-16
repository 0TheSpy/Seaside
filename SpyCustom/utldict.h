#ifndef UTLDICT_H
#define UTLDICT_H

#ifdef _WIN32
#pragma once
#endif

#include "dbg.h"
#include "utlmap.h"

#include "utlsymbol.h"

#include "memdbgon.h"

enum EDictCompareType
{
	k_eDictCompareTypeCaseSensitive = 0,
	k_eDictCompareTypeCaseInsensitive = 1,
	k_eDictCompareTypeFilenames				        
};

#define FOR_EACH_DICT( dictName, iteratorName ) \
	for( int iteratorName=dictName.First(); iteratorName != dictName.InvalidIndex(); iteratorName = dictName.Next( iteratorName ) )

#define FOR_EACH_DICT_FAST( dictName, iteratorName ) \
	for ( int iteratorName = 0; iteratorName < dictName.MaxElement(); ++iteratorName ) if ( !dictName.IsValidIndex( iteratorName ) ) continue; else

template <class T, class I = int >
class CUtlDict
{
public:
	typedef const char* KeyType_t;
	typedef T ElemType_t;

	CUtlDict(int compareType = k_eDictCompareTypeCaseInsensitive, int growSize = 0, int initSize = 0);
	~CUtlDict();

	void EnsureCapacity(int);

	T& Element(I i);
	const T& Element(I i) const;
	T& operator[](I i);
	const T& operator[](I i) const;

	char const* GetElementName(I i) const;

	void		SetElementName(I i, char const* pName);

	unsigned int Count() const;

	I MaxElement() const;

	bool  IsValidIndex(I i) const;

	static I InvalidIndex();

	I  Insert(const char* pName, const T& element);
	I  Insert(const char* pName);

	I  Find(const char* pName) const;
	bool HasElement(const char* pName) const;

	void	RemoveAt(I i);
	void	Remove(const char* pName);
	void	RemoveAll();

	void	Purge();
	void	PurgeAndDeleteElements();	     

	I		First() const;
	I		Next(I i) const;

	typedef I IndexType_t;

protected:
	typedef CUtlMap<const char*, T, I> DictElementMap_t;
	DictElementMap_t m_Elements;
};


template <class T, class I>
CUtlDict<T, I>::CUtlDict(int compareType, int growSize, int initSize) : m_Elements(growSize, initSize)
{
	if (compareType == k_eDictCompareTypeFilenames)
	{
		m_Elements.SetLessFunc(CaselessStringLessThanIgnoreSlashes);
	}
	else if (compareType == k_eDictCompareTypeCaseInsensitive)
	{
		m_Elements.SetLessFunc(CaselessStringLessThan);
	}
	else
	{
		m_Elements.SetLessFunc(StringLessThan);
	}
}

template <class T, class I>
CUtlDict<T, I>::~CUtlDict()
{
	Purge();
}

template <class T, class I>
inline void CUtlDict<T, I>::EnsureCapacity(int num)
{
	return m_Elements.EnsureCapacity(num);
}

template <class T, class I>
inline T& CUtlDict<T, I>::Element(I i)
{
	return m_Elements[i];
}

template <class T, class I>
inline const T& CUtlDict<T, I>::Element(I i) const
{
	return m_Elements[i];
}

template <class T, class I>
inline char const* CUtlDict<T, I>::GetElementName(I i) const
{
	return m_Elements.Key(i);
}

template <class T, class I>
inline T& CUtlDict<T, I>::operator[](I i)
{
	return Element(i);
}

template <class T, class I>
inline const T& CUtlDict<T, I>::operator[](I i) const
{
	return Element(i);
}

template <class T, class I>
inline void CUtlDict<T, I>::SetElementName(I i, char const* pName)
{
	MEM_ALLOC_CREDIT_CLASS();
	free(const_cast<char*>(m_Elements.Key(i)));
	m_Elements.Reinsert(strdup(pName), i);
}

template <class T, class I>
inline	unsigned int CUtlDict<T, I>::Count() const
{
	return m_Elements.Count();
}

template <class T, class I>
inline I CUtlDict<T, I>::MaxElement() const
{
	return m_Elements.MaxElement();
}

template <class T, class I>
inline	bool CUtlDict<T, I>::IsValidIndex(I i) const
{
	return m_Elements.IsValidIndex(i);
}


template <class T, class I>
inline I CUtlDict<T, I>::InvalidIndex()
{
	return DictElementMap_t::InvalidIndex();
}


template <class T, class I>
void CUtlDict<T, I>::RemoveAt(I elem)
{
	free(const_cast<char*>(m_Elements.Key(elem)));
	m_Elements.RemoveAt(elem);
}


template <class T, class I> void CUtlDict<T, I>::Remove(const char* search)
{
	I node = Find(search);
	if (node != InvalidIndex())
	{
		RemoveAt(node);
	}
}


template <class T, class I>
void CUtlDict<T, I>::RemoveAll()
{
	typename DictElementMap_t::IndexType_t index = m_Elements.FirstInorder();
	while (index != m_Elements.InvalidIndex())
	{
		const char* p = m_Elements.Key(index);
		free(const_cast<char*>(p));
		index = m_Elements.NextInorder(index);
	}

	m_Elements.RemoveAll();
}

template <class T, class I>
void CUtlDict<T, I>::Purge()
{
	RemoveAll();
}


template <class T, class I>
void CUtlDict<T, I>::PurgeAndDeleteElements()
{
	I index = m_Elements.FirstInorder();
	while (index != m_Elements.InvalidIndex())
	{
		const char* p = m_Elements.Key(index);
		free(const_cast<char*>(p));
		delete m_Elements[index];
		index = m_Elements.NextInorder(index);
	}

	m_Elements.RemoveAll();
}


template <class T, class I>
I CUtlDict<T, I>::Insert(const char* pName, const T& element)
{
	MEM_ALLOC_CREDIT_CLASS();
	return m_Elements.Insert(strdup(pName), element);
}

template <class T, class I>
I CUtlDict<T, I>::Insert(const char* pName)
{
	MEM_ALLOC_CREDIT_CLASS();
	return m_Elements.Insert(strdup(pName));
}


template <class T, class I>
I CUtlDict<T, I>::Find(const char* pName) const
{
	MEM_ALLOC_CREDIT_CLASS();
	if (pName)
		return m_Elements.Find(pName);
	else
		return InvalidIndex();
}

template <class T, class I>
bool CUtlDict<T, I>::HasElement(const char* pName) const
{
	if (pName)
		return m_Elements.IsValidIndex(m_Elements.Find(pName));
	else
		return false;
}


template <class T, class I>
I CUtlDict<T, I>::First() const
{
	return m_Elements.FirstInorder();
}

template <class T, class I>
I CUtlDict<T, I>::Next(I i) const
{
	return m_Elements.NextInorder(i);
}

#include "memdbgoff.h"

#endif  
