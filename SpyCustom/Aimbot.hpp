#pragma once


#include "sdk/c_baseentity.h"
#include "Interfaces.hpp"
#include "Options.hpp"


// autowall objects structure 
struct FireBulletData_t
{
    Vector			vecPosition = { };
    Vector			vecDirection = { };
    CGameTrace			enterTrace = { };
    float			flCurrentDamage = 0.0f;
    int				iPenetrateCount = 0;
};

bool IsBreakableEntity(CBaseEntity* pEntity);
void ScaleDamage(int iHitGroup, CBaseEntity* pEntity, float flWeaponArmorRatio, float& flDamage);
bool TraceToExit(CGameTrace& enterTrace, CGameTrace& exitTrace, Vector vecPosition, Vector vecDirection);
bool HandleBulletPenetration(CBaseEntity* pLocal, CCSWeaponInfo* pWeaponData, surfacedata_t* pEnterSurfaceData, FireBulletData_t& data);
void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int fMask, ITraceFilter* pFilter, CGameTrace* pTrace);
bool SimulateFireBullet(C_BaseEntity* pLocal, C_BaseAttributableItem* pWeapon, FireBulletData_t& data);
float GetDamage(C_BaseEntity* pLocal, const Vector& vecPoint, FireBulletData_t& dataOut);
bool ScanDamage(CBaseEntity* pLocal, Vector vecStart, Vector vecEnd);

C_BasePlayer* GetBestTarget(CUserCmd* cmd);
void AimBot(CUserCmd* cmd);

