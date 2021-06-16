#ifndef UTLBUFFER_H
#define UTLBUFFER_H

#ifdef _WIN32
#pragma once
#endif

#include "utlmemory.h"
#include "byteswap.h"
#include <stdarg.h>


struct characterset_t;


class CUtlCharConversion
{
public:
	struct ConversionArray_t
	{
		char m_nActualChar;
		char* m_pReplacementString;
	};

	CUtlCharConversion(char nEscapeChar, const char* pDelimiter, int nCount, ConversionArray_t* pArray);
	char GetEscapeChar() const;
	const char* GetDelimiter() const;
	int GetDelimiterLength() const;

	const char* GetConversionString(char c) const;
	int GetConversionLength(char c) const;
	int MaxConversionLength() const;

	virtual char FindConversion(const char* pString, int* pLength);

protected:
	struct ConversionInfo_t
	{
		int m_nLength;
		char* m_pReplacementString;
	};

	char m_nEscapeChar;
	const char* m_pDelimiter;
	int m_nDelimiterLength;
	int m_nCount;
	int m_nMaxConversionLength;
	char m_pList[256];
	ConversionInfo_t m_pReplacements[256];
};

#define BEGIN_CHAR_CONVERSION( _name, _delimiter, _escapeChar )	\
	static CUtlCharConversion::ConversionArray_t s_pConversionArray ## _name[] = {

#define END_CHAR_CONVERSION( _name, _delimiter, _escapeChar ) \
	}; \
	CUtlCharConversion _name( _escapeChar, _delimiter, sizeof( s_pConversionArray ## _name ) / sizeof( CUtlCharConversion::ConversionArray_t ), s_pConversionArray ## _name );

#define BEGIN_CUSTOM_CHAR_CONVERSION( _className, _name, _delimiter, _escapeChar ) \
	static CUtlCharConversion::ConversionArray_t s_pConversionArray ## _name[] = {

#define END_CUSTOM_CHAR_CONVERSION( _className, _name, _delimiter, _escapeChar ) \
	}; \
	_className _name( _escapeChar, _delimiter, sizeof( s_pConversionArray ## _name ) / sizeof( CUtlCharConversion::ConversionArray_t ), s_pConversionArray ## _name );

CUtlCharConversion* GetCStringCharConversion();

CUtlCharConversion* GetNoEscCharConversion();


#define SetUtlBufferOverflowFuncs( _get, _put )	\
	SetOverflowFuncs( static_cast <UtlBufferOverflowFunc_t>( _get ), static_cast <UtlBufferOverflowFunc_t>( _put ) )



typedef unsigned short ushort;

template < class A >
static const char* GetFmtStr(int nRadix = 10, bool bPrint = true) { Assert(0); return ""; }
#if defined( LINUX ) || defined( __clang__ ) || ( defined( _MSC_VER ) && _MSC_VER >= 1900 )
template <> const char* GetFmtStr< short >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%hd"; }
template <> const char* GetFmtStr< ushort >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%hu"; }
template <> const char* GetFmtStr< int >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%d"; }
template <> const char* GetFmtStr< uint >(int nRadix, bool bPrint) { Assert(nRadix == 10 || nRadix == 16); return nRadix == 16 ? "%x" : "%u"; }
template <> const char* GetFmtStr< int64 >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%lld"; }
template <> const char* GetFmtStr< float >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%f"; }
template <> const char* GetFmtStr< double >(int nRadix, bool bPrint) { Assert(nRadix == 10); return bPrint ? "%.15lf" : "%lf"; }               
#else
template <> static const char* GetFmtStr< short >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%hd"; }
template <> static const char* GetFmtStr< ushort >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%hu"; }
template <> static const char* GetFmtStr< int >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%d"; }
template <> static const char* GetFmtStr< uint >(int nRadix, bool bPrint) { Assert(nRadix == 10 || nRadix == 16); return nRadix == 16 ? "%x" : "%u"; }
template <> static const char* GetFmtStr< int64 >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%lld"; }
template <> static const char* GetFmtStr< float >(int nRadix, bool bPrint) { Assert(nRadix == 10); return "%f"; }
template <> static const char* GetFmtStr< double >(int nRadix, bool bPrint) { Assert(nRadix == 10); return bPrint ? "%.15lf" : "%lf"; }               
#endif
class CUtlBuffer
{
public:
	enum SeekType_t
	{
		SEEK_HEAD = 0,
		SEEK_CURRENT,
		SEEK_TAIL
	};

	enum BufferFlags_t
	{
		TEXT_BUFFER = 0x1,			          
		EXTERNAL_GROWABLE = 0x2,	                     
		CONTAINS_CRLF = 0x4,		          
		READ_ONLY = 0x8,			        
		AUTO_TABS_DISABLED = 0x10,	     
	};

	typedef bool (CUtlBuffer::* UtlBufferOverflowFunc_t)(int nSize);

	CUtlBuffer(int growSize = 0, int initSize = 0, int nFlags = 0);
	CUtlBuffer(const void* pBuffer, int size, int nFlags = 0);
	CUtlBuffer(const void* pBuffer, int size, bool crap) = delete;

	CUtlBuffer(const CUtlBuffer&);   
	CUtlBuffer& operator= (const CUtlBuffer&);   

#if VALVE_CPP11
	CUtlBuffer(CUtlBuffer&& moveFrom);   
	CUtlBuffer& operator= (CUtlBuffer&& moveFrom);   
#endif

	unsigned char	GetFlags() const;

	void			SetBufferType(bool bIsText, bool bContainsCRLF);

	void			EnsureCapacity(int num);

	void* AccessForDirectRead(int nBytes);

	void			SetExternalBuffer(void* pMemory, int nSize, int nInitialPut, int nFlags = 0);
	bool			IsExternallyAllocated() const;
	void			AssumeMemory(void* pMemory, int nSize, int nInitialPut, int nFlags = 0);
	void* Detach();
	void* DetachMemory();

	void			CopyBuffer(const CUtlBuffer& buffer);
	void			CopyBuffer(const void* pubData, int cubData);

	void			Swap(CUtlBuffer& buf);
	void			Swap(CUtlMemory<uint8>& mem);


	FORCEINLINE void ActivateByteSwappingIfBigEndian(void)
	{
		if ((IsX360() || IsPS3()))
			ActivateByteSwapping(true);
	}


	void			ActivateByteSwapping(bool bActivate);
	void			SetBigEndian(bool bigEndian);
	bool			IsBigEndian(void);

	void			Clear();

	void			Purge();

	void			Spew();

	char			GetChar();
	unsigned char	GetUnsignedChar();
	short			GetShort();
	unsigned short	GetUnsignedShort();
	int				GetInt();
	int64			GetInt64();
	unsigned int	GetIntHex();
	unsigned int	GetUnsignedInt();
	uint64			GetUnsignedInt64();
	float			GetFloat();
	double			GetDouble();
	void* GetPtr();
	void			GetString(char* pString, int nMaxChars);
	void			Get(void* pMem, int size);
	void			GetLine(char* pLine, int nMaxChars);

	template <typename T> void GetObjects(T* dest, int count = 1);

	int				GetUpTo(void* pMem, int nSize);

	void			GetDelimitedString(CUtlCharConversion* pConv, char* pString, int nMaxChars = 0);
	char			GetDelimitedChar(CUtlCharConversion* pConv);

	int				PeekStringLength();

	int				PeekDelimitedStringLength(CUtlCharConversion* pConv, bool bActualSize = true);

	int				Scanf(SCANF_FORMAT_STRING const char* pFmt, ...);
	int				VaScanf(const char* pFmt, va_list list);

	void			EatWhiteSpace();

	bool			EatCPPComment();

	bool			ParseToken(const char* pStartingDelim, const char* pEndingDelim, char* pString, int nMaxLen);

	bool			GetToken(const char* pToken);

	int				ParseToken(characterset_t* pBreaks, char* pTokenBuf, int nMaxLen, bool bParseComments = true);

	void			PutChar(char c);
	void			PutUnsignedChar(unsigned char uc);
	void			PutShort(short s);
	void			PutUnsignedShort(unsigned short us);
	void			PutInt(int i);
	void			PutInt64(int64 i);
	void			PutUnsignedInt(unsigned int u);
	void			PutUnsignedInt64(uint64 u);
	void			PutFloat(float f);
	void			PutDouble(double d);
	void			PutPtr(void*);        
	void			PutString(const char* pString);
	void			Put(const void* pMem, int size);

	template <typename T> void PutObjects(T* src, int count = 1);

	void			PutDelimitedString(CUtlCharConversion* pConv, const char* pString);
	void			PutDelimitedChar(CUtlCharConversion* pConv, char c);

	void			Printf(PRINTF_FORMAT_STRING const char* pFmt, ...) FMTFUNCTION(2, 3);
	void			VaPrintf(const char* pFmt, va_list list);

	void* PeekPut(int offset = 0);
	const void* PeekGet(int offset = 0) const;
	const void* PeekGet(int nMaxSize, int nOffset);

	int TellPut() const;
	int TellGet() const;

	int TellMaxPut() const;

	int GetBytesRemaining() const;

	void SeekPut(SeekType_t type, int offset);
	void SeekGet(SeekType_t type, int offset);

	const void* Base() const;
	void* Base();

	int Size() const;

	bool IsText() const;

	bool IsGrowable() const;

	bool IsValid() const;

	bool ContainsCRLF() const;

	bool IsReadOnly() const;

	bool ConvertCRLF(CUtlBuffer& outBuf);

	void PushTab();
	void PopTab();

	void EnableTabs(bool bEnable);

#if !defined( _GAMECONSOLE )
	void SwapCopy(CUtlBuffer& other);
#endif

protected:
	enum
	{
		PUT_OVERFLOW = 0x1,
		GET_OVERFLOW = 0x2,
		MAX_ERROR_FLAG = GET_OVERFLOW,
	};

	void SetOverflowFuncs(UtlBufferOverflowFunc_t getFunc, UtlBufferOverflowFunc_t putFunc);

	bool OnPutOverflow(int nSize);
	bool OnGetOverflow(int nSize);

protected:
	bool CheckPut(int size);
	bool CheckGet(int size);

	void AddNullTermination(int nPut);
	bool WasLastCharacterCR();
	void PutTabs();

	char GetDelimitedCharInternal(CUtlCharConversion* pConv);
	void PutDelimitedCharInternal(CUtlCharConversion* pConv, char c);

	bool PutOverflow(int nSize);
	bool GetOverflow(int nSize);

	bool PeekStringMatch(int nOffset, const char* pString, int nLen);

	int	PeekLineLength();

	int PeekWhiteSpace(int nOffset);

	bool CheckPeekGet(int nOffset, int nSize);

	bool CheckArbitraryPeekGet(int nOffset, int& nIncrement);

	template <typename T> void GetType(T& dest);
	template <typename T> void GetTypeBin(T& dest);
	template <typename T> bool GetTypeText(T& value, int nRadix = 10);
	template <typename T> void GetObject(T* src);

	template <typename T> void PutType(T src);
	template <typename T> void PutTypeBin(T src);
	template <typename T> void PutObject(T* src);

	CUtlMemory<unsigned char> m_Memory;
	int m_Get;
	int m_Put;

	unsigned char m_Error;
	unsigned char m_Flags;
	unsigned char m_Reserved;
#if defined( _GAMECONSOLE )
	unsigned char pad;
#endif

	int m_nTab;
	int m_nMaxPut;
	int m_nOffset;

	UtlBufferOverflowFunc_t m_GetOverflowFunc;
	UtlBufferOverflowFunc_t m_PutOverflowFunc;

	CByteswap	m_Byteswap;
};


inline CUtlBuffer& operator<<(CUtlBuffer& b, char v)
{
	b.PutChar(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, unsigned char v)
{
	b.PutUnsignedChar(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, short v)
{
	b.PutShort(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, unsigned short v)
{
	b.PutUnsignedShort(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, int v)
{
	b.PutInt(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, unsigned int v)
{
	b.PutUnsignedInt(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, float v)
{
	b.PutFloat(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, double v)
{
	b.PutDouble(v);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, const char* pv)
{
	b.PutString(pv);
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, const Vector& v)
{
	b << v.x << " " << v.y << " " << v.z;
	return b;
}

inline CUtlBuffer& operator<<(CUtlBuffer& b, const Vector2D& v)
{
	b << v.x << " " << v.y;
	return b;
}


class CUtlInplaceBuffer : public CUtlBuffer
{
public:
	CUtlInplaceBuffer(int growSize = 0, int initSize = 0, int nFlags = 0);

public:
	bool InplaceGetLinePtr(    char** ppszInBufferPtr,    int* pnLineLength);

	char* InplaceGetLinePtr(void);
};


inline int CUtlBuffer::TellGet() const
{
	return m_Get;
}


inline int CUtlBuffer::GetBytesRemaining() const
{
	return m_nMaxPut - TellGet();
}


inline const void* CUtlBuffer::PeekGet(int offset) const
{
	return &m_Memory[m_Get + offset - m_nOffset];
}


template <typename T>
inline void CUtlBuffer::GetObject(T* dest)
{
	if (CheckGet(sizeof(T)))
	{
		if (!m_Byteswap.IsSwappingBytes() || (sizeof(T) == 1))
		{
			*dest = *(T*)PeekGet();
		}
		else
		{
			m_Byteswap.SwapFieldsToTargetEndian<T>(dest, (T*)PeekGet());
		}
		m_Get += sizeof(T);
	}
	else
	{
		Q_memset(&dest, 0, sizeof(T));
	}
}


template <typename T>
inline void CUtlBuffer::GetObjects(T* dest, int count)
{
	for (int i = 0; i < count; ++i, ++dest)
	{
		GetObject<T>(dest);
	}
}


template <typename T>
inline void CUtlBuffer::GetTypeBin(T& dest)
{
	if (CheckGet(sizeof(T)))
	{
		if (!m_Byteswap.IsSwappingBytes() || (sizeof(T) == 1))
		{
			dest = *(T*)PeekGet();
		}
		else
		{
			m_Byteswap.SwapBufferToTargetEndian<T>(&dest, (T*)PeekGet());
		}
		m_Get += sizeof(T);
	}
	else
	{
		dest = 0;
	}
}

template <>
inline void CUtlBuffer::GetTypeBin< float >(float& dest)
{
	if (CheckGet(sizeof(float)))
	{
		uintp pData = (uintp)PeekGet();
		if ((IsX360() || IsPS3()) && (pData & 0x03))
		{
			((unsigned char*)&dest)[0] = ((unsigned char*)pData)[0];
			((unsigned char*)&dest)[1] = ((unsigned char*)pData)[1];
			((unsigned char*)&dest)[2] = ((unsigned char*)pData)[2];
			((unsigned char*)&dest)[3] = ((unsigned char*)pData)[3];
		}
		else
		{
			dest = *(float*)pData;
		}
		if (m_Byteswap.IsSwappingBytes())
		{
			m_Byteswap.SwapBufferToTargetEndian< float >(&dest, &dest);
		}
		m_Get += sizeof(float);
	}
	else
	{
		dest = 0;
	}
}

template <>
inline void CUtlBuffer::GetTypeBin< double >(double& dest)
{
	if (CheckGet(sizeof(double)))
	{
		uintp pData = (uintp)PeekGet();
		if ((IsX360() || IsPS3()) && (pData & 0x07))
		{
			((unsigned char*)&dest)[0] = ((unsigned char*)pData)[0];
			((unsigned char*)&dest)[1] = ((unsigned char*)pData)[1];
			((unsigned char*)&dest)[2] = ((unsigned char*)pData)[2];
			((unsigned char*)&dest)[3] = ((unsigned char*)pData)[3];
			((unsigned char*)&dest)[4] = ((unsigned char*)pData)[4];
			((unsigned char*)&dest)[5] = ((unsigned char*)pData)[5];
			((unsigned char*)&dest)[6] = ((unsigned char*)pData)[6];
			((unsigned char*)&dest)[7] = ((unsigned char*)pData)[7];
		}
		else
		{
			dest = *(double*)pData;
		}
		if (m_Byteswap.IsSwappingBytes())
		{
			m_Byteswap.SwapBufferToTargetEndian< double >(&dest, &dest);
		}
		m_Get += sizeof(double);
	}
	else
	{
		dest = 0;
	}
}

template < class T >
inline T StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	Assert(0);
	*ppEnd = pString;
	return 0;
}

template <>
inline int8 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	return (int8)strtol(pString, ppEnd, nRadix);
}

template <>
inline uint8 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	return (uint8)strtoul(pString, ppEnd, nRadix);
}

template <>
inline int16 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	return (int16)strtol(pString, ppEnd, nRadix);
}

template <>
inline uint16 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	return (uint16)strtoul(pString, ppEnd, nRadix);
}

template <>
inline int32 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	return (int32)strtol(pString, ppEnd, nRadix);
}

template <>
inline uint32 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	return (uint32)strtoul(pString, ppEnd, nRadix);
}

template <>
inline int64 StringToNumber(char* pString, char** ppEnd, int nRadix)
{
#if defined(_PS3) || defined(POSIX)
	return (int64)strtoll(pString, ppEnd, nRadix);
#else  
	return (int64)_strtoi64(pString, ppEnd, nRadix);
#endif  
}

template <>
inline float StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	NOTE_UNUSED(nRadix);
	return (float)strtod(pString, ppEnd);
}

template <>
inline double StringToNumber(char* pString, char** ppEnd, int nRadix)
{
	NOTE_UNUSED(nRadix);
	return (double)strtod(pString, ppEnd);
}

template <typename T>
inline bool CUtlBuffer::GetTypeText(T& value, int nRadix  )
{
	int nLength = 128;
	if (!CheckArbitraryPeekGet(0, nLength))
	{
		value = 0;
		return false;
	}

	char* pStart = (char*)PeekGet();
	char* pEnd = pStart;
	value = StringToNumber< T >(pStart, &pEnd, nRadix);

	int nBytesRead = (int)(pEnd - pStart);
	if (nBytesRead == 0)
		return false;

	m_Get += nBytesRead;
	return true;
}

template <typename T>
inline void CUtlBuffer::GetType(T& dest)
{
	if (!IsText())
	{
		GetTypeBin(dest);
	}
	else
	{
		GetTypeText(dest);
	}
}

inline char CUtlBuffer::GetChar()
{
	char c;
	GetTypeBin(c);     
	return c;
}

inline unsigned char CUtlBuffer::GetUnsignedChar()
{
	unsigned char c;
	if (!IsText())
	{
		GetTypeBin(c);
	}
	else
	{
		c = (unsigned char)GetUnsignedShort();
	}
	return c;
}

inline short CUtlBuffer::GetShort()
{
	short s;
	GetType(s);
	return s;
}

inline unsigned short CUtlBuffer::GetUnsignedShort()
{
	unsigned short s;
	GetType(s);
	return s;
}

inline int CUtlBuffer::GetInt()
{
	int i;
	GetType(i);
	return i;
}

inline int64 CUtlBuffer::GetInt64()
{
	int64 i;
	GetType(i);
	return i;
}

inline unsigned int CUtlBuffer::GetIntHex()
{
	uint i;
	if (!IsText())
	{
		GetTypeBin(i);
	}
	else
	{
		GetTypeText(i, 16);
	}
	return i;
}

inline unsigned int CUtlBuffer::GetUnsignedInt()
{
	unsigned int i;
	GetType(i);
	return i;
}

inline uint64 CUtlBuffer::GetUnsignedInt64()
{
	uint64 i;
	GetType(i);
	return i;
}


inline float CUtlBuffer::GetFloat()
{
	float f;
	GetType(f);
	return f;
}

inline double CUtlBuffer::GetDouble()
{
	double d;
	GetType(d);
	return d;
}

inline void* CUtlBuffer::GetPtr()
{
	void* p;
#if !defined(X64BITS) && !defined(PLATFORM_64BITS)
	p = (void*)GetUnsignedInt();
#else
	p = (void*)GetInt64();
#endif
	return p;
}

inline unsigned char CUtlBuffer::GetFlags() const
{
	return m_Flags;
}


inline bool CUtlBuffer::IsExternallyAllocated() const
{
	return m_Memory.IsExternallyAllocated();
}


inline int CUtlBuffer::TellPut() const
{
	return m_Put;
}


inline int CUtlBuffer::TellMaxPut() const
{
	return m_nMaxPut;
}


inline void* CUtlBuffer::PeekPut(int offset)
{
	return &m_Memory[m_Put + offset - m_nOffset];
}


template <typename T>
inline void CUtlBuffer::PutObject(T* src)
{
	if (CheckPut(sizeof(T)))
	{
		if (!m_Byteswap.IsSwappingBytes() || (sizeof(T) == 1))
		{
			*(T*)PeekPut() = *src;
		}
		else
		{
			m_Byteswap.SwapFieldsToTargetEndian<T>((T*)PeekPut(), src);
		}
		m_Put += sizeof(T);
		AddNullTermination(m_Put);
	}
}


template <typename T>
inline void CUtlBuffer::PutObjects(T* src, int count)
{
	for (int i = 0; i < count; ++i, ++src)
	{
		PutObject<T>(src);
	}
}


template <typename T>
inline void CUtlBuffer::PutTypeBin(T src)
{
	if (CheckPut(sizeof(T)))
	{
		if (!m_Byteswap.IsSwappingBytes() || (sizeof(T) == 1))
		{
			*(T*)PeekPut() = src;
		}
		else
		{
			m_Byteswap.SwapBufferToTargetEndian<T>((T*)PeekPut(), &src);
		}
		m_Put += sizeof(T);
		AddNullTermination(m_Put);
	}
}

#if defined( _GAMECONSOLE )
template <>
inline void CUtlBuffer::PutTypeBin< float >(float src)
{
	if (CheckPut(sizeof(src)))
	{
		if (m_Byteswap.IsSwappingBytes())
		{
			m_Byteswap.SwapBufferToTargetEndian<float>(&src, &src);
		}

		unsigned pData = (unsigned)PeekPut();
		if (pData & 0x03)
		{
			byte* dst = (byte*)pData;
			byte* srcPtr = (byte*)&src;
			dst[0] = srcPtr[0];
			dst[1] = srcPtr[1];
			dst[2] = srcPtr[2];
			dst[3] = srcPtr[3];
		}
		else
		{
			*(float*)pData = src;
		}

		m_Put += sizeof(float);
		AddNullTermination(m_Put);
	}
}

template <>
inline void CUtlBuffer::PutTypeBin< double >(double src)
{
	if (CheckPut(sizeof(src)))
	{
		if (m_Byteswap.IsSwappingBytes())
		{
			m_Byteswap.SwapBufferToTargetEndian<double>(&src, &src);
		}

		unsigned pData = (unsigned)PeekPut();
		if (pData & 0x07)
		{
			byte* dst = (byte*)pData;
			byte* srcPtr = (byte*)&src;
			dst[0] = srcPtr[0];
			dst[1] = srcPtr[1];
			dst[2] = srcPtr[2];
			dst[3] = srcPtr[3];
			dst[4] = srcPtr[4];
			dst[5] = srcPtr[5];
			dst[6] = srcPtr[6];
			dst[7] = srcPtr[7];
		}
		else
		{
			*(double*)pData = src;
		}

		m_Put += sizeof(double);
		AddNullTermination(m_Put);
	}
}
#endif

template <typename T>
inline void CUtlBuffer::PutType(T src)
{
	if (!IsText())
	{
		PutTypeBin(src);
	}
	else
	{
		Printf(GetFmtStr< T >(), src);
	}
}

inline bool CUtlBuffer::WasLastCharacterCR()
{
	if (!IsText() || (TellPut() == 0))
		return false;
	return (*(const char*)PeekPut(-1) == '\n');
}

inline void CUtlBuffer::PutTabs()
{
	int nTabCount = (m_Flags & AUTO_TABS_DISABLED) ? 0 : m_nTab;
	for (int i = nTabCount; --i >= 0; )
	{
		PutTypeBin<char>('\t');
	}
}


inline void CUtlBuffer::PushTab()
{
	++m_nTab;
}

inline void CUtlBuffer::PopTab()
{
	if (--m_nTab < 0)
	{
		m_nTab = 0;
	}
}


inline void CUtlBuffer::EnableTabs(bool bEnable)
{
	if (bEnable)
	{
		m_Flags &= ~AUTO_TABS_DISABLED;
	}
	else
	{
		m_Flags |= AUTO_TABS_DISABLED;
	}
}

inline void CUtlBuffer::PutChar(char c)
{
	if (WasLastCharacterCR())
	{
		PutTabs();
	}

	PutTypeBin(c);
}

inline void CUtlBuffer::PutUnsignedChar(unsigned char c)
{
	if (!IsText())
	{
		PutTypeBin(c);
	}
	else
	{
		PutUnsignedShort(c);
	}
}

inline void  CUtlBuffer::PutShort(short s)
{
	PutType(s);
}

inline void CUtlBuffer::PutUnsignedShort(unsigned short s)
{
	PutType(s);
}

inline void CUtlBuffer::PutInt(int i)
{
	PutType(i);
}

inline void CUtlBuffer::PutInt64(int64 i)
{
	PutType(i);
}

inline void CUtlBuffer::PutUnsignedInt(unsigned int u)
{
	PutType(u);
}

inline void CUtlBuffer::PutUnsignedInt64(uint64 i)
{
	PutType(i);
}


inline void CUtlBuffer::PutFloat(float f)
{
	PutType(f);
}

inline void CUtlBuffer::PutDouble(double d)
{
	PutType(d);
}

inline void CUtlBuffer::PutPtr(void* p)
{
	if (!IsText())
	{
		PutTypeBin(p);
	}
	else
	{
		Printf("0x%p", p);
	}
}

inline bool CUtlBuffer::IsText() const
{
	return (m_Flags & TEXT_BUFFER) != 0;
}


inline bool CUtlBuffer::IsGrowable() const
{
	return (m_Flags & EXTERNAL_GROWABLE) != 0;
}


inline bool CUtlBuffer::IsValid() const
{
	return m_Error == 0;
}


inline bool CUtlBuffer::ContainsCRLF() const
{
	return IsText() && ((m_Flags & CONTAINS_CRLF) != 0);
}


inline bool CUtlBuffer::IsReadOnly() const
{
	return (m_Flags & READ_ONLY) != 0;
}


inline const void* CUtlBuffer::Base() const
{
	return m_Memory.Base();
}

inline void* CUtlBuffer::Base()
{
	return m_Memory.Base();
}

inline int CUtlBuffer::Size() const
{
	return m_Memory.NumAllocated();
}


inline void CUtlBuffer::Clear()
{
	m_Get = 0;
	m_Put = 0;
	m_Error = 0;
	m_nOffset = 0;
	m_nMaxPut = -1;
	AddNullTermination(m_Put);
}

inline void CUtlBuffer::Purge()
{
	m_Get = 0;
	m_Put = 0;
	m_nOffset = 0;
	m_nMaxPut = 0;
	m_Error = 0;
	m_Memory.Purge();
}

inline void* CUtlBuffer::AccessForDirectRead(int nBytes)
{
	Assert(m_Get == 0 && m_Put == 0 && m_nMaxPut == 0);
	EnsureCapacity(nBytes);
	m_nMaxPut = nBytes;
	return Base();
}

inline void* CUtlBuffer::Detach()
{
	void* p = m_Memory.Detach();
	Clear();
	return p;
}

inline void CUtlBuffer::Spew()
{
	SeekGet(CUtlBuffer::SEEK_HEAD, 0);

	char pTmpLine[1024];
	while (IsValid() && GetBytesRemaining())
	{
		V_memset(pTmpLine, 0, sizeof(pTmpLine));
		Get(pTmpLine, MIN((size_t)GetBytesRemaining(), sizeof(pTmpLine) - 1));
		Msg(_T("%s"), pTmpLine);
	}
}

#if !defined(_GAMECONSOLE)
inline void CUtlBuffer::SwapCopy(CUtlBuffer& other)
{
	m_Get = other.m_Get;
	m_Put = other.m_Put;
	m_Error = other.m_Error;
	m_Flags = other.m_Flags;
	m_Reserved = other.m_Reserved;
	m_nTab = other.m_nTab;
	m_nMaxPut = other.m_nMaxPut;
	m_nOffset = other.m_nOffset;
	m_GetOverflowFunc = other.m_GetOverflowFunc;
	m_PutOverflowFunc = other.m_PutOverflowFunc;
	m_Byteswap = other.m_Byteswap;

	m_Memory.Swap(other.m_Memory);
}
#endif

inline void CUtlBuffer::CopyBuffer(const CUtlBuffer& buffer)
{
	CopyBuffer(buffer.Base(), buffer.TellPut());
}

inline void	CUtlBuffer::CopyBuffer(const void* pubData, int cubData)
{
	Clear();
	if (cubData)
	{
		Put(pubData, cubData);
	}
}

#endif  