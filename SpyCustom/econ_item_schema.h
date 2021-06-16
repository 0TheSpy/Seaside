#ifndef ECONITEMSCHEMA_H
#define ECONITEMSCHEMA_H
#ifdef _WIN32
#pragma once
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <string>
#include "steamtypes.h"				   
#include "keyvalues.h"
#include "utldict.h"
#include "econ_item_constants.h"
#include "UtlStringMap.h"

#include "item_selection_criteria.h"
#include "bitvec.h"
#include "smartptr.h"
#include "random.h"
#include "imaterialsystem.h"

#include "expressioncalculator.h"

#if defined(CLIENT_DLL) || defined(GAME_DLL)
#include "weapon_parse.h"
#include "ivmodelinfo.h"
#include "ivmodelrender.h"
#endif

class CEconItem;
#ifdef SOURCE1
#include "gamestringpool.h"
#endif

class CEconItemSchema;
class CEconSharedObjectCache;
class CSOItemRecipe;
class CStickerList;

class CTimedItemRewardDefinition;
void Helper_TemporaryBuildCopyOfLootlistForQuestDrop(CTimedItemRewardDefinition& build, const CTimedItemRewardDefinition* pDescReward, char const* szGameModeExclude);

#define MAX_PAINT_DATA_NAME 128

#define MAX_STICKER_DATA_PATH 128

#define DEFAULT_EMBROIDER_NCOLORS 10
#define DEFAULT_EMBROIDER_GAMMA 0.8

enum CraftFilter_t
{
	CRAFT_FILTER_TRADEUP = -3,
	CRAFT_FILTER_COLLECT = -2,
};

enum EQuestVar_t
{
	k_EQuestVar_First,
	k_EQuestVar_Last
};

class CEconItemRarityDefinition
{
public:
	CEconItemRarityDefinition(void);
	CEconItemRarityDefinition(const CEconItemRarityDefinition& that);
	CEconItemRarityDefinition& operator=(const CEconItemRarityDefinition& rhs);

	~CEconItemRarityDefinition(void) { }

	bool		BInitFromKV(KeyValues* pKVItem, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	int32		GetDBValue(void) const { return m_nValue; }
	const char* GetName(void) const { return !m_strName.IsEmpty() ? m_strName.String() : "unknown"; }
	const char* GetLocKey(void) const { return m_strLocKey.String(); }
	const char* GetWepLocKey(void) const { return m_strWepLocKey.String(); }
	const char* GetLootList(void) const { return m_strLootList.String(); }
	const char* GetRecycleLootList(void) const { return m_strRecycleLootList.String(); }
	const char* GetDropSound(void) const { return m_strDropSound.String(); }
	attrib_colors_t		GetAttribColor(void) const { return m_iAttribColor; }
	const char* GetNextRarity(void) const { return m_strNextRarity.String(); }
	int			GetWhiteCount(void) const { return m_iWhiteCount; }
	int			GetBlackCount(void) const { return m_iBlackCount; }
	float		GetWeight(void) const { return m_flWeight; }

private:

	int32		m_nValue;

	attrib_colors_t		m_iAttribColor;

	CUtlString	m_strName;

	CUtlString  m_strLocKey;
	CUtlString  m_strWepLocKey;

	CUtlString  m_strLootList;
	CUtlString  m_strRecycleLootList;
	CUtlString  m_strDropSound;

	CUtlString  m_strNextRarity;

	int			m_iWhiteCount;
	int			m_iBlackCount;

	float		m_flWeight;
};

typedef int		econ_tag_handle_t;
typedef	uint16	equipped_class_t;
typedef uint16	equipped_slot_t;
typedef uint8	equipped_preset_t;

#define INVALID_ECON_TAG_HANDLE	((econ_tag_handle_t)-1)
#define INVALID_EQUIPPED_SLOT	((equipped_slot_t)-1)
#define INVALID_EQUIPPED_SLOT_BITPACKED 0x3F
#define INVALID_STYLE_INDEX		((style_index_t)-1)
#define INVALID_PRESET_INDEX	((equipped_preset_t)-1)

union attribute_data_union_t
{
	float asFloat;
	uint32 asUint32;
	byte* asBlobPointer;
};

struct static_attrib_t
{
	static_attrib_t()
	{
		iDefIndex = 0;
		m_value.asBlobPointer = NULL;
		m_bForceGCToGenerate = false;

	}

	attrib_definition_index_t	iDefIndex;
	attribute_data_union_t m_value;
	bool	m_bForceGCToGenerate;

	bool BInitFromKV_MultiLine(const char* pszContext, KeyValues* pKVAttribute, CUtlVector<CUtlString>* pVecErrors);

	bool BInitFromKV_SingleLine(const char* pszContext, KeyValues* pKVAttribute, CUtlVector<CUtlString>* pVecErrors);

	const class CEconItemAttributeDefinition* GetAttributeDefinition() const;
};

enum EWebResourceStatus
{
	kWebResource_InvalidName,
	kWebResource_NotLoaded,
	kWebResource_Loading,
	kWebResource_Loaded,
};

struct WeaponPaintableMaterial_t
{
	char m_szName[MAX_PAINT_DATA_NAME];
	char m_szOriginalMaterialName[MAX_PAINT_DATA_NAME];
	char m_szFolderName[MAX_PAINT_DATA_NAME];
	int m_nViewModelSize;						  
	int m_nWorldModelSize;						  
	float m_flWeaponLength;
	float m_flUVScale;
	bool m_bBaseTextureOverride;
	bool m_bMirrorPattern;
};

struct InventoryImageData_t
{
	QAngle* m_pCameraAngles;
	Vector* m_pCameraOffset;
	float m_cameraFOV;
	LightDesc_t* m_pLightDesc[MATERIAL_MAX_LIGHT_COUNT];
	bool m_bOverrideDefaultLight;
};

struct StickerData_t
{
	char	m_szStickerModelPath[MAX_STICKER_DATA_PATH];
	char	m_szStickerMaterialPath[MAX_STICKER_DATA_PATH];
	Vector	m_vWorldModelProjectionStart;
	Vector	m_vWorldModelProjectionEnd;
	char	m_szStickerBoneParentName[32];
};

class CEconItemQualityDefinition
{
public:
	CEconItemQualityDefinition(void);
	CEconItemQualityDefinition(const CEconItemQualityDefinition& that);
	CEconItemQualityDefinition& operator=(const CEconItemQualityDefinition& rhs);

	~CEconItemQualityDefinition(void) { }

	bool		BInitFromKV(KeyValues* pKVItem, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);


	int32		GetDBValue(void) const { return m_nValue; }
	const char* GetName(void) const { return !m_strName.IsEmpty() ? m_strName.Get() : "unknown"; }
	uint32		GetWeight(void) const { return m_unWeight; }
	bool		GetRequiresExplicitMatches(void) const { return m_bExplicitMatchesOnly; }
	bool		CanSupportSet(void) const { return m_bCanSupportSet; }
	const char* GetHexColor(void) const { return !m_strHexColor.IsEmpty() ? m_strHexColor.Get() : "B2B2B2"; }

private:

	int32			m_nValue;

	CUtlConstString	m_strName;

	uint32		m_unWeight;

	bool		m_bExplicitMatchesOnly;

	bool			m_bCanSupportSet;

	CUtlConstString	m_strHexColor;
};

class CEconColorDefinition
{
public:
	bool		BInitFromKV(KeyValues* pKVColor, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	const char* GetName(void) const { return m_strName.Get(); }
	const char* GetColorName(void) const { return m_strColorName.Get(); }		       
	const char* GetHexColor(void) const { return m_strHexColor.Get(); }

private:
	CUtlConstString m_strName;

	CUtlConstString m_strColorName;

	CUtlConstString m_strHexColor;
};

class CEconGraffitiTintDefinition
{
public:
	bool		BInitFromKV(KeyValues* pKVColor, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	int GetID(void) const { return m_nID; }
	const char* GetColorName(void) const { return m_strColorName.Get(); }		       
	const char* GetHexColor(void) const { return m_strHexColor.Get(); }
	uint32 GetHexColorRGB(void) const { return m_unHexColorRGB; }

private:
	int m_nID;

	CUtlConstString m_strColorName;

	CUtlConstString m_strHexColor;
	uint32 m_unHexColorRGB;
};

class CEconMusicDefinition
{
public:
	bool		BInitFromKV(KeyValues* pKVMusicDef, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	uint32		GetID(void) const { return nID; }
	const char* GetName(void) const { return m_strName.Get(); }
	const char* GetNameLocToken(void) const { return m_strNameLocToken.Get(); }
	const char* GetDescription(void) const { return m_strLocDescription.Get(); }
	const char* GetInventoryImage(void) const { return m_strInventoryImage.Get(); }
	const char* GetPedestalDisplayModel(void) const { return m_strPedestalDisplayModel.Get(); }

	const char* GetIconURLSmall() const { return m_sIconURLSmall; }
	const char* GetIconURLLarge() const { return m_sIconURLLarge; }
	void	SetIconURLSmall(const char* szURL) { m_sIconURLSmall = szURL; }
	void	SetIconURLLarge(const char* szURL) { m_sIconURLLarge = szURL; }

private:

	int				nID;
	CUtlConstString m_strName;
	CUtlConstString m_strNameLocToken;
	CUtlConstString	m_strLocDescription;
	CUtlConstString m_strPedestalDisplayModel;
	CUtlConstString	m_strInventoryImage;

	CUtlConstString	m_sIconURLSmall;
	CUtlConstString	m_sIconURLLarge;
};


class CEconQuestDefinition;

struct quest_event_t
{
	quest_event_t(int nStartDay, const char* pchHHMM, int nDurationInMinutes, const char* strTimeZone, CEconQuestDefinition* pQuestDef, RTime32 start, RTime32 end) : m_strHHMM(pchHHMM),
		m_rtEventStart(start),
		m_rtEventEnd(end),
		m_nStartDay(nStartDay),
		m_nDurationInMinutes(nDurationInMinutes),
		m_pQuestDef(pQuestDef),
		m_strTimeZone(strTimeZone) {}
	RTime32						m_rtEventStart;
	RTime32						m_rtEventEnd;
	int							m_nDurationInMinutes;
	int							m_nStartDay;
	CEconQuestDefinition* m_pQuestDef;
	CUtlConstString				m_strHHMM;
	CUtlConstString				m_strTimeZone;


};

typedef CUtlVector<quest_event_t*> QuestEvents_t;
typedef CUtlMap< RTime32, quest_event_t*, int, CDefLess< RTime32 > > QuestEventsSchedule_t;	               


class CEconQuestDefinition
{
public:
	bool		BInitFromKV(KeyValues* pKVQuestDef, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	uint32									GetID(void) const { return nID; }
	const char* GetName(void) const { return m_strName.Get(); }

	const char* GetGameMode(void) const { return m_strGameMode.Get(); }
	const char* GetMapGroup(void) const { return m_strMapGroup.Get(); }
	const char* GetMap(void) const { return m_strMap.Get(); }
	const char* GetRewardLootList(void) const { return m_strRewardLootList.Get(); }

	const char* GetQuestExpression(void) const { return m_strExpr; }
	const char* GetQuestBonusExpression(void) const { return m_strBonusExpr; }

	const uint32							GetDifficulty(void) const { return m_nDifficulty; }
	const uint32							GetOperationalPoints(void) const { return m_nOperationalPoints; }
	const uint32							GetXPReward(void) const { return m_nXPReward; }
	const uint32							GetTargetTeam(void) const { return m_nTargetTeam; }

	const uint32							GetXPBonusPercent(void) const { return m_nXPBonusPercent; }

	const CCopyableUtlVector< uint32 >& GetQuestPoints(void) const { return m_vecQuestPoints; }

	const char* GetQuestConVars(void) const { return m_strQuestConVars.Get(); }
	const bool								IsAnEvent(void) const { return m_bIsAnEvent; }

	static void								TokenizeQuestExpression(const char* szExpression, KeyValues* pKVExpressionTokens);
	static bool								IsQuestExpressionValid(const char* pszQuestExpr);
	static void								ZeroOutQuestExpressionVariables(CExpressionCalculator& expQuest);
	static void								SetQuestExpressionVariable(CExpressionCalculator& expQuest, EQuestVar_t questVar, float flValue);

private:

	int				nID;
	CUtlConstString m_strName;

	CUtlConstString	m_strMapGroup;
	CUtlConstString m_strMap;
	CUtlConstString	m_strGameMode;
	CUtlConstString m_strRewardLootList;

	CUtlConstString m_strExpr;
	CUtlConstString m_strBonusExpr;

	CCopyableUtlVector< uint32 > m_vecQuestPoints;		           
	bool								m_bIsAnEvent;				     

	uint32								m_nDifficulty;				     
	uint32								m_nOperationalPoints;		    
	uint32								m_nXPReward;				      

	uint32								m_nXPBonusPercent;			          
	uint32								m_nTargetTeam;				          

	CUtlConstString						m_strQuestConVars;			            

#ifdef CLIENT_DLL 

public:
	const char* GetNameLocToken(void) const { return m_strNameLocToken.Get(); }
	const char* GetShortNameLocToken(void) const { return m_strShortNameLocToken.Get(); }
	const char* GetDescriptionLocToken(void) const { return m_strDescriptionLocToken.Get(); }
	const char* GetHudDesscriptionLocToken(void) const { return m_strHudDescriptionLocToken.Get(); }
	const char* GetBonusLocToken(void) const { return m_strLocBonus.Get(); }
	KeyValues* GetStringTokens(void) const { return m_kvStringTokens; }
	const char* GetIcon(void) const { return m_strIcon; }

	static void PopulateQuestStringTokens(CEconQuestDefinition& questDef, KeyValues& kvExpressionTokens, KeyValues& kvStringTokens, bool bBonus = false);

private:
	CUtlConstString m_strNameLocToken;
	CUtlConstString m_strShortNameLocToken;
	CUtlConstString	m_strDescriptionLocToken;
	CUtlConstString	m_strHudDescriptionLocToken;
	KeyValues* m_kvStringTokens;
	CUtlConstString m_strLocBonus;
	CUtlConstString m_strIcon;

#endif

};



class CEconCampaignDefinition
{
public:

	class CEconCampaignNodeDefinition
	{
	public:

#ifdef CLIENT_DLL
		class CEconCampaignNodeStoryBlockDefinition
		{
		public:
			bool		BInitFromKV(int nCampaignIndex, int nNodeID, KeyValues* pKVCampaignNodeStoryBlockDef, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

			const char* GetContentFile(void) const { return m_strContentFile.Get(); }
			const char* GetCharacterName(void) const { return m_strCharacterName.Get(); }
			const char* GetStoryBlockExpression(void) const { return m_strExpr.Get(); }
			const char* GetDescription(void) const { return m_strDescription.Get(); }

			float							EvaluateStoryBlockExpression(CEconItemView* pCampaignCoin) const;

		private:
			CUtlConstString			m_strContentFile;			         
			CUtlConstString			m_strCharacterName;			     
			CUtlConstString			m_strExpr;					           
			CUtlConstString			m_strDescription;			     
		};

		const CUtlVector< CEconCampaignNodeStoryBlockDefinition* >& GetStoryBlocks(void) const { return m_vecStoryBlocks; }

		CEconCampaignNodeStoryBlockDefinition* GetBestScoringStoryBlock(CEconItemView* pCampaignCoin) const;

	private:
		CUtlVector< CEconCampaignNodeStoryBlockDefinition* >	m_vecStoryBlocks;			    
	public:
#endif

		bool		BInitFromKV(int nCampaignIndex, KeyValues* pKVCampaignNodeDef, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

		const uint32					GetID(void) const { return m_nID; }
		const uint32					GetQuestIndex(void) const { return m_nQuestIndex; }
		const CUtlVector< uint32 >& GetNextNodes(void) const { return m_vecNextNodes; }
		const uint32					GetCampaignID(void) const { return m_CampaignID; }



	private:

		uint32					m_nID;						    
		uint32					m_nQuestIndex;				    
		uint32					m_CampaignID;
		CUtlVector< uint32 >	m_vecNextNodes;				         



	};

	typedef CUtlMap<int, CEconCampaignNodeDefinition*, int, CDefLess<int> >	CampaignNodeDefinitionsList_t;

	bool		BInitFromKV(KeyValues* pKVCampaignDef, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	const uint32							GetID(void) const { return m_nID; }
	const char* GetNameLocToken(void) const { return m_strNameLocToken.Get(); }
	const char* GetDescription(void) const { return m_strLocDescription.Get(); }
	const CampaignNodeDefinitionsList_t& GetStartNodes(void) const { return m_mapStartNodes; }
	const CampaignNodeDefinitionsList_t& GetCampaignNodes(void) const { return m_mapCampaignNodes; }
	uint32									GetSeasonNumber() const { return m_nSeasonNumber; }

	void								GetAccessibleCampaignNodes(const uint32 unCampaignCompletionBitfield, CUtlVector< CEconCampaignNodeDefinition* >& vecAccessibleNodes);
	void								Helper_RecursiveGetAccessibleCampaignNodes(const uint32 unCampaignCompletionBitfield, const CEconCampaignNodeDefinition* pNode, CUtlVector< CEconCampaignNodeDefinition* >& vecAccessibleNodes);


private:

	int								m_nID;
	CUtlConstString					m_strName;
	CUtlConstString					m_strNameLocToken;
	CUtlConstString					m_strLocDescription;
	CampaignNodeDefinitionsList_t	m_mapCampaignNodes;
	CampaignNodeDefinitionsList_t	m_mapStartNodes;
	uint32							m_nSeasonNumber;			      

};

bool ResolveQuestIdToCampaignAndIndex(uint16 unQuestID, uint32& unCampaignID, uint32& unCamapaignNodeID);
void TokenizeCalculatorExpression(const char* szExpression, KeyValues* pKVExpressionTokens);

struct item_list_entry_t
{
	item_list_entry_t()
	{
		memset(this, 0, sizeof(*this));
		m_nPaintKitSeed = -2;
		m_flPaintKitWear = -2;
	}

	bool operator==(const item_list_entry_t other) const
	{
		return (
			(m_nItemDef == other.m_nItemDef) &&
			(m_nPaintKit == other.m_nPaintKit) &&
			(m_nPaintKitSeed == other.m_nPaintKitSeed) &&
			(m_flPaintKitWear == other.m_flPaintKitWear) &&
			(m_nStickerKit == other.m_nStickerKit) &&
			(m_nMusicKit == other.m_nMusicKit) &&
			(m_bIsNestedList == other.m_bIsNestedList) &&
			(m_bIsUnusualList == other.m_bIsUnusualList) &&
			(m_bAlreadyUsedInRecursiveCreation == m_bAlreadyUsedInRecursiveCreation)
			);
	}

	bool InitFromName(const char* pchName);

	int m_nItemDef;

	int m_nPaintKit;
	int m_nPaintKitSeed;
	float m_flPaintKitWear;

	uint32 m_nStickerKit;

	uint32 m_nMusicKit;

	bool m_bIsNestedList;
	bool m_bIsUnusualList;
	mutable bool m_bAlreadyUsedInRecursiveCreation;
};

class CEconItemSetDefinition : public IEconItemSetDefinition
{
public:
	CEconItemSetDefinition(void);
	CEconItemSetDefinition(const CEconItemSetDefinition& that);
	CEconItemSetDefinition& operator=(const CEconItemSetDefinition& rhs);

	~CEconItemSetDefinition(void) {}

	virtual bool BInitFromKV(KeyValues* pKVItemSet, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	virtual const char* GetName(void)	const { return m_pszName; }
	virtual const char* GetLocKey(void) const { return m_pszLocalizedName; }
	virtual const char* GetUnlocalizedName(void) const { return m_pszUnlocalizedName; }
	virtual int			GetBundle(void) const { return m_iBundleItemDef; }
	virtual int			GetItemCount(void) const { return m_ItemEntries.Count(); }
	virtual int			GetItemDef(int iIndex) const { return m_ItemEntries[iIndex].m_nItemDef; }
	virtual int			GetItemPaintKit(int iIndex) const { return m_ItemEntries[iIndex].m_nPaintKit; }
	virtual int			GetItemRarity(int iIndex) const;
	virtual int			GetHighestItemRarityValue(void) const;
	virtual item_definition_index_t GetCraftReward(void) const { return m_nCraftReward; }

public:

	const char* m_pszName;		 
	const char* m_pszLocalizedName;
	const char* m_pszUnlocalizedName;
	const char* m_pszLocalizedDescription;
	CUtlVector<item_list_entry_t> m_ItemEntries;
	int					m_iBundleItemDef;	           
	bool				m_bIsCollection;
	bool				m_bIsHiddenSet;		                 
	item_definition_index_t	m_nCraftReward;

	struct itemset_attrib_t
	{
		int		m_iAttribDefIndex;
		attrib_value_t	m_valValue;
	};
	CUtlVector<itemset_attrib_t>	m_iAttributes;
};

struct favored_lootlist_t
{
	favored_lootlist_t()
	{
		iHeroID = 0;
		iBonus = 0;
	}

	favored_lootlist_t& operator=(const favored_lootlist_t& rhs)
	{
		iHeroID = rhs.iHeroID;
		iBonus = rhs.iBonus;
		return *this;
	}

	uint iHeroID;
	uint iBonus;
};

class CEconLootListDefinition : public IEconLootListDefinition
{
public:
	CEconLootListDefinition(void);
	CEconLootListDefinition(const CEconLootListDefinition& that);
	CEconLootListDefinition& operator=(const CEconLootListDefinition& rhs);

	~CEconLootListDefinition(void);

	bool AddRandomAtrributes(KeyValues* pRandomAttributesKV, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);
	bool	BInitFromKV(KeyValues* pKVLootList, KeyValues* pKVRandomAttributeTemplates, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL, bool bServerList = false);
	virtual const char* GetName() const { return m_pszName; }
	const CUtlVector<item_list_entry_t>& GetLootListContents() const { return m_ItemEntries; }
	virtual float GetWeight(int iIdx) const { return m_flWeights[iIdx]; }
	virtual KeyValues* GenerateKeyValues() const;
	virtual bool IsServerList() const { return m_bServerList; }
	virtual bool HasUnusualLoot() const;
	virtual int GetAdditionalDropCount(void) const { return m_AdditionalDrops.Count(); }
	virtual bool GetAdditionalDrop(int iIndex, CUtlString& strLootList, float& flChance) const;

	virtual int GetRandomAttributeGroupCount(void) const { return m_RandomAttribs.Count(); }
	virtual bool GetRandomAttributeGroup(int iIndex, float& flChance, float& flTotalWeight) const;
	virtual int GetRandomAttributeCount(int iGroup) const;
	virtual bool GetRandomAttribute(int iGroup, int iIndex, float& flWeight, int& iValue, int& iDefIndex) const;

	virtual void PurgeItems(void);

	bool GetPublicListContents() const
	{
		return true;
	}

	struct loot_list_additional_drop_t
	{
		float		m_fChance;
		bool		m_bPremiumOnly;
		const char* m_pszLootListDefName;
	};
	CUtlVector<loot_list_additional_drop_t>	m_AdditionalDrops;

protected:
	const char* m_pszName;

	CUtlVector<item_list_entry_t> m_ItemEntries;
	uint32				 m_unHeroID;

	bool				m_bPublicListContents;	               
#ifndef GC_DLL
	bool				m_bWillProduceStatTrak;	   
#endif
	float				m_flTotalWeight;
	CUtlVector<float>	m_flWeights;

	struct lootlist_attrib_t
	{
		static_attrib_t	m_staticAttrib;
		float	m_flWeight;
		float	m_flRangeMin;
		float	m_flRangeMax;

		CCopyableUtlVector< uint32 > m_vecValues;

		bool BInitFromKV(const char* pszContext, KeyValues* pKVKey, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors);
	};

	struct random_attrib_t
	{
		float				m_flChanceOfRandomAttribute;
		float				m_flTotalAttributeWeight;
		bool				m_bPickAllAttributes;
		CUtlVector<lootlist_attrib_t> m_RandomAttributes;
	};

	CUtlVector<random_attrib_t*>			m_RandomAttribs;
	bool				m_bServerList;
};

class CEconCraftingRecipeDefinition
{
public:
	CEconCraftingRecipeDefinition(void);
	virtual ~CEconCraftingRecipeDefinition(void) { }

	bool		BInitFromKV(KeyValues* pKVItem, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);
	bool		BInitFromSet(const IEconItemSetDefinition* pSet, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	virtual void CopyPolymorphic(const CEconCraftingRecipeDefinition* pSourceDef) { *this = *pSourceDef; }

	void		SetDefinitionIndex(uint32 iIndex) { m_nDefIndex = iIndex; }
	int32		GetDefinitionIndex(void) const { return m_nDefIndex; }
	const char* GetName(void) const { return !m_strName.IsEmpty() ? m_strName.String() : "unknown"; }
	const char* GetName_A(void) const { return !m_strN_A.IsEmpty() ? m_strN_A.String() : "unknown"; }
	const char* GetDescInputs(void) const { return !m_strDescInputs.IsEmpty() ? m_strDescInputs.String() : "unknown"; }
	const char* GetDescOutputs(void) const { return !m_strDescOutputs.IsEmpty() ? m_strDescOutputs.String() : "unknown"; }

	const char* GetDescI_A(void) const { return !m_strDI_A.IsEmpty() ? m_strDI_A.String() : "unknown"; }
	const char* GetDescI_B(void) const { return !m_strDI_B.IsEmpty() ? m_strDI_B.String() : "unknown"; }
	const char* GetDescI_C(void) const { return !m_strDI_C.IsEmpty() ? m_strDI_C.String() : "unknown"; }
	const char* GetDescO_A(void) const { return !m_strDO_A.IsEmpty() ? m_strDO_A.String() : "unknown"; }
	const char* GetDescO_B(void) const { return !m_strDO_B.IsEmpty() ? m_strDO_B.String() : "unknown"; }
	const char* GetDescO_C(void) const { return !m_strDO_C.IsEmpty() ? m_strDO_C.String() : "unknown"; }

	const wchar_t* GetLocName(void) const { return m_wszName; }
	const wchar_t* GetLocDescription(void) const { return m_wszDesc; }

	void		SetDisabled(bool bDisabled) { m_bDisabled = bDisabled; }
	bool		IsDisabled(void) const { return m_bDisabled; }
	bool		RequiresAllSameClass(void) { return m_bRequiresAllSameClass; }
	bool		RequiresAllSameSlot(void) { return m_bRequiresAllSameSlot; }
	bool		IsAlwaysKnown(void) const { return m_bAlwaysKnown; }
	bool		IsPremiumAccountOnly(void) const { return m_bPremiumAccountOnly; }
	recipecategories_t	GetCategory(void) const { return m_iCategory; }
	int			GetFilter(void) const { return m_iFilter; }
	void		SetFilter(int nFilter) { m_iFilter = nFilter; }
	int			GetTotalInputItemsRequired(void) const;
	int			GetTotalOutputItems(void) const { return m_OutputItemsCriteria.Count(); }

	virtual bool ItemListMatchesInputs(const CUtlVector< CEconItem* >& vecCraftingItems, bool bAllowPartialMatch = false) const;

	const CUtlVector<CItemSelectionCriteria>* GetInputItems(void) const { return &m_InputItemsCriteria; }
	const CUtlVector<uint8>& GetInputItemDupeCounts(void) const { return m_InputItemDupeCounts; }
	const CUtlVector<CItemSelectionCriteria>& GetOutputItems(void) const { return m_OutputItemsCriteria; }

	bool		BSerializeToMsg(CSOItemRecipe& msg) const;
	bool		BDeserializeFromMsg(const CSOItemRecipe& msg);

private:
	void GenerateLocStrings(void);

protected:
	int32		m_nDefIndex;

	CUtlString	m_strName;
	CUtlString	m_strN_A;
	CUtlString	m_strDescInputs;
	CUtlString	m_strDescOutputs;
	CUtlString	m_strDI_A;
	CUtlString	m_strDI_B;
	CUtlString	m_strDI_C;
	CUtlString	m_strDO_A;
	CUtlString	m_strDO_B;
	CUtlString	m_strDO_C;

	wchar_t		m_wszName[64];
	wchar_t		m_wszDesc[512];

	bool		m_bDisabled;
	bool		m_bRequiresAllSameClass;
	bool		m_bRequiresAllSameSlot;
	int			m_iCacheClassUsageForOutputFromItem;
	int			m_iCacheSlotUsageForOutputFromItem;
	int			m_iCacheSetForOutputFromItem;
	bool		m_bAlwaysKnown;
	bool		m_bPremiumAccountOnly;
	recipecategories_t	m_iCategory;
	int			m_iFilter;

	CUtlVector<CItemSelectionCriteria>	m_InputItemsCriteria;
	CUtlVector<uint8>					m_InputItemDupeCounts;

	CUtlVector<CItemSelectionCriteria>	m_OutputItemsCriteria;
};

enum
{
	ATTDESCFORM_VALUE_IS_PERCENTAGE,			  	
	ATTDESCFORM_VALUE_IS_INVERTED_PERCENTAGE,	  	          
	ATTDESCFORM_VALUE_IS_ADDITIVE,				  	
	ATTDESCFORM_VALUE_IS_ADDITIVE_PERCENTAGE,	  	
	ATTDESCFORM_VALUE_IS_OR,					            
	ATTDESCFORM_VALUE_IS_DATE,					    
	ATTDESCFORM_VALUE_IS_ACCOUNT_ID,			     
	ATTDESCFORM_VALUE_IS_PARTICLE_INDEX,		     
	ATTDESCFORM_VALUE_IS_ITEM_DEF,				    
	ATTDESCFORM_VALUE_IS_COLOR,					     
	ATTDESCFORM_VALUE_IS_GAME_TIME,				  	
	ATTDESCFORM_VALUE_IS_MINS_AS_HOURS,			            
	ATTDESCFORM_VALUE_IS_REPLACE,				    
};

enum attrib_effect_types_t
{
	ATTRIB_EFFECT_NEUTRAL = 0,
	ATTRIB_EFFECT_POSITIVE,
	ATTRIB_EFFECT_NEGATIVE,

	NUM_EFFECT_TYPES,
};

enum EAssetClassAttrExportRule_t
{
	k_EAssetClassAttrExportRule_Default = 0,
	k_EAssetClassAttrExportRule_Bucketed = (1 << 0),	       
	k_EAssetClassAttrExportRule_Skip = (1 << 1),	        
	k_EAssetClassAttrExportRule_GCOnly = (1 << 2),	            
};

class CEconItemAttributeDefinition : public IEconItemAttributeDefinition
{
public:
	CEconItemAttributeDefinition(void);
	CEconItemAttributeDefinition(const CEconItemAttributeDefinition& that);
	CEconItemAttributeDefinition& operator=(const CEconItemAttributeDefinition& rhs);

	~CEconItemAttributeDefinition(void);

	bool	BInitFromKV(KeyValues* pKVAttribute, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	virtual attrib_definition_index_t GetDefinitionIndex(void) const { return m_nDefIndex; }
	virtual const char* GetDefinitionName(void) const { return m_pszDefinitionName; }

	virtual KeyValues* GetRawDefinition(void) const { return m_pKVAttribute; }

	bool		IsHidden(void) const { return m_bHidden; }
	bool		BForceWebSchemaOutput(void) const { return m_bWebSchemaOutputForced; }
	bool		IsStoredAsInteger(void) const { return m_bStoredAsInteger; }
	bool		IsStoredAsFloat(void) const { return !m_bStoredAsInteger; }
	bool		IsInstanceData() const { return m_bInstanceData; }
	EAssetClassAttrExportRule_t GetAssetClassAttrExportRule() const { return m_eAssetClassAttrExportRule; }
	uint32		GetAssetClassBucket() const { return m_unAssetClassBucket; }
	int			GetDescriptionFormat(void) const { return m_iDescriptionFormat; }
	int			GetScore(void) const { return m_iScore; }
	virtual const char* GetDescriptionString(void) const { return m_pszDescriptionString; }
	const char* GetArmoryDescString(void) const { return m_pszArmoryDesc; }
	virtual const char* GetAttributeClass(void) const { return m_pszAttributeClass; }
	attrib_effect_types_t GetEffectType(void) const { return m_iEffectType; }

	const class ISchemaAttributeType* GetAttributeType(void) const { return m_pAttrType; }

#ifndef GC_DLL
	void		ClearStringCache(void) const { m_iszAttributeClass = NULL_STRING; }
	string_t	GetCachedClass(void) const
	{
		if (m_iszAttributeClass == NULL_STRING && m_pszAttributeClass)
		{
			m_iszAttributeClass = AllocPooledString(m_pszAttributeClass);
		}
		return m_iszAttributeClass;
	}
#endif

private:
	KeyValues* m_pKVAttribute;

	attrib_definition_index_t	m_nDefIndex;

	const class ISchemaAttributeType* m_pAttrType;

	bool		m_bHidden;

	bool		m_bWebSchemaOutputForced;

	bool		m_bStoredAsInteger;

	bool		m_bInstanceData;
	EAssetClassAttrExportRule_t	m_eAssetClassAttrExportRule;			             
	uint32		m_unAssetClassBucket;		              

	attrib_effect_types_t m_iEffectType;

	int			m_iDescriptionFormat;
	const char* m_pszDescriptionString;

	const char* m_pszDescriptionTag;

	const char* m_pszArmoryDesc;
	int			m_iScore;

	const char* m_pszDefinitionName;

	const char* m_pszAttributeClass;

#if defined(CLIENT_DLL) || defined(GAME_DLL) || defined(GC)
	mutable string_t	m_iszAttributeClass;	            
#endif

};

class CEconSoundMaterialDefinition
{
public:
	CEconSoundMaterialDefinition(void);
	CEconSoundMaterialDefinition(const CEconSoundMaterialDefinition& that);
	CEconSoundMaterialDefinition& operator=(const CEconSoundMaterialDefinition& rhs);

	~CEconSoundMaterialDefinition(void) { }

	bool		BInitFromKV(KeyValues* pKVItem, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	int32		GetID(void) const { return m_nID; }
	const char* GetName(void) const { return !m_strName.IsEmpty() ? m_strName.String() : "unknown"; }

	const char* GetStartDragSound(void) const { return m_strStartDragSound.Get(); }
	const char* GetEndDragSound(void) const { return m_strEndDragSound.Get(); }
	const char* GetEquipSound(void) const { return m_strEquipSound.Get(); }

private:

	int32		m_nID;

	CUtlString	m_strName;

	CUtlString  m_strStartDragSound;
	CUtlString  m_strEndDragSound;

	CUtlString  m_strEquipSound;
};


#define MAX_VISUALS_CUSTOM_SOUNDS		10


#if defined(CLIENT_DLL) || defined(GAME_DLL)
enum
{
	kAttachedModelDisplayFlag_WorldModel = 0x01,
	kAttachedModelDisplayFlag_ViewModel = 0x02,

	kAttachedModelDisplayFlag_MaskAll = kAttachedModelDisplayFlag_WorldModel | kAttachedModelDisplayFlag_ViewModel,
};

struct attachedmodel_t
{
	const char* m_pszModelName;
	int m_iModelDisplayFlags;
};

struct attachedparticle_t
{
	int m_iParticleIndex;
	int m_nStyle;
};

enum wearableanimplayback_t
{
	WAP_ON_SPAWN,				        
	WAP_START_BUILDING,			                
	WAP_STOP_BUILDING,			                

	NUM_WAP_TYPES,
};

struct animation_on_wearable_t
{
	int						iActivity;
	const char* pszActivity;
	wearableanimplayback_t	iPlayback;
	const char* pszReplacement;
	int						iReplacement;            
	const char* pszSequence;
	const char* pszRequiredItem;
	const char* pszScene;
	float					flFrequency;
};

struct sound_on_wearable_t
{
	const char* pszSound;
	const char* pszReplacement;
};

struct particle_on_wearable_t
{
	const char* pszParticle;
	const char* pszReplacement;
	bool					bFlyingCourierEffect;
};

struct particlesnapshot_on_wearable_t
{
	const char* pszParticleSnapshot;
	const char* pszReplacement;
};

struct particle_control_point_on_wearable_t
{
	const char* pszParticle;
	int						nParticleControlPoint;
	Vector					vecCPValue;
};

struct codecontrolledbodygroupdata_t
{
	const char* pFuncName;
	void* pFunc;
};

#endif    

struct style_unlock_info
{
	const char* pszItemName;
	int iPrice;
	int iStylePreReq;
	const char* pszAttrib;
	uint32 iAttribValue;

	bool IsLockable() const
	{
		return pszItemName || pszAttrib;
	}

	bool HasUnlockPrice() const
	{
		return iPrice && pszItemName;
	}

	bool HasUnlockAttrib() const
	{
		return iAttribValue && pszAttrib;
	}
};

class CEconStyleInfo
{
public:
	CEconStyleInfo()
	{
		m_iIndex = 0;
		m_iSkin = 0;
		m_iIcon = 0;
		m_pszName = NULL;
		m_pszBasePlayerModel = NULL;
		m_UnlockInfo.pszItemName = NULL;
		m_UnlockInfo.iPrice = 0;
		m_UnlockInfo.iStylePreReq = 0;
		m_UnlockInfo.pszAttrib = NULL;
		m_UnlockInfo.iAttribValue = 0;
	}

	virtual ~CEconStyleInfo()
	{
	}

	virtual void BInitFromKV(KeyValues* pKVItem, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors);

#if defined(CLIENT_DLL) || defined(GAME_DLL)
	virtual void GeneratePrecacheModelStringsForStyle(CUtlVector<const char*>* out_pVecModelStrings) const;
#endif

	int GetSkin() const
	{
		return m_iSkin;
	}

	int GetIndex() const { return m_iIndex; }
	const char* GetName() const { return m_pszName; }
	const char* GetBasePlayerDisplayModel() const { return m_pszBasePlayerModel; }
	const CUtlVector<const char*>& GetAdditionalHideBodygroups() const { return m_vecAdditionalHideBodygroups; }
	int GetIcon() const { return m_iIcon; }
	const style_unlock_info* GetUnlockInfo() const { return &m_UnlockInfo; }

protected:
	int m_iIndex;
	int m_iSkin;
	int m_iIcon;
	const char* m_pszName;
	const char* m_pszBasePlayerModel;
	CUtlVector<const char*> m_vecAdditionalHideBodygroups;
	style_unlock_info m_UnlockInfo;
};

struct courier_t
{
	const char* m_pszModelName;
	int m_iTeam;
};

struct announcer_preview_t
{
	CUtlString m_strFileName;
	CUtlString m_strCaption;
};

struct ability_icon_replacement_t
{
	CUtlString m_strAbilityName;
	CUtlString m_strReplacement;
};

enum EAssetModifier
{
	AM_Invalid = -1,
	AM_Activity,
	AM_Announcer,
	AM_AnnouncerPreview,
	AM_HudSkin,
	AM_AbilityName,
	AM_Sound,
	AM_Speech,
	AM_Particle,
	AM_ParticleSnapshot,
	AM_ParticleControlPoint,
	AM_EntityModel,
	AM_ViewModel,
	AM_EntityScale,
	AM_Icon,
	AM_AbilityIcon,
	AM_Courier,
	AM_CourierFlying,
	AM_HeroModelChange,
	AM_MAX
};

struct AssetModifier
{
	EAssetModifier	m_Type;
	CUtlString		m_strAsset;
	CUtlString		m_strModifier;
	float			m_flModifier;
	float			m_flFrequency;
	int				m_iStyle;
};

struct AssetInfo
{
	AssetInfo()
	{
#if defined(CLIENT_DLL) || defined(GAME_DLL)
		iHideParentBodyGroup = -1;

		m_ModifiedBodyGroupNames.SetLessFunc(StringLessThan);
		m_CodeControlledBodyGroupNames.SetLessFunc(StringLessThan);

		iSkin = 0;
		bUsePerClassBodygroups = false;
		m_pszMaterialOverride = NULL;
		m_pszParticleEffect = NULL;
		m_pszParticleSnapshot = NULL;
		for (int i = 0; i < MAX_VISUALS_CUSTOM_SOUNDS; i++)
		{
			m_pszCustomSounds[i] = NULL;
		}

		for (int i = 0; i < NUM_SHOOT_SOUND_TYPES; i++)
		{
			m_pszWeaponSoundReplacements[i] = NULL;
		}

		m_iViewModelBodyGroupOverride = -1;
		m_iViewModelBodyGroupStateOverride = -1;
		m_iWorldModelBodyGroupOverride = -1;
		m_iWorldModelBodyGroupStateOverride = -1;

		m_pszSpeechConcept = NULL;
		m_pszChatMessage = NULL;
		m_pszAnnouncerName.Clear();
		m_pszAnnouncerResource.Clear();
		m_pszEntityModel = NULL;
		m_pszViewModel = NULL;
		m_pszEntityClass = NULL;
		m_flScaleSize = 1.f;
		m_pszScaleClass = NULL;
		m_bSkipModelCombine = false;

		m_pszPrimaryAmmo = NULL;
		m_pszWeaponTypeString = NULL;
		m_pszAddonLocation = NULL;
		m_pszEjectBrassEffect = NULL;
		m_pszTracerEffect = NULL;
		m_pszMuzzleFlashEffect1stPerson = NULL;
		m_pszMuzzleFlashEffect1stPersonAlt = NULL;
		m_pszMuzzleFlashEffect3rdPerson = NULL;
		m_pszMuzzleFlashEffect3rdPersonAlt = NULL;
		m_pszHeatEffect = NULL;
		m_pszPlayerAnimationExtension = NULL;

		m_pszOriginalIcon = NULL;
		m_pszNewIcon = NULL;

		m_mapAssetModifiers.SetLessFunc(DefLessFunc(int));


#endif    
	}

	~AssetInfo()
	{
		m_Styles.PurgeAndDeleteElements();
	}

	void AddAssetModifier(AssetModifier* newMod);
	CUtlVector<AssetModifier*>* GetAssetModifiers(EAssetModifier type);
	const char* GetModifierByAsset(EAssetModifier type, const char* pszAsset, int iStyle = -1);
	const char* GetAssetByModifier(EAssetModifier type, const char* pszModifier, int iStyle = -1);

	CUtlMap< int, CUtlVector<AssetModifier*>* >	m_mapAssetModifiers;

#if defined(CLIENT_DLL) || defined(GAME_DLL)	
	int iHideParentBodyGroup;
	CUtlMap<const char*, int> m_ModifiedBodyGroupNames;         
	CUtlMap<const char*, codecontrolledbodygroupdata_t> m_CodeControlledBodyGroupNames;
	int			iSkin;
	bool		bUsePerClassBodygroups;
	CUtlVector<attachedmodel_t>	m_AttachedModels;
	CUtlVector<attachedparticle_t>	m_AttachedParticles;
	CUtlVector<animation_on_wearable_t> m_Animations;
	CUtlVector<sound_on_wearable_t> m_Sounds;
	CUtlVector<particle_on_wearable_t> m_Particles;
	CUtlVector<particlesnapshot_on_wearable_t> m_ParticleSnapshots;
	CUtlVector<particle_control_point_on_wearable_t> m_ParticleControlPoints;
	const char* m_pszCustomSounds[MAX_VISUALS_CUSTOM_SOUNDS];
	const char* m_pszMaterialOverride;
	const char* m_pszMuzzleFlash;
	const char* m_pszTracerEffect;
	const char* m_pszParticleEffect;
	const char* m_pszParticleSnapshot;
	const char* m_pszWeaponSoundReplacements[NUM_SHOOT_SOUND_TYPES];

	const char* m_pszPrimaryAmmo;
	const char* m_pszWeaponTypeString;
	const char* m_pszAddonLocation;
	const char* m_pszEjectBrassEffect;
	const char* m_pszMuzzleFlashEffect1stPerson;
	const char* m_pszMuzzleFlashEffect1stPersonAlt;
	const char* m_pszMuzzleFlashEffect3rdPerson;
	const char* m_pszMuzzleFlashEffect3rdPersonAlt;
	const char* m_pszHeatEffect;
	const char* m_pszPlayerAnimationExtension;

	int m_iViewModelBodyGroupOverride;
	int m_iViewModelBodyGroupStateOverride;
	int m_iWorldModelBodyGroupOverride;
	int m_iWorldModelBodyGroupStateOverride;
	bool m_bSkipModelCombine;
	CUtlVector<const char*>								m_vecAnimationModifiers;

	const char* m_pszSpeechConcept;
	const char* m_pszChatMessage;

	CUtlString m_pszAnnouncerName;
	CUtlString m_pszAnnouncerResource;
	CUtlVector< announcer_preview_t > m_vecAnnouncerPreview;

	CUtlString m_pszHudSkinName;

	CUtlString m_pszAbilityName;

	const char* m_pszEntityModel;
	const char* m_pszViewModel;
	const char* m_pszEntityClass;

	float		m_flScaleSize;
	const char* m_pszScaleClass;

	const char* m_pszOriginalIcon;
	const char* m_pszNewIcon;

	CUtlVector< ability_icon_replacement_t > m_vecAbilityIconReplacements;


#endif    

	CUtlVector<CEconStyleInfo*> m_Styles;
};

enum item_capabilities_t
{
	ITEM_CAP_NONE = 0,
	ITEM_CAP_PAINTABLE = 1 << 0,		            
	ITEM_CAP_NAMEABLE = 1 << 1,		            
	ITEM_CAP_DECODABLE = 1 << 2,		       
	ITEM_CAP_CAN_DELETE = 1 << 3,		       
	ITEM_CAP_CAN_CUSTOMIZE_TEXTURE = 1 << 4,	  
	ITEM_CAP_USABLE = 1 << 5,	  
	ITEM_CAP_USABLE_GC = 1 << 6,		            
	ITEM_CAP_CAN_GIFT_WRAP = 1 << 7,	  
	ITEM_CAP_USABLE_OUT_OF_GAME = 1 << 8,		            
	ITEM_CAP_CAN_COLLECT = 1 << 9,	  
	ITEM_CAP_CAN_CRAFT_COUNT = 1 << 10,	  
	ITEM_CAP_CAN_CRAFT_MARK = 1 << 11,	  
	ITEM_CAP_PAINTABLE_TEAM_COLORS = 1 << 12,	  
	ITEM_CAP_CAN_BE_RESTORED = 1 << 13,	  
	ITEM_CAP_CAN_USE_STRANGE_PARTS = 1 << 14,	  
	ITEM_CAP_PAINTABLE_UNUSUAL = 1 << 15,	  
	ITEM_CAP_CAN_INCREMENT = 1 << 16,	  
	ITEM_CAP_USES_ESSENCE = 1 << 17,	  
	ITEM_CAP_AUTOGRAPH = 1 << 18,	  
	ITEM_CAP_RECIPE = 1 << 19,	  
	ITEM_CAP_CAN_STICKER = 1 << 20,		          
	ITEM_CAP_STATTRACK_SWAP = 1 << 21,		      
	NUM_ITEM_CAPS = 22,
};

enum { ITEM_CAP_DEFAULT = ITEM_CAP_NONE };         
enum { ITEM_CAP_TOOL_DEFAULT = ITEM_CAP_NONE };	        

struct bundleinfo_t
{
	CUtlVector <item_list_entry_t > vecItemEntries;
};

#ifdef CLIENT_DLL
namespace vgui
{
	class Panel;
}
#endif  

class IEconConsumable
{

};

class IEconItemInterface;

class IEconTool
{
	friend class CEconSharedToolSupport;

public:
	IEconTool(const char* pszTypeName, const char* pszUseString, const char* pszUsageRestriction, item_capabilities_t unCapabilities, KeyValues* pUsageKV)
		: m_pszTypeName(pszTypeName)
		, m_pszUseString(pszUseString)
		, m_pszUsageRestriction(pszUsageRestriction)
		, m_unCapabilities(unCapabilities)
	{
		if (pUsageKV)
		{
			KeyValues* pBonusItemDefs = pUsageKV->FindKey("bonus_itemdefs");
			if (pBonusItemDefs)
			{
				FOR_EACH_SUBKEY(pBonusItemDefs, pBonusItemDef)
				{
					m_vecBonusItemDef.AddToTail(atoi(pBonusItemDef->GetName()));
				}
			}
		}
	}

	virtual ~IEconTool() { }

	const char* GetUsageRestriction() const { return m_pszUsageRestriction; }
	item_capabilities_t GetCapabilities() const { return m_unCapabilities; }

	virtual bool CanApplyTo(const IEconItemInterface* pTool, const IEconItemInterface* pToolSubject) const { Assert(pTool); Assert(pToolSubject); return true; }
	virtual bool ShouldDisplayQuantity(const IEconItemInterface* pTool) const;
	virtual bool RequiresToolEscrowPeriod() const { return false; }

	virtual bool IsValid() const { return true; }

	const char* GetTypeName() const { return m_pszTypeName; }		                       
	const char* GetUseString() const { return m_pszUseString; }

	int GetBonusItemDefCount(void) const { return m_vecBonusItemDef.Count(); }
	int GetBonusItemDef(int i) const { return m_vecBonusItemDef[i]; }

	virtual IEconConsumable* CreateEconConsumable() const
	{
		Assert(!"IEconTool::CreateEconConsumable(): unimplemented call!");
		return NULL;
	}

#ifdef CLIENT_DLL
	virtual bool ShouldShowContainedItemPanel(const IEconItemInterface* pItem) const { Assert(!"IEconTool::ShouldShowContainedItemPanel(): we don't expect this to be called on anything besides gifts!"); return false; }
	virtual const char* GetUseCommandLocalizationToken(const IEconItemInterface* pItem, const char* pszDefault = "#ApplyOnItem") const;

	virtual void OnClientUseConsumable(class C_EconItemView* pItem) const
	{
		Assert(!"IEconTool::OnClientUseConsumable(): unimplemented call!");
	}

	virtual void OnClientApplyTool(class C_EconItemView* pTool, class C_EconItemView* pSubject) const
	{
		Assert(!"IEconTool::OnClientApplyTool(): unimplemented call!");
	}
	virtual void OnClientApplyCommit(class C_EconItemView* pTool, class C_EconItemView* pSubject) const
	{
		Assert(!"IEconTool::OnClientApplyCommit(): unimplemented call!");
	}
#endif  

private:
	const char* m_pszTypeName;
	const char* m_pszUseString;
	const char* m_pszUsageRestriction;
	item_capabilities_t m_unCapabilities;
	CUtlVector<int>	m_vecBonusItemDef;
};

enum EItemType
{
	k_EItemTypeNone,
	k_EItemTypeCoupon,
	k_EItemTypeCampaign,
	k_EItemTypeSelfOpeningPurchase,
	k_EItemTypeOperationCoin,
	k_EItemTypePrestigeCoin,
	k_EItemTypeTool,
};
const char* PchNameFromEItemType(EItemType eType);

enum EItemSlot
{
	k_EItemSlotNone,
	k_EItemSlotMelee,
	k_EItemSlotSecondary,
	k_EItemSlotSMG,
	k_EItemSlotRifle,
	k_EItemSlotHeavy,
	k_EItemSlotFlair,
	k_EItemSlotMusicKit,
};
const char* PchNameFromEItemSlot(EItemSlot eSlot);
EItemSlot EItemSlotFromName(const char* pchName);

class CEconItemDefinition : public IEconItemDefinition
{
public:
	CEconItemDefinition(void);
	virtual ~CEconItemDefinition(void);

	void PurgeStaticAttributes(void);

public:
	virtual bool	BInitFromKV(KeyValues* pKVItem, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);
#if defined(CLIENT_DLL) || defined(GAME_DLL)
	virtual bool	BInitFromTestItemKVs(int iNewDefIndex, KeyValues* pKVItem, CEconItemSchema& pschema);
	virtual void	GeneratePrecacheModelStrings(bool bDynamicLoad, CUtlVector<const char*>* out_pVecModelStrings) const;
	virtual void	GeneratePrecacheSoundStrings(CUtlVector<const char*>* out_pVecSoundStrings) const;
	virtual void	GeneratePrecacheEffectStrings(CUtlVector<const char*>* out_pVecEffectStrings) const;
#endif
	virtual void	CopyPolymorphic(const CEconItemDefinition* pSourceDef) { *this = *pSourceDef; }

	bool		BInitItemMappings(CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors);

	virtual void BInitVisualBlockFromKV(KeyValues* pKVItem, IEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);
	void		BInitStylesBlockFromKV(KeyValues* pKVStyles, CEconItemSchema& pschema, AssetInfo* pAssetInfo, CUtlVector<CUtlString>* pVecErrors);
	bool		BInitAlternateIconsFromKV(KeyValues* pKVAlternateIcons, CEconItemSchema& pschema, AssetInfo* pAssetInfo, CUtlVector<CUtlString>* pVecErrors);

	virtual item_definition_index_t	GetDefinitionIndex(void) const { return m_nDefIndex; }
	const CUtlVector<item_definition_index_t>& GetAssociatedItemsDefIndexes() const { return m_nAssociatedItemsDefIndexes; }
	virtual const char* GetPrefabName(void) const { return m_szPrefab; }
	bool		BEnabled(void) const { return m_bEnabled; }
	bool		BLoadOnDemand(void) const { return m_bLoadOnDemand; }
	bool		BHasBeenLoaded(void) const { return m_bHasBeenLoaded; }
	const char* GetDefinitionName(void) const { return m_pszDefinitionName; }
	const char* GetItemClass(void) const { return m_pszItemClassname; }
	virtual const char* GetItemBaseName(void) const { return m_pszItemBaseName; }
	const char* GetBrassModelOverride(void) const { return m_pszBrassModelOverride; }
	virtual const char* GetItemTypeName(void) const { return m_pszItemTypeName; }
	virtual uint32 GetItemTypeID(void) const { return m_unItemTypeID; }
	uint8		GetMinLevel(void) const { return m_unMinItemLevel; }
	uint8		GetMaxLevel(void) const { return m_unMaxItemLevel; }
	virtual uint8	GetRarity(void) const { return m_nItemRarity; }
	uint8		GetQuality(void) const { return m_nItemQuality; }
	uint8		GetForcedQuality(void) const { return m_nForcedItemQuality; }
	uint8		GetDefaultDropQuality(void) const { return m_nDefaultDropItemQuality; }
	uint8		GetDefaultDropQuantity(void) const { return m_nDefaultDropQuantity; }
	KeyValues* GetRawDefinition(void) const { return m_pKVItem; }
	const CUtlVector<static_attrib_t>& GetStaticAttributes(void) const { return m_vecStaticAttributes; }
	uint32		GetNumConcreteItems() const { return m_unNumConcreteItems; }
	int			GetSoundMaterialID() const { return m_nSoundMaterialID; }

	bool		IsHidden(void) const { return m_bHidden; }
	bool		IsImported(void) const { return m_bImported; }
	bool		IsOnePerAccountCDKEY(void) const { return m_bOnePerAccountCDKEY; }
	bool		IsAllowedInMatch(void) const { return m_bAllowedInThisMatch; }
	virtual bool IsBaseItem(void) const { return m_bBaseItem; }
	virtual bool IsDefaultSlotItem(void) const { return m_bDefaultSlotItem; }
	virtual bool IsBundle(void) const { return m_BundleInfo != NULL; }
	bool		HasProperName(void) const { return m_bProperName; }
	const char* GetClassToken(void) const { return m_pszClassToken; }
	const char* GetSlotToken(void) const { return m_pszSlotToken; }
	bool		ShouldAttachToHands(void) const { return m_bAttachToHands; }
	bool		ShouldAttachToHandsVMOnly(void) const { return m_bAttachToHandsVMOnly; }
	bool		ShouldFlipViewmodels(void) const { return m_bFlipViewModel; }
	int			GetInventoryImagePosition(int iIndex) const { Assert(iIndex >= 0 && iIndex < 2); return m_iInventoryImagePosition[iIndex]; }
	int			GetInventoryImageSize(int iIndex) const { Assert(iIndex >= 0 && iIndex < 2); return m_iInventoryImageSize[iIndex]; }
	int			GetDropType(void) const { return m_iDropType; }
	const char* GetHolidayRestriction(void) const { return m_pszHolidayRestriction; }
	int			GetSubType(void) const { return m_iSubType; }
	item_capabilities_t GetCapabilities(void) const { return m_iCapabilities; }
	void		AddItemSet(int nIndex);
	const CUtlVector< int >& GetItemSets(void) const;
	int			GetArmoryRemap(void) const { return m_iArmoryRemap; }
	int			GetStoreRemap(void) const { return m_iStoreRemap; }
	virtual int	GetLoadoutSlot(void) const { return 0; }
	virtual int	GetHeroID(void) const { return 0; }
	KeyValues* GetPortraitsKV(void) const { return m_pPortraitsKV; }
	KeyValues* GetPortraitKVForModel(const char* pszModelName) const;

	AssetInfo* GetAssetInfo() const { return m_pAssetInfo; }

	bool IsTool() const { return m_pTool != NULL; }
	bool IsToolAndNotACrate() const { return (IsTool() && GetEconTool() && V_strcmp(GetEconTool()->GetTypeName(), "supply_crate") != 0); }
	const IEconTool* GetEconTool() const { return m_pTool; }
	template < class T >
	const T* GetTypedEconTool() const { return dynamic_cast<const T*>(GetEconTool()); }

	virtual bool IsPreviewableInStore() const { return true; }

	const bundleinfo_t* GetBundleInfo(void) const { return m_BundleInfo; }
	virtual int GetBundleItemCount(void) const { return m_BundleInfo ? m_BundleInfo->vecItemEntries.Count() : 0; }
	virtual int GetBundleItem(int iIndex) const { return m_BundleInfo ? m_BundleInfo->vecItemEntries[iIndex].m_nItemDef : -1; }
	virtual int GetBundleItemPaintKitID(int iIndex) const { return m_BundleInfo ? m_BundleInfo->vecItemEntries[iIndex].m_nPaintKit : -1; }

	const CUtlVector< const CEconItemDefinition* >& GetContainingBundles() const { return m_vecContainingBundleItemDefs; }
	uint32 GetContainingBundleCount() const { return m_vecContainingBundleItemDefs.Count(); }

	typedef CUtlVector< uint32 > WorkshopContributorList_t;

	void AddSteamWorkshopContributor(uint32 unAccountID) { if (m_vecSteamWorkshopContributors.InvalidIndex() == m_vecSteamWorkshopContributors.Find(unAccountID)) { m_vecSteamWorkshopContributors.AddToTail(unAccountID); } }
	const WorkshopContributorList_t& GetSteamWorkshopContributors() const { return m_vecSteamWorkshopContributors; }
	bool BIsSteamWorkshopItem() const { return m_vecSteamWorkshopContributors.Count() > 0; }

	const char* GetIconClassname(void) const { return m_pszItemIconClassname; }
	const char* GetLogClassname(void) const { return m_pszItemLogClassname; }
	const char* GetInventoryModel(void) const { return m_pszInventoryModel; }
	virtual const char* GetInventoryImage(void) const { return m_pszInventoryImage; }
	const char* GetInventoryOverlayImage(int idx) const { if (m_pszInventoryOverlayImages.IsValidIndex(idx)) return m_pszInventoryOverlayImages[idx]; else return NULL; }
	int			GetInventoryOverlayImageCount(void) const { return m_pszInventoryOverlayImages.Count(); }
	const char* GetIconURLSmall() const { return m_sIconURLSmall; }
	const char* GetIconURLLarge() const { return m_sIconURLLarge; }
	void	SetIconURLSmall(const char* szURL) { m_sIconURLSmall = szURL; }
	void	SetIconURLLarge(const char* szURL) { m_sIconURLLarge = szURL; }
	virtual const char* GetBasePlayerDisplayModel() const { return m_pszBaseDisplayModel; }
	virtual const char* GetWorldDisplayModel()	const { return m_pszWorldDisplayModel; }
	virtual const char* GetWorldDroppedModel()	const { return m_pszWorldDroppedModel; }

	virtual const char* GetIconDisplayModel()	const;
	virtual const char* GetBuyMenuDisplayModel()	const;
	virtual const char* GetPedestalDisplayModel()	const;
	virtual const char* GetMagazineModel()	const;
	virtual const char* GetScopeLensMaskModel()	const;
	virtual const char* GetUidModel() const;
	virtual const char* GetStatTrakModelByType(uint32 nType) const;

	virtual const int	 GetNumSupportedStickerSlots() const;
	virtual const char* GetStickerSlotModelBySlotIndex(uint32 nIndex) const;
	virtual const Vector& GetStickerSlotWorldProjectionStartBySlotIndex(uint32 nIndex) const;
	virtual const Vector& GetStickerSlotWorldProjectionEndBySlotIndex(uint32 nIndex) const;
	virtual const char* GetStickerWorldModelBoneParentNameBySlotIndex(uint32 nIndex) const;
	virtual const char* GetStickerSlotMaterialBySlotIndex(uint32 nIndex) const;
	virtual const char* GetIconDefaultImage() const { return m_pszIconDefaultImage; }
	virtual const char* GetExtraWearableModel(void) const { return m_pszWorldExtraWearableModel; }
	virtual const char* GetParticleFile(void) const { return m_pszParticleFile; }
	virtual const char* GetParticleSnapshotFile(void) const { return m_pszParticleSnapshotFile; }
	const CUtlVector< WeaponPaintableMaterial_t >* GetPaintData(void) const { return &m_PaintData; }
	const InventoryImageData_t* GetInventoryImageData(void) const { return m_pInventoryImageData; }
	virtual const char* GetItemDesc(void) const { return m_pszItemDesc; }
	const char* GetArmoryDescString(void) const { return m_pszArmoryDesc; }
	RTime32		GetExpirationDate(void) const { return m_rtExpiration; }
	RTime32		GetDefCreationDate(void) const { return m_rtDefCreation; }
	bool		ShouldShowInArmory(void) const { return m_bShouldShowInArmory; }
	bool		IsActingAsAWearable(void) const { return m_bActAsWearable; }
	bool		GetHideBodyGroupsDeployedOnly(void) const { return m_bHideBodyGroupsDeployedOnly; }
	virtual bool	IsPackBundle(void) const { return m_bIsPackBundle; }
	virtual bool	IsPackItem(void) const { return NULL != m_pOwningPackBundle; }
	CEconItemDefinition* GetOwningPackBundle() { return m_pOwningPackBundle; }
	const CEconItemDefinition* GetOwningPackBundle() const { return m_pOwningPackBundle; }
#if ECONITEM_DATABASE_AUDIT_TABLES_FEATURE
	const char* GetDatabaseAuditTableName(void) const { return m_pszDatabaseAuditTable; }
#endif
	const char* GetAlternateIcon(int iAlternateIcon) const;

	equip_region_mask_t GetEquipRegionMask(void) const { return m_unEquipRegionMask; }
	equip_region_mask_t GetEquipRegionConflictMask(void) const { return m_unEquipRegionConflictMask; }

	void		SetAllowedInMatch(bool bAllowed) { m_bAllowedInThisMatch = bAllowed; }
	void		SetHasBeenLoaded(bool bLoaded) { m_bHasBeenLoaded = bLoaded; }

	bool		BRandomProxy(void) const { return NULL != m_pProxyCriteria; }
	CItemSelectionCriteria* PProxyCriteria(void) const { return m_pProxyCriteria; }

	uint32		RollItemLevel(void) const;

	const char* GetFirstSaleDate(void) const;

	virtual bool	IsRecent(void) const { return false; }

	void		IterateAttributes(class IEconItemAttributeIterator* pIterator) const;

#if defined(CLIENT_DLL) || defined(GAME_DLL)
	int						GetNumAttachedModels() const;
	attachedmodel_t* GetAttachedModelData(int iIdx) const;
	int						GetNumAttachedParticles() const;
	attachedparticlesystem_t* GetAttachedParticleData(int iIdx) const;
	bool					IsAttachedParticleDataValidForStyle(int iIdx, int nStyle) const;
	int						GetNumAnimations() const;
	animation_on_wearable_t* GetAnimationData(int iIdx) const;
	int						GetNumAnimationModifiers() const;
	const char* GetAnimationModifier(int iIdx) const;

	Activity				GetActivityOverride(Activity baseAct) const;
	const char* GetReplacementForActivityOverride(Activity baseAct) const;
	int						GetNumSounds() const;
	sound_on_wearable_t* GetSoundData(int iIdx) const;
	const char* GetReplacementSound(const char* pszSoundName) const;
	int						GetNumParticles() const;
	particle_on_wearable_t* GetParticleData(int iIdx) const;
	const char* GetReplacementParticleEffect(const char* pszParticleName) const;
	int						GetNumParticleSnapshots() const;
	particlesnapshot_on_wearable_t* GetParticleSnapshotData(int iIdx) const;
	const char* GetReplacementParticleSnapshot(const char* pszParticleSnapshotName) const;

	int						GetNumParticleControlPoints() const;
	particle_control_point_on_wearable_t* GetParticleControlPointData(int iIdx) const;

	bool					GetReplacementControlPoint(int nIndex, const char* pszParticleName, int& nOutputCP, Vector& nCPValue) const;

	virtual bool			IsContentStreamable() const;

	const char* GetAnnouncerName() const;
	const char* GetAnnouncerResource() const;

	const char* GetEntityOverrideModel() const;
	const char* GetViewOverrideModel() const;
	const char* GetEntityOverrideClass() const;

	const char* GetHeroModelChangeModel() const;

	const char* GetUIOverrideIcon() const;
	const char* GetUIOverrideOriginalIcon() const;

	const char* GetUIOverrideAbilityIcon(const char* pszAbilityName) const;

	const char* GetScaleOverrideClass() const;
	float					GetScaleOverrideSize() const;

	bool					SkipModelCombine(void) const;

	const char* GetMuzzleFlash() const;
	const char* GetTracerEffect() const;
	const char* GetParticleEffect() const;
	const char* GetParticleSnapshot() const;
	const char* GetMaterialOverride() const;
	const char* GetCustomSound(int iSound) const;
	const char* GetWeaponReplacementSound(  int iSound) const;

	const char* GetPrimaryAmmo(void) const;
	const char* GetWeaponTypeString(void) const;
	const char* GetAddonLocation(void) const;
	const char* GetEjectBrassEffect(void) const;
	const char* GetMuzzleFlashEffect1stPerson(void) const;
	const char* GetMuzzleFlashEffect1stPersonAlt(void) const;
	const char* GetMuzzleFlashEffect3rdPerson(void) const;
	const char* GetMuzzleFlashEffect3rdPersonAlt(void) const;
	const char* GetHeatEffect(void) const;
	const char* GetPlayerAnimationExtension(void) const;

	int						GetHiddenParentBodygroup() const;
	int						GetNumModifiedBodyGroups() const;
	const char* GetModifiedBodyGroup(int i, int& body) const;
	bool					UsesPerClassBodygroups() const;
	int						GetNumCodeControlledBodyGroups() const;
	const char* GetCodeControlledBodyGroup(int i, struct codecontrolledbodygroupdata_t& ccbgd) const;

	int						GetViewmodelBodygroupOverride() const;
	int						GetViewmodelBodygroupStateOverride() const;
	int						GetWorldmodelBodygroupOverride() const;
	int						GetWorldmodelBodygroupStateOverride() const;

#endif    

	style_index_t			GetNumStyles() const;
	const CEconStyleInfo* GetStyleInfo(style_index_t unStyle) const;


	int						GetPopularitySeed() const { return m_nPopularitySeed; }

	bool					HasEconTag(econ_tag_handle_t tag) const { return m_vecTags.IsValidIndex(m_vecTags.Find(tag)); }

	bool					ShoulDisableStyleSelector(void) const { return m_bDisableStyleSelection; }

#if defined(CLIENT_DLL) || defined(GAME_DLL)
	int						GetStyleSkin(style_index_t unStyle) const;
#endif    

	virtual bool			IsPublicItem(void) const { return m_bPublicItem; }
	virtual bool			IgnoreInCollectionView(void) const { return m_bIgnoreInCollectionView; }

	const char* GetLootListName(void) const { return m_pszLootListName; }


	EItemType GetItemType() const { return m_eItemType; }
	bool GetAllowPurchaseStandalone() const { return m_bAllowPurchaseStandalone; }

private:
	KeyValues* m_pKVItem;

	item_definition_index_t	m_nDefIndex;

	CUtlVector< item_definition_index_t > m_nAssociatedItemsDefIndexes;

	bool		m_bEnabled;

	const char* m_szPrefab;

	uint8		m_unMinItemLevel;
	uint8		m_unMaxItemLevel;

	uint8		m_nItemRarity;

	uint8		m_nItemQuality;
	uint8		m_nForcedItemQuality;
	uint8		m_nDefaultDropItemQuality;

	uint8		m_nDefaultDropQuantity;

	CUtlVector<static_attrib_t> m_vecStaticAttributes;

	uint8		m_nPopularitySeed;

	KeyValues* m_pPortraitsKV;

	const char* m_pszItemBaseName;
	bool			m_bProperName;		                
	const char* m_pszItemTypeName;

	uint32			m_unItemTypeID;

	const char* m_pszItemDesc;

	RTime32			m_rtExpiration;

	RTime32			m_rtDefCreation;

	const char* m_pszInventoryModel;
	const char* m_pszInventoryImage;
	CUtlVector<const char*>	m_pszInventoryOverlayImages;
	int				m_iInventoryImagePosition[2];
	int				m_iInventoryImageSize[2];

	const char* m_pszBaseDisplayModel;
	bool			m_bLoadOnDemand;
	bool			m_bHasBeenLoaded;

	bool			m_bHideBodyGroupsDeployedOnly;

	const char* m_pszWorldDisplayModel;
	const char* m_pszWorldExtraWearableModel;		          

	const char* m_pszWorldDroppedModel;
	char m_szWorldDroppedModel[80];

	CUtlVector<StickerData_t>  m_vStickerModels;

	const char* m_pszIconDefaultImage;

	bool			m_bAttachToHands;
	bool			m_bAttachToHandsVMOnly;

	bool			m_bFlipViewModel;

	bool			m_bActAsWearable;

	mutable CUtlVector< int > m_iItemSets;

	AssetInfo* m_pAssetInfo;

	EItemType		m_eItemType;
	bool			m_bAllowPurchaseStandalone;

	const char* m_pszBrassModelOverride;

	IEconTool* m_pTool;
	bundleinfo_t* m_BundleInfo;
	item_capabilities_t m_iCapabilities;

	uint32			m_unNumConcreteItems;		                                      

	int				m_nSoundMaterialID;

	bool			m_bDisableStyleSelection;

	CUtlString		m_sIconURLSmall;
	CUtlString		m_sIconURLLarge;

	const char* m_pszParticleFile;				         
	const char* m_pszParticleSnapshotFile;		          

	const char* m_pszLootListName;				               

	CUtlVector<int> m_nCharacterPaintKitIndices;

protected:
	CUtlVector< WeaponPaintableMaterial_t > m_PaintData;
	InventoryImageData_t* m_pInventoryImageData;

private:

	const char* m_pszItemClassname;

	const char* m_pszItemLogClassname;

	const char* m_pszItemIconClassname;

	const char* m_pszDefinitionName;

#if ECONITEM_DATABASE_AUDIT_TABLES_FEATURE
	const char* m_pszDatabaseAuditTable;
#endif

	bool			m_bHidden;
	bool			m_bShouldShowInArmory;
	bool			m_bBaseItem;
	bool			m_bDefaultSlotItem;
	bool			m_bImported;
	bool			m_bOnePerAccountCDKEY;

	bool			m_bIsPackBundle;

	CEconItemDefinition* m_pOwningPackBundle;

	const char* m_pszArmoryDesc;

	int				m_iArmoryRemap;
	int				m_iStoreRemap;
	const char* m_pszArmoryRemap;
	const char* m_pszStoreRemap;

	const char* m_pszClassToken;
	const char* m_pszSlotToken;

	int				m_iDropType;

	const char* m_pszHolidayRestriction;

	int				m_iSubType;

	bool			m_bAllowedInThisMatch;

	CItemSelectionCriteria* m_pProxyCriteria;

	equip_region_mask_t	m_unEquipRegionMask;			        
	equip_region_mask_t m_unEquipRegionConflictMask;	             

	CUtlMap<uint32, const char*>* m_pMapAlternateIcons;

	CUtlVector<econ_tag_handle_t>	m_vecTags;
	CUtlVector<const CEconItemDefinition*> m_vecContainingBundleItemDefs;	          
	WorkshopContributorList_t m_vecSteamWorkshopContributors;

	friend class CEconItemSchema;
	bool			m_bPublicItem;
	bool			m_bIgnoreInCollectionView;
};

inline style_index_t CEconItemDefinition::GetNumStyles() const
{
	const AssetInfo* pAssetInfo = GetAssetInfo();

	if (!pAssetInfo)
		return 0;

	return pAssetInfo->m_Styles.Count();
}

inline const CEconStyleInfo* CEconItemDefinition::GetStyleInfo(style_index_t unStyle) const
{
	const AssetInfo* pAssetInfo = GetAssetInfo();
	if (!pAssetInfo || !pAssetInfo->m_Styles.IsValidIndex(unStyle))
		return NULL;

	return pAssetInfo->m_Styles[unStyle];
}

#if defined(CLIENT_DLL) || defined(GAME_DLL)
inline int CEconItemDefinition::GetNumAttachedModels() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_AttachedModels.Count();
}

inline attachedmodel_t* CEconItemDefinition::GetAttachedModelData(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	Assert(iIdx < GetAssetInfo()->m_AttachedModels.Count());
	if (iIdx >= GetAssetInfo()->m_AttachedModels.Count())
		return NULL;

	return &GetAssetInfo()->m_AttachedModels[iIdx];
}

inline int CEconItemDefinition::GetNumAnimations() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_Animations.Count();
}

inline animation_on_wearable_t* CEconItemDefinition::GetAnimationData(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	Assert(iIdx < GetAssetInfo()->m_Animations.Count());
	if (iIdx < 0 || iIdx >= GetAssetInfo()->m_Animations.Count())
		return NULL;

	return &GetAssetInfo()->m_Animations[iIdx];
}

inline int CEconItemDefinition::GetNumAnimationModifiers() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_vecAnimationModifiers.Count();
}

inline const char* CEconItemDefinition::GetAnimationModifier(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	if (iIdx < 0 || iIdx >= GetAssetInfo()->m_vecAnimationModifiers.Count())
		return NULL;

	return GetAssetInfo()->m_vecAnimationModifiers[iIdx];
}

inline const char* CEconItemDefinition::GetAnnouncerName() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszAnnouncerName;
}

inline const char* CEconItemDefinition::GetAnnouncerResource() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszAnnouncerResource;
}

inline const char* CEconItemDefinition::GetUIOverrideIcon() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszNewIcon;
}

inline const char* CEconItemDefinition::GetUIOverrideOriginalIcon() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszOriginalIcon;
}

inline const char* CEconItemDefinition::GetUIOverrideAbilityIcon(const char* pszAbilityName) const
{
	if (!pszAbilityName)
		return NULL;

	if (!GetAssetInfo())
		return NULL;

	FOR_EACH_VEC(GetAssetInfo()->m_vecAbilityIconReplacements, i)
	{
		ability_icon_replacement_t* pszAbilityReplacement = &GetAssetInfo()->m_vecAbilityIconReplacements.Element(i);
		if (pszAbilityReplacement)
		{
			if (!Q_strcmp(pszAbilityName, pszAbilityReplacement->m_strAbilityName))
			{
				return pszAbilityReplacement->m_strReplacement;
			}
		}
	}

	return NULL;
}

inline const char* CEconItemDefinition::GetEntityOverrideModel() const
{
	if (!GetAssetInfo())
		return NULL;

	CUtlVector<AssetModifier*>* pAssetModifierList = GetAssetInfo()->GetAssetModifiers(AM_EntityModel);
	if (pAssetModifierList && pAssetModifierList->Count() > 0)
		return pAssetModifierList->Element(0)->m_strModifier.String();

	return NULL;
}

inline const char* CEconItemDefinition::GetViewOverrideModel() const
{
	if (!GetAssetInfo())
		return NULL;

	CUtlVector<AssetModifier*>* pAssetModifierList = GetAssetInfo()->GetAssetModifiers(AM_ViewModel);
	if (pAssetModifierList && pAssetModifierList->Count() > 0)
		return pAssetModifierList->Element(0)->m_strModifier.String();

	return NULL;
}

inline const char* CEconItemDefinition::GetEntityOverrideClass() const
{
	if (!GetAssetInfo())
		return NULL;

	CUtlVector<AssetModifier*>* pAssetModifierList = GetAssetInfo()->GetAssetModifiers(AM_EntityModel);
	if (pAssetModifierList && pAssetModifierList->Count() > 0)
		return pAssetModifierList->Element(0)->m_strAsset.String();

	return NULL;
}

inline const char* CEconItemDefinition::GetScaleOverrideClass() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszScaleClass;
}

inline const char* CEconItemDefinition::GetHeroModelChangeModel() const
{
	if (!GetAssetInfo())
		return NULL;

	CUtlVector<AssetModifier*>* pAssetModifierList = GetAssetInfo()->GetAssetModifiers(AM_HeroModelChange);
	if (pAssetModifierList && pAssetModifierList->Count() > 0)
		return pAssetModifierList->Element(0)->m_strModifier.String();

	return NULL;
}

inline float CEconItemDefinition::GetScaleOverrideSize() const
{
	if (!GetAssetInfo())
		return 0.0f;

	return GetAssetInfo()->m_flScaleSize;
}

inline bool CEconItemDefinition::SkipModelCombine(void) const
{
	if (!GetAssetInfo())
		return false;

	return GetAssetInfo()->m_bSkipModelCombine;
}

inline int CEconItemDefinition::GetNumSounds() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_Sounds.Count();
}

inline sound_on_wearable_t* CEconItemDefinition::GetSoundData(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	Assert(iIdx < GetAssetInfo()->m_Sounds.Count());
	if (iIdx >= GetAssetInfo()->m_Sounds.Count())
		return NULL;

	return &GetAssetInfo()->m_Sounds[iIdx];
}

inline int CEconItemDefinition::GetNumParticles() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_Particles.Count();
}

inline particle_on_wearable_t* CEconItemDefinition::GetParticleData(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	Assert(iIdx < GetAssetInfo()->m_Particles.Count());
	if (iIdx >= GetAssetInfo()->m_Particles.Count())
		return NULL;

	return &GetAssetInfo()->m_Particles[iIdx];
}

inline int CEconItemDefinition::GetNumParticleSnapshots() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_ParticleSnapshots.Count();
}

inline particlesnapshot_on_wearable_t* CEconItemDefinition::GetParticleSnapshotData(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	Assert(iIdx < GetAssetInfo()->m_ParticleSnapshots.Count());
	if (iIdx >= GetAssetInfo()->m_ParticleSnapshots.Count())
		return NULL;

	return &GetAssetInfo()->m_ParticleSnapshots[iIdx];
}

inline int CEconItemDefinition::GetNumParticleControlPoints() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_ParticleControlPoints.Count();
}

inline particle_control_point_on_wearable_t* CEconItemDefinition::GetParticleControlPointData(int iIdx) const
{
	if (!GetAssetInfo())
		return NULL;

	Assert(iIdx < GetAssetInfo()->m_ParticleControlPoints.Count());
	if (iIdx >= GetAssetInfo()->m_ParticleControlPoints.Count())
		return NULL;

	return &GetAssetInfo()->m_ParticleControlPoints[iIdx];
}

inline int CEconItemDefinition::GetNumAttachedParticles() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_AttachedParticles.Count();
}

inline const char* CEconItemDefinition::GetMaterialOverride() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszMaterialOverride;
}


inline const char* CEconItemDefinition::GetMuzzleFlash() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszMuzzleFlash;
}

inline const char* CEconItemDefinition::GetTracerEffect() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszTracerEffect;
}

inline const char* CEconItemDefinition::GetParticleEffect() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszParticleEffect;
}

inline const char* CEconItemDefinition::GetParticleSnapshot() const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszParticleSnapshot;
}

inline int CEconItemDefinition::GetHiddenParentBodygroup() const
{
	if (!GetAssetInfo())
		return -1;

	return GetAssetInfo()->iHideParentBodyGroup;
}

inline int CEconItemDefinition::GetNumModifiedBodyGroups() const
{
	if (!GetAssetInfo())
		return -1;

	return GetAssetInfo()->m_ModifiedBodyGroupNames.Count();
}

inline const char* CEconItemDefinition::GetModifiedBodyGroup(int i, int& body) const
{
	if (!GetAssetInfo())
		return NULL;

	body = GetAssetInfo()->m_ModifiedBodyGroupNames[i];
	return GetAssetInfo()->m_ModifiedBodyGroupNames.Key(i);
}

inline int CEconItemDefinition::GetNumCodeControlledBodyGroups() const
{
	if (!GetAssetInfo())
		return -1;

	return GetAssetInfo()->m_CodeControlledBodyGroupNames.Count();
}

inline const char* CEconItemDefinition::GetCodeControlledBodyGroup(int i, codecontrolledbodygroupdata_t& ccbgd) const
{
	if (!GetAssetInfo())
		return NULL;

	ccbgd = GetAssetInfo()->m_CodeControlledBodyGroupNames[i];
	return GetAssetInfo()->m_CodeControlledBodyGroupNames.Key(i);
}

#if defined(CLIENT_DLL) || defined(GAME_DLL)
inline int CEconItemDefinition::GetStyleSkin(style_index_t unStyle) const
{
	const CEconStyleInfo* pStyle = GetStyleInfo(unStyle);

	return pStyle
		? pStyle->GetSkin()
		: -1;
}
#endif    

inline int CEconItemDefinition::GetViewmodelBodygroupOverride() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_iViewModelBodyGroupOverride;
}

inline int CEconItemDefinition::GetViewmodelBodygroupStateOverride() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_iViewModelBodyGroupStateOverride;
}

inline int CEconItemDefinition::GetWorldmodelBodygroupOverride() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_iWorldModelBodyGroupOverride;
}

inline int CEconItemDefinition::GetWorldmodelBodygroupStateOverride() const
{
	if (!GetAssetInfo())
		return 0;

	return GetAssetInfo()->m_iWorldModelBodyGroupStateOverride;
}

inline bool CEconItemDefinition::UsesPerClassBodygroups() const
{
	if (!GetAssetInfo())
		return false;

	return GetAssetInfo()->bUsePerClassBodygroups;
}

inline const char* CEconItemDefinition::GetCustomSound(int iSound) const
{
	if (!GetAssetInfo())
		return NULL;
	if (iSound < 0 || iSound >= MAX_VISUALS_CUSTOM_SOUNDS)
		return NULL;
	return GetAssetInfo()->m_pszCustomSounds[iSound];
}

inline const char* CEconItemDefinition::GetWeaponReplacementSound(    int iSound) const
{
	if (!GetAssetInfo())
		return NULL;
	if (iSound < 0 || iSound >= NUM_SHOOT_SOUND_TYPES)
		return NULL;
	return GetAssetInfo()->m_pszWeaponSoundReplacements[iSound];
}

inline const char* CEconItemDefinition::GetPrimaryAmmo(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszPrimaryAmmo;
}

inline const char* CEconItemDefinition::GetWeaponTypeString(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszWeaponTypeString;
}

inline const char* CEconItemDefinition::GetAddonLocation(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszAddonLocation;
}

inline const char* CEconItemDefinition::GetEjectBrassEffect(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszEjectBrassEffect;
}

inline const char* CEconItemDefinition::GetMuzzleFlashEffect1stPerson(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszMuzzleFlashEffect1stPerson;
}

inline const char* CEconItemDefinition::GetMuzzleFlashEffect1stPersonAlt(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszMuzzleFlashEffect1stPersonAlt;
}

inline const char* CEconItemDefinition::GetMuzzleFlashEffect3rdPerson(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszMuzzleFlashEffect3rdPerson;
}

inline const char* CEconItemDefinition::GetMuzzleFlashEffect3rdPersonAlt(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszMuzzleFlashEffect3rdPersonAlt;
}

inline const char* CEconItemDefinition::GetHeatEffect(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszHeatEffect;
}

inline const char* CEconItemDefinition::GetPlayerAnimationExtension(void) const
{
	if (!GetAssetInfo())
		return NULL;

	return GetAssetInfo()->m_pszPlayerAnimationExtension;
}

#endif    

class CTimedItemRewardDefinition
{
public:
	CTimedItemRewardDefinition(void);
	CTimedItemRewardDefinition(const CTimedItemRewardDefinition& that);
	CTimedItemRewardDefinition& operator=(const CTimedItemRewardDefinition& rhs);

	~CTimedItemRewardDefinition(void);

	bool		BInitFromKV(KeyValues* pKVTimedReward, CEconItemSchema& pschema, CUtlVector<CUtlString>* pVecErrors = NULL);

	uint32		GetRandomFrequency(void) const { return RandomFloat(m_unMinFreq, m_unMaxFreq); }
	uint32		GetMinFrequency(void) const { return m_unMinFreq; }
	uint32		GetMaxFrequency(void) const { return m_unMaxFreq; }

	float		GetChance(void) const;
	RTime32		GetForcedBaselineAdjustmentTime(void) const { return m_rtForcedBaselineAdjustment; }
	RTime32		GetForcedLastDropAdjustmentTime(void) const { return m_rtForcedLastDropTimeAdjustment; }
	uint32		GetHoursInRewardPeriod(void) const { return m_unHoursInRewardPeriod; }
	uint32		GetHoursBetweenDropsRealtime(void) const { return m_unHoursBetweenDropsRealtime; }
	uint32		GetTotalPointsForPeriod(float flHoursOfPlaytimeInPeriod) const
	{
		float flHours = flHoursOfPlaytimeInPeriod;
		if (flHours <= 0.0f)
			return 0;

		uint32 unPoints = 0;
		uint32 numPointsToProrate = 0;
		uint32 nHoursPlayed = (uint32)flHours;   
		if (!nHoursPlayed)
		{	         
			numPointsToProrate = (m_arrTotalPointsBasedOnHoursPlayed.Count()) ? m_arrTotalPointsBasedOnHoursPlayed.Head() : m_unPointsPerHourOverplayed;
		}
		else if (uint32 numHoursToCreditFromTable = MIN(nHoursPlayed, (uint32)m_arrTotalPointsBasedOnHoursPlayed.Count()))
		{	             
			nHoursPlayed -= numHoursToCreditFromTable;
			flHours -= numHoursToCreditFromTable;
			unPoints += m_arrTotalPointsBasedOnHoursPlayed[numHoursToCreditFromTable - 1];
			numPointsToProrate = (numHoursToCreditFromTable < (uint32)m_arrTotalPointsBasedOnHoursPlayed.Count())
				? (m_arrTotalPointsBasedOnHoursPlayed[numHoursToCreditFromTable] - m_arrTotalPointsBasedOnHoursPlayed[numHoursToCreditFromTable - 1])
				: m_unPointsPerHourOverplayed;
		}

		if (nHoursPlayed)
		{	             
			unPoints += nHoursPlayed * m_unPointsPerHourOverplayed;
			numPointsToProrate = m_unPointsPerHourOverplayed;
			flHours -= nHoursPlayed;
		}

		unPoints += flHours * numPointsToProrate;
		return unPoints;
	}
	uint32		GetPointsPerPeriodRollover(void) const { return m_unPointsPerPeriodRollover; }

	const CItemSelectionCriteria& GetCriteria(void) const { return m_criteria; }
	int GetLootListCount(void) const { return m_arrLootLists.Count(); }
	const CEconLootListDefinition* GetLootList(int iLootListIdx = 0) const { return m_arrLootLists.IsValidIndex(iLootListIdx) ? m_arrLootLists[iLootListIdx] : NULL; }
private:
	uint32		m_unMinFreq;
	uint32		m_unMaxFreq;

	RTime32		m_rtForcedBaselineAdjustment;	                  
	RTime32		m_rtForcedLastDropTimeAdjustment;	                   
	uint32		m_unHoursInRewardPeriod;		          
	uint32		m_unHoursBetweenDropsRealtime;	            
	CUtlVector< uint32 > m_arrTotalPointsBasedOnHoursPlayed;	                
	uint32		m_unPointsPerHourOverplayed;	              
	uint32		m_unPointsPerPeriodRollover;	      

	float		m_flChance;


	CItemSelectionCriteria m_criteria;
	CUtlVector< const CEconLootListDefinition* > m_arrLootLists;

	CUtlVector< const CEconLootListDefinition* > m_arrDynamicLootLists;
};

class CItemLevelingDefinition
{
public:
	CItemLevelingDefinition(void);
	CItemLevelingDefinition(const CItemLevelingDefinition& that);
	CItemLevelingDefinition& operator=(const CItemLevelingDefinition& rhs);

	~CItemLevelingDefinition(void);

	bool		BInitFromKV(KeyValues* pKVItemLevel, CEconItemSchema& pschema, const char* pszLevelBlockName, CUtlVector<CUtlString>* pVecErrors = NULL);

	uint32		GetLevel(void) const { return m_unLevel; }
	uint32		GetRequiredScore(void) const { return m_unRequiredScore; }
	const char* GetNameLocalizationKey(void) const { return m_pszLocalizedName_LocalStorage; }

private:
	uint32		m_unLevel;
	uint32		m_unRequiredScore;
	char* m_pszLocalizedName_LocalStorage;
};

struct AchievementAward_t
{
	AchievementAward_t(const AchievementAward_t& rhs)
		: m_sNativeName(rhs.m_sNativeName),
		m_unSourceAppId(rhs.m_unSourceAppId),
		m_unAuditData(rhs.m_unAuditData)
	{
		m_vecDefIndex.CopyArray(rhs.m_vecDefIndex.Base(), rhs.m_vecDefIndex.Count());
	}
	AchievementAward_t() {}

	CUtlString m_sNativeName;
	AppId_t m_unSourceAppId;
	uint32 m_unAuditData;
	CUtlVector<uint16> m_vecDefIndex;
};

enum eTimedRewardType
{
	kTimedRewards_Reward,
	kTimedRewards_PremiumTimedDrop,
	kTimedRewards_Crate,
	kTimedRewards_Operation,
	kTimedRewards_Capsule,
	kTimedRewards_Quest,
	kTimedRewards_FreeSprayDropsOct2016,
	kNumTimedRewards
};

struct kill_eater_score_type_t
{
	int32		m_nValue;
	const char* m_pszTypeString;
	const char* m_pszModelAttributeString;
	const char* m_pszLevelBlockName;
	bool		m_bUseLevelBlock;
};

class CWebResource
{
public:
	CWebResource() : m_bOnDemand(false), m_pKeyValues(NULL), m_fnLoadCallback(NULL) {}
	CWebResource(const CWebResource& other)
	{
		m_strName = other.m_strName;
		m_strURL = other.m_strURL;
		m_bOnDemand = other.m_bOnDemand;
		m_pKeyValues = other.m_pKeyValues->MakeCopy();
		m_fnLoadCallback = other.m_fnLoadCallback;
	}
	~CWebResource() { m_pKeyValues->deleteThis(); }

	CUtlString	m_strName;
	CUtlString	m_strURL;
	bool		m_bOnDemand;
	KeyValues* m_pKeyValues;
	void (*m_fnLoadCallback)(const char*, KeyValues*);

	static		bool	s_Initialized;
};

class CForeignAppImports
{
public:
	CForeignAppImports() : m_mapDefinitions(DefLessFunc(uint16)) {}

	void AddMapping(uint16 unForeignDefIndex, const CEconItemDefinition* pDefn);
	const CEconItemDefinition* FindMapping(uint16 unForeignDefIndex) const;

private:
	CUtlMap< uint16, const CEconItemDefinition*> m_mapDefinitions;
};

class ISchemaAttributeType
{
public:
	virtual ~ISchemaAttributeType() { }

	virtual unsigned int GetTypeUniqueIdentifier() const = 0;

	virtual void LoadEconAttributeValue(CEconItem* pTargetItem, const CEconItemAttributeDefinition* pAttrDef, const union attribute_data_union_t& value) const = 0;

	virtual void ConvertEconAttributeValueToByteStream(const union attribute_data_union_t& value, std::string* out_psBytes) const = 0;

	virtual bool BConvertStringToEconAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const char* pszValue, union attribute_data_union_t* out_pValue) const = 0;

	virtual void ConvertEconAttributeValueToString(const CEconItemAttributeDefinition* pAttrDef, const attribute_data_union_t& value, std::string* out_ps) const = 0;

	virtual void LoadByteStreamToEconAttributeValue(CEconItem* pTargetItem, const CEconItemAttributeDefinition* pAttrDef, const std::string& sBytes) const = 0;

	virtual void InitializeNewEconAttributeValue(attribute_data_union_t* out_pValue) const = 0;

	virtual void UnloadEconAttributeValue(union attribute_data_union_t* out_pValue) const = 0;

	virtual bool OnIterateAttributeValue(class IEconItemAttributeIterator* pIterator, const CEconItemAttributeDefinition* pAttrDef, const attribute_data_union_t& value) const = 0;

	virtual bool BSupportsGameplayModificationAndNetworking() const { return false; }
};




struct CAppliedStickerInfo_t
{
	int nID;
	float flWearMin;
	float flWearMax;
	float flScale;
	float flRotate;
};

class CStickerKit
{
public:

	CStickerKit(const CStickerKit& rhs)
		: sName(rhs.sName),
		sDescriptionString(rhs.sDescriptionString),
		sItemName(rhs.sItemName),
		sMaterialPath(rhs.sMaterialPath),
		m_strInventoryImage(rhs.m_strInventoryImage)
	{
		nID = rhs.nID;
		nRarity = rhs.nRarity;
		flRotateStart = rhs.flRotateStart;
		flRotateEnd = rhs.flRotateEnd;
		flScaleMin = rhs.flScaleMin;
		flScaleMax = rhs.flScaleMax;
		flWearMin = rhs.flWearMin;
		flWearMax = rhs.flWearMax;
		bMaterialPathIsAbsolute = rhs.bMaterialPathIsAbsolute;

		m_nEventID = rhs.m_nEventID;
		m_nEventTeamID = rhs.m_nEventTeamID;
		m_nPlayerID = rhs.m_nPlayerID;

		m_pKVItem = rhs.m_pKVItem ? rhs.m_pKVItem->MakeCopy() : NULL;
	}

	CStickerKit(void)
	{
		nID = 0;
		nRarity = 1;
		flRotateStart = 0.0f;
		flRotateEnd = 0.0f;
		flScaleMax = flScaleMin = 1.0f;
		flWearMin = 0.0f;
		flWearMax = 1.0f;
		bMaterialPathIsAbsolute = false;

		m_nEventID = 0;
		m_nEventTeamID = 0;
		m_nPlayerID = 0;

		m_pKVItem = NULL;
	}

	~CStickerKit()
	{
		if (m_pKVItem)
			m_pKVItem->deleteThis();
		m_pKVItem = NULL;
	}

	bool InitFromKeyValues(KeyValues* pKVEntry, const CStickerKit* pDefault, CUtlVector<CUtlString>* pVecErrors = NULL);
	bool GenerateStickerApplicationInfo(CAppliedStickerInfo_t* pInfo) const;

	int nID;
	int nRarity;
	CUtlString sName;
	CUtlString sDescriptionString;
	CUtlString sItemName;
	CUtlString sMaterialPath;
	CUtlString sMaterialPathNoDrips;
	CUtlString m_strInventoryImage;

	int m_nEventID;
	int m_nEventTeamID;
	int m_nPlayerID;

	bool bMaterialPathIsAbsolute;

	float flRotateStart;
	float flRotateEnd;

	float flScaleMin;
	float flScaleMax;

	float flWearMin;
	float flWearMax;

	const char* GetInventoryImage(void) const { return m_strInventoryImage; }

	const char* GetIconURLSmall() const { return m_sIconURLSmall; }
	const char* GetIconURLLarge() const { return m_sIconURLLarge; }
	void	SetIconURLSmall(const char* szURL) { m_sIconURLSmall = szURL; }
	void	SetIconURLLarge(const char* szURL) { m_sIconURLLarge = szURL; }

	KeyValues* GetRawDefinition(void) const { return m_pKVItem; }

private:
	CUtlString m_sIconURLSmall;
	CUtlString m_sIconURLLarge;
	KeyValues* m_pKVItem;
};

class CStickerList
{
public:
	CStickerList(const CStickerList& rhs)
	{
		flWearMin = rhs.flWearMin;
		flWearMax = rhs.flWearMax;

		flTotalWeight = rhs.flTotalWeight;
		arrElements.AddMultipleToTail(rhs.arrElements.Count(), rhs.arrElements.Base());
	}
	CStickerList()
	{
		flWearMin = 0.0f;
		flWearMax = 1.0f;

		flTotalWeight = 0.0f;
	}

	bool InitFromKeyValues(KeyValues* pKVEntry, CUtlVector<CUtlString>* pVecErrors = NULL);

	bool GenerateStickerApplicationInfo(CAppliedStickerInfo_t* pInfo) const;

	struct sticker_list_entry_t
	{
		const CStickerList* pList;
		const CStickerKit* pKit;
		float flWeight;
	};
	CUtlVector< sticker_list_entry_t > arrElements;
	float flTotalWeight;

	float flWearMin;
	float flWearMax;
};

class CPaintKit
{
public:
	enum { NUM_COLORS = 4 };

	CPaintKit(const CPaintKit& rhs)
		: sName(rhs.sName),
		sDescriptionString(rhs.sDescriptionString),
		sDescriptionTag(rhs.sDescriptionTag),
		sVmtPath(rhs.sVmtPath),
		sPattern(rhs.sPattern),
		sLogoMaterial(rhs.sLogoMaterial)
	{
		nID = rhs.nID;

		kvVmtOverrides = rhs.kvVmtOverrides->MakeCopy();

		bBaseDiffuseOverride = rhs.bBaseDiffuseOverride;
		nRarity = rhs.nRarity;
		nStyle = rhs.nStyle;
		flWearDefault = rhs.flWearDefault;
		flWearRemapMin = rhs.flWearRemapMin;
		flWearRemapMax = rhs.flWearRemapMax;
		nFixedSeed = rhs.nFixedSeed;
		uchPhongExponent = rhs.uchPhongExponent;
		uchPhongAlbedoBoost = rhs.uchPhongAlbedoBoost;
		uchPhongIntensity = rhs.uchPhongIntensity;
		flPatternScale = rhs.flPatternScale;
		flPatternOffsetXStart = rhs.flPatternOffsetXStart;
		flPatternOffsetXEnd = rhs.flPatternOffsetXEnd;
		flPatternOffsetYStart = rhs.flPatternOffsetYStart;
		flPatternOffsetYEnd = rhs.flPatternOffsetYEnd;
		flPatternRotateStart = rhs.flPatternRotateStart;
		flPatternRotateEnd = rhs.flPatternRotateEnd;
		flLogoScale = rhs.flLogoScale;
		flLogoOffsetX = rhs.flLogoOffsetX;
		flLogoOffsetY = rhs.flLogoOffsetY;
		flLogoRotation = rhs.flLogoRotation;
		bIgnoreWeaponSizeScale = rhs.bIgnoreWeaponSizeScale;
		nViewModelExponentOverrideSize = rhs.nViewModelExponentOverrideSize;
		bOnlyFirstMaterial = rhs.bOnlyFirstMaterial;

		memcpy(rgbaLogoColor, rhs.rgbaLogoColor, sizeof(rgbaLogoColor));
		memcpy(rgbaLogoColor, rhs.rgbaLogoColor, sizeof(rgbaLogoColor));
	}

	CPaintKit(void)
	{
		nID = 0;

		kvVmtOverrides = nullptr;

		bBaseDiffuseOverride = false;
		nRarity = 1;
		nStyle = 0;
		flWearDefault = 0.5f;
		flWearRemapMin = 0.0f;
		flWearRemapMax = 1.0f;
		nFixedSeed = 0;
		uchPhongExponent = 16;
		uchPhongAlbedoBoost = 0;
		uchPhongIntensity = 255;
		flPatternScale = 0.5f;
		flPatternOffsetXStart = 0.0f;
		flPatternOffsetXEnd = 0.0f;
		flPatternOffsetYStart = 0.0f;
		flPatternOffsetYEnd = 0.0f;
		flPatternRotateStart = 0.0f;
		flPatternRotateEnd = 0.0f;
		flLogoScale = 1.8f;
		flLogoOffsetX = 1.0f;
		flLogoOffsetY = 2.0f;
		flLogoRotation = 0.0f;
		bIgnoreWeaponSizeScale = false;
		nViewModelExponentOverrideSize = 256;
		bOnlyFirstMaterial = false;

		memset(rgbaColor, 128, sizeof(rgbaColor));
		memset(rgbaLogoColor, 128, sizeof(rgbaLogoColor));
	}

	~CPaintKit()
	{
		if (kvVmtOverrides)
		{
			kvVmtOverrides->deleteThis();
			kvVmtOverrides = nullptr;
		}
	}

	bool InitFromKeyValues(KeyValues* pKVEntry, const CPaintKit* pDefault, bool bHandleAbsolutePaths = false);
	void FillKeyValuesForWorkshop(KeyValues* pKVToFill) const;

	int nID;
	CUtlString sName;
	CUtlString sDescriptionString;
	CUtlString sDescriptionTag;

	CUtlString sPattern;
	CUtlString sLogoMaterial;
	bool bBaseDiffuseOverride;
	int nRarity;
	int nStyle;
	Color rgbaColor[NUM_COLORS];
	Color rgbaLogoColor[NUM_COLORS];
	float flWearDefault;
	float flWearRemapMin;
	float flWearRemapMax;
	unsigned char nFixedSeed;
	unsigned char uchPhongExponent;
	unsigned char uchPhongAlbedoBoost;
	unsigned char uchPhongIntensity;
	float flPatternScale;
	float flPatternOffsetXStart;
	float flPatternOffsetXEnd;
	float flPatternOffsetYStart;
	float flPatternOffsetYEnd;
	float flPatternRotateStart;
	float flPatternRotateEnd;
	float flLogoScale;
	float flLogoOffsetX;
	float flLogoOffsetY;
	float flLogoRotation;
	bool bIgnoreWeaponSizeScale;
	int nViewModelExponentOverrideSize;
	bool bOnlyFirstMaterial;

	CUtlString sVmtPath;
	KeyValues* kvVmtOverrides;
};

class AlternateIconData_t
{
public:
	AlternateIconData_t()
	{
	}

	const char* GetInventoryImage(void) const { return sSimpleName; }

	const char* GetIconURLSmall() const { return m_sIconURLSmall; }
	const char* GetIconURLLarge() const { return m_sIconURLLarge; }
	void	SetIconURLSmall(const char* szURL) { m_sIconURLSmall = szURL; }
	void	SetIconURLLarge(const char* szURL) { m_sIconURLLarge = szURL; }

	CUtlString sSimpleName;
	CUtlString sLargeSimpleName;

private:
	CUtlString m_sIconURLSmall;
	CUtlString m_sIconURLLarge;
};

class CProPlayerData
{
public:

	CProPlayerData(const CProPlayerData& rhs)
	{
		m_nAccountID = rhs.m_nAccountID;
		m_sName = rhs.m_sName;
		m_sCode = rhs.m_sCode;
		m_rtDOB = rhs.m_rtDOB;
		m_sGeo = rhs.m_sGeo;

		m_pKVItem = rhs.m_pKVItem ? rhs.m_pKVItem->MakeCopy() : NULL;
	}

	CProPlayerData(void)
	{
		m_nAccountID = 0;
		m_rtDOB = 0;
		m_pKVItem = NULL;
	}

	~CProPlayerData()
	{
		if (m_pKVItem)
			m_pKVItem->deleteThis();
		m_pKVItem = NULL;
	}

	bool BInitFromKeyValues(KeyValues* pDef, CUtlVector<CUtlString>* pVecErrors = NULL);

	KeyValues* GetRawDefinition(void) const { return m_pKVItem; }
	uint32 GetAccountID() const { return m_nAccountID; }
	char const* GetName() const { return m_sName; }
	char const* GetCode() const { return m_sCode; }
	RTime32 GetDOB() const { return m_rtDOB; }
	char const* GetGeo() const { return m_sGeo; }

private:
	KeyValues* m_pKVItem;
	uint32 m_nAccountID;
	CUtlString m_sName;
	CUtlString m_sCode;
	RTime32 m_rtDOB;
	CUtlString m_sGeo;
};

typedef CUtlDict<CUtlConstString, int> ArmoryStringDict_t;
typedef CUtlDict< CUtlVector<CItemLevelingDefinition>* > LevelBlockDict_t;
typedef CUtlMap<unsigned int, kill_eater_score_type_t>	KillEaterScoreMap_t;

struct attr_type_t
{
	CUtlConstString m_sName;
	const ISchemaAttributeType* m_pAttrType;

	attr_type_t(const char* pszName, const ISchemaAttributeType* pAttrType)
		: m_sName(pszName)
		, m_pAttrType(pAttrType)
	{
	}
};

#if defined(CLIENT_DLL) || defined(GAME_DLL)
class IDelayedSchemaData
{
public:
	virtual ~IDelayedSchemaData() {}
	virtual bool InitializeSchema(CEconItemSchema* pItemSchema) = 0;

protected:
	bool InitializeSchemaInternal(CEconItemSchema* pItemSchema, CUtlBuffer& bufRawData, bool bInitAsBinary, uint32 nExpectedVersion);
};
#endif    

class CEconStorePriceSheet;

class CEconItemSchema : public IEconItemSchema
{
public:
	CEconItemSchema();

private:
	CEconItemSchema(const CEconItemSchema& rhs);
	CEconItemSchema& operator=(CEconItemSchema& rhs);

public:
	virtual ~CEconItemSchema(void) { Reset(); };

	virtual bool BInit(const char* fileName, const char* pathID, CUtlVector<CUtlString>* pVecErrors = NULL);
	bool		BInitBinaryBuffer(CUtlBuffer& buffer, CUtlVector<CUtlString>* pVecErrors = NULL);
	bool		BInitTextBuffer(CUtlBuffer& buffer, CUtlVector<CUtlString>* pVecErrors = NULL);

	uint32		GetVersion() const { return m_unVersion; }
	uint32		GetResetCount() const { return m_unResetCount; }

	static CUniformRandomStream& GetRandomStream() { return m_RandomStream; }

	static uint32 CalculateKeyValuesVersion(KeyValues* pKV);

#if defined(CLIENT_DLL) || defined(GAME_DLL)
	void		MaybeInitFromBuffer(IDelayedSchemaData* pDelayedSchemaData);

	bool		BInitFromDelayedBuffer();
#endif    

	equipped_class_t	GetFirstValidClass() const { return m_unFirstValidClass; }
	equipped_class_t	GetLastValidClass() const { return m_unLastValidClass; }
	bool				IsValidClass(equipped_class_t unClass) { return unClass >= m_unFirstValidClass && unClass <= m_unLastValidClass; }
	equipped_slot_t		GetFirstValidItemSlot() const { return m_unFirstValidItemSlot; }
	equipped_slot_t		GetLastValidItemSlot() const { return m_unLastValidItemSlot; }
	bool				IsValidItemSlot(equipped_slot_t unSlot) { return unSlot >= m_unFirstValidItemSlot && unSlot <= m_unLastValidItemSlot; }
	uint32				GetNumAllowedItemPresets() const { return m_unNumItemPresets; }
	bool				IsValidPreset(equipped_preset_t unPreset) const { return unPreset <= m_unNumItemPresets; }
	uint32				GetMinLevel() const { return m_unMinLevel; }
	uint32				GetMaxLevel() const { return m_unMaxLevel; }
	uint32				GetSumQualityWeights() const { return m_unSumQualityWeights; }

	typedef CUtlHashMapLarge<int, CEconItemDefinition*>	ItemDefinitionMap_t;
	const ItemDefinitionMap_t& GetItemDefinitionMap() const { return m_mapItems; }

	typedef CUtlMap<int, CEconItemDefinition*, int, CDefLess<int> >	SortedItemDefinitionMap_t;
	const SortedItemDefinitionMap_t& GetSortedItemDefinitionMap() const { return m_mapItemsSorted; }

	typedef CUtlDict<CEconLootListDefinition> LootListDefinitionDict_t;
	const LootListDefinitionDict_t& GetLootLists() const { return m_dictLootLists; }

	typedef CUtlMap<int, const char*, int, CDefLess<int> > RevolvingLootListDefinitionMap_t;
	const RevolvingLootListDefinitionMap_t& GetRevolvingLootLists() const { return m_mapRevolvingLootLists; }
	const RevolvingLootListDefinitionMap_t& GetQuestRewardLootLists() const { return m_mapQuestRewardLootLists; }

	typedef CUtlMap<const char*, int> BodygroupStateMap_t;
	const BodygroupStateMap_t& GetDefaultBodygroupStateMap() const { return m_mapDefaultBodygroupState; }

	typedef CUtlVector<CEconColorDefinition*>	ColorDefinitionsList_t;

	typedef CUtlMap<int, CEconMusicDefinition*, int, CDefLess<int> >	MusicDefinitionsList_t;
	typedef CUtlMap<int, CEconQuestDefinition*, int, CDefLess<int> >	QuestDefinitionsList_t;
	typedef CUtlMap<int, CEconCampaignDefinition*, int, CDefLess<int> >	CampaignDefinitionsList_t;


	const MusicDefinitionsList_t& GetMusicDefinitionMap() const { return m_mapMusicDefs; }

	const CEconItemDefinition* GetDefaultItemDefinition() const { return m_pDefaultItemDefinition; }
	IEconItemDefinition* GetDefaultItemDefinitionInterface() { return m_pDefaultItemDefinition; }

	const CUtlMap<int, CEconItemQualityDefinition, int, CDefLess<int> >& GetQualityDefinitionMap() const { return m_mapQualities; }

	typedef CUtlVector< CEconItemAttributeDefinition* > EconAttrDefsContainer_t;
	EconAttrDefsContainer_t& GetAttributeDefinitionContainer() { return m_mapAttributesContainer; }
	const EconAttrDefsContainer_t& GetAttributeDefinitionContainer() const { return m_mapAttributesContainer; }

	typedef CUtlMap<int, CEconCraftingRecipeDefinition*, int, CDefLess<int> > RecipeDefinitionMap_t;
	const RecipeDefinitionMap_t& GetRecipeDefinitionMap() const { return m_mapRecipes; }
	const CUtlMap<const char*, CEconItemSetDefinition, int >& GetItemSets() const { return m_mapItemSets; }

#if defined(CLIENT_DLL) || defined(GAME_DLL)
	const ArmoryStringDict_t& GetArmoryDataItemClasses() const { return m_dictArmoryItemClassesDataStrings; }
	const ArmoryStringDict_t& GetArmoryDataItemTypes() const { return m_dictArmoryItemTypesDataStrings; }
	const ArmoryStringDict_t& GetArmoryDataItems() const { return m_dictArmoryItemDataStrings; }
	const ArmoryStringDict_t& GetArmoryDataAttributes() const { return m_dictArmoryAttributeDataStrings; }
#endif

	const CTimedItemRewardDefinition* GetTimedReward(eTimedRewardType type) const;

	const CEconLootListDefinition* GetLootListByName(const char* pListName, int* out_piIndex = NULL) const;
	const CEconLootListDefinition* GetLootListByIndex(int iIdx) const { return m_dictLootLists.IsValidIndex(iIdx) ? &(m_dictLootLists[iIdx]) : NULL; }

	virtual void PurgeLootLists(void) { m_dictLootLists.Purge(); }
	virtual const IEconLootListDefinition* GetLootListInterfaceByName(const char* pListName, int* out_piIndex = NULL) { return GetLootListByName(pListName, out_piIndex); }
	virtual const IEconLootListDefinition* GetLootListInterfaceByIndex(int iIdx) const { return GetLootListByIndex(iIdx); }
	virtual const int GetLootListIndex(const char* pListName) { return m_dictLootLists.Find(pListName); }
	virtual const int GetLootListInterfaceCount(void) const { return m_dictLootLists.Count(); }

	const CEconItemSetDefinition* GetItemSet(const char* pSetName, int* piIndex = NULL) const;
	const CEconItemSetDefinition* GetItemSetByIndex(int iIdx) const { return m_mapItemSets.IsValidIndex(iIdx) ? &(m_mapItemSets[iIdx]) : NULL; }

	uint8 GetDefaultQuality() const { return AE_UNIQUE; }

	virtual int GetItemSetCount(void) const { return m_mapItemSets.Count(); }
	virtual const IEconItemSetDefinition* GetItemSet(int iIndex) const;

	void AssignDefaultBodygroupState(const char* pszBodygroupName, int iValue);

	equip_region_mask_t GetEquipRegionMaskByName(const char* pRegionName) const;

	struct EquipRegion
	{
		CUtlConstString		m_sName;
		unsigned int		m_unBitIndex;		                       
		equip_region_mask_t m_unMask;			    
	};

	typedef CUtlVector<EquipRegion>		EquipRegionsList_t;
	const EquipRegionsList_t& GetEquipRegionsList() const { return m_vecEquipRegionsList; }

	virtual KeyValues* FindDefinitionPrefabByName(const char* pszPrefabName) const;

	equip_region_mask_t GetEquipRegionBitMaskByName(const char* pRegionName) const;

	CUtlVector< CEconItemDefinition* >& GetBundles() { return m_vecBundles; }	       

private:
	void SetEquipRegionConflict(int iRegion, unsigned int unBit);
	int GetEquipRegionIndexByName(const char* pRegionName) const;

public:
	bool BGetItemQualityFromName(const char* pchName, uint8* nQuality) const;
	const CEconItemQualityDefinition* GetQualityDefinition(int nQuality) const;
	const CEconItemQualityDefinition* GetQualityDefinitionByName(const char* pszDefName) const;
	virtual int GetQualityDefinitionCount(void) { return m_mapQualities.Count(); }
	virtual const char* GetQualityName(uint8 iQuality);
	virtual int GetQualityIndex(const char* pszQuality);
	bool BGetItemRarityFromName(const char* pchName, uint8* nRarity) const;
	const CEconItemRarityDefinition* GetRarityDefinitionByMapIndex(int nRarityIndex) const;
	const CEconItemRarityDefinition* GetRarityDefinition(int nRarity) const;
	const CEconItemRarityDefinition* GetRarityDefinitionByName(const char* pszDefName) const;
	virtual int GetRarityDefinitionCount(void) const { return m_mapRarities.Count(); }
	virtual const char* GetRarityName(uint8 iRarity);
	virtual const char* GetRarityLocKey(uint8 iRarity);
	virtual const char* GetRarityColor(uint8 iRarity);
	virtual const char* GetRarityLootList(uint8 iRarity);
	virtual int GetRarityIndex(const char* pszRarity);
	CEconItemDefinition* GetItemDefinitionMutable(int iItemIndex, bool bNoDefault = false);
	const CEconItemDefinition* GetItemDefinition(int iItemIndex, bool bNoDefault = false) const;
	const CEconItemDefinition* GetItemDefinitionByMapIndex(int iMapIndex) const;
	const CEconItemAttributeDefinition* GetAttributeDefinition(int iAttribIndex) const;
	virtual const IEconItemAttributeDefinition* GetAttributeDefinitionInterface(int iAttribIndex) const { return GetAttributeDefinition(iAttribIndex); }
	const CEconItemAttributeDefinition* GetAttributeDefinitionByName(const char* pszDefName) const;
	const CEconCraftingRecipeDefinition* GetRecipeDefinition(int iRecipeIndex) const;
	int GetPaintKitDefinitionCount(void) const { return m_mapPaintKits.Count(); }
	void AddPaintKitDefinition(int iPaintKitID, CPaintKit* pPaintKit);
	void RemovePaintKitDefinition(int iPaintKitID);
	const unsigned int GetPaintKitCount() const;
	const CPaintKit* GetPaintKitDefinition(int iPaintKitID) const;
	const CPaintKit* GetPaintKitDefinitionByMapIndex(int iMapIndex);
	const CPaintKit* GetPaintKitDefinitionByName(const char* pchName) const;

	int GetStickerKitDefinitionCount(void) const { return m_mapStickerKits.Count(); }
	void AddStickerKitDefinition(int iStickerKitID, CStickerKit* pStickerKit);
	void RemoveStickerKitDefinition(int iStickerKitID);
	const CStickerKit* GetStickerKitDefinition(int iStickerKitID) const;
	const CStickerKit* GetStickerKitDefinitionByMapIndex(int iMapIndex);
	const CStickerKit* GetStickerKitDefinitionByName(const char* pchName) const;
	const CStickerList* GetStickerListDefinitionByName(const char* pchName) const;
	const CEconMusicDefinition* GetMusicKitDefinitionByName(const char* pchName) const;

	const CEconColorDefinition* GetColorDefinitionByName(const char* pszDefName) const;

	int GetGraffitiTintMaxValidDefID() const { return m_nMaxValidGraffitiTintDefID; }
	const CEconGraffitiTintDefinition* GetGraffitiTintDefinitionByID(int nID) const;
	const CEconGraffitiTintDefinition* GetGraffitiTintDefinitionByName(const char* pszDefName) const;

	const CEconMusicDefinition* GetMusicDefinition(uint32 unMusicID) const;

	CEconQuestDefinition* GetQuestDefinition(uint32 unQuestID) const;
	const QuestDefinitionsList_t& GetQuestDefinitionMap(void) const { return m_mapQuestDefs; }

	const QuestEventsSchedule_t& GetAndUpdateQuestEventsSchedule(void);
	const QuestEvents_t& GetQuestEvents(void) const { return m_vecQuestEvents; }

	CEconCampaignDefinition* GetCampaignDefinition(uint32 unCampaignID) const;
	const CampaignDefinitionsList_t& GetCampaignDefinitionMap(void) const { return m_mapCampaignDefs; }

	virtual int GetToolType(const char* pszToolType) const { return -1; }
	virtual int GetNumPrefabs(void) { return m_mapDefinitionPrefabs.Count(); }
	virtual const char* GetPrefabName(int idx) { return m_mapDefinitionPrefabs.Key(idx); }

	const CEconSoundMaterialDefinition* GetSoundMaterialDefinitionByID(int nSoundMaterialID) const;
	const CEconSoundMaterialDefinition* GetSoundMaterialDefinitionByName(const char* pszSoundMaterialName) const;
	virtual const char* GetSoundMaterialNameByID(int nSoundMaterialID);
	virtual int GetSoundMaterialID(const char* pszSoundMaterial);

	virtual int GetSoundMaterialCount(void) { return m_mapSoundMaterials.Count(); }
	virtual int GetSoundMaterialIDByIndex(int nIndex);

#ifdef CLIENT_DLL
	EWebResourceStatus LoadWebResource(CUtlString pszName, void (*fnCallback)(const char*, KeyValues*), bool bForceReload = false);
	void SetWebResource(CUtlString strName, KeyValues* pResourceKV);
#endif

	const CProPlayerData* GetProPlayerDataByAccountID(uint32 unAccountID) const;
	const CUtlVector< const CProPlayerData* >* GetProPlayersDataForEventIDTeamID(int nEventID, int nTeamID) const;

	virtual uint32 GetHeroID(const char* pszHeroName) { return 0; }

	bool BCanGSCreateItems(uint32 unIP) const;
	const AchievementAward_t* GetAchievementRewardByDefIndex(uint16 usDefIndex) const;
	bool BHasAchievementRewards(void) const { return (m_dictAchievementRewards.Count() > 0); }

	static CUtlString ComputeAchievementName(AppId_t unAppID, const char* pchNativeAchievementName);

	CEconItemDefinition* GetItemDefinitionByName(const char* pszDefName);
	const CEconItemDefinition* GetItemDefinitionByName(const char* pszDefName) const;

	attachedparticlesystem_t* GetAttributeControlledParticleSystem(int id);
	attachedparticlesystem_t* GetAttributeControlledParticleSystemByIndex(int id);
	attachedparticlesystem_t* FindAttributeControlledParticleSystem(const char* pchSystemName, int* outID = NULL);
	typedef CUtlMap<int, attachedparticlesystem_t, int, CDefLess<int> > ParticleDefinitionMap_t;
	const ParticleDefinitionMap_t& GetAttributeControlledParticleSystems() const { return m_mapAttributeControlledParticleSystems; }
	virtual int GetNumAttributeControlledParticleSystems() const { return GetAttributeControlledParticleSystems().Count(); }

	const KillEaterScoreMap_t& GetKillEaterScoreTypes() const { return m_mapKillEaterScoreTypes; }

	const kill_eater_score_type_t* FindKillEaterScoreType(uint32 unScoreType) const;

	const CUtlVector<attr_type_t>& GetAttributeTypes() const { return m_vecAttributeTypes; }
	const ISchemaAttributeType* GetAttributeType(const char* pszAttrTypeName) const;

	const LevelBlockDict_t& GetItemLevelingDataDict() const { return m_vecItemLevelingData; }

	const CUtlVector<CItemLevelingDefinition>* GetItemLevelingData(const char* pszLevelBlockName) const
	{
		LevelBlockDict_t::IndexType_t i = m_vecItemLevelingData.Find(pszLevelBlockName);
		if (i == LevelBlockDict_t::InvalidIndex())
			return NULL;

		return m_vecItemLevelingData[i];
	}

	const CItemLevelingDefinition* GetItemLevelForScore(const char* pszLevelBlockName, uint32 unScore) const;
	const char* GetKillEaterScoreTypeLocString(uint32 unScoreType) const;
	bool GetKillEaterScoreTypeUseLevelData(uint32 unScoreType) const;
	const char* GetKillEaterScoreTypeLevelingDataName(uint32 unScoreType) const;
#if defined(CLIENT_DLL) || defined(GAME_DLL)
	virtual void ItemTesting_CreateTestDefinition(int iCloneFromItemDef, int iNewDef, KeyValues* pNewKV);
	virtual void ItemTesting_DiscardTestDefinition(int iDef);
#endif

	econ_tag_handle_t GetHandleForTag(const char* pszTagName);			         

	typedef CUtlDict<econ_tag_handle_t> EconTagDict_t;

public:
	virtual CEconItemDefinition* CreateEconItemDefinition() { return new CEconItemDefinition; }
	virtual CEconCraftingRecipeDefinition* CreateCraftingRecipeDefinition() { return new CEconCraftingRecipeDefinition; }
	virtual CEconStyleInfo* CreateEconStyleInfo() { return new CEconStyleInfo; }

	virtual IEconTool* CreateEconToolImpl(const char* pszToolType, const char* pszUseString, const char* pszUsageRestriction, item_capabilities_t unCapabilities, KeyValues* pUsageKV);
	virtual int GetItemDefinitionCount() { return m_mapItems.Count(); }

	AlternateIconData_t* GetAlternateIcon(uint64 ullAlternateIcon);

	typedef CUtlMap< uint64, AlternateIconData_t, int, CDefLess<uint64> >	AlternateIconsMap_t;
	AlternateIconsMap_t& GetAlternateIconsMap() { return m_mapAlternateIcons; }


protected:
	virtual void Reset(void);

	virtual KeyValues* GetRawDefinition(void) const { return m_pKVRawDefinition; }
	virtual IEconItemDefinition* GetItemDefinitionInterface(int iDefIndex) { return GetItemDefinitionMutable(iDefIndex, true); }
	virtual int GetLoadoutSlotCount() { return 0; }
	virtual const char* GetLoadoutSlotName(int iSlot) { return NULL; }
	virtual int GetLoadoutSlot(const char* pszSlotName) { return 0; }
	virtual int GetCharacterSlotType(int iCharacter, int iSlot) { return 0; }
	virtual int GetCharacterID(const char* pszCharacterName) { return 0; }
	virtual bool DeleteItemDefinition(int iDefIndex);
	virtual int GetCharacterCount(void) { return 0; }
	virtual const char* GetCharacterName(int iCharacter) { return NULL; }
	virtual const char* GetCharacterModel(int iCharacter) { return NULL; }

	virtual int CalculateNumberOfConcreteItems(const CEconItemDefinition* pItemDef);	       

	virtual bool BInitItems(KeyValues* pKVItems, CUtlVector<CUtlString>* pVecErrors);
	virtual bool BInitItemMappings(CUtlVector<CUtlString>* pVecErrors);
	virtual bool BInitBundles(CUtlVector<CUtlString>* pVecErrors);
	virtual bool BInitPaymentRules(CUtlVector<CUtlString>* pVecErrors);
	virtual bool BInitItemSets(KeyValues* pKVItemSets, CUtlVector<CUtlString>* pVecErrors);

private:
	bool BInitGameInfo(KeyValues* pKVGameInfo, CUtlVector<CUtlString>* pVecErrors);
	bool BInitAttributeTypes(CUtlVector<CUtlString>* pVecErrors);
	bool BInitDefinitionPrefabs(KeyValues* pKVPrefabs, CUtlVector<CUtlString>* pVecErrors);
	bool BInitRarities(KeyValues* pKVRarities, CUtlVector<CUtlString>* pVecErrors);
	bool BInitQualities(KeyValues* pKVAttributes, CUtlVector<CUtlString>* pVecErrors);
	bool BInitColors(KeyValues* pKVColors, CUtlVector<CUtlString>* pVecErrors);
	bool BInitGraffitiTints(KeyValues* pKVColors, CUtlVector<CUtlString>* pVecErrors);
	bool BInitAttributes(KeyValues* pKVAttributes, CUtlVector<CUtlString>* pVecErrors);
	bool BInitEquipRegions(KeyValues* pKVEquipRegions, CUtlVector<CUtlString>* pVecErrors);
	bool BInitEquipRegionConflicts(KeyValues* pKVEquipRegions, CUtlVector<CUtlString>* pVecErrors);
	bool BInitTimedRewards(KeyValues* pKVTimeRewards, CUtlVector<CUtlString>* pVecErrors);
	bool BInitAchievementRewards(KeyValues* pKVTimeRewards, CUtlVector<CUtlString>* pVecErrors);
	bool BInitRecipes(KeyValues* pKVRecipes, CUtlVector<CUtlString>* pVecErrors);
	virtual bool BInitLootLists(KeyValues* pKVLootLists, KeyValues* pKVRandomAttributeTemplates, CUtlVector<CUtlString>* pVecErrors, bool bServerLists);
	bool BInitRevolvingLootLists(KeyValues* pKVRevolvingLootLists, CUtlVector<CUtlString>* pVecErrors);
	bool BInitQuestRewardLootLists(KeyValues* pKVQuestRewardLootLists, CUtlVector<CUtlString>* pVecErrors);
#ifdef TF_CLIENT_DLL
	bool BInitConcreteItemCounts(CUtlVector<CUtlString>* pVecErrors);
#endif
	bool BInitItemLevels(KeyValues* pKVItemLevels, CUtlVector<CUtlString>* pVecErrors);
	bool BInitKillEaterScoreTypes(KeyValues* pKVItemLevels, CUtlVector<CUtlString>* pVecErrors);
	bool BInitAlternateIcons(KeyValues* pKVAlternateIcons, CUtlVector<CUtlString>* pVecErrors);
	bool BInitAlternateIcon(uint64 ullAltIconKey, char const* szSimpleName, KeyValues* pKVAlternateIcon, CUtlVector<CUtlString>* pVecErrors);
	bool BInitStickerKits(KeyValues* pKVStickerKits, CUtlVector<CUtlString>* pVecErrors);
	bool BInitSoundMaterials(KeyValues* pKVSoundMaterials, CUtlVector<CUtlString>* pVecErrors);
	bool BInitStickerLists(KeyValues* pKVStickerKits, CUtlVector<CUtlString>* pVecErrors);

	bool BInitPaintKits(KeyValues* pKVPaintKits, CUtlVector<CUtlString>* pVecErrors);
	bool BInitPaintKitsRarity(KeyValues* pKVPaintKitsRarity, CUtlVector<CUtlString>* pVecErrors);

	bool BInitMusicDefs(KeyValues* pKVMusicDefs, CUtlVector<CUtlString>* pVecErrors);

	bool BInitQuestDefs(KeyValues* pKVQuestDefs, CUtlVector<CUtlString>* pVecErrors);

	bool BInitQuestEvents(KeyValues* pKVQuestEvents, CUtlVector<CUtlString>* pVecErrors);

	bool BInitCampaignDefs(KeyValues* pKVCampaignDefs, CUtlVector<CUtlString>* pVecErrors);

	bool BInitProPlayers(KeyValues* pKVData, CUtlVector<CUtlString>* pVecErrors);

#ifdef CLIENT_DLL
	bool BInitWebResources(KeyValues* pKVWebResources, CUtlVector<CUtlString>* pVecErrors);
#endif

	bool BPostSchemaInitStartupChecks(CUtlVector<CUtlString>* pVecErrors);

	virtual attachedparticlesystem_t GetAttachedParticleSystemInfo(KeyValues* pParticleSystemKV, int32 nItemIndex) const;
	virtual bool BInitAttributeControlledParticleSystems(KeyValues* pKVParticleSystems, CUtlVector<CUtlString>* pVecErrors);
#if defined(CLIENT_DLL) || defined(GAME_DLL)
	bool BInitArmoryData(KeyValues* pKVArmoryData, CUtlVector<CUtlString>* pVecErrors);
#else
	bool BInitExperiements(KeyValues* pKVExperiments, CUtlVector<CUtlString>* pVecErrors);
	bool BInitForeignImports(CUtlVector<CUtlString>* pVecErrors);

	CForeignAppImports* FindOrAddAppImports(AppId_t unAppID);
#endif

protected:
	virtual bool BInitSchema(KeyValues* pKVRawDefinition, CUtlVector<CUtlString>* pVecErrors = NULL);
private:
	bool			m_bSchemaUpdatesEnabled;

	uint32			m_unResetCount;

	KeyValues* m_pKVRawDefinition;
	uint32			m_unVersion;

	equipped_class_t	m_unFirstValidClass;
	equipped_class_t	m_unLastValidClass;

	equipped_slot_t		m_unFirstValidItemSlot;
	equipped_slot_t		m_unLastValidItemSlot;

	uint32			m_unNumItemPresets;

	uint32			m_unMinLevel;
	uint32			m_unMaxLevel;

	uint32			m_unSumQualityWeights;

	CUtlVector<attr_type_t>								m_vecAttributeTypes;

	CUtlMap<int, CEconItemRarityDefinition, int, CDefLess<int> >		m_mapRarities;

	CUtlMap<int, CEconItemQualityDefinition, int, CDefLess<int> >		m_mapQualities;

	ItemDefinitionMap_t									m_mapItems;

	SortedItemDefinitionMap_t							m_mapItemsSorted;

	CEconItemDefinition* m_pDefaultItemDefinition;

	EconAttrDefsContainer_t								m_mapAttributesContainer;

	RecipeDefinitionMap_t								m_mapRecipes;

	CUtlMap<const char*, CEconItemSetDefinition, int >	m_mapItemSets;

	CUtlMap<int, const char*, int, CDefLess<int> >	m_mapRevolvingLootLists;

	CUtlMap<int, const char*, int, CDefLess<int> >	m_mapQuestRewardLootLists;

	LootListDefinitionDict_t							m_dictLootLists;

	CUtlVector<CTimedItemRewardDefinition>				m_vecTimedRewards;

	CUtlDict<CWebResource*, int>						m_dictWebResources;

	AlternateIconsMap_t	m_mapAlternateIcons;

	CUtlDict< AchievementAward_t*, int >				m_dictAchievementRewards;
	CUtlMap< uint32, AchievementAward_t*, int, CDefLess<uint32> >				m_mapAchievementRewardsByData;

	CUtlMap<int, CPaintKit*, int, CDefLess<int> >						m_mapPaintKits;

	CUtlMap<int, CStickerKit*, int, CDefLess<int> >					m_mapStickerKits;
	CUtlDict< CStickerKit*, int >						m_dictStickerKits;

	CUtlDict< CStickerList*, int >						m_dictStickerLists;

	CUtlMap<int, attachedparticlesystem_t, int, CDefLess<int> >				m_mapAttributeControlledParticleSystems;

	EquipRegionsList_t									m_vecEquipRegionsList;

	CUtlMap<const char*, KeyValues*, int>			m_mapDefinitionPrefabs;

	ColorDefinitionsList_t								m_vecColorDefs;
	CUtlVector< CEconGraffitiTintDefinition* >			m_vecGraffitiTintDefs;
	CUtlStringMap< CEconGraffitiTintDefinition* >		m_mapGraffitiTintByName;
	int													m_nMaxValidGraffitiTintDefID;

	MusicDefinitionsList_t								m_mapMusicDefs;

	QuestDefinitionsList_t								m_mapQuestDefs;

	CampaignDefinitionsList_t							m_mapCampaignDefs;

	QuestEvents_t										m_vecQuestEvents;
	QuestEventsSchedule_t								m_mapQuestEventsSchedule;

	typedef CUtlMap< uint32, CProPlayerData*, int, CDefLess< uint32 > > MapProPlayersByAccountID_t;
	typedef CUtlStringMap< CProPlayerData* > MapProPlayersByName_t;
	typedef CUtlMap< uint64, CUtlVector< const CProPlayerData* >*, int, CDefLess< uint64 > > MapProPlayersByEventIDTeamID_t;
	MapProPlayersByAccountID_t m_mapProPlayersByAccountID;
	MapProPlayersByName_t m_mapProPlayersByCode;
	MapProPlayersByEventIDTeamID_t m_mapProPlayersByEventIDTeamID;

	CUtlMap<int, CEconSoundMaterialDefinition, int, CDefLess<int> >	m_mapSoundMaterials;

	BodygroupStateMap_t									m_mapDefaultBodygroupState;

	EconTagDict_t										m_dictTags;

	KillEaterScoreMap_t									m_mapKillEaterScoreTypes;

	LevelBlockDict_t m_vecItemLevelingData;

#if defined(CLIENT_DLL) || defined(GAME_DLL)
	ArmoryStringDict_t m_dictArmoryItemTypesDataStrings;
	ArmoryStringDict_t m_dictArmoryItemClassesDataStrings;
	ArmoryStringDict_t m_dictArmoryAttributeDataStrings;
	ArmoryStringDict_t m_dictArmoryItemDataStrings;

	IDelayedSchemaData* m_pDelayedSchemaData;
#endif

	CUtlVector< CEconItemDefinition* > m_vecBundles;	      

	static CUniformRandomStream m_RandomStream;      

	bool m_bSchemaParsingItems;
};

extern CEconItemSchema& GEconItemSchema();

template < class T >
class CSchemaFieldHandle
{
public:
	explicit CSchemaFieldHandle(const char* szName)
		: m_szName(szName)
	{
		m_pRef = GetTypedRef();
		m_unSchemaGeneration = GEconItemSchema().GetResetCount();
#if _DEBUG
		m_unVersion_Debug = GEconItemSchema().GetVersion();
#endif
	}

	operator const T* (void) const
	{
		uint32 unSchemaGeneration = GEconItemSchema().GetResetCount();
		if (m_unSchemaGeneration != unSchemaGeneration)
		{
			m_pRef = GetTypedRef();
			m_unSchemaGeneration = unSchemaGeneration;
#if _DEBUG
			m_unVersion_Debug = GEconItemSchema().GetVersion();
#endif
		}

#if _DEBUG
		Assert(m_unVersion_Debug == GEconItemSchema().GetVersion());
#endif
		return m_pRef;
	}

	const T* operator->(void) const
	{
		return static_cast<const T*>(*this);
	}

	const char* GetName(void) const
	{
		return m_szName;
	}

private:
	const T* GetTypedRef() const;

private:
	const char* m_szName;

	mutable const T* m_pRef;
	mutable uint32 m_unSchemaGeneration;
#if _DEBUG
	mutable uint32 m_unVersion_Debug;
#endif
};

template < >
inline const CEconColorDefinition* CSchemaFieldHandle<CEconColorDefinition>::GetTypedRef(void) const
{
	return GEconItemSchema().GetColorDefinitionByName(m_szName);
}

template < >
inline const CEconItemAttributeDefinition* CSchemaFieldHandle<CEconItemAttributeDefinition>::GetTypedRef(void) const
{
	return GEconItemSchema().GetAttributeDefinitionByName(m_szName);
}

template < >
inline const CEconItemDefinition* CSchemaFieldHandle<CEconItemDefinition>::GetTypedRef(void) const
{
	return GEconItemSchema().GetItemDefinitionByName(m_szName);
}

template < >
inline const CEconLootListDefinition* CSchemaFieldHandle<CEconLootListDefinition>::GetTypedRef(void) const
{
	return GEconItemSchema().GetLootListByName(m_szName);
}

typedef CSchemaFieldHandle<CEconColorDefinition>			CSchemaColorDefHandle;
typedef CSchemaFieldHandle<CEconMusicDefinition>			CSchemaMusicDefHandle;
typedef CSchemaFieldHandle<CEconItemAttributeDefinition>	CSchemaAttributeDefHandle;
typedef CSchemaFieldHandle<CEconItemDefinition>				CSchemaItemDefHandle;
typedef CSchemaFieldHandle<CEconLootListDefinition>			CSchemaLootListDefHandle;

inline const CEconItemAttributeDefinition* static_attrib_t::GetAttributeDefinition() const
{
	return GEconItemSchema().GetAttributeDefinition(iDefIndex);
}

int StringFieldToInt(const char* szValue, const char** pValueStrings, int iNumStrings, bool bDontAssert = false);
int StringFieldToInt(const char* szValue, const CUtlVector<const char*>& vecValueStrings, bool bDontAssert = false);

enum EStickerAttributeType
{
	k_EStickerAttribute_ID,
	k_EStickerAttribute_Wear,
	k_EStickerAttribute_Scale,
	k_EStickerAttribute_Rotation,
	k_EStickerAttribute_Count,
};
const int g_nNumStickerAttrs = 6;
const CSchemaAttributeDefHandle& GetStickerAttributeDefHandle(int attrNum, EStickerAttributeType type);

enum ECampaignAttributeType
{
	k_ECampaignAttribute_CompletionBitfield,
	k_ECampaignAttribute_LastCompletedQuest,

};
const int g_nNumCampaigns = 8;	         
const CSchemaAttributeDefHandle& GetCampaignAttributeDefHandle(int nCampaignID, ECampaignAttributeType type);

extern const uint32 g_unNumWearBuckets;
uint64 Helper_GetAlternateIconKeyForWeaponPaintWearItem(item_definition_index_t nDefIdx, uint32 nPaintId, uint32 nWear);

uint64 Helper_GetAlternateIconKeyForTintedStickerItem(uint32 nStickerKitID, uint32 unTintID);


#endif 