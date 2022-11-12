#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#ifdef _WIN32
#pragma once
#endif

class CBaseEntity;
class IEntityListener;

class CBaseEntityClassList
{
public:
	CBaseEntityClassList();
	~CBaseEntityClassList();
	virtual void LevelShutdownPostEntity() = 0;

	CBaseEntityClassList* m_pNextClassList;
};

template< class T >
class CEntityClassList : public CBaseEntityClassList
{
public:
	virtual void LevelShutdownPostEntity() { m_pClassList = NULL; }

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

abstract_class IEntityFindFilter
{
public:
	virtual bool ShouldFindEntity(CBaseEntity * pEntity) = 0;
	virtual CBaseEntity* GetFilterResult(void) = 0;
};


class CGlobalEntityList : public CBaseEntityList
{
public:
private:
	int m_iHighestEnt;      
	int m_iNumEnts;
	int m_iNumEdicts;

	bool m_bClearingEntities;
	CUtlVector<IEntityListener*>	m_entityListeners;

public:
	CBaseHandle AddNetworkableEntity(IHandleEntity* pEnt, int index, int iForcedSerialNum = -1);
	CBaseHandle AddNonNetworkableEntity(IHandleEntity* pEnt);

	void UpdateName(IHandleEntity* pEnt, CBaseHandle hEnt);
	void UpdateName(IHandleEntity* pEnt);

	IServerNetworkable* GetServerNetworkable(CBaseHandle hEnt) const;
	CBaseNetworkable* GetBaseNetworkable(CBaseHandle hEnt) const;
	CBaseEntity* GetBaseEntity(CBaseHandle hEnt) const;
	edict_t* GetEdict(CBaseHandle hEnt) const;

	int NumberOfEntities(void);
	int NumberOfEdicts(void);

	void AddToDeleteList(IServerNetworkable* ent);
	void CleanupDeleteList(void);
	int ResetDeleteList(void);

	void Clear(void);

	bool	IsClearingEntities() { return m_bClearingEntities; }

	void AddListenerEntity(IEntityListener* pListener);
	void RemoveListenerEntity(IEntityListener* pListener);

	void ReportEntityFlagsChanged(CBaseEntity* pEntity, unsigned int flagsOld, unsigned int flagsNow);
	void AddPostClientMessageEntity(CBaseEntity* pEntity);
	void PostClientMessagesSent();

	void NotifyCreateEntity(CBaseEntity* pEnt);
	void NotifySpawn(CBaseEntity* pEnt);
	void NotifyRemoveEntity(CBaseEntity* pEnt);
	CBaseEntity* NextEnt(CBaseEntity* pCurrentEnt);
	CBaseEntity* FirstEnt() { return NextEnt(NULL); }

	template< class T >
	T* NextEntByClass(T* start)
	{
		for (CBaseEntity* x = NextEnt(start); x; x = NextEnt(x))
		{
			start = dynamic_cast<T*>(x);
			if (start)
				return start;
		}
		return NULL;
	}

	bool		 IsEntityPtr(void* pTest);
	CBaseEntity* FindEntityByClassname(CBaseEntity* pStartEntity, const char* szName);
	CBaseEntity* FindEntityByName(CBaseEntity* pStartEntity, const char* szName, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL, IEntityFindFilter* pFilter = NULL);
	CBaseEntity* FindEntityByName(CBaseEntity* pStartEntity, string_t iszName, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL, IEntityFindFilter* pFilter = NULL)
	{
		return FindEntityByName(pStartEntity, STRING(iszName), pSearchingEntity, pActivator, pCaller, pFilter);
	}
	CBaseEntity* FindEntityInSphere(CBaseEntity* pStartEntity, const Vector& vecCenter, float flRadius);
	CBaseEntity* FindEntityByTarget(CBaseEntity* pStartEntity, const char* szName);
	CBaseEntity* FindEntityByModel(CBaseEntity* pStartEntity, const char* szModelName);
	CBaseEntity* FindEntityByOutputTarget(CBaseEntity* pStartEntity, string_t iTarget);

	CBaseEntity* FindEntityByNameNearest(const char* szName, const Vector& vecSrc, float flRadius, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL);
	CBaseEntity* FindEntityByNameWithin(CBaseEntity* pStartEntity, const char* szName, const Vector& vecSrc, float flRadius, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL);
	CBaseEntity* FindEntityByClassnameNearest(const char* szName, const Vector& vecSrc, float flRadius);
	CBaseEntity* FindEntityByClassnameNearest2D(const char* szName, const Vector& vecSrc, float flRadius);
	CBaseEntity* FindEntityByClassnameWithin(CBaseEntity* pStartEntity, const char* szName, const Vector& vecSrc, float flRadius);
	CBaseEntity* FindEntityByClassnameWithin(CBaseEntity* pStartEntity, const char* szName, const Vector& vecMins, const Vector& vecMaxs);

	CBaseEntity* FindEntityGeneric(CBaseEntity* pStartEntity, const char* szName, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL);
	CBaseEntity* FindEntityGenericWithin(CBaseEntity* pStartEntity, const char* szName, const Vector& vecSrc, float flRadius, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL);
	CBaseEntity* FindEntityGenericNearest(const char* szName, const Vector& vecSrc, float flRadius, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL);

	CBaseEntity* FindEntityNearestFacing(const Vector& origin, const Vector& facing, float threshold);
	CBaseEntity* FindEntityClassNearestFacing(const Vector& origin, const Vector& facing, float threshold, char* classname);
	CBaseEntity* FindEntityByNetname(CBaseEntity* pStartEntity, const char* szModelName);

	CBaseEntity* FindEntityProcedural(const char* szName, CBaseEntity* pSearchingEntity = NULL, CBaseEntity* pActivator = NULL, CBaseEntity* pCaller = NULL);

	CBaseEntity* FindEntityByClassnameFast(CBaseEntity* pStartEntity, string_t iszClassname);
	CBaseEntity* FindEntityByClassnameNearestFast(string_t iszClassname, const Vector& vecSrc, float flRadius);
	CBaseEntity* FindEntityByNameFast(CBaseEntity* pStartEntity, string_t iszName);

	CGlobalEntityList();

protected:

	virtual void OnAddEntity(IHandleEntity* pEnt, CBaseHandle handle);
	virtual void OnRemoveEntity(IHandleEntity* pEnt, CBaseHandle handle);

};

extern CGlobalEntityList gEntList;


inline CBaseHandle CGlobalEntityList::AddNetworkableEntity(IHandleEntity* pEnt, int index, int iForcedSerialNum)
{
	CBaseHandle h = CBaseEntityList::AddNetworkableEntity(pEnt, index, iForcedSerialNum);
	UpdateName(pEnt, h);
	return h;
}

inline CBaseHandle CGlobalEntityList::AddNonNetworkableEntity(IHandleEntity* pEnt)
{
	CBaseHandle h = CBaseEntityList::AddNonNetworkableEntity(pEnt);
	UpdateName(pEnt, h);
	return h;
}

inline edict_t* CGlobalEntityList::GetEdict(CBaseHandle hEnt) const
{
	IServerUnknown* pUnk = static_cast<IServerUnknown*>(LookupEntity(hEnt));
	if (pUnk)
		return pUnk->GetNetworkable()->GetEdict();
	else
		return NULL;
}

inline CBaseNetworkable* CGlobalEntityList::GetBaseNetworkable(CBaseHandle hEnt) const
{
	IServerUnknown* pUnk = static_cast<IServerUnknown*>(LookupEntity(hEnt));
	if (pUnk)
		return pUnk->GetNetworkable()->GetBaseNetworkable();
	else
		return NULL;
}

inline IServerNetworkable* CGlobalEntityList::GetServerNetworkable(CBaseHandle hEnt) const
{
	IServerUnknown* pUnk = static_cast<IServerUnknown*>(LookupEntity(hEnt));
	if (pUnk)
		return pUnk->GetNetworkable();
	else
		return NULL;
}

inline CBaseEntity* CGlobalEntityList::GetBaseEntity(CBaseHandle hEnt) const
{
	IServerUnknown* pUnk = static_cast<IServerUnknown*>(LookupEntity(hEnt));
	if (pUnk)
		return pUnk->GetBaseEntity();
	else
		return NULL;
}


#if 0

template <class ENT_TYPE>
inline bool FindEntityByName(const char* pszName, ENT_TYPE** ppResult)
{
	CBaseEntity* pBaseEntity = gEntList.FindEntityByName(NULL, pszName);

	if (pBaseEntity)
		*ppResult = dynamic_cast<ENT_TYPE*>(pBaseEntity);
	else
		*ppResult = NULL;

	return (*ppResult != NULL);
}

template <>
inline bool FindEntityByName<CBaseEntity>(const char* pszName, CBaseEntity** ppResult)
{
	*ppResult = gEntList.FindEntityByName(NULL, pszName);
	return (*ppResult != NULL);
}

template <>
inline bool FindEntityByName<CAI_BaseNPC>(const char* pszName, CAI_BaseNPC** ppResult)
{
	CBaseEntity* pBaseEntity = gEntList.FindEntityByName(NULL, pszName);

	if (pBaseEntity)
		*ppResult = pBaseEntity->MyNPCPointer();
	else
		*ppResult = NULL;

	return (*ppResult != NULL);
}
#endif
struct entitem_t
{
	EHANDLE hEnt;
	struct entitem_t* pNext;

	static void* operator new(size_t stAllocateBlock);
	static void* operator new(size_t stAllocateBlock, int nBlockUse, const char* pFileName, int nLine);
	static void operator delete(void* pMem);
	static void operator delete(void* pMem, int nBlockUse, const char* pFileName, int nLine) { operator delete(pMem); }
};

class CEntityList
{
public:
	CEntityList();
	~CEntityList();

	int m_iNumItems;
	entitem_t* m_pItemList;	    

	void AddEntity(CBaseEntity*);
	void DeleteEntity(CBaseEntity*);
};

enum notify_system_event_t
{
	NOTIFY_EVENT_TELEPORT = 0,
	NOTIFY_EVENT_DESTROY,
};

struct notify_teleport_params_t
{
	Vector prevOrigin;
	QAngle prevAngles;
	bool physicsRotate;
};

struct notify_destroy_params_t
{
};

struct notify_system_event_params_t
{
	union
	{
		const notify_teleport_params_t* pTeleport;
		const notify_destroy_params_t* pDestroy;
	};
	notify_system_event_params_t(const notify_teleport_params_t* pInTeleport) { pTeleport = pInTeleport; }
	notify_system_event_params_t(const notify_destroy_params_t* pInDestroy) { pDestroy = pInDestroy; }
};


abstract_class INotify
{
public:
	virtual void AddEntity(CBaseEntity * pNotify, CBaseEntity * pWatched) = 0;

	virtual void RemoveEntity(CBaseEntity* pNotify, CBaseEntity* pWatched) = 0;

	virtual void ReportNamedEvent(CBaseEntity* pEntity, const char* pEventName) = 0;

	virtual void ReportSystemEvent(CBaseEntity* pEntity, notify_system_event_t eventType, const notify_system_event_params_t& params) = 0;

	inline void ReportDestroyEvent(CBaseEntity* pEntity)
	{
		notify_destroy_params_t destroy;
		ReportSystemEvent(pEntity, NOTIFY_EVENT_DESTROY, notify_system_event_params_t(&destroy));
	}

	inline void ReportTeleportEvent(CBaseEntity* pEntity, const Vector& prevOrigin, const QAngle& prevAngles, bool physicsRotate)
	{
		notify_teleport_params_t teleport;
		teleport.prevOrigin = prevOrigin;
		teleport.prevAngles = prevAngles;
		teleport.physicsRotate = physicsRotate;
		ReportSystemEvent(pEntity, NOTIFY_EVENT_TELEPORT, notify_system_event_params_t(&teleport));
	}

	virtual void ClearEntity(CBaseEntity* pNotify) = 0;
};

class IEntityListener
{
public:
	virtual void OnEntityCreated(CBaseEntity* pEntity) {};
	virtual void OnEntitySpawned(CBaseEntity* pEntity) {};
	virtual void OnEntityDeleted(CBaseEntity* pEntity) {};
};

extern INotify* g_pNotify;

void EntityTouch_Add(CBaseEntity* pEntity);
void EntityTouch_Remove(CBaseEntity* pEntity);
int AimTarget_ListCount();
int AimTarget_ListCopy(CBaseEntity* pList[], int listMax);
CBaseEntity* AimTarget_ListElement(int iIndex);
void AimTarget_ForceRepopulateList();

void SimThink_EntityChanged(CBaseEntity* pEntity);
int SimThink_ListCount();
int SimThink_ListCopy(CBaseEntity* pList[], int listMax);

#endif  