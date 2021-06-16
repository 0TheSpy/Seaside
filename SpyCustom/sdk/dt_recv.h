#ifndef DATATABLE_RECV_H
#define DATATABLE_RECV_H

#ifdef _WIN32
#pragma once
#endif

#include "dt_common.h"
#include "dbg.h"


#define ADDRESSPROXY_NONE	-1


class RecvTable;
class RecvProp;


class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;		    

	DVariant		m_Value;			       

	int				m_iElement;			     

	int				m_ObjectID;			     
};


typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

typedef void (*ArrayLengthRecvProxyFn)(void* pStruct, int objectID, int currentArrayLength);


typedef void (*DataTableRecvVarProxyFn)(const RecvProp* pProp, void** pOut, void* pData, int objectID);


class CStandardRecvProxies
{
public:
	CStandardRecvProxies();

	RecvVarProxyFn m_Int32ToInt8;
	RecvVarProxyFn m_Int32ToInt16;
	RecvVarProxyFn m_Int32ToInt32;
	RecvVarProxyFn m_FloatToFloat;
	RecvVarProxyFn m_VectorToVector;
#ifdef SUPPORTS_INT64
	RecvVarProxyFn m_Int64ToInt64;
#endif
};
extern CStandardRecvProxies g_StandardRecvProxies;


class CRecvDecoder;


class RecvProp
{
public:
	RecvProp();

	void					InitArray(int nElements, int elementStride);

	int						GetNumElements() const;
	void					SetNumElements(int nElements);

	int						GetElementStride() const;
	void					SetElementStride(int stride);

	int						GetFlags() const;

	const char* GetName() const;
	SendPropType			GetType() const;

	RecvTable* GetDataTable() const;
	void					SetDataTable(RecvTable* pTable);

	RecvVarProxyFn			GetProxyFn() const;
	void					SetProxyFn(RecvVarProxyFn fn);

	DataTableRecvVarProxyFn	GetDataTableProxyFn() const;
	void					SetDataTableProxyFn(DataTableRecvVarProxyFn fn);

	int						GetOffset() const;
	void					SetOffset(int o);

	RecvProp* GetArrayProp() const;
	void					SetArrayProp(RecvProp* pProp);

	void					SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy);
	ArrayLengthRecvProxyFn	GetArrayLengthProxy() const;

	bool					IsInsideArray() const;
	void					SetInsideArray();

	const void* GetExtraData() const;
	void				SetExtraData(const void* pData);

	const char* GetParentArrayPropName();
	void				SetParentArrayPropName(const char* pArrayPropName);

public:

	const char* m_pVarName;
	SendPropType			m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;


	bool					m_bInsideArray;		             

	const void* m_pExtraData;

	RecvProp* m_pArrayProp;
	ArrayLengthRecvProxyFn	m_ArrayLengthProxy;

	RecvVarProxyFn			m_ProxyFn;
	DataTableRecvVarProxyFn	m_DataTableProxyFn;	  

	RecvTable* m_pDataTable;		  
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	const char* m_pParentArrayPropName;
};


class RecvTable
{
public:

	typedef RecvProp	PropType;

	RecvTable();
	RecvTable(RecvProp* pProps, int nProps, const char* pNetTableName);
	~RecvTable();

	void		Construct(RecvProp* pProps, int nProps, const char* pNetTableName);

	int			GetNumProps();
	RecvProp* GetProp(int i);

	const char* GetName();

	void		SetInitialized(bool bInitialized);
	bool		IsInitialized() const;

	void		SetInMainList(bool bInList);
	bool		IsInMainList() const;


public:

	RecvProp* m_pProps;
	int				m_nProps;

	CRecvDecoder* m_pDecoder;

	const char* m_pNetTableName;	       


	bool			m_bInitialized;
	bool			m_bInMainList;
};


inline int RecvTable::GetNumProps()
{
	return m_nProps;
}

inline RecvProp* RecvTable::GetProp(int i)
{
	Assert(i >= 0 && i < m_nProps);
	return &m_pProps[i];
}

inline const char* RecvTable::GetName()
{
	return m_pNetTableName;
}

inline void RecvTable::SetInitialized(bool bInitialized)
{
	m_bInitialized = bInitialized;
}

inline bool RecvTable::IsInitialized() const
{
	return m_bInitialized;
}

inline void RecvTable::SetInMainList(bool bInList)
{
	m_bInMainList = bInList;
}

inline bool RecvTable::IsInMainList() const
{
	return m_bInMainList;
}


#define BEGIN_RECV_TABLE(className, tableName) \
	BEGIN_RECV_TABLE_NOBASE(className, tableName) \
		RecvPropDataTable("baseclass", 0, 0, className::BaseClass::m_pClassRecvTable, DataTableRecvProxy_StaticDataTable),

#define BEGIN_RECV_TABLE_NOBASE(className, tableName) \
	template <typename T> int ClientClassInit(T *); \
	namespace tableName { \
		struct ignored; \
	} \
	template <> int ClientClassInit<tableName::ignored>(tableName::ignored *); \
	namespace tableName {	\
		RecvTable g_RecvTable; \
		int g_RecvTableInit = ClientClassInit((tableName::ignored *)NULL); \
	} \
	template <> int ClientClassInit<tableName::ignored>(tableName::ignored *) \
	{ \
		typedef className currentRecvDTClass; \
		const char *pRecvTableName = #tableName; \
		RecvTable &RecvTable = tableName::g_RecvTable; \
		static RecvProp RecvProps[] = { \
			RecvPropInt("should_never_see_this", 0, sizeof(int)),		              

#define END_RECV_TABLE() \
			}; \
		RecvTable.Construct(RecvProps+1, sizeof(RecvProps) / sizeof(RecvProp) - 1, pRecvTableName); \
		return 1; \
	}


#define RECVINFO(varName)						#varName, offsetof(currentRecvDTClass, varName), sizeof(((currentRecvDTClass*)0)->varName)
#define RECVINFO_NAME(varName, remoteVarName)	#remoteVarName, offsetof(currentRecvDTClass, varName), sizeof(((currentRecvDTClass*)0)->varName)
#define RECVINFO_STRING(varName)				#varName, offsetof(currentRecvDTClass, varName), STRINGBUFSIZE(currentRecvDTClass, varName)
#define RECVINFO_BASECLASS(tableName)			RecvPropDataTable("this", 0, 0, &REFERENCE_RECV_TABLE(tableName))
#define RECVINFO_ARRAY(varName)					#varName, offsetof(currentRecvDTClass, varName), sizeof(((currentRecvDTClass*)0)->varName[0]), sizeof(((currentRecvDTClass*)0)->varName)/sizeof(((currentRecvDTClass*)0)->varName[0])

#define RECVINFO_NOSIZE(varName)				#varName, offsetof(currentRecvDTClass, varName)
#define RECVINFO_DT(varName)					RECVINFO_NOSIZE(varName)
#define RECVINFO_DTNAME(varName,remoteVarName)	#remoteVarName, offsetof(currentRecvDTClass, varName)


void RecvProxy_FloatToFloat(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_VectorToVector(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_VectorXYToVectorXY(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_QuaternionToQuaternion(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_Int32ToInt8(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_Int32ToInt16(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_StringToString(const CRecvProxyData* pData, void* pStruct, void* pOut);
void RecvProxy_Int32ToInt32(const CRecvProxyData* pData, void* pStruct, void* pOut);
#ifdef SUPPORTS_INT64
void RecvProxy_Int64ToInt64(const CRecvProxyData* pData, void* pStruct, void* pOut);
#endif

void DataTableRecvProxy_StaticDataTable(const RecvProp* pProp, void** pOut, void* pData, int objectID);

void DataTableRecvProxy_PointerDataTable(const RecvProp* pProp, void** pOut, void* pData, int objectID);


RecvProp RecvPropFloat(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	              
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_FloatToFloat
);

RecvProp RecvPropVector(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	              
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_VectorToVector
);

RecvProp RecvPropVectorXY(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	              
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_VectorXYToVectorXY
);

#define RecvPropQAngles RecvPropVector

#if 0                       

RecvProp RecvPropQuaternion(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	              
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_QuaternionToQuaternion
);
#endif

RecvProp RecvPropInt(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	              
	int flags = 0,
	RecvVarProxyFn varProxy = 0
);

RecvProp RecvPropString(
	const char* pVarName,
	int offset,
	int bufferSize,
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_StringToString
);

RecvProp RecvPropDataTable(
	const char* pVarName,
	int offset,
	int flags,
	RecvTable* pTable,
	DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable
);

RecvProp RecvPropArray3(
	const char* pVarName,
	int offset,
	int sizeofVar,
	int elements,
	RecvProp pArrayProp,
	DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable
);

RecvProp InternalRecvPropArray(
	const int elementCount,
	const int elementStride,
	const char* pName,
	ArrayLengthRecvProxyFn proxy
);


#define RecvPropVirtualArray( arrayLengthProxy, maxArrayLength, varTemplate, propertyName ) \
	varTemplate, \
	InternalRecvPropArray( \
		maxArrayLength, \
		0, \
		#propertyName, \
		arrayLengthProxy \
		)


#define RecvPropVariableLengthArray( arrayLengthProxy, varTemplate, arrayName )			\
	varTemplate,										\
	InternalRecvPropArray(								\
		sizeof(((currentRecvDTClass*)0)->arrayName) / PROPSIZEOF(currentRecvDTClass, arrayName[0]), \
		PROPSIZEOF(currentRecvDTClass, arrayName[0]),	\
		#arrayName,										\
		arrayLengthProxy								\
		)


#define RecvPropArray( varTemplate, arrayName )			\
	RecvPropVariableLengthArray( 0, varTemplate, arrayName )


#define RecvPropArray2( arrayLengthProxy, varTemplate, elementCount, elementStride, arrayName )		\
	varTemplate,																	\
	InternalRecvPropArray( elementCount, elementStride, #arrayName, arrayLengthProxy )


inline void RecvProp::InitArray(int nElements, int elementStride)
{
	m_RecvType = DPT_Array;
	m_nElements = nElements;
	m_ElementStride = elementStride;
}

inline int RecvProp::GetNumElements() const
{
	return m_nElements;
}

inline void RecvProp::SetNumElements(int nElements)
{
	m_nElements = nElements;
}

inline int RecvProp::GetElementStride() const
{
	return m_ElementStride;
}

inline void RecvProp::SetElementStride(int stride)
{
	m_ElementStride = stride;
}

inline int RecvProp::GetFlags() const
{
	return m_Flags;
}

inline const char* RecvProp::GetName() const
{
	return m_pVarName;
}

inline SendPropType RecvProp::GetType() const
{
	return m_RecvType;
}

inline RecvTable* RecvProp::GetDataTable() const
{
	return m_pDataTable;
}

inline void RecvProp::SetDataTable(RecvTable* pTable)
{
	m_pDataTable = pTable;
}

inline RecvVarProxyFn RecvProp::GetProxyFn() const
{
	return m_ProxyFn;
}

inline void RecvProp::SetProxyFn(RecvVarProxyFn fn)
{
	m_ProxyFn = fn;
}

inline DataTableRecvVarProxyFn RecvProp::GetDataTableProxyFn() const
{
	return m_DataTableProxyFn;
}

inline void RecvProp::SetDataTableProxyFn(DataTableRecvVarProxyFn fn)
{
	m_DataTableProxyFn = fn;
}

inline int RecvProp::GetOffset() const
{
	return m_Offset;
}

inline void RecvProp::SetOffset(int o)
{
	m_Offset = o;
}

inline RecvProp* RecvProp::GetArrayProp() const
{
	return m_pArrayProp;
}

inline void RecvProp::SetArrayProp(RecvProp* pProp)
{
	m_pArrayProp = pProp;
}

inline void RecvProp::SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy)
{
	m_ArrayLengthProxy = proxy;
}

inline ArrayLengthRecvProxyFn RecvProp::GetArrayLengthProxy() const
{
	return m_ArrayLengthProxy;
}

inline bool RecvProp::IsInsideArray() const
{
	return m_bInsideArray;
}

inline void RecvProp::SetInsideArray()
{
	m_bInsideArray = true;
}

inline const void* RecvProp::GetExtraData() const
{
	return m_pExtraData;
}

inline void RecvProp::SetExtraData(const void* pData)
{
	m_pExtraData = pData;
}

inline const char* RecvProp::GetParentArrayPropName()
{
	return m_pParentArrayPropName;
}

inline void	RecvProp::SetParentArrayPropName(const char* pArrayPropName)
{
	m_pParentArrayPropName = pArrayPropName;
}

#endif  