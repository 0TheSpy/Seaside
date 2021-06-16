#ifndef DATATABLE_COMMON_H
#define DATATABLE_COMMON_H

#ifdef _WIN32
#pragma once
#endif	

#include "basetypes.h"
#include "dbg.h"
#include "strtools.h"
#include <stddef.h>

#ifdef LINUX
#undef offsetof
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
#endif

#define MAX_DATATABLES		1024	      
#define MAX_DATATABLE_PROPS	4096

#define MAX_ARRAY_ELEMENTS	2048		         

#define HIGH_DEFAULT		-121121.121121f

#define BITS_FULLRES	-1	         
#define BITS_WORLDCOORD	-2	     

#define DT_MAX_STRING_BITS			9
#define DT_MAX_STRING_BUFFERSIZE	(1<<DT_MAX_STRING_BITS)	         

#define STRINGBUFSIZE(className, varName)	sizeof( ((className*)0)->varName )

#define PROPSIZEOF(className, varName)		sizeof(((className*)0)->varName)


#define SPROP_UNSIGNED			(1<<0)	   

#define SPROP_COORD				(1<<1)	            
#define SPROP_NOSCALE			(1<<2)	            

#define SPROP_ROUNDDOWN			(1<<3)	            

#define SPROP_ROUNDUP			(1<<4)	            

#define SPROP_NORMAL			(1<<5)	               

#define SPROP_EXCLUDE			(1<<6)	                

#define SPROP_XYZE				(1<<7)	     

#define SPROP_INSIDEARRAY		(1<<8)	                 
#define SPROP_PROXY_ALWAYS_YES	(1<<9)	            
#define SPROP_CHANGES_OFTEN		(1<<10)	                 

#define SPROP_IS_A_VECTOR_ELEM	(1<<11)	      

#define SPROP_COLLAPSIBLE		(1<<12)	                
#define SPROP_COORD_MP					(1<<13)         
#define SPROP_COORD_MP_LOWPRECISION 	(1<<14)                     
#define SPROP_COORD_MP_INTEGRAL			(1<<15)         

#define SPROP_VARINT					SPROP_NORMAL	                   

#define SPROP_NUMFLAGBITS_NETWORKED		16

#define SPROP_ENCODED_AGAINST_TICKCOUNT	(1<<16)

#define SPROP_NUMFLAGBITS				17

#define SIZEOF_IGNORE		-1


#define EXTERN_SEND_TABLE(tableName)	namespace tableName {extern SendTable g_SendTable;}
#define EXTERN_RECV_TABLE(tableName)	namespace tableName {extern RecvTable g_RecvTable;}

#define REFERENCE_SEND_TABLE(tableName)	tableName::g_SendTable
#define REFERENCE_RECV_TABLE(tableName)	tableName::g_RecvTable


class SendProp;

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,          
	DPT_String,
	DPT_Array,	          
	DPT_DataTable,
#if 0                       
	DPT_Quaternion,
#endif

#ifdef SUPPORTS_INT64
	DPT_Int64,
#endif

	DPT_NUMSendPropTypes

} SendPropType;


class DVariant
{
public:
	DVariant() { m_Type = DPT_Float; }
	DVariant(float val) { m_Type = DPT_Float; m_Float = val; }

	const char* ToString()
	{
		static char text[128];

		switch (m_Type)
		{
		case DPT_Int:
			Q_snprintf(text, sizeof(text), "%i", m_Int);
			break;
		case DPT_Float:
			Q_snprintf(text, sizeof(text), "%.3f", m_Float);
			break;
		case DPT_Vector:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2]);
			break;
		case DPT_VectorXY:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f)",
				m_Vector[0], m_Vector[1]);
			break;
#if 0                       
		case DPT_Quaternion:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f %.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3]);
			break;
#endif
		case DPT_String:
			if (m_pString)
				return m_pString;
			else
				return "NULL";
			break;
		case DPT_Array:
			Q_snprintf(text, sizeof(text), "Array");
			break;
		case DPT_DataTable:
			Q_snprintf(text, sizeof(text), "DataTable");
			break;
#ifdef SUPPORTS_INT64
		case DPT_Int64:
			Q_snprintf(text, sizeof(text), "%I64d", m_Int64);
			break;
#endif
		default:
			Q_snprintf(text, sizeof(text), "DVariant type %i unknown", m_Type);
			break;
		}

		return text;
	}

	union
	{
		float	m_Float;
		int		m_Int;
		const char* m_pString;
		void* m_pData;	  
#if 0                       
		float	m_Vector[4];
#else
		float	m_Vector[3];
#endif

		int64	m_Int64;
	};
	SendPropType	m_Type;
};


inline int NumBitsForCount(int nMaxElements)
{
	int nBits = 0;
	while (nMaxElements > 0)
	{
		++nBits;
		nMaxElements >>= 1;
	}
	return nBits;
}


#endif  