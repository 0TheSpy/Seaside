#ifndef CLIENT_THINKLIST_H
#define CLIENT_THINKLIST_H
#ifdef _WIN32
#pragma once
#endif


#include "igamesystem.h"
#include "utllinkedlist.h"
#include "cliententitylist.h"
#include "iclientthinkable.h"
#include "utlrbtree.h"


#define CLIENT_THINK_ALWAYS	-1293
#define CLIENT_THINK_NEVER	-1


#define INVALID_THINK_HANDLE ClientThinkList()->GetInvalidThinkHandle()


class CClientThinkList : public IGameSystemPerFrame
{
public:

	CClientThinkList();
	virtual					~CClientThinkList();

	virtual char const* Name() { return "CClientThinkList"; }
	virtual bool			IsPerFrame() { return true; }

	void					SetNextClientThink(ClientEntityHandle_t hEnt, float nextTime);

	void					RemoveThinkable(ClientEntityHandle_t hEnt);

	ClientThinkHandle_t		GetInvalidThinkHandle();

	void					PerformThinkFunctions();

	void					AddToDeleteList(ClientEntityHandle_t hEnt);
	void					RemoveFromDeleteList(ClientEntityHandle_t hEnt);

public:

	virtual bool Init();
	virtual void PostInit() {};
	virtual void Shutdown();
	virtual void LevelInitPreEntity();
	virtual void LevelInitPostEntity() {}
	virtual void LevelShutdownPreEntity();
	virtual void LevelShutdownPostEntity();
	virtual void PreRender();
	virtual void PostRender() { }
	virtual void Update(float frametime);
	virtual void OnSave() {}
	virtual void OnRestore() {}
	virtual void SafeRemoveIfDesired() {}

private:
	struct ThinkEntry_t
	{
		ClientEntityHandle_t	m_hEnt;
		float					m_flNextClientThink;
		float					m_flLastClientThink;
		int						m_nIterEnum;
	};

	struct ThinkListChanges_t
	{
		ClientEntityHandle_t	m_hEnt;
		ClientThinkHandle_t		m_hThink;
		float					m_flNextTime;
	};

private:
	void			SetNextClientThink(ClientThinkHandle_t hThink, float nextTime);
	void			RemoveThinkable(ClientThinkHandle_t hThink);
	void			PerformThinkFunction(ThinkEntry_t* pEntry, float curtime);
	ThinkEntry_t* GetThinkEntry(ClientThinkHandle_t hThink);
	void			CleanUpDeleteList();

	void			AddEntityToFrameThinkList(ThinkEntry_t* pEntry, bool bAlwaysChain, int& nCount, ThinkEntry_t** ppFrameThinkList);

private:
	CUtlLinkedList<ThinkEntry_t, unsigned short>	m_ThinkEntries;

	CUtlVector<ClientEntityHandle_t>	m_aDeleteList;
	CUtlVector<ThinkListChanges_t>		m_aChangeList;

	int m_nIterEnum;
	bool m_bInThinkLoop;
};


inline ClientThinkHandle_t CClientThinkList::GetInvalidThinkHandle()
{
	return (ClientThinkHandle_t)(uintp)m_ThinkEntries.InvalidIndex();
}


inline CClientThinkList::ThinkEntry_t* CClientThinkList::GetThinkEntry(ClientThinkHandle_t hThink)
{
	return &m_ThinkEntries[(unsigned long)hThink];
}


inline CClientThinkList* ClientThinkList()
{
	extern CClientThinkList g_ClientThinkList;
	return &g_ClientThinkList;
}


#endif  