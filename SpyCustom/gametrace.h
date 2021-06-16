#ifndef GAMETRACE_H
#define GAMETRACE_H
#ifdef _WIN32
#pragma once
#endif


#include "cmodel.h"
#include "utlvector.h"
#include "ihandleentity.h"
#include "ispatialpartition.h"

#if defined( CLIENT_DLL )
class C_BaseEntity;
#else
class CBaseEntity;
#endif


class CGameTrace : public CBaseTrace
{
public:

	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const;

	bool isVisible() const; 

#if defined( ENGINE_DLL )
	void SetEdict(edict_t* pEdict);
	edict_t* GetEdict() const;
#endif	


public:

	float		fractionleftsolid;		            
	csurface_t	surface;				     
	int			hitgroup;				        
	short		physicsbone;			       

#if defined( CLIENT_DLL )
	C_BaseEntity* m_pEnt;
#else
	IClientEntity* m_pEnt;
#endif
	
	int			hitbox;					      

	CGameTrace() {}

private:
	CGameTrace(const CGameTrace& vOther);
};


inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}


typedef CGameTrace trace_t;

#define TLD_DEF_LEAF_MAX	256
#define TLD_DEF_ENTITY_MAX	1024

class CTraceListData : public IPartitionEnumerator
{
public:

	CTraceListData(int nLeafMax = TLD_DEF_LEAF_MAX, int nEntityMax = TLD_DEF_ENTITY_MAX)
	{
		MEM_ALLOC_CREDIT();
		m_nLeafCount = 0;
		m_aLeafList.SetSize(nLeafMax);

		m_nEntityCount = 0;
		m_aEntityList.SetSize(nEntityMax);
	}

	~CTraceListData()
	{
		m_nLeafCount = 0;
		m_aLeafList.RemoveAll();

		m_nEntityCount = 0;
		m_aEntityList.RemoveAll();
	}

	void Reset(void)
	{
		m_nLeafCount = 0;
		m_nEntityCount = 0;
	}

	bool	IsEmpty(void) const { return (m_nLeafCount == 0 && m_nEntityCount == 0); }

	int		LeafCount(void) const { return m_nLeafCount; }
	int		LeafCountMax(void) const { return m_aLeafList.Count(); }
	void    LeafCountReset(void) { m_nLeafCount = 0; }

	int		EntityCount(void) const { return m_nEntityCount; }
	int		EntityCountMax(void) const { return m_aEntityList.Count(); }
	void	EntityCountReset(void) { m_nEntityCount = 0; }

	void AddLeaf(int iLeaf)
	{
		if (m_nLeafCount >= m_aLeafList.Count())
		{
			DevMsg("CTraceListData: Max leaf count along ray exceeded!\n");
			m_aLeafList.AddMultipleToTail(m_aLeafList.Count());
		}

		m_aLeafList[m_nLeafCount] = iLeaf;
		m_nLeafCount++;
	}

	IterationRetval_t EnumElement(IHandleEntity* pHandleEntity)
	{
		if (m_nEntityCount >= m_aEntityList.Count())
		{
			DevMsg("CTraceListData: Max entity count along ray exceeded!\n");
			m_aEntityList.AddMultipleToTail(m_aEntityList.Count());
		}

		m_aEntityList[m_nEntityCount] = pHandleEntity;
		m_nEntityCount++;

		return ITERATION_CONTINUE;
	}

public:

	int							m_nLeafCount;
	CUtlVector<int>				m_aLeafList;

	int							m_nEntityCount;
	CUtlVector<IHandleEntity*>	m_aEntityList;
};

#endif  