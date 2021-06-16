#ifndef DATATABLE_SEND_H
#define DATATABLE_SEND_H

#ifdef _WIN32
#pragma once
#endif

#include "dt_common.h"
#include "dbg.h"
#include "const.h"
#include "bitvec.h"


typedef void (*SendVarProxyFn)(const SendProp* pProp, const void* pStructBase, const void* pData, DVariant* pOut, int iElement, int objectID);

class CSendProxyRecipients;

typedef void* (*SendTableProxyFn)(
	const SendProp* pProp,
	const void* pStructBase,
	const void* pData,
	CSendProxyRecipients* pRecipients,
	int objectID);


class CNonModifiedPointerProxy
{
public:
	CNonModifiedPointerProxy(SendTableProxyFn fn);

public:

	SendTableProxyFn m_Fn;
	CNonModifiedPointerProxy* m_pNext;
};


#define REGISTER_SEND_PROXY_NON_MODIFIED_POINTER( sendProxyFn ) static CNonModifiedPointerProxy __proxy_##sendProxyFn( sendProxyFn );


class CStandardSendProxiesV1
{
public:
	CStandardSendProxiesV1();

	SendVarProxyFn m_Int8ToInt32;
	SendVarProxyFn m_Int16ToInt32;
	SendVarProxyFn m_Int32ToInt32;

	SendVarProxyFn m_UInt8ToInt32;
	SendVarProxyFn m_UInt16ToInt32;
	SendVarProxyFn m_UInt32ToInt32;

	SendVarProxyFn m_FloatToFloat;
	SendVarProxyFn m_VectorToVector;

#ifdef SUPPORTS_INT64
	SendVarProxyFn m_Int64ToInt64;
	SendVarProxyFn m_UInt64ToInt64;
#endif
};

class CStandardSendProxies : public CStandardSendProxiesV1
{
public:
	CStandardSendProxies();

	SendTableProxyFn m_DataTableToDataTable;
	SendTableProxyFn m_SendLocalDataTable;
	CNonModifiedPointerProxy** m_ppNonModifiedPointerProxies;
};

extern CStandardSendProxies g_StandardSendProxies;


#define MAX_DATATABLE_PROXIES	32

class CSendProxyRecipients
{
public:
	void	SetAllRecipients();					            
	void	ClearAllRecipients();

	void	SetRecipient(int iClient);		               
	void	ClearRecipient(int iClient);

	void	SetOnly(int iClient);

public:
	CBitVec< ABSOLUTE_PLAYER_LIMIT >	m_Bits;
};

inline void CSendProxyRecipients::SetAllRecipients()
{
	m_Bits.SetAll();
}

inline void CSendProxyRecipients::ClearAllRecipients()
{
	m_Bits.ClearAll();
}

inline void CSendProxyRecipients::SetRecipient(int iClient)
{
	m_Bits.Set(iClient);
}

inline void	CSendProxyRecipients::ClearRecipient(int iClient)
{
	m_Bits.Clear(iClient);
}

inline void CSendProxyRecipients::SetOnly(int iClient)
{
	m_Bits.ClearAll();
	m_Bits.Set(iClient);
}



typedef int (*ArrayLengthSendProxyFn)(const void* pStruct, int objectID);



class RecvProp;
class SendTable;
class CSendTablePrecalc;


#define DATATABLE_PROXY_INDEX_NOPROXY	255
#define DATATABLE_PROXY_INDEX_INVALID	254

class SendProp
{
public:
	SendProp();
	virtual				~SendProp();

	void				Clear();

	int					GetOffset() const;
	void				SetOffset(int i);

	SendVarProxyFn		GetProxyFn() const;
	void				SetProxyFn(SendVarProxyFn f);

	SendTableProxyFn	GetDataTableProxyFn() const;
	void				SetDataTableProxyFn(SendTableProxyFn f);

	SendTable* GetDataTable() const;
	void				SetDataTable(SendTable* pTable);

	char const* GetExcludeDTName() const;

	const char* GetParentArrayPropName() const;
	void				SetParentArrayPropName(char* pArrayPropName);

	const char* GetName() const;

	bool				IsSigned() const;

	bool				IsExcludeProp() const;

	bool				IsInsideArray() const;	      
	void				SetInsideArray();

	void				SetArrayProp(SendProp* pProp);
	SendProp* GetArrayProp() const;

	void					SetArrayLengthProxy(ArrayLengthSendProxyFn fn);
	ArrayLengthSendProxyFn	GetArrayLengthProxy() const;

	int					GetNumElements() const;
	void				SetNumElements(int nElements);

	int					GetNumArrayLengthBits() const;

	int					GetElementStride() const;

	SendPropType		GetType() const;

	int					GetFlags() const;
	void				SetFlags(int flags);

	const void* GetExtraData() const;
	void				SetExtraData(const void* pData);

public:

	RecvProp* m_pMatchingRecvProp;	        
	SendPropType	m_Type;
	int				m_nBits;
	float			m_fLowValue;
	float			m_fHighValue;

	SendProp* m_pArrayProp;					              
	ArrayLengthSendProxyFn	m_ArrayLengthProxy;	      

	int				m_nElements;		             
	int				m_ElementStride;	     

	const char* m_pExcludeDTName;			                   
	const char* m_pParentArrayPropName;

	const char* m_pVarName;
	float			m_fHighLowMul;

private:

	int					m_Flags;				  

	SendVarProxyFn		m_ProxyFn;				   
	SendTableProxyFn	m_DataTableProxyFn;		   

	SendTable* m_pDataTable;

	int					m_Offset;

	const void* m_pExtraData;
};


inline int SendProp::GetOffset() const
{
	return m_Offset;
}

inline void SendProp::SetOffset(int i)
{
	m_Offset = i;
}

inline SendVarProxyFn SendProp::GetProxyFn() const
{
	Assert(m_Type != DPT_DataTable);
	return m_ProxyFn;
}

inline void SendProp::SetProxyFn(SendVarProxyFn f)
{
	m_ProxyFn = f;
}

inline SendTableProxyFn SendProp::GetDataTableProxyFn() const
{
	Assert(m_Type == DPT_DataTable);
	return m_DataTableProxyFn;
}

inline void SendProp::SetDataTableProxyFn(SendTableProxyFn f)
{
	m_DataTableProxyFn = f;
}

inline SendTable* SendProp::GetDataTable() const
{
	return m_pDataTable;
}

inline void SendProp::SetDataTable(SendTable* pTable)
{
	m_pDataTable = pTable;
}

inline char const* SendProp::GetExcludeDTName() const
{
	return m_pExcludeDTName;
}

inline const char* SendProp::GetParentArrayPropName() const
{
	return m_pParentArrayPropName;
}

inline void	SendProp::SetParentArrayPropName(char* pArrayPropName)
{
	Assert(!m_pParentArrayPropName);
	m_pParentArrayPropName = pArrayPropName;
}

inline const char* SendProp::GetName() const
{
	return m_pVarName;
}


inline bool SendProp::IsSigned() const
{
	return !(m_Flags & SPROP_UNSIGNED);
}

inline bool SendProp::IsExcludeProp() const
{
	return (m_Flags & SPROP_EXCLUDE) != 0;
}

inline bool	SendProp::IsInsideArray() const
{
	return (m_Flags & SPROP_INSIDEARRAY) != 0;
}

inline void SendProp::SetInsideArray()
{
	m_Flags |= SPROP_INSIDEARRAY;
}

inline void SendProp::SetArrayProp(SendProp* pProp)
{
	m_pArrayProp = pProp;
}

inline SendProp* SendProp::GetArrayProp() const
{
	return m_pArrayProp;
}

inline void SendProp::SetArrayLengthProxy(ArrayLengthSendProxyFn fn)
{
	m_ArrayLengthProxy = fn;
}

inline ArrayLengthSendProxyFn SendProp::GetArrayLengthProxy() const
{
	return m_ArrayLengthProxy;
}

inline int SendProp::GetNumElements() const
{
	return m_nElements;
}

inline void SendProp::SetNumElements(int nElements)
{
	m_nElements = nElements;
}

inline int SendProp::GetElementStride() const
{
	return m_ElementStride;
}

inline SendPropType SendProp::GetType() const
{
	return m_Type;
}

inline int SendProp::GetFlags() const
{
	return m_Flags;
}

inline void SendProp::SetFlags(int flags)
{
	Assert(!(flags & ~((1 << SPROP_NUMFLAGBITS) - 1)));
	m_Flags = flags;
}

inline const void* SendProp::GetExtraData() const
{
	return m_pExtraData;
}

inline void SendProp::SetExtraData(const void* pData)
{
	m_pExtraData = pData;
}


class SendTable
{
public:

	typedef SendProp PropType;

	SendTable();
	SendTable(SendProp* pProps, int nProps, const char* pNetTableName);
	~SendTable();

	void		Construct(SendProp* pProps, int nProps, const char* pNetTableName);

	const char* GetName() const;

	int			GetNumProps() const;
	SendProp* GetProp(int i);

	bool		IsInitialized() const;
	void		SetInitialized(bool bInitialized);

	void		SetWriteFlag(bool bHasBeenWritten);
	bool		GetWriteFlag() const;

	bool		HasPropsEncodedAgainstTickCount() const;
	void		SetHasPropsEncodedAgainstTickcount(bool bState);

public:

	SendProp* m_pProps;
	int			m_nProps;

	const char* m_pNetTableName;	       

	CSendTablePrecalc* m_pPrecalc;


protected:
	bool		m_bInitialized : 1;
	bool		m_bHasBeenWritten : 1;
	bool		m_bHasPropsEncodedAgainstCurrentTickCount : 1;     
};


inline const char* SendTable::GetName() const
{
	return m_pNetTableName;
}


inline int SendTable::GetNumProps() const
{
	return m_nProps;
}


inline SendProp* SendTable::GetProp(int i)
{
	Assert(i >= 0 && i < m_nProps);
	return &m_pProps[i];
}


inline bool SendTable::IsInitialized() const
{
	return m_bInitialized;
}


inline void SendTable::SetInitialized(bool bInitialized)
{
	m_bInitialized = bInitialized;
}


inline bool SendTable::GetWriteFlag() const
{
	return m_bHasBeenWritten;
}


inline void SendTable::SetWriteFlag(bool bHasBeenWritten)
{
	m_bHasBeenWritten = bHasBeenWritten;
}

inline bool SendTable::HasPropsEncodedAgainstTickCount() const
{
	return m_bHasPropsEncodedAgainstCurrentTickCount;
}

inline void SendTable::SetHasPropsEncodedAgainstTickcount(bool bState)
{
	m_bHasPropsEncodedAgainstCurrentTickCount = bState;
}

#define BEGIN_SEND_TABLE(className, tableName) \
	BEGIN_SEND_TABLE_NOBASE(className, tableName) \
		SendPropDataTable("baseclass", 0, className::BaseClass::m_pClassSendTable, SendProxy_DataTableToDataTable),

#define BEGIN_SEND_TABLE_NOBASE(className, tableName) \
	template <typename T> int ServerClassInit(T *); \
	namespace tableName { \
		struct ignored; \
	} \
	template <> int ServerClassInit<tableName::ignored>(tableName::ignored *); \
	namespace tableName { \
		SendTable g_SendTable;\
		int g_SendTableInit = ServerClassInit((tableName::ignored *)NULL); \
	} \
	template <> int ServerClassInit<tableName::ignored>(tableName::ignored *) \
	{ \
		typedef className currentSendDTClass; \
		static const char *g_pSendTableName = #tableName; \
		SendTable &sendTable = tableName::g_SendTable; \
		static SendProp g_SendProps[] = { \
			SendPropInt("should_never_see_this", 0, sizeof(int)),		              

#define END_SEND_TABLE() \
		};\
		sendTable.Construct(g_SendProps+1, sizeof(g_SendProps) / sizeof(SendProp) - 1, g_pSendTableName);\
		return 1; \
	} 


#define SENDINFO(varName)					#varName, offsetof(currentSendDTClass::MakeANetworkVar_##varName, varName), sizeof(((currentSendDTClass*)0)->varName)
#define SENDINFO_ARRAY(varName)				#varName, offsetof(currentSendDTClass::MakeANetworkVar_##varName, varName), sizeof(((currentSendDTClass*)0)->varName[0])
#define SENDINFO_ARRAY3(varName)			#varName, offsetof(currentSendDTClass::MakeANetworkVar_##varName, varName), sizeof(((currentSendDTClass*)0)->varName[0]), sizeof(((currentSendDTClass*)0)->varName)/sizeof(((currentSendDTClass*)0)->varName[0])
#define SENDINFO_ARRAYELEM(varName, i)		#varName "[" #i "]", offsetof(currentSendDTClass::MakeANetworkVar_##varName, varName[i]), sizeof(((currentSendDTClass*)0)->varName[0])
#define SENDINFO_NETWORKARRAYELEM(varName, i)#varName "[" #i "]", offsetof(currentSendDTClass::MakeANetworkVar_##varName, varName.m_Value[i]), sizeof(((currentSendDTClass*)0)->varName.m_Value[0])

#define SENDINFO_VECTORELEM(varName, i)		#varName "[" #i "]", -(int)offsetof(currentSendDTClass::MakeANetworkVar_##varName, varName.m_Value[i]), sizeof(((currentSendDTClass*)0)->varName.m_Value[0])

#define SENDINFO_STRUCTELEM(varName)		#varName, offsetof(currentSendDTClass, varName), sizeof(((currentSendDTClass*)0)->varName.m_Value)
#define SENDINFO_STRUCTARRAYELEM(varName, i)#varName "[" #i "]", offsetof(currentSendDTClass, varName.m_Value[i]), sizeof(((currentSendDTClass*)0)->varName.m_Value[0])

#define SENDINFO_NOCHECK(varName)						#varName, offsetof(currentSendDTClass, varName), sizeof(((currentSendDTClass*)0)->varName)
#define SENDINFO_STRING_NOCHECK(varName)				#varName, offsetof(currentSendDTClass, varName)
#define SENDINFO_DT(varName)							#varName, offsetof(currentSendDTClass, varName)
#define SENDINFO_DT_NAME(varName, remoteVarName)		#remoteVarName, offsetof(currentSendDTClass, varName)
#define SENDINFO_NAME(varName,remoteVarName)			#remoteVarName, offsetof(currentSendDTClass, varName), sizeof(((currentSendDTClass*)0)->varName)




void SendProxy_QAngles(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
void SendProxy_AngleToFloat(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
void SendProxy_FloatToFloat(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
void SendProxy_VectorToVector(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
void SendProxy_VectorXYToVectorXY(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
#if 0                       
void SendProxy_QuaternionToQuaternion(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
#endif

void SendProxy_Int8ToInt32(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
void SendProxy_Int16ToInt32(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
void SendProxy_Int32ToInt32(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
#ifdef SUPPORTS_INT64
void SendProxy_Int64ToInt64(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
#endif
void SendProxy_StringToString(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);

void* SendProxy_DataTableToDataTable(const SendProp* pProp, const void* pStructBase, const void* pData, CSendProxyRecipients* pRecipients, int objectID);

void* SendProxy_DataTablePtrToDataTable(const SendProp* pProp, const void* pStructBase, const void* pData, CSendProxyRecipients* pRecipients, int objectID);

void* SendProxy_SendLocalDataTable(const SendProp* pProp, const void* pStruct, const void* pVarData, CSendProxyRecipients* pRecipients, int objectID);


SendProp SendPropFloat(
	const char* pVarName,		  
	int offset,					    
	int sizeofVar = SIZEOF_IGNORE,
	int nBits = 32,				       
	int flags = 0,
	float fLowValue = 0.0f,			       
	float fHighValue = HIGH_DEFAULT,	      
	SendVarProxyFn varProxy = SendProxy_FloatToFloat
);

SendProp SendPropVector(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,
	int nBits = 32,					           
	int flags = SPROP_NOSCALE,
	float fLowValue = 0.0f,			       
	float fHighValue = HIGH_DEFAULT,	      
	SendVarProxyFn varProxy = SendProxy_VectorToVector
);

SendProp SendPropVectorXY(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,
	int nBits = 32,					           
	int flags = SPROP_NOSCALE,
	float fLowValue = 0.0f,			       
	float fHighValue = HIGH_DEFAULT,	      
	SendVarProxyFn varProxy = SendProxy_VectorXYToVectorXY
);

#if 0                       
SendProp SendPropQuaternion(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,
	int nBits = 32,					           
	int flags = SPROP_NOSCALE,
	float fLowValue = 0.0f,			       
	float fHighValue = HIGH_DEFAULT,	      
	SendVarProxyFn varProxy = SendProxy_QuaternionToQuaternion
);
#endif

SendProp SendPropAngle(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,
	int nBits = 32,
	int flags = 0,
	SendVarProxyFn varProxy = SendProxy_AngleToFloat
);

SendProp SendPropQAngles(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,
	int nBits = 32,
	int flags = 0,
	SendVarProxyFn varProxy = SendProxy_QAngles
);

SendProp SendPropInt(
	const char* pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	    
	int nBits = -1,					               
	int flags = 0,
	SendVarProxyFn varProxy = 0
);

inline SendProp SendPropModelIndex(const char* pVarName, int offset, int sizeofVar = SIZEOF_IGNORE)
{
	return SendPropInt(pVarName, offset, sizeofVar, SP_MODEL_INDEX_BITS, 0);
}

SendProp SendPropString(
	const char* pVarName,
	int offset,
	int bufferLen,
	int flags = 0,
	SendVarProxyFn varProxy = SendProxy_StringToString);

SendProp SendPropDataTable(
	const char* pVarName,
	int offset,
	SendTable* pTable,
	SendTableProxyFn varProxy = SendProxy_DataTableToDataTable
);

SendProp SendPropArray3(
	const char* pVarName,
	int offset,
	int sizeofVar,
	int elements,
	SendProp pArrayProp,
	SendTableProxyFn varProxy = SendProxy_DataTableToDataTable
);



SendProp InternalSendPropArray(
	const int elementCount,
	const int elementStride,
	const char* pName,
	ArrayLengthSendProxyFn proxy
);


#define SendPropArray( varTemplate, arrayName )			\
	SendPropVariableLengthArray(						\
		0,												\
		varTemplate,									\
		arrayName )

#define SendPropVirtualArray( arrayLengthSendProxy, maxArrayLength, varTemplate, propertyName )	\
	varTemplate,										\
	InternalSendPropArray(								\
		maxArrayLength,									\
		0,												\
		#propertyName,									\
		arrayLengthSendProxy							\
		)


#define SendPropVariableLengthArray( arrayLengthSendProxy, varTemplate, arrayName )	\
	varTemplate,										\
	InternalSendPropArray(								\
		sizeof(((currentSendDTClass*)0)->arrayName) / PROPSIZEOF(currentSendDTClass, arrayName[0]), \
		PROPSIZEOF(currentSendDTClass, arrayName[0]),	\
		#arrayName,										\
		arrayLengthSendProxy							\
		)

#define SendPropArray2( arrayLengthSendProxy, varTemplate, elementCount, elementStride, arrayName )		\
	varTemplate,																	\
	InternalSendPropArray( elementCount, elementStride, #arrayName, arrayLengthSendProxy )




SendProp SendPropExclude(
	const char* pDataTableName,	        
	const char* pPropName		      
);


#endif  