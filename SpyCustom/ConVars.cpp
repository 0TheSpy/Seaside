#include "sdk/convar.h"
#include <string>

#include "Interfaces.hpp"


static void V_strncpy(char* pDest, char const* pSrc, int maxLen)
{
	strncpy(pDest, pSrc, maxLen);
	if (maxLen > 0)
	{
		pDest[maxLen - 1] = 0;
	}
}

static int V_snprintf(char* pDest, int maxLen, const char* pFormat, ...)
{
	va_list marker;

	va_start(marker, pFormat);
#ifdef _WIN32
	int len = _vsnprintf(pDest, maxLen, pFormat, marker);
#elif POSIX
	int len = vsnprintf(pDest, maxLen, pFormat, marker);
#else
#error "define vsnprintf type."
#endif
	va_end(marker);

	if (len < 0)
	{
		len = maxLen;
		pDest[maxLen - 1] = 0;
	}

	return len;
}

CUtlBinaryBlock::CUtlBinaryBlock(int growSize, int initSize)
{
	MEM_ALLOC_CREDIT();
	m_Memory.Init(growSize, initSize);

	m_nActualLength = 0;
}

CUtlBinaryBlock::CUtlBinaryBlock(void* pMemory, int nSizeInBytes, int nInitialLength) : m_Memory((unsigned char*)pMemory, nSizeInBytes)
{
	m_nActualLength = nInitialLength;
}

CUtlBinaryBlock::CUtlBinaryBlock(const void* pMemory, int nSizeInBytes) : m_Memory((const unsigned char*)pMemory, nSizeInBytes)
{
	m_nActualLength = nSizeInBytes;
}

CUtlBinaryBlock::CUtlBinaryBlock(const CUtlBinaryBlock& src)
{
	Set(src.Get(), src.Length());
}

void CUtlBinaryBlock::SetLength(int nLength)
{
	MEM_ALLOC_CREDIT();
	Assert(!m_Memory.IsReadOnly());

	m_nActualLength = nLength;
	if (nLength > m_Memory.NumAllocated())
	{
		int nOverFlow = nLength - m_Memory.NumAllocated();
		m_Memory.Grow(nOverFlow);

		if (nLength > m_Memory.NumAllocated())
		{
			m_nActualLength = m_Memory.NumAllocated();
		}
	}

#ifdef _DEBUG
	if (m_Memory.NumAllocated() > m_nActualLength)
	{
		memset(((char*)m_Memory.Base()) + m_nActualLength, 0xEB, m_Memory.NumAllocated() - m_nActualLength);
	}
#endif
}

void CUtlBinaryBlock::Set(const void* pValue, int nLen)
{
	Assert(!m_Memory.IsReadOnly());

	if (!pValue)
	{
		nLen = 0;
	}

	SetLength(nLen);

	if (m_nActualLength)
	{
		if (((const char*)m_Memory.Base()) >= ((const char*)pValue) + nLen ||
			((const char*)m_Memory.Base()) + m_nActualLength <= ((const char*)pValue))
		{
			memcpy(m_Memory.Base(), pValue, m_nActualLength);
		}
		else
		{
			memmove(m_Memory.Base(), pValue, m_nActualLength);
		}
	}
}

CUtlString::CUtlString()
{
}

CUtlString::CUtlString(const char* pString)
{
	Set(pString);
}

CUtlString::CUtlString(const CUtlString& string)
{
	Set(string.Get());
}

CUtlString::CUtlString(void* pMemory, int nSizeInBytes, int nInitialLength) : m_Storage(pMemory, nSizeInBytes, nInitialLength)
{
}

CUtlString::CUtlString(const void* pMemory, int nSizeInBytes) : m_Storage(pMemory, nSizeInBytes)
{
}

void CUtlString::Set(const char* pValue)
{
	Assert(!m_Storage.IsReadOnly());
	int nLen = pValue ? Q_strlen(pValue) + 1 : 0;
	m_Storage.Set(pValue, nLen);
}

const char* CUtlString::Get() const
{
	if (m_Storage.Length() == 0)
	{
		return "";
	}

	return reinterpret_cast<const char*>(m_Storage.Get());
}

ConCommandBase* ConCommandBase::s_pConCommandBases = NULL;
IConCommandBaseAccessor* ConCommandBase::s_pAccessor = NULL;
static int s_nCVarFlag = 0;
static int s_nDLLIdentifier = -1;	          
static bool s_bRegistered = false;

class CDefaultAccessor : public IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase* pVar)
	{
		iff.g_pCVar->RegisterConCommand(pVar);
		return true;
	}
};

static CDefaultAccessor s_DefaultAccessor;

void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor* pAccessor)
{
	if (!iff.g_pCVar || s_bRegistered)
		return;

	Assert(s_nDLLIdentifier < 0);
	s_bRegistered = true;
	s_nCVarFlag = nCVarFlag;
	s_nDLLIdentifier = iff.g_pCVar->AllocateDLLIdentifier();

	ConCommandBase* pCur, * pNext;

	ConCommandBase::s_pAccessor = pAccessor ? pAccessor : &s_DefaultAccessor;
	pCur = ConCommandBase::s_pConCommandBases;
	while (pCur)
	{
		pNext = pCur->m_pNext;
		pCur->AddFlags(s_nCVarFlag);
		pCur->Init();
		pCur = pNext;
	}

	ConCommandBase::s_pConCommandBases = NULL;
}

void ConVar_Unregister()
{
	if (!iff.g_pCVar || !s_bRegistered)
		return;

	Assert(s_nDLLIdentifier >= 0);
	iff.g_pCVar->UnregisterConCommands(s_nDLLIdentifier);
	s_nDLLIdentifier = -1;
	s_bRegistered = false;
}


ConCommandBase::ConCommandBase(void)
{
	m_bRegistered = false;
	m_pszName = NULL;
	m_pszHelpString = NULL;

	m_nFlags = 0;
	m_pNext = NULL;
}

ConCommandBase::ConCommandBase(const char* pName, const char* pHelpString , int flags  )
{
	Create(pName, pHelpString, flags);
}

ConCommandBase::~ConCommandBase(void)
{
}

bool ConCommandBase::IsCommand(void) const
{
	return true;
}


CVarDLLIdentifier_t ConCommandBase::GetDLLIdentifier() const
{
	return s_nDLLIdentifier;
}


void ConCommandBase::Create(const char* pName, const char* pHelpString  , int flags  )
{
	static char* empty_string = (char*)"";

	m_bRegistered = false;

	Assert(pName);
	m_pszName = pName;
	m_pszHelpString = pHelpString ? pHelpString : empty_string;

	m_nFlags = flags;

#ifdef ALLOW_DEVELOPMENT_CVARS
	m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
#endif

	if (!(m_nFlags & FCVAR_UNREGISTERED))
	{
		m_pNext = s_pConCommandBases;
		s_pConCommandBases = this;
	}
	else
	{
		m_pNext = NULL;
	}

	if (s_pAccessor)
	{
		Init();
	}
}


void ConCommandBase::Init()
{
	if (s_pAccessor)
	{
		s_pAccessor->RegisterConCommandBase(this);
	}
}

void ConCommandBase::Shutdown()
{
	if (iff.g_pCVar)
	{
		iff.g_pCVar->UnregisterConCommand(this);
	}
}


const char* ConCommandBase::GetName(void) const
{
	return m_pszName;
}


bool ConCommandBase::IsFlagSet(int flag) const
{
	return (flag & m_nFlags) ? true : false;
}

void ConCommandBase::AddFlags(int flags)
{
	m_nFlags |= flags;

#ifdef ALLOW_DEVELOPMENT_CVARS
	m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
#endif
}

void ConCommandBase::RemoveFlags(int flags)
{
	m_nFlags &= ~flags;
}


int ConCommandBase::GetFlags(void) const
{
	return m_nFlags;
}


const ConCommandBase* ConCommandBase::GetNext(void) const
{
	return m_pNext;
}

ConCommandBase* ConCommandBase::GetNext(void)
{
	return m_pNext;
}


char* ConCommandBase::CopyString(const char* from)
{
	int		len;
	char* to;

	len = V_strlen(from);
	if (len <= 0)
	{
		to = new char[1];
		to[0] = 0;
	}
	else
	{
		to = new char[len + 1];
		Q_strncpy(to, from, len + 1);
	}
	return to;
}

const char* ConCommandBase::GetHelpText(void) const
{
	return m_pszHelpString;
}

bool ConCommandBase::IsRegistered(void) const
{
	return m_bRegistered;
}


struct characterset_t
{
	char set[256];
};

static characterset_t s_BreakSet;
static bool s_bBuiltBreakSet = false;



void CCommand::Reset()
{
	m_nArgc = 0;
	m_nArgv0Size = 0;
	m_pArgSBuffer[0] = 0;
}

characterset_t* CCommand::DefaultBreakSet()
{
	return &s_BreakSet;
}

int DefaultCompletionFunc(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
	return 0;
}


ConCommand::ConCommand(const char* pName, FnCommandCallbackVoid_t callback, const char* pHelpString  , int flags  , FnCommandCompletionCallback completionFunc  )
{
	m_fnCommandCallbackV1 = callback;
	m_bUsingNewCommandCallback = false;
	m_bUsingCommandCallbackInterface = false;
	m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
	m_bHasCompletionCallback = completionFunc != 0 ? true : false;

	BaseClass::Create(pName, pHelpString, flags);
}

ConCommand::ConCommand(const char* pName, FnCommandCallback_t callback, const char* pHelpString  , int flags  , FnCommandCompletionCallback completionFunc  )
{
	m_fnCommandCallback = callback;
	m_bUsingNewCommandCallback = true;
	m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
	m_bHasCompletionCallback = completionFunc != 0 ? true : false;
	m_bUsingCommandCallbackInterface = false;

	BaseClass::Create(pName, pHelpString, flags);
}

ConCommand::ConCommand(const char* pName, ICommandCallback* pCallback, const char* pHelpString  , int flags  , ICommandCompletionCallback* pCompletionCallback  )
{
	m_pCommandCallback = pCallback;
	m_bUsingNewCommandCallback = false;
	m_pCommandCompletionCallback = pCompletionCallback;
	m_bHasCompletionCallback = (pCompletionCallback != 0);
	m_bUsingCommandCallbackInterface = true;

	BaseClass::Create(pName, pHelpString, flags);
}

ConCommand::~ConCommand(void)
{
}


bool ConCommand::IsCommand(void) const
{
	return true;
}


void ConCommand::Dispatch(const CCommand& command)
{
	if (m_bUsingNewCommandCallback)
	{
		if (m_fnCommandCallback)
		{
			(*m_fnCommandCallback)(command);
			return;
		}
	}
	else if (m_bUsingCommandCallbackInterface)
	{
		if (m_pCommandCallback)
		{
			m_pCommandCallback->CommandCallback(command);
			return;
		}
	}
	else
	{
		if (m_fnCommandCallbackV1)
		{
			(*m_fnCommandCallbackV1)();
			return;
		}
	}

	AssertMsg(0, ("Encountered ConCommand '%s' without a callback!\n", GetName()));
}


int	ConCommand::AutoCompleteSuggest(const char* partial, CUtlVector< CUtlString >& commands)
{
	if (m_bUsingCommandCallbackInterface)
	{
		if (!m_pCommandCompletionCallback)
			return 0;
		return m_pCommandCompletionCallback->CommandCompletionCallback(partial, commands);
	}

	Assert(m_fnCompletionCallback);
	if (!m_fnCompletionCallback)
		return 0;

	char rgpchCommands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH];
	int iret = (m_fnCompletionCallback)(partial, rgpchCommands);
	for (int i = 0; i < iret; ++i)
	{
		CUtlString str = rgpchCommands[i];
		commands.AddToTail(str);
	}
	return iret;
}


bool ConCommand::CanAutoComplete(void)
{
	return m_bHasCompletionCallback;
}



ConVar::ConVar(const char* pName, const char* pDefaultValue, int flags    )
{
	Create(pName, pDefaultValue, flags);
}

ConVar::ConVar(const char* pName, const char* pDefaultValue, int flags, const char* pHelpString)
{
	Create(pName, pDefaultValue, flags, pHelpString);
}

ConVar::ConVar(const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax)
{
	Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
}

ConVar::ConVar(const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, FnChangeCallback_t callback)
{
	Create(pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback);
}

ConVar::ConVar(const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback)
{
	Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
}


ConVar::~ConVar(void)
{
	if (m_Value.m_pszString) {
		delete[] m_Value.m_pszString;
		m_Value.m_pszString = NULL;
	}
}


void ConVar::InstallChangeCallback(FnChangeCallback_t callback, bool bInvoke)
{
	if (callback) {
		if (m_fnChangeCallbacks.GetOffset(callback) != -1) {
			m_fnChangeCallbacks.AddToTail(callback);
			if (bInvoke)
				callback(this, m_Value.m_pszString, m_Value.m_fValue);
		}
		else {
		}
	}
	else {
	}
}

bool ConVar::IsFlagSet(int flag) const
{
	return (flag & m_pParent->m_nFlags) ? true : false;
}

const char* ConVar::GetHelpText(void) const
{
	return m_pParent->m_pszHelpString;
}

void ConVar::AddFlags(int flags)
{
	m_pParent->m_nFlags |= flags;

#ifdef ALLOW_DEVELOPMENT_CVARS
	m_pParent->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
#endif
}

bool ConVar::IsRegistered(void) const
{
	return m_pParent->m_bRegistered;
}

const char* ConVar::GetName(void) const
{
	return m_pParent->m_pszName;
}

bool ConVar::IsCommand(void) const
{
	return false;
}

void ConVar::Init()
{
	BaseClass::Init();
}

void ConVar::InternalSetValue(const char* value)
{
	printfdbg("Executed ConVar::InternalSetValue to %s\n", value);

	float fNewValue;
	char  tempVal[32];
	char* val;

	auto temp = *(uint32_t*)&m_Value.m_fValue ^ (uint32_t)this;
	float flOldValue = *(float*)(&temp);

	val = (char*)value;
	fNewValue = (float)atof(value);

	if (ClampValue(fNewValue)) {
		snprintf(tempVal, sizeof(tempVal), "%f", fNewValue);
		val = tempVal;
	}


	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fNewValue ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = (uint32_t)fNewValue ^ (uint32_t)this;

	if (!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
		ChangeStringValue(val, flOldValue);
	}
}

void ConVar::ChangeStringValue(const char* tempVal, float flOldValue)
{
	printfdbg("Executed ConVar::ChangeStringValue (tempVal %s; flOldValue %f)\n", tempVal, flOldValue);

	char* pszOldValue = (char*)stackalloc(m_Value.m_StringLength);
	memcpy(pszOldValue, m_Value.m_pszString, m_Value.m_StringLength);

	int len = strlen(tempVal) + 1;

	if (len > m_Value.m_StringLength) {
		if (m_Value.m_pszString) {
			delete[] m_Value.m_pszString;
		}

		m_Value.m_pszString = new char[len];
		m_Value.m_StringLength = len;
	}

	memcpy(m_Value.m_pszString, std::to_string(this->GetFloat()).c_str(), len);

	for (int i = 0; i < m_fnChangeCallbacks.Count(); i++) {
		m_fnChangeCallbacks[i](this, pszOldValue, flOldValue);
	}

	if (iff.g_pCVar)
		iff.g_pCVar->CallGlobalChangeCallbacks(this, pszOldValue, flOldValue);
}

void ConVar::InternalSetColorValue(Color cValue)
{
	int color = (int)cValue.GetRawColor();
	InternalSetIntValue(color);
}

bool ConVar::ClampValue(float& value)
{
	if (m_bHasMin && (value < m_fMinVal))
	{
		value = m_fMinVal;
		return true;
	}

	if (m_bHasMax && (value > m_fMaxVal))
	{
		value = m_fMaxVal;
		return true;
	}

	return false;
}



void ConVar::InternalSetFloatValue(float fNewValue)
{
	printfdbg("Executed ConVar::InternalSetFloatValue to %f\n",fNewValue);

	if (fNewValue == m_Value.m_fValue)
		return;

	ClampValue(fNewValue);

	float flOldValue = m_Value.m_fValue;
	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fNewValue ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = (uint32_t)fNewValue ^ (uint32_t)this;

	if (!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
		char tempVal[32];
		snprintf(tempVal, sizeof(tempVal), "%f", m_Value.m_fValue);
		ChangeStringValue(tempVal, flOldValue);
	}
	else {
	}
}

void ConVar::InternalSetIntValue(int nValue)
{
	printfdbg("Executed ConVar::InternalSetIntValue to %d\n",nValue);

	if (nValue == ((int)m_Value.m_nValue ^ (int)this))
		return;

	float fValue = (float)nValue;
	if (ClampValue(fValue)) {
		nValue = (int)(fValue);
	}

	float flOldValue = m_Value.m_fValue;
	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fValue ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = *(uint32_t*)&nValue ^ (uint32_t)this;

	if (!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
		char tempVal[32];
		snprintf(tempVal, sizeof(tempVal), "%d", m_Value.m_nValue);
		ChangeStringValue(tempVal, flOldValue);
	}
	else {
	}
}

void ConVar::Create(const char* pName, const char* pDefaultValue, int flags  ,
	const char* pHelpString  , bool bMin  , float fMin  ,
	bool bMax  , float fMax  , FnChangeCallback_t callback  )
{
	static const char* empty_string = "";

	m_pParent = this;

	m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

	m_Value.m_StringLength = strlen(m_pszDefaultValue) + 1;
	m_Value.m_pszString = new char[m_Value.m_StringLength];
	memcpy(m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength);

	m_bHasMin = bMin;
	m_fMinVal = fMin;
	m_bHasMax = bMax;
	m_fMaxVal = fMax;

	if (callback)
		m_fnChangeCallbacks.AddToTail(callback);

	float value = (float)atof(m_Value.m_pszString);

	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&value ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = *(uint32_t*)&value ^ (uint32_t)this;

	BaseClass::Create(pName, pHelpString, flags);
}

void ConVar::SetValue(const char* value)
{
	printfdbg("Executed ConVar::SetValue to %s\n",value);
	ConVar* var = (ConVar*)m_pParent;
	var->InternalSetValue(value);
}

void ConVar::SetValue(float value)
{
	printfdbg("Executed ConVar::SetValue to %f\n", value);
	ConVar* var = (ConVar*)m_pParent;
	var->InternalSetFloatValue(value);
}

void ConVar::SetValue(int value)
{
	printfdbg("Executed ConVar::SetValue to %d\n", value);
	ConVar* var = (ConVar*)m_pParent;
	var->InternalSetIntValue(value);
}

void ConVar::SetValue(Color value)
{
	ConVar* var = (ConVar*)m_pParent;
	var->InternalSetColorValue(value);
}


int ConVar::GetFlags(void) const
{
	return m_pParent->m_nFlags;
}

int ConVar::GetSplitScreenPlayerSlot(void) const
{
	return 0;
}

const char* ConVar::GetBaseName(void) const
{
	return m_pParent->m_pszName;
}

void ConVar::Revert(void)
{
	ConVar* var = (ConVar*)m_pParent;
	var->SetValue(var->m_pszDefaultValue);
}

bool ConVar::GetMin(float& minVal) const
{
	minVal = m_pParent->m_fMinVal;
	return m_pParent->m_bHasMin;
}

bool ConVar::GetMax(float& maxVal) const
{
	maxVal = m_pParent->m_fMaxVal;
	return m_pParent->m_bHasMax;
}

const char* ConVar::GetDefault(void) const
{
	return m_pParent->m_pszDefaultValue;
}

class CEmptyConVar : public ConVar
{
public:
	CEmptyConVar() : ConVar("", "0") {}
	virtual void SetValue(const char* pValue) {}
	virtual void SetValue(float flValue) {}
	virtual void SetValue(int nValue) {}
	virtual const char* GetName(void) const { return ""; }
	virtual bool IsFlagSet(int nFlags) const { return false; }
};

static CEmptyConVar s_EmptyConVar;

ConVarRef::ConVarRef(const char* pName)
{
	Init(pName, false);
}

ConVarRef::ConVarRef(const char* pName, bool bIgnoreMissing)
{
	Init(pName, bIgnoreMissing);
}

void ConVarRef::Init(const char* pName, bool bIgnoreMissing)
{
	m_pConVar = iff.g_pCVar ? iff.g_pCVar->FindVar(pName) : &s_EmptyConVar;
	if (!m_pConVar)
	{
		m_pConVar = &s_EmptyConVar;
	}
	m_pConVarState = static_cast<ConVar*>(m_pConVar);
	if (!IsValid())
	{
		static bool bFirst = true;
		if (iff.g_pCVar || bFirst)
		{
			if (!bIgnoreMissing)
			{
				printfdbg("ConVarRef %s doesn't point to an existing ConVar\n", pName);
			}
			bFirst = false;
		}
	}
}

ConVarRef::ConVarRef(IConVar* pConVar)
{
	m_pConVar = pConVar ? pConVar : &s_EmptyConVar;
	m_pConVarState = static_cast<ConVar*>(m_pConVar);
}

bool ConVarRef::IsValid() const
{
	return m_pConVar != &s_EmptyConVar;
}


void ConVar_PrintFlags(const ConCommandBase* var)
{
	bool any = false;
	if (var->IsFlagSet(FCVAR_GAMEDLL))
	{
		iff.myConMsg(" game");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_CLIENTDLL))
	{
		iff.myConMsg(" client");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_ARCHIVE))
	{
		iff.myConMsg(" archive");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_NOTIFY))
	{
		iff.myConMsg(" notify");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_SPONLY))
	{
		iff.myConMsg(" singleplayer");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_NOT_CONNECTED))
	{
		iff.myConMsg(" notconnected");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_CHEAT))
	{
		iff.myConMsg(" cheat");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_REPLICATED))
	{
		iff.myConMsg(" replicated");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_SERVER_CAN_EXECUTE))
	{
		iff.myConMsg(" server_can_execute");
		any = true;
	}

	if (var->IsFlagSet(FCVAR_CLIENTCMD_CAN_EXECUTE))
	{
		iff.myConMsg(" clientcmd_can_execute");
		any = true;
	}

	if (any)
	{
		iff.myConMsg("\n");
	}
}

