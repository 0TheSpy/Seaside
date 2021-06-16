#ifndef VTF_H
#define VTF_H

#ifdef _WIN32
#pragma once
#endif

#include "imageformat.h"
#include "platform.h"

#ifndef VTF_FILE_FORMAT_ONLY

class CUtlBuffer;
class Vector;
struct Rect_t;
class IFileSystem;

enum CompiledVtfFlags
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_SRGB = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,

	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,

	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,

	TEXTUREFLAGS_STAGING_MEMORY = 0x00080000,
	TEXTUREFLAGS_IMMEDIATE_CLEANUP = 0x00100000,
	TEXTUREFLAGS_IGNORE_PICMIP = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,

	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,

	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,

	TEXTUREFLAGS_CLAMPU = 0x02000000,

	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,					     

	TEXTUREFLAGS_SSBUMP = 0x08000000,

	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,

	TEXTUREFLAGS_BORDER = 0x20000000,

	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000,
};

enum VersionedVtfFlags
{
	VERSIONED_VTF_FLAGS_MASK_7_3 = ~0xD1780400,	           
};


struct VtfProcessingOptions
{
	uint32 cbSize;					     

	enum Flags0
	{
		OPT_DECAY_R = 0x00000001,	  
		OPT_DECAY_G = 0x00000002,	  
		OPT_DECAY_B = 0x00000004,	  
		OPT_DECAY_A = 0x00000008,	  

		OPT_DECAY_EXP_R = 0x00000010,	      
		OPT_DECAY_EXP_G = 0x00000020,	      
		OPT_DECAY_EXP_B = 0x00000040,	      
		OPT_DECAY_EXP_A = 0x00000080,	      

		OPT_NOCOMPRESS = 0x00000100,	    
		OPT_NORMAL_DUDV = 0x00000200,	    
		OPT_FILTER_NICE = 0x00000400,	   

		OPT_SET_ALPHA_ONEOVERMIP = 0x00001000,	    
		OPT_PREMULT_COLOR_ONEOVERMIP = 0x00002000,	   
		OPT_MIP_ALPHATEST = 0x00004000,	   
	};

	uint32 flags0;					    

	uint8 clrDecayGoal[4];			       
	uint8 numNotDecayMips[4];		            
	float fDecayExponentBase[4];	        
};


enum CubeMapFaceIndex_t
{
	CUBEMAP_FACE_RIGHT = 0,
	CUBEMAP_FACE_LEFT,
	CUBEMAP_FACE_BACK,	        
	CUBEMAP_FACE_FRONT,	        
	CUBEMAP_FACE_UP,
	CUBEMAP_FACE_DOWN,

	CUBEMAP_FACE_SPHEREMAP,

	CUBEMAP_FACE_COUNT
};


enum LookDir_t
{
	LOOK_DOWN_X = 0,
	LOOK_DOWN_NEGX,
	LOOK_DOWN_Y,
	LOOK_DOWN_NEGY,
	LOOK_DOWN_Z,
	LOOK_DOWN_NEGZ,
};


#define IMAGE_FORMAT_DEFAULT	((ImageFormat)-2)

class IVTFTexture
{
public:
	virtual ~IVTFTexture() {}

	virtual bool Init(int nWidth, int nHeight, int nDepth, ImageFormat fmt, int nFlags, int iFrameCount, int nForceMipCount = -1) = 0;

	virtual void SetBumpScale(float flScale) = 0;
	virtual void SetReflectivity(const Vector& vecReflectivity) = 0;

	virtual void InitLowResImage(int nWidth, int nHeight, ImageFormat fmt) = 0;

	virtual void* SetResourceData(uint32 eType, void const* pData, size_t nDataSize) = 0;

	virtual void* GetResourceData(uint32 eType, size_t* pDataSize) const = 0;

	virtual bool HasResourceEntry(uint32 eType) const = 0;

	virtual unsigned int GetResourceTypes(uint32* arrTypesBuffer, int numTypesBufferElems) const = 0;

	virtual bool Unserialize(CUtlBuffer& buf, bool bHeaderOnly = false, int nSkipMipLevels = 0) = 0;
	virtual bool Serialize(CUtlBuffer& buf) = 0;

	virtual void LowResFileInfo(int* pStartLocation, int* pSizeInBytes) const = 0;
	virtual void ImageFileInfo(int nFrame, int nFace, int nMip, int* pStartLocation, int* pSizeInBytes) const = 0;
	virtual int FileSize(int nMipSkipCount = 0) const = 0;

	virtual int Width() const = 0;
	virtual int Height() const = 0;
	virtual int Depth() const = 0;
	virtual int MipCount() const = 0;

	virtual int RowSizeInBytes(int nMipLevel) const = 0;

	virtual int FaceSizeInBytes(int nMipLevel) const = 0;

	virtual ImageFormat Format() const = 0;
	virtual int FaceCount() const = 0;
	virtual int FrameCount() const = 0;
	virtual int Flags() const = 0;

	virtual float BumpScale() const = 0;

	virtual int LowResWidth() const = 0;
	virtual int LowResHeight() const = 0;
	virtual ImageFormat LowResFormat() const = 0;

	virtual const Vector& Reflectivity() const = 0;

	virtual bool IsCubeMap() const = 0;
	virtual bool IsNormalMap() const = 0;
	virtual bool IsVolumeTexture() const = 0;

	virtual void ComputeMipLevelDimensions(int iMipLevel, int* pMipWidth, int* pMipHeight, int* pMipDepth) const = 0;

	virtual int ComputeMipSize(int iMipLevel) const = 0;

	virtual void ComputeMipLevelSubRect(Rect_t* pSrcRect, int nMipLevel, Rect_t* pSubRect) const = 0;

	virtual int ComputeFaceSize(int iStartingMipLevel = 0) const = 0;

	virtual int ComputeTotalSize() const = 0;

	virtual unsigned char* ImageData() = 0;

	virtual unsigned char* ImageData(int iFrame, int iFace, int iMipLevel) = 0;

	virtual unsigned char* ImageData(int iFrame, int iFace, int iMipLevel, int x, int y, int z = 0) = 0;

	virtual unsigned char* LowResImageData() = 0;

	virtual	void ConvertImageFormat(ImageFormat fmt, bool bNormalToDUDV) = 0;

	virtual void GenerateSpheremap(LookDir_t lookDir = LOOK_DOWN_Z) = 0;

	virtual void GenerateHemisphereMap(unsigned char* pSphereMapBitsRGBA, int targetWidth,
		int targetHeight, LookDir_t lookDir, int iFrame) = 0;

	virtual void FixCubemapFaceOrientation() = 0;

	virtual void GenerateMipmaps() = 0;

	virtual void PutOneOverMipLevelInAlpha() = 0;

	virtual void ComputeReflectivity() = 0;

	virtual void ComputeAlphaFlags() = 0;

	virtual bool ConstructLowResImage() = 0;

	virtual void PostProcess(bool bGenerateSpheremap, LookDir_t lookDir = LOOK_DOWN_Z, bool bAllowFixCubemapOrientation = true) = 0;

	virtual void MatchCubeMapBorders(int iStage, ImageFormat finalFormat, bool bSkybox) = 0;

	virtual void SetAlphaTestThreshholds(float flBase, float flHighFreq) = 0;

#if defined( _X360 )
	virtual int UpdateOrCreate(const char* pFilename, const char* pPathID = NULL, bool bForce = false) = 0;
	virtual bool UnserializeFromBuffer(CUtlBuffer& buf, bool bBufferIsVolatile, bool bHeaderOnly, bool bPreloadOnly, int nMipSkipCount) = 0;
	virtual int FileSize(bool bPreloadOnly, int nMipSkipCount) const = 0;
	virtual int MappingWidth() const = 0;
	virtual int MappingHeight() const = 0;
	virtual int MappingDepth() const = 0;
	virtual int MipSkipCount() const = 0;
	virtual bool IsPreTiled() const = 0;
	virtual unsigned char* LowResImageSample() = 0;
	virtual void ReleaseImageMemory() = 0;
#endif

	virtual void SetPostProcessingSettings(VtfProcessingOptions const* pOptions) = 0;
};

IVTFTexture* CreateVTFTexture();
void DestroyVTFTexture(IVTFTexture* pTexture);

int VTFFileHeaderSize(int nMajorVersion = -1, int nMinorVersion = -1);

typedef bool (*CompressFunc_t)(CUtlBuffer& inputBuffer, CUtlBuffer& outputBuffer);
bool ConvertVTFTo360Format(const char* pDebugName, CUtlBuffer& sourceBuf, CUtlBuffer& targetBuf, CompressFunc_t pCompressFunc);

bool GetVTFPreload360Data(const char* pDebugName, CUtlBuffer& fileBufferIn, CUtlBuffer& preloadBufferOut);

#include "vector.h"

#endif  


#include "datamap.h"

#pragma pack(1)

#define VTF_MAJOR_VERSION 7
#define VTF_MINOR_VERSION 4

struct VTFFileBaseHeader_t
{
	DECLARE_BYTESWAP_DATADESC();
	char fileTypeString[4];     
	int version[2]; 		 
	int headerSize;
};

struct VTFFileHeaderV7_1_t : public VTFFileBaseHeader_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short	width;
	unsigned short	height;
	unsigned int	flags;
	unsigned short	numFrames;
	unsigned short	startFrame;
#if !defined( POSIX ) && !defined( _X360 )
	VectorAligned	reflectivity;
#else
	char			pad1[4];
	Vector			reflectivity;
	char			pad2[4];
#endif
	float			bumpScale;
	ImageFormat		imageFormat;
	unsigned char	numMipLevels;
	ImageFormat		lowResImageFormat;
	unsigned char	lowResImageWidth;
	unsigned char	lowResImageHeight;
};

struct VTFFileHeaderV7_2_t : public VTFFileHeaderV7_1_t
{
	DECLARE_BYTESWAP_DATADESC();

	unsigned short depth;
};

#define BYTE_POS( byteVal, shft )	uint32( uint32(uint8(byteVal)) << uint8(shft * 8) )
#if !defined( _X360 )
#define MK_VTF_RSRC_ID(a, b, c)		uint32( BYTE_POS(a, 0) | BYTE_POS(b, 1) | BYTE_POS(c, 2) )
#define MK_VTF_RSRCF(d)				BYTE_POS(d, 3)
#else
#define MK_VTF_RSRC_ID(a, b, c)		uint32( BYTE_POS(a, 3) | BYTE_POS(b, 2) | BYTE_POS(c, 1) )
#define MK_VTF_RSRCF(d)				BYTE_POS(d, 0)
#endif

enum ResourceEntryType
{
	VTF_LEGACY_RSRC_LOW_RES_IMAGE = MK_VTF_RSRC_ID(0x01, 0, 0),	   
	VTF_LEGACY_RSRC_IMAGE = MK_VTF_RSRC_ID(0x30, 0, 0),	  

	VTF_RSRC_SHEET = MK_VTF_RSRC_ID(0x10, 0, 0),			  
};

enum ResourceEntryTypeFlag
{
	RSRCF_HAS_NO_DATA_CHUNK = MK_VTF_RSRCF(0x02),	       
	RSRCF_MASK = MK_VTF_RSRCF(0xFF)	     
};

enum HeaderDetails
{
	MAX_RSRC_DICTIONARY_ENTRIES = 32,		      
	MAX_X360_RSRC_DICTIONARY_ENTRIES = 4,	          
};

struct ResourceEntryInfo
{
	union
	{
		unsigned int	eType;		          
		unsigned char	chTypeBytes[4];
	};
	unsigned int		resData;	          
};

struct VTFFileHeaderV7_3_t : public VTFFileHeaderV7_2_t
{
	DECLARE_BYTESWAP_DATADESC();

	char			pad4[3];
	unsigned int	numResources;

#if defined( _X360 ) || defined( POSIX )
	char			pad5[8];
#endif

};

struct VTFFileHeader_t : public VTFFileHeaderV7_3_t
{
	DECLARE_BYTESWAP_DATADESC();
};

#define VTF_X360_MAJOR_VERSION	0x0360
#define VTF_X360_MINOR_VERSION	8
struct VTFFileHeaderX360_t : public VTFFileBaseHeader_t
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned int	flags;
	unsigned short	width;					      
	unsigned short	height;					      
	unsigned short	depth;					      
	unsigned short	numFrames;
	unsigned short	preloadDataSize;		         
	unsigned char	mipSkipCount;			     
	unsigned char	numResources;
	Vector			reflectivity;			     
	float			bumpScale;
	ImageFormat		imageFormat;
	unsigned char	lowResImageSample[4];
	unsigned int	compressedSize;

};

#define VTF_RSRC_TEXTURE_LOD_SETTINGS ( MK_VTF_RSRC_ID( 'L','O','D' ) )
struct TextureLODControlSettings_t
{
	uint8 m_ResolutionClampX;
	uint8 m_ResolutionClampY;

	uint8 m_ResolutionClampX_360;
	uint8 m_ResolutionClampY_360;
};

#define VTF_RSRC_TEXTURE_SETTINGS_EX ( MK_VTF_RSRC_ID( 'T','S','0' ) )
struct TextureSettingsEx_t
{
	enum Flags0			   
	{
		UNUSED = 0x01,
	};

	uint8 m_flags0;		     
	uint8 m_flags1;		      
	uint8 m_flags2;		      
	uint8 m_flags3;		      
};

#define VTF_RSRC_TEXTURE_CRC ( MK_VTF_RSRC_ID( 'C','R','C' ) )

#pragma pack()

#endif  