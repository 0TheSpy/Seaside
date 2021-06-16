#ifndef UTLSTRING_H
#define UTLSTRING_H
#ifdef _WIN32
#pragma once
#endif


#include "utlmemory.h"
#include "strtools.h"
#include "limits.h"

#if defined( OSX )
#define wcsdup wcsdup_osx
inline wchar_t* wcsdup_osx(const wchar_t* pString)
{
	wchar_t* pMemory;

	if (!pString)
		return NULL;

	size_t len = (wcslen(pString) + 1);
	if ((pMemory = (wchar_t*)malloc(len * sizeof(wchar_t))) != NULL)
	{
		return wcscpy(pMemory, pString);
	}

	return NULL;
}
#endif

class CUtlBinaryBlock
{
public:
	CUtlBinaryBlock(int growSize = 0, int initSize = 0);

	CUtlBinaryBlock(void* pMemory, int nSizeInBytes, int nInitialLength);
	CUtlBinaryBlock(const void* pMemory, int nSizeInBytes);
	CUtlBinaryBlock(const CUtlBinaryBlock& src);

	void		Get(void* pValue, int nMaxLen) const;
	void		Set(const void* pValue, int nLen);
	const void* Get() const;
	void* Get();

	unsigned char& operator[](int i);
	const unsigned char& operator[](int i) const;

	int			Length() const;
	void		SetLength(int nLength);	    
	bool		IsEmpty() const;
	void		Clear();
	void		Purge();

	bool		IsReadOnly() const;

	CUtlBinaryBlock& operator=(const CUtlBinaryBlock& src);

	bool operator==(const CUtlBinaryBlock& src) const;

private:
	CUtlMemory<unsigned char> m_Memory;
	int m_nActualLength;
};


inline const void* CUtlBinaryBlock::Get() const
{
	return m_Memory.Base();
}

inline void* CUtlBinaryBlock::Get()
{
	return m_Memory.Base();
}

inline int CUtlBinaryBlock::Length() const
{
	return m_nActualLength;
}

inline unsigned char& CUtlBinaryBlock::operator[](int i)
{
	return m_Memory[i];
}

inline const unsigned char& CUtlBinaryBlock::operator[](int i) const
{
	return m_Memory[i];
}

inline bool CUtlBinaryBlock::IsReadOnly() const
{
	return m_Memory.IsReadOnly();
}

inline bool CUtlBinaryBlock::IsEmpty() const
{
	return Length() == 0;
}

inline void CUtlBinaryBlock::Clear()
{
	SetLength(0);
}

inline void CUtlBinaryBlock::Purge()
{
	SetLength(0);
	m_Memory.Purge();
}


class CUtlString
{
public:
	typedef enum
	{
		PATTERN_NONE = 0x00000000,
		PATTERN_DIRECTORY = 0x00000001
	} TUtlStringPattern;

public:
	CUtlString();
	CUtlString(const char* pString);
	CUtlString(const CUtlString& string);

	CUtlString(void* pMemory, int nSizeInBytes, int nInitialLength);
	CUtlString(const void* pMemory, int nSizeInBytes);

	const char* Get() const;
	void		Set(const char* pValue);

	void Clear() { Set(NULL); }

	operator const char* () const;

	const char* String() const { return Get(); }

	int			Length() const;
	bool		IsEmpty() const;

	void		SetLength(int nLen);
	char* Get();
	void		Purge();

	void		ToLower();
	void		ToUpper();

	void		Append(const char* pchAddition);

	void		StripTrailingSlash();

	CUtlString& operator=(const CUtlString& src);
	CUtlString& operator=(const char* src);

	bool operator==(const CUtlString& src) const;
	bool operator==(const char* src) const;
	bool operator!=(const CUtlString& src) const { return !operator==(src); }
	bool operator!=(const char* src) const { return !operator==(src); }

	inline friend bool operator==(const char* lhs, const CUtlString& rhs) { return rhs.operator==(lhs); }
	inline friend bool operator!=(const char* lhs, const CUtlString& rhs) { return rhs.operator!=(lhs); }

	CUtlString& operator+=(const CUtlString& rhs);
	CUtlString& operator+=(const char* rhs);
	CUtlString& operator+=(char c);
	CUtlString& operator+=(int rhs);
	CUtlString& operator+=(double rhs);

	bool IsValid() const;

	bool MatchesPattern(const CUtlString& Pattern, int nFlags = 0);		         

	int Format(PRINTF_FORMAT_STRING const char* pFormat, ...);
	void SetDirect(const char* pValue, int nChars);

	typedef const char* AltArgumentType_t;

	CUtlString Slice(int32 nStart = 0, int32 nEnd = INT_MAX);

	CUtlString Left(int32 nChars);
	CUtlString Right(int32 nChars);

	CUtlString Replace(char cFrom, char cTo);

	CUtlString AbsPath(const char* pStartingDir = NULL);

	CUtlString UnqualifiedFilename();

	CUtlString DirName();

	static CUtlString PathJoin(const char* pStr1, const char* pStr2);

	static int __cdecl SortCaseInsensitive(const CUtlString* pString1, const CUtlString* pString2);
	static int __cdecl SortCaseSensitive(const CUtlString* pString1, const CUtlString* pString2);

private:
	CUtlBinaryBlock m_Storage;
};

inline bool CUtlString::IsEmpty() const
{
	return Length() == 0;
}

inline bool CUtlString::IsValid() const
{
	return (String() != NULL);
}

inline int __cdecl CUtlString::SortCaseInsensitive(const CUtlString* pString1, const CUtlString* pString2)
{
	return V_stricmp(pString1->String(), pString2->String());
}

inline int __cdecl CUtlString::SortCaseSensitive(const CUtlString* pString1, const CUtlString* pString2)
{
	return V_strcmp(pString1->String(), pString2->String());
}

template < typename T >
class StringFuncs
{
public:
	static T* Duplicate(const T* pValue);
	static void		 Copy(T* out_pOut, const T* pIn, int iLengthInChars);
	static int		 Compare(const T* pLhs, const T* pRhs);
	static int		 CaselessCompare(const T* pLhs, const T* pRhs);
	static int		 Length(const T* pValue);
	static const T* FindChar(const T* pStr, const T cSearch);
	static const T* EmptyString();
	static const T* NullDebugString();
};

template < >
class StringFuncs<char>
{
public:
	static char* Duplicate(const char* pValue) { return strdup(pValue); }
	static void		   Copy(OUT_CAP(iLengthInChars) char* out_pOut, const char* pIn, int iLengthInChars) { strncpy(out_pOut, pIn, iLengthInChars); }
	static int		   Compare(const char* pLhs, const char* pRhs) { return strcmp(pLhs, pRhs); }
	static int		   CaselessCompare(const char* pLhs, const char* pRhs) { return Q_strcasecmp(pLhs, pRhs); }
	static int		   Length(const char* pValue) { return (int)strlen(pValue); }
	static const char* FindChar(const char* pStr, const char cSearch) { return strchr(pStr, cSearch); }
	static const char* EmptyString() { return ""; }
	static const char* NullDebugString() { return "(null)"; }
};

template < >
class StringFuncs<wchar_t>
{
public:
	static wchar_t* Duplicate(const wchar_t* pValue) { return wcsdup(pValue); }
	static void			  Copy(OUT_CAP(iLengthInChars) wchar_t* out_pOut, const wchar_t* pIn, int iLengthInChars) { wcsncpy(out_pOut, pIn, iLengthInChars); }
	static int			  Compare(const wchar_t* pLhs, const wchar_t* pRhs) { return wcscmp(pLhs, pRhs); }
	static int			  CaselessCompare(const wchar_t* pLhs, const wchar_t* pRhs);   
	static int			  Length(const wchar_t* pValue) { return (int)wcslen(pValue); }
	static const wchar_t* FindChar(const wchar_t* pStr, const wchar_t cSearch) { return wcschr(pStr, cSearch); }
	static const wchar_t* EmptyString() { return L""; }
	static const wchar_t* NullDebugString() { return L"(null)"; }
};

template < typename T = char >
class CUtlConstStringBase
{
public:
	CUtlConstStringBase() : m_pString(NULL) {}
	explicit CUtlConstStringBase(const T* pString) : m_pString(NULL) { Set(pString); }
	CUtlConstStringBase(const CUtlConstStringBase& src) : m_pString(NULL) { Set(src.m_pString); }
	~CUtlConstStringBase() { Set(NULL); }

	void Set(const T* pValue);
	void Clear() { Set(NULL); }

	const T* Get() const { return m_pString ? m_pString : StringFuncs<T>::EmptyString(); }
	operator const T* () const { return m_pString ? m_pString : StringFuncs<T>::EmptyString(); }

	bool IsEmpty() const { return m_pString == NULL; }         

	int Compare(const T* rhs) const;

	bool operator<(const T* rhs) const { return Compare(rhs) < 0; }
	bool operator==(const T* rhs) const { return Compare(rhs) == 0; }
	bool operator!=(const T* rhs) const { return Compare(rhs) != 0; }
	bool operator<(const CUtlConstStringBase& rhs) const { return Compare(rhs.m_pString) < 0; }
	bool operator==(const CUtlConstStringBase& rhs) const { return Compare(rhs.m_pString) == 0; }
	bool operator!=(const CUtlConstStringBase& rhs) const { return Compare(rhs.m_pString) != 0; }

	inline friend bool operator<(const T* lhs, const CUtlConstStringBase& rhs) { return rhs.Compare(lhs) > 0; }
	inline friend bool operator==(const T* lhs, const CUtlConstStringBase& rhs) { return rhs.Compare(lhs) == 0; }
	inline friend bool operator!=(const T* lhs, const CUtlConstStringBase& rhs) { return rhs.Compare(lhs) != 0; }

	CUtlConstStringBase& operator=(const T* src) { Set(src); return *this; }
	CUtlConstStringBase& operator=(const CUtlConstStringBase& src) { Set(src.m_pString); return *this; }

	typedef const T* AltArgumentType_t;

protected:
	const T* m_pString;
};

template < typename T >
void CUtlConstStringBase<T>::Set(const T* pValue)
{
	if (pValue != m_pString)
	{
		free((void*)m_pString);
		m_pString = pValue && pValue[0] ? StringFuncs<T>::Duplicate(pValue) : NULL;
	}
}

template < typename T >
int CUtlConstStringBase<T>::Compare(const T* rhs) const
{
	if (!rhs || !rhs[0])
		return m_pString ? 1 : 0;

	if (!m_pString)
		return -1;

	return StringFuncs<T>::Compare(m_pString, rhs);
}

typedef	CUtlConstStringBase<char>		CUtlConstString;
typedef	CUtlConstStringBase<wchar_t>	CUtlConstWideString;

template < typename T > struct UTLConstStringCaselessStringLessFunctor { bool operator()(const CUtlConstStringBase<T>& a, const char* b) const { return StringFuncs<T>::CaselessCompare(a.Get(), b) < 0; } };
template < typename T > struct UTLConstStringCaselessStringEqualFunctor { bool operator()(const CUtlConstStringBase<T>& a, const char* b) const { return StringFuncs<T>::CaselessCompare(a.Get(), b) == 0; } };


#endif  