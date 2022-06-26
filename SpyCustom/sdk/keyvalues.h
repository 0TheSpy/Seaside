#ifndef KEYVALUES_H
#define KEYVALUES_H

#ifdef _WIN32
#pragma once
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#include "utlvector.h"
#include "color.h"
#include "exprevaluator.h"

#define TRACK_KV_ADD( ptr, name )	track.AddKv( ptr, name )
#define SPLIT_3_BYTES_INTO_1_AND_2( x1, x2, x3 ) do { x1 = (uint8)(x3); x2 = (uint16)( (x3) >> 8 ); } while( 0 )


#define FOR_EACH_SUBKEY( kvRoot, kvSubKey ) \
	for ( KeyValues * kvSubKey = kvRoot->GetFirstSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextKey() )

#define FOR_EACH_TRUE_SUBKEY( kvRoot, kvSubKey ) \
	for ( KeyValues * kvSubKey = kvRoot->GetFirstTrueSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextTrueSubKey() )

#define FOR_EACH_VALUE( kvRoot, kvValue ) \
	for ( KeyValues * kvValue = kvRoot->GetFirstValue(); kvValue != NULL; kvValue = kvValue->GetNextValue() )


class IBaseFileSystem;
class CUtlBuffer;
class Color;
class CKeyValuesTokenReader;
class KeyValues;
class IKeyValuesDumpContext;
typedef void* FileHandle_t;
class CKeyValuesGrowableStringTable;


#define KV_BINARY_POOLED_FORMAT 0xAA


#define FOR_EACH_SUBKEY( kvRoot, kvSubKey ) \
	for ( KeyValues * kvSubKey = kvRoot->GetFirstSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextKey() )

#define FOR_EACH_TRUE_SUBKEY( kvRoot, kvSubKey ) \
	for ( KeyValues * kvSubKey = kvRoot->GetFirstTrueSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextTrueSubKey() )

#define FOR_EACH_VALUE( kvRoot, kvValue ) \
	for ( KeyValues * kvValue = kvRoot->GetFirstValue(); kvValue != NULL; kvValue = kvValue->GetNextValue() )


class KeyValues
{
	friend class CKeyValuesTokenReader;

public:
	static void SetUseGrowableStringTable(bool bUseGrowableTable);

	explicit KeyValues(const char* setName);

	class AutoDelete
	{
	public:
		explicit inline AutoDelete(KeyValues* pKeyValues) : m_pKeyValues(pKeyValues) {}
		explicit inline AutoDelete(const char* pchKVName) : m_pKeyValues(new KeyValues(pchKVName)) {}
		inline ~AutoDelete(void) { delete m_pKeyValues; }
		inline void Assign(KeyValues* pKeyValues) { m_pKeyValues = pKeyValues; }
		inline KeyValues* Detach() { KeyValues* retval = m_pKeyValues; Assign(NULL); return retval; }
		KeyValues* operator->() { return m_pKeyValues; }
		operator KeyValues* () { return m_pKeyValues; }
	private:
		AutoDelete(AutoDelete const& x);  
		AutoDelete& operator= (AutoDelete const& x);  
	protected:
		KeyValues* m_pKeyValues;
	};

	class AutoDeleteInline : public AutoDelete
	{
	public:
		explicit inline AutoDeleteInline(KeyValues* pKeyValues) : AutoDelete(pKeyValues) {}
		inline operator KeyValues* () const { return m_pKeyValues; }
		inline KeyValues* Get() const { return m_pKeyValues; }
	};

	KeyValues(const char* setName, const char* firstKey, const char* firstValue);
	KeyValues(const char* setName, const char* firstKey, const wchar_t* firstValue);
	KeyValues(const char* setName, const char* firstKey, int firstValue);
	KeyValues(const char* setName, const char* firstKey, const char* firstValue, const char* secondKey, const char* secondValue);
	KeyValues(const char* setName, const char* firstKey, int firstValue, const char* secondKey, int secondValue);

	~KeyValues();

	const char* GetName() const;
	void SetName(const char* setName);

	int GetNameSymbol() const;
	int GetNameSymbolCaseSensitive() const;

	void UsesEscapeSequences(bool state);   
	bool LoadFromFile(IBaseFileSystem* filesystem, const char* resourceName, const char* pathID = NULL);
	
	bool SaveToFile(IBaseFileSystem* filesystem, const char* resourceName, const char* pathID = NULL, bool bWriteEmptySubkeys = false);

	bool LoadFromBuffer(char const* resourceName, const char* pBuffer, IBaseFileSystem* pFileSystem = NULL, const char* pPathID = NULL);

	bool LoadFromBuffer(char const* resourceName, CUtlBuffer& buf, IBaseFileSystem* pFileSystem = NULL, const char* pPathID = NULL);



	KeyValues* FindKey(const char* keyName, bool bCreate = false);
	KeyValues* FindKey(int keySymbol) const;
	KeyValues* CreateNewKey();		                           
	void AddSubKey(KeyValues* pSubkey);	              
	void RemoveSubKey(KeyValues* subKey);	          
	void InsertSubKey(int nIndex, KeyValues* pSubKey);          
	bool ContainsSubKey(KeyValues* pSubKey);              
	void SwapSubKey(KeyValues* pExistingSubKey, KeyValues* pNewSubKey);	                     
	void ElideSubKey(KeyValues* pSubKey);	                    
	KeyValues* CreateKey(const char* keyName);
	KeyValues* CreatePeerKey(const char* keyName);

	KeyValues* GetFirstSubKey() { return m_pSub; }	       
	KeyValues* GetNextKey() { return m_pPeer; }		    
	void SetNextKey(KeyValues* pDat);

	KeyValues* FindLastSubKey();	                           

	KeyValues* GetFirstTrueSubKey();
	KeyValues* GetNextTrueSubKey();

	KeyValues* GetFirstValue();	                  
	KeyValues* GetNextValue();


	int   GetInt(const char* keyName = NULL, int defaultValue = 0);
	uint64 GetUint64(const char* keyName = NULL, uint64 defaultValue = 0);
	float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f);
	const char* GetString(const char* keyName = NULL, const char* defaultValue = "");
	const wchar_t* GetWString(const char* keyName = NULL, const wchar_t* defaultValue = L"");
	void* GetPtr(const char* keyName = NULL, void* defaultValue = (void*)0);
	Color GetColor(const char* keyName = NULL, const Color& defaultColor = Color(0, 0, 0, 0));
	bool GetBool(const char* keyName = NULL, bool defaultValue = false) { return GetInt(keyName, defaultValue ? 1 : 0) ? true : false; }
	bool  IsEmpty(const char* keyName = NULL);

	int   GetInt(int keySymbol, int defaultValue = 0);
	uint64 GetUint64(int keySymbol, uint64 defaultValue = 0);
	float GetFloat(int keySymbol, float defaultValue = 0.0f);
	const char* GetString(int keySymbol, const char* defaultValue = "");
	const wchar_t* GetWString(int keySymbol, const wchar_t* defaultValue = L"");
	void* GetPtr(int keySymbol, void* defaultValue = (void*)0);
	Color GetColor(int keySymbol       );
	bool GetBool(int keySymbol, bool defaultValue = false) { return GetInt(keySymbol, defaultValue ? 1 : 0) ? true : false; }
	bool  IsEmpty(int keySymbol);

	void SetWString(const char* keyName, const wchar_t* value);
	void SetString(const char* keyName, const char* value);
	void SetInt(const char* keyName, int value);
	void SetUint64(const char* keyName, uint64 value);
	void SetFloat(const char* keyName, float value);
	void SetPtr(const char* keyName, void* value);
	void SetColor(const char* keyName, Color value);
	void SetBool(const char* keyName, bool value) { SetInt(keyName, value ? 1 : 0); }

	void* operator new(size_t iAllocSize);
	void* operator new(size_t iAllocSize, int nBlockUse, const char* pFileName, int nLine);
	void operator delete(void* pMem);
	void operator delete(void* pMem, int nBlockUse, const char* pFileName, int nLine);

	KeyValues& operator=(KeyValues& src);

	bool IsEqual(KeyValues* pRHS);

	void ChainKeyValue(KeyValues* pChain);

	void RecursiveSaveToFile(CUtlBuffer& buf, int indentLevel);

	bool WriteAsBinary(CUtlBuffer& buffer) const;
	bool ReadAsBinary(CUtlBuffer& buffer, int nStackDepth = 0);

	bool WriteAsBinaryFiltered(CUtlBuffer& buffer);
	bool ReadAsBinaryFiltered(CUtlBuffer& buffer, int nStackDepth = 0);

	KeyValues* MakeCopy(void) const;

	void CopySubkeys(KeyValues* pParent) const;

	void Clear(void);

	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_COMPILED_INT_BYTE,			          
		TYPE_COMPILED_INT_0,			        
		TYPE_COMPILED_INT_1,			        
		TYPE_NUMTYPES,
	};
	types_t GetDataType(const char* keyName = NULL);
	types_t GetDataType() const;

	void deleteThis();

	void SetStringValue(char const* strValue);

	void UnpackIntoStructure(struct KeyValuesUnpackStructure const* pUnpackTable, void* pDest);

	bool ProcessResolutionKeys(const char* pResString);

	bool Dump(IKeyValuesDumpContext* pDump, int nIndentLevel = 0);

	enum MergeKeyValuesOp_t
	{
		MERGE_KV_ALL,
		MERGE_KV_UPDATE,	               
		MERGE_KV_DELETE,	         
		MERGE_KV_BORROW,	                   
	};
	void MergeFrom(KeyValues* kvMerge, MergeKeyValuesOp_t eOp = MERGE_KV_ALL);

	static KeyValues* FromString(char const* szName, char const* szStringVal, char const** ppEndOfParse = NULL);

	KeyValues* CreateKeyUsingKnownLastChild(const char* keyName, KeyValues* pLastChild);
	void AddSubkeyUsingKnownLastChild(KeyValues* pSubKey, KeyValues* pLastChild);

	KeyValues(KeyValues&);	     


	void RecursiveCopyKeyValues(KeyValues& src);
	void RemoveEverything();
	void RecursiveSaveToFile(IBaseFileSystem* filesystem, FileHandle_t f, CUtlBuffer* pBuf, int indentLevel, bool bWriteEmptySubkeys = false);
	void WriteConvertedString(IBaseFileSystem* filesystem, FileHandle_t f, CUtlBuffer* pBuf, const char* pszString);

	void RecursiveLoadFromBuffer(char const* resourceName, CUtlBuffer& buf);

	void AppendIncludedKeys(CUtlVector< KeyValues* >& includedKeys);
	void ParseIncludedKeys(char const* resourceName, const char* filetoinclude,
		IBaseFileSystem* pFileSystem, const char* pPathID, CUtlVector< KeyValues* >& includedKeys);

	void MergeBaseKeys(CUtlVector< KeyValues* >& baseKeys);
	void RecursiveMergeKeyValues(KeyValues* baseKV);

	void InternalWrite(IBaseFileSystem* filesystem, FileHandle_t f, CUtlBuffer* pBuf, const void* pData, int len);

	void Init();
	const char* ReadToken(CUtlBuffer& buf, bool& wasQuoted, bool& wasConditional);
	void WriteIndents(IBaseFileSystem* filesystem, FileHandle_t f, CUtlBuffer* pBuf, int indentLevel);

	void FreeAllocatedValue();
	void AllocateValueBlock(int size);

	bool ReadAsBinaryPooledFormat(CUtlBuffer& buf, IBaseFileSystem* pFileSystem, unsigned int poolKey, GetSymbolProc_t pfnEvaluateSymbolProc);

	bool EvaluateConditional(const char* str);

	uint32 m_iKeyName : 24;	       
	uint32 m_iKeyNameCaseSensitive1 : 8;	         

	char* m_sValue;
	wchar_t* m_wsValue;

	union
	{
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char	   m_iDataType;
	char	   m_bHasEscapeSequences;             
	uint16	   m_iKeyNameCaseSensitive2;	         

	uint32     m_iUnk1;
	uint32     m_iUnk2;

	KeyValues* m_pPeer;	      
	KeyValues* m_pSub;	         
	KeyValues* m_pChain;        

	GetSymbolProc_t	m_pExpressionGetSymbolProc;

private:
	static int (*s_pfGetSymbolForString)(const char* name, bool bCreate);
	static const char* (*s_pfGetStringForSymbol)(int symbol);
	static CKeyValuesGrowableStringTable* s_pGrowableStringTable;

public:
	static int GetSymbolForStringClassic(const char* name, bool bCreate = true);
	static const char* GetStringForSymbolClassic(int symbol);

	static int GetSymbolForStringGrowable(const char* name, bool bCreate = true);
	static const char* GetStringForSymbolGrowable(int symbol);
};

typedef KeyValues::AutoDelete KeyValuesAD;

enum KeyValuesUnpackDestinationTypes_t
{
	UNPACK_TYPE_FLOAT,										    
	UNPACK_TYPE_VECTOR,										    
	UNPACK_TYPE_VECTOR_COLOR,								        
	UNPACK_TYPE_STRING,										        
	UNPACK_TYPE_INT,										    
	UNPACK_TYPE_FOUR_FLOATS,	                 
	UNPACK_TYPE_TWO_FLOATS,		               
};

#define UNPACK_FIXED( kname, kdefault, dtype, ofs ) { kname, kdefault, dtype, ofs, 0 }
#define UNPACK_VARIABLE( kname, kdefault, dtype, ofs, sz ) { kname, kdefault, dtype, ofs, sz }
#define UNPACK_END_MARKER { NULL, NULL, UNPACK_TYPE_FLOAT, 0 }

struct KeyValuesUnpackStructure
{
	char const* m_pKeyName;									    
	char const* m_pKeyDefault;								  
	KeyValuesUnpackDestinationTypes_t m_eDataType;			  
	size_t m_nFieldOffset;									    
	size_t m_nFieldSize;									      
};

inline int   KeyValues::GetInt(int keySymbol, int defaultValue)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetInt((const char*)NULL, defaultValue) : defaultValue;
}

inline uint64 KeyValues::GetUint64(int keySymbol, uint64 defaultValue)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetUint64((const char*)NULL, defaultValue) : defaultValue;
}

inline float KeyValues::GetFloat(int keySymbol, float defaultValue)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetFloat((const char*)NULL, defaultValue) : defaultValue;
}

inline const char* KeyValues::GetString(int keySymbol, const char* defaultValue)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetString((const char*)NULL, defaultValue) : defaultValue;
}

inline const wchar_t* KeyValues::GetWString(int keySymbol, const wchar_t* defaultValue)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetWString((const char*)NULL, defaultValue) : defaultValue;
}

inline void* KeyValues::GetPtr(int keySymbol, void* defaultValue)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetPtr((const char*)NULL, defaultValue) : defaultValue;
}

inline Color KeyValues::GetColor(int keySymbol)
{
	Color defaultValue(0, 0, 0, 0);
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->GetColor() : defaultValue;
}

inline bool  KeyValues::IsEmpty(int keySymbol)
{
	KeyValues* dat = FindKey(keySymbol);
	return dat ? dat->IsEmpty() : true;
}


class IKeyValuesDumpContext
{
public:
	virtual bool KvBeginKey(KeyValues* pKey, int nIndentLevel) = 0;
	virtual bool KvWriteValue(KeyValues* pValue, int nIndentLevel) = 0;
	virtual bool KvEndKey(KeyValues* pKey, int nIndentLevel) = 0;
};

class IKeyValuesDumpContextAsText : public IKeyValuesDumpContext
{
public:
	virtual bool KvBeginKey(KeyValues* pKey, int nIndentLevel);
	virtual bool KvWriteValue(KeyValues* pValue, int nIndentLevel);
	virtual bool KvEndKey(KeyValues* pKey, int nIndentLevel);

public:
	virtual bool KvWriteIndent(int nIndentLevel);
	virtual bool KvWriteText(char const* szText) = 0;
};

class CKeyValuesDumpContextAsDevMsg : public IKeyValuesDumpContextAsText
{
public:
	CKeyValuesDumpContextAsDevMsg(int nDeveloperLevel = 1) : m_nDeveloperLevel(nDeveloperLevel) {}

public:
	virtual bool KvBeginKey(KeyValues* pKey, int nIndentLevel);
	virtual bool KvWriteText(char const* szText);

protected:
	int m_nDeveloperLevel;
};

inline bool KeyValuesDumpAsDevMsg(KeyValues* pKeyValues, int nIndentLevel = 0, int nDeveloperLevel = 1)
{
	CKeyValuesDumpContextAsDevMsg ctx(nDeveloperLevel);
	return pKeyValues->Dump(&ctx, nIndentLevel);
}


#endif  
