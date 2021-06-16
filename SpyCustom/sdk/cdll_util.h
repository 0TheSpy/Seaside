#if !defined( UTIL_H )
#define UTIL_H

#ifdef _WIN32
#pragma once
#endif

#include "soundflags.h"
#include "vector.h"
#include "shareddefs.h"

#include "shake.h"
#include "imageformat.h"
#include "ispatialpartition.h"
#include "MaterialSystemUtil.h"

class Vector;
class QAngle;
class IMaterial;
class ITexture;
class IClientEntity;
class CHudTexture;
class CGameTrace;
class C_BaseEntity;

struct Ray_t;
struct client_textmessage_t;
typedef CGameTrace trace_t;

namespace vgui
{
	typedef unsigned long HFont;
};



extern bool g_MakingDevShots;

int		ScreenHeight(void);
int		ScreenWidth(void);

#define XRES(x)	( x  * ( ( float )ScreenWidth() / 640.0 ) )
#define YRES(y)	( y  * ( ( float )ScreenHeight() / 480.0 ) )

int		UTIL_ComputeStringWidth(vgui::HFont& font, const char* str);
int		UTIL_ComputeStringWidth(vgui::HFont& font, const wchar_t* str);
float	UTIL_AngleDiff(float destAngle, float srcAngle);
void	UTIL_Bubbles(const Vector& mins, const Vector& maxs, int count);
void	UTIL_Smoke(const Vector& origin, const float scale, const float framerate);
void	UTIL_ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName = NULL);
int		UTIL_PrecacheDecal(const char* name, bool preload = false);
void	UTIL_EmitAmbientSound(C_BaseEntity* entity, const Vector& vecOrigin, const char* samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch);
void	UTIL_SetOrigin(C_BaseEntity* entity, const Vector& vecOrigin);
void	UTIL_ScreenShake(const Vector& center, float amplitude, float frequency, float duration, float radius, ShakeCommand_t eCommand, bool bAirShake = false);
byte* UTIL_LoadFileForMe(const char* filename, int* pLength);
void	UTIL_FreeFile(byte* buffer);
void	UTIL_MakeSafeName(const char* oldName, OUT_Z_CAP(newNameBufSize) char* newName, int newNameBufSize);	                  
const char* UTIL_SafeName(const char* oldName);	       
void	UTIL_ReplaceKeyBindings(const wchar_t* inbuf, int inbufsizebytes, OUT_Z_BYTECAP(outbufsizebytes) wchar_t* outbuf, int outbufsizebytes);

unsigned char UTIL_ComputeEntityFade(C_BaseEntity* pEntity, float flMinDist, float flMaxDist, float flFadeScale);

client_textmessage_t* TextMessageGet(const char* pName);

char* VarArgs(PRINTF_FORMAT_STRING const char* format, ...);


int		GetSpectatorTarget();
int		GetSpectatorMode(void);
bool	IsPlayerIndex(int index);
int		GetLocalPlayerIndex(void);
int		GetLocalPlayerVisionFilterFlags(bool bWeaponsCheck = false);
bool	IsLocalPlayerUsingVisionFilterFlags(int nFlags, bool bWeaponsCheck = false);
int		GetLocalPlayerTeam(void);
bool	IsLocalPlayerSpectator(void);
void	NormalizeAngles(QAngle& angles);
void	InterpolateAngles(const QAngle& start, const QAngle& end, QAngle& output, float frac);
void	InterpolateVector(float frac, const Vector& src, const Vector& dest, Vector& output);

const char* nexttoken(char* token, const char* str, char sep);

enum
{
	LIGHT_INDEX_TE_DYNAMIC = 0x10000000,
	LIGHT_INDEX_PLAYER_BRIGHT = 0x20000000,
	LIGHT_INDEX_MUZZLEFLASH = 0x40000000,
};

void UTIL_PrecacheOther(const char* szClassname);

void UTIL_SetTrace(trace_t& tr, const Ray_t& ray, C_BaseEntity* edict, float fraction, int hitgroup, unsigned int contents, const Vector& normal, float intercept);

bool GetVectorInScreenSpace(Vector pos, int& iX, int& iY, Vector* vecOffset = NULL);
bool GetVectorInHudSpace(Vector pos, int& iX, int& iY, Vector* vecOffset = NULL);
bool GetTargetInScreenSpace(C_BaseEntity* pTargetEntity, int& iX, int& iY, Vector* vecOffset = NULL);
bool GetTargetInHudSpace(C_BaseEntity* pTargetEntity, int& iX, int& iY, Vector* vecOffset = NULL);

class C_BasePlayer;
void ClientPrint(C_BasePlayer* player, int msg_dest, const char* msg_name, const char* param1 = NULL, const char* param2 = NULL, const char* param3 = NULL, const char* param4 = NULL);

int			UTIL_EntitiesInBox(C_BaseEntity** pList, int listMax, const Vector& mins, const Vector& maxs, int flagMask, int partitionMask = PARTITION_CLIENT_NON_STATIC_EDICTS);
int			UTIL_EntitiesInSphere(C_BaseEntity** pList, int listMax, const Vector& center, float radius, int flagMask, int partitionMask = PARTITION_CLIENT_NON_STATIC_EDICTS);
int			UTIL_EntitiesAlongRay(C_BaseEntity** pList, int listMax, const Ray_t& ray, int flagMask, int partitionMask = PARTITION_CLIENT_NON_STATIC_EDICTS);

#define MAX_SPHERE_QUERY	256
class CEntitySphereQuery
{
public:
	CEntitySphereQuery(const Vector& center, float radius, int flagMask = 0, int partitionMask = PARTITION_CLIENT_NON_STATIC_EDICTS);
	C_BaseEntity* GetCurrentEntity();
	inline void NextEntity() { m_listIndex++; }

private:
	int			m_listIndex;
	int			m_listCount;
	C_BaseEntity* m_pList[MAX_SPHERE_QUERY];
};

C_BaseEntity* CreateEntityByName(const char* className);
template< class T >
T* _CreateEntity(T* newClass, const char* className)
{
	T* newEnt = dynamic_cast<T*>(CreateEntityByName(className));
	if (!newEnt)
	{
		Warning("classname %s used to create wrong class type\n", className);
		Assert(0);
	}

	return newEnt;
}

#define CREATE_ENTITY( newClass, className ) _CreateEntity( (newClass*)NULL, className )
#define CREATE_UNSAVED_ENTITY( newClass, className ) _CreateEntityTemplate( (newClass*)NULL, className )

inline bool FStrEq(const char* sz1, const char* sz2)
{
	return (sz1 == sz2 || V_stricmp(sz1, sz2) == 0);
}

void UTIL_BoundToWorldSize(Vector* pVecPos);

void UTIL_IncrementMapKey(const char* pszCustomKey);

int UTIL_GetMapKeyCount(const char* pszCustomKey);

bool UTIL_HasLoadedAnyMap();

#endif  