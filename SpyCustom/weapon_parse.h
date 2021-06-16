#ifndef WEAPON_PARSE_H
#define WEAPON_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"
#include "GameEventListener.h"
#include "utlsortvector.h"
#include "gamestringpool.h"

#ifdef CLIENT_DLL
#define CEconItemView C_EconItemView
#endif

class IFileSystem;
class CEconItemView;

typedef unsigned short WEAPON_FILE_INFO_HANDLE;


enum WeaponSound_t {
	EMPTY,
	SINGLE,
	SINGLE_ACCURATE,
	SINGLE_NPC,
	WPN_DOUBLE,        
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	NEARLYEMPTY,
	FAST_RELOAD,

	NUM_SHOOT_SOUND_TYPES,
};

int GetWeaponSoundFromString(const char* pszString);

#define MAX_SHOOT_SOUNDS	16			        

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

#define WEAPON_PRINTNAME_MISSING "!!! Missing printname on weapon"


class CHudTexture;
class KeyValues;

struct WeaponInfoLookup
{
	size_t m_nWeaponParseDataOffset;
	_fieldtypes m_fieldType;
	CGameString m_iszAttribClassName;

	WeaponInfoLookup(void) {}
	WeaponInfoLookup(size_t offset, _fieldtypes p_fieldType, const char* szAttribClassName);
	WeaponInfoLookup(const WeaponInfoLookup& WepInfoLookup);
};


class CWeaponInfoLookupListLess
{
public:
	bool Less(WeaponInfoLookup* const& src1, WeaponInfoLookup* const& src2, void* pCtx)
	{
		if (src1->m_iszAttribClassName.Get() < src2->m_iszAttribClassName.Get())
			return true;

		return false;
	}
};

class FileWeaponInfo_t
{
public:

	FileWeaponInfo_t();
	virtual ~FileWeaponInfo_t() {}

	virtual void Parse(KeyValues* pKeyValuesData, const char* szWeaponName);

	virtual void RefreshDynamicParameters() {};

public:
	bool					bParsedScript;
	bool					bLoadedHudElements;

	char					szClassName[MAX_WEAPON_STRING];
	char					szPrintName[MAX_WEAPON_STRING];			      

	int GetIndexofAttribute(string_t iszAttribClassName) const;
	static CUtlSortVector< WeaponInfoLookup*, CWeaponInfoLookupListLess > ms_vecWeaponInfoLookup;

protected:
	char					szViewModel[MAX_WEAPON_STRING];			     
	char					szWorldModel[MAX_WEAPON_STRING];		         
	char					szAmmo1[MAX_WEAPON_AMMO_NAME];			   
	char					szWorldDroppedModel[MAX_WEAPON_STRING];

	static bool				ms_bWeaponInfoLookupInitialized;

public:
	char					szAnimationPrefix[MAX_WEAPON_PREFIX];	              
	int						iSlot;									  
	int						iPosition;								     
	int						iMaxClip1;								        
	int						iMaxClip2;								        
	int						iDefaultClip1;							          
	int						iDefaultClip2;							          
	int						iWeight;								          
	int						iRumbleEffect;							        
	bool					bAutoSwitchTo;							         
	bool					bAutoSwitchFrom;						               
	int						iFlags;									   

	char					szAmmo2[MAX_WEAPON_AMMO_NAME];			   
	char					szAIAddOn[MAX_WEAPON_STRING];			      

	char					aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

private:
	int						iAmmoType;
	int						iAmmo2Type;

public:

	bool					m_bMeleeWeapon;		        

	bool					m_bBuiltRightHanded;
	bool					m_bAllowFlipping;	         


	virtual int		GetPrimaryClipSize(const CEconItemView* pWepView = NULL, int nAlt = 0, float flScale = 1.0f) const { return 0; }
	virtual int		GetSecondaryClipSize(const CEconItemView* pWepView = NULL, int nAlt = 0, float flScale = 1.0f) const { return 0; }
	virtual int		GetDefaultPrimaryClipSize(const CEconItemView* pWepView = NULL, int nAlt = 0, float flScale = 1.0f) const { return 0; }
	virtual int		GetDefaultSecondaryClipSize(const CEconItemView* pWepView = NULL, int nAlt = 0, float flScale = 1.0f) const { return 0; }
	virtual int		GetPrimaryReserveAmmoMax(const CEconItemView* pWepView = NULL, int nAlt = 0, float flScale = 1.0f) const { return 0; }
	virtual int		GetSecondaryReserveAmmoMax(const CEconItemView* pWepView = NULL, int nAlt = 0, float flScale = 1.0f) const { return 0; }

	const char* GetWorldModel(const CEconItemView* pWepView = NULL, int iTeam = 0) const;
	const char* GetViewModel(const CEconItemView* pWepView = NULL, int iTeam = 0) const;
	const char* GetWorldDroppedModel(const CEconItemView* pWepView = NULL, int iTeam = 0) const;
	const char* GetPrimaryAmmo(const CEconItemView* pWepView = NULL) const;

	int GetPrimaryAmmoType(const CEconItemView* pWepView = NULL) const;

	int						iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;

	bool					bShowUsageHint;							             

};

WEAPON_FILE_INFO_HANDLE LookupWeaponInfoSlot(const char* name);
FileWeaponInfo_t* GetFileWeaponInfoFromHandle(WEAPON_FILE_INFO_HANDLE handle);
WEAPON_FILE_INFO_HANDLE GetInvalidWeaponInfoHandle(void);

void PrecacheFileWeaponInfoDatabase();


KeyValues* ReadEncryptedKVFile(IFileSystem* filesystem, const char* szFilenameWithoutExtension, const unsigned char* pICEKey, bool bForceReadEncryptedFile = false);


extern FileWeaponInfo_t* CreateWeaponInfo();

extern void LoadEquipmentData();

class CWeaponDatabase : public CAutoGameSystem, public CGameEventListener
{
public:
	CWeaponDatabase();

	void Reset();
	bool LoadManifest();
	void PrecacheAllWeapons();
	void RefreshAllWeapons();

	WEAPON_FILE_INFO_HANDLE FindWeaponInfo(const char* name);
	FileWeaponInfo_t* GetFileWeaponInfoFromHandle(WEAPON_FILE_INFO_HANDLE handle);

protected:
	friend void LoadEquipmentData();

	virtual bool Init();

	WEAPON_FILE_INFO_HANDLE FindOrCreateWeaponInfo(const char* name);
	bool LoadWeaponDataFromFile(IFileSystem* filesystem, const char* szWeaponName, const unsigned char* pICEKey);
	void FireGameEvent(IGameEvent* event);

private:
	CUtlDict< FileWeaponInfo_t*, unsigned short > m_WeaponInfoDatabase;
	bool m_bPreCached;
};

extern CWeaponDatabase g_WeaponDatabase;


#endif  