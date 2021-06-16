#ifndef MESSAGEMAP_H
#define MESSAGEMAP_H

#ifdef _WIN32
#pragma once
#endif

#include "dmxelement.h"

#pragma pointers_to_members( full_generality, virtual_inheritance )

namespace vgui
{


#ifndef ARRAYSIZE
#define ARRAYSIZE(p)	(sizeof(p)/sizeof(p[0]))
#endif


	enum DataType_t
	{
		DATATYPE_VOID,
		DATATYPE_CONSTCHARPTR,
		DATATYPE_INT,
		DATATYPE_FLOAT,
		DATATYPE_PTR,
		DATATYPE_BOOL,
		DATATYPE_KEYVALUES,
		DATATYPE_CONSTWCHARPTR,
		DATATYPE_UINT64,
		DATATYPE_HANDLE,      
	};

	class Panel;
	typedef uintp VPANEL;

	typedef void (Panel::* MessageFunc_t)(void);

#pragma warning(disable:4121)
	struct MessageMapItem_t
	{
		const char* name;
		ALIGN16 MessageFunc_t func;

		int numParams;

		DataType_t firstParamType;
		const char* firstParamName;

		DataType_t secondParamType;
		const char* secondParamName;

		int nameSymbol;
		int firstParamSymbol;
		int secondParamSymbol;
	};

#define DECLARE_PANELMESSAGEMAP( className )												\
	static void AddToMap( char const *scriptname, vgui::MessageFunc_t function, int paramCount, int p1type, const char *p1name, int p2type, const char *p2name ) 	\
	{																					\
		vgui::PanelMessageMap *map = vgui::FindOrAddPanelMessageMap( GetPanelClassName() );			\
																						\
		vgui::MessageMapItem_t entry;															\
		entry.name = scriptname;														\
		entry.func = function;															\
		entry.numParams = paramCount;													\
		entry.firstParamType = (vgui::DataType_t)p1type;								\
		entry.firstParamName = p1name;													\
		entry.secondParamType = (vgui::DataType_t)p2type;								\
		entry.secondParamName = p2name;													\
		entry.nameSymbol = 0;															\
		entry.firstParamSymbol = 0;														\
		entry.secondParamSymbol = 0;													\
																						\
		map->entries.AddToTail( entry );												\
	}																					\
																						\
	static void ChainToMap( void )														\
	{																					\
		static bool chained = false;													\
		if ( chained )																	\
			return;																		\
		chained = true;																	\
		vgui::PanelMessageMap *map = vgui::FindOrAddPanelMessageMap( GetPanelClassName() );			\
		map->pfnClassName = &GetPanelClassName;											\
		if ( map && GetPanelBaseClassName() && GetPanelBaseClassName()[0] )				\
		{																				\
			map->baseMap = vgui::FindOrAddPanelMessageMap( GetPanelBaseClassName() );			\
		}																				\
	}																					\
																						\
	class className##_RegisterMap;															\
	friend class className##_RegisterMap;													\
	class className##_RegisterMap															\
	{																					\
	public:																				\
		className##_RegisterMap()															\
		{																				\
			className::ChainToMap();													\
		}																				\
	};																					\
	className##_RegisterMap m_RegisterClass;												\
																						\
	virtual vgui::PanelMessageMap *GetMessageMap()											\
	{																					\
		static vgui::PanelMessageMap *s_pMap = vgui::FindOrAddPanelMessageMap( GetPanelClassName() );	\
		return s_pMap;																	\
	}

#define VGUI_USEKEYBINDINGMAPS	1

#if defined( VGUI_USEKEYBINDINGMAPS )

#define DECLARE_CLASS_SIMPLE( className, baseClassName ) \
	typedef baseClassName BaseClass; \
	typedef className ThisClass;	\
public:								\
	DECLARE_PANELMESSAGEMAP( className ); \
	DECLARE_PANELANIMATION( className ); \
	DECLARE_KEYBINDINGMAP( className ); \
	static char const *GetPanelClassName() { return #className; } \
	static char const *GetPanelBaseClassName() { return #baseClassName; }

#define DECLARE_CLASS_SIMPLE_NOBASE( className ) \
	typedef className ThisClass;	\
public:							\
	DECLARE_PANELMESSAGEMAP( className ); \
	DECLARE_PANELANIMATION( className ); \
	DECLARE_KEYBINDINGMAP( className ); \
	static char const *GetPanelClassName() { return #className; } \
	static char const *GetPanelBaseClassName() { return NULL; }

#else    

#define DECLARE_CLASS_SIMPLE( className, baseClassName ) \
	typedef baseClassName BaseClass; \
	typedef className ThisClass;	\
public:								\
	DECLARE_PANELMESSAGEMAP( className ); \
	DECLARE_PANELANIMATION( className ); \
	static char const *GetPanelClassName() { return #className; } \
	static char const *GetPanelBaseClassName() { return #baseClassName; }

#define DECLARE_CLASS_SIMPLE_NOBASE( className ) \
	typedef className ThisClass;	\
public:							\
	DECLARE_PANELMESSAGEMAP( className ); \
	DECLARE_PANELANIMATION( className ); \
	static char const *GetPanelClassName() { return #className; } \
	static char const *GetPanelBaseClassName() { return NULL; }

#endif  

#define _MessageFuncCommon( name, scriptname, paramCount, p1type, p1name, p2type, p2name )	\
	class PanelMessageFunc_##name; \
	friend class PanelMessageFunc_##name; \
	class PanelMessageFunc_##name \
	{ \
	public: \
		static void InitVar() \
		{ \
			static bool bAdded = false; \
			if ( !bAdded ) \
			{ \
				bAdded = true; \
				AddToMap( scriptname, (vgui::MessageFunc_t)&ThisClass::name, paramCount, p1type, p1name, p2type, p2name ); \
			} \
		}												\
		PanelMessageFunc_##name()						\
		{												\
			PanelMessageFunc_##name::InitVar();			\
		}												\
	};													\
	PanelMessageFunc_##name m_##name##_register;		\

#define MESSAGE_FUNC( name, scriptname )			_MessageFuncCommon( name, scriptname, 0, 0, 0, 0, 0 );	virtual void name( void )

#define MESSAGE_FUNC_INT( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_INT, #p1, 0, 0 );	virtual void name( int p1 )
#define MESSAGE_FUNC_UINT64( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_UINT64, #p1, 0, 0 );	virtual void name( uint64 p1 )
#define MESSAGE_FUNC_PTR( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_PTR, #p1, 0, 0 );	virtual void name( vgui::Panel *p1 )
#define MESSAGE_FUNC_HANDLE( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_HANDLE, #p1, 0, 0 );	virtual void name( vgui::VPANEL p1 )
#define MESSAGE_FUNC_FLOAT( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_FLOAT, #p1, 0, 0 );	virtual void name( float p1 )
#define MESSAGE_FUNC_CHARPTR( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_CONSTCHARPTR, #p1, 0, 0 );	virtual void name( const char *p1 )
#define MESSAGE_FUNC_WCHARPTR( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_CONSTWCHARPTR, #p1, 0, 0 ); virtual void name( const wchar_t *p1 )

#define MESSAGE_FUNC_INT_INT( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_INT, #p1, vgui::DATATYPE_INT, #p2 );	virtual void name( int p1, int p2 )
#define MESSAGE_FUNC_PTR_INT( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_PTR, #p1, vgui::DATATYPE_INT, #p2 );	virtual void name( vgui::Panel *p1, int p2 )
#define MESSAGE_FUNC_HANDLE_INT( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_HANDLE, #p1, vgui::DATATYPE_INT, #p2 );	virtual void name( vgui::VPANEL p1, int p2 )
#define MESSAGE_FUNC_ENUM_ENUM( name, scriptname, t1, p1, t2, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_PTR, #p1, vgui::DATATYPE_PTR, #p2 );	virtual void name( t1 p1, t2 p2 )
#define MESSAGE_FUNC_INT_CHARPTR( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_INT, #p1, vgui::DATATYPE_CONSTCHARPTR, #p2 );	virtual void name( int p1, const char *p2 )
#define MESSAGE_FUNC_PTR_CHARPTR( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_PTR, #p1, vgui::DATATYPE_CONSTCHARPTR, #p2 );	virtual void name( vgui::Panel *p1, const char *p2 )
#define MESSAGE_FUNC_HANDLE_CHARPTR( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_HANDLE, #p1, vgui::DATATYPE_CONSTCHARPTR, #p2 );	virtual void name( vgui::VPANEL p1, const char *p2 )
#define MESSAGE_FUNC_PTR_WCHARPTR( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_PTR, #p1, vgui::DATATYPE_CONSTWCHARPTR, #p2 );	virtual void name( vgui::Panel *p1, const wchar_t *p2 )
#define MESSAGE_FUNC_HANDLE_WCHARPTR( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_HANDLE, #p1, vgui::DATATYPE_CONSTWCHARPTR, #p2 );	virtual void name( vgui::VPANEL p1, const wchar_t *p2 )
#define MESSAGE_FUNC_CHARPTR_CHARPTR( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_CONSTCHARPTR, #p1, vgui::DATATYPE_CONSTCHARPTR, #p2 );	virtual void name( const char *p1, const char *p2 )

#define MESSAGE_FUNC_PARAMS( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_KEYVALUES, NULL, 0, 0 );	virtual void name( KeyValues *p1 )

#define MESSAGE_FUNC_NV( name, scriptname )			_MessageFuncCommon( name, scriptname, 0, 0, 0, 0, 0 );	void name( void )
#define MESSAGE_FUNC_NV_INT( name, scriptname, p1 )	_MessageFuncCommon( name, scriptname, 1, vgui::DATATYPE_INT, #p1, 0, 0 );	void name( int p1 )
#define MESSAGE_FUNC_NV_INT_INT( name, scriptname, p1, p2 )	_MessageFuncCommon( name, scriptname, 2, vgui::DATATYPE_INT, #p1, vgui::DATATYPE_INT, #p2 );	void name( int p1, int p2 )


	struct PanelMessageMap
	{
		PanelMessageMap()
		{
			baseMap = NULL;
			pfnClassName = NULL;
			processed = false;
		}

		CUtlVector< MessageMapItem_t > entries;
		bool processed;
		PanelMessageMap* baseMap;
		char const* (*pfnClassName)(void);
	};

	PanelMessageMap* FindPanelMessageMap(char const* className);
	PanelMessageMap* FindOrAddPanelMessageMap(char const* className);



#define MAP_MESSAGE( type, name, func )						{ name, (vgui::MessageFunc_t)(&type::func), 0 }

#define MAP_MESSAGE_PARAMS( type, name, func )				{ name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_KEYVALUES, NULL }

#define MAP_MESSAGE_PTR( type, name, func, param1 )			{ name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_PTR, param1 }
#define MAP_MESSAGE_INT( type, name, func, param1 )			{ name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_INT, param1 }
#define MAP_MESSAGE_BOOL( type, name, func, param1 )		{ name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_BOOL, param1 }
#define MAP_MESSAGE_FLOAT( type, name, func, param1 )		{ name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_FLOAT, param1 }
#define MAP_MESSAGE_PTR( type, name, func, param1 )			{ name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_PTR, param1 }
#define MAP_MESSAGE_CONSTCHARPTR( type, name, func, param1) { name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_CONSTCHARPTR, param1 }
#define MAP_MESSAGE_CONSTWCHARPTR( type, name, func, param1) { name, (vgui::MessageFunc_t)(&type::func), 1, vgui::DATATYPE_CONSTWCHARPTR, param1 }

#define MAP_MESSAGE_INT_INT( type, name, func, param1, param2 ) { name, (vgui::MessageFunc_t)&type::func, 2, vgui::DATATYPE_INT, param1, vgui::DATATYPE_INT, param2 }
#define MAP_MESSAGE_PTR_INT( type, name, func, param1, param2 ) { name, (vgui::MessageFunc_t)&type::func, 2, vgui::DATATYPE_PTR, param1, vgui::DATATYPE_INT, param2 }
#define MAP_MESSAGE_INT_CONSTCHARPTR( type, name, func, param1, param2 ) { name, (vgui::MessageFunc_t)&type::func, 2, vgui::DATATYPE_INT, param1, vgui::DATATYPE_CONSTCHARPTR, param2 }
#define MAP_MESSAGE_PTR_CONSTCHARPTR( type, name, func, param1, param2 ) { name, (vgui::MessageFunc_t)&type::func, 2, vgui::DATATYPE_PTR, param1, vgui::DATATYPE_CONSTCHARPTR, param2 }
#define MAP_MESSAGE_PTR_CONSTWCHARPTR( type, name, func, param1, param2 ) { name, (vgui::MessageFunc_t)&type::func, 2, vgui::DATATYPE_PTR, param1, vgui::DATATYPE_CONSTWCHARPTR, param2 }
#define MAP_MESSAGE_CONSTCHARPTR_CONSTCHARPTR( type, name, func, param1, param2 ) { name, (vgui::MessageFunc_t)&type::func, 2, vgui::DATATYPE_CONSTCHARPTR, param1, vgui::DATATYPE_CONSTCHARPTR, param2 }


	struct PanelMap_t
	{
		MessageMapItem_t* dataDesc;
		int dataNumFields;
		const char* dataClassName;
		PanelMap_t* baseMap;
		int processed;
	};

#define DECLARE_PANELMAP() \
	static vgui::PanelMap_t m_PanelMap; \
	static vgui::MessageMapItem_t m_MessageMap[]; \
	virtual vgui::PanelMap_t *GetPanelMap( void );

#define IMPLEMENT_PANELMAP( derivedClass, baseClass ) \
	vgui::PanelMap_t derivedClass::m_PanelMap = { derivedClass::m_MessageMap, ARRAYSIZE(derivedClass::m_MessageMap), #derivedClass, &baseClass::m_PanelMap }; \
	vgui::PanelMap_t *derivedClass::GetPanelMap( void ) { return &m_PanelMap; }

	typedef vgui::Panel* (*PANELCREATEFUNC)(void);

	class CBuildFactoryHelper
	{
	public:
		static CBuildFactoryHelper* m_sHelpers;

	public:
		CBuildFactoryHelper(char const* className, PANELCREATEFUNC func);

		CBuildFactoryHelper* GetNext(void);

		char const* GetClassName() const;

		vgui::Panel* CreatePanel();

		static vgui::Panel* InstancePanel(char const* className);
		static void GetFactoryNames(CUtlVector< char const* >& list);

		static CDmxElement* CreatePanelDmxElement(vgui::Panel* pPanel);
		static Panel* UnserializeDmxElementPanel(CDmxElement* pElement);

		static bool Serialize(CUtlBuffer& buf, vgui::Panel* pPanel);
		static bool Unserialize(Panel** ppPanel, CUtlBuffer& buf, const char* pFileName = NULL);


	private:

		static bool HasFactory(char const* className);

		CBuildFactoryHelper* m_pNext;

		int					m_Type;
		PANELCREATEFUNC		m_CreateFunc;
		char const* m_pClassName;
	};

#define DECLARE_BUILD_FACTORY( className )										\
	static vgui::Panel *Create_##className( void )							\
		{																		\
			return new className( NULL, NULL );									\
		};																		\
		static vgui::CBuildFactoryHelper g_##className##_Helper( #className, Create_##className );\
	className *g_##className##LinkerHack = NULL;

#define DECLARE_BUILD_FACTORY_DEFAULT_TEXT( className, defaultText )			\
	static vgui::Panel *Create_##className( void )							\
		{																		\
			return new className( NULL, NULL, #defaultText );					\
		};																		\
	static vgui::CBuildFactoryHelper g_##className##_Helper( #className, Create_##className );\
	className *g_##className##LinkerHack = NULL;

#define DECLARE_BUILD_FACTORY_CUSTOM( className, createFunc )					\
	static vgui::CBuildFactoryHelper g_##className##_Helper( #className, createFunc );\
	className *g_##className##LinkerHack = NULL;

#define DECLARE_BUILD_FACTORY_CUSTOM_ALIAS( className, factoryName, createFunc )					\
	static vgui::CBuildFactoryHelper g_##factoryName##_Helper( #factoryName, createFunc );\
	className *g_##factoryName##LinkerHack = NULL;

}   


#endif  