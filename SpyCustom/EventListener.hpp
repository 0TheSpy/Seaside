#ifndef EVENTLISTEN
#define EVENTLISTEN
#pragma once

#pragma comment( lib, "Winmm.lib" )

#include "Interfaces.hpp"
#include "sdk/c_baseentity.h"
#include "Options.hpp"


struct bullet
{
    bullet(int userID, Vector pos, int dmg, bool hs, float curtime)
    {
        this->userid = userid;
        this->hs = hs;
        this->curtime = curtime;
        this->time = curtime + 0.0f;
        this->pos = pos;
        this->dmg = dmg;
    }
    int userid;
    bool hs;
    float time;
    float curtime;
    Vector pos;
    int dmg;
};
std::vector<bullet> bulletdata;

#define PI 3.14159265

class EventListener : public IGameEventListener2
{
public:
    EventListener()
    {
        if (!iff.g_pGameEvents->AddListener(this, "bullet_impact", false)) 
            printfdbg("Can't add listener bullet_impact\n"); 
        

        if (!iff.g_pGameEvents->AddListener(this, "player_hurt", false)) 
            printfdbg("Can't add listener player_hurt\n"); 
       

        if (!iff.g_pGameEvents->AddListener(this, "vote_cast", false)) 
            printfdbg("Can't add listener vote_cast\n"); 
        

        if (!iff.g_pGameEvents->AddListener(this, "game_newmap", false)) 
            printfdbg("Can't add listener game_newmap\n"); 
        
         
        printfdbg("Event Listener created\n"); 

    }

    ~EventListener()
    {
        iff.g_pGameEvents->RemoveListener(this);
    }

    int GetEventDebugID() override
    {
        return EVENT_DEBUG_ID_INIT;
    }

    virtual void FireGameEvent(IGameEvent* event)
    {

        if (*g_Options.hitmarker || *g_Options.hitsound) {
            if (!strcmp(event->GetName(), "player_hurt"))
            {
                short hitgroup = event->GetInt("hitgroup");
                short attacker = event->GetInt("attacker");
                short userid = event->GetInt("userid");
                short dmg_health = event->GetInt("dmg_health");

                if (iff.g_pEngineClient->GetPlayerForUserID(attacker) == iff.g_pEngineClient->GetLocalPlayer()) {

                    int pid = iff.g_pEngineClient->GetPlayerForUserID(userid);
                    C_BasePlayer* pEntity = (C_BasePlayer*)iff.g_pEntityList->GetClientEntity(pid);
                    Vector pos = pEntity->GetBonePosition(8); 

                    if (hitgroup == 1)
                    {
                        if (*g_Options.hitsound) {
                            //iff.g_pEngineClient->ExecuteClientCmd("play hitsound_crit.wav");
                            char pathtosnd[MAX_PATH] = "play "; 
                            strcat(pathtosnd, g_Options.hspath.value->mystring);
                            iff.g_pEngineClient->ExecuteClientCmd(pathtosnd);
                        }

                        if (*g_Options.hitmarker) 
                            bulletdata.push_back(bullet(userid, pos, dmg_health, 1, iff.g_pGlobals->curtime));
                    }
                    else
                    { 
                        if (*g_Options.hitsound) {
                            char pathtosnd[MAX_PATH] = "play ";
                            strcat(pathtosnd, g_Options.obpath.value->mystring);
                            iff.g_pEngineClient->ExecuteClientCmd(pathtosnd);
                        }

                        if (*g_Options.hitmarker)
                            bulletdata.push_back(bullet(userid, pos, dmg_health, 0, iff.g_pGlobals->curtime));
                    }


                }
            }
        }
        
        
        if (!strcmp(event->GetName(), "bullet_impact"))
        {
            
            short userid = event->GetInt("userid");
            int localplayer = iff.g_pEngineClient->GetLocalPlayer();


            if (iff.g_pEngineClient->GetPlayerForUserID(userid) == localplayer) {


                float x = event->GetFloat("x");
                float y = event->GetFloat("y");
                float z = event->GetFloat("z");

                C_BasePlayer* pEntity = (C_BasePlayer*)iff.g_pEntityList->GetClientEntity(localplayer);
                Vector src = pEntity->GetOrigin() + pEntity->GetViewOffset(); 
                Vector dst = { x,y,z };

                Vector ang = pEntity->GetAngles();

                ang.x -= 90.0f; ang.y -= 90.0f; 
                
                static float margin = 3.0f;
                static float coeff = 1.5f;
                src.x += cos(ang.x * PI / 180) * margin * sin(ang.y * PI / 180) + cos(ang.y * PI / 180) * margin * coeff;
                src.y += -cos(ang.x * PI / 180) * margin * cos(ang.y * PI / 180) + sin(ang.y * PI / 180) * margin * coeff;
                src.z += sin(ang.x * PI / 180) * margin;

                
                if (*g_Options.beamtrace) {

                    BeamInfo_t beamInfo;
                    beamInfo.m_nType = *g_Options.beamtype; 
                    beamInfo.m_pszModelName = g_Options.beampath.value->mystring;

                    beamInfo.m_nModelIndex = -1;
                    beamInfo.m_flHaloScale = 0.0f;
                    beamInfo.m_pStartEnt = pEntity; 
                    beamInfo.m_pEndEnt = NULL; 

                    beamInfo.m_vecEnd = dst;
                    beamInfo.m_vecStart = src;   
                    beamInfo.m_flLife = *g_Options.beamlife;  
                    beamInfo.m_flFadeLength = *g_Options.beamfadelength;  
                    beamInfo.m_flAmplitude = *g_Options.beamamplitude; 
                    beamInfo.m_nSegments = *g_Options.beamsegments;    
                    beamInfo.m_bRenderable = true;
           
                    beamInfo.m_flSpeed = *g_Options.beamspeed;
                    beamInfo.m_nStartFrame = *g_Options.beamstartframe;
                    beamInfo.m_flFrameRate = *g_Options.beamframerate; 
                    beamInfo.m_flWidth = *g_Options.beamwidth;
                    beamInfo.m_flEndWidth = *g_Options.beamwidth;

                    beamInfo.m_flBlue = g_Options.beamcolor.value->b * 255.0f;
                    beamInfo.m_flGreen = g_Options.beamcolor.value->g * 255.0f;
                    beamInfo.m_flRed = g_Options.beamcolor.value->r * 255.0f;
                    beamInfo.m_flBrightness = g_Options.beamcolor.value->a * 255.0f;

                    beamInfo.m_nFlags = *g_Options.beamflags;     
                    Beam_t* myBeam = iff.g_pViewRenderBeams->CreateBeamPoints(beamInfo);
                    if (myBeam) iff.g_pViewRenderBeams->DrawBeam(myBeam);
                    else
                    {
                    }
                }
                 
                 
                if (*g_Options.attacheffects) {

                    switch (g_Options.effects.value->type) {

                    case 0:
                    {
                        switch (g_Options.effects.value->selectedc)
                        {
                        case 0:
                        {
                            iff.g_pEffects->Smoke(dst, -1, g_Options.effects.value->fx_fsize, g_Options.effects.value->fx_fspeed);
                            break;
                        }
                        case 2: 
                        {
                            iff.g_pEffects->Dust(dst, Vector(0.0f, 0.0f, 1.0f), g_Options.effects.value->fx_fsize, g_Options.effects.value->fx_fspeed);
                            break;
                        }
                        case 1:
                        {
                            iff.g_pEffects->Sparks(dst, g_Options.effects.value->fx_magnitude, g_Options.effects.value->fx_traillength);
                            break;
                        }
                        case 3:
                        {
                            iff.g_pEffects->MuzzleFlash(dst, CalcAngle(dst, src), g_Options.effects.value->fx_fsize, g_Options.effects.value->fx_itype);
                            break;
                        }
                        case 4: 
                        {
                            iff.g_pEffects->MetalSparks(dst, Vector( 0.0f, 0.0f, 0.0f ) );
                            break;
                        }
                        case 6:
                        {
                            iff.g_pEffects->Ricochet(dst, Vector(0.0f, 0.0f, 1.0f));
                            break;
                        }
                        case 5:
                        {
                            iff.g_pEffects->EnergySplash(dst, src, g_Options.effects.value->fx_bexplosive);
                            break;
                        }

                        }      
                        break;
                    }    

                    case 1:
                    {
                        CEffectData asscock;
                        asscock.m_fFlags = g_Options.effects.value->fflags;
                        asscock.m_vOrigin = dst;
                        asscock.m_vStart = src;
                        asscock.m_vNormal.Init(0.0f, 0.0f, 1.0f);
                        asscock.m_flScale = g_Options.effects.value->fscale;
                        asscock.m_nColor = g_Options.effects.value->icolors;
                        asscock.m_flMagnitude = g_Options.effects.value->fmagtinude;
                        asscock.m_flRadius = g_Options.effects.value->fradius;
                        iff.DispatchEffect(opt.DispatchEffect.at(g_Options.effects.value->selectedc).c_str(), asscock);

                        break;
                    }    



                    case 2:
                    {
                        switch (g_Options.effects.value->selectedc)
                        {
                        case 0: 
                        {
                            CTeslaInfo teslaInfo;
                            teslaInfo.m_flBeamWidth = g_Options.effects.value->fbeamwidth;
                            teslaInfo.m_flRadius = g_Options.effects.value->teslaradius;
                            teslaInfo.m_nEntIndex = -1;
                            teslaInfo.m_vColor.Init(g_Options.effects.value->teslacolor[0], g_Options.effects.value->teslacolor[1], g_Options.effects.value->teslacolor[2]);       
                            teslaInfo.m_vPos = dst;          
                            teslaInfo.m_flTimeVisible = g_Options.effects.value->fltimevisible;
                            teslaInfo.m_nBeams = g_Options.effects.value->ibeams;
                            teslaInfo.m_pszSpriteName = g_Options.effects.value->szSpriteName;
                            iff.FX_Tesla(teslaInfo);

                            break;
                        }

                         

                        case 1:   
                        { 
                                
                            dlight_t* pDlight = iff.g_pEfx->CL_AllocDlight(1);
                            pDlight->origin = dst;
                            pDlight->radius = g_Options.effects.value->frad; 
                            pDlight->color.r = g_Options.effects.value->dlightcolor[0]*255.0f;
                            pDlight->color.g = g_Options.effects.value->dlightcolor[1] * 255.0f;
                            pDlight->color.b = g_Options.effects.value->dlightcolor[2] * 255.0f;

                            pDlight->color.exponent = 5;
                            pDlight->die = iff.g_pGlobals->curtime + g_Options.effects.value->fdie;
                            pDlight->decay = 0; 
                            pDlight->key = g_Options.effects.value->key;
                            pDlight->style = g_Options.effects.value->style;
                            


                            break;
                        }

                        
                        

                        }
                        break;
                    }    







                    }

                }

                
            }
        
        }
         



        if (*g_Options.votereveal) {
            if (!strcmp(event->GetName(), "vote_cast"))
            {
                int vote = event->GetInt("vote_option");
                int id = event->GetInt("entityid");
                short team = event->GetInt("team");
                if (iff.g_ClientMode->m_pChatElement)
                {
                    player_info_t pinfo;
                    iff.g_pEngineClient->GetPlayerInfo(id, &pinfo);

                    if (pinfo.name) {
                        char team_byte[3];
                        if (team == 2) memcpy(team_byte, " \x07", 3); //red
                        else
                            if (team == 3) memcpy(team_byte, " \x0B", 3); //blu
                            else
                                memcpy(team_byte, " \x08", 3); //gray

                        printfdbg("%d %s voted %d\n", team, pinfo.name, vote);
                        iff.g_ClientMode->m_pChatElement->ChatPrintf_v(0, 0, std::string("").
                            //append(" \x06"). 
                            append(team_byte).
                            append(pinfo.name).
                            append(" \x01").  
                            append("voted").
                            append((vote == 0 ? std::string(" \x04").append("YES") : std::string(" \x02").append("NO"))).c_str());
                    }
                }
            }
        }

        if (!strcmp(event->GetName(), "game_newmap"))
        {
            printfdbg("game_newmap\n");
            OnLevelInit();
        }





    }
};



#endif 
