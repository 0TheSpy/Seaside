#ifndef ECONITEMINTERFACE_H
#define ECONITEMINTERFACE_H

#ifdef _WIN32
#pragma once
#endif

#include "game_item_schema.h"			   

class CAttribute_String;

template < typename T >
struct StripConstIfPresent { typedef T ResultType; };

template < typename T > struct StripConstIfPresent<const T> { typedef T ResultType; };

template < typename T, typename U >
struct AreTypesIdentical { enum { kValue = false }; };

template < typename T > struct AreTypesIdentical<T, T> { enum { kValue = true }; };

template < typename T>
struct IsValidAttributeValueTypeImpl { enum { kValue = false }; };

template < > struct IsValidAttributeValueTypeImpl<attrib_value_t> { enum { kValue = true }; };
template < > struct IsValidAttributeValueTypeImpl<float> { enum { kValue = true }; };
template < > struct IsValidAttributeValueTypeImpl<CAttribute_String> { enum { kValue = true }; };
template < > struct IsValidAttributeValueTypeImpl<Vector> { enum { kValue = true }; };

template < typename T >
struct IsValidAttributeValueType : public IsValidAttributeValueTypeImpl< typename StripConstIfPresent<T>::ResultType > { };

class IEconItemAttributeIterator
{
public:
	virtual ~IEconItemAttributeIterator() { }

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, attrib_value_t value) = 0;
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, float value) = 0;
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const CAttribute_String& value) = 0;
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const Vector& value) = 0;
};

class CEconItemSpecificAttributeIterator : public IEconItemAttributeIterator
{
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, attrib_value_t value) { return true; }
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const CAttribute_String& value) { return true; }
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const Vector& value) { return true; }
};


class IEconItemUntypedAttributeIterator : public IEconItemAttributeIterator
{
public:
	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, attrib_value_t) OVERRIDE
	{
		return OnIterateAttributeValueUntyped(pAttrDef);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, float) OVERRIDE
	{
		return OnIterateAttributeValueUntyped(pAttrDef);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const CAttribute_String&) OVERRIDE
	{
		return OnIterateAttributeValueUntyped(pAttrDef);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const Vector&) OVERRIDE
	{
		return OnIterateAttributeValueUntyped(pAttrDef);
	}

private:
	virtual bool OnIterateAttributeValueUntyped(const CEconItemAttributeDefinition* pAttrDef) = 0;
};

class CAttributeIterator_HasAttribute : public IEconItemUntypedAttributeIterator
{
public:
	CAttributeIterator_HasAttribute(const CEconItemAttributeDefinition* pAttrDef)
		: m_pAttrDef(pAttrDef)
		, m_bFound(false)
	{
		Assert(m_pAttrDef);
	}

	bool WasFound() const
	{
		return m_bFound;
	}

private:
	bool OnIterateAttributeValueUntyped(const CEconItemAttributeDefinition* pAttrDef) OVERRIDE
	{
		if (m_pAttrDef == pAttrDef)
		{
			m_bFound = true;
		}

		return !m_bFound;
	}

private:
	const CEconItemAttributeDefinition* m_pAttrDef;
	bool m_bFound;
};

template < typename TActualTypeInMemory, typename TTreatAsThisType = TActualTypeInMemory >
class CAttributeIterator_GetTypedAttributeValue : public IEconItemAttributeIterator
{
public:
	CAttributeIterator_GetTypedAttributeValue(const CEconItemAttributeDefinition* pAttrDef, TTreatAsThisType* outpValue)
		: m_pAttrDef(pAttrDef)
		, m_outpValue(outpValue)
		, m_bFound(false)
	{
		COMPILE_TIME_ASSERT(IsValidAttributeValueType<TActualTypeInMemory>::kValue);

		COMPILE_TIME_ASSERT(((AreTypesIdentical<TActualTypeInMemory, attrib_value_t>::kValue && AreTypesIdentical<TTreatAsThisType, float>::kValue) ||
			(AreTypesIdentical<TActualTypeInMemory, CAttribute_String>::kValue && AreTypesIdentical<TTreatAsThisType, const char*>::kValue) ||
			AreTypesIdentical<TActualTypeInMemory, TTreatAsThisType>::kValue));

		Assert(m_pAttrDef);
		Assert(outpValue);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, attrib_value_t value) OVERRIDE
	{
		return OnIterateAttributeValueTyped(pAttrDef, value);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, float value) OVERRIDE
	{
		return OnIterateAttributeValueTyped(pAttrDef, value);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const CAttribute_String& value) OVERRIDE
	{
		return OnIterateAttributeValueTyped(pAttrDef, value);
	}

	virtual bool OnIterateAttributeValue(const CEconItemAttributeDefinition* pAttrDef, const Vector& value) OVERRIDE
	{
		return OnIterateAttributeValueTyped(pAttrDef, value);
	}

	bool WasFound() const
	{
		return m_bFound;
	}

private:
	template < typename TAnyOtherType >
	bool OnIterateAttributeValueTyped(const CEconItemAttributeDefinition* pAttrDef, const TAnyOtherType& value)
	{
		COMPILE_TIME_ASSERT(IsValidAttributeValueType<TAnyOtherType>::kValue);

		AssertMsg(m_pAttrDef != pAttrDef, "Incorrect type found for attribute during iteration.");

		return true;
	}

	bool OnIterateAttributeValueTyped(const CEconItemAttributeDefinition* pAttrDef, const TActualTypeInMemory& value)
	{
		if (m_pAttrDef == pAttrDef)
		{
			m_bFound = true;
			CopyAttributeValueToOutput(&value, reinterpret_cast<TTreatAsThisType*>(m_outpValue));
		}

		return !m_bFound;
	}

private:
	static void CopyAttributeValueToOutput(const TActualTypeInMemory* pValue, TTreatAsThisType* out_pValue)
	{
		COMPILE_TIME_ASSERT(sizeof(TActualTypeInMemory) == sizeof(TTreatAsThisType));

		Assert(pValue);
		Assert(out_pValue);

		*out_pValue = *reinterpret_cast<const TTreatAsThisType*>(pValue);
	}

private:
	const CEconItemAttributeDefinition* m_pAttrDef;
	TTreatAsThisType* m_outpValue;
	bool m_bFound;
};

void CopyStringAttributeValueToCharPointerOutput(const CAttribute_String* pValue, const char** out_pValue);

template < >
inline void CAttributeIterator_GetTypedAttributeValue<CAttribute_String, const char*>::CopyAttributeValueToOutput(const CAttribute_String* pValue, const char** out_pValue)
{
	CopyStringAttributeValueToCharPointerOutput(pValue, out_pValue);
}

template < typename TAttributeContainerType >
bool FindAttribute(const TAttributeContainerType* pSomethingThatHasAnIterateAttributesFunction, const CEconItemAttributeDefinition* pAttrDef)
{
	if (!pAttrDef)
		return false;

	CAttributeIterator_HasAttribute it(pAttrDef);
	pSomethingThatHasAnIterateAttributesFunction->IterateAttributes(&it);
	return it.WasFound();
}

template < typename TActualTypeInMemory, typename TTreatAsThisType, typename TAttributeContainerType >
bool FindAttribute_UnsafeBitwiseCast(const TAttributeContainerType* pSomethingThatHasAnIterateAttributesFunction, const CEconItemAttributeDefinition* pAttrDef, TTreatAsThisType* out_pValue)
{
	if (!pAttrDef)
		return false;

	CAttributeIterator_GetTypedAttributeValue<TActualTypeInMemory, TTreatAsThisType> it(pAttrDef, out_pValue);
	pSomethingThatHasAnIterateAttributesFunction->IterateAttributes(&it);
	return it.WasFound();
}

template < typename TAttributeContainerType, typename T >
bool FindAttribute(const TAttributeContainerType* pSomethingThatHasAnIterateAttributesFunction, const CEconItemAttributeDefinition* pAttrDef, T* out_pValue)
{
	return FindAttribute_UnsafeBitwiseCast<T, T, TAttributeContainerType>(pSomethingThatHasAnIterateAttributesFunction, pAttrDef, out_pValue);
}

class IEconItemInterface
{
public:
	virtual ~IEconItemInterface() { }

	bool FindAttribute(const CEconItemAttributeDefinition* pAttrDef) const
	{
		return ::FindAttribute(this, pAttrDef);
	}

	template < typename T >
	bool FindAttribute(const CEconItemAttributeDefinition* pAttrDef, T* out_pValue) const
	{
		return ::FindAttribute(this, pAttrDef, out_pValue);
	}

	virtual int GetCustomPaintKitIndex(void) const;
	virtual int GetCustomPaintKitSeed(void) const;
	virtual float GetCustomPaintKitWear(float flWearDefault = 0.0f) const;

	virtual float GetStickerAttributeBySlotIndexFloat(int nSlotIndex, EStickerAttributeType type, float flDefault) const;
	virtual uint32 GetStickerAttributeBySlotIndexInt(int nSlotIndex, EStickerAttributeType type, uint32 uiDefault) const;

	virtual bool IsTradable() const;
	virtual bool IsMarketable() const;
	virtual bool IsCommodity() const;
	virtual bool IsUsableInCrafting() const;
	virtual bool IsHiddenFromDropList() const;

	virtual RTime32 GetExpirationDate() const;

	virtual const GameItemDefinition_t* GetItemDefinition() const = 0;

	virtual uint32			GetAccountID() const = 0;
	virtual uint64			GetItemID() const = 0;
	virtual int32			GetQuality() const = 0;
	virtual int32			GetRarity() const = 0;
	virtual uint8			GetFlags() const = 0;
	virtual eEconItemOrigin GetOrigin() const = 0;
	virtual uint16			GetQuantity() const = 0;
	virtual uint32			GetItemLevel() const = 0;
	virtual bool			GetInUse() const = 0;			            

	virtual const char* GetCustomName() const = 0;		            
	virtual const char* GetCustomDesc() const = 0;		             

	virtual int				GetItemSetIndex() const = 0;

	virtual void IterateAttributes(IEconItemAttributeIterator* pIterator) const = 0;

	bool IsPotentiallyTradable() const;

	RTime32 GetCacheRefreshDateTime() const;

	RTime32 GetTradableAfterDateTime() const;
	RTime32 GetUseAfterDateTime() const;
};

#endif  