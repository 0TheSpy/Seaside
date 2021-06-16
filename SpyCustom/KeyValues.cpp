#include "keyvalues.h"
#include "IKeyValuesSystem.h"

#include "Interfaces.hpp"

void V_strncpy(char* pDest, char const* pSrc, int maxLen)
{
    strncpy(pDest, pSrc, maxLen);
    if (maxLen > 0)
    {
        pDest[maxLen - 1] = 0;
    }
}

char* V_strncat(char* pDest, const char* pSrc, size_t destBufferSize, int max_chars_to_copy)
{
	size_t charstocopy = (size_t)0;

	size_t len = strlen(pDest);
	size_t srclen = strlen(pSrc);
	if (max_chars_to_copy <= COPY_ALL_CHARACTERS) 
		charstocopy = srclen; 
	else 
		charstocopy = (size_t)min(max_chars_to_copy, (int)srclen); 

	if (len + charstocopy >= destBufferSize) 
		charstocopy = destBufferSize - len - 1; 

	if ((int)charstocopy <= 0) 
		return pDest; 

	ANALYZE_SUPPRESS(6059);                        
	char* pOut = strncat(pDest, pSrc, charstocopy);
	return pOut;
}


int _V_strcmp(const char* file, int line, const char* s1, const char* s2)
{  
	return strcmp(s1, s2);
}

int V_strncmp(const char* s1, const char* s2, int count)
{
	while (count > 0)
	{
		if (*s1 != *s2)
			return (unsigned char)*s1 < (unsigned char)*s2 ? -1 : 1;   
		if (*s1 == '\0')
			return 0;        
		s1++;
		s2++;
		count--;
	}

	return 0;      
}

int V_stricmp(const char* str1, const char* str2)
{
	if (str1 == str2)
	{
		return 0;
	}
	const unsigned char* s1 = (const unsigned char*)str1;
	const unsigned char* s2 = (const unsigned char*)str2;
	for (; *s1; ++s1, ++s2)
	{
		if (*s1 != *s2)
		{
			unsigned char c1 = *s1 | 0x20;
			unsigned char c2 = *s2 | 0x20;
			if (c1 != c2 || (unsigned char)(c1 - 'a') > ('z' - 'a'))
			{
				if ((c1 | c2) >= 0x80) return stricmp((const char*)s1, (const char*)s2);
				if ((unsigned char)(c1 - 'a') > ('z' - 'a')) c1 = *s1;
				if ((unsigned char)(c2 - 'a') > ('z' - 'a')) c2 = *s2;
				return c1 > c2 ? 1 : -1;
			}
		}
	}
	return *s2 ? -1 : 0;
}

class CLeakTrack
{
public:
    CLeakTrack()
    {
    }
    ~CLeakTrack()
    {
        if (keys.Count() != 0)
        {
            Assert(0);
        }
    }

    struct kve
    {
        KeyValues* kv;
        char		name[256];
    };

    void AddKv(KeyValues* kv, char const* name)
    {
        kve k;
        V_strncpy(k.name, name ? name : "NULL", sizeof(k.name));
        k.kv = kv;

        keys.AddToTail(k);
    }

    void RemoveKv(KeyValues* kv)
    {
        int c = keys.Count();
        for (int i = 0; i < c; i++)
        {
            if (keys[i].kv == kv)
            {
                keys.Remove(i);
                break;
            }
        }
    }

    CUtlVector< kve > keys;
};


static CLeakTrack track;

void KeyValues::Init()
{
    m_iKeyName = 0;
    m_iKeyNameCaseSensitive1 = 0;
    m_iKeyNameCaseSensitive2 = 0;
    m_iDataType = TYPE_NONE;

    m_pSub = NULL;
    m_pPeer = NULL;
    m_pChain = NULL;

    m_sValue = NULL;
    m_wsValue = NULL;
    m_pValue = NULL;

    m_bHasEscapeSequences = 0;
}

void KeyValues::SetName(const char* setName)
{
    HKeySymbol hCaseSensitiveKeyName = INVALID_KEY_SYMBOL, hCaseInsensitiveKeyName = INVALID_KEY_SYMBOL;
    hCaseSensitiveKeyName = iff.keyValuesSystem->GetSymbolForStringCaseSensitive(hCaseInsensitiveKeyName, setName);

    m_iKeyName = hCaseInsensitiveKeyName;
    SPLIT_3_BYTES_INTO_1_AND_2(m_iKeyNameCaseSensitive1, m_iKeyNameCaseSensitive2, hCaseSensitiveKeyName);
}

KeyValues::KeyValues(const char* setName)
{
    TRACK_KV_ADD(this, setName);
    Init();
    SetName(setName);
}

void* KeyValues::operator new(size_t iAllocSize)
{
    MEM_ALLOC_CREDIT();
    return iff.keyValuesSystem->AllocKeyValuesMemory(iAllocSize);
}

void KeyValues::operator delete(void* pMem)
{
	iff.keyValuesSystem->FreeKeyValuesMemory((KeyValues*)pMem);
}

#define TRACK_KV_ADD( ptr, name )	track.AddKv( ptr, name )
#define TRACK_KV_REMOVE( ptr )		track.RemoveKv( ptr )


KeyValues::~KeyValues()
{
	TRACK_KV_REMOVE(this);
	RemoveEverything();
}

void KeyValues::RemoveEverything()
{
	KeyValues* dat;
	KeyValues* datNext = NULL;
	for (dat = m_pSub; dat != NULL; dat = datNext)
	{
		datNext = dat->m_pPeer;
		dat->m_pPeer = NULL;
		delete dat;
	}

	for (dat = m_pPeer; dat && dat != this; dat = datNext)
	{
		datNext = dat->m_pPeer;
		dat->m_pPeer = NULL;
		delete dat;
	}

	delete[] m_sValue;
	m_sValue = NULL;
	delete[] m_wsValue;
	m_wsValue = NULL;
}

char CUtlCharConversion::FindConversion(const char* pString, int* pLength)
{
	for (int i = 0; i < m_nCount; ++i)
	{
		if (!Q_strcmp(pString, m_pReplacements[(unsigned char)m_pList[i]].m_pReplacementString))
		{
			*pLength = m_pReplacements[(unsigned char)m_pList[i]].m_nLength;
			return m_pList[i];
		}
	}

	*pLength = 0;
	return '\0';
}

CUtlCharConversion::CUtlCharConversion(char nEscapeChar, const char* pDelimiter, int nCount, ConversionArray_t* pArray)
{
	m_nEscapeChar = nEscapeChar;
	m_pDelimiter = pDelimiter;
	m_nCount = nCount;
	m_nDelimiterLength = Q_strlen(pDelimiter);
	m_nMaxConversionLength = 0;

	memset(m_pReplacements, 0, sizeof(m_pReplacements));

	for (int i = 0; i < nCount; ++i)
	{
		m_pList[i] = pArray[i].m_nActualChar;
		ConversionInfo_t& info = m_pReplacements[(unsigned char)m_pList[i]];
		Assert(info.m_pReplacementString == 0);
		info.m_pReplacementString = pArray[i].m_pReplacementString;
		info.m_nLength = Q_strlen(info.m_pReplacementString);
		if (info.m_nLength > m_nMaxConversionLength)
		{
			m_nMaxConversionLength = info.m_nLength;
		}
	}
}

bool CUtlBuffer::OnGetOverflow(int nSize)
{
	return (this->*m_GetOverflowFunc)(nSize);
}


class CUtlCStringConversion : public CUtlCharConversion
{
public:
	CUtlCStringConversion(char nEscapeChar, const char* pDelimiter, int nCount, ConversionArray_t* pArray);

	virtual char FindConversion(const char* pString, int* pLength);

private:
	char m_pConversion[256];
};

CUtlCStringConversion::CUtlCStringConversion(char nEscapeChar, const char* pDelimiter, int nCount, ConversionArray_t* pArray) :
	CUtlCharConversion(nEscapeChar, pDelimiter, nCount, pArray)
{
	memset(m_pConversion, 0x0, sizeof(m_pConversion));
	for (int i = 0; i < nCount; ++i)
	{
		m_pConversion[(unsigned char)pArray[i].m_pReplacementString[0]] = pArray[i].m_nActualChar;
	}
}

char CUtlCStringConversion::FindConversion(const char* pString, int* pLength)
{
	char c = m_pConversion[(unsigned char)pString[0]];
	*pLength = (c != '\0') ? 1 : 0;
	return c;
}

bool CUtlBuffer::OnPutOverflow(int nSize)
{
	return (this->*m_PutOverflowFunc)(nSize);
}

void CUtlBuffer::SetOverflowFuncs(UtlBufferOverflowFunc_t getFunc, UtlBufferOverflowFunc_t putFunc)
{
	m_GetOverflowFunc = getFunc;
	m_PutOverflowFunc = putFunc;
}

void CUtlBuffer::Get(void* pMem, int size)
{
	if (size > 0 && CheckGet(size))
	{
		int Index = m_Get - m_nOffset;
		Assert(m_Memory.IsIdxValid(Index) && m_Memory.IsIdxValid(Index + size - 1));

		memcpy(pMem, &m_Memory[Index], size);
		m_Get += size;
	}
}

void CUtlBuffer::GetString(char* pString, int nMaxChars)
{
	if (!IsValid())
	{
		*pString = 0;
		return;
	}

	if (nMaxChars == 0)
	{
		nMaxChars = INT_MAX;
	}

	int nLen = PeekStringLength();

	if (IsText())
	{
		EatWhiteSpace();
	}

	if (nLen == 0)
	{
		*pString = 0;
		m_Error |= GET_OVERFLOW;
		return;
	}

	if (nLen <= nMaxChars)
	{
		Get(pString, nLen - 1);
		pString[nLen - 1] = 0;
	}
	else
	{
		Get(pString, nMaxChars - 1);
		pString[nMaxChars - 1] = 0;
		SeekGet(SEEK_CURRENT, nLen - 1 - nMaxChars);
	}

	if (!IsText())
	{
		VerifyEquals(GetChar(), 0);
	}
}


bool CUtlBuffer::CheckPut(int nSize)
{
	if ((m_Error & PUT_OVERFLOW) || IsReadOnly())
		return false;

	if ((m_Put < m_nOffset) || (m_Memory.NumAllocated() < m_Put - m_nOffset + nSize))
	{
		if (!OnPutOverflow(nSize))
		{
			m_Error |= PUT_OVERFLOW;
			return false;
		}
	}
	return true;
}

char CUtlCharConversion::GetEscapeChar() const
{
	return m_nEscapeChar;
}

int CUtlCharConversion::MaxConversionLength() const
{
	return m_nMaxConversionLength;
}


char CUtlBuffer::GetDelimitedCharInternal(CUtlCharConversion* pConv)
{
	char c = GetChar();
	if (c == pConv->GetEscapeChar())
	{
		int nLength = pConv->MaxConversionLength();
		if (!CheckArbitraryPeekGet(0, nLength))
			return '\0';

		c = pConv->FindConversion((const char*)PeekGet(), &nLength);
		SeekGet(SEEK_CURRENT, nLength);
	}

	return c;
}

bool CUtlBuffer::PutOverflow(int nSize)
{
	MEM_ALLOC_CREDIT();

	if (m_Memory.IsExternallyAllocated())
	{
		if (!IsGrowable())
			return false;

		m_Memory.ConvertToGrowableMemory(0);
	}

	while (Size() < m_Put - m_nOffset + nSize)
	{
		m_Memory.Grow();
	}

	return true;
}

bool CUtlBuffer::GetOverflow(int nSize)
{
	return false;
}

bool CUtlBuffer::PeekStringMatch(int nOffset, const char* pString, int nLen)
{
	if (!CheckPeekGet(nOffset, nLen))
		return false;
	return !Q_strncmp((const char*)PeekGet(nOffset), pString, nLen);
}

int CUtlBuffer::PeekWhiteSpace(int nOffset)
{
	if (!IsText() || !IsValid())
		return 0;

	while (CheckPeekGet(nOffset, sizeof(char)))
	{
		if (!isspace(*(unsigned char*)PeekGet(nOffset)))
			break;
		nOffset += sizeof(char);
	}

	return nOffset;
}

bool CUtlBuffer::CheckArbitraryPeekGet(int nOffset, int& nIncrement)
{
	if (TellGet() + nOffset >= TellMaxPut())
	{
		nIncrement = 0;
		return false;
	}

	if (TellGet() + nOffset + nIncrement > TellMaxPut())
	{
		nIncrement = TellMaxPut() - TellGet() - nOffset;
	}

	CheckPeekGet(nOffset, nIncrement);
	int nMaxGet = TellMaxPut() - TellGet();
	if (nMaxGet < nIncrement)
	{
		nIncrement = nMaxGet;
	}
	return (nIncrement != 0);
}



int	CUtlBuffer::PeekStringLength()
{
	if (!IsValid())
		return 0;

	int nOffset = 0;
	if (IsText())
	{
		nOffset = PeekWhiteSpace(nOffset);
	}

	int nStartingOffset = nOffset;

	do
	{
		int nPeekAmount = 128;

		if (!CheckArbitraryPeekGet(nOffset, nPeekAmount))
		{
			if (nOffset == nStartingOffset)
				return 0;
			return nOffset - nStartingOffset + 1;
		}

		const char* pTest = (const char*)PeekGet(nOffset);

		if (!IsText())
		{
			for (int i = 0; i < nPeekAmount; ++i)
			{
				if (pTest[i] == 0)
					return (i + nOffset - nStartingOffset + 1);
			}
		}
		else
		{
			for (int i = 0; i < nPeekAmount; ++i)
			{
				if (isspace((unsigned char)pTest[i]) || (pTest[i] == 0))
					return (i + nOffset - nStartingOffset + 1);
			}
		}

		nOffset += nPeekAmount;

	} while (true);
}


class CUtlNoEscConversion : public CUtlCharConversion
{
public:
	CUtlNoEscConversion(char nEscapeChar, const char* pDelimiter, int nCount, ConversionArray_t* pArray) :
		CUtlCharConversion(nEscapeChar, pDelimiter, nCount, pArray) {}

	virtual char FindConversion(const char* pString, int* pLength) { *pLength = 0; return 0; }
};

BEGIN_CUSTOM_CHAR_CONVERSION(CUtlCStringConversion, s_StringCharConversion, (char*)"\"", '\\')
	{ '\n', (char*)"n" },
	{ '\t', (char*)"t" },
	{ '\v', (char*)"v" },
	{ '\b', (char*)"b" },
	{ '\r', (char*)"r" },
	{ '\f', (char*)"f" },
	{ '\a', (char*)"a" },
	{ '\\', (char*)"\\" },
	{ '\?', (char*)"\?" },
	{ '\'', (char*)"\'" },
	{ '\"', (char*)"\"" },
END_CUSTOM_CHAR_CONVERSION(CUtlCStringConversion, s_StringCharConversion, (char*)"\"", '\\')

CUtlCharConversion* GetCStringCharConversion()
{
		return &s_StringCharConversion;
}

BEGIN_CUSTOM_CHAR_CONVERSION(CUtlNoEscConversion, s_NoEscConversion, (char*)"\"", 0x7F)
	{
		0x7F, (char*)""
	},
END_CUSTOM_CHAR_CONVERSION(CUtlNoEscConversion, s_NoEscConversion, (char*)"\"", 0x7F)

CUtlCharConversion* GetNoEscCharConversion()
{
	return &s_NoEscConversion;
}


const char* CUtlCharConversion::GetDelimiter() const
{
	return m_pDelimiter;
}

int CUtlCharConversion::GetDelimiterLength() const
{
	return m_nDelimiterLength;
}

void CUtlBuffer::AddNullTermination(int m)
{
	if (m_Put > m_nMaxPut)
	{
		if (!IsReadOnly() && ((m_Error & PUT_OVERFLOW) == 0))
		{
			if (CheckPut(1))
			{
				int Index = m_Put - m_nOffset;
				Assert(m_Memory.IsIdxValid(Index));
				if (Index >= 0)
				{
					m_Memory[Index] = 0;
				}
			}
			else
			{
				m_Error &= ~PUT_OVERFLOW;
			}
		}
		m_nMaxPut = m_Put;
	}
}

CUtlBuffer::CUtlBuffer(int growSize, int initSize, int nFlags) :
	m_Error(0)
{
	MEM_ALLOC_CREDIT();
	m_Memory.Init(growSize, initSize);
	m_Get = 0;
	m_Put = 0;
	m_nTab = 0;
	m_nOffset = 0;
	m_Flags = nFlags;
	if ((initSize != 0) && !IsReadOnly())
	{
		m_nMaxPut = -1;
		AddNullTermination(0);
	}
	else
	{
		m_nMaxPut = 0;
	}
	SetOverflowFuncs(&CUtlBuffer::GetOverflow, &CUtlBuffer::PutOverflow);
}

CUtlBuffer::CUtlBuffer(const void* pBuffer, int nSize, int nFlags) :
	m_Memory((unsigned char*)pBuffer, nSize), m_Error(0)
{
	Assert(nSize != 0);

	m_Get = 0;
	m_Put = 0;
	m_nTab = 0;
	m_nOffset = 0;
	m_Flags = nFlags;
	if (IsReadOnly())
	{
		m_nMaxPut = nSize;
	}
	else
	{
		m_nMaxPut = -1;
		AddNullTermination(0);
	}
	SetOverflowFuncs(&CUtlBuffer::GetOverflow, &CUtlBuffer::PutOverflow);
}


void CUtlBuffer::GetDelimitedString(CUtlCharConversion* pConv, char* pString, int nMaxChars)
{
	if (!IsText() || !pConv)
	{
		GetString(pString, nMaxChars);
		return;
	}

	if (!IsValid())
	{
		*pString = 0;
		return;
	}

	if (nMaxChars == 0)
	{
		nMaxChars = INT_MAX;
	}

	EatWhiteSpace();
	if (!PeekStringMatch(0, pConv->GetDelimiter(), pConv->GetDelimiterLength()))
		return;

	SeekGet(SEEK_CURRENT, pConv->GetDelimiterLength());

	int nRead = 0;
	while (IsValid())
	{
		if (PeekStringMatch(0, pConv->GetDelimiter(), pConv->GetDelimiterLength()))
		{
			SeekGet(SEEK_CURRENT, pConv->GetDelimiterLength());
			break;
		}

		char c = GetDelimitedCharInternal(pConv);

		if (nRead < nMaxChars)
		{
			pString[nRead] = c;
			++nRead;
		}
	}

	if (nRead >= nMaxChars)
	{
		nRead = nMaxChars - 1;
	}
	pString[nRead] = '\0';
}

void CUtlBuffer::EatWhiteSpace()
{
	if (IsText() && IsValid())
	{
		while (CheckGet(sizeof(char)))
		{
			if (!isspace(*(const unsigned char*)PeekGet()))
				break;
			m_Get += sizeof(char);
		}
	}
}
bool CUtlBuffer::EatCPPComment()
{
	if (IsText() && IsValid())
	{
		const char* pPeek = (const char*)PeekGet(2 * sizeof(char), 0);
		if (!pPeek || (pPeek[0] != '/') || (pPeek[1] != '/'))
			return false;

		m_Get += 2;

		for (char c = GetChar(); IsValid(); c = GetChar())
		{
			if (c == '\n')
				break;
		}
		return true;
	}
	return false;
}

bool CUtlBuffer::CheckGet(int nSize)
{
	if (m_Error & GET_OVERFLOW)
		return false;

	if (TellMaxPut() < m_Get + nSize)
	{
		m_Error |= GET_OVERFLOW;
		return false;
	}

	if ((m_Get < m_nOffset) || (m_Memory.NumAllocated() < m_Get - m_nOffset + nSize))
	{
		if (!OnGetOverflow(nSize))
		{
			m_Error |= GET_OVERFLOW;
			return false;
		}
	}

	return true;
}


bool CUtlBuffer::CheckPeekGet(int nOffset, int nSize)
{
	if (m_Error & GET_OVERFLOW)
		return false;

	bool bOk = CheckGet(nOffset + nSize);
	m_Error &= ~GET_OVERFLOW;
	return bOk;
}

const void* CUtlBuffer::PeekGet(int nMaxSize, int nOffset)
{
	if (!CheckPeekGet(nOffset, nMaxSize))
		return NULL;

	int Index = m_Get + nOffset - m_nOffset;
	Assert(m_Memory.IsIdxValid(Index) && m_Memory.IsIdxValid(Index + nMaxSize - 1));

	return &m_Memory[Index];
}

void CUtlBuffer::SeekGet(SeekType_t type, int offset)
{
	switch (type)
	{
	case SEEK_HEAD:
		m_Get = offset;
		break;

	case SEEK_CURRENT:
		m_Get += offset;
		break;

	case SEEK_TAIL:
		m_Get = m_nMaxPut - offset;
		break;
	}

	if (m_Get > m_nMaxPut)
	{
		m_Error |= GET_OVERFLOW;
	}
	else
	{
		m_Error &= ~GET_OVERFLOW;
		if (m_Get < m_nOffset || m_Get >= m_nOffset + Size())
		{
			OnGetOverflow(-1);
		}
	}
}



void KeyValues::Clear(void)
{
	delete m_pSub;
	m_pSub = NULL;
	m_iDataType = TYPE_NONE;
}

void KeyValues::deleteThis()
{
	delete this;
}


KeyValues* KeyValues::CreateKey(const char* keyName)
{
	KeyValues* dat = new KeyValues(keyName);
	dat->UsesEscapeSequences(m_bHasEscapeSequences != 0);       
	AddSubKey(dat);
	return dat;
}


void KeyValues::AddSubKey(KeyValues* pSubkey)
{
	Assert(pSubkey->m_pPeer == NULL);

	if (m_pSub == NULL)
	{
		m_pSub = pSubkey;
	}
	else
	{
		KeyValues* pTempDat = m_pSub;
		while (pTempDat->GetNextKey() != NULL)
		{
			pTempDat = pTempDat->GetNextKey();
		}

		pTempDat->SetNextKey(pSubkey);
	}
}


void KeyValues::RemoveSubKey(KeyValues* subKey)
{
	if (!subKey)
		return;

	if (m_pSub == subKey)
	{
		m_pSub = subKey->m_pPeer;
	}
	else
	{
		KeyValues* kv = m_pSub;
		while (kv->m_pPeer)
		{
			if (kv->m_pPeer == subKey)
			{
				kv->m_pPeer = subKey->m_pPeer;
				break;
			}

			kv = kv->m_pPeer;
		}
	}

	subKey->m_pPeer = NULL;
}

static char* s_LastFileLoadingFrom = ( char*)"unknown";      


void KeyValues::UsesEscapeSequences(bool state)
{
	m_bHasEscapeSequences = state;
}


int V_snprintf(char* pDest, int maxLen, char const* pFormat, ...)
{
	Assert(maxLen >= 0);
	AssertValidWritePtr(pDest, maxLen);
	va_list marker;

	va_start(marker, pFormat);
#ifdef _WIN32
	int len = _vsnprintf(pDest, maxLen, pFormat, marker);
#elif defined _LINUX || defined __APPLE__
	int len = vsnprintf(pDest, maxLen, pFormat, marker);
#else
#error "define vsnprintf type."
#endif
	va_end(marker);

	if (len < 0)
	{
		len = maxLen;
		pDest[maxLen - 1] = 0;
	}

	return len;
}


int V_vsnprintf(char* pDest, int maxLen, char const* pFormat, va_list params)
{
	Assert(maxLen > 0);
	AssertValidWritePtr(pDest, maxLen);
	int len = _vsnprintf(pDest, maxLen, pFormat, params);

	if (len < 0)
	{
		len = maxLen;
		pDest[maxLen - 1] = 0;
	}

	return len;
}

KeyValues* KeyValues::FindKey(int keySymbol) const
{
	for (KeyValues* dat = m_pSub; dat != NULL; dat = dat->m_pPeer)
	{
		if (dat->m_iKeyName == keySymbol)
			return dat;
	}

	return NULL;
}


int (*KeyValues::s_pfGetSymbolForString)(const char* name, bool bCreate) = &KeyValues::GetSymbolForStringClassic;
const char* (*KeyValues::s_pfGetStringForSymbol)(int symbol) = &KeyValues::GetStringForSymbolClassic;
CKeyValuesGrowableStringTable* KeyValues::s_pGrowableStringTable = NULL;

int KeyValues::GetSymbolForStringClassic(const char* name, bool bCreate)
{
	return iff.keyValuesSystem->GetSymbolForString(name, bCreate);
}

const char* KeyValues::GetStringForSymbolClassic(int symbol)
{
	return iff.keyValuesSystem->GetStringForSymbol(symbol);
}

KeyValues* KeyValues::GetNextTrueSubKey()
{
	KeyValues* pRet = m_pPeer;
	while (pRet && pRet->m_iDataType != TYPE_NONE)
		pRet = pRet->m_pPeer;

	return pRet;
}


KeyValues* KeyValues::FindKey(const char* keyName, bool bCreate)
{
	if (!keyName || !keyName[0])
		return this;

	char szBuf[256];
	const char* subStr = strchr(keyName, '/');
	const char* searchStr = keyName;

	if (subStr)
	{
		int size = subStr - keyName;
		Q_memcpy(szBuf, keyName, size);
		szBuf[size] = 0;
		searchStr = szBuf;
	}

	HKeySymbol iSearchStr = s_pfGetSymbolForString(searchStr, bCreate);

	if (iSearchStr == INVALID_KEY_SYMBOL)
	{
		return NULL;
	}

	KeyValues* lastItem = NULL;
	KeyValues* dat;
	for (dat = m_pSub; dat != NULL; dat = dat->m_pPeer)
	{
		lastItem = dat;	                  

		if (dat->m_iKeyName == iSearchStr)
		{
			break;
		}
	}

	if (!dat && m_pChain)
	{
		dat = m_pChain->FindKey(keyName, false);
	}

	if (!dat)
	{
		if (bCreate)
		{
			dat = new KeyValues(searchStr);
			dat->UsesEscapeSequences(m_bHasEscapeSequences != 0);	     
			if (lastItem)
			{
				lastItem->m_pPeer = dat;
			}
			else
			{
				m_pSub = dat;
			}
			dat->m_pPeer = NULL;

			m_iDataType = TYPE_NONE;
		}
		else
		{
			return NULL;
		}
	}

	if (subStr)
	{
		return dat->FindKey(subStr + 1, bCreate);
	}

	return dat;
}

void KeyValues::SetString(const char* keyName, const char* value)
{
	KeyValues* dat = FindKey(keyName, true);

	if (dat)
	{
		if (dat->m_iDataType == TYPE_STRING && dat->m_sValue == value)
		{
			return;
		}

		delete[] dat->m_sValue;
		delete[] dat->m_wsValue;
		dat->m_wsValue = NULL;

		if (!value)
		{
			value = "";
		}

		int len = Q_strlen(value);
		dat->m_sValue = new char[len + 1];
		Q_memcpy(dat->m_sValue, value, len + 1);

		dat->m_iDataType = TYPE_STRING;
	}
}


const char* KeyValues::GetString(const char* keyName, const char* defaultValue)
{
	KeyValues* dat = FindKey(keyName, false);
	if (dat)
	{
		char buf[64];
		switch (dat->m_iDataType)
		{
		case TYPE_FLOAT:
			Q_snprintf(buf, sizeof(buf), "%f", dat->m_flValue);
			SetString(keyName, buf);
			break;
		case TYPE_INT:
		case TYPE_PTR:
			Q_snprintf(buf, sizeof(buf), "%d", dat->m_iValue);
			SetString(keyName, buf);
			break;
		case TYPE_UINT64:
			Q_snprintf(buf, sizeof(buf), "%lld", *((uint64*)(dat->m_sValue)));
			SetString(keyName, buf);
			break;

		case TYPE_WSTRING:
		{
			char wideBuf[512];
			int result = Q_UnicodeToUTF8(dat->m_wsValue, wideBuf, 512);
			if (result)
			{
				SetString(keyName, wideBuf);
			}
			else
			{
				return defaultValue;
			}
			break;
		}
		case TYPE_STRING:
			break;
		default:
			return defaultValue;
		};

		return dat->m_sValue;
	}
	return defaultValue;
}


const char* KeyValues::GetName(void) const
{
	return iff.keyValuesSystem->GetStringForSymbol(m_iKeyName);
}

int KeyValues::GetNameSymbol() const
{
	return m_iKeyName;
}

bool KeyValues::LoadFromFile(IBaseFileSystem* filesystem, const char* resourceName, const char* pathID)
{
	Assert(filesystem);
	Assert(IsX360() || (IsPC() && _heapchk() == _HEAPOK));

	FileHandle_t f = iff.g_pFullFileSystem->Open(resourceName, "rb", pathID);
	if (!f)
		return false;

	s_LastFileLoadingFrom = (char*)resourceName;

	int fileSize = filesystem->Size(f);
	unsigned bufSize = ((IFileSystem*)filesystem)->GetOptimalReadSize(f, fileSize + 1);

	char* buffer = (char*)((IFileSystem*)filesystem)->AllocOptimalReadBuffer(f, bufSize);
	Assert(buffer);

	bool bRetOK = (((IFileSystem*)filesystem)->ReadEx(buffer, bufSize, fileSize, f) != 0);

	filesystem->Close(f);	    

	if (bRetOK)
	{
		buffer[fileSize] = 0;      
		bRetOK = LoadFromBuffer(resourceName, buffer, filesystem);
	}

	((IFileSystem*)filesystem)->FreeOptimalReadBuffer(buffer);

	return bRetOK;
}

void KeyValues::SetNextKey(KeyValues* pDat)
{
	m_pPeer = pDat;
}

void KeyValues::CopySubkeys(KeyValues* pParent) const
{
	KeyValues* pPrev = NULL;
	for (KeyValues* sub = m_pSub; sub != NULL; sub = sub->m_pPeer)
	{
		KeyValues* dat = sub->MakeCopy();

		if (pPrev)
			pPrev->m_pPeer = dat;
		else
			pParent->m_pSub = dat;
		dat->m_pPeer = NULL;
		pPrev = dat;
	}
}

KeyValues* KeyValues::MakeCopy(void) const
{
	KeyValues* newKeyValue = new KeyValues(GetName());

	newKeyValue->m_iDataType = m_iDataType;
	switch (m_iDataType)
	{
	case TYPE_STRING:
	{
		if (m_sValue)
		{
			int len = Q_strlen(m_sValue);
			Assert(!newKeyValue->m_sValue);
			newKeyValue->m_sValue = new char[len + 1];
			Q_memcpy(newKeyValue->m_sValue, m_sValue, len + 1);
		}
	}
	break;
	case TYPE_WSTRING:
	{
		if (m_wsValue)
		{
			int len = wcslen(m_wsValue);
			newKeyValue->m_wsValue = new wchar_t[len + 1];
			Q_memcpy(newKeyValue->m_wsValue, m_wsValue, (len + 1) * sizeof(wchar_t));
		}
	}
	break;

	case TYPE_INT:
		newKeyValue->m_iValue = m_iValue;
		break;

	case TYPE_FLOAT:
		newKeyValue->m_flValue = m_flValue;
		break;

	case TYPE_PTR:
		newKeyValue->m_pValue = m_pValue;
		break;

	case TYPE_COLOR:
		newKeyValue->m_Color[0] = m_Color[0];
		newKeyValue->m_Color[1] = m_Color[1];
		newKeyValue->m_Color[2] = m_Color[2];
		newKeyValue->m_Color[3] = m_Color[3];
		break;

	case TYPE_UINT64:
		newKeyValue->m_sValue = new char[sizeof(uint64)];
		Q_memcpy(newKeyValue->m_sValue, m_sValue, sizeof(uint64));
		break;
	};

	CopySubkeys(newKeyValue);
	return newKeyValue;
}



const int MAX_ERROR_STACK = 64;
class CKeyValuesErrorStack
{
public:
	CKeyValuesErrorStack() : m_pFilename("NULL"), m_errorIndex(0), m_maxErrorIndex(0) {}

	void SetFilename(const char* pFilename)
	{
		m_pFilename = pFilename;
		m_maxErrorIndex = 0;
	}

	int Push(int symName)
	{
		if (m_errorIndex < MAX_ERROR_STACK)
		{
			m_errorStack[m_errorIndex] = symName;
		}
		m_errorIndex++;
		m_maxErrorIndex = max(m_maxErrorIndex, (m_errorIndex - 1));
		return m_errorIndex - 1;
	}

	void Pop()
	{
		m_errorIndex--;
		Assert(m_errorIndex >= 0);
	}

	void Reset(int stackLevel, int symName)
	{
		Assert(stackLevel >= 0 && stackLevel < m_errorIndex);
		m_errorStack[stackLevel] = symName;
	}

	void ReportError(const char* pError)
	{
		printf("KeyValues Error: %s in file %s\n", pError, m_pFilename);
		for (int i = 0; i < m_maxErrorIndex; i++)
		{
			if (m_errorStack[i] != INVALID_KEY_SYMBOL)
			{
				if (i < m_errorIndex)
				{
					printf("%s, ", iff.keyValuesSystem->GetStringForSymbol(m_errorStack[i]));
				}
				else
				{
					printf("(*%s*), ", iff.keyValuesSystem->GetStringForSymbol(m_errorStack[i]));
				}
			}
		}
		printf("\n");
	}

private:
	int		m_errorStack[MAX_ERROR_STACK];
	const char* m_pFilename;
	int		m_errorIndex;
	int		m_maxErrorIndex;
} g_KeyValuesErrorStack;


class CKeyErrorContext
{
public:
	CKeyErrorContext(KeyValues* pKv)
	{
		Init(pKv->GetNameSymbol());
	}

	~CKeyErrorContext()
	{
		g_KeyValuesErrorStack.Pop();
	}
	CKeyErrorContext(int symName)
	{
		Init(symName);
	}
	void Reset(int symName)
	{
		g_KeyValuesErrorStack.Reset(m_stackLevel, symName);
	}
private:
	void Init(int symName)
	{
		m_stackLevel = g_KeyValuesErrorStack.Push(symName);
	}

	int m_stackLevel;
};


void KeyValues::RecursiveLoadFromBuffer(char const* resourceName, CUtlBuffer& buf)
{
	CKeyErrorContext errorReport(this);
	bool wasQuoted;
	bool wasConditional;
	CKeyErrorContext errorKey(INVALID_KEY_SYMBOL);
	while (1)
	{
		bool bAccepted = true;

		const char* name = ReadToken(buf, wasQuoted, wasConditional);

		if (!name)	   
		{
			g_KeyValuesErrorStack.ReportError("RecursiveLoadFromBuffer:  got EOF instead of keyname");
			break;
		}

		if (!*name)       
		{
			g_KeyValuesErrorStack.ReportError("RecursiveLoadFromBuffer:  got empty keyname");
			break;
		}

		if (*name == '}' && !wasQuoted)	     
			break;

		KeyValues* dat = CreateKey(name);

		errorKey.Reset(dat->GetNameSymbol());

		const char* value = ReadToken(buf, wasQuoted, wasConditional);

		if (wasConditional && value)
		{
			bAccepted = EvaluateConditional(value);

			value = ReadToken(buf, wasQuoted, wasConditional);
		}

		if (!value)
		{
			g_KeyValuesErrorStack.ReportError("RecursiveLoadFromBuffer:  got NULL key");
			break;
		}

		if (*value == '}' && !wasQuoted)
		{
			g_KeyValuesErrorStack.ReportError("RecursiveLoadFromBuffer:  got } in key");
			break;
		}

		if (*value == '{' && !wasQuoted)
		{
			errorKey.Reset(INVALID_KEY_SYMBOL);
			dat->RecursiveLoadFromBuffer(resourceName, buf);
		}
		else
		{
			if (wasConditional)
			{
				g_KeyValuesErrorStack.ReportError("RecursiveLoadFromBuffer:  got conditional between key and value");
				break;
			}

			if (dat->m_sValue)
			{
				delete[] dat->m_sValue;
				dat->m_sValue = NULL;
			}

			int len = Q_strlen(value);

			char* pIEnd;	     
			char* pFEnd;	     
			const char* pSEnd = value + len;     

			int ival = strtol(value, &pIEnd, 10);
			float fval = (float)strtod(value, &pFEnd);

			if (*value == 0)
			{
				dat->m_iDataType = TYPE_STRING;
			}
			else if ((18 == len) && (value[0] == '0') && (value[1] == 'x'))
			{
				int64 retVal = 0;
				for (int i = 2; i < 2 + 16; i++)
				{
					char digit = value[i];
					if (digit >= 'a')
						digit -= 'a' - ('9' + 1);
					else
						if (digit >= 'A')
							digit -= 'A' - ('9' + 1);
					retVal = (retVal * 16) + (digit - '0');
				}
				dat->m_sValue = new char[sizeof(uint64)];
				*((uint64*)dat->m_sValue) = retVal;
				dat->m_iDataType = TYPE_UINT64;
			}
			else if ((pFEnd > pIEnd) && (pFEnd == pSEnd))
			{
				dat->m_flValue = fval;
				dat->m_iDataType = TYPE_FLOAT;
			}
			else if (pIEnd == pSEnd)
			{
				dat->m_iValue = ival;
				dat->m_iDataType = TYPE_INT;
			}
			else
			{
				dat->m_iDataType = TYPE_STRING;
			}

			if (dat->m_iDataType == TYPE_STRING)
			{
				dat->m_sValue = new char[len + 1];
				Q_memcpy(dat->m_sValue, value, len + 1);
			}

			int prevPos = buf.TellGet();
			const char* peek = ReadToken(buf, wasQuoted, wasConditional);
			if (wasConditional)
			{
				bAccepted = EvaluateConditional(peek);
			}
			else
			{
				buf.SeekGet(CUtlBuffer::SEEK_HEAD, prevPos);
			}
		}

		if (!bAccepted)
		{
			this->RemoveSubKey(dat);
			dat->deleteThis();
			dat = NULL;
		}
	}
}


void KeyValues::AppendIncludedKeys(CUtlVector< KeyValues* >& includedKeys)
{
	int includeCount = includedKeys.Count();
	int i;
	for (i = 0; i < includeCount; i++)
	{
		KeyValues* kv = includedKeys[i];
		Assert(kv);

		KeyValues* insertSpot = this;
		while (insertSpot->GetNextKey())
		{
			insertSpot = insertSpot->GetNextKey();
		}

		insertSpot->SetNextKey(kv);
	}
}

void KeyValues::RecursiveMergeKeyValues(KeyValues* baseKV)
{
	for (KeyValues* baseChild = baseKV->m_pSub; baseChild != NULL; baseChild = baseChild->m_pPeer)
	{
		bool bFoundMatch = false;

		for (KeyValues* newChild = m_pSub; newChild != NULL; newChild = newChild->m_pPeer)
		{
			if (!Q_strcmp(baseChild->GetName(), newChild->GetName()))
			{
				newChild->RecursiveMergeKeyValues(baseChild);
				bFoundMatch = true;
				break;
			}
		}

		if (!bFoundMatch)
		{
			KeyValues* dat = baseChild->MakeCopy();
			Assert(dat);
			AddSubKey(dat);
		}
	}
}

bool KeyValues::EvaluateConditional(const char* str)
{
	bool bResult = false;
	bool bXboxUI = IsX360();

	if (bXboxUI)
	{
		bResult = !Q_stricmp("[$X360]", str);
	}
	else
	{
		bResult = !Q_stricmp("[$WIN32]", str);
	}

	return bResult;
}

void KeyValues::MergeBaseKeys(CUtlVector< KeyValues* >& baseKeys)
{
	int includeCount = baseKeys.Count();
	int i;
	for (i = 0; i < includeCount; i++)
	{
		KeyValues* kv = baseKeys[i];
		Assert(kv);

		RecursiveMergeKeyValues(kv);
	}
}


void KeyValues::ParseIncludedKeys(char const* resourceName, const char* filetoinclude,
	IBaseFileSystem* pFileSystem, const char* pPathID, CUtlVector< KeyValues* >& includedKeys)
{
	Assert(resourceName);
	Assert(filetoinclude);
	Assert(pFileSystem);
	if (!pFileSystem)
	{
		return;
	}
	char fullpath[512];
	Q_strncpy(fullpath, resourceName, sizeof(fullpath));
	bool done = false;
	int len = Q_strlen(fullpath);
	while (!done)
	{
		if (len <= 0)
		{
			break;
		}

		if (fullpath[len - 1] == '\\' ||
			fullpath[len - 1] == '/')
		{
			break;
		}

		fullpath[len - 1] = 0;
		--len;
	}

	Q_strncat(fullpath, filetoinclude, sizeof(fullpath), COPY_ALL_CHARACTERS);

	KeyValues* newKV = new KeyValues(fullpath);

	newKV->UsesEscapeSequences(m_bHasEscapeSequences != 0);	     

	if (newKV->LoadFromFile(pFileSystem, fullpath, pPathID))
	{
		includedKeys.AddToTail(newKV);
	}
	else
	{
		printf("KeyValues::ParseIncludedKeys: Couldn't load included keyvalue file %s\n", fullpath);
		newKV->deleteThis();
	}

}






	int V_UnicodeToUTF8(const wchar_t* pUnicode, char* pUTF8, int cubDestSizeInBytes)
	{
		AssertValidReadPtr(pUnicode);

		if (cubDestSizeInBytes > 0)
		{
			pUTF8[0] = 0;
		}

#ifdef _WIN32
		int cchResult = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pUTF8, cubDestSizeInBytes, NULL, NULL);
#elif POSIX
		int cchResult = 0;
		if (pUnicode && pUTF8)
			cchResult = wcstombs(pUTF8, pUnicode, cubDestSizeInBytes) + 1;
#endif

		if (cubDestSizeInBytes > 0)
		{
			pUTF8[cubDestSizeInBytes - 1] = 0;
		}

		return cchResult;
	}



#define KEYVALUES_TOKEN_SIZE	1024
static char s_pTokenBuf[KEYVALUES_TOKEN_SIZE];

#pragma warning (disable:4706)
const char* KeyValues::ReadToken(CUtlBuffer& buf, bool& wasQuoted, bool& wasConditional)
{
	wasQuoted = false;
	wasConditional = false;

	if (!buf.IsValid())
		return NULL;

	while (true)
	{
		buf.EatWhiteSpace();
		if (!buf.IsValid())
			return NULL;	     

		if (!buf.EatCPPComment())
			break;
	}

	const char* c = (const char*)buf.PeekGet(sizeof(char), 0);
	if (!c)
		return NULL;

	if (*c == '\"')
	{
		wasQuoted = true;
		buf.GetDelimitedString(m_bHasEscapeSequences ? GetCStringCharConversion() : GetNoEscCharConversion(),
			s_pTokenBuf, KEYVALUES_TOKEN_SIZE);
		return s_pTokenBuf;
	}

	if (*c == '{' || *c == '}')
	{
		s_pTokenBuf[0] = *c;
		s_pTokenBuf[1] = 0;
		buf.SeekGet(CUtlBuffer::SEEK_CURRENT, 1);
		return s_pTokenBuf;
	}

	bool bReportedError = false;
	bool bConditionalStart = false;
	int nCount = 0;
	while (c = (const char*)buf.PeekGet(sizeof(char), 0))
	{
		if (*c == 0)
			break;

		if (*c == '"' || *c == '{' || *c == '}')
			break;

		if (*c == '[')
			bConditionalStart = true;

		if (*c == ']' && bConditionalStart)
		{
			wasConditional = true;
		}

		if (isspace(*c))
			break;

		if (nCount < (KEYVALUES_TOKEN_SIZE - 1))
		{
			s_pTokenBuf[nCount++] = *c;	    
		}
		else if (!bReportedError)
		{
			bReportedError = true;
			g_KeyValuesErrorStack.ReportError(" ReadToken overflow");
		}

		buf.SeekGet(CUtlBuffer::SEEK_CURRENT, 1);
	}
	s_pTokenBuf[nCount] = 0;
	return s_pTokenBuf;
}
#pragma warning (default:4706)


bool KeyValues::LoadFromBuffer(char const* resourceName, CUtlBuffer& buf, IBaseFileSystem* pFileSystem, const char* pPathID)
{
	KeyValues* pPreviousKey = NULL;
	KeyValues* pCurrentKey = this;
	CUtlVector< KeyValues* > includedKeys;
	CUtlVector< KeyValues* > baseKeys;
	bool wasQuoted;
	bool wasConditional;
	g_KeyValuesErrorStack.SetFilename(resourceName);
	do
	{
		bool bAccepted = true;

		const char* s = ReadToken(buf, wasQuoted, wasConditional);
		if (!buf.IsValid() || !s || *s == 0)
			break;

		if (!Q_stricmp(s, "#include"))	       
		{
			s = ReadToken(buf, wasQuoted, wasConditional);
			if (!s || *s == 0)
			{
				g_KeyValuesErrorStack.ReportError("#include is NULL ");
			}
			else
			{
				ParseIncludedKeys(resourceName, s, pFileSystem, pPathID, includedKeys);
			}

			continue;
		}
		else if (!Q_stricmp(s, "#base"))
		{
			s = ReadToken(buf, wasQuoted, wasConditional);
			if (!s || *s == 0)
			{
				g_KeyValuesErrorStack.ReportError("#base is NULL ");
			}
			else
			{
				ParseIncludedKeys(resourceName, s, pFileSystem, pPathID, baseKeys);
			}

			continue;
		}

		if (!pCurrentKey)
		{
			pCurrentKey = new KeyValues(s);
			Assert(pCurrentKey);

			pCurrentKey->UsesEscapeSequences(m_bHasEscapeSequences != 0);      

			if (pPreviousKey)
			{
				pPreviousKey->SetNextKey(pCurrentKey);
			}
		}
		else
		{
			pCurrentKey->SetName(s);
		}

		s = ReadToken(buf, wasQuoted, wasConditional);

		if (wasConditional)
		{
			bAccepted = EvaluateConditional(s);

			s = ReadToken(buf, wasQuoted, wasConditional);
		}

		if (s && *s == '{' && !wasQuoted)
		{
			pCurrentKey->RecursiveLoadFromBuffer(resourceName, buf);
		}
		else
		{
			g_KeyValuesErrorStack.ReportError("LoadFromBuffer: missing {");
		}

		if (!bAccepted)
		{
			if (pPreviousKey)
			{
				pPreviousKey->SetNextKey(NULL);
			}
			pCurrentKey->Clear();
		}
		else
		{
			pPreviousKey = pCurrentKey;
			pCurrentKey = NULL;
		}
	} while (buf.IsValid());

	AppendIncludedKeys(includedKeys);
	{
		int i;
		for (i = includedKeys.Count() - 1; i > 0; i--)
		{
			KeyValues* kv = includedKeys[i];
			kv->deleteThis();
		}
	}

	MergeBaseKeys(baseKeys);
	{
		int i;
		for (i = baseKeys.Count() - 1; i >= 0; i--)
		{
			KeyValues* kv = baseKeys[i];
			kv->deleteThis();
		}
	}

	g_KeyValuesErrorStack.SetFilename("");

	return true;
}


bool KeyValues::LoadFromBuffer(char const* resourceName, const char* pBuffer, IBaseFileSystem* pFileSystem, const char* pPathID)
{
	if (!pBuffer)
		return true;

	int nLen = Q_strlen(pBuffer);
	CUtlBuffer buf(pBuffer, nLen, CUtlBuffer::READ_ONLY | CUtlBuffer::TEXT_BUFFER);
	return LoadFromBuffer(resourceName, buf, pFileSystem, pPathID);
}