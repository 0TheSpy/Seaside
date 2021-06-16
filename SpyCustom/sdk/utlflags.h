#ifndef UTLFLAGS_H
#define UTLFLAGS_H
#ifdef _WIN32
#pragma once
#endif

#include "dbg.h"


template< class T >
class CUtlFlags
{
public:
	CUtlFlags(int nInitialFlags = 0);

	void SetFlag(int nFlagMask);
	void SetFlag(int nFlagMask, bool bEnable);

	void ClearFlag(int nFlagMask);
	void ClearAllFlags();
	bool IsFlagSet(int nFlagMask) const;

	bool IsAnyFlagSet() const;

private:
	T m_nFlags;
};


template< class T >
CUtlFlags<T>::CUtlFlags(int nInitialFlags)
{
	Assert(nInitialFlags == (T)nInitialFlags);

	m_nFlags = (T)nInitialFlags;
}


template< class T >
void CUtlFlags<T>::SetFlag(int nFlagMask)
{
	Assert(nFlagMask == (T)nFlagMask);

	m_nFlags |= (T)nFlagMask;
}

template< class T >
void CUtlFlags<T>::SetFlag(int nFlagMask, bool bEnable)
{
	Assert(nFlagMask == (T)nFlagMask);

	if (bEnable)
	{
		m_nFlags |= (T)nFlagMask;
	}
	else
	{
		m_nFlags &= ~((T)nFlagMask);
	}
}


template< class T >
void CUtlFlags<T>::ClearFlag(int nFlagMask)
{
	Assert(nFlagMask == (T)nFlagMask);
	m_nFlags &= ~((T)nFlagMask);
}

template< class T >
void CUtlFlags<T>::ClearAllFlags()
{
	m_nFlags = 0;
}


template< class T >
bool CUtlFlags<T>::IsFlagSet(int nFlagMask) const
{
	Assert(nFlagMask == (T)nFlagMask);
	return (m_nFlags & nFlagMask) != 0;
}


template< class T >
bool CUtlFlags<T>::IsAnyFlagSet() const
{
	return m_nFlags != 0;
}


#endif  