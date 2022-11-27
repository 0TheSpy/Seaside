#ifndef CMODEL_H
#define CMODEL_H
#ifdef _WIN32
#pragma once
#endif

#include "trace.h"
#include "dbg.h"
#include "basehandle.h"

struct edict_t;
struct model_t;

#include "bspflags.h"
#define	AREA_SOLID		1
#define	AREA_TRIGGERS	2

#include "vcollide.h"

struct cmodel_t
{
	Vector		mins, maxs;
	Vector		origin;		    
	int			headnode;

	vcollide_t	vcollisionData;
};

struct csurface_t
{
	const char* name;
	short		surfaceProps;
	unsigned short	flags;		               
};

struct Ray_t
{


	VectorAligned  m_Start;	      
	VectorAligned  m_Delta;	      
	VectorAligned  m_StartOffset;	          
	VectorAligned  m_Extents;	         
	const matrix3x4_t* m_pWorldAxisTransform;
	bool	m_IsRay;	    
	bool	m_IsSwept;	    

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const& start, Vector const& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		m_StartOffset.Init();
		m_Start = start;
	}

	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis)
		{
			if (m_Delta[iAxis] != 0.0f)
			{
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else
			{
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}

private:
};


#endif  


#include "gametrace.h"