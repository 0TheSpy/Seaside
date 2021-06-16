#ifndef DMXATTRIBUTE_H
#define DMXATTRIBUTE_H

#ifdef _WIN32
#pragma once
#endif

#include "dmattributetypes.h"
#include "utlvector.h"
#include "utlrbtree.h"
#include "utlsymbol.h"
#include "mempool.h"
#include "dmxloader.h"


class CDmxElement;





#define DECLARE_DMX_ATTRIBUTE_TYPE_INTERNAL( _className, _storageType, _attributeType, _attributeName, _defaultSetStatement ) \
	template< > class CDmAttributeInfo< _className >						\
	{																		\
	private:																\
		enum { ATTRIBUTE_TYPE = _attributeType };							\
		typedef _storageType StorageType_t;									\
		static DmAttributeType_t AttributeType() { return _attributeType; }	\
		static const char *AttributeTypeName() { return _attributeName; }	\
		static void SetDefaultValue( _className& value ) { _defaultSetStatement }	\
		friend class CDmxAttribute;											\
		friend class CDmxElement;											\
	};																		\

#define DECLARE_DMX_ATTRIBUTE_ARRAY_TYPE_INTERNAL( _className, _storageType, _attributeType, _attributeName ) \
	template< > class CDmAttributeInfo< CUtlVector<_className> >				\
	{																			\
	private:																	\
		enum { ATTRIBUTE_TYPE = _attributeType };								\
		typedef _storageType StorageType_t;										\
		static DmAttributeType_t AttributeType() { return _attributeType; }		\
		static const char *AttributeTypeName() { return _attributeName; }		\
		static void SetDefaultValue( CUtlVector< _className >& value ) { value.RemoveAll(); }	\
		friend class CDmxAttribute;												\
		friend class CDmxElement;												\
	};																			\

#define DECLARE_DMX_ATTRIBUTE_TYPE( _className, _attributeType, _attributeName, _defaultSetStatement ) \
	DECLARE_DMX_ATTRIBUTE_TYPE_INTERNAL( _className, _className, _attributeType, _attributeName, _defaultSetStatement )

#define DECLARE_DMX_ATTRIBUTE_ARRAY_TYPE( _className, _attributeType, _attributeName )\
	DECLARE_DMX_ATTRIBUTE_ARRAY_TYPE_INTERNAL( _className, CUtlVector< _className >, _attributeType, _attributeName )



DECLARE_DMX_ATTRIBUTE_TYPE(CDmxElement*, AT_ELEMENT, "element", value = 0; )
DECLARE_DMX_ATTRIBUTE_ARRAY_TYPE(CDmxElement*, AT_ELEMENT_ARRAY, "element_array")

DECLARE_DMX_ATTRIBUTE_TYPE(CUtlString, AT_STRING, "string", value.Set(NULL); )
DECLARE_DMX_ATTRIBUTE_ARRAY_TYPE(CUtlString, AT_STRING_ARRAY, "string_array")


class CDmxAttribute
{
	DECLARE_DMX_ALLOCATOR();

public:
	DmAttributeType_t GetType() const;
	const char* GetTypeString() const;
	template< class T > bool IsA() const;

	const char* GetName() const;
	CUtlSymbolLarge GetNameSymbol() const;
	void SetName(const char* pName);

	template< class T >	const T& GetValue() const;
	template< class T > const CUtlVector< T >& GetArray() const;
	const char* GetValueString() const;

	template< class T > void SetValue(const T& value);
	void SetValue(const char* pString);
	void SetValue(char* pString);
	void SetValue(const void* pBuffer, size_t nLen);
	void SetValue(const CDmxAttribute* pAttribute);

	template< class T > CUtlVector< T >& GetArrayForEdit();

	void	SetToDefaultValue();

	void SetValueFromString(const char* pValue);
	const char* GetValueAsString(char* pBuffer, size_t nBufLen) const;

	int GetArrayCount() const;

	bool Unserialize(DmAttributeType_t type, CUtlBuffer& buf);
	bool UnserializeElement(DmAttributeType_t type, CUtlBuffer& buf);
	bool Serialize(CUtlBuffer& buf) const;
	bool SerializeElement(int nIndex, CUtlBuffer& buf) const;
	bool SerializesOnMultipleLines() const;

	static int AttributeDataSize(DmAttributeType_t type);
	static DmAttributeType_t ArrayAttributeBasicType(DmAttributeType_t type);

private:
	CDmxAttribute(const char* pAttributeName);
	CDmxAttribute(CUtlSymbolLarge attributeName);
	~CDmxAttribute();

	void AllocateDataMemory(DmAttributeType_t type);
	void AllocateDataMemory_AndConstruct(DmAttributeType_t type);
	void FreeDataMemory();


	void SetValue(DmAttributeType_t type, const void* pSrc, int nLen);
	void SetArrayValue(DmAttributeType_t type, const void* pSrc, int nDataTypeSize, int nArrayLength, int nSrcStride);
	void GetArrayValue(DmAttributeType_t type, void* pDest, int nDataTypeSize, int nArrayLength, const char* pDefaultString = NULL) const;
	void SetArrayCount(int nArrayCount);
	const void* GetArrayBase(void) const;

	template < class VT, class T > void ConstructDataMemory(void);
	template < class VT, class T > void DestructDataMemory(void);
	template < class VT, class T > void SetArrayCount(int nArrayCount);
	template < class VT, class T > void GetArrayCount(int& nArrayCount) const;
	template < class VT, class T > void GetArrayBase(const void*& pBasePtr) const;
	template < class VT, class T > void SerializesOnMultipleLines(bool& bResult) const;
	template < class VT, class T > void SerializeType(bool& bSuccess, CUtlBuffer& buf) const;
	template < class VT, class T > void SerializeTypedElement(bool& bSuccess, int nIndex, CUtlBuffer& buf) const;
	template < class VT, class T > void UnserializeType(bool& bSuccess, CUtlBuffer& buf);
	template < class VT, class T > void UnserializeTypedElement(bool& bSuccess, CUtlBuffer& buf);
	template < class VT, class T > void SetDefaultValue(void);


	DmAttributeType_t m_Type;
	CUtlSymbolLarge m_Name;
	void* m_pData;

	static CUtlSymbolTableLargeMT s_AttributeNameSymbols;

	friend class CDmxElement;

public:

	static const char* s_pAttributeTypeName[AT_TYPE_COUNT];

};


inline DmAttributeType_t CDmxAttribute::GetType() const
{
	return m_Type;
}

template< class T > inline bool CDmxAttribute::IsA() const
{
	return GetType() == CDmAttributeInfo< T >::ATTRIBUTE_TYPE;
}

inline CUtlSymbolLarge CDmxAttribute::GetNameSymbol() const
{
	return m_Name;
}


template< class T > void CDmxAttribute::SetValue(const T& value)
{
	AllocateDataMemory(CDmAttributeInfo<T>::AttributeType());
	CopyConstruct((T*)m_pData, value);
}


inline const char* CDmxAttribute::GetValueString() const
{
	if (m_Type == AT_STRING)
		return *(CUtlString*)m_pData;
	return "";
}

template< class T >
inline const T& CDmxAttribute::GetValue() const
{
	if (CDmAttributeInfo<T>::AttributeType() == m_Type)
		return *(T*)m_pData;

	static T defaultValue;
	CDmAttributeInfo<T>::SetDefaultValue(defaultValue);
	return defaultValue;
}

template< class T >
inline const CUtlVector< T >& CDmxAttribute::GetArray() const
{
	if (CDmAttributeInfo< CUtlVector< T > >::AttributeType() == m_Type)
		return *(CUtlVector< T >*)m_pData;

	static CUtlVector<T> defaultArray;
	return defaultArray;
}

template< class T >
inline CUtlVector< T >& CDmxAttribute::GetArrayForEdit()
{
	if (CDmAttributeInfo< CUtlVector< T > >::AttributeType() == m_Type)
		return *(CUtlVector< T >*)m_pData;

	AllocateDataMemory(CDmAttributeInfo< CUtlVector< T > >::AttributeType());
	Construct((CUtlVector<T>*)m_pData);
	return *(CUtlVector< T >*)m_pData;
}

#endif  