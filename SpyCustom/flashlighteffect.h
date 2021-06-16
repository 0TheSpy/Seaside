#ifndef FLASHLIGHTEFFECT_H
#define FLASHLIGHTEFFECT_H
#ifdef _WIN32
#pragma once
#endif
 
#include "MaterialSystemUtil.h" 


class CountdownTimer
{
public:
#ifdef CLIENT_DLL
	DECLARE_PREDICTABLE();
#endif
	DECLARE_CLASS_NOBASE(CountdownTimer);
	DECLARE_EMBEDDED_NETWORKVAR();

	CountdownTimer(void);

	void Reset(void)
	{
		m_timestamp = Now() + m_duration;
	}

	void Start(float duration)
	{
		m_timestamp = Now() + duration;
		m_duration = duration;
	}

	void StartFromTime(float startTime, float duration)
	{
		m_timestamp = startTime + duration;
		m_duration = duration;
	}

	void Invalidate(void)
	{
		m_timestamp = -1.0f;
	}

	bool HasStarted(void) const
	{
		return (m_timestamp > 0.0f);
	}

	bool IsElapsed(void) const
	{
		return (Now() > m_timestamp);
	}

	float GetElapsedTime(void) const
	{
		return Now() - m_timestamp + m_duration;
	}

	float GetRemainingTime(void) const
	{
		return (m_timestamp - Now());
	}

	float GetCountdownDuration(void) const
	{
		return (m_timestamp > 0.0f) ? m_duration : 0.0f;
	}

	float GetRemainingRatio(void) const
	{
		if (HasStarted())
		{
			float left = GetRemainingTime() / m_duration;
			if (left < 0.0f)
				return 0.0f;
			if (left > 1.0f)
				return 1.0f;
			return left;
		}

		return 0.0f;
	}

private:
	CNetworkVar(float, m_duration);
	CNetworkVar(float, m_timestamp);
	float Now(void) const;		        
};

struct dlight_t;




class CFlashlightEffect
{
public:

	CFlashlightEffect(int nEntIndex = 0, const char* pszTextureName = NULL, float flFov = 0.0f, float flFarZ = 0.0f, float flLinearAtten = 0.0f);
	~CFlashlightEffect();

	void UpdateLight(int nEntIdx, const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp, float flFov,
		float flFarZ, float flLinearAtten, bool castsShadows, const char* pTextureName);
	void UpdateLight(int nEntIdx, const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp, float flFov,
		bool castsShadows, ITexture* pFlashlightTexture, const Vector& vecBrightness, bool bTracePlayers = true);

	void TurnOn();
	void TurnOff();
	void SetMuzzleFlashEnabled(bool bEnabled, float flBrightness);
	bool IsOn(void) { return m_bIsOn; }

	ClientShadowHandle_t GetFlashlightHandle(void) { return m_FlashlightHandle; }
	void SetFlashlightHandle(ClientShadowHandle_t Handle) { m_FlashlightHandle = Handle; }

	const char* GetFlashlightTextureName(void) const
	{
		return m_textureName;
	}

	int GetEntIndex(void) const
	{
		return m_nEntIndex;
	}

	bool UpdateDefaultFlashlightState(FlashlightState_t& state, const Vector& vecPos, const Vector& vecDir, const Vector& vecRight,
		const Vector& vecUp, bool castsShadows, bool bTracePlayers = true);
	bool ComputeLightPosAndOrientation(const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp,
		Vector& vecFinalPos, Quaternion& quatOrientation, bool bTracePlayers);
	void LightOff();

	void UpdateFlashlightTexture(const char* pTextureName);
	void UpdateLightTopDown(const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp);

	bool m_bIsOn;
	int m_nEntIndex;
	ClientShadowHandle_t m_FlashlightHandle;

	bool m_bMuzzleFlashEnabled;
	float m_flMuzzleFlashBrightness;

	float m_flFov;
	float m_flFarZ;
	float m_flLinearAtten;
	bool  m_bCastsShadows;

	float m_flCurrentPullBackDist;

	CTextureReference m_FlashlightTexture;

	CTextureReference m_MuzzleFlashTexture;

	char m_textureName[64];
};

class CHeadlightEffect : public CFlashlightEffect
{
public:

	CHeadlightEffect();
	~CHeadlightEffect();

	virtual void UpdateLight(const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp, int nDistance);
};
 
 

class CFlashlightEffectManager
{
private:
	CFlashlightEffect* m_pFlashlightEffect;
	const char* m_pFlashlightTextureName;
	int m_nFlashlightEntIndex;
	float m_flFov;
	float m_flFarZ;
	float m_flLinearAtten;
	int m_nMuzzleFlashFrameCountdown;
	CountdownTimer m_muzzleFlashTimer;
	float m_flMuzzleFlashBrightness;
	bool m_bFlashlightOn;
	int m_nFXComputeFrame;
	bool m_bFlashlightOverride;

public:
	CFlashlightEffectManager() : m_pFlashlightEffect(NULL), m_pFlashlightTextureName(NULL), m_nFlashlightEntIndex(-1), m_flFov(0.0f),
		m_flFarZ(0.0f), m_flLinearAtten(0.0f), m_nMuzzleFlashFrameCountdown(0), m_flMuzzleFlashBrightness(1.0f),
		m_bFlashlightOn(false), m_nFXComputeFrame(-1), m_bFlashlightOverride(false) {}

	void TurnOnFlashlight(int nEntIndex = 0, const char* pszTextureName = NULL, float flFov = 0.0f, float flFarZ = 0.0f, float flLinearAtten = 0.0f)
	{
		m_pFlashlightTextureName = pszTextureName;
		m_nFlashlightEntIndex = nEntIndex;
		m_flFov = flFov;
		m_flFarZ = flFarZ;
		m_flLinearAtten = flLinearAtten;
		m_bFlashlightOn = true;

		if (m_bFlashlightOverride)
		{
			return;
		}

		if (!m_pFlashlightEffect)
		{
			if (pszTextureName)
			{
				m_pFlashlightEffect = new CFlashlightEffect(m_nFlashlightEntIndex, pszTextureName, flFov, flFarZ, flLinearAtten);
			}
			else
			{
				m_pFlashlightEffect = new CFlashlightEffect(m_nFlashlightEntIndex);
			}

			if (!m_pFlashlightEffect)
			{
				return;
			}
		}

		m_pFlashlightEffect->TurnOn();
	}

	void TurnOffFlashlight(bool bForce = false)
	{
		m_pFlashlightTextureName = NULL;
		m_bFlashlightOn = false;

		if (bForce)
		{
			m_bFlashlightOverride = false;
			m_nMuzzleFlashFrameCountdown = 0;
			m_muzzleFlashTimer.Invalidate();
			delete m_pFlashlightEffect;
			m_pFlashlightEffect = NULL;
			return;
		}

		if (m_bFlashlightOverride)
		{
			return;
		}

		if (m_nMuzzleFlashFrameCountdown == 0 && m_muzzleFlashTimer.IsElapsed())
		{
			delete m_pFlashlightEffect;
			m_pFlashlightEffect = NULL;
		}
	}

	bool IsFlashlightOn() const { return m_bFlashlightOn; }

	void UpdateFlashlight(const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp, float flFov, bool castsShadows,
		float flFarZ, float flLinearAtten, const char* pTextureName = NULL);
	void SetEntityIndex(int index)
	{
		m_nFlashlightEntIndex = index;
	}

	void TriggerMuzzleFlash()
	{
	}

	const char* GetFlashlightTextureName(void) const
	{
		return m_pFlashlightTextureName;
	}

	int GetFlashlightEntIndex(void) const
	{
		return m_nFlashlightEntIndex;
	}

	void EnableFlashlightOverride(bool bEnable)
	{
		m_bFlashlightOverride = bEnable;

		if (!m_bFlashlightOverride)
		{
			if (m_bFlashlightOn && m_pFlashlightEffect == NULL)
			{
				TurnOnFlashlight(m_nFlashlightEntIndex, m_pFlashlightTextureName, m_flFov, m_flFarZ, m_flLinearAtten);
			}
			else if (!m_bFlashlightOn && m_pFlashlightEffect)
			{
				delete m_pFlashlightEffect;
				m_pFlashlightEffect = NULL;
			}
		}
	}

	void UpdateFlashlightOverride(bool bFlashlightOn, const Vector& vecPos, const Vector& vecDir, const Vector& vecRight, const Vector& vecUp,
		float flFov, bool castsShadows, ITexture* pFlashlightTexture, const Vector& vecBrightness)
	{
		Assert(m_bFlashlightOverride);
		if (!m_bFlashlightOverride)
		{
			return;
		}

		if (bFlashlightOn && !m_pFlashlightEffect)
		{
			m_pFlashlightEffect = new CFlashlightEffect(m_nFlashlightEntIndex);
		}
		else if (!bFlashlightOn && m_pFlashlightEffect)
		{
			delete m_pFlashlightEffect;
			m_pFlashlightEffect = NULL;
		}

		if (m_pFlashlightEffect)
		{
			m_pFlashlightEffect->UpdateLight(m_nFlashlightEntIndex, vecPos, vecDir, vecRight, vecUp, flFov, castsShadows, pFlashlightTexture, vecBrightness, false);
		}
	}
};





#endif  
