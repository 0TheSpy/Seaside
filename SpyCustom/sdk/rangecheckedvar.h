#ifndef RANGECHECKEDVAR_H
#define RANGECHECKEDVAR_H
#ifdef _WIN32
#pragma once
#endif


#include "dbg.h"
#include "threadtools.h"
#include "vector.h"
#include <float.h>


class CDisableRangeChecks
{
public:
	CDisableRangeChecks();
	~CDisableRangeChecks();
};


template< class T >
inline void RangeCheck(const T& value, int minValue, int maxValue)
{
#ifdef _DEBUG
	extern bool g_bDoRangeChecks;
	if (ThreadInMainThread() && g_bDoRangeChecks)
	{
		Assert(_finite(value));
	}
#endif
}

inline void RangeCheck(const Vector& value, int minValue, int maxValue)
{
#ifdef _DEBUG
	RangeCheck(value.x, minValue, maxValue);
	RangeCheck(value.y, minValue, maxValue);
	RangeCheck(value.z, minValue, maxValue);
#endif
}


template< class T, int minValue, int maxValue, int startValue >
class CRangeCheckedVar
{
public:

	inline CRangeCheckedVar()
	{
		m_Val = startValue;
	}

	inline CRangeCheckedVar(const T& value)
	{
		*this = value;
	}

	T GetRaw() const
	{
		return m_Val;
	}

	inline void Clamp()
	{
		if (m_Val < minValue)
			m_Val = minValue;
		else if (m_Val > maxValue)
			m_Val = maxValue;
	}

	inline operator const T& () const
	{
		return m_Val;
	}

	inline CRangeCheckedVar<T, minValue, maxValue, startValue>& operator=(const T& value)
	{
		RangeCheck(value, minValue, maxValue);
		m_Val = value;
		return *this;
	}

	inline CRangeCheckedVar<T, minValue, maxValue, startValue>& operator+=(const T& value)
	{
		return (*this = m_Val + value);
	}

	inline CRangeCheckedVar<T, minValue, maxValue, startValue>& operator-=(const T& value)
	{
		return (*this = m_Val - value);
	}

	inline CRangeCheckedVar<T, minValue, maxValue, startValue>& operator*=(const T& value)
	{
		return (*this = m_Val * value);
	}

	inline CRangeCheckedVar<T, minValue, maxValue, startValue>& operator/=(const T& value)
	{
		return (*this = m_Val / value);
	}

private:

	T m_Val;
};

#endif  