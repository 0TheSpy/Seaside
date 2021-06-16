#ifndef ECON_ITEM_CONSTANTS_H
#define ECON_ITEM_CONSTANTS_H
#ifdef _WIN32
#pragma once
#endif

#include "game_item_schema.h"
#include "econ_item_constants.h"
#include "localization_provider.h"
#include "econ_item_interface.h"
#include "econ_item.h"
#include "utlhashtable.h"

#if defined(CLIENT_DLL)
#include "iclientrenderable.h"

#if defined(CSTRIKE15)
#include "custommaterialowner.h"
#endif  

#include "icompositetexturegenerator.h"

#endif  

#include "MaterialSystemUtil.h"

#if defined(TF_DLL)
#include "tf_item_schema.h"
#endif  

#if defined(CLIENT_DLL) 
#define CEconItemView C_EconItemView

#if defined(CSTRIKE15)
typedef void (*ImageReadyCallback_t)(const CEconItemView* pItemView, CUtlBuffer& rawImageRgba, int nWidth, int nHeight, uint64 nItemID);
struct RenderToRTData_t;
class CMergedMDL;
#endif  

#endif  

#include "econ_item_view_helpers.h"

#define RGB_INT_RED  12073019
#define RGB_INT_BLUE 5801378
#define ECON_ITEM_GENERATED_ICON_WIDTH 512
#define ECON_ITEM_GENERATED_ICON_HEIGHT 384
#define ECON_ITEM_GENERATED_PINBOARD_ICON_WIDTH 512
#define ECON_ITEM_GENERATED_PINBOARD_ICON_HEIGHT 384
#define ECON_ITEM_GENERATED_ICON_DIR "resource/Flash/econ/weapons/cached/"

#define SCHEMA_BASE_ITEM_MAX 499           

class CEconItemAttribute;
class CAttributeManager;

#if defined(CSTRIKE_CLIENT_DLL)
class IVisualsDataProcessor;
#endif

struct stickerMaterialReference_t
{
	CMaterialReference	m_pMaterialReferenceFirstPerson;
	CMaterialReference	m_pMaterialReferenceThirdPerson;
	int					m_nSlotIndex;
};

class CAttributeList
{
	friend class CEconItemView;
	friend class CTFPlayer;

	DECLARE_CLASS_NOBASE(CAttributeList);
public:
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_DATADESC();

	CAttributeList();
	CAttributeList& operator=(const CAttributeList& src);

	void					Init();
	void					SetManager(CAttributeManager* pManager);

	void					IterateAttributes(class IEconItemAttributeIterator* pIterator);

	void					DestroyAllAttributes(void);

private:
	void					AddAttribute(CEconItemAttribute* pAttribute);
	void					SetOrAddAttributeValueByName(const char* pszAttribDefName, float flValue);

	void					RemoveAttribute(const char* pszAttribDefName);
	void					RemoveAttributeByIndex(int iIndex);

public:
	CEconItemAttribute* GetAttributeByName(const char* pszAttribDefName);
	const CEconItemAttribute* GetAttributeByName(const char* pszAttribDefName) const;

	CEconItemAttribute* GetAttributeByClass(const char* szAttribClass);
	const CEconItemAttribute* GetAttributeByClass(const char* szAttribClass) const;

	CEconItemAttribute* GetAttributeByDefIndex(uint16 unAttrDefIndex);
	const CEconItemAttribute* GetAttributeByDefIndex(uint16 unAttrDefIndex) const;


	void					SetValue(CEconItemAttribute* pAttrib, float flValue);

	void					UpdateManagerCache(void);

private:
	int						GetNumAttributes(void) const { return m_Attributes.Count(); }
	CEconItemAttribute* GetAttribute(int iIndex) { Assert(iIndex >= 0 && iIndex < m_Attributes.Count()); return &m_Attributes[iIndex]; }
	const CEconItemAttribute* GetAttribute(int iIndex) const { Assert(iIndex >= 0 && iIndex < m_Attributes.Count()); return &m_Attributes[iIndex]; }

	CUtlVector<CEconItemAttribute>		m_Attributes;

	CAttributeManager* m_pManager;
};

class CEconItemAttribute
{
	DECLARE_CLASS_NOBASE(CEconItemAttribute);
public:
	DECLARE_EMBEDDED_NETWORKVAR();

	CEconItemAttribute(const attrib_definition_index_t iAttributeIndex, float flValue);
	CEconItemAttribute(const attrib_definition_index_t iAttributeIndex, uint32 unValue);

	CEconItemAttribute& operator=(const CEconItemAttribute& val);
	attrib_definition_index_t			GetAttribIndex(void) const { return m_iAttributeDefinitionIndex; }
	void			SetAttribIndex(attrib_definition_index_t iIndex) { m_iAttributeDefinitionIndex = iIndex; }

	const CEconItemAttributeDefinition* GetStaticData(void) const;

	float			GetValue(void) const;

	uint32			GetIntValue(void) const;

	float			GetInitialValue(void) const { return m_flInitialValue; }
	int				GetRefundableCurrency(void) const { return m_nRefundableCurrency; }
	void			AddRefundableCurrency(int nAdd) { m_nRefundableCurrency += nAdd; }
	void			RemovedRefundableCurrency(int nSubtract) { m_nRefundableCurrency -= nSubtract; }

	bool			GetSetBonus(void) { return m_bSetBonus; }
	void			SetSetBonus(bool bBonus) { m_bSetBonus = bBonus; }

private:
	void			SetValue(float flValue);

	void			SetIntValue(uint32 unValue);

	friend class CAttributeList;
	friend class CEconItemSystem;

	void			Init(void);

private:
	CNetworkVar(attrib_definition_index_t, m_iAttributeDefinitionIndex);

	CNetworkVar(float, m_flValue);

	CNetworkVar(float, m_flInitialValue);
	CNetworkVar(int, m_nRefundableCurrency);

	CNetworkVar(bool, m_bSetBonus);         

public:
	CEconItemAttribute(void);
};

#if defined(CLIENT_DLL)
class CEconItemView : public CDefaultClientRenderable, public IEconItemInterface, public CCustomMaterialOwner
#else
class CEconItemView : public IEconItemInterface
#endif
{
#if defined(CLIENT_DLL) || defined(GAME_DLL)
	DECLARE_CLASS_NOBASE(CEconItemView);
public:
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_DATADESC();
#endif

public:
	CEconItemView();
	CEconItemView(const CEconItemView& src);
	virtual ~CEconItemView();
	CEconItemView& operator=(const CEconItemView& src);
	bool 				operator==(const CEconItemView& other) const;
	bool				operator!=(const CEconItemView& other) const { return !operator==(other); }

	virtual const GameItemDefinition_t* GetItemDefinition() const
	{
		return GetStaticData();
	}

public:

	virtual int32			GetQuality() const;
	virtual int32			GetRarity() const;
	virtual style_index_t	GetStyle() const;
	virtual uint8			GetFlags() const;
	virtual eEconItemOrigin GetOrigin() const;
	virtual uint16				GetQuantity() const;

	virtual const char* GetCustomName() const;
	virtual const char* GetCustomDesc() const;

	virtual int				GetItemSetIndex() const;

	virtual bool			GetInUse() const { return GetSOCData() ? GetSOCData()->GetInUse() : false; }

	virtual void			IterateAttributes(class IEconItemAttributeIterator* pIterator) const OVERRIDE;

	bool					IsValid(void) const { return m_bInitialized; }
	void					Invalidate(void);

	void					Init(int iDefIndex, int iQuality, int iLevel, uint32 iAccountID = 0);
	void					SetInitialized(bool bInit) { m_bInitialized = bInit; }
	bool					Init(CEconItem* pItem);

	const GameItemDefinition_t* GetStaticData(void) const;

	void					SetNonSOEconItem(CEconItem* pItem) { m_pNonSOEconItem = pItem; }

	void					MarkDescriptionDirty(void);

	virtual bool			IsStyleUnlocked(int iStyle) const;

private:
	void					EnsureDescriptionIsBuilt(void) const;
public:
	void					SetGrayedOutReason(const char* pszGrayedOutReason);

	void					SetItemIndex(item_definition_index_t iIndex) { m_iItemDefinitionIndex = iIndex; MarkDescriptionDirty(); }
	item_definition_index_t	GetItemIndex(void) const { return m_iItemDefinitionIndex; }

	void					SetItemQuality(int iQuality) { m_iEntityQuality = iQuality; MarkDescriptionDirty(); }
	int						GetItemQuality(void) const { return m_iEntityQuality; }
	void					SetItemLevel(uint32 unLevel) { m_iEntityLevel = unLevel; MarkDescriptionDirty(); }
	uint32					GetItemLevel(void) const { return m_iEntityLevel; }

	int						GetItemQuantity() const;
#ifdef CLIENT_DLL
	void					SetIsStoreItem(bool bIsStoreItem) { m_bIsStoreItem = bIsStoreItem; MarkDescriptionDirty(); }
	void					SetIsTradeItem(bool bIsTradeItem) { m_bIsTradeItem = bIsTradeItem; MarkDescriptionDirty(); }
	void					SetItemQuantity(int iQuantity) { m_iEntityQuantity = iQuantity; MarkDescriptionDirty(); }
	void					SetClientItemFlags(uint8 unFlags);
	void					SetItemRarityOverride(int iRarity) { m_iRarityOverride = iRarity; MarkDescriptionDirty(); }
	void					SetItemQualityOverride(int iQuality) { m_iQualityOverride = iQuality; MarkDescriptionDirty(); }
	void					SetItemStyleOverride(style_index_t unNewStyleOverride);
#endif
	style_index_t			GetItemStyle() const;

	void					SetItemID(itemid_t iIdx) { m_iItemID = iIdx; m_iItemIDHigh = (m_iItemID >> 32); m_iItemIDLow = (m_iItemID & 0xFFFFFFFF); }
#ifdef CLIENT_DLL
	virtual itemid_t		GetItemID(void) const { uint64 iTmp = ((((int64)m_iItemIDHigh) << 32) | m_iItemIDLow); return (itemid_t)iTmp; }
#else
	itemid_t				GetItemID(void) const { return m_iItemID; }
#endif
	uint32					GetItemIDHigh(void) const { return m_iItemIDHigh; }
	uint32					GetItemIDLow(void)  const { return m_iItemIDLow; }

	itemid_t				GetFauxItemIDFromDefinitionIndex(void) const;

	uint32					GetAccountID(void) const { return m_iAccountID; }

	void					SetInventoryPosition(uint32 iPosition) { m_iInventoryPosition = iPosition; }
	const uint32			GetInventoryPosition(void) const { return m_iInventoryPosition; }

	const char* GetInventoryModel(void);
	const char* GetInventoryImage(void) const;
	bool					HasGeneratedInventoryImage(void) const;
	bool					GetInventoryImageData(int* iPosition, int* iSize);
	const char* GetInventoryOverlayImage(int idx);
	int						GetInventoryOverlayImageCount(void);

	IMaterial* GetToolStickerMaterial(void);
	bool					IsStickerTool(void);

	const char* GetPlayerDisplayModel(int iClass = 0) const;

	const char* GetWorldDisplayModel();
	const char* GetExtraWearableModel();
	const char* GetIconDisplayModel();
	const char* GetBuyMenuDisplayModel();
	const char* GetWorldDroppedModel();
	const char* GetPedestalDisplayModel();
	const char* GetMagazineModel();

	const char* GetScopeLensMaskModel();

	const char* GetStatTrakModelByType(int nStatTrakType);

	const char* GetUidModel();

	int						GetAnimationSlot(void);

	int						GetDropType(void);

	void					DestroyAllAttributes(void);

	void					AddAttribute(CEconItemAttribute* pAttribute);

	void					SetOrAddAttributeValueByName(const char* pszAttribDefName, float flValue);

	void					InitNetworkedDynamicAttributesForDemos(void);
	void					UpdateNetworkedDynamicAttributesForDemos(attrib_definition_index_t nDef, float flNewValue);

#if defined(CSTRIKE_CLIENT_DLL)
	void					UpdateGeneratedMaterial(bool bIgnorePicMip = false, CompositeTextureSize_t diffuseTextureSize = COMPOSITE_TEXTURE_SIZE_512);
#endif  

	const char* GetStickerSlotModelBySlotIndex(int nIndex);
	int						GetNumSupportedStickerSlots(void);
	Vector					GetStickerSlotWorldProjectionStartBySlotIndex(int nIndex);
	Vector					GetStickerSlotWorldProjectionEndBySlotIndex(int nIndex);
	const char* GetStickerWorldModelBoneParentNameBySlotIndex(int nIndex);
	const char* GetStickerSlotMaterialBySlotIndex(int nIndex);
	IMaterial* GetStickerIMaterialBySlotIndex(int nIndex, bool bThirdPerson = false);
	void					GenerateStickerMaterials(void);
	bool					ItemHasAnyStickersApplied(void);
	bool					ItemHasAnyFreeStickerSlots(void);
	int						GetStickerSlotFirstFreeFromIndex(int nIndex = 0);


	void					SetCustomNameOverride(const char* pszCustomName);

	virtual void            GenerateKillEaterTypeVector(void);
	virtual void            GetKillEaterTypes(CUtlSortVector<uint32>& types);
	virtual int32           GetKillEaterValueByType(uint32 uKillEaterType);


	bool                          m_bKillEaterTypesCached;
	CUtlSortVector<uint32>        m_vCachedKillEaterTypes;
	int                           m_nKillEaterValuesCacheFrame;
	CUtlHashtable<uint32, int32>  m_vCachedKillEaterValues;

private:
	void					Cleanup(void);

#if defined(CSTRIKE_CLIENT_DLL)
	void					CreateCustomWeaponMaterials(int nWeaponId, bool bIgnorePicMip, CompositeTextureSize_t diffuseTextureSize = COMPOSITE_TEXTURE_SIZE_512);
	void					CreateCustomClothingMaterials(const char* pchSkinIdent, int nSlotId, int nTeam, bool bIgnorePicMip, CompositeTextureSize_t diffuseTextureSize = COMPOSITE_TEXTURE_SIZE_512);

public:
	void					Update(void);
	void					SaveInventoryImageAsPNG(int nWidth, int nHeight);
	const CUtlBuffer* GetInventoryImageRgba(int nWidth, int nHeight, ImageReadyCallback_t pImageReadyCallback);
	bool					CanGenerateInventoryImageRgba(void);
	void					ClearInventoryImageRgba(void);
	bool					LoadCachedInventoryImage(void);
	void					SaveInventoryImage(CUtlBuffer& rawImageRgba);
	void					GenerateCachedInventoryImageName();

	static void				CleanInventoryImageCacheDir(void);

	void					FinishLoadCachedInventoryImage(void* pData, int numReadBytes, FSAsyncStatus_t asyncStatus);

	enum AsyncFixupState_t
	{
		AFS_Init,					      
		AFS_LoadingInProgress,		      
		AFS_LoadingDone,			          
		AFS_FixupDone,				      
	} m_asyncFixupState;

	int						m_nNumAsyncReadBytes;
	FSAsyncStatus_t			m_asyncStatus;
	CUtlBuffer				m_inventoryImageRgba;

private:
	static bool				m_sbHasCleanedInventoryImageCacheDir;

	bool					m_bInventoryImageRgbaRequested;
	bool					m_bInventoryImageTriedCache;
	ImageReadyCallback_t	m_pImageReadyCallback;
	int						m_nInventoryImageRgbaWidth;
	int						m_nInventoryImageRgbaHeight;

	FSAsyncControl_t		m_hAsyncControl;

	char					m_szCurrentLoadCachedFileName[MAX_PATH];

	RenderToRTData_t* m_pRenderToRTData;
	IVTFTexture* m_pScratchVTF;
	CMergedMDL* m_pRenderToRTMDL;
#endif  

	CUtlVector< stickerMaterialReference_t > m_pStickerMaterials;




	void					RemoveAttribute(const char* pszAttribDefName);

public:

	const bool				GetCombinedAttributeClassValue(float& flValue, string_t iszAttribClass) const;

	CEconItemAttribute* GetAttributeByClass(const char* szAttribClass);
	const CEconItemAttribute* GetAttributeByClass(const char* szAttribClass) const;
	CEconItemAttribute* GetAttributeByDefIndex(int iAttributeDefIndex);
	const CEconItemAttribute* GetAttributeByDefIndex(int iAttributeDefIndex) const;
	CEconItemAttribute* GetAttributeByName(const char* pszAttribDefName);
	const CEconItemAttribute* GetAttributeByName(const char* pszAttribDefName) const;

	int						  GetNumAttributes(void) const { return m_AttributeList.GetNumAttributes(); }
	CEconItemAttribute* GetAttribute(int iIndex) { return m_AttributeList.GetAttribute(iIndex); }
	const CEconItemAttribute* GetAttribute(int iIndex) const { return m_AttributeList.GetAttribute(iIndex); }
	CAttributeList* GetAttributeList(void) { return m_AttributeList.Get(); }
	const CAttributeList* GetAttributeList(void) const { return m_AttributeList.Get(); }

	int						GetModifiedRGBValue(bool bAltColor = false);
	int						GetCustomPaintKitIndex(void) const;

	const char* GetCustomPaintKitDbgName(void) const;

	bool					CanCollect(CEconItemView& subject);

	uint64					 GetCustomUserTextureID();
	const CPaintKit* GetCustomPaintKit(void) const;

	CEconItem* GetSOCData(void) const;

	bool					IsEquipped(void) const { return GetSOCData() && GetSOCData()->IsEquipped(); }
	bool					IsEquippedForClass(equipped_class_t unClass) const { return GetSOCData() && GetSOCData()->IsEquippedForClass(unClass); }
	void					UpdateEquippedState(equipped_class_t unClass, equipped_slot_t unSlot) { if (GetSOCData()) { GetSOCData()->UpdateEquippedState(unClass, unSlot); } }
	equipped_slot_t			GetEquippedPositionForClass(equipped_class_t unClass) const { return GetSOCData() ? GetSOCData()->GetEquippedPositionForClass(unClass) : INVALID_EQUIPPED_SLOT; }

	EItemSlot				GetSlot(void) const { return EItemSlotFromName(GetItemDefinition()->GetRawDefinition()->GetString("item_slot")); }

	int						GetQualityParticleType();

	int						GetSkin() const;

#if defined(CSTRIKE_CLIENT_DLL)
	IVisualsDataProcessor* GetVisualsDataProcessor(int nIndex) { return (nIndex < m_ppVisualsDataProcessors.Count()) ? m_ppVisualsDataProcessors[nIndex] : NULL; }
	IVisualsDataProcessor* GetVisualsDataProcessorByName(const char* szName) const;

	static CEconItemView* FindOrCreateEconItemViewForItemID(uint64 uiItemId);
#else
	typedef CUtlMap< itemid_t, uint64, int, CDefLess< itemid_t > > UtlMapLookupByID_t;
	static UtlMapLookupByID_t s_mapLookupByID;
#endif

#if defined ( GAME_DLL )
	void UpdateNetworkedCustomName();
#endif

protected:
	CNetworkVar(item_definition_index_t, m_iItemDefinitionIndex);

	CNetworkVar(int, m_iEntityQuality);

	CNetworkVar(uint32, m_iEntityLevel);

	itemid_t			m_iItemID;
	CNetworkVar(uint32, m_iItemIDHigh);
	CNetworkVar(uint32, m_iItemIDLow);

	CNetworkVar(uint32, m_iAccountID);

	CNetworkVar(uint32, m_iInventoryPosition);

	CEconItem* m_pNonSOEconItem;

	CNetworkVar(bool, m_bInitialized);

#if defined( CLIENT_DLL )
	bool					m_bIsStoreItem;
	bool					m_bIsTradeItem;
	int						m_iEntityQuantity;
	int						m_iRarityOverride;
	int						m_iQualityOverride;
	uint8					m_unClientFlags;

	style_index_t			m_unOverrideStyle;

public:
	const wchar_t* GetItemName(bool bUncustomized = false) const;

	const class CEconItemDescription* GetDescription() const { EnsureDescriptionIsBuilt(); return m_pDescription; }

private:
	mutable class CEconItemDescription* m_pDescription;
	mutable char* m_pszGrayedOutReason;

#endif
#if defined(CSTRIKE_CLIENT_DLL)
	CUtlVector< IVisualsDataProcessor* > m_ppVisualsDataProcessors;
#endif

protected:

#if defined(CLIENT_DLL)
	virtual const Vector& GetRenderOrigin(void) { return vec3_origin; }
	virtual const QAngle& GetRenderAngles(void) { return vec3_angle; }
	virtual bool			ShouldDraw(void) { return false; }
	virtual bool			IsTransparent(void) { return false; }
	virtual const matrix3x4_t& RenderableToWorldTransform() { static matrix3x4_t mat; SetIdentityMatrix(mat); return mat; }
	virtual void			GetRenderBounds(Vector& mins, Vector& maxs) { return; }
#endif

private:
	CNetworkVarEmbedded(CAttributeList, m_AttributeList);
	CNetworkVarEmbedded(CAttributeList, m_NetworkedDynamicAttributesForDemos);

	CNetworkString(m_szCustomName, MAX_ITEM_CUSTOM_NAME_DATABASE_SIZE);

	char m_szCustomNameOverride[MAX_ITEM_CUSTOM_NAME_DATABASE_SIZE];
	GCSDK::CAutoPtr< char > m_autoptrInventoryImageGeneratedPath;
};

#endif  