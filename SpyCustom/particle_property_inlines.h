#ifndef PARTICLEPROPERTY_H
#pragma message("Do not include particle_property_inlines.h from anywhere other than particle_property.h!")
#pragma error
#endif

#ifndef PARTICLEPROPERTY_INLINES_H
#define PARTICLEPROPERTY_INLINES_H
#ifdef _WIN32
#pragma once
#endif


void CParticleProperty::SetControlPointParent(CNewParticleEffect* pEffect, int whichControlPoint, int parentIdx)
{
	pEffect->SetControlPointParent(whichControlPoint, parentIdx);
}

CNewParticleEffect* CParticleProperty::GetParticleEffectFromIdx(int idx)
{
	return m_ParticleEffects[idx].pParticleEffect.GetObject();
}



#endif   