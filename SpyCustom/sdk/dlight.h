#if !defined ( DLIGHTH )
#define DLIGHTH
#ifdef _WIN32
#pragma once
#endif

#include "vector.h"

enum
{
    DLIGHT_NO_WORLD_ILLUMINATION = 0x1,
    DLIGHT_NO_MODEL_ILLUMINATION = 0x2,

    DLIGHT_ADD_DISPLACEMENT_ALPHA = 0x4,
    DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA = 0x8,
    DLIGHT_DISPLACEMENT_MASK = (DLIGHT_ADD_DISPLACEMENT_ALPHA | DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA),
};

struct dlight_t
{
    int     flags;
    Vector  origin;
    float   radius;
    ColorRGBExp32	   color;           
    float   die;                     
    float   decay;                  
    float   minlight;                
    int     key;
    int     style;               

    Vector  m_Direction;             
    float   m_InnerAngle;
    float   m_OuterAngle;

    float GetRadius() const
    {
        return radius;
    }

    float GetRadiusSquared() const
    {
        return radius * radius;
    }

    float IsRadiusGreaterThanZero() const
    {
        return radius > 0.0f;
    }
};

#endif