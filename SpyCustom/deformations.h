#ifndef DEFORMATIONS_H
#define DEFORMATIONS_H

#ifdef _WIN32
#pragma once
#endif

#include "platform.h"

enum DeformationType_t
{
	DEFORMATION_CLAMP_TO_BOX_IN_WORLDSPACE = 1,							   
};


struct DeformationBase_t										     
{
	DeformationType_t m_eType;
};


struct BoxDeformation_t : DeformationBase_t
{
	Vector m_SourceMins;									    
	float m_flPad0;
	Vector m_SourceMaxes;
	float m_flPad1;

	Vector m_ClampMins;
	float m_flPad2;
	Vector m_ClampMaxes;
	float m_flPad3;

	FORCEINLINE BoxDeformation_t(void)
	{
		m_eType = DEFORMATION_CLAMP_TO_BOX_IN_WORLDSPACE;
		m_SourceMins.Init(0, 0, 0);
		m_SourceMaxes.Init(-1, -1, -1);

		m_ClampMins.Init(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		m_ClampMaxes.Init(FLT_MAX, FLT_MAX, FLT_MAX);
	}

};



#endif