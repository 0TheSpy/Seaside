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
#undef new

class CHudElement : public CGameEventListener
{
public:
	DECLARE_CLASS_NOBASE(CHudElement);

	CHudElement(const char* pElementName);
	virtual						~CHudElement();

	virtual void				Init(void) { return; }

	virtual void				VidInit(void) { return; }

	virtual void				LevelInit(void) { return; };
	virtual void				LevelShutdown(void) { return; };

	virtual void				Reset(void) { return; }

	virtual void				ProcessInput(void) { return; }
	virtual const char* GetName(void) const { return m_pElementName; };

	virtual bool				ShouldDraw(void);

	virtual bool				IsActive(void) { return m_bActive; };
	virtual void				SetActive(bool bActive);

	virtual void				SetHiddenBits(int iBits);

	bool						IsParentedToClientDLLRootPanel() const;
	void						SetParentedToClientDLLRootPanel(bool parented);

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

public:   

	virtual void FireGameEvent(IGameEvent* event) {}

public:

	bool						m_bActive;

protected:
	int							m_iHiddenBits;

private:
	const char* m_pElementName;
	bool						m_bNeedsRemove;
	bool						m_bIsParentedToClientDLLRootPanel;

	CUtlVector< int >			m_HudRenderGroups;
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