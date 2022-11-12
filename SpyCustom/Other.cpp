#include "Other.hpp"
#include "PatternScan.hpp"



void UpdateFlashLight(CFlashlightEffect* pFlashLight, const Vector& vecPos, const Vector& vecForward, const Vector& vecRight, const Vector& vecUp)
{ 
    typedef void(__thiscall* UpdateLight_t)(void*, int, const Vector&, const Vector&, const Vector&, const Vector&, float, float, float, bool, const char*);

    static UpdateLight_t oUpdateLight = NULL;

    if (!oUpdateLight)
    {
        DWORD callInstruction = FindPatternV2("client.dll", "E8 ? ?? ? ? 8B 06 F3 0F 10 46");     
        DWORD relativeAddress = *(DWORD*)(callInstruction + 1);    
        DWORD nextInstruction = callInstruction + 5;       
        oUpdateLight = (UpdateLight_t)(nextInstruction + relativeAddress);         
    }

    oUpdateLight(pFlashLight, pFlashLight->m_nEntIndex, vecPos, vecForward, vecRight, vecUp,
        pFlashLight->m_flFov, pFlashLight->m_flFarZ, pFlashLight->m_flLinearAtten, pFlashLight->m_bCastsShadows,
        pFlashLight->m_textureName);
}


CFlashlightEffect* CreateFlashLight(int nEntIndex, const char* pszTextureName, float flFov, float flFarZ, float flLinearAtten)
{
  
    CFlashlightEffect* pFlashLight = reinterpret_cast<CFlashlightEffect*>(iff.g_pMemAlloc->Alloc(sizeof(CFlashlightEffect)));

    if (!pFlashLight)
        return NULL;

      static DWORD oConstructor = FindPatternV2("client.dll", "55 8B EC F3 0F 10 45 ? B8");

    __asm
    {
        movss xmm3, flFov
        mov ecx, pFlashLight
        push flLinearAtten
        push flFarZ
        push pszTextureName
        push nEntIndex
        call oConstructor
    }
    pFlashLight->m_bIsOn = true;


    return pFlashLight;
}

void DestroyFlashLight(CFlashlightEffect* pFlashLight)
{
    static DWORD oDestructor = FindPatternV2("client.dll", "56 8B F1 E8 ? ? ? ? 8B 4E 28");

    __asm
    {
        mov ecx, pFlashLight
        push ecx
        call oDestructor
    }

}

void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
    float sr, sp, sy, cr, cp, cy;

    SinCos(DEG2RAD(angles[1]), &sy, &cy);
    SinCos(DEG2RAD(angles[0]), &sp, &cp);
    SinCos(DEG2RAD(angles[2]), &sr, &cr);

    if (forward)
    {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }

    if (right)
    {
        right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
        right->y = (-1 * sr * sp * sy + -1 * cr * cy);
        right->z = -1 * sr * cp;
    }

    if (up)
    {
        up->x = (cr * sp * cy + -sr * -sy);
        up->y = (cr * sp * sy + -sr * cy);
        up->z = cr * cp;
    }
}


static bool ssToggleButton(ButtonCode_t code)
{
    static int buttonPressedTick = 0;
    if (iff.g_pInputSystem->IsButtonDown(code) && (GetTickCount64() - buttonPressedTick) > 300)
    {
        buttonPressedTick = GetTickCount64();
        return true;
    }
    return false;
}

void FlashlightRun(C_BasePlayer* local)
{
    static CFlashlightEffect* pFlashLight = NULL;

    if (opt.disconnected)
    {
        printfdbg("nullify pFlashlight bc disconnected\n"); 
        pFlashLight = NULL;
        opt.disconnected = 0;
    }

    if (ssToggleButton(KEY_L))
    {
        if (!pFlashLight)
        { 
            printfdbg("creating fl\n"); 
            pFlashLight = CreateFlashLight(local->GetIndex(), g_Options.flashlightTexture.value->mystring, *g_Options.flashlightFOV, *g_Options.flashlightFarZ, *g_Options.flashlightLinearAtten); 
            iff.g_pEngineClient->ExecuteClientCmd("play items/flashlight1.wav");
        }
        else
        { 
            printfdbg("destroying fl\n"); 
            DestroyFlashLight(pFlashLight);
            pFlashLight = NULL;
            iff.g_pEngineClient->ExecuteClientCmd("play items/flashlight1.wav");
        }
    }

    if (pFlashLight)
    { 
        Vector f, r, u;
        Vector viewAngles;

        iff.g_pEngineClient->GetViewAngles(viewAngles);
        AngleVectors(viewAngles, &f, &r, &u);

        pFlashLight->m_bIsOn =  true;
        pFlashLight->m_bCastsShadows = *g_Options.flashlightShadows;
        pFlashLight->m_flFov = *g_Options.flashlightFOV;       

        UpdateFlashLight(pFlashLight, local->GetOrigin() + local->GetViewOffset(), f, r, u);
         
    }
}


#define DRAW_SCREEN_EFFECT(material) \
{ \
    const auto drawFunction = relativeToAbsolute<uintptr_t>(FindPatternV2("client.dll", "E8 ? ? ? ? 83 C4 0C 8D 4D F8") + 1); \
    int w, h; \
    iff.g_pVGuiSurface->GetScreenSize(w, h); \
    __asm { \
        __asm push h \
        __asm push w \
        __asm push 0 \
        __asm xor edx, edx \
        __asm mov ecx, material \
        __asm call drawFunction \
        __asm add esp, 12 \
    } \
}

void NightvisionRun(C_BasePlayer* local) {
     
    static int m_flNightVisionAlpha = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashDuration") - 0x1C;
  
    if (ssToggleButton(KEY_N) )
    {
        if (!local->GetNightvision())
        {
            local->GetNightvision() = true;
            *(float*)(uintptr_t(local) + m_flNightVisionAlpha) = 1.0f;
            iff.g_pEngineClient->ExecuteClientCmd("play items/nvg_on.wav");

        }
        else
        {
            local->GetNightvision() = false;
            *(float*)(uintptr_t(local) + m_flNightVisionAlpha) = 0;
            iff.g_pEngineClient->ExecuteClientCmd("play items/nvg_off.wav");
        }
    }
}


void do_precipitation() {
    static void* rain_networkable = nullptr; 
    C_Precipitation* rain_ent = (C_Precipitation*)iff.g_pEntityList->GetClientEntity(MAX_EDICTS - 1);
    static ClientClass* precipitation_client_class = nullptr;




    if (!iff.g_pEngineClient->IsInGame() || !iff.g_pEngineClient->IsConnected()) {
        rain_networkable = rain_ent = nullptr;
        return;
    }

    int localplayer_index = iff.g_pEngineClient->GetLocalPlayer();
    C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(iff.g_pEntityList->GetClientEntity(localplayer_index));
    if (!localplayer) return;

    if (!(localplayer->GetLifeState() == LIFE_ALIVE && localplayer->GetHealth() > 0))
        return;

    

    if (!precipitation_client_class) {
        for (auto pclass = iff.g_pClient->GetAllClasses(); pclass && !precipitation_client_class; pclass = pclass->m_pNext)
            if (strstr(pclass->GetName(), "CPrecipitation"))  
            { 
                printfdbg("class found %x\n", pclass); 
                precipitation_client_class = pclass;
            }

    }
   

    else {
        if (!rain_ent && precipitation_client_class && precipitation_client_class->m_pCreateFn) {
             
            printfdbg("Creating precipitation\n"); 

            rain_networkable = ((void* (*)(int, int))precipitation_client_class->m_pCreateFn)(MAX_EDICTS - 1, 0);

            if (rain_networkable) {
                rain_ent = (C_Precipitation*)iff.g_pEntityList->GetClientEntity(MAX_EDICTS - 1);

                rain_ent->GetPrecipitationType() = (PrecipitationType_t)*g_Options.weathertype; 

                rain_ent->PreDataUpdate(DataUpdateType_t::DATA_UPDATE_CREATED);
                rain_ent->OnPreDataChanged(DataUpdateType_t::DATA_UPDATE_CREATED);

                rain_ent->GetMins() = Vector(-32767.0f, -32767.0f, -32767.0f);
                rain_ent->GetMaxs() = Vector(32767.0f, 32767.0f, 32767.0f);

                rain_ent->OnDataChanged(DataUpdateType_t::DATA_UPDATE_CREATED);
                rain_ent->PostDataUpdate(DataUpdateType_t::DATA_UPDATE_CREATED);
                 
                printfdbg("Created precipitation %x\n", rain_ent); 
            }
        }
    }

}


