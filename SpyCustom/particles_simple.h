#ifndef PARTICLES_SIMPLE_H
#define PARTICLES_SIMPLE_H
#ifdef _WIN32
#pragma once
#endif

#include "particlemgr.h"
#include "ParticleSphereRenderer.h"
#include "smartptr.h"


class CParticleEffect : public IParticleEffect
{
public:
	DECLARE_CLASS_NOBASE(CParticleEffect);

	friend class CRefCountAccessor;

	void				SetSortOrigin(const Vector& vSortOrigin);

	PMaterialHandle		GetPMaterial(const char* name);

	Particle* AddParticle(unsigned int particleSize, PMaterialHandle material, const Vector& origin);

	CParticleEffectBinding& GetBinding() { return m_ParticleEffect; }

	const char* GetEffectName();

	void AddFlags(int iFlags) { m_Flags |= iFlags; }
	void RemoveFlags(int iFlags) { m_Flags &= ~iFlags; }

	void SetDontRemove(bool bSet)
	{
		if (bSet)
			AddFlags(FLAG_DONT_REMOVE);
		else
			RemoveFlags(FLAG_DONT_REMOVE);
	}

public:

	virtual void				SetParticleCullRadius(float radius);
	virtual void				NotifyRemove(void);
	virtual const Vector& GetSortOrigin();
	virtual void				NotifyDestroyParticle(Particle* pParticle);
	virtual void				Update(float flTimeDelta);

	void						SetDynamicallyAllocated(bool bDynamic = true);

	virtual bool				ShouldSimulate() const { return m_bSimulate; }
	virtual void				SetShouldSimulate(bool bSim) { m_bSimulate = bSim; }

	int							AllocateToolParticleEffectId();
	int							GetToolParticleEffectId() const;
protected:
	CParticleEffect(const char* pDebugName);
	virtual						~CParticleEffect();

	int							IsReleased();

	enum
	{
		FLAG_ALLOCATED = (1 << 1),	        
								FLAG_DONT_REMOVE = (1 << 2),
	};

	char const* m_pDebugName;

	CParticleEffectBinding		m_ParticleEffect;
	Vector						m_vSortOrigin;

	int							m_Flags;		   

	bool						m_bSimulate;
	int							m_nToolParticleEffectId;

private:
	void						AddRef();
	void						Release();

	int							m_RefCount;		            
	CParticleEffect(const CParticleEffect&);     
};

inline int CParticleEffect::GetToolParticleEffectId() const
{
	return m_nToolParticleEffectId;
}

inline int CParticleEffect::AllocateToolParticleEffectId()
{
	m_nToolParticleEffectId = ParticleMgr()->AllocateToolParticleEffectId();
	return m_nToolParticleEffectId;
}


enum SimpleParticleFlag_t
{
	SIMPLE_PARTICLE_FLAG_WINDBLOWN = 0x1,
	SIMPLE_PARTICLE_FLAG_NO_VEL_DECAY = 0x2	           
};

class SimpleParticle : public Particle
{
public:
	SimpleParticle() : m_iFlags(0) {}

	Vector		m_vecVelocity;
	float		m_flRoll;
	float		m_flDieTime;	     
	float		m_flLifetime;	         
	unsigned char	m_uchColor[3];
	unsigned char	m_uchStartAlpha;
	unsigned char	m_uchEndAlpha;
	unsigned char	m_uchStartSize;
	unsigned char	m_uchEndSize;
	unsigned char 	m_iFlags;	   
	float		m_flRollDelta;
};



class CSimpleEmitter : public CParticleEffect
{
public:

	DECLARE_CLASS(CSimpleEmitter, CParticleEffect);

	static CSmartPtr<CSimpleEmitter>	Create(const char* pDebugName);

	virtual void	SimulateParticles(CParticleSimulateIterator* pIterator);
	virtual void	RenderParticles(CParticleRenderIterator* pIterator);

	void			SetNearClip(float nearClipMin, float nearClipMax);

	void			SetDrawBeforeViewModel(bool state = true);

	SimpleParticle* AddSimpleParticle(PMaterialHandle hMaterial, const Vector& vOrigin, float flDieTime = 3, unsigned char uchSize = 10);

	void			SetShouldDrawForSplitScreenUser(int nSlot);

protected:
	CSimpleEmitter(const char* pDebugName = NULL);
	virtual			~CSimpleEmitter();

	virtual	float	UpdateAlpha(const SimpleParticle* pParticle);
	virtual float	UpdateScale(const SimpleParticle* pParticle);
	virtual	float	UpdateRoll(SimpleParticle* pParticle, float timeDelta);
	virtual	void	UpdateVelocity(SimpleParticle* pParticle, float timeDelta);
	virtual Vector	UpdateColor(const SimpleParticle* pParticle);

	float			m_flNearClipMin;
	float			m_flNearClipMax;

	int				m_nSplitScreenPlayerSlot;


private:
	CSimpleEmitter(const CSimpleEmitter&);     
};

class CEmberEffect : public CSimpleEmitter
{
public:
	CEmberEffect(const char* pDebugName);
	static CSmartPtr<CEmberEffect>	Create(const char* pDebugName);

	virtual void UpdateVelocity(SimpleParticle* pParticle, float timeDelta);
	virtual Vector UpdateColor(const SimpleParticle* pParticle);

private:
	CEmberEffect(const CEmberEffect&);     
};


class CFireSmokeEffect : public CSimpleEmitter
{
public:
	CFireSmokeEffect(const char* pDebugName);
	static CSmartPtr<CFireSmokeEffect>	Create(const char* pDebugName);

	virtual void UpdateVelocity(SimpleParticle* pParticle, float timeDelta);
	virtual float UpdateAlpha(const SimpleParticle* pParticle);

protected:
	VPlane	m_planeClip;

private:
	CFireSmokeEffect(const CFireSmokeEffect&);     
};


class CFireParticle : public CSimpleEmitter
{
public:
	CFireParticle(const char* pDebugName);
	static CSmartPtr<CFireParticle>	Create(const char* pDebugName);

	virtual Vector UpdateColor(const SimpleParticle* pParticle);

private:
	CFireParticle(const CFireParticle&);     
};


#endif  