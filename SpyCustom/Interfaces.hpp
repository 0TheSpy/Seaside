#ifndef INTERFACES
#define INTERFACES
#pragma once

//random crashes while loading config without DEBUG defined
#define DEBUG 

#ifdef DEBUG
#define printfdbg(data, ...) printf(XorStr(data), __VA_ARGS__)
#else
#define printfdbg(...)
#endif

#define CLIENT_DLL 

#include "GetVfunc.hpp"

#include <iostream>
#include <windows.h>
#include "sdk/icliententitylist.h"
#include "sdk/CGameUI.h"
#include "sdk/GameEventManager.h"
#include "sdk/cdll_int.h"
#include "sdk/ISurface.h"
#include "sdk/IEngineVgui.h"
#include "sdk/ivmodelinfo.h"
#include "sdk/idatacache.h"
#include "sdk/ILocalize.h"
#include "sdk/IGameUI.h"
#include "sdk/dt_recv.h"
#include "sdk/client_class.h"
#include "sdk/enginetrace.h"
#include "sdk/gameconsole.h"
#include "sdk/ivrenderview.h"
#include "sdk/ivdebugoverlay.h"
#include "sdk/vphysics_interface.h"
#include "sdk/iefx.h"
#include "sdk/c_input.h"
#include "sdk/IEffects.h"
#include "sdk/iviewrender_beams.h"
#include "sdk/sdk_hud_chat.h"
#include "sdk/fx.h" 
#include "sdk/globalvars_base.h"
#include "sdk/IInput.h"
#include "sdk/icliententity.h"
#include "sdk/iclient.h"
#include "sdk/cdll_int.h"
#include "sdk/baseclientstate.h"
#include "imgui/imgui_impl_dx9.h"
#include "sdk/flashlighteffect.h"
#include "sdk/inetchannel.h" 
#include "sdk/animationlayer.h" 
#include "sdk/clientleafsystem.h"
#include "sdk/filesystem.h"
#include "sdk/modelloader.h"
#include "sdk/IEngineSound.h"
#include "sdk/iclientshadowmgr.h"
#include "sdk/cs_view_scene.h"
#include "sdk/iclientmode.h"
#include "sdk/itexture.h"
#include "sdk/precipitation_shared.h"
#include "sdk/iinputsystem.h"
#include "sdk/vpkparser.h"
#include "sdk/itempents.h" 
#include "sdk/c_te_legacytempents.h"
#include "sdk/iglobalvarsbase.h"
#include "sdk/iprediction.h"
#include "sdk/inetmessage.h"
#include "sdk/ClientNetMessage.h"
#include "sdk/clientmode_shared.h"
#include "sdk/isteamclient.h"
#include "sdk/isteamgamecoordinator.h"
#include "sdk/gamerules.h"
#include "sdk/igametypes.h"
#include "sdk/gamemovement.h" 

#include "XorStr.hpp"

PVOID FindHudElement(const char* name);

auto is_code_ptr(void* ptr) -> bool;
  
class IF {
public:
	IClientEntityList* g_pEntityList = nullptr;
	CGameUI* g_pGameUI = nullptr;
	vgui::IPanel* g_pVGuiPanel = nullptr;
	CGameEventManager* g_pGameEvents = nullptr;
	IVEngineClient* g_pEngineClient = nullptr;
	vgui::ISurface* g_pVGuiSurface = nullptr;
	IEngineVGui* g_pEnginevgui = nullptr;
	vgui::IVGui* g_pVGui = nullptr;
	IBaseClientDLL* g_pClient = nullptr;
	IVModelInfoClient* g_pMdlInfo = nullptr;
	IVModelRender* g_pMdlRender = nullptr;
	IMaterialSystem* g_pMaterialSystem = nullptr;
	IKeyValuesSystem* keyValuesSystem = nullptr;
	ICvar* g_pCVar = nullptr;
	vgui::ISystem* g_pVGuiSystem = nullptr;
	vgui::ISchemeManager* g_pVGuiSchemeManager = nullptr;
	vgui::IInput* g_pVGuiInput = nullptr;
	IFileSystem* g_pFullFileSystem = nullptr;
	vgui::ILocalize* g_pVGuiLocalize = nullptr;
	IEngineTrace* g_pEnginetrace = nullptr;
	IVEfx* g_pEfx = nullptr;
	IGameConsole* g_pGameConsole = nullptr;
	IPhysicsSurfaceProps* g_pPhysProps = nullptr;
	IVRenderView* g_pRenderView = nullptr;
	IVDebugOverlay* g_pDebugOverlay = nullptr;
	CGlobalVarsBase* g_pGlobals = nullptr;
	CInput* g_pInput = nullptr;
	IEffects* g_pEffects = nullptr;
	IViewRenderBeams* g_pViewRenderBeams = nullptr;
	IStudioRender* g_pStudioRender = nullptr;
	CHudChat* g_pChatElement = nullptr;
	using FX_TeslaFn = void(__thiscall*)(CTeslaInfo&);
	FX_TeslaFn FX_Tesla = nullptr;
	using FX_BloodSprayFn = bool(__cdecl*)(const CEffectData&);
	FX_BloodSprayFn BloodSprayFn = nullptr;
	using FXDispatchEffect = int(__fastcall*)(const char* name, const CEffectData&);
	FXDispatchEffect DispatchEffect = nullptr;
	void Init();
	void* GetInterface(const char* dllname, const char* interfacename);
	typedef void(__cdecl* CONMSGPROC)(const char*, ...);
	CONMSGPROC myConMsg = nullptr;
	typedef void(__cdecl* CONCOLORMSGPROC)(Color& clr, const char*, ...);
	CONCOLORMSGPROC myConColorMsg = nullptr;
	PVOID LoadFromFile = nullptr;
	IDirect3DDevice9* g_pD3DDevice9 = nullptr;
	IMemAlloc* g_pMemAlloc = nullptr;
	ISteamGameCoordinator* g_SteamGameCoordinator = nullptr;
	ISteamUser* g_SteamUser = nullptr;
	CClientState* g_pClientState = nullptr;
	CHudElement* g_pHudElement = nullptr;
	IClientLeafSystem* g_pClientLeafSystem = nullptr;
	INetworkStringTableContainer* g_pNetworkStringTableContainer = nullptr;
	std::add_pointer_t<const char** __fastcall(const char* playerModelName)> getPlayerViewmodelArmConfigForPlayerModel;
	std::add_pointer_t<const char** __fastcall(void* _this, void* edx)> fn_get_account_data;
	IMDLCache* g_pMdlCache = nullptr;
	IModelLoader* g_ModelLoader = nullptr;
	IEngineSound* g_pEngineSound = nullptr;
	IClientShadowMgr* g_pClientShadowMgr = nullptr;
	CCSViewRender* g_ViewRender = nullptr;
	ClientModeShared* g_ClientMode = nullptr; 
	IInputSystem* g_pInputSystem = nullptr;
	ITempEnts* g_pTempEnts = nullptr; 
	DWORD ParticleCollectionSimulateAdr = 0; 
	CPrediction* g_pPrediction = nullptr; 
	IGameTypes* g_pGameTypes = nullptr;
	CHudElement* HudUniqueAlerts = nullptr;
	C_GameRulesProxy* GameRulesProxy = nullptr; //C_GameRulesProxy
	DWORD dwRadarBase = NULL;
	IMoveHelper* g_pMoveHelper = NULL; //CMoveHelperClient 
	IGameMovement* g_pGameMovement = NULL; 
	
};




extern IF iff;

typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);

template <typename T>
static constexpr auto relativeToAbsolute(uintptr_t address) noexcept
{
	return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}


namespace vgui {
	inline vgui::IInput* input()
	{
		return iff.g_pVGuiInput;
	}

	inline vgui::ISchemeManager* scheme()
	{
		return iff.g_pVGuiSchemeManager;
	}

	inline vgui::ISurface* surface()
	{
		return iff.g_pVGuiSurface;
	}

	inline vgui::ISystem* system()
	{
		return iff.g_pVGuiSystem;
	}

	inline vgui::IVGui* ivgui()
	{
		return iff.g_pVGui;
	}

	inline vgui::IPanel* ipanel()
	{
		return iff.g_pVGuiPanel;
	}
}


class ICvar2 : public ICvar
{
public:
	ConVar* FindVar2(const char* var)
	{
		printfdbg("Trying to find var %s\n", var);
		typedef ConVar* (*oFindVar)(void*, const char*);
		return getvfunc<oFindVar>(this, 16)(this, var);
	}

	template <typename... Values>
	void ConsoleColorPrintf2(const Color& MsgColor, const char* szMsgFormat, Values... Parameters)
	{
		printfdbg("Trying to print %s\n", szMsgFormat);
		typedef void(*oConsoleColorPrintf)(void*, const Color&, const char*, ...);
		return getvfunc<oConsoleColorPrintf>(this, 25)(this, MsgColor, szMsgFormat, Parameters...);
	}
};

struct model_t
{
	void* fnHandle;                
	char    szName[260];             
	__int32 nLoadFlags;              
	__int32 nServerCount;            
	__int32 type;                    
	__int32 flags;                   
	Vector  vecMins;                 
	Vector  vecMaxs;                 
	float   radius;                  
	char    pad[0x1C];              
};


void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

enum ItemDefinitionIndex
{
	ITEM_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE_GOLD = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	MAX_ITEMDEFINITIONINDEX
};


static int random(int min, int max) noexcept
{
	return rand() % (max - min + 1) + min;
}


enum Sequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};


void SetValueUnrestricted(const char* cvar, float value);
void SetIntUnrestricted(const char* cvar, int value);
void SetFloatUnrestricted(const char* cvar, float value);
float GetVisibleFloat(const char* cvar);
const char* GetVisibleValue(const char* cvar);
  
static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{ 
	__asm
	{
		push    ebp
		mov     ebp, esp
		and esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

void NETSetConVar(const char* name, const char* value); 

void TextMsg(std::string text); 
void ShowMenu(std::string text);
std::string GetName(int id);
 
bool VoiceRecordStart(const char* pUncompressedFile); 


#endif
