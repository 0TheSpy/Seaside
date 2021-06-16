#if !defined( CLIENT_CLASS_H )
#define CLIENT_CLASS_H
#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "dt_recv.h"

class Vector;
class CMouthInfo;


#include "iclientrenderable.h"
#include "iclientnetworkable.h"


class ClientClass;
extern ClientClass* g_pClientClassHead;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	ClientClass(const char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable* pRecvTable)
	{
		m_pNetworkName = pNetworkName;
		m_pCreateFn = createFn;
		m_pCreateEventFn = createEventFn;
		m_pRecvTable = pRecvTable;

		m_pNext = g_pClientClassHead;
		g_pClientClassHead = this;
	}

	const char* GetName()
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;	     
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int						m_ClassID;	    
};

#define DECLARE_CLIENTCLASS() \
	virtual int YouForgotToImplementOrDeclareClientClass();\
	virtual ClientClass* GetClientClass();\
	static RecvTable *m_pClassRecvTable; \
	DECLARE_CLIENTCLASS_NOBASE()


#define ALLOW_DATATABLES_PRIVATE_ACCESS() \
	template <typename T> friend int ClientClassInit(T *);


#define DECLARE_CLIENTCLASS_NOBASE ALLOW_DATATABLES_PRIVATE_ACCESS

#define IMPLEMENT_CLIENTCLASS(clientClassName, dataTable, serverClassName) \
	INTERNAL_IMPLEMENT_CLIENTCLASS_PROLOGUE(clientClassName, dataTable, serverClassName) \
	static IClientNetworkable* _##clientClassName##_CreateObject( int entnum, int serialNum ) \
	{ \
		clientClassName *pRet = new clientClassName; \
		if ( !pRet ) \
			return 0; \
		pRet->Init( entnum, serialNum ); \
		return pRet; \
	} \
	ClientClass __g_##clientClassName##ClientClass(#serverClassName, \
													_##clientClassName##_CreateObject, \
													NULL,\
													&dataTable::g_RecvTable);

#define IMPLEMENT_CLIENTCLASS_FACTORY(clientClassName, dataTable, serverClassName, factory) \
	INTERNAL_IMPLEMENT_CLIENTCLASS_PROLOGUE(clientClassName, dataTable, serverClassName) \
	ClientClass __g_##clientClassName##ClientClass(#serverClassName, \
													factory, \
													NULL,\
													&dataTable::g_RecvTable);

#define IMPLEMENT_CLIENTCLASS_DT(clientClassName, dataTable, serverClassName)\
	IMPLEMENT_CLIENTCLASS(clientClassName, dataTable, serverClassName)\
	BEGIN_RECV_TABLE(clientClassName, dataTable)

#define IMPLEMENT_CLIENTCLASS_DT_NOBASE(clientClassName, dataTable, serverClassName)\
	IMPLEMENT_CLIENTCLASS(clientClassName, dataTable, serverClassName)\
	BEGIN_RECV_TABLE_NOBASE(clientClassName, dataTable)


#define IMPLEMENT_CLIENTCLASS_EVENT(clientClassName, dataTable, serverClassName)\
	INTERNAL_IMPLEMENT_CLIENTCLASS_PROLOGUE(clientClassName, dataTable, serverClassName)\
	static clientClassName __g_##clientClassName; \
	static IClientNetworkable* _##clientClassName##_CreateObject() {return &__g_##clientClassName;}\
	ClientClass __g_##clientClassName##ClientClass(#serverClassName, \
													NULL,\
													_##clientClassName##_CreateObject, \
													&dataTable::g_RecvTable);

#define IMPLEMENT_CLIENTCLASS_EVENT_DT(clientClassName, dataTable, serverClassName)\
	namespace dataTable {extern RecvTable g_RecvTable;}\
	IMPLEMENT_CLIENTCLASS_EVENT(clientClassName, dataTable, serverClassName)\
	BEGIN_RECV_TABLE(clientClassName, dataTable)


#define IMPLEMENT_CLIENTCLASS_EVENT_POINTER(clientClassName, dataTable, serverClassName, ptr)\
	INTERNAL_IMPLEMENT_CLIENTCLASS_PROLOGUE(clientClassName, dataTable, serverClassName)\
	static IClientNetworkable* _##clientClassName##_CreateObject() {return ptr;}\
	ClientClass __g_##clientClassName##ClientClass(#serverClassName, \
													NULL,\
													_##clientClassName##_CreateObject, \
													&dataTable::g_RecvTable);

#define IMPLEMENT_CLIENTCLASS_EVENT_NONSINGLETON(clientClassName, dataTable, serverClassName)\
	static IClientNetworkable* _##clientClassName##_CreateObject() \
	{ \
		clientClassName *p = new clientClassName; \
		if ( p ) \
			p->Init( -1, 0 ); \
		return p; \
	} \
	ClientClass __g_##clientClassName##ClientClass(#serverClassName, \
													NULL,\
													_##clientClassName##_CreateObject, \
													&dataTable::g_RecvTable);


#define INTERNAL_IMPLEMENT_CLIENTCLASS_PROLOGUE(clientClassName, dataTable, serverClassName) \
	namespace dataTable {extern RecvTable g_RecvTable;}\
	extern ClientClass __g_##clientClassName##ClientClass;\
	RecvTable*		clientClassName::m_pClassRecvTable = &dataTable::g_RecvTable;\
	int				clientClassName::YouForgotToImplementOrDeclareClientClass() {return 0;}\
	ClientClass*	clientClassName::GetClientClass() {return &__g_##clientClassName##ClientClass;}

#endif  