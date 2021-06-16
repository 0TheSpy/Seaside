#ifndef PARTICLEDRAW_H
#define PARTICLEDRAW_H


class IMaterial;
class CMeshBuilder;
class CParticleSubTexture;


class ParticleDraw
{
	friend class CParticleEffectBinding;

public:

	ParticleDraw();

	void			Init(CMeshBuilder* pMeshBuilder, IMaterial* pMaterial, float fTimeDelta);

	float			GetTimeDelta() const;

	CMeshBuilder* GetMeshBuilder();

	CParticleSubTexture* m_pSubTexture;

private:
	CMeshBuilder* m_pMeshBuilder;
	IMaterial* m_pMaterial;
	float			m_fTimeDelta;
};



inline ParticleDraw::ParticleDraw()
{
	m_pMaterial = 0;
}

inline void ParticleDraw::Init(CMeshBuilder* pMeshBuilder, IMaterial* pMaterial, float fTimeDelta)
{
	m_pMeshBuilder = pMeshBuilder;
	m_pMaterial = pMaterial;
	m_fTimeDelta = fTimeDelta;
}

inline float ParticleDraw::GetTimeDelta() const
{
	return m_fTimeDelta;
}

inline CMeshBuilder* ParticleDraw::GetMeshBuilder()
{
	return m_pMeshBuilder;
}

#endif