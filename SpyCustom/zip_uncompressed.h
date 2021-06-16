#ifndef ZIP_UNCOMPRESSED_H
#define ZIP_UNCOMPRESSED_H
#ifdef _WIN32
#pragma once
#endif

#include "datamap.h"

#define PKID( a, b ) (((b)<<24)|((a)<<16)|('K'<<8)|'P')

#pragma pack(1)

struct ZIP_EndOfCentralDirRecord
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned int	signature;    
	unsigned short	numberOfThisDisk;    
	unsigned short	numberOfTheDiskWithStartOfCentralDirectory;   
	unsigned short	nCentralDirectoryEntries_ThisDisk;	  
	unsigned short	nCentralDirectoryEntries_Total;	  
	unsigned int	centralDirectorySize;   
	unsigned int	startOfCentralDirOffset;   
	unsigned short	commentLength;   
};

struct ZIP_FileHeader
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned int	signature;      
	unsigned short	versionMadeBy;       
	unsigned short	versionNeededToExtract;        
	unsigned short	flags;        
	unsigned short	compressionMethod;      
	unsigned short	lastModifiedTime;        
	unsigned short	lastModifiedDate;        
	unsigned int	crc32;     
	unsigned int	compressedSize;      
	unsigned int	uncompressedSize;      
	unsigned short	fileNameLength;       
	unsigned short	extraFieldLength;       
	unsigned short	fileCommentLength;       
	unsigned short	diskNumberStart;       
	unsigned short	internalFileAttribs;       
	unsigned int	externalFileAttribs;       
	unsigned int	relativeOffsetOfLocalHeader;         
};

struct ZIP_LocalFileHeader
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned int	signature;        
	unsigned short	versionNeededToExtract;        
	unsigned short	flags;        
	unsigned short	compressionMethod;      
	unsigned short	lastModifiedTime;        
	unsigned short	lastModifiedDate;        
	unsigned int	crc32;     
	unsigned int	compressedSize;      
	unsigned int	uncompressedSize;      
	unsigned short	fileNameLength;       
	unsigned short	extraFieldLength;       
};

#define PRELOAD_SECTION_NAME	"__preload_section.pre"
#define PRELOAD_HDR_VERSION		3
#define XZIP_COMMENT_LENGTH		32
#define INVALID_PRELOAD_ENTRY	( (unsigned short)-1 )

struct ZIP_PreloadHeader
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned int Version;					 
	unsigned int DirectoryEntries;			     
	unsigned int PreloadDirectoryEntries;	            
	unsigned int Alignment;					     
};

struct ZIP_PreloadDirectoryEntry
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned int Length;					        
	unsigned int DataOffset;				                
};

struct ZIP_PreloadRemapTable
{
	DECLARE_BYTESWAP_DATADESC();
	unsigned short PreloadIndex;			            
};

#pragma pack()

#endif  