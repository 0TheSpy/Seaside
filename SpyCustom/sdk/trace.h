#ifndef TRACE_H
#define TRACE_H

#ifdef _WIN32
#pragma once
#endif


#include "mathlib.h"



#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

class CBaseTrace
{
public:

	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	Vector			vecStart;		// start position
	Vector			vecEnd;			// final position
	cplane_t		plane;
	

	float			fraction;				       

	int				contents;				       
	unsigned short	dispFlags;				       

	bool			allsolid;				      
	bool			startsolid;				          


	CBaseTrace() {}

private:
	CBaseTrace(const CBaseTrace& vOther);
};

#endif  