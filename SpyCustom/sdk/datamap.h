#ifndef DATAMAP_H
#define DATAMAP_H
#ifdef _WIN32
#pragma once
#endif

#ifndef VECTOR_H
#include "vector.h"
#endif

#include "utlvector.h"

#include "memdbgon.h"

class SINGLE_INHERITANCE CBaseEntity;
struct inputdata_t;

#define INVALID_TIME (FLT_MAX * -1.0)       

typedef enum _fieldtypes
{
	FIELD_VOID = 0,			    
	FIELD_FLOAT,			    
	FIELD_STRING,			      
	FIELD_VECTOR,			     
	FIELD_QUATERNION,		  
	FIELD_INTEGER,			    
	FIELD_BOOLEAN,			              
	FIELD_SHORT,			   
	FIELD_CHARACTER,		  
	FIELD_COLOR32,			      
	FIELD_EMBEDDED,			                
	FIELD_CUSTOM,			          

	FIELD_CLASSPTR,			  
	FIELD_EHANDLE,			  
	FIELD_EDICT,			  

	FIELD_POSITION_VECTOR,	           
	FIELD_TIME,				          
	FIELD_TICK,				         
	FIELD_MODELNAME,		         
	FIELD_SOUNDNAME,		         

	FIELD_INPUT,			          
	FIELD_FUNCTION,			       

	FIELD_VMATRIX,			       

	FIELD_VMATRIX_WORLDSPACE,                 
	FIELD_MATRIX3X4_WORLDSPACE,	                

	FIELD_INTERVAL,			               
	FIELD_MODELINDEX,		   
	FIELD_MATERIALINDEX,	         

	FIELD_VECTOR2D,			  

	FIELD_TYPECOUNT,		   
} fieldtype_t;


template <int FIELD_TYPE>
class CDatamapFieldSizeDeducer
{
public:
	enum
	{
		SIZE = 0
	};

	static int FieldSize()
	{
		return 0;
	}
};

#define DECLARE_FIELD_SIZE( _fieldType, _fieldSize )	\
	template< > class CDatamapFieldSizeDeducer<_fieldType> { public: enum { SIZE = _fieldSize }; static int FieldSize() { return _fieldSize; } };
#define FIELD_SIZE( _fieldType )	CDatamapFieldSizeDeducer<_fieldType>::SIZE
#define FIELD_BITS( _fieldType )	(FIELD_SIZE( _fieldType ) * 8)

DECLARE_FIELD_SIZE(FIELD_FLOAT, sizeof(float))
DECLARE_FIELD_SIZE(FIELD_STRING, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_VECTOR, 3 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_VECTOR2D, 2 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_QUATERNION, 4 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_INTEGER, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_BOOLEAN, sizeof(char))
DECLARE_FIELD_SIZE(FIELD_SHORT, sizeof(short))
DECLARE_FIELD_SIZE(FIELD_CHARACTER, sizeof(char))
DECLARE_FIELD_SIZE(FIELD_COLOR32, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_CLASSPTR, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_EHANDLE, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_EDICT, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_POSITION_VECTOR, 3 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_TIME, sizeof(float))
DECLARE_FIELD_SIZE(FIELD_TICK, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_MODELNAME, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_SOUNDNAME, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_INPUT, sizeof(int))
#ifdef POSIX
DECLARE_FIELD_SIZE(FIELD_FUNCTION, sizeof(uint64))
#else
DECLARE_FIELD_SIZE(FIELD_FUNCTION, sizeof(int*))
#endif
DECLARE_FIELD_SIZE(FIELD_VMATRIX, 16 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_VMATRIX_WORLDSPACE, 16 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_MATRIX3X4_WORLDSPACE, 12 * sizeof(float))
DECLARE_FIELD_SIZE(FIELD_INTERVAL, 2 * sizeof(float))        
DECLARE_FIELD_SIZE(FIELD_MODELINDEX, sizeof(int))
DECLARE_FIELD_SIZE(FIELD_MATERIALINDEX, sizeof(int))


#define ARRAYSIZE2D(p)		(sizeof(p)/sizeof(p[0][0]))
#define SIZE_OF_ARRAY(p)	_ARRAYSIZE(p)

#define _FIELD(name,fieldtype,count,flags,mapname,tolerance)		{ fieldtype, #name, { offsetof(classNameTypedef, name), 0 }, count, flags, mapname, NULL, NULL, NULL, sizeof( ((classNameTypedef *)0)->name ), NULL, 0, tolerance }
#define DEFINE_FIELD_NULL	{ FIELD_VOID,0, {0,0},0,0,0,0,0,0}
#define DEFINE_FIELD(name,fieldtype)			_FIELD(name, fieldtype, 1,  FTYPEDESC_SAVE, NULL, 0 )
#define DEFINE_KEYFIELD(name,fieldtype, mapname) _FIELD(name, fieldtype, 1,  FTYPEDESC_KEY | FTYPEDESC_SAVE, mapname, 0 )
#define DEFINE_KEYFIELD_NOT_SAVED(name,fieldtype, mapname)_FIELD(name, fieldtype, 1,  FTYPEDESC_KEY, mapname, 0 )
#define DEFINE_AUTO_ARRAY(name,fieldtype)		_FIELD(name, fieldtype, SIZE_OF_ARRAY(((classNameTypedef *)0)->name), FTYPEDESC_SAVE, NULL, 0 )
#define DEFINE_AUTO_ARRAY_KEYFIELD(name,fieldtype,mapname)		_FIELD(name, fieldtype, SIZE_OF_ARRAY(((classNameTypedef *)0)->name), FTYPEDESC_SAVE, mapname, 0 )
#define DEFINE_ARRAY(name,fieldtype, count)		_FIELD(name, fieldtype, count, FTYPEDESC_SAVE, NULL, 0 )
#define DEFINE_ENTITY_FIELD(name,fieldtype)			_FIELD(edict_t, name, fieldtype, 1,  FTYPEDESC_KEY | FTYPEDESC_SAVE, #name, 0 )
#define DEFINE_ENTITY_GLOBAL_FIELD(name,fieldtype)	_FIELD(edict_t, name, fieldtype, 1,  FTYPEDESC_KEY | FTYPEDESC_SAVE | FTYPEDESC_GLOBAL, #name, 0 )
#define DEFINE_GLOBAL_FIELD(name,fieldtype)	_FIELD(name, fieldtype, 1,  FTYPEDESC_GLOBAL | FTYPEDESC_SAVE, NULL, 0 )
#define DEFINE_GLOBAL_KEYFIELD(name,fieldtype, mapname)	_FIELD(name, fieldtype, 1,  FTYPEDESC_GLOBAL | FTYPEDESC_KEY | FTYPEDESC_SAVE, mapname, 0 )
#define DEFINE_CUSTOM_FIELD(name,datafuncs)	{ FIELD_CUSTOM, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE, NULL, datafuncs, NULL }
#define DEFINE_CUSTOM_KEYFIELD(name,datafuncs,mapname)	{ FIELD_CUSTOM, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE | FTYPEDESC_KEY, mapname, datafuncs, NULL }
#define DEFINE_AUTO_ARRAY2D(name,fieldtype)		_FIELD(name, fieldtype, ARRAYSIZE2D(((classNameTypedef *)0)->name), FTYPEDESC_SAVE, NULL, 0 )
#define DEFINE_BITFIELD(name,fieldtype,bitcount)	DEFINE_ARRAY(name,fieldtype,((bitcount+FIELD_BITS(fieldtype)-1)&~(FIELD_BITS(fieldtype)-1)) / FIELD_BITS(fieldtype) )
#define DEFINE_INDEX(name,fieldtype)			_FIELD(name, fieldtype, 1,  FTYPEDESC_INDEX, NULL, 0 )

#define DEFINE_EMBEDDED( name )						\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE, NULL, NULL, NULL, &(((classNameTypedef *)0)->name.m_DataMap), sizeof( ((classNameTypedef *)0)->name ), NULL, 0, 0.0f }

#define DEFINE_EMBEDDED_OVERRIDE( name, overridetype )	\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE, NULL, NULL, NULL, &((overridetype *)0)->m_DataMap, sizeof( ((classNameTypedef *)0)->name ), NULL, 0, 0.0f }

#define DEFINE_EMBEDDEDBYREF( name )					\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE | FTYPEDESC_PTR, NULL, NULL, NULL, &(((classNameTypedef *)0)->name->m_DataMap), sizeof( *(((classNameTypedef *)0)->name) ), NULL, 0, 0.0f }

#define DEFINE_EMBEDDED_ARRAY( name, count )			\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, count, FTYPEDESC_SAVE, NULL, NULL, NULL, &(((classNameTypedef *)0)->name->m_DataMap), sizeof( ((classNameTypedef *)0)->name[0] ), NULL, 0, 0.0f  }

#define DEFINE_EMBEDDED_AUTO_ARRAY( name )			\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, SIZE_OF_ARRAY( ((classNameTypedef *)0)->name ), FTYPEDESC_SAVE, NULL, NULL, NULL, &(((classNameTypedef *)0)->name->m_DataMap), sizeof( ((classNameTypedef *)0)->name[0] ), NULL, 0, 0.0f  }

#ifndef NO_ENTITY_PREDICTION

#define DEFINE_PRED_TYPEDESCRIPTION( name, fieldtype )						\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE, NULL, NULL, NULL, &fieldtype::m_PredMap }

#define DEFINE_PRED_TYPEDESCRIPTION_PTR( name, fieldtype )						\
	{ FIELD_EMBEDDED, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_SAVE | FTYPEDESC_PTR, NULL, NULL, NULL, &fieldtype::m_PredMap }

#else

#define DEFINE_PRED_TYPEDESCRIPTION( name, fieldtype )		DEFINE_FIELD_NULL
#define DEFINE_PRED_TYPEDESCRIPTION_PTR( name, fieldtype )	DEFINE_FIELD_NULL

#endif

#define DEFINE_PRED_FIELD(name,fieldtype, flags)			_FIELD(name, fieldtype, 1,  flags, NULL, 0.0f )
#define DEFINE_PRED_ARRAY(name,fieldtype, count,flags)		_FIELD(name, fieldtype, count, flags, NULL, 0.0f )
#define DEFINE_FIELD_NAME(localname,netname,fieldtype)		_FIELD(localname, fieldtype, 1,  0, #netname, 0.0f )
#define DEFINE_PRED_FIELD_TOL(name,fieldtype, flags,tolerance)			_FIELD(name, fieldtype, 1,  flags, NULL, tolerance )
#define DEFINE_PRED_ARRAY_TOL(name,fieldtype, count,flags,tolerance)		_FIELD(name, fieldtype, count, flags, NULL, tolerance)
#define DEFINE_FIELD_NAME_TOL(localname,netname,fieldtolerance)		_FIELD(localname, fieldtype, 1,  0, #netname, tolerance )

#define DEFINE_INPUT( name, fieldtype, inputname ) { fieldtype, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_INPUT | FTYPEDESC_SAVE | FTYPEDESC_KEY,	inputname, NULL, NULL, NULL, sizeof( ((classNameTypedef *)0)->name ) }
#define DEFINE_INPUTFUNC( fieldtype, inputname, inputfunc ) { fieldtype, #inputfunc, { NULL, NULL }, 1, FTYPEDESC_INPUT, inputname, NULL, static_cast <inputfunc_t> (&classNameTypedef::inputfunc) }

class ISaveRestoreOps;
extern ISaveRestoreOps* eventFuncs;
#define DEFINE_OUTPUT( name, outputname )	{ FIELD_CUSTOM, #name, { offsetof(classNameTypedef, name), 0 }, 1, FTYPEDESC_OUTPUT | FTYPEDESC_SAVE | FTYPEDESC_KEY, outputname, eventFuncs }

#define DEFINE_FUNCTION_RAW( function, func_type )			{ FIELD_VOID, nameHolder.GenerateName(#function), { NULL, NULL }, 1, FTYPEDESC_FUNCTIONTABLE, NULL, NULL, (inputfunc_t)((func_type)(&classNameTypedef::function)) }
#define DEFINE_FUNCTION( function )			DEFINE_FUNCTION_RAW( function, inputfunc_t )


#define FTYPEDESC_GLOBAL			0x0001		        
#define FTYPEDESC_SAVE				0x0002		      
#define FTYPEDESC_KEY				0x0004		              
#define FTYPEDESC_INPUT				0x0008		                
#define FTYPEDESC_OUTPUT			0x0010		           
#define FTYPEDESC_FUNCTIONTABLE		0x0020		          
#define FTYPEDESC_PTR				0x0040		         
#define FTYPEDESC_OVERRIDE			0x0080		                  

#define FTYPEDESC_INSENDTABLE		0x0100		        
#define FTYPEDESC_PRIVATE			0x0200		                    
#define FTYPEDESC_NOERRORCHECK		0x0400		                

#define FTYPEDESC_MODELINDEX		0x0800		          

#define FTYPEDESC_INDEX				0x1000		            

#define FTYPEDESC_VIEW_OTHER_PLAYER		0x2000		             
#define FTYPEDESC_VIEW_OWN_TEAM			0x4000		                
#define FTYPEDESC_VIEW_NEVER			0x8000		           

#define TD_MSECTOLERANCE		0.001f		                 

struct typedescription_t;


class ISaveRestoreOps;

typedef void (CBaseEntity::* inputfunc_t)(inputdata_t& data);

struct datamap_t;
struct typedescription_t;

enum
{
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,

	TD_OFFSET_COUNT,
};

struct typedescription_t
{
	fieldtype_t			fieldType;
	const char* fieldName;
	int					fieldOffset[TD_OFFSET_COUNT];        
	unsigned short		fieldSize;
	short				flags;
	const char* externalName;
	ISaveRestoreOps* pSaveRestoreOps;
	inputfunc_t			inputFunc;
	datamap_t* td;

	int					fieldSizeInBytes;

	struct typedescription_t* override_field;

	int					override_count;

	float				fieldTolerance;
};


struct datamap_t
{
	typedescription_t* dataDesc;
	int					dataNumFields;
	char const* dataClassName;
	datamap_t* baseMap;

	bool				chains_validated;
	bool				packed_offsets_computed;
	int					packed_size;

#if defined( _DEBUG )
	bool				bValidityChecked;
#endif  
};


#define DECLARE_SIMPLE_DATADESC() \
	static datamap_t m_DataMap; \
	static datamap_t *GetBaseMap(); \
	template <typename T> friend void DataMapAccess(T *, datamap_t **p); \
	template <typename T> friend datamap_t *DataMapInit(T *);

#define	DECLARE_DATADESC() \
	DECLARE_SIMPLE_DATADESC() \
	virtual datamap_t *GetDataDescMap( void );

#define BEGIN_DATADESC( className ) \
	datamap_t className::m_DataMap = { 0, 0, #className, NULL }; \
	datamap_t *className::GetDataDescMap( void ) { return &m_DataMap; } \
	datamap_t *className::GetBaseMap() { datamap_t *pResult; DataMapAccess((BaseClass *)NULL, &pResult); return pResult; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_DATADESC_NO_BASE( className ) \
	datamap_t className::m_DataMap = { 0, 0, #className, NULL }; \
	datamap_t *className::GetDataDescMap( void ) { return &m_DataMap; } \
	datamap_t *className::GetBaseMap() { return NULL; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_SIMPLE_DATADESC( className ) \
	datamap_t className::m_DataMap = { 0, 0, #className, NULL }; \
	datamap_t *className::GetBaseMap() { return NULL; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_SIMPLE_DATADESC_( className, BaseClass ) \
	datamap_t className::m_DataMap = { 0, 0, #className, NULL }; \
	datamap_t *className::GetBaseMap() { datamap_t *pResult; DataMapAccess((BaseClass *)NULL, &pResult); return pResult; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_DATADESC_GUTS( className ) \
	template <typename T> datamap_t *DataMapInit(T *); \
	template <> datamap_t *DataMapInit<className>( className * ); \
	namespace className##_DataDescInit \
	{ \
		datamap_t *g_DataMapHolder = DataMapInit( (className *)NULL );             \
	} \
	\
	template <> datamap_t *DataMapInit<className>( className * ) \
	{ \
		typedef className classNameTypedef; \
		static CDatadescGeneratedNameHolder nameHolder(#className); \
		className::m_DataMap.baseMap = className::GetBaseMap(); \
		static typedescription_t dataDesc[] = \
		{ \
		{ FIELD_VOID,0, {0,0},0,0,0,0,0,0},        

#define END_DATADESC() \
		}; \
		\
		if ( sizeof( dataDesc ) > sizeof( dataDesc[0] ) ) \
		{ \
			classNameTypedef::m_DataMap.dataNumFields = SIZE_OF_ARRAY( dataDesc ) - 1; \
			classNameTypedef::m_DataMap.dataDesc 	  = &dataDesc[1]; \
		} \
		else \
		{ \
			classNameTypedef::m_DataMap.dataNumFields = 1; \
			classNameTypedef::m_DataMap.dataDesc 	  = dataDesc; \
		} \
		return &classNameTypedef::m_DataMap; \
	}

#define IMPLEMENT_NULL_SIMPLE_DATADESC( derivedClass ) \
	BEGIN_SIMPLE_DATADESC( derivedClass ) \
	END_DATADESC()

#define IMPLEMENT_NULL_SIMPLE_DATADESC_( derivedClass, baseClass ) \
	BEGIN_SIMPLE_DATADESC_( derivedClass, baseClass ) \
	END_DATADESC()

#define IMPLEMENT_NULL_DATADESC( derivedClass ) \
	BEGIN_DATADESC( derivedClass ) \
	END_DATADESC()

#define BEGIN_BITFIELD( name ) \
	union \
	{ \
		char name; \
		struct \
		{

#define END_BITFIELD() \
		}; \
	};

#define DECLARE_BYTESWAP_DATADESC() DECLARE_SIMPLE_DATADESC()
#define BEGIN_BYTESWAP_DATADESC(name) BEGIN_SIMPLE_DATADESC(name) 
#define BEGIN_BYTESWAP_DATADESC_(name,base) BEGIN_SIMPLE_DATADESC_(name,base) 
#define END_BYTESWAP_DATADESC() END_DATADESC()

template <typename T>
inline void DataMapAccess(T* ignored, datamap_t** p)
{
	*p = &T::m_DataMap;
}

class CDatadescGeneratedNameHolder
{
public:
	CDatadescGeneratedNameHolder(const char* pszBase)
		: m_pszBase(pszBase)
	{
		m_nLenBase = strlen(m_pszBase);
	}

	~CDatadescGeneratedNameHolder()
	{
		for (int i = 0; i < m_Names.Count(); i++)
		{
			delete m_Names[i];
		}
	}

	const char* GenerateName(const char* pszIdentifier)
	{
		char* pBuf = new char[m_nLenBase + strlen(pszIdentifier) + 1];
		strcpy(pBuf, m_pszBase);
		strcat(pBuf, pszIdentifier);
		m_Names.AddToTail(pBuf);
		return pBuf;
	}

private:
	const char* m_pszBase;
	size_t m_nLenBase;
	CUtlVector<char*> m_Names;
};

#include "memdbgoff.h"

#endif  