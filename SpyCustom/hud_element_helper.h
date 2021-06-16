#ifndef HUD_ELEMENT_HELPER_H
#define HUD_ELEMENT_HELPER_H
#ifdef _WIN32
#pragma once
#endif

class CHudElement;

class CHudElementHelper
{
public:
	static CHudElementHelper* m_sHelpers;
	static void CreateAllElements(void);

public:
	CHudElementHelper(CHudElement* (*pfnCreate)(void), int depth);

	CHudElementHelper* GetNext(void);

private:
	CHudElementHelper* m_pNext;
	CHudElement* (*m_pfnCreate)(void);

	int					m_iDepth;
};

#define DECLARE_HUDELEMENT( className )											\
	static CHudElement *Create_##className( void )							\
		{																		\
			return new className( #className );									\
		};																		\
	static CHudElementHelper g_##className##_Helper( Create_##className, 50 );

#define DECLARE_HUDELEMENT_DEPTH( className, depth )											\
	static CHudElement *Create_##className( void )							\
		{																		\
			return new className( #className );									\
		};																		\
	static CHudElementHelper g_##className##_Helper( Create_##className, depth );

#define DECLARE_NAMED_HUDELEMENT( className, panelName )						\
	static CHudElement *Create_##panelName( void )							\
		{																		\
			return new className( #panelName );									\
		};																		\
	static CHudElementHelper g_##panelName##_Helper( Create_##panelName, 50 );

#define GET_HUDELEMENT( className )												\
	( className *)gHUD.FindElement( #className )

#define GET_NAMED_HUDELEMENT( className, panelName )							\
	( className *)gHUD.FindElement( #panelName )


#define DECLARE_MULTIPLY_INHERITED()							\
    void *operator new( size_t stAllocateBlock )				\
	{															\
		return CHudElement::operator new ( stAllocateBlock );	\
	}															\
	void* operator new( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine )	\
	{ 																								\
		return CHudElement::operator new ( stAllocateBlock, nBlockUse, pFileName, nLine );	\
	}															\
	void operator delete( void *pMem )							\
	{															\
		CHudElement::operator delete ( pMem );					\
	}															\
	void operator delete( void *pMem, int nBlockUse, const char *pFileName, int nLine )	\
	{															\
		CHudElement::operator delete ( pMem, nBlockUse, pFileName, nLine );	\
	}

#define IMPLEMENT_OPERATORS_NEW_AND_DELETE			DECLARE_MULTIPLY_INHERITED

#endif  