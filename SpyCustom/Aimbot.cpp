#include "Aimbot.hpp"

///autowall
/*
bool IsBreakableEntity(C_BaseEntity* pEntity)  
{
    // @ida isbreakableentity: 55 8B EC 51 56 8B F1 85 F6 74 68

    // skip invalid entities
    if (pEntity == nullptr)
        return false;

    const int iHealth = ((C_BasePlayer*)pEntity)->GetHealth();

    // first check to see if it's already broken
    if (iHealth < 0 && pEntity->IsMaxHealth() > 0)
        return true;

    if (pEntity->GetTakeDamage() != DAMAGE_YES)
    { 
        const char* szClassName = pEntity->GetClientClass()->GetName();
        if (!strcmp(szClassName, XorStr("func_brush")))
            return false;
    }

    const int nCollisionGroup = pEntity->GetCollisionGroup();

    if (nCollisionGroup != COLLISION_GROUP_PUSHAWAY && nCollisionGroup != COLLISION_GROUP_BREAKABLE_GLASS && nCollisionGroup != COLLISION_GROUP_NONE)
        return false;

    if (iHealth > 200)
        return false;

    IMultiplayerPhysics* pPhysicsInterface = dynamic_cast<IMultiplayerPhysics*>(pEntity);
    if (pPhysicsInterface != nullptr)
    {
        if (pPhysicsInterface->GetMultiplayerPhysicsMode() != PHYSICS_MULTIPLAYER_SOLID)
            return false;
    }
    else
    {
        const char* szClassName = pEntity->GetClientClass()->GetName();

        if (!strcmp(szClassName, XorStr("func_breakable")) || !strcmp(szClassName, XorStr("func_breakable_surf")))
        {
            if (!strcmp(szClassName, XorStr("func_breakable_surf")))
            {
                CBreakableSurface* pSurface = static_cast<CBreakableSurface*>(pEntity);

                // don't try to break it if it has already been broken
                if (pSurface->IsBroken())
                    return false;
            }
        }
        else if (pEntity->PhysicsSolidMaskForEntity() & CONTENTS_PLAYERCLIP)
        {
            // hostages and players use CONTENTS_PLAYERCLIP, so we can use it to ignore them
            return false;
        }
    }

    IBreakableWithPropData* pBreakableInterface = dynamic_cast<IBreakableWithPropData*>(pEntity);
    if (pBreakableInterface != nullptr)
    {
        // bullets don't damage it - ignore
        if (pBreakableInterface->GetDmgModBullet() <= 0.0f)
            return false;
    }

    return true;
}

void ScaleDamage(int iHitGroup, C_BaseEntity* pEntity, float flWeaponArmorRatio, float& flDamage)
{
    const bool bHeavyArmor = pEntity->HasHeavyArmor();
    const int iArmor = pEntity->GetArmor();

    static ConVar* mp_damage_scale_ct_head = iff.g_pCVar->FindVar(XorStr("mp_damage_scale_ct_head"));
    static ConVar* mp_damage_scale_t_head = iff.g_pCVar->FindVar(XorStr("mp_damage_scale_t_head"));

    static ConVar* mp_damage_scale_ct_body = iff.g_pCVar->FindVar(XorStr("mp_damage_scale_ct_body"));
    static ConVar* mp_damage_scale_t_body = iff.g_pCVar->FindVar(XorStr("mp_damage_scale_t_body"));

    const float flHeadScale = ((C_BasePlayer*)pEntity)->GetTeam() == TEAM_CT ? mp_damage_scale_ct_head->GetFloat() : ((C_BasePlayer*)pEntity)->GetTeam() == TEAM_TERRORIST ? mp_damage_scale_t_head->GetFloat() : 1.0f;
    const float flBodyScale = ((C_BasePlayer*)pEntity)->GetTeam() == TEAM_CT ? mp_damage_scale_ct_body->GetFloat() : ((C_BasePlayer*)pEntity)->GetTeam() == TEAM_TERRORIST ? mp_damage_scale_t_body->GetFloat() : 1.0f;

    switch (iHitGroup)
    {
    case HITGROUP_HEAD:
        flDamage *= (bHeavyArmor ? 2.0f : 4.0f) * flHeadScale;
        break;
    case HITGROUP_STOMACH:
        flDamage *= 1.25f * flBodyScale;
        break;
    case HITGROUP_CHEST:
    case HITGROUP_LEFTARM:
    case HITGROUP_RIGHTARM:
        flDamage *= flBodyScale;
        break;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        flDamage *= 0.75f * flBodyScale;
        break;
    default:
        break;
    }

    // check is armored
    if (iArmor > 0 && ((iHitGroup == HITGROUP_HEAD && pEntity->HasHelmet()) || (iHitGroup >= HITGROUP_GENERIC && iHitGroup <= HITGROUP_RIGHTARM)))
    {
        float flArmorScale = 1.0f, flArmorBonusRatio = 0.5f, flArmorRatio = flWeaponArmorRatio * 0.5f;

        if (bHeavyArmor)
        {
            flArmorScale = 0.33f;
            flArmorBonusRatio = 0.33f;
            flArmorRatio *= 0.5f;
        }

        float flNewDamage = flDamage * flArmorRatio;

        if (bHeavyArmor)
            flNewDamage *= 0.85f;

        if (((flDamage - flDamage * flArmorRatio) * (flArmorScale * flArmorBonusRatio)) > iArmor)
            flNewDamage = flDamage - iArmor / flArmorBonusRatio;

        flDamage = flNewDamage;
    }
}

bool TraceToExit(CGameTrace& enterTrace, CGameTrace& exitTrace, Vector vecPosition, Vector vecDirection)  
{
    float flDistance = 0.0f;
    int iStartContents = 0;

    while (flDistance <= 90.0f)
    {
        // add extra distance to our ray
        flDistance += 4.0f;

        // multiply the direction vector to the distance so we go outwards, add our position to it
        Vector vecStart = vecPosition + vecDirection * flDistance;

        if (!iStartContents)
            iStartContents = iff.g_pEnginetrace->GetPointContents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

        const int iCurrentContents = iff.g_pEnginetrace->GetPointContents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

        if (!(iCurrentContents & MASK_SHOT_HULL) || (iCurrentContents & CONTENTS_HITBOX && iCurrentContents != iStartContents))
        {
            // setup our end position by deducting the direction by the extra added distance
            const Vector vecEnd = vecStart - (vecDirection * 4.0f);

            // trace ray to world
            Ray_t rayWorld; rayWorld.Init(vecStart, vecEnd);
            iff.g_pEnginetrace->TraceRay(rayWorld, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exitTrace);

            // check if a hitbox is in-front of our enemy and if they are behind of a solid wall
            if (exitTrace.startsolid && exitTrace.surface.flags & SURF_HITBOX)
            {
                // trace ray to entity
                Ray_t ray; ray.Init(vecStart, vecPosition);
                CTraceFilter filter(exitTrace.m_pEnt);

                iff.g_pEnginetrace->TraceRay(ray, MASK_SHOT_HULL, &filter, &exitTrace);

                if (exitTrace.DidHit() && !exitTrace.startsolid)
                {
                    vecStart = exitTrace.vecEnd;
                    return true;
                }

                continue;
            }

            if (exitTrace.DidHit() && !exitTrace.startsolid)
            {
                if (IsBreakableEntity(enterTrace.m_pEnt) && IsBreakableEntity(exitTrace.m_pEnt))
                    return true;

                if (enterTrace.surface.flags & SURF_NODRAW || (!(exitTrace.surface.flags & SURF_NODRAW) && exitTrace.plane.normal.DotProduct(vecDirection) <= 1.0f))
                {
                    const float flMultiplier = exitTrace.fraction * 4.0f;
                    vecStart -= vecDirection * flMultiplier;
                    return true;
                }

                continue;
            }

            if (!exitTrace.DidHit() || exitTrace.startsolid)
            {
                if (enterTrace.m_pEnt != nullptr && enterTrace.m_pEnt->GetIndex() != 0 && IsBreakableEntity(enterTrace.m_pEnt))
                {
                    // did hit breakable non world entity
                    exitTrace = enterTrace;
                    exitTrace.vecEnd = vecStart + vecDirection;
                    return true;
                }

                continue;
            }
        }
    }

    return false;
}

bool HandleBulletPenetration(CBaseEntity* pLocal, CCSWeaponInfo* pWeaponData, surfacedata_t* pEnterSurfaceData, FireBulletData_t& data)  
{
    static ConVar* ff_damage_reduction_bullets = iff.g_pCVar->FindVar(XorStr("ff_damage_reduction_bullets"));
    static ConVar* ff_damage_bullet_penetration = iff.g_pCVar->FindVar(XorStr("ff_damage_bullet_penetration"));

    const float flReductionDamage = ff_damage_reduction_bullets->GetFloat();
    const float flPenetrateDamage = ff_damage_bullet_penetration->GetFloat();

    const MaterialHandle_t hEnterMaterial = pEnterSurfaceData->game.hMaterial;
    const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;
    const bool bIsSolidSurf = ((data.enterTrace.contents >> 3) & CONTENTS_SOLID);
    const bool bIsLightSurf = ((data.enterTrace.surface.flags >> 7) & SURF_LIGHT);

    CGameTrace exitTrace = { };

    if (data.iPenetrateCount <= 0 ||
        (!data.iPenetrateCount && !bIsLightSurf && !bIsSolidSurf && hEnterMaterial != CHAR_TEX_GRATE && hEnterMaterial != CHAR_TEX_GLASS) ||
        (pWeaponData->m_flPenetration <= 0.0f) ||
        (!TraceToExit(data.enterTrace, exitTrace, data.enterTrace.vecEnd, data.vecDirection) && !(I::EngineTrace->GetPointContents(data.enterTrace.vecEnd, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL)))
        return false;

    const surfacedata_t* pExitSurfaceData = I::PhysicsProps->GetSurfaceData(exitTrace.surface.surfaceProps);
    const MaterialHandle_t hExitMaterial = pExitSurfaceData->game.hMaterial;
    const float flExitPenetrationModifier = pExitSurfaceData->game.flPenetrationModifier;

    float flDamageLostModifier = 0.16f;
    float flPenetrationModifier = 0.0f;

    if (hEnterMaterial == CHAR_TEX_GRATE || hEnterMaterial == CHAR_TEX_GLASS)
    {
        flDamageLostModifier = 0.05f;
        flPenetrationModifier = 3.0f;
    }
    else if (bIsSolidSurf || bIsLightSurf)
    {
        flDamageLostModifier = 0.16f;
        flPenetrationModifier = 1.0f;
    }
    else if (hEnterMaterial == CHAR_TEX_FLESH && (((C_BasePlayer*)pLocal)->GetTeam() == ((C_BasePlayer*)data.enterTrace.m_pEnt)->GetTeam() && flReductionDamage == 0.0f))
    {
        if (flPenetrateDamage == 0.0f)
            return false;

        // shoot through teammates
        flDamageLostModifier = 0.16f;
        flPenetrationModifier = flPenetrateDamage;
    }
    else
    {
        flDamageLostModifier = 0.16f;
        flPenetrationModifier = (flEnterPenetrationModifier + flExitPenetrationModifier) * 0.5f;
    }

    if (hEnterMaterial == hExitMaterial)
    {
        if (hExitMaterial == CHAR_TEX_CARDBOARD || hExitMaterial == CHAR_TEX_WOOD)
            flPenetrationModifier = 3.0f;
        else if (hExitMaterial == CHAR_TEX_PLASTIC)
            flPenetrationModifier = 2.0f;
    }

    const float flTraceDistance = (exitTrace.vecEnd - data.enterTrace.vecEnd).LengthSqr();

    // penetration modifier
    const float flModifier = std::max(0.0f, 1.0f / flPenetrationModifier);

    // this calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
    const float flLostDamage = (data.flCurrentDamage * flDamageLostModifier + std::max(0.0f, 3.75f / pWeaponData->flPenetration) * (flModifier * 3.0f)) + ((flModifier * flTraceDistance) / 24.0f);

    // did we loose too much damage?
    if (flLostDamage > data.flCurrentDamage)
        return false;

    // we can't use any of the damage that we've lost
    if (flLostDamage > 0.0f)
        data.flCurrentDamage -= flLostDamage;

    // do we still have enough damage to deal?
    if (data.flCurrentDamage < 1.0f)
        return false;

    data.vecPosition = exitTrace.vecEnd;
    --data.iPenetrateCount;
    return true;
}


void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int fMask, ITraceFilter* pFilter, CGameTrace* pTrace)
{
    CGameTrace trace = { };
    float flSmallestFraction = pTrace->fraction;

    Ray_t ray; ray.Init(vecAbsStart, vecAbsEnd);

    for (int i = 1; i < I::Globals->nMaxClients; i++)
    {
        CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);

        if (pEntity == nullptr || !pEntity->IsAlive() || pEntity->IsDormant())
            continue;

        if (pFilter != nullptr && !pFilter->ShouldHitEntity(pEntity, fMask))
            continue;

        ICollideable* pCollideable = pEntity->GetCollideable();

        if (pCollideable == nullptr)
            continue;

        // get bounding box
        const Vector vecMin = pCollideable->OBBMins_();
        const Vector vecMax = pCollideable->OBBMaxs_();

        // calculate world space center
        const Vector vecCenter = (vecMax + vecMin) * 0.5f;
        const Vector vecPosition = vecCenter + pEntity->GetAbsOrigin();

        Vector vecTo = vecPosition - vecAbsStart;
        Vector vecDirection = vecAbsEnd - vecAbsStart;
        const float flLength = vecDirection.NormalizeInPlace();

        const float flRangeAlong = vecDirection.DotProduct(vecTo);
        float flRange = 0.0f;

        // calculate distance to ray
        if (flRangeAlong < 0.0f)
            // off start point
            flRange = -vecTo.Length();
        else if (flRangeAlong > flLength)
            // off end point
            flRange = -(vecPosition - vecAbsEnd).Length();
        else
            // within ray bounds
            flRange = (vecPosition - (vecDirection * flRangeAlong + vecAbsStart)).Length();

        if (flRange < 0.0f || flRange > 60.0f)
            continue;

        I::EngineTrace->ClipRayToEntity(ray, fMask | CONTENTS_HITBOX, pEntity, &trace);

        if (trace.fraction < flSmallestFraction)
        {
            // we shortened the ray - save off the trace
            *pTrace = trace;
            flSmallestFraction = trace.fraction;
        }
    }
}


bool SimulateFireBullet(C_BaseEntity* pLocal, C_BaseAttributableItem* pWeapon, FireBulletData_t& data)
{
    CCSWeaponInfo* pWeaponData = iff.g_pWeaponSystem->GetWpnData((ItemDefinitionIndex)pWeapon->GetItemDefinitionIndex());

    if (pWeaponData == nullptr)
        return false;

    float flMaxRange = pWeaponData->m_flRange;

    // the total number of surfaces any bullet can penetrate in a single flight is capped at 4
    data.iPenetrateCount = 4;
    // set our current damage to what our gun's initial damage reports it will do
    data.flCurrentDamage = static_cast<float>(pWeaponData->m_iDamage);

    float flTraceLenght = 0.0f;
    CTraceFilter filter(pLocal);

    while (data.iPenetrateCount > 0 && data.flCurrentDamage >= 1.0f)
    {
        // max bullet range
        flMaxRange -= flTraceLenght;

        // end position of bullet
        const Vector vecEnd = data.vecPosition + data.vecDirection * flMaxRange;

        Ray_t ray; ray.Init(data.vecPosition, vecEnd);
        iff.g_pEnginetrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.enterTrace);

        // check for player hitboxes extending outside their collision bounds
        ClipTraceToPlayers(data.vecPosition, vecEnd + data.vecDirection * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.enterTrace);

        surfacedata_t* pEnterSurfaceData = I::PhysicsProps->GetSurfaceData(data.enterTrace.surface.surfaceProps);
        const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

        // we didn't hit anything, stop tracing shoot
        if (data.enterTrace.fraction == 1.0f)
            break;

        // calculate the damage based on the distance the bullet traveled
        flTraceLenght += data.enterTrace.fraction * flMaxRange;
        data.flCurrentDamage *= std::powf(pWeaponData->m_flRangeModifier, flTraceLenght / MAX_DAMAGE);

        // check is actually can shoot through
        if (flTraceLenght > 3000.0f || flEnterPenetrationModifier < 0.1f)
            break;

        // check is can do damage
        if (data.enterTrace.hitgroup != HITGROUP_GENERIC && data.enterTrace.hitgroup != HITGROUP_GEAR && pLocal->IsEnemy(data.enterTrace.m_pEnt))
        {
            // we got target - scale damage
            ScaleDamage(data.enterTrace.hitgroup, data.enterTrace.m_pEnt, pWeaponData->m_flArmorRatio, data.flCurrentDamage);
            return true;
        }

        // calling handlebulletpenetration here reduces our penetration pounter, and if it returns true, we can't shoot through it
        if (!HandleBulletPenetration(pLocal, pWeaponData, pEnterSurfaceData, data))
            break;
    }

    return false;
}

float GetDamage(C_BaseEntity* pLocal, const Vector& vecPoint, FireBulletData_t& dataOut)
{
    const Vector vecPosition = pLocal->GetViewPos();

    // setup data
    FireBulletData_t data = { };
    data.vecPosition = vecPosition;
    data.vecDirection = (vecPoint - vecPosition).Normalized();

    auto wep = ((C_BaseCombatCharacter*)pLocal)->GetActiveWeapon();
    C_BaseAttributableItem* pWeapon = (C_BaseAttributableItem*)iff.g_pEntityList->GetClientEntityFromHandle(wep);

    if (pWeapon == nullptr)
        return -1.0f;

    if (!SimulateFireBullet(pLocal, pWeapon, data))
        return -1.0f;

    dataOut = data;
    return data.flCurrentDamage;
}

bool ScanDamage(CBaseEntity* pLocal, Vector vecStart, Vector vecEnd)
{
    CGameTrace trace = { };
    if (1) //C::Get<bool>(Vars.bAimAutoWall)
    {
        FireBulletData_t data = { };

        // get autowall damage and data from it
        float flDamage = GetDamage(pLocal, vecEnd, data);

        // check for minimal damage
        if (flDamage < 1);// C::Get<int>(Vars.iAimMinimalDamage))
        return false;

        // copy trace from autowall
        trace = data.enterTrace;
    }

    //Traced target 
    CBaseEntity* pEntity = trace.m_pEnt;

    //Check the traced target is vailed
    if (pEntity == nullptr || !pEntity->IsAlive() || pEntity->IsDormant() || !pEntity->IsPlayer() || pEntity->HasImmunity() || !pLocal->IsEnemy(pEntity))
    {
        return false;
    }

    //We can shot the target
    return true;
}
*/



C_BasePlayer* GetBestTarget(CUserCmd* cmd)
{
    static C_CS_PlayerResource** g_player_resource = C_CS_PlayerResource::GetPlayerResource();

    float ofov = g_Options.aimbotfov;
    float nfov = 0;
    C_BasePlayer* player = nullptr;

    for (int iPlayer = 0; iPlayer < iff.g_pGlobals->maxClients; iPlayer++)
    {
        auto pCSPlayer = reinterpret_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(iPlayer));
        if (!pCSPlayer)
            continue;
        if (pCSPlayer == iff.pLocal || pCSPlayer->GetTeam() == iff.pLocal->GetTeam())
            continue;
        if (pCSPlayer->IsDormant())
            continue;
        if (!((*g_player_resource)->IsAlive()[iPlayer] && pCSPlayer->GetHealth() > 0))
            continue;
        if (pCSPlayer->HasImmunity())
            continue;
        Vector eyepos = iff.pLocal->GetViewPos();
        Vector enemyheadpos = pCSPlayer->GetBonePosition(8);

        // If the enemy is not visible, do the scan damage 
        if (!iff.pLocal->IsVisible(pCSPlayer, enemyheadpos))
        {
            //if (!ScanDamage(iff.pLocal, eyepos, enemyheadpos))
                continue;
        }

        Vector angleTo = CalcAngle(eyepos, enemyheadpos);
        angleTo.Clamp();
        if (g_Options.rcs) {
            Vector myangles;
            iff.g_pEngineClient->GetViewAngles(myangles);
            nfov = myangles.DistTo(angleTo);
        }
        else
            nfov = cmd->viewangles.DistTo(angleTo);
        if (nfov < ofov)
        {
            ofov = nfov;
            player = pCSPlayer;
        }
    }

    //printfdbg("Best target %x\n", player);
    return player;
}

void AimBot(CUserCmd* cmd)
{
    static C_CS_PlayerResource** g_player_resource = C_CS_PlayerResource::GetPlayerResource();

    if (!iff.g_pEngineClient->IsConnected() || !iff.g_pEngineClient->IsInGame())
        return;
    if (!iff.pLocal)
        return;
    if (!(*g_player_resource)->IsAlive()[iff.pLocal->GetIndex()])
        return;
    C_BasePlayer* target = GetBestTarget(cmd);
    if (target)
    {
        //player_info_t pinfo;
        //iff.g_pEngineClient->GetPlayerInfo(target->GetIndex(), &pinfo);
        //printfdbg("Best target %x %s\n", target, pinfo.name); 
        Vector eyepos = iff.pLocal->GetViewPos();
        Vector targethead = target->GetBonePosition(8);
        Vector viewangles = CalcAngle(eyepos, targethead);
        viewangles.Clamp();
        Vector delta = cmd->viewangles - viewangles;
        delta.Clamp();
        Vector finalAng = cmd->viewangles - delta;// / (variables::aimbot_smoothing * 20);
        finalAng.Clamp(); finalAng.Normalize();
        //  if (variables::aimbot_isvisiblecheck && csgo::local_player->can_see_player_pos(target, target->get_eye_pos()))
        //  {
        //      cmd->viewangles = finalAng;
        //      interfaces::engine->set_view_angles(cmd->viewangles);
        //  }
        //  else if (!variables::aimbot_isvisiblecheck)
         // {

          //rcs
        if (g_Options.rcs) {
            Vector aimPunchAngles = iff.pLocal->GetAimPunchAngle();
            Vector newAngles = (finalAng - aimPunchAngles * 2);
            newAngles.Clamp(); newAngles.Normalize();
            cmd->viewangles = newAngles;
        }
        else
            cmd->viewangles = finalAng;

        if (!g_Options.silentaim)
            iff.g_pEngineClient->SetViewAngles((QAngle&)finalAng);
        if (g_Options.aimbotautoshoot)
            cmd->buttons |= IN_ATTACK;
        //}
    }
}

