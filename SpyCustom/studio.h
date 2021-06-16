#ifndef STUDIO_H
#define STUDIO_H

#ifdef _WIN32
#pragma once
#endif

#include "basetypes.h"
#include "vector2d.h"
#include "vector.h"
#include "vector4d.h"
#include "compressed_vector.h"
#include "dbg.h"
#include "threadtools.h"
#include "mathlib.h"
#include "utlvector.h"
#include "utlhash.h"
#include "datamap.h"
#include "generichash.h"
#include "localflexcontroller.h"
#include "utlsymbol.h"

#define STUDIO_ENABLE_PERF_COUNTERS

#define STUDIO_SEQUENCE_ACTIVITY_LOOKUPS_ARE_SLOW 0 
#define STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE 1

class IMaterial;
class IMesh;
class IMorph;
struct virtualmodel_t;
struct vertexFileHeader_t;
struct thinModelVertices_t;

namespace OptimizedModel
{
	struct StripHeader_t;
}


#define STUDIO_VERSION		48

#ifndef _XBOX
#define MAXSTUDIOTRIANGLES	65536	   
#define MAXSTUDIOVERTS		65536	   
#define	MAXSTUDIOFLEXVERTS	10000	              
#else
#define MAXSTUDIOTRIANGLES	25000
#define MAXSTUDIOVERTS		10000
#define	MAXSTUDIOFLEXVERTS	1000
#endif
#define MAXSTUDIOSKINS		32		  
#define MAXSTUDIOBONES		128		    
#define MAXSTUDIOFLEXDESC	1024	         
#define MAXSTUDIOFLEXCTRL	96		      
#define MAXSTUDIOPOSEPARAM	24
#define MAXSTUDIOBONECTRLS	4
#define MAXSTUDIOANIMBLOCKS 256

#define MAXSTUDIOBONEBITS	7		    

#define MAX_NUM_BONES_PER_VERT 3

#define NEW_EVENT_STYLE ( 1 << 10 )

struct mstudiodata_t
{
	int		count;
	int		offset;
};

#define STUDIO_PROC_AXISINTERP	1
#define STUDIO_PROC_QUATINTERP	2
#define STUDIO_PROC_AIMATBONE	3
#define STUDIO_PROC_AIMATATTACH 4
#define STUDIO_PROC_JIGGLE 5

struct mstudioaxisinterpbone_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				control;           
	int				axis;	   
	Vector			pos[6];	      
	Quaternion		quat[6];      

	mstudioaxisinterpbone_t() {}
private:
	mstudioaxisinterpbone_t(const mstudioaxisinterpbone_t& vOther);
};


struct mstudioquatinterpinfo_t
{
	DECLARE_BYTESWAP_DATADESC();
	float			inv_tolerance;	       
	Quaternion		trigger;	   
	Vector			pos;		  
	Quaternion		quat;		  

	mstudioquatinterpinfo_t() {}
private:
	mstudioquatinterpinfo_t(const mstudioquatinterpinfo_t& vOther);
};

struct mstudioquatinterpbone_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				control;    
	int				numtriggers;
	int				triggerindex;
	inline mstudioquatinterpinfo_t* pTrigger(int i) const { return  (mstudioquatinterpinfo_t*)(((byte*)this) + triggerindex) + i; };

	mstudioquatinterpbone_t() {}
private:
	mstudioquatinterpbone_t(const mstudioquatinterpbone_t& vOther);
};


#define JIGGLE_IS_FLEXIBLE				0x01
#define JIGGLE_IS_RIGID					0x02
#define JIGGLE_HAS_YAW_CONSTRAINT		0x04
#define JIGGLE_HAS_PITCH_CONSTRAINT		0x08
#define JIGGLE_HAS_ANGLE_CONSTRAINT		0x10
#define JIGGLE_HAS_LENGTH_CONSTRAINT	0x20
#define JIGGLE_HAS_BASE_SPRING			0x40
#define JIGGLE_IS_BOING					0x80		      

struct mstudiojigglebone_t
{
	DECLARE_BYTESWAP_DATADESC();

	int				flags;

	float			length;					         
	float			tipMass;

	float			yawStiffness;
	float			yawDamping;
	float			pitchStiffness;
	float			pitchDamping;
	float			alongStiffness;
	float			alongDamping;

	float			angleLimit;				      

	float			minYaw;					  
	float			maxYaw;					  
	float			yawFriction;
	float			yawBounce;

	float			minPitch;				  
	float			maxPitch;				  
	float			pitchFriction;
	float			pitchBounce;

	float			baseMass;
	float			baseStiffness;
	float			baseDamping;
	float			baseMinLeft;
	float			baseMaxLeft;
	float			baseLeftFriction;
	float			baseMinUp;
	float			baseMaxUp;
	float			baseUpFriction;
	float			baseMinForward;
	float			baseMaxForward;
	float			baseForwardFriction;

	float			boingImpactSpeed;
	float			boingImpactAngle;
	float			boingDampingRate;
	float			boingFrequency;
	float			boingAmplitude;

private:
};

struct mstudioaimatbone_t
{
	DECLARE_BYTESWAP_DATADESC();

	int				parent;
	int				aim;		     
	Vector			aimvector;
	Vector			upvector;
	Vector			basepos;

	mstudioaimatbone_t() {}
private:
	mstudioaimatbone_t(const mstudioaimatbone_t& vOther);
};

struct mstudiobone_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int		 			parent;		  
	int					bonecontroller[6];	      

	Vector				pos;
	Quaternion			quat;
	RadianEuler			rot;
	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		  
	mutable int			physicsbone;	     
	inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
	int					surfacepropidx;	      
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
	int					contents;		      

	int					unused[8];		   

	mstudiobone_t() {}
private:
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiolinearbone_t
{
	DECLARE_BYTESWAP_DATADESC();

	int numbones;

	int flagsindex;
	inline int flags(int i) const { Assert(i >= 0 && i < numbones); return *((int*)(((byte*)this) + flagsindex) + i); };
	inline int* pflags(int i) { Assert(i >= 0 && i < numbones); return ((int*)(((byte*)this) + flagsindex) + i); };

	int	parentindex;
	inline int parent(int i) const { Assert(i >= 0 && i < numbones); return *((int*)(((byte*)this) + parentindex) + i); };

	int	posindex;
	inline Vector pos(int i) const { Assert(i >= 0 && i < numbones); return *((Vector*)(((byte*)this) + posindex) + i); };

	int quatindex;
	inline Quaternion quat(int i) const { Assert(i >= 0 && i < numbones); return *((Quaternion*)(((byte*)this) + quatindex) + i); };

	int rotindex;
	inline RadianEuler rot(int i) const { Assert(i >= 0 && i < numbones); return *((RadianEuler*)(((byte*)this) + rotindex) + i); };

	int posetoboneindex;
	inline matrix3x4_t poseToBone(int i) const { Assert(i >= 0 && i < numbones); return *((matrix3x4_t*)(((byte*)this) + posetoboneindex) + i); };

	int	posscaleindex;
	inline Vector posscale(int i) const { Assert(i >= 0 && i < numbones); return *((Vector*)(((byte*)this) + posscaleindex) + i); };

	int	rotscaleindex;
	inline Vector rotscale(int i) const { Assert(i >= 0 && i < numbones); return *((Vector*)(((byte*)this) + rotscaleindex) + i); };

	int	qalignmentindex;
	inline Quaternion qalignment(int i) const { Assert(i >= 0 && i < numbones); return *((Quaternion*)(((byte*)this) + qalignmentindex) + i); };

	int unused[6];

	mstudiolinearbone_t() {}
private:
	mstudiolinearbone_t(const mstudiolinearbone_t& vOther);
};


enum StudioBoneFlexComponent_t
{
	STUDIO_BONE_FLEX_INVALID = -1,	 
	STUDIO_BONE_FLEX_TX = 0,		  
	STUDIO_BONE_FLEX_TY = 1,		  
	STUDIO_BONE_FLEX_TZ = 2			  
};


struct mstudioboneflexdrivercontrol_t
{
	DECLARE_BYTESWAP_DATADESC();

	int m_nBoneComponent;		      
	int m_nFlexControllerIndex;	    
	float m_flMin;				           
	float m_flMax;				           

	mstudioboneflexdrivercontrol_t() {}
private:
	mstudioboneflexdrivercontrol_t(const mstudioboneflexdrivercontrol_t& vOther);
};


struct mstudioboneflexdriver_t
{
	DECLARE_BYTESWAP_DATADESC();

	int m_nBoneIndex;			     
	int m_nControlCount;		      
	int m_nControlIndex;		         

	inline mstudioboneflexdrivercontrol_t* pBoneFlexDriverControl(int i) const
	{
		Assert(i >= 0 && i < m_nControlCount);
		return (mstudioboneflexdrivercontrol_t*)(((byte*)this) + m_nControlIndex) + i;
	}

	int unused[3];

	mstudioboneflexdriver_t() {}
private:
	mstudioboneflexdriver_t(const mstudioboneflexdriver_t& vOther);
};


#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01	        
#define BONE_PHYSICS_PROCEDURAL		0x02	     
#define BONE_ALWAYS_PROCEDURAL		0x04	     
#define BONE_SCREEN_ALIGN_SPHERE	0x08	         
#define BONE_SCREEN_ALIGN_CYLINDER	0x10	          

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	         
#define BONE_USED_BY_ATTACHMENT		0x00000200	         
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	            
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	          

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8

#define BONE_TYPE_MASK				0x00F00000
#define BONE_FIXED_ALIGNMENT		0x00100000	             

#define BONE_HAS_SAVEFRAME_POS		0x00200000	 
#define BONE_HAS_SAVEFRAME_ROT		0x00400000	 

struct mstudiobonecontroller_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					bone;	   
	int					type;	       
	float				start;
	float				end;
	int					rest;	     
	int					inputfield;	      
	int					unused[8];
};

struct mstudiobbox_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					bone;
	int					group;				  
	Vector				bbmin;				  
	Vector				bbmax;
	int					szhitboxnameindex;	       
	int					unused[8];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiomodelgroup_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					szlabelindex;	  
	inline char* const pszLabel(void) const { return ((char*)this) + szlabelindex; }
	int					sznameindex;	  
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
};

struct mstudiomodelgrouplookup_t
{
	int					modelgroup;
	int					indexwithingroup;
};

struct mstudioevent_t
{
	DECLARE_BYTESWAP_DATADESC();
	float				cycle;
	int					event;
	int					type;
	inline const char* pszOptions(void) const { return options; }
	char				options[64];

	int					szeventindex;
	inline char* const pszEventName(void) const { return ((char*)this) + szeventindex; }
};

#define	ATTACHMENT_FLAG_WORLD_ALIGN 0x10000

struct mstudioattachment_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	unsigned int		flags;
	int					localbone;
	matrix3x4_t			local;   
	int					unused[8];
};

#define IK_SELF 1
#define IK_WORLD 2
#define IK_GROUND 3
#define IK_RELEASE 4
#define IK_ATTACHMENT 5
#define IK_UNLATCH 6

struct mstudioikerror_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		pos;
	Quaternion	q;

	mstudioikerror_t() {}

private:
	mstudioikerror_t(const mstudioikerror_t& vOther);
};

union mstudioanimvalue_t;

struct mstudiocompressedikerror_t
{
	DECLARE_BYTESWAP_DATADESC();
	float	scale[6];
	short	offset[6];
	inline mstudioanimvalue_t* pAnimvalue(int i) const { if (offset[i] > 0) return  (mstudioanimvalue_t*)(((byte*)this) + offset[i]); else return NULL; };
	mstudiocompressedikerror_t() {}

private:
	mstudiocompressedikerror_t(const mstudiocompressedikerror_t& vOther);
};

struct mstudioikrule_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			index;

	int			type;
	int			chain;

	int			bone;

	int			slot;	       
	float		height;
	float		radius;
	float		floor;
	Vector		pos;
	Quaternion	q;

	int			compressedikerrorindex;
	inline mstudiocompressedikerror_t* pCompressedError() const { return (mstudiocompressedikerror_t*)(((byte*)this) + compressedikerrorindex); };
	int			unused2;

	int			iStart;
	int			ikerrorindex;
	inline mstudioikerror_t* pError(int i) const { return  (ikerrorindex) ? (mstudioikerror_t*)(((byte*)this) + ikerrorindex) + (i - iStart) : NULL; };

	float		start;	   
	float		peak;	    
	float		tail;	    
	float		end;	    

	float		unused3;	 
	float		contact;	     
	float		drop;		           
	float		top;		     

	int			unused6;
	int			unused7;
	int			unused8;

	int			szattachmentindex;		    
	inline char* const pszAttachment(void) const { return ((char*)this) + szattachmentindex; }

	int			unused[7];

	mstudioikrule_t() {}

private:
	mstudioikrule_t(const mstudioikrule_t& vOther);
};


struct mstudioiklock_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			chain;
	float		flPosWeight;
	float		flLocalQWeight;
	int			flags;

	int			unused[4];
};


struct mstudiolocalhierarchy_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			iBone;			   
	int			iNewParent;		    

	float		start;			   
	float		peak;			    
	float		tail;			    
	float		end;			    

	int			iStart;			   

	int			localanimindex;
	inline mstudiocompressedikerror_t* pLocalAnim() const { return (mstudiocompressedikerror_t*)(((byte*)this) + localanimindex); };

	int			unused[4];
};



union mstudioanimvalue_t
{
	struct
	{
		byte	valid;
		byte	total;
	} num;
	short		value;
};

struct mstudioanim_valueptr_t
{
	DECLARE_BYTESWAP_DATADESC();
	short	offset[3];
	inline mstudioanimvalue_t* pAnimvalue(int i) const { if (offset[i] > 0) return  (mstudioanimvalue_t*)(((byte*)this) + offset[i]); else return NULL; };
};

#define STUDIO_ANIM_RAWPOS	0x01  
#define STUDIO_ANIM_RAWROT	0x02  
#define STUDIO_ANIM_ANIMPOS	0x04  
#define STUDIO_ANIM_ANIMROT	0x08  
#define STUDIO_ANIM_DELTA	0x10
#define STUDIO_ANIM_RAWROT2	0x20  


struct mstudioanim_t
{
	DECLARE_BYTESWAP_DATADESC();
	byte				bone;
	byte				flags;		  

	inline byte* pData(void) const { return (((byte*)this) + sizeof(struct mstudioanim_t)); };
	inline mstudioanim_valueptr_t* pRotV(void) const { return (mstudioanim_valueptr_t*)(pData()); };
	inline mstudioanim_valueptr_t* pPosV(void) const { return (mstudioanim_valueptr_t*)(pData()) + ((flags & STUDIO_ANIM_ANIMROT) != 0); };

	inline Quaternion48* pQuat48(void) const { return (Quaternion48*)(pData()); };
	inline Quaternion64* pQuat64(void) const { return (Quaternion64*)(pData()); };
	inline Vector48* pPos(void) const { return (Vector48*)(pData() + ((flags & STUDIO_ANIM_RAWROT) != 0) * sizeof(*pQuat48()) + ((flags & STUDIO_ANIM_RAWROT2) != 0) * sizeof(*pQuat64())); };

	short				nextoffset;
	inline mstudioanim_t* pNext(void) const { if (nextoffset != 0) return  (mstudioanim_t*)(((byte*)this) + nextoffset); else return NULL; };
};

struct mstudiomovement_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					endframe;
	int					motionflags;
	float				v0;			     
	float				v1;			     
	float				angle;		        
	Vector				vector;		        
	Vector				position;	     

	mstudiomovement_t() {}
private:
	mstudiomovement_t(const mstudiomovement_t& vOther);
};

struct studiohdr_t;

struct mstudioanimblock_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					datastart;
	int					dataend;
};

struct mstudioanimsections_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					animblock;
	int					animindex;
};

struct mstudioanimdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					baseptr;
	inline studiohdr_t* pStudiohdr(void) const { return (studiohdr_t*)(((byte*)this) + baseptr); }

	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }

	float				fps;		   	
	int					flags;		  

	int					numframes;

	int					nummovements;
	int					movementindex;
	inline mstudiomovement_t* const pMovement(int i) const { return (mstudiomovement_t*)(((byte*)this) + movementindex) + i; };

	int					unused1[6];			         	

	int					animblock;
	int					animindex;	        
	mstudioanim_t* pAnimBlock(int block, int index) const;           
	mstudioanim_t* pAnim(int* piFrame, float& flStall) const;         
	mstudioanim_t* pAnim(int* piFrame) const;         

	int					numikrules;
	int					ikruleindex;	         
	int					animblockikruleindex;          
	mstudioikrule_t* pIKRule(int i) const;

	int					numlocalhierarchy;
	int					localhierarchyindex;
	mstudiolocalhierarchy_t* pHierarchy(int i) const;

	int					sectionindex;
	int					sectionframes;              
	inline mstudioanimsections_t* const pSection(int i) const { return (mstudioanimsections_t*)(((byte*)this) + sectionindex) + i; }

	short				zeroframespan;	   
	short				zeroframecount;    
	int					zeroframeindex;
	byte* pZeroFrameData() const { if (zeroframeindex) return (((byte*)this) + zeroframeindex); else return NULL; };
	mutable float		zeroframestalltime;		    

	mstudioanimdesc_t() {}
private:
	mstudioanimdesc_t(const mstudioanimdesc_t& vOther);
};

struct mstudioikrule_t;

struct mstudioautolayer_t
{
	DECLARE_BYTESWAP_DATADESC();
	short				iSequence;
	short				iPose;
	int					flags;
	float				start;	   
	float				peak;	    
	float				tail;	    
	float				end;	    
};

struct mstudioactivitymodifier_t
{
	DECLARE_BYTESWAP_DATADESC();

	int					sznameindex;
	inline char* pszName() { return (sznameindex) ? (char*)(((byte*)this) + sznameindex) : NULL; }
};

struct mstudioseqdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					baseptr;
	inline studiohdr_t* pStudiohdr(void) const { return (studiohdr_t*)(((byte*)this) + baseptr); }

	int					szlabelindex;
	inline char* const pszLabel(void) const { return ((char*)this) + szlabelindex; }

	int					szactivitynameindex;
	inline char* const pszActivityName(void) const { return ((char*)this) + szactivitynameindex; }

	int					flags;		  

	int					activity;	       
	int					actweight;

	int					numevents;
	int					eventindex;
	inline mstudioevent_t* pEvent(int i) const { Assert(i >= 0 && i < numevents); return (mstudioevent_t*)(((byte*)this) + eventindex) + i; };

	Vector				bbmin;		    
	Vector				bbmax;

	int					numblends;

	int					animindexindex;

	inline int			anim(int x, int y) const
	{
		if (x >= groupsize[0])
		{
			x = groupsize[0] - 1;
		}

		if (y >= groupsize[1])
		{
			y = groupsize[1] - 1;
		}

		int offset = y * groupsize[0] + x;
		short* blends = (short*)(((byte*)this) + animindexindex);
		int value = (int)blends[offset];
		return value;
	}

	int					movementindex;	      
	int					groupsize[2];
	int					paramindex[2];	      
	float				paramstart[2];	    
	float				paramend[2];	    
	int					paramparent;

	float				fadeintime;		       
	float				fadeouttime;	       

	int					localentrynode;		    
	int					localexitnode;		    
	int					nodeflags;		  

	float				entryphase;		     
	float				exitphase;		     

	float				lastframe;		     

	int					nextseq;		   
	int					pose;			        

	int					numikrules;

	int					numautolayers;	
	int					autolayerindex;
	inline mstudioautolayer_t* pAutolayer(int i) const { Assert(i >= 0 && i < numautolayers); return (mstudioautolayer_t*)(((byte*)this) + autolayerindex) + i; };

	int					weightlistindex;
	inline float* pBoneweight(int i) const { return ((float*)(((byte*)this) + weightlistindex) + i); };
	inline float		weight(int i) const { return *(pBoneweight(i)); };

	int					posekeyindex;
	float* pPoseKey(int iParam, int iAnim) const { return (float*)(((byte*)this) + posekeyindex) + iParam * groupsize[0] + iAnim; }
	float				poseKey(int iParam, int iAnim) const { return *(pPoseKey(iParam, iAnim)); }

	int					numiklocks;
	int					iklockindex;
	inline mstudioiklock_t* pIKLock(int i) const { Assert(i >= 0 && i < numiklocks); return (mstudioiklock_t*)(((byte*)this) + iklockindex) + i; };

	int					keyvalueindex;
	int					keyvaluesize;
	inline const char* KeyValueText(void) const { return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : NULL; }

	int					cycleposeindex;		         

	int					activitymodifierindex;
	int					numactivitymodifiers;
	inline mstudioactivitymodifier_t* pActivityModifier(int i) const { Assert(i >= 0 && i < numactivitymodifiers); return activitymodifierindex != 0 ? (mstudioactivitymodifier_t*)(((byte*)this) + activitymodifierindex) + i : NULL; };

	int					unused[5];		           

	mstudioseqdesc_t() {}
private:
	mstudioseqdesc_t(const mstudioseqdesc_t& vOther);
};


struct mstudioposeparamdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int					flags;	 
	float				start;	  
	float				end;	  
	float				loop;	          
};

struct mstudioflexdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					szFACSindex;
	inline char* const pszFACS(void) const { return ((char*)this) + szFACSindex; }
};



struct mstudioflexcontroller_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sztypeindex;
	inline char* const pszType(void) const { return ((char*)this) + sztypeindex; }
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	mutable int			localToGlobal;	       
	float				min;
	float				max;
};


enum FlexControllerRemapType_t
{
	FLEXCONTROLLER_REMAP_PASSTHRU = 0,
	FLEXCONTROLLER_REMAP_2WAY,	                
	FLEXCONTROLLER_REMAP_NWAY,	                             
	FLEXCONTROLLER_REMAP_EYELID
};


class CStudioHdr;
struct mstudioflexcontrollerui_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }

	int					szindex0;
	int					szindex1;
	int					szindex2;

	inline const mstudioflexcontroller_t* pController(void) const
	{
		return !stereo ? (mstudioflexcontroller_t*)((char*)this + szindex0) : NULL;
	}
	inline char* const	pszControllerName(void) const { return !stereo ? pController()->pszName() : NULL; }
	inline int			controllerIndex(const CStudioHdr& cStudioHdr) const;

	inline const mstudioflexcontroller_t* pLeftController(void) const
	{
		return stereo ? (mstudioflexcontroller_t*)((char*)this + szindex0) : NULL;
	}
	inline char* const	pszLeftName(void) const { return stereo ? pLeftController()->pszName() : NULL; }
	inline int			leftIndex(const CStudioHdr& cStudioHdr) const;

	inline const mstudioflexcontroller_t* pRightController(void) const
	{
		return stereo ? (mstudioflexcontroller_t*)((char*)this + szindex1) : NULL;
	}
	inline char* const	pszRightName(void) const { return stereo ? pRightController()->pszName() : NULL; }
	inline int			rightIndex(const CStudioHdr& cStudioHdr) const;

	inline const mstudioflexcontroller_t* pNWayValueController(void) const
	{
		return remaptype == FLEXCONTROLLER_REMAP_NWAY ? (mstudioflexcontroller_t*)((char*)this + szindex2) : NULL;
	}
	inline char* const	pszNWayValueName(void) const { return remaptype == FLEXCONTROLLER_REMAP_NWAY ? pNWayValueController()->pszName() : NULL; }
	inline int			nWayValueIndex(const CStudioHdr& cStudioHdr) const;

	inline int			Count() const { return (stereo ? 2 : 1) + (remaptype == FLEXCONTROLLER_REMAP_NWAY ? 1 : 0); }
	inline const mstudioflexcontroller_t* pController(int index) const;

	unsigned char		remaptype;	    
	bool				stereo;		     
	byte				unused[2];
};


struct mstudiovertanim_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short		index;
	byte				speed;	 
	byte				side;	 

protected:
	union
	{
		short			delta[3];
		float16			flDelta[3];
	};

	union
	{
		short			ndelta[3];
		float16			flNDelta[3];
	};

public:
	inline void ConvertToFixed(float flVertAnimFixedPointScale)
	{
		delta[0] = flDelta[0].GetFloat() / flVertAnimFixedPointScale;
		delta[1] = flDelta[1].GetFloat() / flVertAnimFixedPointScale;
		delta[2] = flDelta[2].GetFloat() / flVertAnimFixedPointScale;
		ndelta[0] = flNDelta[0].GetFloat() / flVertAnimFixedPointScale;
		ndelta[1] = flNDelta[1].GetFloat() / flVertAnimFixedPointScale;
		ndelta[2] = flNDelta[2].GetFloat() / flVertAnimFixedPointScale;
	}

	inline Vector GetDeltaFixed(float flVertAnimFixedPointScale)
	{
		return Vector(delta[0] * flVertAnimFixedPointScale, delta[1] * flVertAnimFixedPointScale, delta[2] * flVertAnimFixedPointScale);
	}
	inline Vector GetNDeltaFixed(float flVertAnimFixedPointScale)
	{
		return Vector(ndelta[0] * flVertAnimFixedPointScale, ndelta[1] * flVertAnimFixedPointScale, ndelta[2] * flVertAnimFixedPointScale);
	}
	inline void GetDeltaFixed4DAligned(Vector4DAligned* vFillIn, float flVertAnimFixedPointScale)
	{
		vFillIn->Set(delta[0] * flVertAnimFixedPointScale, delta[1] * flVertAnimFixedPointScale, delta[2] * flVertAnimFixedPointScale, 0.0f);
	}
	inline void GetNDeltaFixed4DAligned(Vector4DAligned* vFillIn, float flVertAnimFixedPointScale)
	{
		vFillIn->Set(ndelta[0] * flVertAnimFixedPointScale, ndelta[1] * flVertAnimFixedPointScale, ndelta[2] * flVertAnimFixedPointScale, 0.0f);
	}
	inline Vector GetDeltaFloat()
	{
		return Vector(flDelta[0].GetFloat(), flDelta[1].GetFloat(), flDelta[2].GetFloat());
	}
	inline Vector GetNDeltaFloat()
	{
		return Vector(flNDelta[0].GetFloat(), flNDelta[1].GetFloat(), flNDelta[2].GetFloat());
	}
	inline void SetDeltaFixed(const Vector& vInput, float flVertAnimFixedPointScale)
	{
		delta[0] = vInput.x / flVertAnimFixedPointScale;
		delta[1] = vInput.y / flVertAnimFixedPointScale;
		delta[2] = vInput.z / flVertAnimFixedPointScale;
	}
	inline void SetNDeltaFixed(const Vector& vInputNormal, float flVertAnimFixedPointScale)
	{
		ndelta[0] = vInputNormal.x / flVertAnimFixedPointScale;
		ndelta[1] = vInputNormal.y / flVertAnimFixedPointScale;
		ndelta[2] = vInputNormal.z / flVertAnimFixedPointScale;
	}

	inline void SetDeltaFloat(const Vector& vInput)
	{
		flDelta[0].SetFloat(vInput.x);
		flDelta[1].SetFloat(vInput.y);
		flDelta[2].SetFloat(vInput.z);
	}
	inline void SetNDeltaFloat(const Vector& vInputNormal)
	{
		flNDelta[0].SetFloat(vInputNormal.x);
		flNDelta[1].SetFloat(vInputNormal.y);
		flNDelta[2].SetFloat(vInputNormal.z);
	}

	class CSortByIndex
	{
	public:
		bool operator()(const mstudiovertanim_t& left, const mstudiovertanim_t& right)const
		{
			return left.index < right.index;
		}
	};
	friend class CSortByIndex;

	mstudiovertanim_t() {}
};


struct mstudiovertanim_wrinkle_t : public mstudiovertanim_t
{
	DECLARE_BYTESWAP_DATADESC();

	short	wrinkledelta;

	inline void SetWrinkleFixed(float flWrinkle, float flVertAnimFixedPointScale)
	{
		int nWrinkleDeltaInt = flWrinkle / flVertAnimFixedPointScale;
		wrinkledelta = clamp(nWrinkleDeltaInt, -32767, 32767);
	}

	inline Vector4D GetDeltaFixed(float flVertAnimFixedPointScale)
	{
		return Vector4D(delta[0] * flVertAnimFixedPointScale, delta[1] * flVertAnimFixedPointScale, delta[2] * flVertAnimFixedPointScale, wrinkledelta * flVertAnimFixedPointScale);
	}

	inline void GetDeltaFixed4DAligned(Vector4DAligned* vFillIn, float flVertAnimFixedPointScale)
	{
		vFillIn->Set(delta[0] * flVertAnimFixedPointScale, delta[1] * flVertAnimFixedPointScale, delta[2] * flVertAnimFixedPointScale, wrinkledelta * flVertAnimFixedPointScale);
	}

	inline float GetWrinkleDeltaFixed(float flVertAnimFixedPointScale)
	{
		return wrinkledelta * flVertAnimFixedPointScale;
	}
};


enum StudioVertAnimType_t
{
	STUDIO_VERT_ANIM_NORMAL = 0,
	STUDIO_VERT_ANIM_WRINKLE,
};

struct mstudioflex_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					flexdesc;	  

	float				target0;	 
	float				target1;	 
	float				target2;	 
	float				target3;	 

	int					numverts;
	int					vertindex;

	inline	mstudiovertanim_t* pVertanim(int i) const { Assert(vertanimtype == STUDIO_VERT_ANIM_NORMAL); return (mstudiovertanim_t*)(((byte*)this) + vertindex) + i; };
	inline	mstudiovertanim_wrinkle_t* pVertanimWrinkle(int i) const { Assert(vertanimtype == STUDIO_VERT_ANIM_WRINKLE); return  (mstudiovertanim_wrinkle_t*)(((byte*)this) + vertindex) + i; };

	inline	byte* pBaseVertanim() const { return ((byte*)this) + vertindex; };
	inline	int	VertAnimSizeBytes() const { return (vertanimtype == STUDIO_VERT_ANIM_NORMAL) ? sizeof(mstudiovertanim_t) : sizeof(mstudiovertanim_wrinkle_t); }

	int					flexpair;	   
	unsigned char		vertanimtype;	  
	unsigned char		unusedchar[3];
	int					unused[6];
};


struct mstudioflexop_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		op;
	union
	{
		int		index;
		float	value;
	} d;
};

struct mstudioflexrule_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					flex;
	int					numops;
	int					opindex;
	inline mstudioflexop_t* iFlexOp(int i) const { return  (mstudioflexop_t*)(((byte*)this) + opindex) + i; };
};

struct mstudioboneweight_t
{
	DECLARE_BYTESWAP_DATADESC();
	float	weight[MAX_NUM_BONES_PER_VERT];
	char	bone[MAX_NUM_BONES_PER_VERT];
	byte	numbones;

};

struct mstudiovertex_t
{
	DECLARE_BYTESWAP_DATADESC();
	mstudioboneweight_t	m_BoneWeights;
	Vector				m_vecPosition;
	Vector				m_vecNormal;
	Vector2D			m_vecTexCoord;

	mstudiovertex_t() {}

private:
	mstudiovertex_t(const mstudiovertex_t& vOther);
};

struct mstudiotexture_t
{
	DECLARE_BYTESWAP_DATADESC();
	int						sznameindex;
	inline char* const		pszName(void) const { return ((char*)this) + sznameindex; }
	int						flags;
	int						used;
	int						unused1;
	mutable IMaterial* material;                   
	mutable void* clientmaterial;	        

	int						unused[10];
};

struct mstudioeyeball_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int		bone;
	Vector	org;
	float	zoffset;
	float	radius;
	Vector	up;
	Vector	forward;
	int		texture;

	int		unused1;
	float	iris_scale;
	int		unused2;

	int		upperflexdesc[3];	             
	int		lowerflexdesc[3];
	float	uppertarget[3];		         
	float	lowertarget[3];

	int		upperlidflexdesc;	          
	int		lowerlidflexdesc;
	int		unused[4];			          
	bool	m_bNonFACS;			     
	char	unused3[3];
	int		unused4[7];

	mstudioeyeball_t() {}
private:
	mstudioeyeball_t(const mstudioeyeball_t& vOther);
};


struct mstudioiklink_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		bone;
	Vector	kneeDir;	       
	Vector	unused0;	 

	mstudioiklink_t() {}
private:
	mstudioiklink_t(const mstudioiklink_t& vOther);
};

struct mstudioikchain_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int				linktype;
	int				numlinks;
	int				linkindex;
	inline mstudioiklink_t* pLink(int i) const { return (mstudioiklink_t*)(((byte*)this) + linkindex) + i; };
};


struct mstudioiface_t
{
	unsigned short a, b, c;		   
};


struct mstudiomodel_t;

struct mstudio_modelvertexdata_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector* Position(int i) const;
	Vector* Normal(int i) const;
	Vector4D* TangentS(int i) const;
	Vector2D* Texcoord(int i) const;
	mstudioboneweight_t* BoneWeights(int i) const;
	mstudiovertex_t* Vertex(int i) const;
	bool				HasTangentData(void) const;
	int					GetGlobalVertexIndex(int i) const;
	int					GetGlobalTangentIndex(int i) const;

	const void* pVertexData;
	const void* pTangentData;
};

struct mstudio_meshvertexdata_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector* Position(int i) const;
	Vector* Normal(int i) const;
	Vector4D* TangentS(int i) const;
	Vector2D* Texcoord(int i) const;
	mstudioboneweight_t* BoneWeights(int i) const;
	mstudiovertex_t* Vertex(int i) const;
	bool				HasTangentData(void) const;
	int					GetModelVertexIndex(int i) const;
	int					GetGlobalVertexIndex(int i) const;

	const mstudio_modelvertexdata_t* modelvertexdata;

	int					numLODVertexes[MAX_NUM_LODS];
};

struct mstudiomesh_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					material;

	int					modelindex;
	mstudiomodel_t* pModel() const;

	int					numvertices;		    
	int					vertexoffset;		  

	const mstudio_meshvertexdata_t* GetVertexData(void* pModelData = NULL);
	const thinModelVertices_t* GetThinVertexData(void* pModelData = NULL);

	int					numflexes;			  
	int					flexindex;
	inline mstudioflex_t* pFlex(int i) const { return (mstudioflex_t*)(((byte*)this) + flexindex) + i; };

	int					materialtype;
	int					materialparam;

	int					meshid;

	Vector				center;

	mstudio_meshvertexdata_t vertexdata;

	int					unused[8];    

	mstudiomesh_t() {}
private:
	mstudiomesh_t(const mstudiomesh_t& vOther);
};

struct mstudiomodel_t
{
	DECLARE_BYTESWAP_DATADESC();
	inline const char* pszName(void) const { return name; }
	char				name[64];

	int					type;

	float				boundingradius;

	int					nummeshes;
	int					meshindex;
	inline mstudiomesh_t* pMesh(int i) const { return (mstudiomesh_t*)(((byte*)this) + meshindex) + i; };

	int					numvertices;		    
	int					vertexindex;		  
	int					tangentsindex;		  

	const vertexFileHeader_t* CacheVertexData(void* pModelData);

	const mstudio_modelvertexdata_t* GetVertexData(void* pModelData = NULL);
	const thinModelVertices_t* GetThinVertexData(void* pModelData = NULL);

	int					numattachments;
	int					attachmentindex;

	int					numeyeballs;
	int					eyeballindex;
	inline  mstudioeyeball_t* pEyeball(int i) { return (mstudioeyeball_t*)(((byte*)this) + eyeballindex) + i; };

	mstudio_modelvertexdata_t vertexdata;

	int					unused[8];		   
};

inline bool mstudio_modelvertexdata_t::HasTangentData(void) const
{
	return (pTangentData != NULL);
}

inline int mstudio_modelvertexdata_t::GetGlobalVertexIndex(int i) const
{
	mstudiomodel_t* modelptr = (mstudiomodel_t*)((byte*)this - offsetof(mstudiomodel_t, vertexdata));
	Assert((modelptr->vertexindex % sizeof(mstudiovertex_t)) == 0);
	return (i + (modelptr->vertexindex / sizeof(mstudiovertex_t)));
}

inline int mstudio_modelvertexdata_t::GetGlobalTangentIndex(int i) const
{
	mstudiomodel_t* modelptr = (mstudiomodel_t*)((byte*)this - offsetof(mstudiomodel_t, vertexdata));
	Assert((modelptr->tangentsindex % sizeof(Vector4D)) == 0);
	return (i + (modelptr->tangentsindex / sizeof(Vector4D)));
}

inline mstudiovertex_t* mstudio_modelvertexdata_t::Vertex(int i) const
{
	return (mstudiovertex_t*)pVertexData + GetGlobalVertexIndex(i);
}

inline Vector* mstudio_modelvertexdata_t::Position(int i) const
{
	return &Vertex(i)->m_vecPosition;
}

inline Vector* mstudio_modelvertexdata_t::Normal(int i) const
{
	return &Vertex(i)->m_vecNormal;
}

inline Vector4D* mstudio_modelvertexdata_t::TangentS(int i) const
{
	return (Vector4D*)pTangentData + GetGlobalTangentIndex(i);
}

inline Vector2D* mstudio_modelvertexdata_t::Texcoord(int i) const
{
	return &Vertex(i)->m_vecTexCoord;
}

inline mstudioboneweight_t* mstudio_modelvertexdata_t::BoneWeights(int i) const
{
	return &Vertex(i)->m_BoneWeights;
}

inline mstudiomodel_t* mstudiomesh_t::pModel() const
{
	return (mstudiomodel_t*)(((byte*)this) + modelindex);
}

inline bool mstudio_meshvertexdata_t::HasTangentData(void) const
{
	return modelvertexdata->HasTangentData();
}

inline const mstudio_meshvertexdata_t* mstudiomesh_t::GetVertexData(void* pModelData)
{
	this->pModel()->GetVertexData(pModelData);
	vertexdata.modelvertexdata = &(this->pModel()->vertexdata);

	if (!vertexdata.modelvertexdata->pVertexData)
		return NULL;

	return &vertexdata;
}

inline const thinModelVertices_t* mstudiomesh_t::GetThinVertexData(void* pModelData)
{
	return this->pModel()->GetThinVertexData(pModelData);
}

inline int mstudio_meshvertexdata_t::GetModelVertexIndex(int i) const
{
	mstudiomesh_t* meshptr = (mstudiomesh_t*)((byte*)this - offsetof(mstudiomesh_t, vertexdata));
	return meshptr->vertexoffset + i;
}

inline int mstudio_meshvertexdata_t::GetGlobalVertexIndex(int i) const
{
	return modelvertexdata->GetGlobalVertexIndex(GetModelVertexIndex(i));
}

inline Vector* mstudio_meshvertexdata_t::Position(int i) const
{
	return modelvertexdata->Position(GetModelVertexIndex(i));
};

inline Vector* mstudio_meshvertexdata_t::Normal(int i) const
{
	return modelvertexdata->Normal(GetModelVertexIndex(i));
};

inline Vector4D* mstudio_meshvertexdata_t::TangentS(int i) const
{
	return modelvertexdata->TangentS(GetModelVertexIndex(i));
}

inline Vector2D* mstudio_meshvertexdata_t::Texcoord(int i) const
{
	return modelvertexdata->Texcoord(GetModelVertexIndex(i));
};

inline mstudioboneweight_t* mstudio_meshvertexdata_t::BoneWeights(int i) const
{
	return modelvertexdata->BoneWeights(GetModelVertexIndex(i));
};

inline mstudiovertex_t* mstudio_meshvertexdata_t::Vertex(int i) const
{
	return modelvertexdata->Vertex(GetModelVertexIndex(i));
}

enum studiomeshgroupflags_t
{
	MESHGROUP_IS_FLEXED = 0x1,
	MESHGROUP_IS_HWSKINNED = 0x2,
	MESHGROUP_IS_DELTA_FLEXED = 0x4
};


struct studiomeshgroup_t
{
	IMesh* m_pMesh;
	int				m_NumStrips;
	int				m_Flags;		  
	OptimizedModel::StripHeader_t* m_pStripData;
	unsigned short* m_pGroupIndexToMeshIndex;
	int				m_NumVertices;
	int* m_pUniqueTris;	   
	unsigned short* m_pIndices;
	bool			m_MeshNeedsRestore;
	short			m_ColorMeshID;
	IMorph* m_pMorph;

	inline unsigned short MeshIndex(int i) const { return m_pGroupIndexToMeshIndex[m_pIndices[i]]; }
};


struct studiomeshdata_t
{
	int					m_NumGroup;
	studiomeshgroup_t* m_pMeshGroup;
};

struct studioloddata_t
{
	studiomeshdata_t* m_pMeshData;      
	float				m_SwitchPoint;
	int					numMaterials;
	IMaterial** ppMaterials;       
	int* pMaterialFlags;       

	int* m_pHWMorphDecalBoneRemap;
	int					m_nDecalBoneCount;
};

struct studiohwdata_t
{
	int					m_RootLOD;	       
	int					m_NumLODs;
	studioloddata_t* m_pLODs;
	int					m_NumStudioMeshes;

	inline float LODMetric(float unitSphereSize) const { return (unitSphereSize != 0.0f) ? (100.0f / unitSphereSize) : 0.0f; }
	inline int GetLODForMetric(float lodMetric) const
	{
		if (!m_NumLODs)
			return 0;

		int numLODs = (m_pLODs[m_NumLODs - 1].m_SwitchPoint < 0.0f) ? m_NumLODs - 1 : m_NumLODs;

		for (int i = m_RootLOD; i < numLODs - 1; i++)
		{
			if (m_pLODs[i + 1].m_SwitchPoint > lodMetric)
				return i;
		}

		return numLODs - 1;
	}
};

struct mstudiobodyparts_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int					nummodels;
	int					base;
	int					modelindex;     
	inline mstudiomodel_t* pModel(int i) const { return (mstudiomodel_t*)(((byte*)this) + modelindex) + i; };
};


struct mstudiomouth_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					bone;
	Vector				forward;
	int					flexdesc;

	mstudiomouth_t() {}
private:
	mstudiomouth_t(const mstudiomouth_t& vOther);
};

struct mstudiohitboxset_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const	pszName(void) const { return ((char*)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t* pHitbox(int i) const { return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i; };
};


struct mstudiosrcbonetransform_t
{
	DECLARE_BYTESWAP_DATADESC();

	int			sznameindex;
	inline const char* pszName(void) const { return ((char*)this) + sznameindex; }
	matrix3x4_t	pretransform;
	matrix3x4_t	posttransform;
};


class virtualgroup_t
{
public:
	virtualgroup_t(void) { cache = NULL; };
	void* cache;
	const studiohdr_t* GetStudioHdr(void) const;

	CUtlVector< int > boneMap;				      
	CUtlVector< int > masterBone;			      
	CUtlVector< int > masterSeq;			      
	CUtlVector< int > masterAnim;			      
	CUtlVector< int > masterAttachment;	     
	CUtlVector< int > masterPose;			      
	CUtlVector< int > masterNode;			      
};

struct virtualsequence_t
{
#ifdef _XBOX
	short flags;
	short activity;
	short group;
	short index;
#else
	int	flags;
	int activity;
	int group;
	int index;
#endif
};

struct virtualgeneric_t
{
#ifdef _XBOX
	short group;
	short index;
#else
	int group;
	int index;
#endif
};


struct virtualmodel_t
{
	void AppendSequences(int group, const studiohdr_t* pStudioHdr);
	void AppendAnimations(int group, const studiohdr_t* pStudioHdr);
	void AppendAttachments(int ground, const studiohdr_t* pStudioHdr);
	void AppendPoseParameters(int group, const studiohdr_t* pStudioHdr);
	void AppendBonemap(int group, const studiohdr_t* pStudioHdr);
	void AppendNodes(int group, const studiohdr_t* pStudioHdr);
	void AppendTransitions(int group, const studiohdr_t* pStudioHdr);
	void AppendIKLocks(int group, const studiohdr_t* pStudioHdr);
	void AppendModels(int group, const studiohdr_t* pStudioHdr);
	void UpdateAutoplaySequences(const studiohdr_t* pStudioHdr);

	virtualgroup_t* pAnimGroup(int animation) { return &m_group[m_anim[animation].group]; }        
	virtualgroup_t* pSeqGroup(int sequence)
	{
		if ((unsigned)sequence >= (unsigned)m_seq.Count())
		{
			Assert(0);
			return 0;
		}
		return &m_group[m_seq[sequence].group];
	}        

	CThreadFastMutex m_Lock;

	CUtlVector< virtualsequence_t > m_seq;
	CUtlVector< virtualgeneric_t > m_anim;
	CUtlVector< virtualgeneric_t > m_attachment;
	CUtlVector< virtualgeneric_t > m_pose;
	CUtlVector< virtualgroup_t > m_group;
	CUtlVector< virtualgeneric_t > m_node;
	CUtlVector< virtualgeneric_t > m_iklock;
	CUtlVector< unsigned short > m_autoplaySequences;
};

struct thinModelVertices_t
{
	void Init(int numBoneInfluences, Vector* positions, unsigned short* normals, float* boneWeights, char* boneIndices)
	{
		Assert(positions != NULL);
		Assert(normals != NULL);
		Assert((numBoneInfluences >= 0) && (numBoneInfluences <= 3));
		Assert(numBoneInfluences > 0 ? !!boneIndices : !boneIndices);
		Assert(numBoneInfluences > 1 ? !!boneWeights : !boneWeights);
		m_numBoneInfluences = numBoneInfluences;
		m_vecPositions = positions;
		m_vecNormals = normals;
		m_boneWeights = boneWeights;
		m_boneIndices = boneIndices;
	}

	void SetPosition(int vertIndex, const Vector& position)
	{
		Assert(m_vecPositions);
		m_vecPositions[vertIndex] = position;
	}

	void SetNormal(int vertIndex, const Vector& normal)
	{
		Assert(m_vecNormals);
		unsigned int packedNormal;
		PackNormal_UBYTE4(normal.x, normal.y, normal.z, &packedNormal);
		m_vecNormals[vertIndex] = (unsigned short)(0x0000FFFF & packedNormal);
	}

	void SetBoneWeights(int vertIndex, const mstudioboneweight_t& boneWeights)
	{
		Assert((m_numBoneInfluences >= 1) && (m_numBoneInfluences <= 3));
		Assert((boneWeights.numbones >= 1) && (boneWeights.numbones <= m_numBoneInfluences));
		int    numStoredWeights = max(0, (m_numBoneInfluences - 1));
		float* pBaseWeight = m_boneWeights + vertIndex * numStoredWeights;
		char* pBaseIndex = m_boneIndices + vertIndex * m_numBoneInfluences;
		for (int i = 0; i < m_numBoneInfluences; i++)
		{
			pBaseIndex[i] = boneWeights.bone[i];
		}
		for (int i = 0; i < numStoredWeights; i++)
		{
			pBaseWeight[i] = boneWeights.weight[i];
		}
	}

	void GetMeshPosition(mstudiomesh_t* pMesh, int meshIndex, Vector* pPosition) const
	{
		Assert(pMesh);
		GetPosition(pMesh->vertexdata.GetGlobalVertexIndex(meshIndex), pPosition);
	}

	void GetMeshNormal(mstudiomesh_t* pMesh, int meshIndex, Vector* pNormal) const
	{
		Assert(pMesh);
		GetNormal(pMesh->vertexdata.GetGlobalVertexIndex(meshIndex), pNormal);
	}

	void GetMeshBoneWeights(mstudiomesh_t* pMesh, int meshIndex, mstudioboneweight_t* pBoneWeights) const
	{
		Assert(pMesh);
		GetBoneWeights(pMesh->vertexdata.GetGlobalVertexIndex(meshIndex), pBoneWeights);
	}

	void GetModelPosition(mstudiomodel_t* pModel, int modelIndex, Vector* pPosition) const
	{
		Assert(pModel);
		GetPosition(pModel->vertexdata.GetGlobalVertexIndex(modelIndex), pPosition);
	}

	void GetModelNormal(mstudiomodel_t* pModel, int modelIndex, Vector* pNormal) const
	{
		Assert(pModel);
		GetNormal(pModel->vertexdata.GetGlobalVertexIndex(modelIndex), pNormal);
	}

	void GetModelBoneWeights(mstudiomodel_t* pModel, int modelIndex, mstudioboneweight_t* pBoneWeights) const
	{
		Assert(pModel);
		GetBoneWeights(pModel->vertexdata.GetGlobalVertexIndex(modelIndex), pBoneWeights);
	}

private:
	void GetPosition(int vertIndex, Vector* pPosition) const
	{
		Assert(pPosition);
		Assert(m_vecPositions);
		*pPosition = m_vecPositions[vertIndex];
	}

	void GetNormal(int vertIndex, Vector* pNormal) const
	{
		Assert(pNormal);
		Assert(m_vecNormals);
		unsigned int packedNormal = 0x0000FFFF & m_vecNormals[vertIndex];
		UnpackNormal_UBYTE4(&packedNormal, pNormal->Base());
	}

	void GetBoneWeights(int vertIndex, mstudioboneweight_t* pBoneWeights) const
	{
		Assert(pBoneWeights);
		Assert((m_numBoneInfluences <= 1) || (m_boneWeights != NULL));
		Assert((m_numBoneInfluences <= 0) || (m_boneIndices != NULL));
		int    numStoredWeights = max(0, (m_numBoneInfluences - 1));
		float* pBaseWeight = m_boneWeights + vertIndex * numStoredWeights;
		char* pBaseIndex = m_boneIndices + vertIndex * m_numBoneInfluences;
		float  sum = 0.0f;
		for (int i = 0; i < MAX_NUM_BONES_PER_VERT; i++)
		{
			if (i < (m_numBoneInfluences - 1))
				pBoneWeights->weight[i] = pBaseWeight[i];
			else
				pBoneWeights->weight[i] = 1.0f - sum;
			sum += pBoneWeights->weight[i];

			pBoneWeights->bone[i] = (i < m_numBoneInfluences) ? pBaseIndex[i] : 0;
		}

		pBoneWeights->numbones = m_numBoneInfluences ? m_numBoneInfluences : 1;
	}

	int				m_numBoneInfluences;       
	float* m_boneWeights;		           
	char* m_boneIndices;		       
	Vector* m_vecPositions;
	unsigned short* m_vecNormals;		          
};

#define MODEL_VERTEX_FILE_ID		(('V'<<24)+('S'<<16)+('D'<<8)+'I')
#define MODEL_VERTEX_FILE_VERSION	4
#define MODEL_VERTEX_FILE_THIN_ID	(('V'<<24)+('C'<<16)+('D'<<8)+'I')

struct vertexFileHeader_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		id;								 
	int		version;						 
	int		checksum;						     
	int		numLODs;						    
	int		numLODVertexes[MAX_NUM_LODS];	      
	int		numFixups;						   
	int		fixupTableStart;				      
	int		vertexDataStart;				      
	int		tangentDataStart;				      

public:

	const mstudiovertex_t* GetVertexData() const
	{
		if ((id == MODEL_VERTEX_FILE_ID) && (vertexDataStart != 0))
			return (mstudiovertex_t*)(vertexDataStart + (byte*)this);
		else
			return NULL;
	}
	const Vector4D* GetTangentData() const
	{
		if ((id == MODEL_VERTEX_FILE_ID) && (tangentDataStart != 0))
			return (Vector4D*)(tangentDataStart + (byte*)this);
		else
			return NULL;
	}
	const  thinModelVertices_t* GetThinVertexData() const
	{
		if ((id == MODEL_VERTEX_FILE_THIN_ID) && (vertexDataStart != 0))
			return (thinModelVertices_t*)(vertexDataStart + (byte*)this);
		else
			return NULL;
	}
};

inline const mstudio_modelvertexdata_t* mstudiomodel_t::GetVertexData(void* pModelData)
{
	const vertexFileHeader_t* pVertexHdr = CacheVertexData(pModelData);
	if (!pVertexHdr)
	{
		vertexdata.pVertexData = NULL;
		vertexdata.pTangentData = NULL;
		return NULL;
	}

	vertexdata.pVertexData = pVertexHdr->GetVertexData();
	vertexdata.pTangentData = pVertexHdr->GetTangentData();

	if (!vertexdata.pVertexData)
		return NULL;

	return &vertexdata;
}

inline const thinModelVertices_t* mstudiomodel_t::GetThinVertexData(void* pModelData)
{
	const vertexFileHeader_t* pVertexHdr = CacheVertexData(pModelData);
	if (!pVertexHdr)
		return NULL;

	return pVertexHdr->GetThinVertexData();
}

struct vertexFileFixup_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		lod;				      
	int		sourceVertexID;		       
	int		numVertexes;
};

#define STUDIOHDR_FLAGS_AUTOGENERATED_HITBOX				0x00000001

#define STUDIOHDR_FLAGS_USES_ENV_CUBEMAP					0x00000002

#define STUDIOHDR_FLAGS_FORCE_OPAQUE						0x00000004

#define STUDIOHDR_FLAGS_TRANSLUCENT_TWOPASS					0x00000008

#define STUDIOHDR_FLAGS_STATIC_PROP							0x00000010

#define STUDIOHDR_FLAGS_USES_FB_TEXTURE						0x00000020

#define STUDIOHDR_FLAGS_HASSHADOWLOD						0x00000040

#define STUDIOHDR_FLAGS_USES_BUMPMAPPING					0x00000080

#define STUDIOHDR_FLAGS_USE_SHADOWLOD_MATERIALS				0x00000100

#define STUDIOHDR_FLAGS_OBSOLETE							0x00000200

#define STUDIOHDR_FLAGS_UNUSED								0x00000400

#define STUDIOHDR_FLAGS_NO_FORCED_FADE						0x00000800

#define STUDIOHDR_FLAGS_FORCE_PHONEME_CROSSFADE				0x00001000

#define STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT		0x00002000

#define STUDIOHDR_FLAGS_FLEXES_CONVERTED					0x00004000

#define STUDIOHDR_FLAGS_BUILT_IN_PREVIEW_MODE				0x00008000

#define STUDIOHDR_FLAGS_AMBIENT_BOOST						0x00010000

#define STUDIOHDR_FLAGS_DO_NOT_CAST_SHADOWS					0x00020000

#define STUDIOHDR_FLAGS_CAST_TEXTURE_SHADOWS				0x00040000


#define STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE			0x00200000

struct studiohdr2_t
{
	DECLARE_BYTESWAP_DATADESC();
	int numsrcbonetransform;
	int srcbonetransformindex;

	int	illumpositionattachmentindex;
	inline int			IllumPositionAttachmentIndex() const { return illumpositionattachmentindex; }

	float flMaxEyeDeflection;
	inline float		MaxEyeDeflection() const { return flMaxEyeDeflection != 0.0f ? flMaxEyeDeflection : 0.866f; }       

	int linearboneindex;
	inline mstudiolinearbone_t* pLinearBones() const { return (linearboneindex) ? (mstudiolinearbone_t*)(((byte*)this) + linearboneindex) : NULL; }

	int sznameindex;
	inline char* pszName() { return (sznameindex) ? (char*)(((byte*)this) + sznameindex) : NULL; }

	int m_nBoneFlexDriverCount;
	int m_nBoneFlexDriverIndex;
	inline mstudioboneflexdriver_t* pBoneFlexDriver(int i) const { Assert(i >= 0 && i < m_nBoneFlexDriverCount); return (mstudioboneflexdriver_t*)(((byte*)this) + m_nBoneFlexDriverIndex) + i; }

	int reserved[56];
};

struct studiohdr_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					id;
	int					version;
	int					checksum;		              
	inline const char* pszName(void) const { if (studiohdr2index && pStudioHdr2()->pszName()) return pStudioHdr2()->pszName(); else return name; }
	char				name[64];
	int					length;
	Vector				eyeposition;	   
	Vector				illumposition;	  
	Vector				hull_min;		    
	Vector				hull_max;
	Vector				view_bbmin;		   
	Vector				view_bbmax;
	int					flags;
	int					numbones;			 
	int					boneindex;
	inline mstudiobone_t* pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t*)(((byte*)this) + boneindex) + i; };
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	       
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		       
	int					numbonecontrollers;		  
	int					bonecontrollerindex;
	inline mstudiobonecontroller_t* pBonecontroller(int i) const { Assert(i >= 0 && i < numbonecontrollers); return (mstudiobonecontroller_t*)(((byte*)this) + bonecontrollerindex) + i; };
	int					numhitboxsets;
	int					hitboxsetindex;

	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		Assert(i >= 0 && i < numhitboxsets);
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	};

	inline mstudiobbox_t* pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	inline int			iHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};


	int					numlocalanim;			 
	int					localanimindex;		  
	inline mstudioanimdesc_t* pLocalAnimdesc(int i) const { if (i < 0 || i >= numlocalanim) i = 0; return (mstudioanimdesc_t*)(((byte*)this) + localanimindex) + i; };

	int					numlocalseq;				 
	int					localseqindex;
	inline mstudioseqdesc_t* pLocalSeqdesc(int i) const { if (i < 0 || i >= numlocalseq) i = 0; return (mstudioseqdesc_t*)(((byte*)this) + localseqindex) + i; };

	bool				SequencesAvailable() const;
	int					GetNumSeq() const;
	mstudioanimdesc_t& pAnimdesc(int i) const;
	mstudioseqdesc_t& pSeqdesc(int i) const;
	int					iRelativeAnim(int baseseq, int relanim) const;	         
	int					iRelativeSeq(int baseseq, int relseq) const;		         

	mutable int			activitylistversion;	        
	mutable int			eventsindexed;
	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version) const;
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	int					numtextures;
	int					textureindex;
	inline mstudiotexture_t* pTexture(int i) const { Assert(i >= 0 && i < numtextures); return (mstudiotexture_t*)(((byte*)this) + textureindex) + i; };


	int					numcdtextures;
	int					cdtextureindex;
	inline char* pCdtexture(int i) const { return (((char*)this) + *((int*)(((byte*)this) + cdtextureindex) + i)); };

	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short* pSkinref(int i) const { return (short*)(((byte*)this) + skinindex) + i; };

	int					numbodyparts;
	int					bodypartindex;
	inline mstudiobodyparts_t* pBodypart(int i) const { return (mstudiobodyparts_t*)(((byte*)this) + bodypartindex) + i; };

	int					numlocalattachments;
	int					localattachmentindex;
	inline mstudioattachment_t* pLocalAttachment(int i) const { Assert(i >= 0 && i < numlocalattachments); return (mstudioattachment_t*)(((byte*)this) + localattachmentindex) + i; };
	int					GetNumAttachments(void) const;
	const mstudioattachment_t& pAttachment(int i) const;
	int					GetAttachmentBone(int i);
	void				SetAttachmentBone(int iAttachment, int iBone);

	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char* pszLocalNodeName(int iNode) const { Assert(iNode >= 0 && iNode < numlocalnodes); return (((char*)this) + *((int*)(((byte*)this) + localnodenameindex) + iNode)); }
	inline byte* pLocalTransition(int i) const { Assert(i >= 0 && i < (numlocalnodes* numlocalnodes)); return (byte*)(((byte*)this) + localnodeindex) + i; };

	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char* pszNodeName(int iNode);
	int					GetTransition(int iFrom, int iTo) const;

	int					numflexdesc;
	int					flexdescindex;
	inline mstudioflexdesc_t* pFlexdesc(int i) const { Assert(i >= 0 && i < numflexdesc); return (mstudioflexdesc_t*)(((byte*)this) + flexdescindex) + i; };

	int					numflexcontrollers;
	int					flexcontrollerindex;
	inline mstudioflexcontroller_t* pFlexcontroller(LocalFlexController_t i) const { Assert(numflexcontrollers == 0 || (i >= 0 && i < numflexcontrollers)); return (mstudioflexcontroller_t*)(((byte*)this) + flexcontrollerindex) + i; };

	int					numflexrules;
	int					flexruleindex;
	inline mstudioflexrule_t* pFlexRule(int i) const { Assert(i >= 0 && i < numflexrules); return (mstudioflexrule_t*)(((byte*)this) + flexruleindex) + i; };

	int					numikchains;
	int					ikchainindex;
	inline mstudioikchain_t* pIKChain(int i) const { Assert(i >= 0 && i < numikchains); return (mstudioikchain_t*)(((byte*)this) + ikchainindex) + i; };

	int					nummouths;
	int					mouthindex;
	inline mstudiomouth_t* pMouth(int i) const { Assert(i >= 0 && i < nummouths); return (mstudiomouth_t*)(((byte*)this) + mouthindex) + i; };

	int					numlocalposeparameters;
	int					localposeparamindex;
	inline mstudioposeparamdesc_t* pLocalPoseParameter(int i) const { Assert(i >= 0 && i < numlocalposeparameters); return (mstudioposeparamdesc_t*)(((byte*)this) + localposeparamindex) + i; };
	int					GetNumPoseParameters(void) const;
	const mstudioposeparamdesc_t& pPoseParameter(int i);
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					surfacepropindex;
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropindex; }

	int					keyvalueindex;
	int					keyvaluesize;
	inline const char* KeyValueText(void) const { return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : NULL; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;
	inline mstudioiklock_t* pLocalIKAutoplayLock(int i) const { Assert(i >= 0 && i < numlocalikautoplaylocks); return (mstudioiklock_t*)(((byte*)this) + localikautoplaylockindex) + i; };

	int					GetNumIKAutoplayLocks(void) const;
	const mstudioiklock_t& pIKAutoplayLock(int i);
	int					CountAutoplaySequences() const;
	int					CopyAutoplaySequences(unsigned short* pOut, int outCount) const;
	int					GetAutoplayList(unsigned short** pOut) const;

	float				mass;
	int					contents;

	int					numincludemodels;
	int					includemodelindex;
	inline mstudiomodelgroup_t* pModelGroup(int i) const { Assert(i >= 0 && i < numincludemodels); return (mstudiomodelgroup_t*)(((byte*)this) + includemodelindex) + i; };
	const studiohdr_t* FindModel(void** cache, char const* modelname) const;

	mutable void* virtualModel;
	virtualmodel_t* GetVirtualModel(void) const;

	int					szanimblocknameindex;
	inline char* const pszAnimBlockName(void) const { return ((char*)this) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	inline mstudioanimblock_t* pAnimBlock(int i) const { Assert(i > 0 && i < numanimblocks); return (mstudioanimblock_t*)(((byte*)this) + animblockindex) + i; };
	mutable void* animblockModel;
	byte* GetAnimBlock(int i) const;

	int					bonetablebynameindex;
	inline const byte* GetBoneTableSortedByName() const { return (byte*)this + bonetablebynameindex; }

	void* pVertexBase;
	void* pIndexBase;

	byte				constdirectionallightdot;

	byte				rootLOD;

	byte				numAllowedRootLODs;

	byte				unused[1];

	int					unused4;       

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;
	mstudioflexcontrollerui_t* pFlexControllerUI(int i) const { Assert(i >= 0 && i < numflexcontrollerui); return (mstudioflexcontrollerui_t*)(((byte*)this) + flexcontrolleruiindex) + i; }

	float				flVertAnimFixedPointScale;
	inline float		VertAnimFixedPointScale() const { return (flags & STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE) ? flVertAnimFixedPointScale : 1.0f / 4096.0f; }

	int					unused3[1];

	int					studiohdr2index;
	studiohdr2_t* pStudioHdr2() const { return (studiohdr2_t*)(((byte*)this) + studiohdr2index); }

	int					NumSrcBoneTransforms() const { return studiohdr2index ? pStudioHdr2()->numsrcbonetransform : 0; }
	const mstudiosrcbonetransform_t* SrcBoneTransform(int i) const { Assert(i >= 0 && i < NumSrcBoneTransforms()); return (mstudiosrcbonetransform_t*)(((byte*)this) + pStudioHdr2()->srcbonetransformindex) + i; }

	inline int			IllumPositionAttachmentIndex() const { return studiohdr2index ? pStudioHdr2()->IllumPositionAttachmentIndex() : 0; }

	inline float		MaxEyeDeflection() const { return studiohdr2index ? pStudioHdr2()->MaxEyeDeflection() : 0.866f; }       

	inline mstudiolinearbone_t* pLinearBones() const { return studiohdr2index ? pStudioHdr2()->pLinearBones() : NULL; }

	inline int			BoneFlexDriverCount() const { return studiohdr2index ? pStudioHdr2()->m_nBoneFlexDriverCount : 0; }
	inline const mstudioboneflexdriver_t* BoneFlexDriver(int i) const { Assert(i >= 0 && i < BoneFlexDriverCount()); return studiohdr2index ? pStudioHdr2()->pBoneFlexDriver(i) : NULL; }

	int					unused2[1];

	studiohdr_t() {}

private:
	studiohdr_t(const studiohdr_t& vOther);

	friend struct virtualmodel_t;
};



class IDataCache;
class IMDLCache;

class CStudioHdr
{
public:
	CStudioHdr(void);
	CStudioHdr(const studiohdr_t* pStudioHdr, IMDLCache* mdlcache = NULL);
	~CStudioHdr() { Term(); }

	void Init(const studiohdr_t* pStudioHdr, IMDLCache* mdlcache = NULL);
	void Term();

public:
	inline bool IsVirtual(void) { return (m_pVModel != NULL); };
	inline bool IsValid(void) { return (m_pStudioHdr != NULL); };
	inline bool IsReadyForAccess(void) const { return (m_pStudioHdr != NULL); };
	inline virtualmodel_t* GetVirtualModel(void) const { return m_pVModel; };
	inline const studiohdr_t* GetRenderHdr(void) const { return m_pStudioHdr; };
	const studiohdr_t* pSeqStudioHdr(int sequence);
	const studiohdr_t* pAnimStudioHdr(int animation);

private:
	mutable const studiohdr_t* m_pStudioHdr;
	mutable virtualmodel_t* m_pVModel;

	const virtualmodel_t* ResetVModel(const virtualmodel_t* pVModel) const;
	const studiohdr_t* GroupStudioHdr(int group);
	mutable CUtlVector< const studiohdr_t* > m_pStudioHdrCache;

	mutable int			m_nFrameUnlockCounter;
	int* m_pFrameUnlockCounter;
	CThreadFastMutex	m_FrameUnlockCounterMutex;

public:
	inline int			numbones(void) const { return m_pStudioHdr->numbones; };
	inline mstudiobone_t* pBone(int i) const { return m_pStudioHdr->pBone(i); };
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		       
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	       

	bool				SequencesAvailable() const;
	int					GetNumSeq(void) const;
	mstudioanimdesc_t& pAnimdesc(int i);
	mstudioseqdesc_t& pSeqdesc(int iSequence);
	int					iRelativeAnim(int baseseq, int relanim) const;	         
	int					iRelativeSeq(int baseseq, int relseq) const;		         

	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version);
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	int					GetNumAttachments(void) const;
	const mstudioattachment_t& pAttachment(int i);
	int					GetAttachmentBone(int i);
	void				SetAttachmentBone(int iAttachment, int iBone);

	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char* pszNodeName(int iNode);
	int					GetTransition(int iFrom, int iTo) const;

	int					GetNumPoseParameters(void) const;
	const mstudioposeparamdesc_t& pPoseParameter(int i);
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					GetNumIKAutoplayLocks(void) const;
	const mstudioiklock_t& pIKAutoplayLock(int i);

	inline int			CountAutoplaySequences() const { return m_pStudioHdr->CountAutoplaySequences(); };
	inline int			CopyAutoplaySequences(unsigned short* pOut, int outCount) const { return m_pStudioHdr->CopyAutoplaySequences(pOut, outCount); };
	inline int			GetAutoplayList(unsigned short** pOut) const { return m_pStudioHdr->GetAutoplayList(pOut); };

	inline int			GetNumBoneControllers(void) const { return m_pStudioHdr->numbonecontrollers; };
	inline mstudiobonecontroller_t* pBonecontroller(int i) const { return m_pStudioHdr->pBonecontroller(i); };

	inline int			numikchains() const { return m_pStudioHdr->numikchains; };
	inline int			GetNumIKChains(void) const { return m_pStudioHdr->numikchains; };
	inline mstudioikchain_t* pIKChain(int i) const { return m_pStudioHdr->pIKChain(i); };

	inline int			numflexrules() const { return m_pStudioHdr->numflexrules; };
	inline mstudioflexrule_t* pFlexRule(int i) const { return m_pStudioHdr->pFlexRule(i); };

	inline int			numflexdesc() const { return m_pStudioHdr->numflexdesc; };
	inline mstudioflexdesc_t* pFlexdesc(int i) const { return m_pStudioHdr->pFlexdesc(i); };

	inline LocalFlexController_t			numflexcontrollers() const { return (LocalFlexController_t)m_pStudioHdr->numflexcontrollers; };
	inline mstudioflexcontroller_t* pFlexcontroller(LocalFlexController_t i) const { return m_pStudioHdr->pFlexcontroller(i); };

	inline int			numflexcontrollerui() const { return m_pStudioHdr->numflexcontrollerui; };
	inline mstudioflexcontrollerui_t* pFlexcontrollerUI(int i) const { return m_pStudioHdr->pFlexControllerUI(i); };

	inline const char* pszName() const { return m_pStudioHdr->pszName(); };

	inline int			numbonecontrollers() const { return m_pStudioHdr->numbonecontrollers; };

	inline int			numhitboxsets() const { return m_pStudioHdr->numhitboxsets; };
	inline mstudiohitboxset_t* pHitboxSet(int i) const { return m_pStudioHdr->pHitboxSet(i); };

	inline mstudiobbox_t* pHitbox(int i, int set) const { return m_pStudioHdr->pHitbox(i, set); };
	inline int			iHitboxCount(int set) const { return m_pStudioHdr->iHitboxCount(set); };

	inline int			numbodyparts() const { return m_pStudioHdr->numbodyparts; };
	inline mstudiobodyparts_t* pBodypart(int i) const { return m_pStudioHdr->pBodypart(i); };

	inline int			numskinfamilies() const { return m_pStudioHdr->numskinfamilies; }

	inline Vector		eyeposition() const { return m_pStudioHdr->eyeposition; };

	inline int			flags() const { return m_pStudioHdr->flags; };

	inline char* const pszSurfaceProp(void) const { return m_pStudioHdr->pszSurfaceProp(); };

	inline float		mass() const { return m_pStudioHdr->mass; };
	inline int			contents() const { return m_pStudioHdr->contents; }

	inline const byte* GetBoneTableSortedByName() const { return m_pStudioHdr->GetBoneTableSortedByName(); };

	inline Vector		illumposition() const { return m_pStudioHdr->illumposition; };

	inline Vector		hull_min() const { return m_pStudioHdr->hull_min; };		    
	inline Vector		hull_max() const { return m_pStudioHdr->hull_max; };

	inline Vector		view_bbmin() const { return m_pStudioHdr->view_bbmin; };		   
	inline Vector		view_bbmax() const { return m_pStudioHdr->view_bbmax; };

	inline int			numtextures() const { return m_pStudioHdr->numtextures; };

	inline int			IllumPositionAttachmentIndex() const { return m_pStudioHdr->IllumPositionAttachmentIndex(); }

	inline float		MaxEyeDeflection() const { return m_pStudioHdr->MaxEyeDeflection(); }

	inline mstudiolinearbone_t* pLinearBones() const { return m_pStudioHdr->pLinearBones(); }

	inline int			BoneFlexDriverCount() const { return m_pStudioHdr->BoneFlexDriverCount(); }
	inline const mstudioboneflexdriver_t* BoneFlexDriver(int i) const { return m_pStudioHdr->BoneFlexDriver(i); }

	inline float		VertAnimFixedPointScale() const { return m_pStudioHdr->VertAnimFixedPointScale(); }

public:
	int IsSequenceLooping(int iSequence);
	float GetSequenceCycleRate(int iSequence);

	void				RunFlexRules(const float* src, float* dest);


public:
	inline int boneFlags(int iBone) const { return m_boneFlags[iBone]; }
	inline int boneParent(int iBone) const { return m_boneParent[iBone]; }

private:
	CUtlVector< int >  m_boneFlags;
	CUtlVector< int >  m_boneParent;

public:

	class CActivityToSequenceMapping   
	{
	public:
		struct SequenceTuple
		{
			short		seqnum;
			short		weight;           
			CUtlSymbol* pActivityModifiers;		     
			int			iNumActivityModifiers;
		};

		struct HashValueType
		{
			int activityIdx;

			int startingIdx;
			int count;
			int totalWeight;

			HashValueType(int _actIdx, int _stIdx, int _ct, int _tW) :
				activityIdx(_actIdx), startingIdx(_stIdx), count(_ct), totalWeight(_tW) {}

			HashValueType() : activityIdx(-1), startingIdx(-1), count(-1), totalWeight(-1)
			{
				AssertMsg(false, "Don't use default HashValueType()!");
			}


			class HashFuncs
			{
			public:
				HashFuncs(int) {}

				bool operator()(const HashValueType& lhs, const HashValueType& rhs) const
				{
					return lhs.activityIdx == rhs.activityIdx;
				}

				unsigned int operator()(const HashValueType& item) const
				{
					return HashInt(item.activityIdx);
				}
			};
		};

		typedef CUtlHash<HashValueType, HashValueType::HashFuncs, HashValueType::HashFuncs> ActivityToValueIdxHash;

		CActivityToSequenceMapping(void)
			: m_pSequenceTuples(NULL), m_iSequenceTuplesCount(0), m_ActToSeqHash(8, 0, 0), m_expectedPStudioHdr(NULL), m_expectedVModel(NULL)
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
			, m_bIsInitialized(false)
#endif
		{};

		~CActivityToSequenceMapping()
		{
			if (m_pSequenceTuples != NULL)
			{
				if (m_pSequenceTuples->pActivityModifiers != NULL)
				{
					delete[] m_pSequenceTuples->pActivityModifiers;
				}
				delete[] m_pSequenceTuples;
			}
		}

		const SequenceTuple* GetSequences(int forActivity, int* outSequenceCount, int* outTotalWeight);

		int NumSequencesForActivity(int forActivity);

#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		inline bool IsInitialized(void) { return m_bIsInitialized; }
#endif

	private:

		void Initialize(CStudioHdr* pstudiohdr);

		void Reinitialize(CStudioHdr* pstudiohdr);

		int SelectWeightedSequence(CStudioHdr* pstudiohdr, int activity, int curSequence);

		int SelectWeightedSequenceFromModifiers(CStudioHdr* pstudiohdr, int activity, CUtlSymbol* pActivityModifiers, int iModifierCount);

		SequenceTuple* m_pSequenceTuples;
		unsigned int m_iSequenceTuplesCount;      
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		bool m_bIsInitialized;
#endif

		ActivityToValueIdxHash m_ActToSeqHash;

		const void* m_expectedPStudioHdr;
		const void* m_expectedVModel;

		bool ValidateAgainst(const CStudioHdr* RESTRICT pstudiohdr);
		void SetValidationPair(const CStudioHdr* RESTRICT pstudiohdr);

		friend class CStudioHdr;
	};

	CActivityToSequenceMapping m_ActivityToSequence;

	inline int SelectWeightedSequence(int activity, int curSequence)
	{
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		if (!m_ActivityToSequence.IsInitialized())
		{
			m_ActivityToSequence.Initialize(this);
		}
#endif
		return m_ActivityToSequence.SelectWeightedSequence(this, activity, curSequence);
	}

	inline int SelectWeightedSequenceFromModifiers(int activity, CUtlSymbol* pActivityModifiers, int iModifierCount)
	{
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		if (!m_ActivityToSequence.IsInitialized())
		{
			m_ActivityToSequence.Initialize(this);
		}
#endif
		return m_ActivityToSequence.SelectWeightedSequenceFromModifiers(this, activity, pActivityModifiers, iModifierCount);
	}

	inline bool HaveSequenceForActivity(int activity)
	{
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		if (!m_ActivityToSequence.IsInitialized())
		{
			m_ActivityToSequence.Initialize(this);
		}
#endif
		return (m_ActivityToSequence.NumSequencesForActivity(activity) > 0);
	}

	inline void ReinitializeSequenceMapping(void)
	{
		m_ActivityToSequence.Reinitialize(this);
	}

#ifdef STUDIO_ENABLE_PERF_COUNTERS
public:
	inline void			ClearPerfCounters(void)
	{
		m_nPerfAnimatedBones = 0;
		m_nPerfUsedBones = 0;
		m_nPerfAnimationLayers = 0;
	};

	mutable	int			m_nPerfAnimatedBones;
	mutable	int			m_nPerfUsedBones;
	mutable	int			m_nPerfAnimationLayers;
#endif


};

struct flexweight_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					key;
	float				weight;
	float				influence;
};

struct flexsetting_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					nameindex;

	inline char* pszName(void) const
	{
		return (char*)(((byte*)this) + nameindex);
	}

	int					obsolete1;

	int					numsettings;
	int					index;

	int					obsolete2;

	int					settingindex;

	inline int psetting(byte* base, int i, flexweight_t** weights) const;
};


struct flexsettinghdr_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					id;
	int					version;

	inline const char* pszName(void) const { return name; }
	char				name[64];
	int					length;

	int					numflexsettings;
	int					flexsettingindex;
	inline flexsetting_t* pSetting(int i) const { return (flexsetting_t*)(((byte*)this) + flexsettingindex) + i; };
	int					nameindex;

	int					numindexes;
	int					indexindex;

	inline flexsetting_t* pIndexedSetting(int index) const
	{
		if (index < 0 || index >= numindexes)
		{
			return NULL;
		}

		int i = *((int*)(((byte*)this) + indexindex) + index);

		if (i == -1)
		{
			return NULL;
		}

		return pSetting(i);
	}

	int					numkeys;
	int					keynameindex;
	inline char* pLocalName(int i) const { return (char*)(((byte*)this) + *((int*)(((byte*)this) + keynameindex) + i)); };

	int					keymappingindex;
	inline int* pLocalToGlobal(int i) const { return (int*)(((byte*)this) + keymappingindex) + i; };
	inline int			LocalToGlobal(int i) const { return *pLocalToGlobal(i); };
};

inline int flexsetting_t::psetting(byte* base, int i, flexweight_t** weights) const
{
	*weights = (flexweight_t*)(((byte*)this) + settingindex) + i;
	return numsettings;
};


inline int mstudioflexcontrollerui_t::controllerIndex(const CStudioHdr& cStudioHdr) const
{
	return !stereo ? pController() - cStudioHdr.pFlexcontroller((LocalFlexController_t)0) : -1;
}


inline int mstudioflexcontrollerui_t::rightIndex(const CStudioHdr& cStudioHdr) const
{
	return stereo ? pRightController() - cStudioHdr.pFlexcontroller((LocalFlexController_t)0) : -1;
}


inline int mstudioflexcontrollerui_t::leftIndex(const CStudioHdr& cStudioHdr) const
{
	return stereo ? pLeftController() - cStudioHdr.pFlexcontroller((LocalFlexController_t)0) : -1;
}


inline int mstudioflexcontrollerui_t::nWayValueIndex(const CStudioHdr& cStudioHdr) const
{
	return remaptype == FLEXCONTROLLER_REMAP_NWAY ? pNWayValueController() - cStudioHdr.pFlexcontroller((LocalFlexController_t)0) : -1;
}


inline const mstudioflexcontroller_t* mstudioflexcontrollerui_t::pController(int index) const
{
	if (index < 0 || index > Count())
		return NULL;

	if (remaptype == FLEXCONTROLLER_REMAP_NWAY)
	{
		if (stereo)
			return (mstudioflexcontroller_t*)((char*)this) + *(&szindex0 + index);

		if (index == 0)
			return pController();

		if (index == 1)
			return pNWayValueController();

		return NULL;
	}

	if (index > 1)
		return NULL;

	if (stereo)
		return (mstudioflexcontroller_t*)((char*)this) + *(&szindex0 + index);

	if (index > 0)
		return NULL;

	return pController();
}


#define STUDIO_CONST	1	  
#define STUDIO_FETCH1	2	   
#define STUDIO_FETCH2	3	   
#define STUDIO_ADD		4
#define STUDIO_SUB		5
#define STUDIO_MUL		6
#define STUDIO_DIV		7
#define STUDIO_NEG		8	  
#define STUDIO_EXP		9	  
#define STUDIO_OPEN		10	     
#define STUDIO_CLOSE	11
#define STUDIO_COMMA	12	     
#define STUDIO_MAX		13
#define STUDIO_MIN		14
#define STUDIO_2WAY_0	15	                  
#define STUDIO_2WAY_1	16	                  
#define STUDIO_NWAY		17	                  
#define STUDIO_COMBO	18	             
#define STUDIO_DOMINATE	19	     
#define STUDIO_DME_LOWER_EYELID 20	 
#define STUDIO_DME_UPPER_EYELID 21	 

#define STUDIO_X		0x00000001
#define STUDIO_Y		0x00000002	
#define STUDIO_Z		0x00000004
#define STUDIO_XR		0x00000008
#define STUDIO_YR		0x00000010
#define STUDIO_ZR		0x00000020

#define STUDIO_LX		0x00000040
#define STUDIO_LY		0x00000080
#define STUDIO_LZ		0x00000100
#define STUDIO_LXR		0x00000200
#define STUDIO_LYR		0x00000400
#define STUDIO_LZR		0x00000800

#define STUDIO_LINEAR	0x00001000

#define STUDIO_TYPES	0x0003FFFF
#define STUDIO_RLOOP	0x00040000	     

#define STUDIO_LOOPING	0x0001		          
#define STUDIO_SNAP		0x0002		         
#define STUDIO_DELTA	0x0004		          
#define STUDIO_AUTOPLAY	0x0008		         
#define STUDIO_POST		0x0010		 
#define STUDIO_ALLZEROS	0x0020		       
#define STUDIO_CYCLEPOSE 0x0080		         
#define STUDIO_REALTIME	0x0100		             
#define STUDIO_LOCAL	0x0200		      
#define STUDIO_HIDDEN	0x0400		      
#define STUDIO_OVERRIDE	0x0800		     
#define STUDIO_ACTIVITY	0x1000		        
#define STUDIO_EVENT	0x2000		        
#define STUDIO_WORLD	0x4000		    
#define STUDIO_AL_POST		0x0010		 
#define STUDIO_AL_SPLINE	0x0040		           
#define STUDIO_AL_XFADE		0x0080		                   
#define STUDIO_AL_NOBLEND	0x0200		       
#define STUDIO_AL_LOCAL		0x1000		      
#define STUDIO_AL_POSE		0x4000		          


inline bool Studio_ConvertStudioHdrToNewVersion(studiohdr_t* pStudioHdr)
{
	COMPILE_TIME_ASSERT(STUDIO_VERSION == 48);              

	int version = pStudioHdr->version;
	if (version == STUDIO_VERSION)
		return true;

	bool bResult = true;
	if (version < 46)
	{
		for (int i = 0; i < pStudioHdr->numlocalanim; i++)
		{
			mstudioanimdesc_t* pAnim = (mstudioanimdesc_t*)pStudioHdr->pLocalAnimdesc(i);

			if (pAnim->sectionframes != 0)
			{
				memset(&(pAnim->numframes), 0, (byte*)(pAnim + 1) - (byte*)&(pAnim->numframes));

				pAnim->numframes = 1;
				pAnim->animblock = -1;    
				bResult = false;
			}
		}
	}

	if (version < 47)
	{
		if (pStudioHdr->unused4 != 0)
		{
			pStudioHdr->unused4 = 0;
			bResult = false;
		}
		for (int i = 0; i < pStudioHdr->numlocalanim; i++)
		{
			mstudioanimdesc_t* pAnim = (mstudioanimdesc_t*)pStudioHdr->pLocalAnimdesc(i);
			pAnim->zeroframeindex = 0;
			pAnim->zeroframespan = 0;
		}
	}
	else if (version == 47)
	{
		for (int i = 0; i < pStudioHdr->numlocalanim; i++)
		{
			mstudioanimdesc_t* pAnim = (mstudioanimdesc_t*)pStudioHdr->pLocalAnimdesc(i);
			if (pAnim->zeroframeindex != 0)
			{
				pAnim->zeroframeindex = 0;
				pAnim->zeroframespan = 0;
				bResult = false;
			}
		}
	}

	pStudioHdr->version = STUDIO_VERSION;

	return bResult;
}

inline void Studio_SetRootLOD(studiohdr_t* pStudioHdr, int rootLOD)
{
	if (pStudioHdr->numAllowedRootLODs > 0 &&
		rootLOD >= pStudioHdr->numAllowedRootLODs)
	{
		rootLOD = pStudioHdr->numAllowedRootLODs - 1;
	}

	Assert(rootLOD >= 0 && rootLOD < MAX_NUM_LODS);
	Clamp(rootLOD, 0, MAX_NUM_LODS - 1);

	int vertexindex = 0;
	int tangentsindex = 0;
	int bodyPartID;
	for (bodyPartID = 0; bodyPartID < pStudioHdr->numbodyparts; bodyPartID++)
	{
		mstudiobodyparts_t* pBodyPart = pStudioHdr->pBodypart(bodyPartID);
		int modelID;
		for (modelID = 0; modelID < pBodyPart->nummodels; modelID++)
		{
			mstudiomodel_t* pModel = pBodyPart->pModel(modelID);
			int totalMeshVertexes = 0;
			int meshID;
			for (meshID = 0; meshID < pModel->nummeshes; meshID++)
			{
				mstudiomesh_t* pMesh = pModel->pMesh(meshID);

				pMesh->numvertices = pMesh->vertexdata.numLODVertexes[rootLOD];
				pMesh->vertexoffset = totalMeshVertexes;
				totalMeshVertexes += pMesh->numvertices;
			}

			pModel->numvertices = totalMeshVertexes;
			pModel->vertexindex = vertexindex;
			pModel->tangentsindex = tangentsindex;

			vertexindex += totalMeshVertexes * sizeof(mstudiovertex_t);
			tangentsindex += totalMeshVertexes * sizeof(Vector4D);
		}
	}

	pStudioHdr->rootLOD = rootLOD;
}

inline int Studio_VertexDataSize(const vertexFileHeader_t* pVvdHdr, int rootLOD, bool bNeedsTangentS)
{
	int numVertexes = pVvdHdr->numLODVertexes[rootLOD] + 1;
	int dataLength = pVvdHdr->vertexDataStart + numVertexes * sizeof(mstudiovertex_t);
	if (bNeedsTangentS)
	{
		dataLength += numVertexes * sizeof(Vector4D);
	}

	return dataLength;
}

inline int Studio_LoadVertexes(const vertexFileHeader_t* pTempVvdHdr, vertexFileHeader_t* pNewVvdHdr, int rootLOD, bool bNeedsTangentS)
{
	int					i;
	int					target;
	int					numVertexes;
	vertexFileFixup_t* pFixupTable;

	numVertexes = pTempVvdHdr->numLODVertexes[rootLOD];

	memcpy((void*)pNewVvdHdr, (void*)pTempVvdHdr, pTempVvdHdr->vertexDataStart);

	for (i = 0; i < rootLOD; i++)
	{
		pNewVvdHdr->numLODVertexes[i] = pNewVvdHdr->numLODVertexes[rootLOD];
	}

	if (bNeedsTangentS)
	{
		pNewVvdHdr->tangentDataStart = pNewVvdHdr->vertexDataStart + numVertexes * sizeof(mstudiovertex_t);
	}
	else
	{
		pNewVvdHdr->tangentDataStart = 0;
	}

	if (!pNewVvdHdr->numFixups)
	{
		memcpy(
			(byte*)pNewVvdHdr + pNewVvdHdr->vertexDataStart,
			(byte*)pTempVvdHdr + pTempVvdHdr->vertexDataStart,
			numVertexes * sizeof(mstudiovertex_t));

		if (bNeedsTangentS)
		{
			memcpy(
				(byte*)pNewVvdHdr + pNewVvdHdr->tangentDataStart,
				(byte*)pTempVvdHdr + pTempVvdHdr->tangentDataStart,
				numVertexes * sizeof(Vector4D));
		}

		return numVertexes;
	}

	target = 0;
	pFixupTable = (vertexFileFixup_t*)((byte*)pTempVvdHdr + pTempVvdHdr->fixupTableStart);
	for (i = 0; i < pTempVvdHdr->numFixups; i++)
	{
		if (pFixupTable[i].lod < rootLOD)
		{
			continue;
		}

		memcpy(
			(mstudiovertex_t*)((byte*)pNewVvdHdr + pNewVvdHdr->vertexDataStart) + target,
			(mstudiovertex_t*)((byte*)pTempVvdHdr + pTempVvdHdr->vertexDataStart) + pFixupTable[i].sourceVertexID,
			pFixupTable[i].numVertexes * sizeof(mstudiovertex_t));

		if (bNeedsTangentS)
		{
			memcpy(
				(Vector4D*)((byte*)pNewVvdHdr + pNewVvdHdr->tangentDataStart) + target,
				(Vector4D*)((byte*)pTempVvdHdr + pTempVvdHdr->tangentDataStart) + pFixupTable[i].sourceVertexID,
				pFixupTable[i].numVertexes * sizeof(Vector4D));
		}

		target += pFixupTable[i].numVertexes;
	}

	pNewVvdHdr->numFixups = 0;

	return target;
}

#endif  