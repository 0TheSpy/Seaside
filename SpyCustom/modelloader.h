#if !defined( MOD_LOADER_H )
#define MOD_LOADER_H
#ifdef _WIN32
#pragma once
#endif

struct model_t;
class IMaterial;
class IFileList;


#include "utlmemory.h"


abstract_class IModelLoader
{
public:
	enum REFERENCETYPE
	{
		FMODELLOADER_NOTLOADEDORREFERENCED = 0,
		FMODELLOADER_LOADED = (1 << 0),

		FMODELLOADER_SERVER = (1 << 1),
		FMODELLOADER_CLIENT = (1 << 2),
		FMODELLOADER_CLIENTDLL = (1 << 3),
		FMODELLOADER_STATICPROP = (1 << 4),
		FMODELLOADER_DETAILPROP = (1 << 5),
		FMODELLOADER_REFERENCEMASK = (FMODELLOADER_SERVER | FMODELLOADER_CLIENT | FMODELLOADER_CLIENTDLL | FMODELLOADER_STATICPROP | FMODELLOADER_DETAILPROP),

		FMODELLOADER_TOUCHED_BY_PRELOAD = (1 << 15),
		FMODELLOADER_LOADED_BY_PRELOAD = (1 << 16),
		FMODELLOADER_TOUCHED_MATERIALS = (1 << 17),
	};

	enum ReloadType_t
	{
		RELOAD_LOD_CHANGED = 0,
		RELOAD_EVERYTHING,
		RELOAD_REFRESH_MODELS,
	};

	virtual void		Init(void) = 0;
	virtual void		Shutdown(void) = 0;

	virtual int			GetCount(void) = 0;
	virtual model_t* GetModelForIndex(int i) = 0;

	virtual const char* GetName(const model_t* model) = 0;

	virtual void* GetExtraData(model_t* model) = 0;

	virtual int			GetModelFileSize(const char* name) = 0;

	virtual model_t* GetModelForName(const char* name, REFERENCETYPE referencetype) = 0;
	virtual model_t* ReferenceModel(const char* name, REFERENCETYPE referencetype) = 0;
	virtual void		UnreferenceModel(model_t* model, REFERENCETYPE referencetype) = 0;
	virtual void		UnreferenceAllModels(REFERENCETYPE referencetype) = 0;

	virtual void		UnloadUnreferencedModels(void) = 0;
	virtual void		PurgeUnusedModels(void) = 0;

	virtual bool		Map_GetRenderInfoAllocated(void) = 0;
	virtual void		Map_SetRenderInfoAllocated(bool allocated) = 0;

	virtual void		Map_LoadDisplacements(model_t* model, bool bRestoring) = 0;

	virtual void		Print(void) = 0;

	virtual bool		Map_IsValid(char const* mapname) = 0;

	virtual void		RecomputeSurfaceFlags(model_t* mod) = 0;

	virtual void		Studio_ReloadModels(ReloadType_t reloadType) = 0;

	virtual bool		IsLoaded(const model_t* mod) = 0;

	virtual bool		LastLoadedMapHasHDRLighting(void) = 0;

	virtual void ReloadFilesInList(IFileList* pFilesToReload) = 0;

	virtual const char* GetActiveMapName(void) = 0;
};

extern IModelLoader* modelloader;

class CMapLoadHelper
{
public:
	CMapLoadHelper(int lumpToLoad);
	~CMapLoadHelper(void);

	byte* LumpBase(void);
	int					LumpSize(void);
	int					LumpOffset(void);
	int					LumpVersion() const;
	const char* GetMapName(void);
	char* GetLoadName(void);
	char* GetDiskName(void);
	struct worldbrushdata_t* GetMap(void);

	static void			Init(model_t* pMapModel, const char* pLoadname);
	static void			InitFromMemory(model_t* pMapModel, const void* pData, int nDataSize);
	static void			Shutdown(void);
	static int			GetRefCount(void);

	static void			FreeLightingLump();

	static int			LumpSize(int lumpId);
	static int			LumpOffset(int lumpId);

	void				LoadLumpElement(int nElemIndex, int nElemSize, void* pData);
	void				LoadLumpData(int offset, int size, void* pData);

private:
	int					m_nLumpSize;
	int					m_nLumpOffset;
	int					m_nLumpVersion;
	byte* m_pRawData;
	byte* m_pData;
	byte* m_pUncompressedData;

	int					m_nLumpID;
	char				m_szLumpFilename[MAX_PATH];
};

void Mod_RecomputeTranslucency(model_t* mod, int nSkin, int nBody, void * pClientRenderable);

int Mod_GameLumpSize(int lumpId);
int Mod_GameLumpVersion(int lumpId);
bool Mod_LoadGameLump(int lumpId, void* pBuffer, int size);

int Mod_GetMaterialCount(model_t* mod);

int Mod_GetModelMaterials(model_t* mod, int count, IMaterial** ppMaterial);

bool Mod_MarkWaterSurfaces(model_t* pModel);

void ConnectMDLCacheNotify();
void DisconnectMDLCacheNotify();

void InitStudioModelState(model_t* pModel);

char* GetMapNameOnDisk(char* pDiskName, const char* pFullMapName, unsigned int nDiskNameSize);

extern bool g_bLoadedMapHasBakedPropLighting;
extern bool g_bBakedPropLightingNoSeparateHDR;

#endif  