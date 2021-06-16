#ifndef TIER1_STRTOOLS_H
#define TIER1_STRTOOLS_H

#include "platform.h"

#include <ctype.h>
#include <stdarg.h>
#ifdef _WIN32
#pragma once
#elif POSIX
#include <wchar.h>
#include <math.h>
#include <wctype.h>
#endif

#include <string.h>
#include <stdlib.h>

#ifdef _WIN64
#define str_size unsigned int
#else
#define str_size size_t
#endif

template< class T, class I > class CUtlMemory;
template< class T, class A > class CUtlVector;


void	_V_memset(const char* file, int line, void* dest, int fill, int count);
void	_V_memcpy(const char* file, int line, void* dest, const void* src, int count);
void	_V_memmove(const char* file, int line, void* dest, const void* src, int count);
int		_V_memcmp(const char* file, int line, const void* m1, const void* m2, int count);
int		_V_strlen(const char* file, int line, const char* str);
void	_V_strcpy(const char* file, int line, char* dest, const char* src);
char* _V_strrchr(const char* file, int line, const char* s, char c);
int		_V_strcmp(const char* file, int line, const char* s1, const char* s2);
int		_V_wcscmp(const char* file, int line, const wchar_t* s1, const wchar_t* s2);
char* _V_strstr(const char* file, int line, const char* s1, const char* search);
int		_V_wcslen(const char* file, int line, const wchar_t* pwch);
wchar_t* _V_wcslower(const char* file, int line, wchar_t* start);
wchar_t* _V_wcsupr(const char* file, int line, wchar_t* start);

char* V_strupr(char* start);
char* V_strlower(char* start);
int V_stricmp(const char* s1, const char* s2);
int	V_strncmp(const char* s1, const char* s2, int count);
int V_strnicmp(const char* s1, const char* s2, int n);

#ifdef POSIX

inline char* strupr(char* start)
{
	return V_strupr(start);
}

inline char* strlwr(char* start)
{
	return V_strlower(start);
}

inline wchar_t* _wcslwr(wchar_t* start)
{
	wchar_t* str = start;
	while (str && *str)
	{
		*str = (wchar_t)towlower(static_cast<wint_t>(*str));
		str++;
	}
	return start;
};

inline wchar_t* _wcsupr(wchar_t* start)
{
	wchar_t* str = start;
	while (str && *str)
	{
		*str = (wchar_t)towupper(static_cast<wint_t>(*str));
		str++;
	}
	return start;
};

#endif  


#ifdef _DEBUG

#define V_memset(dest, fill, count)		_V_memset   (__FILE__, __LINE__, (dest), (fill), (count))	
#define V_memcpy(dest, src, count)		_V_memcpy	(__FILE__, __LINE__, (dest), (src), (count))	
#define V_memmove(dest, src, count)		_V_memmove	(__FILE__, __LINE__, (dest), (src), (count))	
#define V_memcmp(m1, m2, count)			_V_memcmp	(__FILE__, __LINE__, (m1), (m2), (count))		
#define V_strlen(str)					_V_strlen	(__FILE__, __LINE__, (str))				
#define V_strcpy(dest, src)				_V_strcpy	(__FILE__, __LINE__, (dest), (src))			
#define V_strrchr(s, c)					_V_strrchr	(__FILE__, __LINE__, (s), (c))				
#define V_strcmp(s1, s2)				_V_strcmp	(__FILE__, __LINE__, (s1), (s2))			
#define V_wcscmp(s1, s2)				_V_wcscmp	(__FILE__, __LINE__, (s1), (s2))			
#define V_strstr(s1, search )			_V_strstr	(__FILE__, __LINE__, (s1), (search) )		
#define V_wcslen(pwch)					_V_wcslen	(__FILE__, __LINE__, (pwch))		
#define V_wcslower(start)				_V_wcslower (__FILE__, __LINE__, (start))		
#define V_wcsupr(start)					_V_wcsupr	(__FILE__, __LINE__, (start))				

#else


inline void		V_memset(void* dest, int fill, int count) { memset(dest, fill, count); }
inline void		V_memcpy(void* dest, const void* src, int count) { memcpy(dest, src, count); }
inline void		V_memmove(void* dest, const void* src, int count) { memmove(dest, src, count); }
inline int		V_memcmp(const void* m1, const void* m2, int count) { return memcmp(m1, m2, count); }
inline int		V_strlen(const char* str) { return (int)strlen(str); }
inline void		V_strcpy(char* dest, const char* src) { strcpy(dest, src); }
inline int		V_wcslen(const wchar_t* pwch) { return (int)wcslen(pwch); }
inline char* V_strrchr(const char* s, char c) { return (char*)strrchr(s, c); }
inline int		V_strcmp(const char* s1, const char* s2) { return strcmp(s1, s2); }
inline int		V_wcscmp(const wchar_t* s1, const wchar_t* s2) { return wcscmp(s1, s2); }
inline char* V_strstr(const char* s1, const char* search) { return (char*)strstr(s1, search); }
inline wchar_t* V_wcslower(wchar_t* start) { return _wcslwr(start); }
inline wchar_t* V_wcsupr(wchar_t* start) { return _wcsupr(start); }

#endif

int			V_atoi(const char* str);
int64 		V_atoi64(const char* str);
uint64 		V_atoui64(const char* str);
float		V_atof(const char* str);
char* V_stristr(char* pStr, const char* pSearch);
const char* V_stristr(const char* pStr, const char* pSearch);
const char* V_strnistr(const char* pStr, const char* pSearch, int n);
const char* V_strnchr(const char* pStr, char c, int n);
inline int V_strcasecmp(const char* s1, const char* s2) { return V_stricmp(s1, s2); }
inline int V_strncasecmp(const char* s1, const char* s2, int n) { return V_strnicmp(s1, s2, n); }
void		V_qsort_s(void* base, size_t num, size_t width, int(__cdecl* compare)(void*, const void*,
	const void*), void* context);


const char* StringAfterPrefix(const char* str, const char* prefix);
const char* StringAfterPrefixCaseSensitive(const char* str, const char* prefix);
inline bool	StringHasPrefix(const char* str, const char* prefix) { return StringAfterPrefix(str, prefix) != NULL; }
inline bool	StringHasPrefixCaseSensitive(const char* str, const char* prefix) { return StringAfterPrefixCaseSensitive(str, prefix) != NULL; }


void			V_normalizeFloatString(char* pFloat);

inline bool V_isdigit(char c)
{
	return c >= '0' && c <= '9';
}

inline bool V_isalpha(char c) { return isalpha((unsigned char)c) != 0; }
inline bool V_isalnum(char c) { return isalnum((unsigned char)c) != 0; }
inline bool V_isprint(char c) { return isprint((unsigned char)c) != 0; }
inline bool V_isxdigit(char c) { return isxdigit((unsigned char)c) != 0; }
inline bool V_ispunct(char c) { return ispunct((unsigned char)c) != 0; }
inline bool V_isgraph(char c) { return isgraph((unsigned char)c) != 0; }
inline bool V_isupper(char c) { return isupper((unsigned char)c) != 0; }
inline bool V_islower(char c) { return islower((unsigned char)c) != 0; }
inline bool V_iscntrl(char c) { return iscntrl((unsigned char)c) != 0; }
inline bool V_isspace(char c) { return isspace((unsigned char)c) != 0; }

void V_strncpy(OUT_Z_CAP(maxLenInChars) char* pDest, const char* pSrc, int maxLenInChars);

template <size_t maxLenInChars> void V_strcpy_safe(OUT_Z_ARRAY char(&pDest)[maxLenInChars], const char* pSrc)
{
	V_strncpy(pDest, pSrc, (int)maxLenInChars);
}

void V_wcsncpy(OUT_Z_BYTECAP(maxLenInBytes) wchar_t* pDest, wchar_t const* pSrc, int maxLenInBytes);
template <size_t maxLenInChars> void V_wcscpy_safe(OUT_Z_ARRAY wchar_t(&pDest)[maxLenInChars], wchar_t const* pSrc)
{
	V_wcsncpy(pDest, pSrc, maxLenInChars * sizeof(*pDest));
}

#define COPY_ALL_CHARACTERS -1
char* V_strncat(INOUT_Z_CAP(cchDest) char* pDest, const char* pSrc, size_t cchDest, int max_chars_to_copy = COPY_ALL_CHARACTERS);
template <size_t cchDest> char* V_strcat_safe(INOUT_Z_ARRAY char(&pDest)[cchDest], const char* pSrc, int nMaxCharsToCopy = COPY_ALL_CHARACTERS)
{
	return V_strncat(pDest, pSrc, (int)cchDest, nMaxCharsToCopy);
}

wchar_t* V_wcsncat(INOUT_Z_CAP(cchDest) wchar_t* pDest, const wchar_t* pSrc, size_t cchDest, int nMaxCharsToCopy = COPY_ALL_CHARACTERS);
template <size_t cchDest> wchar_t* V_wcscat_safe(INOUT_Z_ARRAY wchar_t(&pDest)[cchDest], const wchar_t* pSrc, int nMaxCharsToCopy = COPY_ALL_CHARACTERS)
{
	return V_wcsncat(pDest, pSrc, (int)cchDest, nMaxCharsToCopy);
}

char* V_strnlwr(INOUT_Z_CAP(cchBuf) char* pBuf, size_t cchBuf);
template <size_t cchDest> char* V_strlwr_safe(INOUT_Z_ARRAY char(&pBuf)[cchDest])
{
	return _V_strnlwr(pBuf, (int)cchDest);
}


#ifdef _WIN32
#ifndef _VA_LIST_DEFINED

#ifdef  _M_ALPHA

struct va_list
{
	char* a0;              
	int offset;           
};

#else   

typedef char* va_list;

#endif  

#define _VA_LIST_DEFINED

#endif    

#elif POSIX
#include <stdarg.h>
#endif

#ifdef _WIN32
#define CORRECT_PATH_SEPARATOR '\\'
#define CORRECT_PATH_SEPARATOR_S "\\"
#define INCORRECT_PATH_SEPARATOR '/'
#define INCORRECT_PATH_SEPARATOR_S "/"
#elif POSIX
#define CORRECT_PATH_SEPARATOR '/'
#define CORRECT_PATH_SEPARATOR_S "/"
#define INCORRECT_PATH_SEPARATOR '\\'
#define INCORRECT_PATH_SEPARATOR_S "\\"
#endif

int V_vsnprintf(OUT_Z_CAP(maxLenInCharacters) char* pDest, int maxLenInCharacters, PRINTF_FORMAT_STRING const char* pFormat, va_list params);
template <size_t maxLenInCharacters> int V_vsprintf_safe(OUT_Z_ARRAY char(&pDest)[maxLenInCharacters], PRINTF_FORMAT_STRING const char* pFormat, va_list params) { return V_vsnprintf(pDest, maxLenInCharacters, pFormat, params); }

int V_snprintf(OUT_Z_CAP(maxLenInChars) char* pDest, int maxLenInChars, PRINTF_FORMAT_STRING const char* pFormat, ...) FMTFUNCTION(3, 4);
template <size_t maxLenInChars> int V_sprintf_safe(OUT_Z_ARRAY char(&pDest)[maxLenInChars], PRINTF_FORMAT_STRING const char* pFormat, ...) FMTFUNCTION(2, 3);
template <size_t maxLenInChars> int V_sprintf_safe(OUT_Z_ARRAY char(&pDest)[maxLenInChars], PRINTF_FORMAT_STRING const char* pFormat, ...)
{
	va_list params;
	va_start(params, pFormat);
	int result = V_vsnprintf(pDest, maxLenInChars, pFormat, params);
	va_end(params);
	return result;
}

int V_vsnwprintf(OUT_Z_CAP(maxLenInCharacters) wchar_t* pDest, int maxLenInCharacters, PRINTF_FORMAT_STRING const wchar_t* pFormat, va_list params);
template <size_t maxLenInCharacters> int V_vswprintf_safe(OUT_Z_ARRAY wchar_t(&pDest)[maxLenInCharacters], PRINTF_FORMAT_STRING const wchar_t* pFormat, va_list params) { return V_vsnwprintf(pDest, maxLenInCharacters, pFormat, params); }
int V_vsnprintfRet(OUT_Z_CAP(maxLenInCharacters) char* pDest, int maxLenInCharacters, PRINTF_FORMAT_STRING const char* pFormat, va_list params, bool* pbTruncated);
template <size_t maxLenInCharacters> int V_vsprintfRet_safe(OUT_Z_ARRAY char(&pDest)[maxLenInCharacters], PRINTF_FORMAT_STRING const char* pFormat, va_list params, bool* pbTruncated) { return V_vsnprintfRet(pDest, maxLenInCharacters, pFormat, params, pbTruncated); }

int V_snwprintf(OUT_Z_CAP(maxLenInCharacters) wchar_t* pDest, int maxLenInCharacters, PRINTF_FORMAT_STRING const wchar_t* pFormat, ...);
template <size_t maxLenInChars> int V_swprintf_safe(OUT_Z_ARRAY wchar_t(&pDest)[maxLenInChars], PRINTF_FORMAT_STRING const wchar_t* pFormat, ...)
{
	va_list params;
	va_start(params, pFormat);
	int result = V_vsnwprintf(pDest, maxLenInChars, pFormat, params);
	va_end(params);
	return result;
}

char* V_pretifymem(float value, int digitsafterdecimal = 2, bool usebinaryonek = false);

char* V_pretifynum(int64 value);

int V_UTF8ToUnicode(const char* pUTF8, OUT_Z_BYTECAP(cubDestSizeInBytes) wchar_t* pwchDest, int cubDestSizeInBytes);
int V_UnicodeToUTF8(const wchar_t* pUnicode, OUT_Z_BYTECAP(cubDestSizeInBytes) char* pUTF8, int cubDestSizeInBytes);
int V_UCS2ToUnicode(const ucs2* pUCS2, OUT_Z_BYTECAP(cubDestSizeInBytes) wchar_t* pUnicode, int cubDestSizeInBytes);
int V_UCS2ToUTF8(const ucs2* pUCS2, OUT_Z_BYTECAP(cubDestSizeInBytes) char* pUTF8, int cubDestSizeInBytes);
int V_UnicodeToUCS2(const wchar_t* pUnicode, int cubSrcInBytes, OUT_Z_BYTECAP(cubDestSizeInBytes) char* pUCS2, int cubDestSizeInBytes);
int V_UTF8ToUCS2(const char* pUTF8, int cubSrcInBytes, OUT_Z_BYTECAP(cubDestSizeInBytes) ucs2* pUCS2, int cubDestSizeInBytes);

bool Q_StripPrecedingAndTrailingWhitespace(char* pch);
bool Q_StripPrecedingAndTrailingWhitespaceW(wchar_t* pwch);

bool Q_AggressiveStripPrecedingAndTrailingWhitespace(char* pch);
bool Q_AggressiveStripPrecedingAndTrailingWhitespaceW(wchar_t* pwch);

unsigned char V_nibble(char c);
void V_hextobinary(char const* in, int numchars, byte* out, int maxoutputbytes);
void V_binarytohex(const byte* in, int inputbytes, char* out, int outsize);

void V_FileBase(const char* in, char* out, int maxlen);
void V_StripTrailingSlash(char* ppath);
void V_StripExtension(const char* in, char* out, int outLen);
void V_DefaultExtension(char* path, const char* extension, int pathStringLength);
void V_SetExtension(char* path, const char* extension, int pathStringLength);
void V_StripFilename(char* path);
bool V_StripLastDir(char* dirName, int maxlen);
const char* V_UnqualifiedFileName(const char* in);
void V_ComposeFileName(const char* path, const char* filename, char* dest, int destSize);

bool V_ExtractFilePath(const char* path, char* dest, int destSize);
void V_ExtractFileExtension(const char* path, char* dest, int destSize);

const char* V_GetFileExtension(const char* path);

const char* V_GetFileName(const char* path);

bool V_RemoveDotSlashes(char* pFilename, char separator = CORRECT_PATH_SEPARATOR, bool bRemoveDoubleSlashes = true);

void V_MakeAbsolutePath(char* pOut, int outLen, const char* pPath, const char* pStartingDir = NULL);

bool V_MakeRelativePath(const char* pFullPath, const char* pDirectory, char* pRelativePath, int nBufLen);

void V_FixupPathName(OUT_Z_CAP(nOutLen) char* pOut, size_t nOutLen, const char* pPath);

void V_AppendSlash(INOUT_Z_CAP(strSize) char* pStr, int strSize);

bool V_IsAbsolutePath(IN_Z const char* pPath);

bool V_StrSubst(IN_Z const char* pIn, IN_Z const char* pMatch, const char* pReplaceWith,
	OUT_Z_CAP(outLen) char* pOut, int outLen, bool bCaseSensitive = false);

void V_SplitString(IN_Z const char* pString, IN_Z const char* pSeparator, CUtlVector<char*, CUtlMemory<char*, int> >& outStrings);

void V_SplitString2(IN_Z const char* pString, const char** pSeparators, int nSeparators, CUtlVector<char*, CUtlMemory<char*, int> >& outStrings);

bool V_GetCurrentDirectory(OUT_Z_CAP(maxLen) char* pOut, int maxLen);

bool V_SetCurrentDirectory(const char* pDirName);


void V_StrSlice(const char* pStr, int firstChar, int lastCharNonInclusive, OUT_Z_CAP(outSize) char* pOut, int outSize);

void V_StrLeft(const char* pStr, int nChars, OUT_Z_CAP(outSize) char* pOut, int outSize);

void V_StrRight(const char* pStr, int nChars, OUT_Z_CAP(outSize) char* pOut, int outSize);

char* V_AddBackSlashesToSpecialChars(char const* pSrc);

void V_FixSlashes(char* pname, char separator = CORRECT_PATH_SEPARATOR);

void V_FixDoubleSlashes(char* pStr);

void V_strtowcs(const char* pString, int nInSize, OUT_Z_BYTECAP(nOutSizeInBytes) wchar_t* pWString, int nOutSizeInBytes);
void V_wcstostr(const wchar_t* pWString, int nInSize, OUT_Z_CAP(nOutSizeInBytes) char* pString, int nOutSizeInBytes);

inline void V_strcat(INOUT_Z_CAP(cchDest) char* dest, const char* src, int cchDest)
{
	V_strncat(dest, src, cchDest, COPY_ALL_CHARACTERS);
}

inline void V_wcscat(INOUT_Z_CAP(cchDest) wchar_t* dest, const wchar_t* src, int cchDest)
{
	V_wcsncat(dest, src, cchDest, COPY_ALL_CHARACTERS);
}

template < class NameArray >
int V_GenerateUniqueNameIndex(const char* prefix, const NameArray& nameArray, int startindex = 0)
{
	if (prefix == NULL)
		return 0;

	int freeindex = startindex;

	int nNames = nameArray.Count();
	for (int i = 0; i < nNames; ++i)
	{
		const char* pName = nameArray[i];
		if (!pName)
			continue;

		const char* pIndexStr = StringAfterPrefix(pName, prefix);
		if (pIndexStr)
		{
			int index = *pIndexStr ? atoi(pIndexStr) : 1;
			if (index >= freeindex)
			{
				freeindex = index + 1;
			}
		}
	}

	return freeindex;
}

template < class NameArray >
bool V_GenerateUniqueName(OUT_Z_CAP(memsize) char* name, int memsize, const char* prefix, const NameArray& nameArray)
{
	if (name == NULL || memsize == 0)
		return false;

	if (prefix == NULL)
	{
		name[0] = '\0';
		return false;
	}

	int prefixLength = V_strlen(prefix);
	if (prefixLength + 1 > memsize)
	{
		name[0] = '\0';
		return false;
	}

	int i = V_GenerateUniqueNameIndex(prefix, nameArray);
	if (i <= 0)
	{
		V_strncpy(name, prefix, memsize);
		return true;
	}

	int newlen = prefixLength + (int)log10((float)i) + 1;
	if (newlen + 1 > memsize)
	{
		V_strncpy(name, prefix, memsize);
		return false;
	}

	V_snprintf(name, memsize, "%s%d", prefix, i);
	return true;
}


class CStrAutoEncode
{
public:

	explicit CStrAutoEncode(const char* pch)
	{
		m_pch = pch;
		m_pwch = NULL;
#if !defined( WIN32 ) && !defined(_WIN32)
		m_pucs2 = NULL;
#endif
		m_bCreatedUTF16 = false;
	}

	explicit CStrAutoEncode(const wchar_t* pwch)
	{
		m_pch = NULL;
		m_pwch = pwch;
#if !defined( WIN32 ) && !defined(_WIN32)
		m_pucs2 = NULL;
#endif
		m_bCreatedUTF16 = true;
	}

#if !defined(WIN32) && !defined(_WINDOWS) && !defined(_WIN32)
	explicit CStrAutoEncode(const ucs2* pwch)
	{
		m_pch = NULL;
		m_pwch = NULL;
		m_pucs2 = pwch;
		m_bCreatedUTF16 = true;
	}
#endif

	const char* ToString()
	{
		PopulateUTF8();
		return m_pch;
	}

	char* ToStringWritable()
	{
		PopulateUTF8();
		return const_cast<char*>(m_pch);
	}

	const wchar_t* ToWString()
	{
		PopulateUTF16();
		return m_pwch;
	}

#if !defined( WIN32 ) && !defined(_WIN32)
	const ucs2* ToUCS2String()
	{
		PopulateUCS2();
		return m_pucs2;
	}
#endif

	wchar_t* ToWStringWritable()
	{
		PopulateUTF16();
		return const_cast<wchar_t*>(m_pwch);
	}

	~CStrAutoEncode()
	{
		if (m_bCreatedUTF16)
		{
			delete[] m_pch;
		}
		else
		{
			delete[] m_pwch;
		}
	}

private:
	void PopulateUTF8()
	{
		if (!m_bCreatedUTF16)
			return;					    
		if (m_pwch == NULL)
			return;					       
		if (m_pch != NULL)
			return;					         

		uint32 cbMax = 4 * static_cast<uint32>(V_wcslen(m_pwch)) + 1;
		char* pchTemp = new char[cbMax];
		if (V_UnicodeToUTF8(m_pwch, pchTemp, cbMax))
		{
			uint32 cchAlloc = static_cast<uint32>(V_strlen(pchTemp)) + 1;
			char* pchHeap = new char[cchAlloc];
			V_strncpy(pchHeap, pchTemp, cchAlloc);
			delete[] pchTemp;
			m_pch = pchHeap;
		}
		else
		{
			delete[] pchTemp;
		}
	}

	void PopulateUTF16()
	{
		if (m_bCreatedUTF16)
			return;					    
		if (m_pch == NULL)
			return;					     
		if (m_pwch != NULL)
			return;					         

		uint32 cchMax = static_cast<uint32>(V_strlen(m_pch)) + 1;
		wchar_t* pwchTemp = new wchar_t[cchMax];
		if (V_UTF8ToUnicode(m_pch, pwchTemp, cchMax * sizeof(wchar_t)))
		{
			uint32 cchAlloc = static_cast<uint32>(V_wcslen(pwchTemp)) + 1;
			wchar_t* pwchHeap = new wchar_t[cchAlloc];
			V_wcsncpy(pwchHeap, pwchTemp, cchAlloc * sizeof(wchar_t));
			delete[] pwchTemp;
			m_pwch = pwchHeap;
		}
		else
		{
			delete[] pwchTemp;
		}
	}

#if !defined( WIN32 ) && !defined(_WIN32)
	void PopulateUCS2()
	{
		if (m_pch == NULL)
			return;					     
		if (m_pucs2 != NULL)
			return;					         

		uint32 cchMax = static_cast<uint32>(V_strlen(m_pch)) + 1;
		ucs2* pwchTemp = new ucs2[cchMax];
		if (V_UTF8ToUCS2(m_pch, cchMax, pwchTemp, cchMax * sizeof(ucs2)))
		{
			uint32 cchAlloc = cchMax;
			ucs2* pwchHeap = new ucs2[cchAlloc];
			memcpy(pwchHeap, pwchTemp, cchAlloc * sizeof(ucs2));
			delete[] pwchTemp;
			m_pucs2 = pwchHeap;
		}
		else
		{
			delete[] pwchTemp;
		}
	}
#endif

	const char* m_pch;
	const wchar_t* m_pwch;
#if !defined( WIN32 ) && !defined(_WIN32)
	const ucs2* m_pucs2;
#endif
	bool m_bCreatedUTF16;

};

void Q_URLEncodeRaw(OUT_Z_CAP(nDestLen) char* pchDest, int nDestLen, const char* pchSource, int nSourceLen);

size_t Q_URLDecodeRaw(OUT_CAP(nDecodeDestLen) char* pchDecodeDest, int nDecodeDestLen, const char* pchEncodedSource, int nEncodedSourceLen);

void Q_URLEncode(OUT_Z_CAP(nDestLen) char* pchDest, int nDestLen, const char* pchSource, int nSourceLen);

size_t Q_URLDecode(OUT_CAP(nDecodeDestLen) char* pchDecodeDest, int nDecodeDestLen, const char* pchEncodedSource, int nEncodedSourceLen);


#if !defined( VSTDLIB_BACKWARD_COMPAT )

#define Q_memset				V_memset
#define Q_memcpy				V_memcpy
#define Q_memmove				V_memmove
#define Q_memcmp				V_memcmp
#define Q_strlen				V_strlen
#define Q_strcpy				V_strcpy
#define Q_strrchr				V_strrchr
#define Q_strcmp				V_strcmp
#define Q_wcscmp				V_wcscmp
#define Q_stricmp				V_stricmp
#define Q_strstr				V_strstr
#define Q_strupr				V_strupr
#define Q_strlower				V_strlower
#define Q_wcslen				V_wcslen
#define	Q_strncmp				V_strncmp 
#define	Q_strcasecmp			V_strcasecmp
#define	Q_strncasecmp			V_strncasecmp
#define	Q_strnicmp				V_strnicmp
#define	Q_atoi					V_atoi
#define	Q_atoi64				V_atoi64
#define Q_atoui64				V_atoui64
#define	Q_atof					V_atof
#define	Q_stristr				V_stristr
#define	Q_strnistr				V_strnistr
#define	Q_strnchr				V_strnchr
#define Q_normalizeFloatString	V_normalizeFloatString
#define Q_strncpy				V_strncpy
#define Q_snprintf				V_snprintf
#define Q_snwprintf				V_snwprintf 
#define Q_wcsncpy				V_wcsncpy
#define Q_strncat				V_strncat
#define Q_strnlwr				V_strnlwr
#define Q_vsnprintf				V_vsnprintf
#define Q_vsnprintfRet			V_vsnprintfRet
#define Q_pretifymem			V_pretifymem
#define Q_pretifynum			V_pretifynum
#define Q_UTF8ToUnicode			V_UTF8ToUnicode
#define Q_UnicodeToUTF8			V_UnicodeToUTF8
#define Q_hextobinary			V_hextobinary
#define Q_binarytohex			V_binarytohex
#define Q_FileBase				V_FileBase
#define Q_StripTrailingSlash	V_StripTrailingSlash
#define Q_StripExtension		V_StripExtension
#define	Q_DefaultExtension		V_DefaultExtension
#define Q_SetExtension			V_SetExtension
#define Q_StripFilename			V_StripFilename
#define Q_StripLastDir			V_StripLastDir
#define Q_UnqualifiedFileName	V_UnqualifiedFileName
#define Q_ComposeFileName		V_ComposeFileName
#define Q_ExtractFilePath		V_ExtractFilePath
#define Q_ExtractFileExtension	V_ExtractFileExtension
#define Q_GetFileExtension		V_GetFileExtension
#define Q_RemoveDotSlashes		V_RemoveDotSlashes
#define Q_MakeAbsolutePath		V_MakeAbsolutePath
#define Q_AppendSlash			V_AppendSlash
#define Q_IsAbsolutePath		V_IsAbsolutePath
#define Q_StrSubst				V_StrSubst
#define Q_SplitString			V_SplitString
#define Q_SplitString2			V_SplitString2
#define Q_StrSlice				V_StrSlice
#define Q_StrLeft				V_StrLeft
#define Q_StrRight				V_StrRight
#define Q_FixSlashes			V_FixSlashes
#define Q_strtowcs				V_strtowcs
#define Q_wcstostr				V_wcstostr
#define Q_strcat				V_strcat
#define Q_GenerateUniqueNameIndex	V_GenerateUniqueNameIndex
#define Q_GenerateUniqueName		V_GenerateUniqueName
#define Q_MakeRelativePath		V_MakeRelativePath
#define Q_qsort_s				V_qsort_s

#endif    


#endif	 