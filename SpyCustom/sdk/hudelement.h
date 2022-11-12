#if !defined( HUDELEMENT_H )
#define HUDELEMENT_H
#ifdef _WIN32
#pragma once
#endif

#include "hud.h"
#include "hud_element_helper.h"
#include "networkvar.h"
#include "GameEventListener.h"
#include "memdbgon.h"

#include "panel2d.h"

#undef new

class CHudElement : public CGameEventListener
{
public:
	DECLARE_CLASS_NOBASE(CHudElement);

								CHudElement() {}
	explicit 					CHudElement(const char* pElementName);
	virtual						~CHudElement();

	virtual void				SetHud(CHud* pHud);

	virtual void				Init(void) { return; }

	virtual void				VidInit(void) { return; }

	virtual void				LevelInit(void) { return; };
	virtual void				LevelShutdown(void) { return; };

	virtual void				Reset(void) { return; }

	virtual void				ProcessInput(void) { return; }

	// Called once per frame whether the element is visible or not
	virtual void				Think(void) { return; }

	// Called when time warping occurs, i.e. when instant replay rewinds or forwards client's time
	virtual void				OnTimeJump(void) { return; }

	virtual const char* GetName(void) const { return m_pElementName; };

	virtual bool				ShouldDraw(void);

	virtual bool				IsActive(void) { return m_bActive; };
	virtual void				SetActive(bool bActive);

	virtual void				SetHiddenBits(int iBits);

	virtual void				SetIgnoreGlobalHudDisable(bool hide);
	virtual bool				GetIgnoreGlobalHudDisable(void);

	bool						IsParentedToClientDLLRootPanel() const;
	void						SetParentedToClientDLLRootPanel(bool parented);

	// Return true if this HUD element expects an entry in  HudLayout.res
	virtual bool				WantsHudLayoutEntry(void) const { return true; }

	void* operator new(size_t stAllocateBlock)
	{
		Assert(stAllocateBlock != 0);
		void* pMem = malloc(stAllocateBlock);
		memset(pMem, 0, stAllocateBlock);
		return pMem;
	}

	void* operator new(size_t stAllocateBlock, int nBlockUse, const char* pFileName, int nLine)
	{
		Assert(stAllocateBlock != 0);
		void* pMem = MemAlloc_Alloc(stAllocateBlock, pFileName, nLine);
		memset(pMem, 0, stAllocateBlock);
		return pMem;
	}

	void operator delete(void* pMem)
	{
#if defined( _DEBUG )
		int size = _msize(pMem);
		memset(pMem, 0xcd, size);
#endif
		free(pMem);
	}

	void operator delete(void* pMem, int nBlockUse, const char* pFileName, int nLine)
	{
		operator delete(pMem);
	}

	void SetNeedsRemove(bool needsremove);

	void RegisterForRenderGroup(const char* pszName);
	void UnregisterForRenderGroup(const char* pszGroupName);
	void HideLowerPriorityHudElementsInGroup(const char* pszGroupName);
	void UnhideLowerPriorityHudElementsInGroup(const char* pszGroupName);

	virtual int	GetRenderGroupPriority();

	void SetSplitScreenPlayerSlot(int nSlot);
	int GetSplitScreenPlayerSlot() const;

	virtual void OnSplitScreenStateChanged() {}

public:   

	virtual void FireGameEvent(IGameEvent* event) {}

//protected:
	void InitCHudElementAfterConstruction(const char* pElementName);

public:

	bool						m_bActive;

//protected:
	int							m_iHiddenBits;
	int							m_nSplitScreenPlayerSlot;
	bool						m_ignoreGlobalHudDisable;

//private:
	const char*					m_pElementName;
	bool						m_bNeedsRemove;
	bool						m_bIsParentedToClientDLLRootPanel;

	CUtlVector< int >			m_HudRenderGroups;
	CHud* m_pHud;

	//
	panorama::CPanel2D* GetPanel2D()
	{
		return reinterpret_cast<panorama::CPanel2D*>(uintptr_t(this) - 0x14);
	}
};

#include "utlpriorityqueue.h"

inline bool RenderGroupLessFunc(CHudElement* const& lhs, CHudElement* const& rhs)
{
	return (lhs->GetRenderGroupPriority() < rhs->GetRenderGroupPriority());
}

class CHudRenderGroup
{
public:
	CHudRenderGroup()
	{
		m_pLockingElements.SetLessFunc(RenderGroupLessFunc);
		bHidden = false;
	}

	bool bHidden;
	CUtlPriorityQueue< CHudElement* >	m_pLockingElements;
};

#include "memdbgoff.h"

#endif  