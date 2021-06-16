#ifndef DMATTRIBUTETYPES_H
#define DMATTRIBUTETYPES_H
#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "utlstring.h"
#include "uniqueid.h"
#include "color.h"
#include "vector2d.h"
#include "vector.h"
#include "vector4d.h"
#include "vmatrix.h"
#include "dmelementhandle.h"
#include "utlsymbollarge.h"
#include "timeutils.h"


typedef UniqueId_t DmObjectId_t;


struct DmUnknownAttribute_t
{
	bool operator==(const DmUnknownAttribute_t& src) const { return true; }
};


struct DmElementAttribute_t
{
	DmElementAttribute_t() : m_ElementType(UTL_INVAL_SYMBOL_LARGE) {}

	operator DmElementHandle_t& () { return m_Handle; }
	operator const DmElementHandle_t& () const { return m_Handle; }

	DmElementHandle_t m_Handle;
	CUtlSymbolLarge m_ElementType;
};

struct DmElementArray_t : public CUtlVector< DmElementHandle_t >
{
	DmElementArray_t() : m_ElementType(UTL_INVAL_SYMBOL_LARGE) {}

	CUtlSymbolLarge m_ElementType;
};


enum DmAttributeType_t
{
	AT_UNKNOWN = 0,

	AT_FIRST_VALUE_TYPE,

	AT_ELEMENT = AT_FIRST_VALUE_TYPE,
	AT_INT,
	AT_FLOAT,
	AT_BOOL,
	AT_STRING,
	AT_VOID,
	AT_TIME,
	AT_COLOR, 
	AT_VECTOR2,
	AT_VECTOR3,
	AT_VECTOR4,
	AT_QANGLE,
	AT_QUATERNION,
	AT_VMATRIX,

	AT_FIRST_ARRAY_TYPE,

	AT_ELEMENT_ARRAY = AT_FIRST_ARRAY_TYPE,
	AT_INT_ARRAY,
	AT_FLOAT_ARRAY,
	AT_BOOL_ARRAY,
	AT_STRING_ARRAY,
	AT_VOID_ARRAY,
	AT_TIME_ARRAY,
	AT_COLOR_ARRAY,
	AT_VECTOR2_ARRAY,
	AT_VECTOR3_ARRAY,
	AT_VECTOR4_ARRAY,
	AT_QANGLE_ARRAY,
	AT_QUATERNION_ARRAY,
	AT_VMATRIX_ARRAY,
	AT_TYPE_COUNT,

	AT_TYPE_INVALID,
};

const char* GetTypeString(DmAttributeType_t type);

inline bool IsValueType(DmAttributeType_t type)
{
	return type >= AT_FIRST_VALUE_TYPE && type < AT_FIRST_ARRAY_TYPE;
}

inline bool IsArrayType(DmAttributeType_t type)
{
	return type >= AT_FIRST_ARRAY_TYPE && type < AT_TYPE_COUNT;
}

inline bool IsTopological(DmAttributeType_t type)
{
	return type == AT_ELEMENT || type == AT_ELEMENT_ARRAY;
}

inline DmAttributeType_t ValueTypeToArrayType(DmAttributeType_t type)
{
	Assert(IsValueType(type));
	return (DmAttributeType_t)((type - AT_FIRST_VALUE_TYPE) + AT_FIRST_ARRAY_TYPE);
}

inline DmAttributeType_t ArrayTypeToValueType(DmAttributeType_t type)
{
	Assert(IsArrayType(type));
	return (DmAttributeType_t)((type - AT_FIRST_ARRAY_TYPE) + AT_FIRST_VALUE_TYPE);
}

inline int NumComponents(DmAttributeType_t type)
{
	switch (type)
	{
	case AT_BOOL:
	case AT_INT:
	case AT_FLOAT:
	case AT_TIME:
		return 1;

	case AT_VECTOR2:
		return 2;

	case AT_VECTOR3:
	case AT_QANGLE:
		return 3;

	case AT_COLOR: 
	case AT_VECTOR4:
	case AT_QUATERNION:
		return 4;

	case AT_VMATRIX:
		return 16;

	case AT_ELEMENT:
	case AT_STRING:
	case AT_VOID:
	default:
		return 0;
	}
}

template< typename T >
inline float GetComponent(const T& value, int i)
{
	Assert(0);
	return 0.0f;
}

template <> inline float GetComponent(const bool& value, int i)
{
	Assert(i == 0);
	return value ? 1.0f : 0.0f;
}

template <> inline float GetComponent(const int& value, int i)
{
	Assert(i == 0);
	return float(value);
}

template <> inline float GetComponent(const float& value, int i)
{
	Assert(i == 0);
	return value;
}

template <> inline float GetComponent(const DmeTime_t& value, int i)
{
	Assert(i == 0);
	return value.GetSeconds();
}

template <> inline float GetComponent(const Vector2D& value, int i)
{
	return value[i];
}

template <> inline float GetComponent(const Vector& value, int i)
{
	return value[i];
}

template <> inline float GetComponent(const QAngle& value, int i)
{
	return value[i];
}

template <> inline float GetComponent(const Color& value, int i)
{
	return value[i];
}

template <> inline float GetComponent(const Vector4D& value, int i)
{
	return value[i];
}

template <> inline float GetComponent(const Quaternion& value, int i)
{
	return value[i];
}

template <> inline float GetComponent(const VMatrix& value, int i)
{
	return value.Base()[i];
}


template <typename T>
class CDmAttributeInfo
{
private:
	enum { ATTRIBUTE_TYPE = AT_TYPE_INVALID };

	typedef T StorageType_t;
	friend class CDmxAttribute;

	static DmAttributeType_t AttributeType()
	{
		return AT_TYPE_INVALID;
	}

	static const char* AttributeTypeName()
	{
		return "invalid";
	}

	static void SetDefaultValue(T& value)
	{
		Assert(0);
	}
};


template <>
class CDmAttributeInfo< DmUnknownAttribute_t >
{
public:
	enum { ATTRIBUTE_TYPE = AT_UNKNOWN };

	typedef DmUnknownAttribute_t StorageType_t;

	static DmAttributeType_t AttributeType()
	{
		return AT_UNKNOWN;
	}

	static const char* AttributeTypeName()
	{
		return "unknown";
	}

	static void SetDefaultValue(DmUnknownAttribute_t& value)
	{
		Assert(0);
	}
};


#define DECLARE_ATTRIBUTE_TYPE_INTERNAL( _className, _storageType, _attributeType, _attributeName, _defaultSetStatement ) \
	template< > class CDmAttributeInfo< _className >						\
	{																		\
	public:																	\
		enum { ATTRIBUTE_TYPE = _attributeType };							\
		typedef _storageType StorageType_t;									\
		static DmAttributeType_t AttributeType() { return _attributeType; }	\
		static const char *AttributeTypeName() { return _attributeName; }	\
		static void SetDefaultValue( _className& value ) { _defaultSetStatement }	\
	};																		\

#define DECLARE_ATTRIBUTE_ARRAY_TYPE_INTERNAL( _className, _storageType, _attributeType, _attributeName ) \
	template< > class CDmAttributeInfo< CUtlVector<_className> >				\
	{																			\
	public:																		\
		enum { ATTRIBUTE_TYPE = _attributeType };								\
		typedef _storageType StorageType_t;										\
		static DmAttributeType_t AttributeType() { return _attributeType; }		\
		static const char *AttributeTypeName() { return _attributeName; }		\
		static void SetDefaultValue( CUtlVector< _className >& value ) { value.RemoveAll(); }	\
	};																			\

#define DECLARE_ATTRIBUTE_TYPE( _className, _attributeType, _attributeName, _defaultSetStatement ) \
	DECLARE_ATTRIBUTE_TYPE_INTERNAL( _className, _className, _attributeType, _attributeName, _defaultSetStatement )

#define DECLARE_ATTRIBUTE_ARRAY_TYPE( _className, _attributeType, _attributeName )\
	DECLARE_ATTRIBUTE_ARRAY_TYPE_INTERNAL( _className, CUtlVector< _className >, _attributeType, _attributeName )

DECLARE_ATTRIBUTE_TYPE(int, AT_INT, "int", value = 0; )
DECLARE_ATTRIBUTE_TYPE(float, AT_FLOAT, "float", value = 0.0f; )
DECLARE_ATTRIBUTE_TYPE(bool, AT_BOOL, "bool", value = false; )
DECLARE_ATTRIBUTE_TYPE(Color, AT_COLOR, "color", value.SetColor(0, 0, 0, 255); )
DECLARE_ATTRIBUTE_TYPE(Vector2D, AT_VECTOR2, "vector2", value.Init(0.0f, 0.0f); )
DECLARE_ATTRIBUTE_TYPE(Vector, AT_VECTOR3, "vector3", value.Init(0.0f, 0.0f, 0.0f); )
DECLARE_ATTRIBUTE_TYPE(Vector4D, AT_VECTOR4, "vector4", value.Init(0.0f, 0.0f, 0.0f, 0.0f); )
DECLARE_ATTRIBUTE_TYPE(QAngle, AT_QANGLE, "qangle", value.Init(0.0f, 0.0f, 0.0f); )
DECLARE_ATTRIBUTE_TYPE(Quaternion, AT_QUATERNION, "quaternion", value.Init(0.0f, 0.0f, 0.0f, 1.0f); )
DECLARE_ATTRIBUTE_TYPE(VMatrix, AT_VMATRIX, "matrix", MatrixSetIdentity(value); )
DECLARE_ATTRIBUTE_TYPE(CUtlSymbolLarge, AT_STRING, "string", value = UTL_INVAL_SYMBOL_LARGE; )
DECLARE_ATTRIBUTE_TYPE(CUtlBinaryBlock, AT_VOID, "binary", value.Set(NULL, 0); )
DECLARE_ATTRIBUTE_TYPE(DmeTime_t, AT_TIME, "time", value.SetTenthsOfMS(0); )
DECLARE_ATTRIBUTE_TYPE_INTERNAL(DmElementHandle_t, DmElementAttribute_t, AT_ELEMENT, "element", value = DMELEMENT_HANDLE_INVALID; )

DECLARE_ATTRIBUTE_ARRAY_TYPE(int, AT_INT_ARRAY, "int_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(float, AT_FLOAT_ARRAY, "float_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(bool, AT_BOOL_ARRAY, "bool_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(Color, AT_COLOR_ARRAY, "color_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(Vector2D, AT_VECTOR2_ARRAY, "vector2_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(Vector, AT_VECTOR3_ARRAY, "vector3_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(Vector4D, AT_VECTOR4_ARRAY, "vector4_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(QAngle, AT_QANGLE_ARRAY, "qangle_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(Quaternion, AT_QUATERNION_ARRAY, "quaternion_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(VMatrix, AT_VMATRIX_ARRAY, "matrix_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(CUtlSymbolLarge, AT_STRING_ARRAY, "string_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(CUtlBinaryBlock, AT_VOID_ARRAY, "binary_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE(DmeTime_t, AT_TIME_ARRAY, "time_array")
DECLARE_ATTRIBUTE_ARRAY_TYPE_INTERNAL(DmElementHandle_t, DmElementArray_t, AT_ELEMENT_ARRAY, "element_array")


#endif  