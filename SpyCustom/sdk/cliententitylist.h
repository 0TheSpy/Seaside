#if !defined( CLIENTENTITYLIST_H )
#define CLIENTENTITYLIST_H
#ifdef _WIN32
#pragma once
#endif

#include "dbg.h"
#include "icliententitylist.h"
#include "iclientunknown.h"
#include "utllinkedlist.h"
#include "utlvector.h"
#include "icliententityinternal.h"
#include "ispatialpartition.h"
#include "cdll_util.h"
#include "entitylist_base.h"
#include "utlmap.h"

class C_Beam;
class C_BaseViewModel;
class C_BaseEntity;


#define INPVS_YES			0x0001		       
#define INPVS_THISFRAME		0x0002		                
#define INPVS_NEEDSNOTIFY	0x0004		       

class IClientEntityListener;

abstract_class C_BaseEntityClassList
{
public:
	C_BaseEntityClassList();
	~C_BaseEntityClassList();
	virtual void LevelShutdown() = 0;

	C_BaseEntityClassList* m_pNextClassList;
};

template< class T >
class C_EntityClassList : public C_BaseEntityClassList
{
public:
	virtual void LevelShutdown() { m_pClassList = NULL; }

	void Insert(T* pEntity)
	{
		pEntity->m_pNext = m_pClassList;
		m_pClassList = pEntity;
	}

	void Remove(T* pEntity)
	{
		T** pPrev = &m_pClassList;
		T* pCur = *pPrev;
		while (pCur)
		{
			if (pCur == pEntity)
			{
				*pPrev = pCur->m_pNext;
				return;
			}
			pPrev = &pCur->m_pNext;
			pCur = *pPrev;
		}
	}

	static T* m_pClassList;
};


#define INVALID_CLIENTENTITY_HANDLE CBaseHandle( INVALID_EHANDLE_INDEX )


class CClientEntityList : public CBaseEntityList, public IClientEntityList
{
	friend class C_BaseEntityIterator;
	friend class C_AllBaseEntityIterator;

public:
	CClientEntityList(void);
	virtual 					~CClientEntityList(void);

	void						Release();		     


public:

	virtual IClientNetworkable* GetClientNetworkable(int entnum);
	virtual IClientEntity* GetClientEntity(int entnum);

	virtual int					NumberOfEntities(bool bIncludeNonNetworkable = false);

	virtual IClientUnknown* GetClientUnknownFromHandle(ClientEntityHandle_t hEnt);
	virtual IClientNetworkable* GetClientNetworkableFromHandle(ClientEntityHandle_t hEnt);
	virtual IClientEntity* GetClientEntityFromHandle(ClientEntityHandle_t hEnt);

	virtual int					GetHighestEntityIndex(void);

	virtual void				SetMaxEntities(int maxents);
	virtual int					GetMaxEntities();


protected:

	virtual void OnAddEntity(IHandleEntity* pEnt, CBaseHandle handle);
	virtual void OnRemoveEntity(IHandleEntity* pEnt, CBaseHandle handle);


public:

	IClientUnknown* GetListedEntity(int entnum);

	C_BaseEntity* GetBaseEntity(int entnum);
	ICollideable* GetCollideable(int entnum);

	IClientRenderable* GetClientRenderableFromHandle(ClientEntityHandle_t hEnt);
	C_BaseEntity* GetBaseEntityFromHandle(ClientEntityHandle_t hEnt);
	ICollideable* GetCollideableFromHandle(ClientEntityHandle_t hEnt);
	IClientThinkable* GetClientThinkableFromHandle(ClientEntityHandle_t hEnt);

	ClientEntityHandle_t	EntIndexToHandle(int entnum);
	int						HandleToEntIndex(ClientEntityHandle_t handle);

	bool					IsHandleValid(ClientEntityHandle_t handle) const;

	C_BaseEntity* GetEnt(int entnum) { return GetBaseEntity(entnum); }

	void					RecomputeHighestEntityUsed(void);


	C_BaseEntity* FirstBaseEntity() const;
	C_BaseEntity* NextBaseEntity(C_BaseEntity* pEnt) const;

	class CPVSNotifyInfo
	{
	public:
		IPVSNotify* m_pNotify;
		IClientRenderable* m_pRenderable;
		unsigned char m_InPVSStatus;				     
		unsigned short m_PVSNotifiersLink;			  
	};

	CUtlLinkedList<CPVSNotifyInfo, unsigned short>& GetPVSNotifiers();

	CUtlVector<IClientEntityListener*>	m_entityListeners;

	void AddListenerEntity(IClientEntityListener* pListener);
	void RemoveListenerEntity(IClientEntityListener* pListener);

	void NotifyCreateEntity(C_BaseEntity* pEnt);
	void NotifyRemoveEntity(C_BaseEntity* pEnt);

private:

	struct EntityCacheInfo_t
	{
		IClientNetworkable* m_pNetworkable;
		unsigned short m_BaseEntitiesIndex;	       
	};

	int					m_iNumServerEnts;
	int					m_iMaxServerEnts;

	int					m_iNumClientNonNetworkable;

	int					m_iMaxUsedServerIndex;

	EntityCacheInfo_t	m_EntityCacheInfo[NUM_ENT_ENTRIES];

	CUtlLinkedList<C_BaseEntity*, unsigned short> m_BaseEntities;


private:

	void AddPVSNotifier(IClientUnknown* pUnknown);
	void RemovePVSNotifier(IClientUnknown* pUnknown);

	CUtlLinkedList<CPVSNotifyInfo, unsigned short> m_PVSNotifyInfos;
	CUtlMap<IClientUnknown*, unsigned short, unsigned short> m_PVSNotifierMap;	      
};


class C_AllBaseEntityIterator
{
public:
	C_AllBaseEntityIterator();

	void Restart();
	C_BaseEntity* Next();	       

private:
	unsigned short m_CurBaseEntity;
};

class C_BaseEntityIterator
{
public:
	C_BaseEntityIterator();

	void Restart();
	C_BaseEntity* Next();	       

private:
	unsigned short m_CurBaseEntity;
};

inline bool	CClientEntityList::IsHandleValid(ClientEntityHandle_t handle) const
{
	return handle.Get() != 0;
}

inline IClientUnknown* CClientEntityList::GetListedEntity(int entnum)
{
	return (IClientUnknown*)LookupEntityByNetworkIndex(entnum);
}

inline IClientUnknown* CClientEntityList::GetClientUnknownFromHandle(ClientEntityHandle_t hEnt)
{
	return (IClientUnknown*)LookupEntity(hEnt);
}

inline CUtlLinkedList<CClientEntityList::CPVSNotifyInfo, unsigned short>& CClientEntityList::GetPVSNotifiers()
{
	return m_PVSNotifyInfos;
}


inline ClientEntityHandle_t CClientEntityList::EntIndexToHandle(int entnum)
{
	if (entnum < -1)
		return INVALID_EHANDLE_INDEX;
	IClientUnknown* pUnk = GetListedEntity(entnum);
	return pUnk ? pUnk->GetRefEHandle() : INVALID_EHANDLE_INDEX;
}


extern CClientEntityList* cl_entitylist;

inline CClientEntityList& ClientEntityList()
{
	return *cl_entitylist;
}

class IClientEntityListener
{
public:
	virtual void OnEntityCreated(C_BaseEntity* pEntity) {};
	virtual void OnEntityDeleted(C_BaseEntity* pEntity) {};
};


#endif  