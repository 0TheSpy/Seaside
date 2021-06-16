#ifndef C_BASETEMPENTITY_H
#define C_BASETEMPENTITY_H
#ifdef _WIN32
#pragma once
#endif


#include "client_class.h"
#include "iclientnetworkable.h"
#include "c_recipientfilter.h"


class C_BaseTempEntity : public IClientUnknown, public IClientNetworkable

{
public:
	DECLARE_CLASS_NOBASE(C_BaseTempEntity);
	DECLARE_CLIENTCLASS();

	C_BaseTempEntity(void);
	virtual							~C_BaseTempEntity(void);


public:

	virtual void SetRefEHandle(const CBaseHandle& handle) { Assert(false); }
	virtual const CBaseHandle& GetRefEHandle() const { return *((CBaseHandle*)0); }

	virtual IClientUnknown* GetIClientUnknown() { return this; }
	virtual ICollideable* GetCollideable() { return 0; }
	virtual IClientNetworkable* GetClientNetworkable() { return this; }
	virtual IClientRenderable* GetClientRenderable() { return 0; }
	virtual IClientEntity* GetIClientEntity() { return 0; }
	virtual C_BaseEntity* GetBaseEntity() { return 0; }
	virtual IClientThinkable* GetClientThinkable() { return 0; }
	virtual IClientModelRenderable* GetClientModelRenderable() { return 0; }
	virtual IClientAlphaProperty* GetClientAlphaProperty() { return 0; }

public:

	virtual void					Release();
	virtual void					NotifyShouldTransmit(ShouldTransmitState_t state);
	virtual void					PreDataUpdate(DataUpdateType_t updateType);
	virtual void					PostDataUpdate(DataUpdateType_t updateType);
	virtual void					OnPreDataChanged(DataUpdateType_t updateType);
	virtual void					OnDataChanged(DataUpdateType_t updateType);
	virtual void					SetDormant(bool bDormant);
	virtual bool					IsDormant(void);
	virtual int						entindex(void) const;
	virtual void					ReceiveMessage(int classID, bf_read& msg);
	virtual void* GetDataTableBasePtr();
	virtual void					SetDestroyedOnRecreateEntities(void);

public:

	void NetworkStateChanged() {}
	void NetworkStateChanged(void* pVar) {}

	virtual bool					Init(int entnum, int iSerialNum);

	virtual void					Precache(void);

	virtual bool					ShouldDestroy(void) { return false; };

	C_BaseTempEntity* GetNext(void);

	static C_BaseTempEntity* GetList(void);

	C_BaseTempEntity* GetNextDynamic(void);

	void	GetColorModulation(float* color)
	{
		assert(color);
		color[0] = color[1] = color[2] = 1.0f;
	}

	virtual bool	ShouldReceiveProjectedTextures(int flags) { return false; }

public:
	static C_BaseTempEntity* GetDynamicList();

	static void						PrecacheTempEnts(void);

	static void						ClearDynamicTempEnts(void);

	static void						CheckDynamicTempEnts(void);

private:

	C_BaseTempEntity* m_pNext;
	C_BaseTempEntity* m_pNextDynamic;

	static C_BaseTempEntity* s_pTempEntities;
	static C_BaseTempEntity* s_pDynamicEntities;
};


#endif  