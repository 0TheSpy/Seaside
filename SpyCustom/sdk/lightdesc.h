#ifndef LIGHTDESC_H
#define LIGHTDESC_H

#include "ssemath.h"
#include "vector.h"

enum LightType_t
{
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT,
};

enum LightType_OptimizationFlags_t
{
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION0 = 1,
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION1 = 2,
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION2 = 4,
	LIGHTTYPE_OPTIMIZATIONFLAGS_DERIVED_VALUES_CALCED = 8,
};

struct LightDesc_t
{
	LightType_t m_Type;										 
	Vector m_Color;											  
	Vector m_Position;										    
	Vector m_Direction;										      
	float  m_Range;											    
	float m_Falloff;										      
	float m_Attenuation0;									    
	float m_Attenuation1;									    
	float m_Attenuation2;									    
	float m_Theta;											       
	float m_Phi;											   

	float m_ThetaDot;
	float m_PhiDot;
	unsigned int m_Flags;
protected:
	float OneOver_ThetaDot_Minus_PhiDot;
	float m_RangeSquared;
public:

	void RecalculateDerivedValues(void);			       

	LightDesc_t(void)
	{
	}

	LightDesc_t(const Vector& pos, const Vector& color)
	{
		InitPoint(pos, color);
	}

	LightDesc_t(const Vector& pos, const Vector& color, const Vector& point_at,
		float inner_cone_boundary, float outer_cone_boundary)
	{
		InitSpot(pos, color, point_at, inner_cone_boundary, outer_cone_boundary);
	}

	void InitPoint(const Vector& pos, const Vector& color);
	void InitDirectional(const Vector& dir, const Vector& color);
	void InitSpot(const Vector& pos, const Vector& color, const Vector& point_at,
		float inner_cone_boundary, float outer_cone_boundary);

	void ComputeLightAtPoints(const FourVectors& pos, const FourVectors& normal,
		FourVectors& color, bool DoHalfLambert = false) const;
	void ComputeNonincidenceLightAtPoints(const FourVectors& pos, FourVectors& color) const;
	void ComputeLightAtPointsForDirectional(const FourVectors& pos,
		const FourVectors& normal,
		FourVectors& color, bool DoHalfLambert = false) const;

	void SetupOldStyleAttenuation(float fQuadatricAttn, float fLinearAttn, float fConstantAttn);

	void SetupNewStyleAttenuation(float fFiftyPercentDistance, float fZeroPercentDistance);


	bool IsDirectionWithinLightCone(const Vector& rdir) const
	{
		return ((m_Type != MATERIAL_LIGHT_SPOT) || (rdir.Dot(m_Direction) >= m_PhiDot));
	}

	float OneOverThetaDotMinusPhiDot() const
	{
		return OneOver_ThetaDot_Minus_PhiDot;
	}
};


inline void LightDesc_t::InitPoint(const Vector& pos, const Vector& color)
{
	m_Type = MATERIAL_LIGHT_POINT;
	m_Color = color;
	m_Position = pos;
	m_Range = 0.0;									 
	m_Attenuation0 = 1.0;
	m_Attenuation1 = 0;
	m_Attenuation2 = 0;
	RecalculateDerivedValues();
}


inline void LightDesc_t::InitDirectional(const Vector& dir, const Vector& color)
{
	m_Type = MATERIAL_LIGHT_DIRECTIONAL;
	m_Color = color;
	m_Direction = dir;
	m_Range = 0.0;									 
	m_Attenuation0 = 1.0;
	m_Attenuation1 = 0;
	m_Attenuation2 = 0;
	RecalculateDerivedValues();
}


inline void LightDesc_t::InitSpot(const Vector& pos, const Vector& color, const Vector& point_at,
	float inner_cone_boundary, float outer_cone_boundary)
{
	m_Type = MATERIAL_LIGHT_SPOT;
	m_Color = color;
	m_Position = pos;
	m_Direction = point_at;
	m_Direction -= pos;
	VectorNormalizeFast(m_Direction);
	m_Falloff = 5.0;										   
	m_Theta = inner_cone_boundary;
	m_Phi = outer_cone_boundary;

	m_Range = 0.0;										 

	m_Attenuation0 = 1.0;
	m_Attenuation1 = 0;
	m_Attenuation2 = 0;
	RecalculateDerivedValues();
}


#endif
