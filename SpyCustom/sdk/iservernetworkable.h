#ifndef ISERVERNETWORKABLE_H
#define ISERVERNETWORKABLE_H
#ifdef _WIN32
#pragma once
#endif


#include "ihandleentity.h"
#include "basetypes.h"
#include "bitvec.h"
#include "const.h"
#include "bspfile.h"



#define	MAX_FAST_ENT_CLUSTERS	4
#define	MAX_ENT_CLUSTERS	64
#define MAX_WORLD_AREAS		8


class ServerClass;
class SendTable;
struct edict_t;
class CBaseEntity;
class CSerialEntity;
class CBaseNetworkable;


class CCheckTransmitInfo
{
public:
	edict_t* m_pClientEnt;	    
	byte	m_PVS[PAD_NUMBER(MAX_MAP_CLUSTERS, 8) / 8];
	int		m_nPVSSize;		    

	CBitVec<MAX_EDICTS>* m_pTransmitEdict;	       
	CBitVec<MAX_EDICTS>* m_pTransmitAlways;               

	int 	m_AreasNetworked;      
	int		m_Areas[MAX_WORLD_AREAS];   

	byte	m_AreaFloodNums[MAX_MAP_AREAS];
	int		m_nMapAreas;
};

struct PVSInfo_t
{
	short		m_nHeadNode;

	short		m_nClusterCount;

	unsigned short* m_pClusters;

	short		m_nAreaNum;
	short		m_nAreaNum2;

	float		m_vCenter[3];

private:
	unsigned short m_pClustersInline[MAX_FAST_ENT_CLUSTERS];

	friend class CVEngineServer;
};


class IServerNetworkable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;

	virtual ServerClass* GetServerClass() = 0;

	virtual edict_t* GetEdict() const = 0;

	virtual const char* GetClassName() const = 0;
	virtual void			Release() = 0;

	virtual int				AreaNum() const = 0;

	virtual CBaseNetworkable* GetBaseNetworkable() = 0;
	virtual CBaseEntity* GetBaseEntity() = 0;      
	virtual PVSInfo_t* GetPVSInfo() = 0;     

protected:
	virtual					~IServerNetworkable() {}
};


#endif  