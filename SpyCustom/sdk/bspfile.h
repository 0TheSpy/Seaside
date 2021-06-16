#ifndef BSPFILE_H
#define BSPFILE_H
#pragma once

#ifndef MATHLIB_H
#include "mathlib.h"
#endif

#include "vector4d.h"
#include "datamap.h"
#include "bumpvects.h"
#include "compressed_light_cube.h"

#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'V')		

#define MINBSPVERSION 19
#define BSPVERSION 21


#define MAX_BRUSH_LIGHTMAP_DIM_WITHOUT_BORDER 32
#define MAX_BRUSH_LIGHTMAP_DIM_INCLUDING_BORDER	35

#define MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER	125
#define MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER	128


#define MAX_LIGHTMAP_DIM_WITHOUT_BORDER		MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER
#define MAX_LIGHTMAP_DIM_INCLUDING_BORDER	MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER

#define	MAX_LIGHTSTYLES	64


#define MIN_MAP_DISP_POWER		2	        
#define MAX_MAP_DISP_POWER		4	

#define MAX_DISP_CORNER_NEIGHBORS	4

#define NUM_DISP_POWER_VERTS(power)	( ((1 << (power)) + 1) * ((1 << (power)) + 1) )
#define NUM_DISP_POWER_TRIS(power)	( (1 << (power)) * (1 << (power)) * 2 )

#if !defined( BSP_USE_LESS_MEMORY )
#define	MAX_MAP_MODELS					1024
#define	MAX_MAP_BRUSHES					8192
#define	MAX_MAP_ENTITIES				16384
#define	MAX_MAP_TEXINFO					12288
#define MAX_MAP_TEXDATA					2048
#define MAX_MAP_DISPINFO				2048
#define MAX_MAP_DISP_VERTS				( MAX_MAP_DISPINFO * ((1<<MAX_MAP_DISP_POWER)+1) * ((1<<MAX_MAP_DISP_POWER)+1) )
#define MAX_MAP_DISP_TRIS				( (1 << MAX_MAP_DISP_POWER) * (1 << MAX_MAP_DISP_POWER) * 2 )
#define MAX_DISPVERTS					NUM_DISP_POWER_VERTS( MAX_MAP_DISP_POWER )
#define MAX_DISPTRIS					NUM_DISP_POWER_TRIS( MAX_MAP_DISP_POWER )
#define	MAX_MAP_AREAS					256
#define MAX_MAP_AREA_BYTES				(MAX_MAP_AREAS/8)
#define	MAX_MAP_AREAPORTALS				1024
#define	MAX_MAP_PLANES					65536
#define	MAX_MAP_NODES					65536
#define	MAX_MAP_BRUSHSIDES				65536
#define	MAX_MAP_LEAFS					65536
#define	MAX_MAP_VERTS					65536
#define MAX_MAP_VERTNORMALS				256000
#define MAX_MAP_VERTNORMALINDICES		256000
#define	MAX_MAP_FACES					65536
#define	MAX_MAP_LEAFFACES				65536
#define	MAX_MAP_LEAFBRUSHES 			65536
#define	MAX_MAP_PORTALS					65536
#define MAX_MAP_CLUSTERS				65536
#define MAX_MAP_LEAFWATERDATA			32768
#define MAX_MAP_PORTALVERTS				128000
#define	MAX_MAP_EDGES					256000
#define	MAX_MAP_SURFEDGES				512000
#define	MAX_MAP_LIGHTING				0x1000000
#define	MAX_MAP_VISIBILITY				0x1000000			   
#define	MAX_MAP_TEXTURES				1024
#define MAX_MAP_WORLDLIGHTS				8192
#define MAX_MAP_CUBEMAPSAMPLES			1024
#define MAX_MAP_OVERLAYS				512 
#define MAX_MAP_WATEROVERLAYS			16384
#define MAX_MAP_TEXDATA_STRING_DATA		256000
#define MAX_MAP_TEXDATA_STRING_TABLE	65536
#define MAX_MAP_PRIMITIVES				32768
#define MAX_MAP_PRIMVERTS				65536
#define MAX_MAP_PRIMINDICES				65536

#else

#define	MAX_MAP_MODELS					2
#define	MAX_MAP_BRUSHES					2
#define	MAX_MAP_ENTITIES				2
#define	MAX_MAP_TEXINFO					2
#define MAX_MAP_TEXDATA					2
#define MAX_MAP_DISPINFO				2
#define MAX_MAP_DISP_VERTS				( MAX_MAP_DISPINFO * ((1<<MAX_MAP_DISP_POWER)+1) * ((1<<MAX_MAP_DISP_POWER)+1) )
#define MAX_MAP_DISP_TRIS				( (1 << MAX_MAP_DISP_POWER) * (1 << MAX_MAP_DISP_POWER) * 2 )
#define MAX_DISPVERTS					NUM_DISP_POWER_VERTS( MAX_MAP_DISP_POWER )
#define MAX_DISPTRIS					NUM_DISP_POWER_TRIS( MAX_MAP_DISP_POWER )
#define	MAX_MAP_AREAS					2
#define MAX_MAP_AREA_BYTES				2
#define	MAX_MAP_AREAPORTALS				2
#define	MAX_MAP_PLANES					2
#define	MAX_MAP_NODES					2
#define	MAX_MAP_BRUSHSIDES				2
#define	MAX_MAP_LEAFS					2
#define	MAX_MAP_VERTS					2
#define MAX_MAP_VERTNORMALS				2
#define MAX_MAP_VERTNORMALINDICES		2
#define	MAX_MAP_FACES					2
#define	MAX_MAP_LEAFFACES				2
#define	MAX_MAP_LEAFBRUSHES				2
#define	MAX_MAP_PORTALS					2
#define MAX_MAP_CLUSTERS				2
#define MAX_MAP_LEAFWATERDATA			2
#define MAX_MAP_PORTALVERTS				2
#define	MAX_MAP_EDGES					2
#define	MAX_MAP_SURFEDGES				2
#define	MAX_MAP_LIGHTING				2
#define	MAX_MAP_VISIBILITY				2
#define	MAX_MAP_TEXTURES				2
#define MAX_MAP_WORLDLIGHTS				2
#define MAX_MAP_CUBEMAPSAMPLES			2
#define MAX_MAP_OVERLAYS				2 
#define MAX_MAP_WATEROVERLAYS			2
#define MAX_MAP_TEXDATA_STRING_DATA		2
#define MAX_MAP_TEXDATA_STRING_TABLE	2
#define MAX_MAP_PRIMITIVES				2
#define MAX_MAP_PRIMVERTS				2
#define MAX_MAP_PRIMINDICES				2

#endif  

#define	MAX_KEY		32
#define	MAX_VALUE	1024


enum
{
	CHILDNODE_UPPER_RIGHT = 0,
	CHILDNODE_UPPER_LEFT = 1,
	CHILDNODE_LOWER_LEFT = 2,
	CHILDNODE_LOWER_RIGHT = 3
};


enum
{
	CORNER_LOWER_LEFT = 0,
	CORNER_UPPER_LEFT = 1,
	CORNER_UPPER_RIGHT = 2,
	CORNER_LOWER_RIGHT = 3
};


enum
{
	NEIGHBOREDGE_LEFT = 0,
	NEIGHBOREDGE_TOP = 1,
	NEIGHBOREDGE_RIGHT = 2,
	NEIGHBOREDGE_BOTTOM = 3
};


typedef enum
{
	CORNER_TO_CORNER = 0,
	CORNER_TO_MIDPOINT = 1,
	MIDPOINT_TO_CORNER = 2
} NeighborSpan;


typedef enum
{
	ORIENTATION_CCW_0 = 0,
	ORIENTATION_CCW_90 = 1,
	ORIENTATION_CCW_180 = 2,
	ORIENTATION_CCW_270 = 3
} NeighborOrientation;


enum
{
	LUMP_ENTITIES = 0,	 
	LUMP_PLANES = 1,	 
	LUMP_TEXDATA = 2,	 
	LUMP_VERTEXES = 3,	 
	LUMP_VISIBILITY = 4,	 
	LUMP_NODES = 5,	 
	LUMP_TEXINFO = 6,	 
	LUMP_FACES = 7,	 
	LUMP_LIGHTING = 8,	 
	LUMP_OCCLUSION = 9,
	LUMP_LEAFS = 10,	 
	LUMP_FACEIDS = 11,
	LUMP_EDGES = 12,	 
	LUMP_SURFEDGES = 13,	 
	LUMP_MODELS = 14,	 
	LUMP_WORLDLIGHTS = 15,	 
	LUMP_LEAFFACES = 16,	 
	LUMP_LEAFBRUSHES = 17,	 
	LUMP_BRUSHES = 18,	 
	LUMP_BRUSHSIDES = 19,	 
	LUMP_AREAS = 20,	 
	LUMP_AREAPORTALS = 21,	 
	LUMP_PROPCOLLISION = 22,	     
	LUMP_PROPHULLS = 23,	    
	LUMP_PROPHULLVERTS = 24,	    
	LUMP_PROPTRIS = 25,	       

	LUMP_DISPINFO = 26,
	LUMP_ORIGINALFACES = 27,
	LUMP_PHYSDISP = 28,
	LUMP_PHYSCOLLIDE = 29,
	LUMP_VERTNORMALS = 30,
	LUMP_VERTNORMALINDICES = 31,
	LUMP_DISP_LIGHTMAP_ALPHAS = 32,
	LUMP_DISP_VERTS = 33,		 
	LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS = 34,	   
	LUMP_GAME_LUMP = 35,
	LUMP_LEAFWATERDATA = 36,
	LUMP_PRIMITIVES = 37,
	LUMP_PRIMVERTS = 38,
	LUMP_PRIMINDICES = 39,
	LUMP_PAKFILE = 40,
	LUMP_CLIPPORTALVERTS = 41,
	LUMP_CUBEMAPS = 42,
	LUMP_TEXDATA_STRING_DATA = 43,
	LUMP_TEXDATA_STRING_TABLE = 44,
	LUMP_OVERLAYS = 45,
	LUMP_LEAFMINDISTTOWATER = 46,
	LUMP_FACE_MACRO_TEXTURE_INFO = 47,
	LUMP_DISP_TRIS = 48,
	LUMP_PROP_BLOB = 49,	      
	LUMP_WATEROVERLAYS = 50,
	LUMP_LEAF_AMBIENT_INDEX_HDR = 51,	   
	LUMP_LEAF_AMBIENT_INDEX = 52,	   

	LUMP_LIGHTING_HDR = 53,
	LUMP_WORLDLIGHTS_HDR = 54,
	LUMP_LEAF_AMBIENT_LIGHTING_HDR = 55,	           
	LUMP_LEAF_AMBIENT_LIGHTING = 56,	           

	LUMP_XZIPPAKFILE = 57,           
	LUMP_FACES_HDR = 58,	       
	LUMP_MAP_FLAGS = 59,           
	LUMP_OVERLAY_FADES = 60,	    
	LUMP_OVERLAY_SYSTEM_LEVELS = 61,	            
	LUMP_PHYSLEVEL = 62,
	LUMP_DISP_MULTIBLEND = 63,	   
};


enum
{
	LUMP_LIGHTING_VERSION = 1,
	LUMP_FACES_VERSION = 1,
	LUMP_OCCLUSION_VERSION = 2,
	LUMP_LEAFS_VERSION = 1,
	LUMP_LEAF_AMBIENT_LIGHTING_VERSION = 1,
	LUMP_WORLDLIGHTS_VERSION = 1
};


#define	HEADER_LUMPS		64

#include "zip_uncompressed.h"

struct lump_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		fileofs, filelen;
	int		version;		   
	char	fourCC[4];		              
};

struct BSPHeader_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			ident;
	int			m_nVersion;
	lump_t		lumps[HEADER_LUMPS];
	int			mapRevision;				         
};

#define LVLFLAGS_BAKED_STATIC_PROP_LIGHTING_NONHDR 0x00000001	         
#define LVLFLAGS_BAKED_STATIC_PROP_LIGHTING_HDR    0x00000002           

struct dflagslump_t
{
	DECLARE_BYTESWAP_DATADESC();
	uint32 m_LevelFlags;						 
};

struct lumpfileheader_t
{
	int				lumpOffset;
	int				lumpID;
	int				lumpVersion;
	int				lumpLength;
	int				mapRevision;				         
};

struct dgamelumpheader_t
{
	DECLARE_BYTESWAP_DATADESC();
	int lumpCount;

};

typedef int GameLumpId_t;

#define GAMELUMPFLAG_COMPRESSED	0x0001

struct dgamelump_t
{
	DECLARE_BYTESWAP_DATADESC();
	GameLumpId_t	id;
	unsigned short	flags;
	unsigned short	version;
	int				fileofs;
	int				filelen;
};

extern int g_MapRevision;

struct dmodel_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		mins, maxs;
	Vector		origin;					    
	int			headnode;
	int			firstface, numfaces;	         
};

struct dphysmodel_t
{
	DECLARE_BYTESWAP_DATADESC()
	int			modelIndex;
	int			dataSize;
	int			keydataSize;
	int			solidCount;
};


struct dphyslevelpolytope_t
{
	DECLARE_BYTESWAP_DATADESC()

};


struct DiskPhysics2Polytope_t
{
	DECLARE_BYTESWAP_DATADESC();
	int32 offsetPolytope;            
	int32 offsetInertia;
};


struct dphysdisp_t
{
	DECLARE_BYTESWAP_DATADESC()
	unsigned short numDisplacements;
};

struct dprophull_t
{
	DECLARE_BYTESWAP_DATADESC();
	int m_nVertCount;
	int m_nVertStart;
	int m_nSurfaceProp;
	unsigned int m_nContents;
};

struct dprophulltris_t
{
	DECLARE_BYTESWAP_DATADESC();
	int m_nIndexStart;
	int m_nIndexCount;
};

struct dpropcollision_t
{
	DECLARE_BYTESWAP_DATADESC();
	int m_nHullCount;
	int m_nHullStart;
};

struct dvertex_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector	point;
};

struct dplane_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector	normal;
	float	dist;
	int		type;		       
};

#ifndef BSPFLAGS_H
#include "bspflags.h"
#endif

struct dnode_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			planenum;
	int			children[2];	      
	short		mins[3];		   
	short		maxs[3];
	unsigned short	firstface;
	unsigned short	numfaces;	   
	short			area;		            
};

typedef struct texinfo_s
{
	DECLARE_BYTESWAP_DATADESC();
	float		textureVecsTexelsPerWorldUnits[2][4];			  
	float		lightmapVecsLuxelsPerWorldUnits[2][4];			         
	int			flags;				    
	int			texdata;			      
} texinfo_t;

#define TEXTURE_NAME_LENGTH	 128			     

struct dtexdata_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		reflectivity;
	int			nameStringTableID;				       
	int			width, height;					  
	int			view_width, view_height;		
};


enum
{
	OCCLUDER_FLAGS_INACTIVE = 0x1,
};

struct doccluderdata_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			flags;
	int			firstpoly;				   
	int			polycount;
	Vector		mins;
	Vector		maxs;
	int			area;
};

struct doccluderdataV1_t
{
	int			flags;
	int			firstpoly;				   
	int			polycount;
	Vector		mins;
	Vector		maxs;
};

struct doccluderpolydata_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			firstvertexindex;		   
	int			vertexcount;
	int			planenum;
};


struct CDispSubNeighbor
{
public:
	DECLARE_BYTESWAP_DATADESC();
	unsigned short		GetNeighborIndex() const { return m_iNeighbor; }
	NeighborSpan		GetSpan() const { return (NeighborSpan)m_Span; }
	NeighborSpan		GetNeighborSpan() const { return (NeighborSpan)m_NeighborSpan; }
	NeighborOrientation	GetNeighborOrientation() const { return (NeighborOrientation)m_NeighborOrientation; }

	bool				IsValid() const { return m_iNeighbor != 0xFFFF; }
	void				SetInvalid() { m_iNeighbor = 0xFFFF; }


public:
	unsigned short		m_iNeighbor;		    
	unsigned char		m_NeighborOrientation;		        

	unsigned char		m_Span;						          
	unsigned char		m_NeighborSpan;				      
};


class CDispNeighbor
{
public:
	DECLARE_BYTESWAP_DATADESC();
	void				SetInvalid() { m_SubNeighbors[0].SetInvalid(); m_SubNeighbors[1].SetInvalid(); }

	bool				IsValid() { return m_SubNeighbors[0].IsValid() || m_SubNeighbors[1].IsValid(); }


public:
	CDispSubNeighbor	m_SubNeighbors[2];
};


class CDispCornerNeighbors
{
public:
	DECLARE_BYTESWAP_DATADESC();
	void			SetInvalid() { m_nNeighbors = 0; }


public:
	unsigned short	m_Neighbors[MAX_DISP_CORNER_NEIGHBORS];	   
	unsigned char	m_nNeighbors;
};


class CDispVert
{
public:
	DECLARE_BYTESWAP_DATADESC();
	Vector		m_vVector;		     
	float		m_flDist;		  
	float		m_flAlpha;		    
};

#define DISPTRI_TAG_SURFACE			(1<<0)
#define DISPTRI_TAG_WALKABLE		(1<<1)
#define DISPTRI_TAG_BUILDABLE		(1<<2)
#define DISPTRI_FLAG_SURFPROP1		(1<<3)
#define DISPTRI_FLAG_SURFPROP2		(1<<4)

class CDispTri
{
public:
	DECLARE_BYTESWAP_DATADESC();
	unsigned short m_uiTags;		   
};

#define MAX_MULTIBLEND_CHANNELS		4

class CDispMultiBlend
{
public:
	DECLARE_BYTESWAP_DATADESC();

	Vector4D	m_vMultiBlend;
	Vector4D	m_vAlphaBlend;
	Vector		m_vMultiBlendColors[MAX_MULTIBLEND_CHANNELS];
};

#define DISP_INFO_FLAG_HAS_MULTIBLEND	0x40000000
#define DISP_INFO_FLAG_MAGIC			0x80000000

class ddispinfo_t
{
public:
	DECLARE_BYTESWAP_DATADESC();
	int			NumVerts() const { return NUM_DISP_POWER_VERTS(power); }
	int			NumTris() const { return NUM_DISP_POWER_TRIS(power); }

public:
	Vector		startPosition;						         
	int			m_iDispVertStart;					   
	int			m_iDispTriStart;					   

	int         power;                                       
	int         minTess;                               
	float       smoothingAngle;                        
	int         contents;                             

	unsigned short	m_iMapFace;						       

	int			m_iLightmapAlphaStart;				   
	int			m_iLightmapSamplePositionStart;		   

	CDispNeighbor			m_EdgeNeighbors[4];		    
	CDispCornerNeighbors	m_CornerNeighbors[4];	    

	enum unnamed { ALLOWEDVERTS_SIZE = PAD_NUMBER(MAX_DISPVERTS, 32) / 32 };
	unsigned long	m_AllowedVerts[ALLOWEDVERTS_SIZE];	            
};


struct dedge_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short	v[2];		  
};

#define	MAXLIGHTMAPS	4

enum dprimitive_type
{
	PRIM_TRILIST = 0,
	PRIM_TRISTRIP = 1,
};

struct dprimitive_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned char type;
	unsigned short	firstIndex;
	unsigned short	indexCount;
	unsigned short	firstVert;
	unsigned short	vertCount;
};

struct dprimvert_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		pos;
};

struct dface_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short	planenum;
	byte		side;	       
	byte		onNode;         

	int			firstedge;		      
	short		numedges;
	short		texinfo;
	short       dispinfo;
	short		surfaceFogVolumeID;
	byte		styles[MAXLIGHTMAPS];
	int			lightofs;		    
	float       area;

	int			m_LightmapTextureMinsInLuxels[2];
	int			m_LightmapTextureSizeInLuxels[2];

	int origFace;				         


public:

	unsigned short GetNumPrims() const;
	void SetNumPrims(unsigned short nPrims);
	bool AreDynamicShadowsEnabled();
	void SetDynamicShadowsEnabled(bool bEnabled);

private:
	unsigned short m_NumPrims;	               

public:
	unsigned short	firstPrimID;

	unsigned int	smoothingGroups;
};


inline unsigned short dface_t::GetNumPrims() const
{
	return m_NumPrims & 0x7FFF;
}

inline void dface_t::SetNumPrims(unsigned short nPrims)
{
	Assert((nPrims & 0x8000) == 0);
	m_NumPrims &= ~0x7FFF;
	m_NumPrims |= (nPrims & 0x7FFF);
}

inline bool dface_t::AreDynamicShadowsEnabled()
{
	return (m_NumPrims & 0x8000) == 0;
}

inline void dface_t::SetDynamicShadowsEnabled(bool bEnabled)
{
	if (bEnabled)
		m_NumPrims &= ~0x8000;
	else
		m_NumPrims |= 0x8000;
}

struct dfaceid_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short	hammerfaceid;
};


#define LEAF_FLAGS_SKY			0x01		        
#define LEAF_FLAGS_RADIAL		0x02		          
#define LEAF_FLAGS_SKY2D		0x04		        
#define LEAF_FLAGS_CONTAINS_DETAILOBJECTS 0x08				             

#if defined( _X360 )
#pragma bitfield_order( push, lsb_to_msb )
#endif
#ifdef _MSC_VER
#pragma warning( disable:4201 )	      
#endif
struct dleaf_version_0_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				contents;			      

	short			cluster;

	BEGIN_BITFIELD(bf);
	short			area : 9;
	short			flags : 7;			   
	END_BITFIELD();

	short			mins[3];			   
	short			maxs[3];

	unsigned short	firstleafface;
	unsigned short	numleaffaces;

	unsigned short	firstleafbrush;
	unsigned short	numleafbrushes;
	short			leafWaterDataID;      

	CompressedLightCube m_AmbientLighting;
};

struct dleaf_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				contents;			      

	short			cluster;

	BEGIN_BITFIELD(bf);
	short			area : 9;
	short			flags : 7;			   
	END_BITFIELD();

	short			mins[3];			   
	short			maxs[3];

	unsigned short	firstleafface;
	unsigned short	numleaffaces;

	unsigned short	firstleafbrush;
	unsigned short	numleafbrushes;
	short			leafWaterDataID;      

};
#ifdef _MSC_VER
#pragma warning( default:4201 )	      
#endif
#if defined( _X360 )
#pragma bitfield_order( pop )
#endif

struct dleafambientlighting_t
{
	DECLARE_BYTESWAP_DATADESC();
	CompressedLightCube	cube;
	byte x;		      
	byte y;		      
	byte z;		      
	byte pad;	 
};

struct dleafambientindex_t
{
	DECLARE_BYTESWAP_DATADESC();

	unsigned short ambientSampleCount;
	unsigned short firstAmbientSample;
};

struct dbrushside_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short	planenum;		     
	short			texinfo;
	short			dispinfo;		    
	byte			bevel;			        
	byte			thin;			    
};

struct dbrush_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			firstside;
	int			numsides;
	int			contents;
};

#define	ANGLE_UP	-1
#define	ANGLE_DOWN	-2


#define	DVIS_PVS	0
#define	DVIS_PAS	1
struct dvis_t
{
	int			numclusters;
	int			bitofs[8][2];	 
};

struct dareaportal_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short	m_PortalKey;		           
	unsigned short	otherarea;		      

	unsigned short	m_FirstClipPortalVert;	  
	unsigned short	m_nClipPortalVerts;

	int				planenum;
};


struct darea_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		numareaportals;
	int		firstareaportal;
};

struct dleafwaterdata_t
{
	DECLARE_BYTESWAP_DATADESC();
	float	surfaceZ;
	float	minZ;
	short	surfaceTexInfoID;
};

class CFaceMacroTextureInfo
{
public:
	DECLARE_BYTESWAP_DATADESC();
	unsigned short m_MacroTextureNameID;
};

enum emittype_t
{
	emit_surface,		   
	emit_point,			    
	emit_spotlight,		   
	emit_skylight,		           
	emit_quakelight,	   
	emit_skyambient,	            
};


#define DWL_FLAGS_INAMBIENTCUBE		0x0001	            
#define DWL_FLAGS_CASTENTITYSHADOWS	0x0002	          

struct dworldlight_version0_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		origin;
	Vector		intensity;
	Vector		normal;			    
	int			cluster;
	emittype_t	type;
	int			style;
	float		stopdot;		     
	float		stopdot2;		     
	float		exponent;		 
	float		radius;			  
	float		constant_attn;
	float		linear_attn;
	float		quadratic_attn;
	int			flags;			       
	int			texinfo;		 
	int			owner;			       
};

struct dworldlight_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		origin;
	Vector		intensity;
	Vector		normal;			    
	Vector		shadow_cast_offset;	                     
	int			cluster;
	emittype_t	type;
	int			style;
	float		stopdot;		     
	float		stopdot2;		     
	float		exponent;		 
	float		radius;			  
	float		constant_attn;
	float		linear_attn;
	float		quadratic_attn;
	int			flags;			       
	int			texinfo;		 
	int			owner;			       
};

struct dcubemapsample_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			origin[3];			        
	unsigned char size;				   
};

#define OVERLAY_BSP_FACE_COUNT	64

#define OVERLAY_NUM_RENDER_ORDERS		(1<<OVERLAY_RENDER_ORDER_NUM_BITS)
#define OVERLAY_RENDER_ORDER_NUM_BITS	2
#define OVERLAY_RENDER_ORDER_MASK		0xC000	    

struct doverlay_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			nId;
	short		nTexInfo;

	void			SetFaceCount(unsigned short count);
	unsigned short	GetFaceCount() const;

	void			SetRenderOrder(unsigned short order);
	unsigned short	GetRenderOrder() const;

private:
	unsigned short	m_nFaceCountAndRenderOrder;

public:
	int			aFaces[OVERLAY_BSP_FACE_COUNT];
	float		flU[2];
	float		flV[2];
	Vector		vecUVPoints[4];
	Vector		vecOrigin;
	Vector		vecBasisNormal;
};


inline void doverlay_t::SetFaceCount(unsigned short count)
{
	Assert((count & OVERLAY_RENDER_ORDER_MASK) == 0);
	m_nFaceCountAndRenderOrder &= OVERLAY_RENDER_ORDER_MASK;
	m_nFaceCountAndRenderOrder |= (count & ~OVERLAY_RENDER_ORDER_MASK);
}

inline unsigned short doverlay_t::GetFaceCount() const
{
	return m_nFaceCountAndRenderOrder & ~OVERLAY_RENDER_ORDER_MASK;
}

inline void doverlay_t::SetRenderOrder(unsigned short order)
{
	Assert(order < OVERLAY_NUM_RENDER_ORDERS);
	m_nFaceCountAndRenderOrder &= ~OVERLAY_RENDER_ORDER_MASK;
	m_nFaceCountAndRenderOrder |= (order << (16 - OVERLAY_RENDER_ORDER_NUM_BITS));	      
}

inline unsigned short doverlay_t::GetRenderOrder() const
{
	return (m_nFaceCountAndRenderOrder >> (16 - OVERLAY_RENDER_ORDER_NUM_BITS));
}


struct doverlayfade_t
{
	DECLARE_BYTESWAP_DATADESC();

	float flFadeDistMinSq;
	float flFadeDistMaxSq;
};


struct doverlaysystemlevel_t
{
	DECLARE_BYTESWAP_DATADESC();

	unsigned char nMinCPULevel;
	unsigned char nMaxCPULevel;
	unsigned char nMinGPULevel;
	unsigned char nMaxGPULevel;
};


#define WATEROVERLAY_BSP_FACE_COUNT				256
#define WATEROVERLAY_RENDER_ORDER_NUM_BITS		2
#define WATEROVERLAY_NUM_RENDER_ORDERS			(1<<WATEROVERLAY_RENDER_ORDER_NUM_BITS)
#define WATEROVERLAY_RENDER_ORDER_MASK			0xC000	    
struct dwateroverlay_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				nId;
	short			nTexInfo;

	void			SetFaceCount(unsigned short count);
	unsigned short	GetFaceCount() const;
	void			SetRenderOrder(unsigned short order);
	unsigned short	GetRenderOrder() const;

private:

	unsigned short	m_nFaceCountAndRenderOrder;

public:

	int				aFaces[WATEROVERLAY_BSP_FACE_COUNT];
	float			flU[2];
	float			flV[2];
	Vector			vecUVPoints[4];
	Vector			vecOrigin;
	Vector			vecBasisNormal;
};

inline void dwateroverlay_t::SetFaceCount(unsigned short count)
{
	Assert((count & WATEROVERLAY_RENDER_ORDER_MASK) == 0);
	m_nFaceCountAndRenderOrder &= WATEROVERLAY_RENDER_ORDER_MASK;
	m_nFaceCountAndRenderOrder |= (count & ~WATEROVERLAY_RENDER_ORDER_MASK);
}

inline unsigned short dwateroverlay_t::GetFaceCount() const
{
	return m_nFaceCountAndRenderOrder & ~WATEROVERLAY_RENDER_ORDER_MASK;
}

inline void dwateroverlay_t::SetRenderOrder(unsigned short order)
{
	Assert(order < WATEROVERLAY_NUM_RENDER_ORDERS);
	m_nFaceCountAndRenderOrder &= ~WATEROVERLAY_RENDER_ORDER_MASK;
	m_nFaceCountAndRenderOrder |= (order << (16 - WATEROVERLAY_RENDER_ORDER_NUM_BITS));	      
}

inline unsigned short dwateroverlay_t::GetRenderOrder() const
{
	return (m_nFaceCountAndRenderOrder >> (16 - WATEROVERLAY_RENDER_ORDER_NUM_BITS));
}

#ifndef _DEF_BYTE_
#define _DEF_BYTE_
typedef unsigned char	byte;
typedef unsigned short	word;
#endif


#define	ANGLE_UP	-1
#define	ANGLE_DOWN	-2



struct epair_t
{
	epair_t* next;
	char* key;
	char* value;
};

#define MAX_LIGHTMAPPAGE_WIDTH	256
#define MAX_LIGHTMAPPAGE_HEIGHT	128
typedef struct nameForDatadesc_dlightmappage_t        
{
	DECLARE_BYTESWAP_DATADESC();
	byte	data[MAX_LIGHTMAPPAGE_WIDTH * MAX_LIGHTMAPPAGE_HEIGHT];
	byte	palette[256 * 4];
} dlightmappage_t;

typedef struct nameForDatadesc_dlightmappageinfo_t        
{
	DECLARE_BYTESWAP_DATADESC();
	byte			page;			   
	byte			offset[2];		    
	byte			pad;			 
	ColorRGBExp32	avgColor;		      
} dlightmappageinfo_t;

#endif  