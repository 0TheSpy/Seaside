#ifndef OPTIONS
#define OPTIONS
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}


template <typename T = bool>
class Var {
public:
    std::string name;
    std::shared_ptr<T> value;
    int32_t size;
    Var(std::string name, T v) : name(name) {
        value = std::make_shared<T>(v);
        size = sizeof(T);
    }
    operator T() { return *value; }
    operator T* () { return &*value; }
    operator T() const { return *value; }
};


class float3 
{
public:
    float3() { r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f; }
    float3(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    float3(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

class char256
{
public:
    char256(std::string s)
    {
        strcpy(mystring, s.c_str());
    }
    char mystring[256];
};

class char1024
{
public:
    char1024(std::string s)
    {
        strcpy(mystring, s.c_str());
    }
    char mystring[1024];
};

class sticker
{
public:
    bool isselected = 1;
    char name[256] = "null";
    int stickerid = 0;
    float wear = 0;
    float rotation = 0;
    float scale = 1.0f;
    int stickerc = 0;
    
};


class weapon
{
public:
    weapon() {};
    weapon(char name1[256], int defindex1, char killfeedicon1[256])
    {
        strcpy(name, name1);
        strcpy(killfeedicon, killfeedicon1);
        defindex = defindex1;
        strcpy(killfeediconreplace, killfeedicon1);

        strcpy(stickers[0].name, "(0) null");
        strcpy(stickers[1].name, "(1) null");
        strcpy(stickers[2].name, "(2) null");
        strcpy(stickers[3].name, "(3) null");
        strcpy(stickers[4].name, "(4) null");
        modeldefindex = defindex;
    }
    char name[256] = "";
    int defindex = 0;
    char killfeedicon[256] = "";

    bool active = 0;
    int skinid = 0;
    int skinidc = 0;
    int seed = 0;
    int stattrak = -1;
    bool ownerunk = 0;
    float wear = 0;
    int quality = 0;
    int qualityc = 0;
    char nametag[256] = "";
    char desctag[256] = "";
    char model[256] = "";
    char worldmodel[256] = "";
    int modeldefindex = 0;
    bool modelactive = 0;
    int modelcount = 0;
    char killfeediconreplace[256] = "";
    bool killfeediconactive = 0;

    int rarity = 0;
    int color1 = 0;
    int color2 = 0;
    int color3 = 0;
    int color4 = 0;
    float pearl = 0;
    DWORD link = 0;

    sticker stickers[5];
    

    int isSelected = 0;
};



class weaponz
{
public:
    weaponz() 
    {
        int i = 0;
        arr[i] = weapon((char*)"Knife", WEAPON_KNIFE_T, (char*)"knife_t"); i++;
        arr[i] = weapon((char*)"Glove", GLOVE_STUDDED_BLOODHOUND, (char*)"not_available"); i++;
        arr[i] = weapon((char*)"Agent T", 5036, (char*)"not_available"); i++;
        arr[i] = weapon((char*)"Agent CT", 5037, (char*)"not_available"); i++;
        arr[i] = weapon((char*)"Medal", 1331, (char*)"not_available"); i++;
        arr[i] = weapon((char*)"Music Kit", 1314, (char*)"not_available"); i++;

        arr[i] = weapon((char*)"AK-47", WEAPON_AK47, (char*)"ak47"); i++;
        arr[i] = weapon((char*)"C4 Explosive", WEAPON_C4, (char*)"c4"); i++;

        arr[i] = weapon((char*)"Desert Eagle", WEAPON_DEAGLE, (char*)"deagle"); i++;
        arr[i] = weapon((char*)"Dual Berettas", WEAPON_ELITE, (char*)"elite"); i++;
        arr[i] = weapon((char*)"Five-SeveN", WEAPON_FIVESEVEN, (char*)"fiveseven"); i++;
        arr[i] = weapon((char*)"Glock-18", WEAPON_GLOCK, (char*)"glock"); i++;
        arr[i] = weapon((char*)"AUG", WEAPON_AUG, (char*)"aug"); i++;
        arr[i] = weapon((char*)"AWP", WEAPON_AWP, (char*)"awp"); i++;
        arr[i] = weapon((char*)"FAMAS", WEAPON_FAMAS, (char*)"famas"); i++;
        arr[i] = weapon((char*)"G3SG1", WEAPON_G3SG1, (char*)"g3sg1"); i++;
        arr[i] = weapon((char*)"Galil AR", WEAPON_GALILAR, (char*)"galilar"); i++;
        arr[i] = weapon((char*)"M249", WEAPON_M249, (char*)"m249"); i++;

        arr[i] = weapon((char*)"M4A4", WEAPON_M4A1, (char*)"m4a1"); i++;
        arr[i] = weapon((char*)"MAC-10", WEAPON_MAC10, (char*)"mac10"); i++;
        arr[i] = weapon((char*)"P90", WEAPON_P90, (char*)"p90"); i++;
        arr[i] = weapon((char*)"UMP-45", WEAPON_UMP45, (char*)"ump45"); i++;
        arr[i] = weapon((char*)"XM1014", WEAPON_XM1014, (char*)"xm1014"); i++;
        arr[i] = weapon((char*)"PP-Bizon", WEAPON_BIZON, (char*)"bizon"); i++;
        arr[i] = weapon((char*)"MAG-7", WEAPON_MAG7, (char*)"mag7"); i++;
        arr[i] = weapon((char*)"Negev", WEAPON_NEGEV, (char*)"negev"); i++;
        arr[i] = weapon((char*)"Sawed-Off", WEAPON_SAWEDOFF, (char*)"sawedoff"); i++;

        arr[i] = weapon((char*)"Tec-9", WEAPON_TEC9, (char*)"tec9"); i++;
        arr[i] = weapon((char*)"P2000", WEAPON_HKP2000, (char*)"hkp2000"); i++; 
        arr[i] = weapon((char*)"MP7", WEAPON_MP7, (char*)"mp7"); i++;
        arr[i] = weapon((char*)"MP9", WEAPON_MP9, (char*)"mp9"); i++;
        arr[i] = weapon((char*)"Nova", WEAPON_NOVA, (char*)"nova"); i++;
        arr[i] = weapon((char*)"P250", WEAPON_P250, (char*)"p250"); i++;
        arr[i] = weapon((char*)"SCAR-20", WEAPON_SCAR20, (char*)"scar20"); i++;
        arr[i] = weapon((char*)"SG 553", WEAPON_SG553, (char*)"sg556"); i++;
        arr[i] = weapon((char*)"SSG 08", WEAPON_SSG08, (char*)"ssg08"); i++;

        arr[i] = weapon((char*)"M4A1-S", WEAPON_M4A1_SILENCER, (char*)"m4a1_silencer"); i++;
        arr[i] = weapon((char*)"USP-S", WEAPON_USP_SILENCER, (char*)"usp_silencer"); i++;

        arr[i] = weapon((char*)"CZ75-Auto", WEAPON_CZ75A, (char*)"cz75a"); i++;
        arr[i] = weapon((char*)"RB Revolver", WEAPON_REVOLVER, (char*)"revolver"); i++;

        arr[i] = weapon((char*)"Custom item", 0, (char*)"not_available"); i++;

        weaponzcount = i;
    }


    weapon arr[64];
    int weaponzcount;
};


class Rank
{
public:
    Rank() {};
    Rank(char Name1[256]) 
    {
       strcpy(Name, Name1);
    }
    char Name[256] = "";
    int rank = 0;
    int wins = 0;
    int isselected = 0;
};

class Rankz
{
public:
    Rankz()
    {
        arr[0] = Rank((char*)"Competitive MM");
        arr[1] = Rank((char*)"Wingman");
        arr[2] = Rank((char*)"Danger Zone");
    }
    Rank arr[3];
    int selectedrank = 0;
};


#include "NetVarManager.hpp"  

class DME
{
public:
    DME() 
    {
    };
    DME(char name1[256], int cust1 = -1)
    {
        strcpy(name, name1);
        customtextureselected = cust1;
    }
    DME(char name1[256], char texture1[256], char model[256], int cust1 = -1)
    {
        strcpy(name, name1);
        strcpy(texture, texture1);
        strcpy(texture_temp, texture1); 
        model_hash = fnv2::hashRuntime(model); 
        customtextureselected = cust1;
    }

    DME(char name[256], char prefab[32], int cust1 = -1)
    {
        strcpy(this->name, name);
        strcpy(this->prefab, prefab);
        customtextureselected = cust1;
    }

    char texture_temp[256] = "";
    char texture[256] = ""; 
    char prefab[32];
    int model_hash = 0;
    char name[256] = "";
    bool active = 0;
    bool wireframe = 0;
    bool nodraw = 0;
    bool flat = 0;
    ImVec4 coloralpha = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool isSelected = 0;
    int customtextureselected = -1;
};

class DMEs
{
public:
    DMEs() 
    {
        int i = 0;
        arr[i] = DME((char*)"Weapons", (char*)"(0) water", (char*)"", 0); i++;
        arr[i] = DME((char*)"Glove", (char*)"(0) water", (char*)"", 0); i++;
        arr[i] = DME((char*)"Sleeve", (char*)"(0) water", (char*)"", 0); i++;
        arr[i] = DME((char*)"Agent T", (char*)"(0) water", (char*)"", 0); i++;
        arr[i] = DME((char*)"Agent CT", (char*)"(0) water", (char*)"", 0); i++;
        arr[i] = DME((char*)"Knife", (char*)"(0) water", (char*)"", 0); i++;

        arr[i] = DME((char*)"AK-47"); i++;
        arr[i] = DME((char*)"C4 Explosive"); i++;
        arr[i] = DME((char*)"Desert Eagle"); i++;
        arr[i] = DME((char*)"Dual Berettas"); i++;
        arr[i] = DME((char*)"Five-SeveN"); i++;
        arr[i] = DME((char*)"Glock-18"); i++;
        arr[i] = DME((char*)"AUG"); i++;
        arr[i] = DME((char*)"AWP"); i++;
        arr[i] = DME((char*)"FAMAS"); i++;
        arr[i] = DME((char*)"G3SG1"); i++;
        arr[i] = DME((char*)"Galil AR"); i++;
        arr[i] = DME((char*)"M249"); i++;
        arr[i] = DME((char*)"M4A4"); i++;
        arr[i] = DME((char*)"MAC-10"); i++;
        arr[i] = DME((char*)"P-90"); i++;
        arr[i] = DME((char*)"UMP-45"); i++;
        arr[i] = DME((char*)"XM1014"); i++;
        arr[i] = DME((char*)"PP-Bizon"); i++;
        arr[i] = DME((char*)"MAG-7"); i++;
        arr[i] = DME((char*)"Negev"); i++;
        arr[i] = DME((char*)"Sawed-Off"); i++;
        arr[i] = DME((char*)"Tec-9"); i++;
        arr[i] = DME((char*)"P2000"); i++;
        arr[i] = DME((char*)"MP7"); i++;
        arr[i] = DME((char*)"MP9"); i++;
        arr[i] = DME((char*)"Nova"); i++;
        arr[i] = DME((char*)"P250"); i++;
        arr[i] = DME((char*)"SCAR-20"); i++;
        arr[i] = DME((char*)"SG 553"); i++;
        arr[i] = DME((char*)"SSG 08"); i++;
         
        itemcount = i;
    };

    int itemcount = 0;
    DME arr[64];
};


class Customtexture
{
public:
    Customtexture() {};
    Customtexture(int id)
    {
        char preview1[64] = "(";
        strcat_s(preview1, std::to_string(id).c_str());
        strcat_s(preview1, ") ");
        strcat_s(preview1, "null");
        strcpy(Preview, preview1);
    }
    Customtexture(char name1[256], char keyvalue1[0x1000], int id)
    {
        strcpy(Name, name1); 
        char preview1[64] = "(";
        strcat_s(preview1, std::to_string(id).c_str());
        strcat_s(preview1, ") ");
        strcat_s(preview1, name1 );
        strcpy(Preview, preview1); 
        strcpy(keyvalue, keyvalue1);
    }
    char keyvalue[0x1000] = "";
    char Name[256] = "";
    char Preview[256] = "";
    bool IsSelected = 0;

    void* texturelink = nullptr;
};

class Customtextures
{
public:
    Customtextures()
    {
        int i = 0;
        char buf[0x1000] = "\"Refract\"\n{\n\t\"$model\" 1\n\t\"$refractamount\" \"2.0\"\n\t\"$refracttint\" \"[0 1 1 100]\"\n\t\"$dudvmap\" \"dev/water_dudv\"\n\t\"$normalmap\" \"dev/water_normal\"\n\t\"$surfaceprop\" \"water\"\n\t\"$bumpframe\" \"0\"\n\t\"Proxies\"\n\t{\n\t\t\"AnimatedTexture\"\n\t\t{\n\t\t\t\"animatedtexturevar\" \"$normalmap\"\n\t\t\t\"animatedtextureframenumvar\" \"$bumpframe\"\n\t\t\t\"animatedtextureframerate\" 30.00\n\t\t}\n\t\t\"TextureScroll\"\n\t\t{\n\t\t\t\"texturescrollvar\" \"$bumptransform\"\n\t\t\t\"texturescrollrate\" .05\n\t\t\t\"texturescrollangle\" 45.00\n\t\t}\n\t}\n}";
        arr[i] = Customtexture((char*)"water", buf, i); i++;
        arr[i] = Customtexture(i); i++;
        arr[i] = Customtexture(i); i++;
        arr[i] = Customtexture(i); i++;
        arr[i] = Customtexture(i); i++;
        count = i;
    }
    Customtexture arr[5];
    int count = 0;
};


class Seqs
{
public:
    Seqs() {};
    int seq_orig = 0;
    char seq_orig_def[32] = "";
    int seq_repl = 0;
    char seq_repl_def[32] = "";

    char seq_repl_def_preview[32] = "";

    bool isSelected[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
};

class Sounds
{
public:
    Sounds() {};
    char name[32] = "";
    int hash = 0;
    char wavpath[256] = "ui/beep07.wav";
    int pitch = 100;
    float volume = 1.0f;
};

class Model
{
public:
    Model() {};

    Model(char name[32])
    {
        strcpy(this->name, name);
    };

    Model(char name[32], int defindex, char classid[32], char prefab[32])
    {
        this->defindex = defindex;
        strcpy(this->name, name);
        strcpy(this->classid, classid);
        strcpy(this->prefab, prefab);
    };

    Model(char name[32], int defindex, char classid[256], char vmodel_orig[32], bool s)
    {
        this->defindex = defindex;
        strcpy(this->name, name);
          strcpy(this->vmodel_orig, vmodel_orig);
          strcpy(vmodel_repl_temp, vmodel_orig);
          strcpy(vmodel_repl, vmodel_orig);
        strcpy(this->classid, classid);
    };

    Model(char name[32], char vmodel[32])
    {
        strcpy(this->name, name);
        strcpy(this->vmodel_repl_temp, vmodel);
        strcpy(this->vmodel_repl, vmodel);
    };

    bool findMDLmethode = false;

    char name[32] = "";

    char vmodel_orig[256] = "";
    
    char vmodel_repl_temp[256] = "";
    char wmodel_repl_temp[256] = "";
    
    char vmodel_repl[256] = "";
    char wmodel_repl[256] = "";

    int vmodel_hash = 0;
    int wmodel_hash = 0;

    int defindex = 0;
    bool active = 0;
    bool active_w = 0;
    bool active_scale = 0;
    float scale = 1.0f;

    bool seq_active = 0;
    Seqs seqs[20];

    char classid[32] = "";

    char prefab[32] = "";
    Sounds sounds[5];
    int soundcount = 0;
    bool sound_active = 0;

    bool isSelected = 0;
};

class Models
{
public:
    Models() {   
        int i = 0;

        arr[i] = Model((char*)"Agent T", (char*)"models/player/zombie.mdl"); i++;
        arr[i] = Model((char*)"Agent CT",(char*)"models/player/zombie.mdl"); i++;
        arr[i] = Model((char*)"Arms"); i++;

        arr[i] = Model((char*)"Knife", WEAPON_KNIFE_T, (char*)"CKnife", (char*)"models/weapons/v_knife_default_t.mdl", 1); i++; 

        arr[i] = Model((char*)"AK-47", WEAPON_AK47, (char*)"CAK47", (char*)"weapon_ak47_prefab"); i++; 
        arr[i] = Model((char*)"C4 Explosive", WEAPON_C4, (char*)"CC4", (char*)"c4"); i++; 
        arr[i] = Model((char*)"Desert Eagle", WEAPON_DEAGLE, (char*)"CWeaponDeagle", (char*)"weapon_deagle_prefab"); i++;
        arr[i] = Model((char*)"Dual Berettas", WEAPON_ELITE, (char*)"CWeaponElite", (char*)"weapon_elite_prefab"); i++;
        arr[i] = Model((char*)"Five-SeveN", WEAPON_FIVESEVEN, (char*)"CWeaponFiveSeven", (char*)"weapon_fiveseven_prefab"); i++;
        arr[i] = Model((char*)"Glock-18", WEAPON_GLOCK, (char*)"CWeaponGlock", (char*)"weapon_glock_prefab"); i++; 
        arr[i] = Model((char*)"AUG", WEAPON_AUG, (char*)"CWeaponAug", (char*)"weapon_aug_prefab"); i++;
        arr[i] = Model((char*)"AWP", WEAPON_AWP, (char*)"CWeaponAWP", (char*)"weapon_awp_prefab"); i++;
        arr[i] = Model((char*)"FAMAS", WEAPON_FAMAS, (char*)"CWeaponFamas", (char*)"weapon_famas_prefab"); i++;
        arr[i] = Model((char*)"G3SG1", WEAPON_G3SG1, (char*)"CWeaponG3SG1", (char*)"weapon_g3sg1_prefab"); i++;
        arr[i] = Model((char*)"Galil AR", WEAPON_GALILAR, (char*)"CWeaponGalilAR", (char*)"weapon_galilar_prefab"); i++;
        arr[i] = Model((char*)"M249", WEAPON_M249, (char*)"CWeaponM249", (char*)"weapon_m249_prefab"); i++;
        arr[i] = Model((char*)"M4A4", WEAPON_M4A1, (char*)"CWeaponM4A1", (char*)"weapon_m4a1_prefab"); i++;
        arr[i] = Model((char*)"MAC-10", WEAPON_MAC10, (char*)"CWeaponMAC10", (char*)"weapon_mac10_prefab"); i++;
        arr[i] = Model((char*)"P-90", WEAPON_P90, (char*)"CWeaponP90", (char*)"weapon_p90_prefab"); i++;
        arr[i] = Model((char*)"UMP-45", WEAPON_UMP45, (char*)"CWeaponUMP45", (char*)"weapon_ump45_prefab"); i++;
        arr[i] = Model((char*)"XM1014", WEAPON_XM1014, (char*)"CWeaponXM1014", (char*)"weapon_xm1014_prefab"); i++;
        arr[i] = Model((char*)"PP-Bizon", WEAPON_BIZON, (char*)"CWeaponBizon", (char*)"weapon_bizon_prefab"); i++;
        arr[i] = Model((char*)"MAG-7", WEAPON_MAG7, (char*)"CWeaponMag7", (char*)"weapon_mag7_prefab"); i++;
        arr[i] = Model((char*)"Negev", WEAPON_NEGEV, (char*)"CWeaponNegev", (char*)"weapon_negev_prefab"); i++;
        arr[i] = Model((char*)"Sawed-Off", WEAPON_SAWEDOFF, (char*)"CWeaponSawedoff", (char*)"weapon_sawedoff_prefab"); i++;
        arr[i] = Model((char*)"Tec-9", WEAPON_TEC9, (char*)"CWeaponTec9", (char*)"weapon_tec9_prefab"); i++;
        arr[i] = Model((char*)"P2000", WEAPON_HKP2000, (char*)"CWeaponHKP2000", (char*)"weapon_hkp2000_prefab"); i++;
        arr[i] = Model((char*)"MP7", WEAPON_MP7, (char*)"CWeaponMP7", (char*)"weapon_mp7_prefab"); i++;
        arr[i] = Model((char*)"MP9", WEAPON_MP9, (char*)"CWeaponMP9", (char*)"weapon_mp9_prefab"); i++;
        arr[i] = Model((char*)"Nova", WEAPON_NOVA, (char*)"CWeaponNOVA", (char*)"weapon_nova_prefab"); i++;
        arr[i] = Model((char*)"P250", WEAPON_P250, (char*)"CWeaponP250", (char*)"weapon_p250_prefab"); i++;
        arr[i] = Model((char*)"SCAR-20", WEAPON_SCAR20, (char*)"CWeaponSCAR20", (char*)"weapon_scar20_prefab"); i++;
        arr[i] = Model((char*)"SG 553", WEAPON_SG553, (char*)"CWeaponSG556", (char*)"weapon_sg556_prefab"); i++;
        arr[i] = Model((char*)"SSG 08", WEAPON_SSG08, (char*)"CWeaponSSG08", (char*)"weapon_ssg08_prefab"); i++; 

         


        itemcount = i;
    }
    Model arr[64];
    int itemcount = 0;
};


class Effects 
{
public:
    Effects() {};
    int type = 2; 
    int selectedc = 0;

    int fx_magnitude = 1;
    int fx_traillength = 1;
    float fx_fsize = 5.0f; 
    float fx_fspeed = 1.0f; 
    int fx_itype = 1;
    bool fx_bexplosive = false;

    int icolors = 0;
    int fflags = 0;
    float fscale = 1.0f;
    float fmagtinude = 1.0f;
    float fradius = 1.0f;

    float fbeamwidth = 1.0f;
    float teslaradius = 5.0f;
    float teslacolor[3] = { 1.0f,0,0 };
    float fltimevisible = 0.75f;
    int ibeams = 3;
    char szSpriteName[256] = "sprites/physbeam.vmt";
    float dlightcolor[3] = { 1.0f,0,0 };
    float fdie = 0.1f;
    float frad = 500.0f;
    int style = 1;  
    int key = 1;
    float decay = 0.0f;
};


inline Vector CalcAngle(register const Vector& src, register const Vector& dst)
{
    Vector angles; 
    Vector delta = src - dst; 
    float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);  
    angles.x = atan(delta.z / hyp) * (180.0f / 3.14);  
    angles.y = atanf(delta.y / delta.x) * (180.0f / 3.14) + !((*(DWORD*)&delta.x) >> 31 & 1) * 180.0f;  
    angles.z = 0.0f;  
    return angles;
}

class Options
{
public:
    OPTION(bool, attacheffects, false);
    OPTION(Effects, effects, Effects());
    OPTION(bool, beamtrace, false);

    OPTION(int, beamtype, 0); 
    OPTION(float3, beamcolor, float3(0.95f, 0.20f, 0.0f, 1.0f));

    OPTION(char256, beampath, char256("sprites/purplelaser1.vmt"));
    OPTION(float, beamlife, 1.0f); 

    OPTION(float, beamwidth, 2.5f);
    OPTION(float, beamendwidth, 2.5f);

    OPTION(float, beamhaloscale, 0.0f);
    OPTION(float, beamfadelength, 0.3f); 

    OPTION(float, beamamplitude, 5.0f); 
    OPTION(float, beamstartframe, 0.0f);

    OPTION(float, beamspeed, 0.3f); 
    OPTION(float, beamframerate, 1.0f); 

    OPTION(int, beamsegments, 2);
    OPTION(int, beamflags, 0x8300); 

    OPTION(bool, hitsound, false);
    OPTION(char256, hspath, char256("hitsound_crit.wav")); 
    OPTION(char256, obpath, char256("hitsound_cod.wav")); 

    OPTION(bool, hitmarker, false);

    OPTION(bool, flashlightON, false);
    OPTION(bool, flashlightShadows, false);
    OPTION(float, flashlightFOV, 45.0f);
    OPTION(float, flashlightLinearAtten, 1000.0f);
    OPTION(float, flashlightFarZ, 1000.0f);
    OPTION(char256, flashlightTexture, char256("effects/flashlight001"));
    OPTION(bool, nvgsON, false);
    OPTION(bool, fogactive, false);
    OPTION(float3, fogcolor, float3(0,0,0) ); 
    OPTION(float, fogdensity, 0);
    OPTION(float, fogstart, 0);
    OPTION(float, fogend, 0);
    OPTION(float, tonemapscale, 1.0f);
    OPTION(bool, skyboxactive, false);
    OPTION(char256, skyboxtemp, char256("sky_day02_05"));

    OPTION(bool, weatheractive, false);
    OPTION(int, weathertype, 1);
    OPTION(float, windspeed, 0);
    OPTION(float, rainalpha, 0.4f);
    OPTION(float, rainlength, 0.1f);
    OPTION(float, rainwidth, 0.5f);
    OPTION(float, rainsidevel, 130.0f);
    OPTION(float, rainradius, 1500.0f);
    OPTION(float, rainspeed, 600.0f);

    OPTION(bool, worldcoloractive, false); 
    OPTION(float3, worldcolor, float3(1.0f, 1.0f, 1.0f));
    OPTION(float3, staticpropcolor, float3(1.0f, 1.0f, 1.0f));
    OPTION(float3, modelcolor, float3(1.0f, 1.0f, 1.0f));
    OPTION(float3, smokeskycolor, float3(1.0f, 1.0f, 1.0f));
    OPTION(Models, models, Models());
    OPTION(int, soundhook_count, 0);
    OPTION(int, entityloop_count, 0);
    OPTION(int, playerloop_count, 0);
    OPTION(DMEs, materials, DMEs());
    OPTION(Customtextures, customtextures, Customtextures());
    OPTION(bool, dme_gettextures, false);
    OPTION(int, dmeloop_count, 0);
    OPTION(weaponz, weapons, weaponz());
    OPTION(char1024, playername, char1024("Your name"));
    OPTION(char1024, playerclan, char1024("Your tag"));

    OPTION(bool, discmsg_active, false);
    OPTION(char1024, discmsg, char1024("SFUI_QMM_ERROR_VacBanned")); 

    OPTION(bool, profile_active, false);
    OPTION(bool, prime, false);

    OPTION(Rankz, rankz, Rankz());

    OPTION(int, level, 0);
    OPTION(int, xp, 0);

    OPTION(int, comleader, 0);
    OPTION(int, comteacher, 0);
    OPTION(int, comfriendly, 0);

    OPTION(bool, fakescore, 0);
    OPTION(int, MVPs, 0);
    OPTION(int, score, 0);
    OPTION(int, kills, 0);
    OPTION(int, assists, 0);
    OPTION(int, deaths, 0);

    OPTION(bool, vacban, 0);
    OPTION(int, banreason, 0);
    OPTION(int, banduration, 0);

    OPTION(bool, votereveal, 0);

    OPTION(bool, postproc, false); 
    OPTION(float, fov, 90.0f);
    OPTION(float, viewmodel_fov, 60.0f);
    OPTION(float, viewmodel_x, 1.0f);
    OPTION(float, viewmodel_y, 1.0f);
    OPTION(float, viewmodel_z, -1.0f);
    OPTION(float, viewmodel_ang_x, 0.0f);
    OPTION(float, viewmodel_ang_y, 0.0f);
    OPTION(float, viewmodel_ang_z, 0.0f);
    OPTION(bool, lefthand, false);
    OPTION(bool, viewmodel_moving, false);
    OPTION(float, aspectratio, 0.0f);
    OPTION(bool, shadows, false);
    OPTION(float, ragdollgravity, 600.0f);
    OPTION(float, ragdolltime, 1.0f);
    OPTION(int, fullbright, 0);
    OPTION(bool, drawgray, 0);
    OPTION(bool, showlowresimage, 0); 

    OPTION(bool, bunnyhop, 0);
    OPTION(bool, autostrafe, 0);
    OPTION(bool, slidewalk, 0);
    OPTION(bool, fastduck, 0);
    OPTION(int, changing_name, 0);
    OPTION(bool, c4timer, 0);
    OPTION(bool, rankreveal, 0);
    OPTION(bool, speclist, 0);
    OPTION(bool, debugstuff, 0);
    OPTION(bool, faststop, 0);
    OPTION(bool, moneyreveal, 0);

    OPTION(bool, predict, 0);
    OPTION(bool, blockbot, 0);
};

inline Options g_Options;
inline bool   g_Unload;
 
struct skin
{
    skin(bool active1, int skinid1, int seed1, int stattrak1, bool ownerunk1, float wear1, int quality1, char nametag1[256])
    {
        active = active1;
        skinid = skinid1;
        seed = seed1;
        stattrak = stattrak1;
        ownerunk = ownerunk1;
        wear = wear1;
        quality = quality1;
        strcpy(nametag, nametag1);
    };

    bool active = 0;
    int skinid = 0;
    int skinidc = 0;
    int seed = 0;
    int stattrak = -1;
    bool ownerunk = 0;
    float wear = 0;
    int quality = 0;
    int qualityc = 0;
    char nametag[256] = "";
    char model[256] = "";
    char worldmodel[256] = "";
    int modeldefindex = 0;
    bool modelactive = 0;
    int modelcount = 0;
    char killfeedicon[256] = "";
    char killfeediconorigin[256] = "";
    bool killfeediconactive = 0;
};




class Options_my
{
public:

    std::vector<std::string> IEffects = {
        "Smoke", 
        "Sparks", 
        "Dust", 
        "MuzzleFlash", 
        "MetalSparks", 
        "EnergySplash", 
        "Ricochet" 
    };

    std::vector<std::string> DispatchEffect = {
        "Explosion",
        "WaterSurfaceExplosion",
        "HelicopterMegaBomb",
        "CS_HolidayLight",
        "ManhackSparks",
        "watersplash",
        "watersplashquiet",
        "gunshotsplash",
        "HunterDamage",
        "MuzzleFlash",
        "Smoke",
        "GunshipImpact",
        "BoltImpact",
        "csblood",
        "GlassImpact",
        "RPGShotDown",
        "WheelDust",
        "Error"
    };

    std::vector<std::string> OtherEff = {
      "Tesla", 
      "DLight" 
    };

    std::vector<std::string> EffTypes = {
      "CEffectsClient",
      "CTEEffectDispatch",
      "Other"
    };

    std::map<std::string, std::vector<std::string>> Map = { 
        {"CEffectsClient", IEffects},
        {"CTEEffectDispatch", DispatchEffect},
        {"Other", OtherEff}, 
    };


    HMODULE hModuleGlobal = NULL;

    int needtogetseqact = 0;
    bool netchannedlhooked = 0;

    bool disconnected = 0;
    bool needupdate = 0;
    bool loading = 0; 
    bool unhook = 0;

    std::vector<std::string> names = {
        "\\x10\\xAD\\xAD\\xAD"
    };

    std::vector<std::string> skyboxitems = {
        "sky_day02_05",
        "cs_baggage_skybox_",
        "cs_tibet",
        "vietnam",
        "sky_lunacy",
        "embassy",
        "italy",
        "jungle",
        "office",
        "sky_cs15_daylight01_hdr",
        "sky_cs15_daylight02_hdr",
        "nukeblank",
        "dustblank",
        "sky_venice",
        "sky_cs15_daylight03_hdr",
        "sky_cs15_daylight04_hdr",
        "sky_csgo_cloudy01",
        "sky_csgo_night02",
        "sky_csgo_night02b",
        "vertigo",
        "vertigoblue_hdr",
        "sky_dust",
        "sky_hr_aztec",
        "sky_l4d_rural02_ldr", 
        "vertigo_hdr",
        "sky_csgo_night_flat"
    };
      
    std::vector<std::string> spriteslist = {
         "sprites/purplelaser1.vmt",
         "sprites/physbeam.vmt",
         "sprites/purpleglow1.vmt",
         "sprites/blueglow1.vmt",
         "sprites/white.vmt",
         "sprites/radio.vmt",
         "sprites/gunsmoke.vmt",
         "sprites/bubble.vmt"  
    };


    std::vector<std::string> flashlightlist = {
         "effects/flashlight_border",
         "effects/flashlight_freezecam",
         "effects/flashlight_inspect",
         "effects/flashlight_security001",
         "effects/flashlight001",
         "effects/flashlight001_improved",
         "effects/flashlight001_intro",
         "effects/flashlight002",
         "effects/autumn_access_denied_color",
         "effects/coopphoenixloadingscreen",
         "effects/combinemuzzle1",
         "sprites/purpleglow1",
    };

    std::vector<std::string> dmeitems = {
        "(0) water",
        "(1) null",
        "(2) null",
        "(3) null",
        "(4) null",
    "models/extras/speech_info",
    "models/gibs/glass/glass",
    "models/inventory_items/cologne_prediction/cologne_prediction_glass",
    "models/inventory_items/contributor_map_tokens/contributor_charset_color",
    "models/inventory_items/dogtags/dogtags",
    "models/inventory_items/dogtags/dogtags_lightray",
    "models/inventory_items/dogtags/dogtags_outline",
    "models/inventory_items/dreamhack_trophies/dreamhack_star_blur",
    "models/inventory_items/hydra_crystal/hydra_crystal",
    "models/inventory_items/hydra_crystal/hydra_crystal_detail",
    "models/inventory_items/music_kit/darude_01/mp3_detail",
    "models/inventory_items/payback_gold_01/payback_gold_01",
    "models/inventory_items/phoenix_gold_01/phoenix_gold_01",
    "models/inventory_items/service_medal_2015/glass",
    "models/inventory_items/service_medal_2016/glass_lvl4",
    "models/inventory_items/sticker_inspect/sticker_backing",
    "models/inventory_items/trophy_majors/crystal_blue",
    "models/inventory_items/trophy_majors/crystal_clear",
    "models/inventory_items/trophy_majors/gloss",
    "models/inventory_items/trophy_majors/gold",
    "models/inventory_items/trophy_majors/gold_dust",
    "models/inventory_items/trophy_majors/silver_winners",
    "models/inventory_items/vanguard_gold/vanguard_gold_detail",
    "models/inventory_items/wildfire_gold/wildfire_gold_detail",
    "models/player/ct_fbi/ct_fbi_glass",
    "models/player/t_guerilla/t_guerilla",
    "models/props_foliage/urban_tree03_branches",
    "models/props_shacks/fishing_net01",
    "models/weapons/customization/stickers/cologne2014/esl_c"
    };



    std::vector<std::string> killfeedicons = {
        "Default"
    };



    char sc_selectedweapon[256];
    bool sc_active1 = false;
    int sc_seed1 = 0;
    int sc_stattrak1 = 0;
    bool sc_unk1 = false;
    char sc_selectedskin[256];

    struct stickerstruct
    {
        stickerstruct(int id1, std::string Name1, int count1)
        {
            id = id1;
            Name = Name1;
            count = count1;
            Preview = std::to_string(id).append(": ").append(Name1);
        }
        int id;
        std::string Name;
        int count;
        bool isSelected;
        std::string Preview;
    };

    std::vector<stickerstruct> sc_stickers =
    {

    };

    struct skinstruct
    {
        skinstruct(int id1, std::string Name1, int count1, int rarity1, int color11, int color21, int color31, int color41, float pearl1, DWORD link1)
        {
            id = id1;
            Name = Name1;
            isSelected = 0;
            count = count1;
            Preview = std::to_string(id).append(": ").append(Name1);
            rarity = rarity1;
            color1 = color11;
            color2 = color21;
            color3 = color31;
            color4 = color41;
            pearl = pearl1;
            link = link1;
        }
        int id;
        std::string Name;
        bool isSelected;
        int count;
        std::string Preview;
        int rarity;
        int color1;
        int color2;
        int color3;
        int color4;
        float pearl;
        DWORD link;
    };

    std::vector<skinstruct> sc_skins =
    {

    };
    
    struct selectedweapon
    {
        selectedweapon(std::string Name1, std::string icon1)
        {
            Name = Name1;
            isSelected = 0;
            icon = icon1;
        }; 
        std::string Name;
        std::string icon;
        bool isSelected;
    };
     
    bool show = false;
    int autoconfig = -1;
};

inline Options_my opt; 


#define SOUND_ENTRY_HASH_SEED 0x444F5441

inline uint32 GenerateSoundEntryHash(char const* pSoundEntry)
{
    int nSoundEntryLength = strlen(pSoundEntry);
    char* pSoundEntryLowerCase = (char*)stackalloc(nSoundEntryLength + 1);

    for (int nIndex = 0; nIndex < nSoundEntryLength; nIndex++)
        pSoundEntryLowerCase[nIndex] = tolower(pSoundEntry[nIndex]);

    const uint32 nMagicNumber = 0x5bd1e995;

    uint32 nSoundHash = SOUND_ENTRY_HASH_SEED ^ nSoundEntryLength;

    unsigned char* pData = (unsigned char*)pSoundEntryLowerCase;

    while (nSoundEntryLength >= 4)
    {
        uint32 nLittleDWord = LittleDWord(*(uint32*)pData);

        nLittleDWord *= nMagicNumber;
        nLittleDWord ^= nLittleDWord >> 24;
        nLittleDWord *= nMagicNumber;

        nSoundHash *= nMagicNumber;
        nSoundHash ^= nLittleDWord;

        pData += 4;
        nSoundEntryLength -= 4;
    }

    switch (nSoundEntryLength)
    {
    case 3: nSoundHash ^= pData[2] << 16;
    case 2: nSoundHash ^= pData[1] << 8;
    case 1: nSoundHash ^= pData[0];
        nSoundHash *= nMagicNumber;
    };

    nSoundHash ^= nSoundHash >> 13;
    nSoundHash *= nMagicNumber;
    nSoundHash ^= nSoundHash >> 15;

    return nSoundHash;
}
 

inline int GetCfgIndexByClassId(int ClassID) {
    for (int i = 0; i < g_Options.models.value->itemcount; i++)
    {
        if (fnv2::hash(g_Options.models.value->arr[i].classid) == ClassID)
            return i;
    }
    return -1;
}


inline int GetCfgIndexByPrefab(int Prefab) {
    for (int i = 4; i < g_Options.models.value->itemcount; i++)
    {
        if (fnv2::hash(g_Options.models.value->arr[i].prefab) == Prefab)
            return i;
    }
    return -1;
}


inline int GetCfgIndex(int item_definition_index)
{
    for (int i = 0; i < g_Options.weapons.value->weaponzcount; i++)
    {
        if (g_Options.weapons.value->arr[i].defindex == item_definition_index)
            return i;
    }
    return -1;
}

inline int GetCfgIndex2(int item_definition_index)
{
    for (int i = 2; i < g_Options.models.value->itemcount; i++)
    {
        if (g_Options.models.value->arr[i].defindex == item_definition_index)
            return i;
    }
    return -1;
}


struct soundlist
{
    soundlist(int cfgindex = 0, int sndindex = 0)
    {
        this->cfgindex = cfgindex;
        this->sndindex = sndindex;
    }
    int cfgindex = 0;
    int sndindex = 0; 
};

inline soundlist GetCfgWavpathBySoundHash(int Hash) {
    for (int i = 3; i < g_Options.models.value->itemcount; i++)
    {
        for (int j = 0; j < g_Options.models.value->arr[i].soundcount; j++)
            if (g_Options.models.value->arr[i].sounds[j].hash == Hash)
                if (g_Options.models.value->arr[i].sound_active)
                    return soundlist(i, j);
                else return soundlist(-1, 0);
    }
    return soundlist(-1, 0);
}



void OnLoadCfg();
void OnLevelInit();








#endif
