#ifndef ITEM_SELECTION_CRITERIA_H
#define ITEM_SELECTION_CRITERIA_H
#ifdef _WIN32
#pragma once
#endif

#include "game_item_schema.h"

const int k_cchCreateItemLen = 64;

enum EItemCriteriaOperator
{
	k_EOperator_String_EQ = 0,				      
	k_EOperator_Not = 1,					  
	k_EOperator_String_Not_EQ = 1,			       
	k_EOperator_Float_EQ = 2,				        
	k_EOperator_Float_Not_EQ = 3,			         
	k_EOperator_Float_LT = 4,				        
	k_EOperator_Float_Not_LT = 5,			         
	k_EOperator_Float_LTE = 6,				          
	k_EOperator_Float_Not_LTE = 7,			           
	k_EOperator_Float_GT = 8,				        
	k_EOperator_Float_Not_GT = 9,			         
	k_EOperator_Float_GTE = 10,				          
	k_EOperator_Float_Not_GTE = 11,			           
	k_EOperator_Subkey_Contains = 12,		      
	k_EOperator_Subkey_Not_Contains = 13,	        

	k_EItemCriteriaOperator_Count = 14,
};


EItemCriteriaOperator EItemCriteriaOperatorFromName(const char* pch);
const char* PchNameFromEItemCriteriaOperator(int eItemCriteriaOperator);

class CEconItemSchema;
class CEconItemDefinition;
class CEconItem;
class CSOItemCriteria;
class CSOItemCriteriaCondition;

const uint8	 k_unItemRarity_Any = 0xF;
const uint8	 k_unItemQuality_Any = 0xF;


class CItemSelectionCriteria
{
public:
	CItemSelectionCriteria() :
		m_bItemLevelSet(false),
		m_unItemLevel(0),
		m_bQualitySet(false),
		m_nItemQuality(k_unItemQuality_Any),
		m_bRaritySet(false),
		m_nItemRarity(k_unItemRarity_Any),
		m_unInitialInventory(0),
		m_unInitialQuantity(1),
		m_bForcedQualityMatch(false),
		m_bIgnoreEnabledFlag(false),
		m_bRecentOnly(false),
		m_bIsLootList(false)
	{
	}

	CItemSelectionCriteria(const CItemSelectionCriteria& that);
	CItemSelectionCriteria& operator=(const CItemSelectionCriteria& rhs);
	~CItemSelectionCriteria();

	bool			BItemLevelSet(void) const { return m_bItemLevelSet; }
	uint32		GetItemLevel(void) const { Assert(m_bItemLevelSet); return m_unItemLevel; }
	void			SetItemLevel(uint32 unLevel) { m_unItemLevel = unLevel; m_bItemLevelSet = true; }
	bool			BQualitySet(void) const { return m_bQualitySet; }
	int32			GetQuality(void) const { Assert(m_bQualitySet); return m_nItemQuality; }
	void			SetQuality(int32 nQuality) { m_nItemQuality = nQuality; m_bQualitySet = true; }
	bool			BRaritySet(void) const { return m_bRaritySet; }
	int32			GetRarity(void) const { Assert(m_bRaritySet); return m_nItemRarity; }
	void			SetRarity(int32 nRarity) { m_nItemRarity = nRarity; m_bRaritySet = true; }
	uint32		GetInitialInventory(void) const { return m_unInitialInventory; }
	void			SetInitialInventory(uint32 unInventory) { m_unInitialInventory = unInventory; }
	uint32		GetInitialQuantity(void) const { return m_unInitialQuantity; }
	void			SetInitialQuantity(uint32 unQuantity) { m_unInitialQuantity = unQuantity; }
	void			SetExplicitQualityMatch(bool bExplicit) { m_bForcedQualityMatch = bExplicit; }
	void			SetIgnoreEnabledFlag(bool bIgnore) { m_bIgnoreEnabledFlag = bIgnore; }
	void			SetRecentOnly(bool bCheck) { m_bRecentOnly = bCheck; }
	bool			IsLootList(void) const { return m_bIsLootList; }

	bool			BAddCondition(const char* pszField, EItemCriteriaOperator eOp, float flValue, bool bRequired);
	bool			BAddCondition(const char* pszField, EItemCriteriaOperator eOp, const char* pszValue, bool bRequired);
	int			GetConditionsCount() { return m_vecConditions.Count(); }
	const char* GetValueForFirstConditionOfFieldName(const char* pchName) const;

	bool			BInitFromKV(KeyValues* pKVCriteria, const CEconItemSchema& schemaa);
	bool			BInitFromItemAndPaint(int nItemDef, int nPaintID, const CEconItemSchema& schemaa);

	bool			BSerializeToMsg(CSOItemCriteria& msg) const;
	bool			BDeserializeFromMsg(const CSOItemCriteria& msg);

	bool			BEvaluate(const CEconItemDefinition* pItemDef, const CEconItemSchema& pschema) const;
	bool			BEvaluate(const CEconItem* pItem, const CEconItemSchema& pschema) const;

private:
	class CCondition
	{
	public:
		CCondition(const char* pszField, EItemCriteriaOperator eOp, bool bRequired)
			: m_sField(pszField), m_EOp(eOp), m_bRequired(bRequired)
		{
		}

		virtual ~CCondition() { }

		bool BEvaluate(KeyValues* pKVItem) const;

		virtual bool BSerializeToMsg(CSOItemCriteriaCondition& msg) const;

		EItemCriteriaOperator	GetEOp(void) const { return m_EOp; }
		virtual	const char* GetField(void) { return m_sField.Get(); }
		virtual	const char* GetValue(void) { Assert(0); return NULL; }

	protected:
		virtual bool BInternalEvaluate(KeyValues* pKVItem) const = 0;

		CUtlString				m_sField;
		EItemCriteriaOperator	m_EOp;
		bool					m_bRequired;
	};


	class CStringCondition : public CCondition
	{
	public:
		CStringCondition(const char* pszField, EItemCriteriaOperator eOp, const char* pszValue, bool bRequired)
			: CCondition(pszField, eOp, bRequired), m_sValue(pszValue)
		{
		}

		virtual ~CStringCondition() { }

		virtual	const char* GetValue(void) { return m_sValue.Get(); }

	protected:
		virtual bool BInternalEvaluate(KeyValues* pKVItem) const;
		virtual bool BSerializeToMsg(CSOItemCriteriaCondition& msg) const;

		CUtlString		m_sValue;
	};


	class CFloatCondition : public CCondition
	{
	public:
		CFloatCondition(const char* pszField, EItemCriteriaOperator eOp, float flValue, bool bRequired)
			: CCondition(pszField, eOp, bRequired), m_flValue(flValue)
		{
		}

		virtual ~CFloatCondition() { }

	protected:
		virtual bool BInternalEvaluate(KeyValues* pKVItem) const;
		virtual bool BSerializeToMsg(CSOItemCriteriaCondition& msg) const;

		float				m_flValue;
	};


	class CSetCondition : public CCondition
	{
	public:
		CSetCondition(const char* pszField, EItemCriteriaOperator eOp, const char* pszValue, bool bRequired)
			: CCondition(pszField, eOp, bRequired), m_sValue(pszValue)
		{
		}

		virtual ~CSetCondition() { }

	protected:
		virtual bool BInternalEvaluate(KeyValues* pKVItem) const;

		virtual bool BSerializeToMsg(CSOItemCriteriaCondition& msg) const;

		CUtlString			m_sValue;
	};

	bool			m_bItemLevelSet;
	uint32			m_unItemLevel;
	bool			m_bQualitySet;
	int32			m_nItemQuality;
	bool			m_bRaritySet;
	int32			m_nItemRarity;
	uint32			m_unInitialInventory;
	uint32			m_unInitialQuantity;
	bool			m_bForcedQualityMatch;
	bool			m_bIgnoreEnabledFlag;
	bool			m_bRecentOnly;
	bool			m_bIsLootList;

	CUtlVector<CCondition*>	m_vecConditions;
};


#endif 