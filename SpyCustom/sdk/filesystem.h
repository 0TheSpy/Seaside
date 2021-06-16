#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#ifdef _WIN32
#pragma once
#endif

#include <limits.h>

#include "threadtools.h"
#include "memalloc.h"
#include "tslist.h"
#include "interface.h"
#include "utlsymbol.h"
#include "utlstring.h"
#include "functors.h" 
#include "checksum_crc.h"
#include "checksum_md5.h"
#include "utlqueue.h"
#include "iappsystem.h"
#include "tier2.h"
#ifdef _PS3
#include <sysutil/sysutil_syscache.h>
#include <sysutil/sysutil_gamecontent.h>
struct HddCacheFileStatus;
extern char gSrcGameDataPath[];
class CFileGroupSystem;
#endif

class CUtlBuffer;
class KeyValues;
class IFileList;

typedef void* FileHandle_t;
typedef int FileFindHandle_t;
typedef void (*FileSystemLoggingFunc_t)(const char* fileName, const char* accessType);
typedef int WaitForResourcesHandle_t;

#ifdef _X360
typedef void* HANDLE;
#endif

#define FILESYSTEM_MAX_SEARCH_PATHS 128

enum FileSystemSeek_t
{
	FILESYSTEM_SEEK_HEAD = SEEK_SET,
	FILESYSTEM_SEEK_CURRENT = SEEK_CUR,
	FILESYSTEM_SEEK_TAIL = SEEK_END,
};

enum
{
	FILESYSTEM_INVALID_FIND_HANDLE = -1
};

enum FileWarningLevel_t
{
	FILESYSTEM_WARNING = -1,

	FILESYSTEM_WARNING_QUIET = 0,

	FILESYSTEM_WARNING_REPORTUNCLOSED,

	FILESYSTEM_WARNING_REPORTUSAGE,

	FILESYSTEM_WARNING_REPORTALLACCESSES,

	FILESYSTEM_WARNING_REPORTALLACCESSES_READ,

	FILESYSTEM_WARNING_REPORTALLACCESSES_READWRITE,

	FILESYSTEM_WARNING_REPORTALLACCESSES_ASYNC,

};

enum PathTypeFilter_t
{
	FILTER_NONE = 0,	       
	FILTER_CULLPACK = 1,	         
	FILTER_CULLNONPACK = 2,	      
	FILTER_CULLLOCALIZED = 3,	     
	FILTER_CULLLOCALIZED_ANY = 4,	    
};

enum
{
	PATH_IS_NORMAL = 0x00,      
	PATH_IS_PACKFILE = 0x01,      
	PATH_IS_MAPPACKFILE = 0x02,       
	PATH_IS_DVDDEV = 0x04,      
};
typedef uint32 PathTypeQuery_t;

#define IS_PACKFILE( n ) ( n & ( PATH_IS_PACKFILE | PATH_IS_MAPPACKFILE ) )
#define IS_DVDDEV( n )   ( n & PATH_IS_DVDDEV )

enum DVDMode_t
{
	DVDMODE_OFF = 0,    
	DVDMODE_STRICT = 1,    
	DVDMODE_DEV = 2,      
	DVDMODE_DEV_VISTA = 3,          
};

#ifdef _PS3

enum FsState_t
{
	FS_STATE_INIT = 0,
	FS_STATE_LEVEL_LOAD = 1,
	FS_STATE_LEVEL_RUN = 2,
	FS_STATE_LEVEL_RESTORE = 3,
	FS_STATE_LEVEL_LOAD_END = 4,
	FS_STATE_EXITING = 5
};

enum Ps3FileType_t
{
	PS3_FILETYPE_WAV,
	PS3_FILETYPE_ANI,
	PS3_FILETYPE_BSP,
	PS3_FILETYPE_VMT,
	PS3_FILETYPE_QPRE,
	PS3_FILETYPE_OTHER,
	PS3_FILETYPE_DIR,
	PS3_FILETYPE_UNKNOWN
};


#endif

#if defined( TRACK_BLOCKING_IO )
enum FileBlockingWarning_t
{
	FILESYSTEM_BLOCKING_SYNCHRONOUS = 0,
	FILESYSTEM_BLOCKING_ASYNCHRONOUS_BLOCK,
	FILESYSTEM_BLOCKING_ASYNCHRONOUS,
	FILESYSTEM_BLOCKING_CALLBACKTIMING,

	FILESYSTEM_BLOCKING_NUMBINS,
};

#pragma pack(1)
class FileBlockingItem
{
public:
	enum
	{
		FB_ACCESS_OPEN = 1,
		FB_ACCESS_CLOSE = 2,
		FB_ACCESS_READ = 3,
		FB_ACCESS_WRITE = 4,
		FB_ACCESS_APPEND = 5,
		FB_ACCESS_SIZE = 6
	};

	FileBlockingItem() :
		m_ItemType((FileBlockingWarning_t)0),
		m_flElapsed(0.0f),
		m_nAccessType(0)
	{
		SetFileName(NULL);
	}

	FileBlockingItem(int type, char const* filename, float elapsed, int accessType) :
		m_ItemType((FileBlockingWarning_t)type),
		m_flElapsed(elapsed),
		m_nAccessType(accessType)
	{
		SetFileName(filename);
	}

	void SetFileName(char const* filename)
	{
		if (!filename)
		{
			m_szFilename[0] = 0;
			return;
		}

		int len = Q_strlen(filename);
		if (len >= sizeof(m_szFilename))
		{
			Q_strncpy(m_szFilename, &filename[len - sizeof(m_szFilename) + 1], sizeof(m_szFilename));
		}
		else
		{
			Q_strncpy(m_szFilename, filename, sizeof(m_szFilename));
		}
	}

	char const* GetFileName() const
	{
		return m_szFilename;
	}

	FileBlockingWarning_t	m_ItemType;
	float					m_flElapsed;
	byte					m_nAccessType;
private:

	char					m_szFilename[32];
};
#pragma pack()

class IBlockingFileItemList
{
public:

	virtual void	LockMutex() = 0;
	virtual void	UnlockMutex() = 0;

	virtual int		First() const = 0;
	virtual int		Next(int i) const = 0;
	virtual int		InvalidIndex() const = 0;

	virtual const	FileBlockingItem& Get(int index) const = 0;

	virtual void	Reset() = 0;
};

#endif  

enum FilesystemMountRetval_t
{
	FILESYSTEM_MOUNT_OK = 0,
	FILESYSTEM_MOUNT_FAILED,
};

enum SearchPathAdd_t
{
	PATH_ADD_TO_HEAD,			   
	PATH_ADD_TO_TAIL,			   
	PATH_ADD_TO_TAIL_ATINDEX,	   
};

enum FilesystemOpenExFlags_t
{
	FSOPEN_UNBUFFERED = (1 << 0),
	FSOPEN_FORCE_TRACK_CRC = (1 << 1),		                 
											FSOPEN_NEVERINPACK = (1 << 2),		               
};

#define FILESYSTEM_INVALID_HANDLE	( FileHandle_t )0

struct FileSystemStatistics
{
	CInterlockedUInt	nReads,
		nWrites,
		nBytesRead,
		nBytesWritten,
		nSeeks;
};

typedef void* (*FSAllocFunc_t)(const char* pszFilename, unsigned nBytes);


typedef void (*FSDirtyDiskReportFunc_t)();


DECLARE_POINTER_HANDLE(FSAsyncControl_t);
DECLARE_POINTER_HANDLE(FSAsyncFile_t);
const FSAsyncFile_t FS_INVALID_ASYNC_FILE = (FSAsyncFile_t)(0x0000ffff);


enum FSAsyncStatus_t
{
	FSASYNC_ERR_ALIGNMENT = -6,	      
	FSASYNC_ERR_FAILURE = -5,	   
	FSASYNC_ERR_READING = -4,	    
	FSASYNC_ERR_NOMEMORY = -3,	      
	FSASYNC_ERR_UNKNOWNID = -2,	      
	FSASYNC_ERR_FILEOPEN = -1,	          
	FSASYNC_OK = 0,	   
	FSASYNC_STATUS_PENDING,			       
	FSASYNC_STATUS_INPROGRESS,		    
	FSASYNC_STATUS_ABORTED,			     
	FSASYNC_STATUS_UNSERVICED,		     
};

enum FSAsyncFlags_t
{
	FSASYNC_FLAGS_ALLOCNOFREE = (1 << 0),	        
	FSASYNC_FLAGS_FREEDATAPTR = (1 << 1),	        
	FSASYNC_FLAGS_SYNC = (1 << 2),	           
	FSASYNC_FLAGS_NULLTERMINATE = (1 << 3),	           
};

enum EFileCRCStatus
{
	k_eFileCRCStatus_CantOpenFile,		      
	k_eFileCRCStatus_GotCRC,
	k_eFileCRCStatus_FileInVPK,
};

enum ECacheCRCType
{
	k_eCacheCRCType_SingleFile,
	k_eCacheCRCType_Directory,
	k_eCacheCRCType_Directory_Recursive
};

struct FileAsyncRequest_t;
typedef void (*FSAsyncCallbackFunc_t)(const FileAsyncRequest_t& request, int nBytesRead, FSAsyncStatus_t err);

typedef void (*FSAsyncScanAddFunc_t)(void* pContext, char* pFoundPath, char* pFoundFile);
typedef void (*FSAsyncScanCompleteFunc_t)(void* pContext, FSAsyncStatus_t err);

struct FileAsyncRequest_t
{
	FileAsyncRequest_t() { memset(this, 0, sizeof(*this)); hSpecificAsyncFile = FS_INVALID_ASYNC_FILE; }
	const char* pszFilename;		   
	void* pData;				      
	int						nOffset;			    
	int						nBytes;				       
	FSAsyncCallbackFunc_t	pfnCallback;		   
	void* pContext;			    
	int						priority;			    
	unsigned				flags;				  
	const char* pszPathID;			                
	FSAsyncFile_t			hSpecificAsyncFile;      
	FSAllocFunc_t			pfnAlloc;			         
};


struct FileHash_t
{
	enum EFileHashType_t
	{
		k_EFileHashTypeUnknown = 0,
		k_EFileHashTypeEntireFile = 1,
		k_EFileHashTypeIncompleteFile = 2,
	};
	FileHash_t()
	{
		m_eFileHashType = FileHash_t::k_EFileHashTypeUnknown;
		m_cbFileLen = 0;
		m_PackFileID = 0;
		m_nPackFileNumber = 0;
	}
	int m_eFileHashType;
	CRC32_t m_crcIOSequence;
	MD5Value_t m_md5contents;
	int m_cbFileLen;
	int m_PackFileID;
	int m_nPackFileNumber;

	bool operator==(const FileHash_t& src) const
	{
		return m_crcIOSequence == src.m_crcIOSequence &&
			m_md5contents == src.m_md5contents &&
			m_eFileHashType == src.m_eFileHashType;
	}
	bool operator!=(const FileHash_t& src) const
	{
		return m_crcIOSequence != src.m_crcIOSequence ||
			m_md5contents != src.m_md5contents ||
			m_eFileHashType != src.m_eFileHashType;
	}

};

class CUnverifiedFileHash
{
public:
	char m_PathID[MAX_PATH];
	char m_Filename[MAX_PATH];
	int m_nFileFraction;
	FileHash_t m_FileHash;
};

class CUnverifiedCRCFile
{
public:
	char m_PathID[MAX_PATH];
	char m_Filename[MAX_PATH];
	CRC32_t m_CRC;
};


#define WHITELIST_SPEW_WHILE_LOADING		0x0001	          
#define WHITELIST_SPEW_RELOAD_FILES			0x0002	          
#define WHITELIST_SPEW_DONT_RELOAD_FILES	0x0004	           



#define DLC_LICENSE_ID( x )				( ( ( (unsigned int)( x ) ) >> 24 ) & 0x000000FF )
#define DLC_LICENSE_MINORVERSION( x )	( ( ( (unsigned int)( x ) ) >> 16 ) & 0x000000FF )
#define DLC_LICENSE_FLAGS( x )			( ( ( (unsigned int)( x ) ) & 0x0000FFFF ) )

#define DLCFLAGS_PRESENCE_ONLY			0x0001	     



#define BASEFILESYSTEM_INTERFACE_VERSION		"VBaseFileSystem011"

abstract_class IThreadedFileMD5Processor
{
public:
	virtual int				SubmitThreadedMD5Request(uint8 * pubBuffer, int cubBuffer, int PackFileID, int nPackFileNumber, int nPackFileFraction) = 0;
	virtual bool			BlockUntilMD5RequestComplete(int iRequest, MD5Value_t* pMd5ValueOut) = 0;
	virtual bool			IsMD5RequestComplete(int iRequest, MD5Value_t* pMd5ValueOut) = 0;
};

abstract_class IBaseFileSystem
{
public:
	virtual int				Read(void* pOutput, int size, FileHandle_t file) = 0;
	virtual int				Write(void const* pInput, int size, FileHandle_t file) = 0;

	virtual FileHandle_t	Open(const char* pFileName, const char* pOptions, const char* pathID = 0) = 0;
	virtual void			Close(FileHandle_t file) = 0;


	virtual void			Seek(FileHandle_t file, int pos, FileSystemSeek_t seekType) = 0;
	virtual unsigned int	Tell(FileHandle_t file) = 0;
	virtual unsigned int	Size(FileHandle_t file) = 0;
	virtual unsigned int	Size(const char* pFileName, const char* pPathID = 0) = 0;

	virtual void			Flush(FileHandle_t file) = 0;
	virtual bool			Precache(const char* pFileName, const char* pPathID = 0) = 0;

	virtual bool			FileExists(const char* pFileName, const char* pPathID = 0) = 0;
	virtual bool			IsFileWritable(char const* pFileName, const char* pPathID = 0) = 0;
	virtual bool			SetFileWritable(char const* pFileName, bool writable, const char* pPathID = 0) = 0;

	virtual long			GetFileTime(const char* pFileName, const char* pPathID = 0) = 0;

	virtual bool			ReadFile(const char* pFileName, const char* pPath, CUtlBuffer& buf, int nMaxBytes = 0, int nStartingByte = 0, FSAllocFunc_t pfnAlloc = NULL) = 0;
	virtual bool			WriteFile(const char* pFileName, const char* pPath, CUtlBuffer& buf) = 0;
	virtual bool			UnzipFile(const char* pFileName, const char* pPath, const char* pDestination) = 0;
};

abstract_class IIoStats
{
public:
	virtual void OnFileSeek(int nTimeInMs) = 0;
	virtual void OnFileRead(int nTimeInMs, int nBytesRead) = 0;
	virtual void OnFileOpen(const char* pFileName) = 0;

	virtual int GetNumberOfFileSeeks() = 0;
	virtual int GetTimeInFileSeek() = 0;

	virtual int GetNumberOfFileReads() = 0;
	virtual int GetTimeInFileReads() = 0;
	virtual int GetFileReadTotalSize() = 0;

	virtual int GetNumberOfFileOpens() = 0;

	virtual void Reset() = 0;

protected:
	virtual ~IIoStats()
	{
	}
};

abstract_class IFileSystem : public IAppSystem, public IBaseFileSystem
{
public:
	virtual bool			IsSteam() const = 0;

	virtual	FilesystemMountRetval_t MountSteamContent(int nExtraAppId = -1) = 0;

	virtual void			AddSearchPath(const char* pPath, const char* pathID, SearchPathAdd_t addType = PATH_ADD_TO_TAIL) = 0;
	virtual bool			RemoveSearchPath(const char* pPath, const char* pathID = 0) = 0;

	virtual void			RemoveAllSearchPaths(void) = 0;

	virtual void			RemoveSearchPaths(const char* szPathID) = 0;

	virtual void			MarkPathIDByRequestOnly(const char* pPathID, bool bRequestOnly) = 0;

	virtual bool			IsFileInReadOnlySearchPath(const char* pPath, const char* pathID = 0) = 0;

	virtual const char* RelativePathToFullPath(const char* pFileName, const char* pPathID, char* pLocalPath, int localPathBufferSize, PathTypeFilter_t pathFilter = FILTER_NONE, PathTypeQuery_t* pPathType = NULL) = 0;
#if IsGameConsole()
	virtual bool            GetPackFileInfoFromRelativePath(const char* pFileName, const char* pPathID, char* pPackPath, int nPackPathBufferSize, int64& nPosition, int64& nLength) = 0;
#endif
	virtual int				GetSearchPath(const char* pathID, bool bGetPackFiles, char* pPath, int nMaxLen) = 0;

	virtual bool			AddPackFile(const char* fullpath, const char* pathID) = 0;

	virtual void			RemoveFile(char const* pRelativePath, const char* pathID = 0) = 0;

	virtual bool			RenameFile(char const* pOldPath, char const* pNewPath, const char* pathID = 0) = 0;

	virtual void			CreateDirHierarchy(const char* path, const char* pathID = 0) = 0;

	virtual bool			IsDirectory(const char* pFileName, const char* pathID = 0) = 0;

	virtual void			FileTimeToString(char* pStrip, int maxCharsIncludingTerminator, long fileTime) = 0;

	virtual void			SetBufferSize(FileHandle_t file, unsigned nBytes) = 0;

	virtual bool			IsOk(FileHandle_t file) = 0;

	virtual bool			EndOfFile(FileHandle_t file) = 0;

	virtual char* ReadLine(char* pOutput, int maxChars, FileHandle_t file) = 0;
#if ! defined(SWIG)
	virtual int				FPrintf(FileHandle_t file, PRINTF_FORMAT_STRING const char* pFormat, ...) FMTFUNCTION(3, 4) = 0;
#else
	virtual int				FPrintf(FileHandle_t file, const char* pFormat, ...) FMTFUNCTION(3, 4) = 0;
#endif

	virtual CSysModule* LoadModule(const char* pFileName, const char* pPathID = 0, bool bValidatedDllOnly = true) = 0;
	virtual void			UnloadModule(CSysModule* pModule) = 0;

	virtual const char* FindFirst(const char* pWildCard, FileFindHandle_t* pHandle) = 0;
	virtual const char* FindNext(FileFindHandle_t handle) = 0;
	virtual bool			FindIsDirectory(FileFindHandle_t handle) = 0;
	virtual void			FindClose(FileFindHandle_t handle) = 0;

	virtual const char* FindFirstEx(
		const char* pWildCard,
		const char* pPathID,
		FileFindHandle_t* pHandle
		) = 0;

	virtual void			FindFileAbsoluteList(CUtlVector< CUtlString >& outAbsolutePathNames, const char* pWildCard, const char* pPathID) = 0;

	virtual const char* GetLocalPath(const char* pFileName, char* pLocalPath, int localPathBufferSize) = 0;

	virtual bool			FullPathToRelativePath(const char* pFullpath, char* pRelative, int maxlen) = 0;

	virtual bool			GetCurrentDirectory(char* pDirectory, int maxlen) = 0;

	virtual FileNameHandle_t	FindOrAddFileName(char const* pFileName) = 0;
	virtual bool				String(const FileNameHandle_t& handle, char* buf, int buflen) = 0;

			FSAsyncStatus_t	AsyncRead(const FileAsyncRequest_t& request, FSAsyncControl_t* phControl = NULL) { return AsyncReadMultiple(&request, 1, phControl); }
	virtual FSAsyncStatus_t	AsyncReadMultiple(const FileAsyncRequest_t* pRequests, int nRequests,  FSAsyncControl_t* phControls = NULL) = 0;
	virtual FSAsyncStatus_t	AsyncAppend(const char* pFileName, const void* pSrc, int nSrcBytes, bool bFreeMemory, FSAsyncControl_t* pControl = NULL) = 0;
	virtual FSAsyncStatus_t	AsyncAppendFile(const char* pAppendToFileName, const char* pAppendFromFileName, FSAsyncControl_t* pControl = NULL) = 0;
	virtual void			AsyncFinishAll(int iToPriority = 0) = 0;
	virtual void			AsyncFinishAllWrites() = 0;
	virtual FSAsyncStatus_t	AsyncFlush() = 0;
	virtual bool			AsyncSuspend() = 0;
	virtual bool			AsyncResume() = 0;

	virtual FSAsyncStatus_t	AsyncBeginRead(const char* pszFile, FSAsyncFile_t* phFile) = 0;
	virtual FSAsyncStatus_t	AsyncEndRead(FSAsyncFile_t hFile) = 0;

	virtual FSAsyncStatus_t	AsyncFinish(FSAsyncControl_t hControl, bool wait = true) = 0;
	virtual FSAsyncStatus_t	AsyncGetResult(FSAsyncControl_t hControl, void** ppData, int* pSize) = 0;
	virtual FSAsyncStatus_t	AsyncAbort(FSAsyncControl_t hControl) = 0;
	virtual FSAsyncStatus_t	AsyncStatus(FSAsyncControl_t hControl) = 0;
	virtual FSAsyncStatus_t	AsyncSetPriority(FSAsyncControl_t hControl, int newPriority) = 0;
	virtual void			AsyncAddRef(FSAsyncControl_t hControl) = 0;
	virtual void			AsyncRelease(FSAsyncControl_t hControl) = 0;

	virtual WaitForResourcesHandle_t WaitForResources(const char* resourcelist) = 0;
	virtual bool			GetWaitForResourcesProgress(WaitForResourcesHandle_t handle, float* progress    , bool* complete   ) = 0;
	virtual void			CancelWaitForResources(WaitForResourcesHandle_t handle) = 0;

	virtual int				HintResourceNeed(const char* hintlist, int forgetEverything) = 0;
	virtual bool			IsFileImmediatelyAvailable(const char* pFileName) = 0;

	virtual void			GetLocalCopy(const char* pFileName) = 0;

	virtual void			PrintOpenedFiles(void) = 0;
	virtual void			PrintSearchPaths(void) = 0;

	virtual void			SetWarningFunc(void (*pfnWarning)(const char* fmt, ...)) = 0;
	virtual void			SetWarningLevel(FileWarningLevel_t level) = 0;
	virtual void			AddLoggingFunc(void (*pfnLogFunc)(const char* fileName, const char* accessType)) = 0;
	virtual void			RemoveLoggingFunc(FileSystemLoggingFunc_t logFunc) = 0;

	virtual const FileSystemStatistics* GetFilesystemStatistics() = 0;

#if defined( _PS3 )
	virtual void LogFileAccess(const char* pFullFileName) = 0;

	virtual bool PrefetchFile(const char* pFileName, int nPriority, bool bPersist) = 0;
	virtual bool PrefetchFile(const char* pFileName, int nPriority, bool bPersist, int64 nOffset, int64 nSize) = 0;
	virtual void FlushCache() = 0;
	virtual void SuspendPrefetches(const char* pWhy) = 0;
	virtual void ResumePrefetches(const char* pWhy) = 0;

	virtual void OnSaveStateChanged(bool bSaving) = 0;

	virtual bool IsPrefetchingDone() = 0;

#endif 
	virtual FileHandle_t	OpenEx(const char* pFileName, const char* pOptions, unsigned flags = 0, const char* pathID = 0, char** ppszResolvedFilename = NULL) = 0;

	virtual int				ReadEx(void* pOutput, int sizeDest, int size, FileHandle_t file) = 0;
	virtual int				ReadFileEx(const char* pFileName, const char* pPath, void** ppBuf, bool bNullTerminate = false, bool bOptimalAlloc = false, int nMaxBytes = 0, int nStartingByte = 0, FSAllocFunc_t pfnAlloc = NULL) = 0;

	virtual FileNameHandle_t	FindFileName(char const* pFileName) = 0;

#if defined( TRACK_BLOCKING_IO )
	virtual void			EnableBlockingFileAccessTracking(bool state) = 0;
	virtual bool			IsBlockingFileAccessEnabled() const = 0;

	virtual IBlockingFileItemList* RetrieveBlockingFileAccessInfo() = 0;
#endif

	virtual void SetupPreloadData() = 0;
	virtual void DiscardPreloadData() = 0;

	enum KeyValuesPreloadType_t
	{
		TYPE_VMT,
		TYPE_SOUNDEMITTER,
		TYPE_SOUNDSCAPE,
		TYPE_SOUNDOPERATORS,
		NUM_PRELOAD_TYPES
	};

	virtual KeyValues* LoadKeyValues(KeyValuesPreloadType_t type, char const* filename, char const* pPathID = 0) = 0;
	virtual bool		LoadKeyValues(KeyValues& head, KeyValuesPreloadType_t type, char const* filename, char const* pPathID = 0) = 0;

	virtual FSAsyncStatus_t	AsyncWrite(const char* pFileName, const void* pSrc, int nSrcBytes, bool bFreeMemory, bool bAppend = false, FSAsyncControl_t* pControl = NULL) = 0;
	virtual FSAsyncStatus_t	AsyncWriteFile(const char* pFileName, const CUtlBuffer* pSrc, int nSrcBytes, bool bFreeMemory, bool bAppend = false, FSAsyncControl_t* pControl = NULL) = 0;
	FSAsyncStatus_t			AsyncReadCreditAlloc(const FileAsyncRequest_t& request, const char* pszFile, int line, FSAsyncControl_t* phControl = NULL) { return AsyncReadMultipleCreditAlloc(&request, 1, pszFile, line, phControl); }
	virtual FSAsyncStatus_t	AsyncReadMultipleCreditAlloc(const FileAsyncRequest_t* pRequests, int nRequests, const char* pszFile, int line, FSAsyncControl_t* phControls = NULL) = 0;

	virtual FSAsyncStatus_t AsyncDirectoryScan(const char* pSearchSpec, bool recurseFolders,  void* pContext, FSAsyncScanAddFunc_t pfnAdd, FSAsyncScanCompleteFunc_t pfnDone, FSAsyncControl_t* pControl = NULL) = 0;

	virtual bool			GetFileTypeForFullPath(char const* pFullPath, wchar_t* buf, size_t bufSizeInBytes) = 0;

	virtual bool		ReadToBuffer(FileHandle_t hFile, CUtlBuffer& buf, int nMaxBytes = 0, FSAllocFunc_t pfnAlloc = NULL) = 0;

	virtual bool		GetOptimalIOConstraints(FileHandle_t hFile, unsigned* pOffsetAlign, unsigned* pSizeAlign, unsigned* pBufferAlign) = 0;
	inline unsigned		GetOptimalReadSize(FileHandle_t hFile, unsigned nLogicalSize);
	virtual void* AllocOptimalReadBuffer(FileHandle_t hFile, unsigned nSize = 0, unsigned nOffset = 0) = 0;
	virtual void		FreeOptimalReadBuffer(void*) = 0;

	virtual void		BeginMapAccess() = 0;
	virtual void		EndMapAccess() = 0;

	virtual bool		FullPathToRelativePathEx(const char* pFullpath, const char* pPathId, char* pRelative, int maxlen) = 0;

	virtual int			GetPathIndex(const FileNameHandle_t& handle) = 0;
	virtual long		GetPathTime(const char* pPath, const char* pPathID) = 0;

	virtual DVDMode_t	GetDVDMode() = 0;

	virtual void			EnableWhitelistFileTracking(bool bEnable, bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes) = 0;

	virtual void			RegisterFileWhitelist(IFileList* pWantCRCList, IFileList* pAllowFromDiskList, IFileList** pFilesToReload) = 0;

	virtual void			MarkAllCRCsUnverified() = 0;

	virtual void			CacheFileCRCs(const char* pPathname, ECacheCRCType eType, IFileList* pFilter) = 0;
	virtual EFileCRCStatus	CheckCachedFileHash(const char* pPathID, const char* pRelativeFilename, int nFileFraction, FileHash_t* pFileHash) = 0;

	virtual int				GetUnverifiedFileHashes(CUnverifiedFileHash* pFiles, int nMaxFiles) = 0;

	virtual int				GetWhitelistSpewFlags() = 0;
	virtual void			SetWhitelistSpewFlags(int flags) = 0;

	virtual void			InstallDirtyDiskReportFunc(FSDirtyDiskReportFunc_t func) = 0;

	virtual bool			IsLaunchedFromXboxHDD() = 0;
	virtual bool			IsInstalledToXboxHDDCache() = 0;
	virtual bool			IsDVDHosted() = 0;
	virtual bool			IsInstallAllowed() = 0;

	virtual int				GetSearchPathID(char* pPath, int nMaxLen) = 0;
	virtual bool			FixupSearchPathsAfterInstall() = 0;

	virtual FSDirtyDiskReportFunc_t		GetDirtyDiskReportFunc() = 0;

	virtual void AddVPKFile(char const* pszName, SearchPathAdd_t addType = PATH_ADD_TO_TAIL) = 0;
	virtual void RemoveVPKFile(char const* pszName) = 0;
	virtual void GetVPKFileNames(CUtlVector<CUtlString>& destVector) = 0;
	virtual void			RemoveAllMapSearchPaths() = 0;
	virtual void			SyncDvdDevCache() = 0;

	virtual bool			GetStringFromKVPool(CRC32_t poolKey, unsigned int key, char* pOutBuff, int buflen) = 0;

	virtual bool			DiscoverDLC(int iController) = 0;
	virtual int				IsAnyDLCPresent(bool* pbDLCSearchPathMounted = NULL) = 0;
	virtual bool			GetAnyDLCInfo(int iDLC, unsigned int* pLicenseMask, wchar_t* pTitleBuff, int nOutTitleSize) = 0;
	virtual int				IsAnyCorruptDLC() = 0;
	virtual bool			GetAnyCorruptDLCInfo(int iCorruptDLC, wchar_t* pTitleBuff, int nOutTitleSize) = 0;
	virtual bool			AddDLCSearchPaths() = 0;
	virtual bool			IsSpecificDLCPresent(unsigned int nDLCPackage) = 0;

	virtual void            SetIODelayAlarm(float flThreshhold) = 0;

	virtual bool			AddXLSPUpdateSearchPath(const void* pData, int nSize) = 0;

	virtual IIoStats* GetIoStats() = 0;

	virtual void			CacheAllVPKFileHashes(bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes) = 0;
	virtual bool			CheckVPKFileHash(int PackFileID, int nPackFileNumber, int nFileFraction, MD5Value_t& md5Value) = 0;

	virtual void			GetVPKFileStatisticsKV(KeyValues* pKV) = 0;

};

#if defined( _X360 ) && !defined( _CERT )
extern char g_szXboxProfileLastFileOpened[MAX_PATH];
#define SetLastProfileFileRead( s ) Q_strncpy( g_szXboxProfileLastFileOpened, sizeof( g_szXboxProfileLastFileOpened), pFileName )
#define GetLastProfileFileRead() (&g_szXboxProfileLastFileOpened[0])
#else
#define SetLastProfileFileRead( s ) ((void)0)
#define GetLastProfileFileRead() NULL
#endif

#if defined( _X360 ) && defined( _BASETSD_H_ )
class CXboxDiskCacheSetter
{
public:
	CXboxDiskCacheSetter(SIZE_T newSize)
	{
		m_oldSize = XGetFileCacheSize();
		XSetFileCacheSize(newSize);
	}

	~CXboxDiskCacheSetter()
	{
		XSetFileCacheSize(m_oldSize);
	}
private:
	SIZE_T m_oldSize;
};
#define DISK_INTENSIVE() CXboxDiskCacheSetter cacheSetter( 1024*1024 )
#else
#define DISK_INTENSIVE() ((void)0)
#endif

inline unsigned IFileSystem::GetOptimalReadSize(FileHandle_t hFile, unsigned nLogicalSize)
{
	unsigned align;
	if (GetOptimalIOConstraints(hFile, &align, NULL, NULL))
		return AlignValue(nLogicalSize, align);
	else
		return nLogicalSize;
}

#include "filesystem_passthru.h"

#if (defined(_DEBUG) || defined(USE_MEM_DEBUG))
#define AsyncRead( a, b ) AsyncReadCreditAlloc( a, __FILE__, __LINE__, b )
#define AsyncReadMutiple( a, b, c ) AsyncReadMultipleCreditAlloc( a, b, __FILE__, __LINE__, c )
#endif




#endif  