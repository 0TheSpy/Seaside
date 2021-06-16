#ifndef EDICT_H
#define EDICT_H

#ifdef _WIN32
#pragma once
#endif

#include "vector.h"
#include "cmodel.h"
#include "const.h"
#include "iserverentity.h"
#include "globalvars_base.h"
#include "ICollideable.h"
#include "iservernetworkable.h"
#include "bitvec.h"
#include "convar.h"

struct edict_t;


enum MapLoadType_t
{
	MapLoad_NewGame = 0,
	MapLoad_LoadGame,
	MapLoad_Transition,
	MapLoad_Background,
};


class CGlobalVars : public CGlobalVarsBase
{
public:

	CGlobalVars(bool bIsClient);

public:

	string_t		mapname;
	string_t		mapGroupName;
	int				mapversion;
	string_t		startspot;
	MapLoadType_t	eLoadType;		      
	bool			bMapLoadFailed;	              

	bool			deathmatch;
	bool			coop;
	bool			teamplay;
	int				maxEntities;

	int				serverCount;
	edict_t* pEdicts;
};

inline CGlobalVars::CGlobalVars(bool bIsClient) :
	CGlobalVarsBase(bIsClient)
{
	serverCount = 0;
}


class CPlayerState;
class IServerNetworkable;
class IServerEntity;


#define FL_EDICT_CHANGED	(1<<0)	         
#define FL_EDICT_FREE		(1<<1)	      
#define FL_EDICT_FULL		(1<<2)	      

#define FL_EDICT_FULLCHECK	(0<<0)           
#define FL_EDICT_ALWAYS		(1<<3)	    
#define FL_EDICT_DONTSEND	(1<<4)	    
#define FL_EDICT_PVSCHECK	(1<<5)	       

#define FL_EDICT_PENDING_DORMANT_CHECK	(1<<6)

#define FL_EDICT_DIRTY_PVS_INFORMATION	(1<<7)

#define FL_FULL_EDICT_CHANGED			(1<<8)


#define MAX_CHANGE_OFFSETS	19
#define MAX_EDICT_CHANGE_INFOS	100


class CEdictChangeInfo
{
public:
	unsigned short m_ChangeOffsets[MAX_CHANGE_OFFSETS];
	unsigned short m_nChangeOffsets;
};

class CSharedEdictChangeInfo
{
public:
	CSharedEdictChangeInfo()
	{
		m_iSerialNumber = 1;
	}

	unsigned short m_iSerialNumber;

#ifdef NETWORK_VARS_ENABLED
	CEdictChangeInfo m_ChangeInfos[MAX_EDICT_CHANGE_INFOS];
	unsigned short m_nChangeInfos;	       
#endif
};
extern CSharedEdictChangeInfo* g_pSharedChangeInfo;

class IChangeInfoAccessor
{
public:
	inline void SetChangeInfo(unsigned short info)
	{
		m_iChangeInfo = info;
	}

	inline void SetChangeInfoSerialNumber(unsigned short sn)
	{
		m_iChangeInfoSerialNumber = sn;
	}

	inline unsigned short	 GetChangeInfo() const
	{
		return m_iChangeInfo;
	}

	inline unsigned short	 GetChangeInfoSerialNumber() const
	{
		return m_iChangeInfoSerialNumber;
	}

private:
	unsigned short m_iChangeInfo;
	unsigned short m_iChangeInfoSerialNumber;
};

class CBaseEdict
{
public:

	IServerEntity* GetIServerEntity();
	const IServerEntity* GetIServerEntity() const;

	IServerNetworkable* GetNetworkable();
	IServerUnknown* GetUnknown();

	void				SetEdict(IServerUnknown* pUnk, bool bFullEdict);

	int					AreaNum() const;
	const char* GetClassName() const;

	bool				IsFree() const;
	void				SetFree();
	void				ClearFree();

	bool				HasStateChanged() const;
	void				ClearStateChanged();
	void				StateChanged();
	void				StateChanged(unsigned short offset);

	void				ClearTransmitState();

	void SetChangeInfo(unsigned short info);
	void SetChangeInfoSerialNumber(unsigned short sn);
	unsigned short	 GetChangeInfo() const;
	unsigned short	 GetChangeInfoSerialNumber() const;

public:

#ifdef _XBOX
	unsigned short m_fStateFlags;
#else
	int	m_fStateFlags;
#endif	

	int m_NetworkSerialNumber;	             

	IServerNetworkable* m_pNetworkable;

protected:
	IServerUnknown* m_pUnk;


public:

	IChangeInfoAccessor* GetChangeAccessor();           
	const IChangeInfoAccessor* GetChangeAccessor() const;           
	friend void InitializeEntityDLLFields(edict_t* pEdict);
};


inline IServerEntity* CBaseEdict::GetIServerEntity()
{
	if (m_fStateFlags & FL_EDICT_FULL)
		return (IServerEntity*)m_pUnk;
	else
		return 0;
}

inline bool CBaseEdict::IsFree() const
{
	return (m_fStateFlags & FL_EDICT_FREE) != 0;
}



inline bool	CBaseEdict::HasStateChanged() const
{
	return (m_fStateFlags & FL_EDICT_CHANGED) != 0;
}

inline void	CBaseEdict::ClearStateChanged()
{
	m_fStateFlags &= ~(FL_EDICT_CHANGED | FL_FULL_EDICT_CHANGED);
	SetChangeInfoSerialNumber(0);
}

inline void	CBaseEdict::StateChanged()
{
	m_fStateFlags |= (FL_EDICT_CHANGED | FL_FULL_EDICT_CHANGED);
}

inline void	CBaseEdict::StateChanged(unsigned short offset)
{
#ifdef NETWORK_VARS_ENABLED
	if (m_fStateFlags & FL_FULL_EDICT_CHANGED)
		return;

	m_fStateFlags |= FL_EDICT_CHANGED;

	IChangeInfoAccessor* accessor = GetChangeAccessor();

	if (accessor->GetChangeInfoSerialNumber() == g_pSharedChangeInfo->m_iSerialNumber)
	{
		CEdictChangeInfo* p = &g_pSharedChangeInfo->m_ChangeInfos[accessor->GetChangeInfo()];

		for (unsigned short i = 0; i < p->m_nChangeOffsets; i++)
			if (p->m_ChangeOffsets[i] == offset)
				return;

		if (p->m_nChangeOffsets == MAX_CHANGE_OFFSETS)
		{
			accessor->SetChangeInfoSerialNumber(0);
			m_fStateFlags |= FL_FULL_EDICT_CHANGED;        
		}
		else
		{
			p->m_ChangeOffsets[p->m_nChangeOffsets++] = offset;
		}
	}
	else
	{
		if (g_pSharedChangeInfo->m_nChangeInfos == MAX_EDICT_CHANGE_INFOS)
		{
			accessor->SetChangeInfoSerialNumber(0);
			m_fStateFlags |= FL_FULL_EDICT_CHANGED;
		}
		else
		{
			accessor->SetChangeInfo(g_pSharedChangeInfo->m_nChangeInfos);
			g_pSharedChangeInfo->m_nChangeInfos++;

			accessor->SetChangeInfoSerialNumber(g_pSharedChangeInfo->m_iSerialNumber);

			CEdictChangeInfo* p = &g_pSharedChangeInfo->m_ChangeInfos[accessor->GetChangeInfo()];
			p->m_ChangeOffsets[0] = offset;
			p->m_nChangeOffsets = 1;
		}
	}
#else
	StateChanged();
#endif
}



inline void CBaseEdict::SetFree()
{
	m_fStateFlags |= FL_EDICT_FREE;
}

inline void CBaseEdict::ClearFree()
{
	m_fStateFlags &= ~FL_EDICT_FREE;
}

inline void	CBaseEdict::ClearTransmitState()
{
	m_fStateFlags &= ~(FL_EDICT_ALWAYS | FL_EDICT_PVSCHECK | FL_EDICT_DONTSEND);
}

inline const IServerEntity* CBaseEdict::GetIServerEntity() const
{
	if (m_fStateFlags & FL_EDICT_FULL)
		return (IServerEntity*)m_pUnk;
	else
		return 0;
}

inline IServerUnknown* CBaseEdict::GetUnknown()
{
	return m_pUnk;
}

inline IServerNetworkable* CBaseEdict::GetNetworkable()
{
	return m_pNetworkable;
}

inline void CBaseEdict::SetEdict(IServerUnknown* pUnk, bool bFullEdict)
{
	m_pUnk = pUnk;
	if ((pUnk != NULL) && bFullEdict)
	{
		m_fStateFlags = FL_EDICT_FULL;
	}
	else
	{
		m_fStateFlags = 0;
	}
}

inline int CBaseEdict::AreaNum() const
{
	if (!m_pUnk)
		return 0;

	return m_pNetworkable->AreaNum();
}

inline const char* CBaseEdict::GetClassName() const
{
	if (!m_pUnk)
		return "";
	return m_pNetworkable->GetClassName();
}

inline void CBaseEdict::SetChangeInfo(unsigned short info)
{
	GetChangeAccessor()->SetChangeInfo(info);
}

inline unsigned short CBaseEdict::GetChangeInfo() const
{
	return GetChangeAccessor()->GetChangeInfo();
}

inline unsigned short CBaseEdict::GetChangeInfoSerialNumber() const
{
	return GetChangeAccessor()->GetChangeInfoSerialNumber();
}

struct edict_t : public CBaseEdict
{
public:
	ICollideable* GetCollideable();
};

inline ICollideable* edict_t::GetCollideable()
{
	IServerEntity* pEnt = GetIServerEntity();
	if (pEnt)
		return pEnt->GetCollideable();
	else
		return NULL;
}


#endif  