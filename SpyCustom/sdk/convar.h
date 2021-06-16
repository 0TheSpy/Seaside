#ifndef CONVAR_H
#define CONVAR_H

#if _WIN32
#pragma once
#endif

#include "dbg.h"
#include "iconvar.h"
#include "utlvector.h"
#include "utlstring.h"
#include "icvar.h"

#ifdef _WIN32
#define FORCEINLINE_CVAR FORCEINLINE
#elif POSIX
#define FORCEINLINE_CVAR inline
#else
#error "implement me"
#endif

typedef unsigned int       uint32_t;

class ConVar;
class CCommand;
class ConCommand;
class ConCommandBase;
struct characterset_t;



class IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase* pVar) = 0;
};


#if defined( _X360 ) && !defined( _RETAIL )
void ConVar_PublishToVXConsole();
#endif


typedef void (*FnCommandCallbackVoid_t)(void);
typedef void (*FnCommandCallback_t)(const CCommand& command);

#define COMMAND_COMPLETION_MAXITEMS		64
#define COMMAND_COMPLETION_ITEM_LENGTH	64

typedef int  (*FnCommandCompletionCallback)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);


class ICommandCallback
{
public:
	virtual void CommandCallback(const CCommand& command) = 0;
};

class ICommandCompletionCallback
{
public:
	virtual int  CommandCompletionCallback(const char* pPartial, CUtlVector< CUtlString >& commands) = 0;
};

class ConCommandBase
{
	friend class CCvar;
	friend class ConVar;
	friend class ConCommand;
	friend void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor* pAccessor);
	friend class CDefaultCvar;

public:
	ConCommandBase(void);
	ConCommandBase(const char* pName, const char* pHelpString = 0,
		int flags = 0);

	virtual						~ConCommandBase(void);

	virtual	bool				IsCommand(void) const;

	virtual bool				IsFlagSet(int flag) const;
	virtual void				AddFlags(int flags);
	virtual void                RemoveFlags(int flags); 
	virtual int                 GetFlags() const;
	virtual const char* GetName(void) const;

	virtual const char* GetHelpText(void) const;

	const ConCommandBase* GetNext(void) const;
	ConCommandBase* GetNext(void);

	virtual bool				IsRegistered(void) const;

	virtual CVarDLLIdentifier_t	GetDLLIdentifier() const;

	virtual void				Create(const char* pName, const char* pHelpString = 0,
		int flags = 0);

	virtual void				Init();
	void						Shutdown();

	char* CopyString(const char* from);

	ConCommandBase* m_pNext;

	bool						m_bRegistered;

	const char* m_pszName;
	const char* m_pszHelpString;

	int							m_nFlags;

protected:
	static ConCommandBase* s_pConCommandBases;

	static IConCommandBaseAccessor* s_pAccessor;
};


class CCommand
{
public:
	CCommand();
	CCommand(int nArgC, const char** ppArgV);
	bool Tokenize(const char* pCommand, characterset_t* pBreakSet = NULL);
	void Reset();

	int ArgC() const;
	const char** ArgV() const;
	const char* ArgS() const;					           
	const char* GetCommandString() const;		          
	const char* operator[](int nIndex) const;	   
	const char* Arg(int nIndex) const;		   

	const char* FindArg(const char* pName) const;
	int FindArgInt(const char* pName, int nDefaultVal) const;

	static int MaxCommandLength();
	static characterset_t* DefaultBreakSet();

private:
	enum
	{
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512,
	};

	int		m_nArgc;
	int		m_nArgv0Size;
	char	m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char	m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];
};

inline int CCommand::MaxCommandLength()
{
	return COMMAND_MAX_LENGTH - 1;
}

inline int CCommand::ArgC() const
{
	return m_nArgc;
}

inline const char** CCommand::ArgV() const
{
	return m_nArgc ? (const char**)m_ppArgv : NULL;
}

inline const char* CCommand::ArgS() const
{
	return m_nArgv0Size ? &m_pArgSBuffer[m_nArgv0Size] : "";
}

inline const char* CCommand::GetCommandString() const
{
	return m_nArgc ? m_pArgSBuffer : "";
}

inline const char* CCommand::Arg(int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_nArgc)
		return "";
	return m_ppArgv[nIndex];
}

inline const char* CCommand::operator[](int nIndex) const
{
	return Arg(nIndex);
}


class ConCommand : public ConCommandBase
{
	friend class CCvar;

public:
	typedef ConCommandBase BaseClass;

	ConCommand(const char* pName, FnCommandCallbackVoid_t callback,
		const char* pHelpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0);
	ConCommand(const char* pName, FnCommandCallback_t callback,
		const char* pHelpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0);
	ConCommand(const char* pName, ICommandCallback* pCallback,
		const char* pHelpString = 0, int flags = 0, ICommandCompletionCallback* pCommandCompletionCallback = 0);

	virtual ~ConCommand(void);

	virtual	bool IsCommand(void) const;

	virtual int AutoCompleteSuggest(const char* partial, CUtlVector< CUtlString >& commands);

	virtual bool CanAutoComplete(void);

	virtual void Dispatch(const CCommand& command);

private:
	union
	{
		FnCommandCallbackVoid_t m_fnCommandCallbackV1;
		FnCommandCallback_t m_fnCommandCallback;
		ICommandCallback* m_pCommandCallback;
	};

	union
	{
		FnCommandCompletionCallback	m_fnCompletionCallback;
		ICommandCompletionCallback* m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback : 1;
	bool m_bUsingNewCommandCallback : 1;
	bool m_bUsingCommandCallbackInterface : 1;
};


class ConVar : public ConCommandBase, public IConVar
{
	friend class CCvar;
	friend class ConVarRef;

public:
	typedef ConCommandBase BaseClass;

	ConVar(const char* pName, const char* pDefaultValue, int flags = 0);

	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString);
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax);
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, FnChangeCallback_t callback);
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax,
		FnChangeCallback_t callback);

	virtual						~ConVar(void);
	
	virtual bool				IsFlagSet(int flag) const;
	virtual const char* GetHelpText(void) const;
	virtual bool				IsRegistered(void) const;
	virtual const char* GetName(void) const;
	virtual const char* GetBaseName(void) const; 
	virtual int                 GetSplitScreenPlayerSlot() const; 
	virtual void				AddFlags(int flags);
	virtual int                 GetFlags() const;
	virtual	bool				IsCommand(void) const;
	
	void InstallChangeCallback(FnChangeCallback_t callback, bool bInvoke = true);

	void RemoveChangeCallback(FnChangeCallback_t callbackToRemove); 

	int GetChangeCallbackCount() const { return m_pParent->m_fnChangeCallbacks.Count(); } 
	FnChangeCallback_t GetChangeCallback(int slot) const { return m_pParent->m_fnChangeCallbacks[slot]; } 


	FORCEINLINE_CVAR float			GetFloat(void) const;
	FORCEINLINE_CVAR int			GetInt(void) const;
	FORCEINLINE_CVAR Color          GetColor(void) const; 
	FORCEINLINE_CVAR bool			GetBool() const { return !!GetInt(); }
	FORCEINLINE_CVAR char const* GetString(void) const;

	template <typename T> T Get(void) const;
	template <typename T> T Get(T*) const;

	virtual void				SetValue(const char* value);
	virtual void				SetValue(float value);
	virtual void				SetValue(int value);
	virtual void                    SetValue(Color value); 

	void						Revert(void);
	bool                            HasMin() const; 
	bool                            HasMax() const; 

	struct CVValue_t
	{
		char* m_pszString;
		int     m_StringLength;
		float   m_fValue;
		int     m_nValue;
	};

	bool						GetMin(float& minVal) const;
	bool						GetMax(float& maxVal) const;
	float                           GetMinValue() const; 
	float                           GetMaxValue() const; 
	const char* GetDefault(void) const;
	FORCEINLINE_CVAR CVValue_t& GetRawValue()
	{
		return m_Value;
	}
	FORCEINLINE_CVAR const CVValue_t& GetRawValue() const
	{
		return m_Value;
	}

	bool                        InternalSetColorFromString(const char* value); 
	virtual void				InternalSetValue(const char* value);
	virtual void				InternalSetFloatValue(float fNewValue);
	virtual void				InternalSetIntValue(int nValue);
	virtual void                InternalSetColorValue(Color value);
	virtual bool				ClampValue(float& value);
	virtual void				ChangeStringValue(const char* tempVal, float flOldValue);

	virtual void				Create(const char* pName, const char* pDefaultValue, int flags = 0,
		const char* pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0);

	virtual void				Init();
	int GetFlags() { return m_pParent->m_nFlags; }
public:

	ConVar* m_pParent;

	const char* m_pszDefaultValue;

	CVValue_t                   m_Value; 
	bool						m_bHasMin;
	float						m_fMinVal;
	bool						m_bHasMax;
	float						m_fMaxVal;

	CUtlVector<FnChangeCallback_t> m_fnChangeCallbacks;
};


FORCEINLINE_CVAR float ConVar::GetFloat(void) const
{
	uint32_t xored = *(uint32_t*)&m_pParent->m_Value.m_fValue ^ (uint32_t)this;
	return *(float*)&xored;
}

FORCEINLINE_CVAR int ConVar::GetInt(void) const
{
	return (int)(m_pParent->m_Value.m_nValue ^ (int)this);
}


FORCEINLINE_CVAR const char* ConVar::GetString(void) const
{
	if (m_nFlags & FCVAR_NEVER_AS_STRING)
		return "FCVAR_NEVER_AS_STRING";
	char const* str = m_pParent->m_Value.m_pszString;
	return str ? str : "";
}

class ConVarRef
{
public:
	ConVarRef(const char* pName);
	ConVarRef(const char* pName, bool bIgnoreMissing);
	ConVarRef(IConVar* pConVar);

	void Init(const char* pName, bool bIgnoreMissing);
	bool IsValid() const;
	bool IsFlagSet(int nFlags) const;
	IConVar* GetLinkedConVar();

	float GetFloat(void) const;
	int GetInt(void) const;
	bool GetBool() const { return !!GetInt(); }
	const char* GetString(void) const;

	void SetValue(const char* pValue);
	void SetValue(float flValue);
	void SetValue(int nValue);
	void SetValue(bool bValue);

	const char* GetName() const;

	const char* GetDefault() const;

private:
	IConVar* m_pConVar;
	ConVar* m_pConVarState;
};


FORCEINLINE_CVAR bool ConVarRef::IsFlagSet(int nFlags) const
{
	return (m_pConVar->IsFlagSet(nFlags) != 0);
}

FORCEINLINE_CVAR IConVar* ConVarRef::GetLinkedConVar()
{
	return m_pConVar;
}

FORCEINLINE_CVAR const char* ConVarRef::GetName() const
{
	return m_pConVar->GetName();
}

FORCEINLINE_CVAR void ConVarRef::SetValue(const char* pValue)
{
	m_pConVar->SetValue(pValue);
}

FORCEINLINE_CVAR void ConVarRef::SetValue(float flValue)
{
	m_pConVar->SetValue(flValue);
}

FORCEINLINE_CVAR void ConVarRef::SetValue(int nValue)
{
	m_pConVar->SetValue(nValue);
}

FORCEINLINE_CVAR void ConVarRef::SetValue(bool bValue)
{
	m_pConVar->SetValue(bValue ? 1 : 0);
}

FORCEINLINE_CVAR const char* ConVarRef::GetDefault() const
{
	return m_pConVarState->m_pszDefaultValue;
}


void ConVar_Register(int nCVarFlag = 0, IConCommandBaseAccessor* pAccessor = NULL);
void ConVar_Unregister();


void ConVar_PrintFlags(const ConCommandBase* var);
void ConVar_PrintDescription(const ConCommandBase* pVar);


#pragma warning (disable : 4355 )

template< class T >
class CConCommandMemberAccessor : public ConCommand, public ICommandCallback, public ICommandCompletionCallback
{
	typedef ConCommand BaseClass;
	typedef void (T::* FnMemberCommandCallback_t)(const CCommand& command);
	typedef int  (T::* FnMemberCommandCompletionCallback_t)(const char* pPartial, CUtlVector< CUtlString >& commands);

public:
	CConCommandMemberAccessor(T* pOwner, const char* pName, FnMemberCommandCallback_t callback, const char* pHelpString = 0,
		int flags = 0, FnMemberCommandCompletionCallback_t completionFunc = 0) :
		BaseClass(pName, this, pHelpString, flags, (completionFunc != 0) ? this : NULL)
	{
		m_pOwner = pOwner;
		m_Func = callback;
		m_CompletionFunc = completionFunc;
	}

	~CConCommandMemberAccessor()
	{
		Shutdown();
	}

	void SetOwner(T* pOwner)
	{
		m_pOwner = pOwner;
	}

	virtual void CommandCallback(const CCommand& command)
	{
		Assert(m_pOwner && m_Func);
		(m_pOwner->*m_Func)(command);
	}

	virtual int  CommandCompletionCallback(const char* pPartial, CUtlVector< CUtlString >& commands)
	{
		Assert(m_pOwner && m_CompletionFunc);
		return (m_pOwner->*m_CompletionFunc)(pPartial, commands);
	}

private:
	T* m_pOwner;
	FnMemberCommandCallback_t m_Func;
	FnMemberCommandCompletionCallback_t m_CompletionFunc;
};

#pragma warning ( default : 4355 )


#define CON_COMMAND( name, description ) \
   static void name( const CCommand &args ); \
   static ConCommand name##_command( #name, name, description ); \
   static void name( const CCommand &args )

#define CON_COMMAND_F( name, description, flags ) \
   static void name( const CCommand &args ); \
   static ConCommand name##_command( #name, name, description, flags ); \
   static void name( const CCommand &args )

#define CON_COMMAND_F_COMPLETION( name, description, flags, completion ) \
	static void name( const CCommand &args ); \
	static ConCommand name##_command( #name, name, description, flags, completion ); \
	static void name( const CCommand &args )

#define CON_COMMAND_EXTERN( name, _funcname, description ) \
	void _funcname( const CCommand &args ); \
	static ConCommand name##_command( #name, _funcname, description ); \
	void _funcname( const CCommand &args )

#define CON_COMMAND_EXTERN_F( name, _funcname, description, flags ) \
	void _funcname( const CCommand &args ); \
	static ConCommand name##_command( #name, _funcname, description, flags ); \
	void _funcname( const CCommand &args )

#define CON_COMMAND_MEMBER_F( _thisclass, name, _funcname, description, flags ) \
	void _funcname( const CCommand &args );						\
	friend class CCommandMemberInitializer_##_funcname;			\
	class CCommandMemberInitializer_##_funcname					\
	{															\
	public:														\
		CCommandMemberInitializer_##_funcname() : m_ConCommandAccessor( NULL, name, &_thisclass::_funcname, description, flags )	\
		{														\
			m_ConCommandAccessor.SetOwner( GET_OUTER( _thisclass, m_##_funcname##_register ) );	\
		}														\
	private:													\
		CConCommandMemberAccessor< _thisclass > m_ConCommandAccessor;	\
	};															\
																\
	CCommandMemberInitializer_##_funcname m_##_funcname##_register;		\


#endif  