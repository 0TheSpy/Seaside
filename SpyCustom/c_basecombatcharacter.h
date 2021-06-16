#ifndef C_BASECOMBATCHARACTER_H
#define C_BASECOMBATCHARACTER_H

#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"
#include "c_baseflex.h"
#ifdef GLOWS_ENABLE
#include "glow_outline_effect.h"
#endif  

class C_BaseCombatWeapon;
class C_WeaponCombatShield;

#define BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE 0.9f

class C_BaseCombatCharacter : public C_BaseFlex
{
	DECLARE_CLASS(C_BaseCombatCharacter, C_BaseFlex);
public:
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	C_BaseCombatCharacter(void);
	virtual			~C_BaseCombatCharacter(void);

	virtual void	OnPreDataChanged(DataUpdateType_t updateType);
	virtual void	OnDataChanged(DataUpdateType_t updateType);

	virtual bool	IsBaseCombatCharacter(void) { return true; };
	virtual C_BaseCombatCharacter* MyCombatCharacterPointer(void) { return this; }

	enum FieldOfViewCheckType { USE_FOV, DISREGARD_FOV };
	bool IsAbleToSee(const CBaseEntity* entity, FieldOfViewCheckType checkFOV);	                 
	bool IsAbleToSee(C_BaseCombatCharacter* pBCC, FieldOfViewCheckType checkFOV);	                 

	virtual bool IsLookingTowards(const CBaseEntity* target, float cosTolerance = BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE) const;	                         
	virtual bool IsLookingTowards(const Vector& target, float cosTolerance = BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE) const;	                         

	virtual bool IsInFieldOfView(CBaseEntity* entity) const;	          
	virtual bool IsInFieldOfView(const Vector& pos) const;

	enum LineOfSightCheckType
	{
		IGNORE_NOTHING,
		IGNORE_ACTORS
	};
	virtual bool IsLineOfSightClear(CBaseEntity* entity, LineOfSightCheckType checkType = IGNORE_NOTHING) const;       
	virtual bool IsLineOfSightClear(const Vector& pos, LineOfSightCheckType checkType = IGNORE_NOTHING, CBaseEntity* entityToIgnore = NULL) const;


	void				RemoveAmmo(int iCount, int iAmmoIndex);
	void				RemoveAmmo(int iCount, const char* szName);
	void				RemoveAllAmmo();
	int					GetAmmoCount(int iAmmoIndex) const;
	int					GetAmmoCount(char* szName) const;

	C_BaseCombatWeapon* Weapon_OwnsThisType(const char* pszWeapon, int iSubType = 0) const;           
	virtual	bool		Weapon_Switch(C_BaseCombatWeapon* pWeapon, int viewmodelindex = 0);
	virtual bool		Weapon_CanSwitchTo(C_BaseCombatWeapon* pWeapon);

	bool SwitchToNextBestWeapon(C_BaseCombatWeapon* pCurrent);

	virtual C_BaseCombatWeapon* GetActiveWeapon(void) const;
	int					WeaponCount() const;
	C_BaseCombatWeapon* GetWeapon(int i) const;

	void SetAmmoCount(int iCount, int iAmmoIndex);

	float				GetNextAttack() const { return m_flNextAttack; }
	void				SetNextAttack(float flWait) { m_flNextAttack = flWait; }

	virtual int			BloodColor();

	void SetBloodColor(int nBloodColor);

	virtual void		DoMuzzleFlash();

#ifdef GLOWS_ENABLE
	CGlowObject* GetGlowObject(void) { return m_pGlowEffect; }
	virtual void		GetGlowEffectColor(float* r, float* g, float* b);
#endif  

public:

	float			m_flNextAttack;

protected:

#ifdef GLOWS_ENABLE	
	virtual void		UpdateGlowEffect(void);
	virtual void		DestroyGlowEffect(void);
#endif  

	int			m_bloodColor;			    

private:
	bool				ComputeLOS(const Vector& vecEyePosition, const Vector& vecTarget) const;

	CNetworkArray(int, m_iAmmo, MAX_AMMO_TYPES);

	CHandle<C_BaseCombatWeapon>		m_hMyWeapons[MAX_WEAPONS];
	CHandle< C_BaseCombatWeapon > m_hActiveWeapon;

#ifdef GLOWS_ENABLE
	bool				m_bGlowEnabled;
	bool				m_bOldGlowEnabled;
	CGlowObject* m_pGlowEffect;
#endif  

private:
	C_BaseCombatCharacter(const C_BaseCombatCharacter&);     


#ifdef INVASION_CLIENT_DLL
public:
	virtual void	Release(void);
	virtual void	SetDormant(bool bDormant);
	virtual void	OnPreDataChanged(DataUpdateType_t updateType);
	virtual void	OnDataChanged(DataUpdateType_t updateType);
	virtual void	ClientThink(void);

	virtual bool	CanBePoweredUp(void) { return true; }
	bool			HasPowerup(int iPowerup) { return (m_iPowerups & (1 << iPowerup)) != 0; };
	virtual void	PowerupStart(int iPowerup, bool bInitial);
	virtual void	PowerupEnd(int iPowerup);
	void			RemoveAllPowerups(void);

	void			AddEMPEffect(float flSize);
	void			AddBuffEffect(float flSize);

	C_WeaponCombatShield* GetShield(void);

public:
	int				m_iPowerups;
	int				m_iPrevPowerups;
#endif

};

inline C_BaseCombatCharacter* ToBaseCombatCharacter(C_BaseEntity* pEntity)
{
	if (!pEntity || !pEntity->IsBaseCombatCharacter())
		return NULL;

#if _DEBUG
	return dynamic_cast<C_BaseCombatCharacter*>(pEntity);
#else
	return static_cast<C_BaseCombatCharacter*>(pEntity);
#endif
}

inline int	C_BaseCombatCharacter::WeaponCount() const
{
	return MAX_WEAPONS;
}

inline C_BaseCombatWeapon* C_BaseCombatCharacter::GetWeapon(int i) const
{
	Assert((i >= 0) && (i < MAX_WEAPONS));
	return m_hMyWeapons[i].Get();
}

EXTERN_RECV_TABLE(DT_BaseCombatCharacter);

#endif  