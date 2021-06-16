#ifndef HUD_H
#define HUD_H
#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "utldict.h"
#include "convar.h"
#include "VGUI.h"
#include "Color.h"
#include "bitbuf.h"

namespace vgui
{
	class IScheme;
}

typedef struct wrect_s
{
	int	left;
	int right;
	int top;
	int bottom;
} wrect_t;

class CHudTexture
{
public:
	CHudTexture();
	CHudTexture& operator =(const CHudTexture& src);
	virtual ~CHudTexture();

	int Width() const
	{
		return rc.right - rc.left;
	}

	int Height() const
	{
		return rc.bottom - rc.top;
	}

	void Precache(void);

	int EffectiveWidth(float flScale) const;
	int EffectiveHeight(float flScale) const;

	void DrawSelf(int x, int y, const Color& clr) const;
	void DrawSelf(int x, int y, int w, int h, const Color& clr) const;
	void DrawSelfCropped(int x, int y, int cropx, int cropy, int cropw, int croph, Color clr) const;
	void DrawSelfCropped(int x, int y, int cropx, int cropy, int cropw, int croph, int finalWidth, int finalHeight, Color clr) const;

	char		szShortName[64];
	char		szTextureFile[64];

	bool		bRenderUsingFont;
	bool		bPrecached;
	char		cCharacterInFont;
	vgui::HFont hFont;

	int			textureId;
	float		texCoords[4];

	wrect_t		rc;
};

#include "hudtexturehandle.h"

class CHudElement;
class CHudRenderGroup;

class CHud
{
public:
	static const int			HUDPB_HORIZONTAL;
	static const int			HUDPB_VERTICAL;
	static const int			HUDPB_HORIZONTAL_INV;

public:
	CHud();
	~CHud();

	void						Init(void);
	void						VidInit(void);
	void						Shutdown(void);
	void						LevelInit(void);
	void						LevelShutdown(void);

	void						ResetHUD(void);

	void						OnRestore();

	void						Think();

	void						ProcessInput(bool bActive);
	void						UpdateHud(bool bActive);

	void						InitColors(vgui::IScheme* pScheme);

	void						AddHudElement(CHudElement* pHudElement);
	void						RemoveHudElement(CHudElement* pHudElement);
	CHudElement* FindElement(const char* pName);

	bool						IsHidden(int iHudFlags);

	float						GetSensitivity();
	float						GetFOVSensitivityAdjust();

	void						DrawProgressBar(int x, int y, int width, int height, float percentage, Color& clr, unsigned char type);
	void						DrawIconProgressBar(int x, int y, CHudTexture* icon, CHudTexture* icon2, float percentage, Color& clr, int type);

	CHudTexture* GetIcon(const char* szIcon);

	CHudTexture* AddUnsearchableHudIconToList(CHudTexture& texture);
	CHudTexture* AddSearchableHudIconToList(CHudTexture& texture);

	void						RefreshHudTextures();

	void						MsgFunc_ResetHUD(bf_read& msg);
	void 						MsgFunc_SendAudio(bf_read& msg);

	int							LookupRenderGroupIndexByName(const char* pszGroupName);
	bool						LockRenderGroup(int iGroupIndex, CHudElement* pLocker = NULL);
	bool						UnlockRenderGroup(int iGroupIndex, CHudElement* pLocker = NULL);
	bool						IsRenderGroupLockedFor(CHudElement* pHudElement, int iGroupIndex);
	int							RegisterForRenderGroup(const char* pszGroupName);
	int							AddHudRenderGroup(const char* pszGroupName);
	bool						DoesRenderGroupExist(int iGroupIndex);

	void						SetScreenShotTime(float flTime) { m_flScreenShotTime = flTime; }

public:

	int							m_iKeyBits;
#ifndef _XBOX
	float						m_flMouseSensitivity;
	float						m_flMouseSensitivityFactor;
#endif
	float						m_flFOVSensitivityAdjust;

	Color						m_clrNormal;
	Color						m_clrCaution;
	Color						m_clrYellowish;

	CUtlVector< CHudElement* >	m_HudList;

private:
	void						InitFonts();

	void						SetupNewHudTexture(CHudTexture* t);

	bool						m_bHudTexturesLoaded;

	CUtlDict< CHudTexture*, int >		m_Icons;

	CUtlVector< const char* >				m_RenderGroupNames;
	CUtlMap< int, CHudRenderGroup* >		m_RenderGroups;

	float						m_flScreenShotTime;      
};

extern CHud gHUD;

extern vgui::HFont g_hFontTrebuchet24;

void LoadHudTextures(CUtlDict< CHudTexture*, int >& list, const char* szFilenameWithoutExtension, const unsigned char* pICEKey);

void GetHudSize(int& w, int& h);

#endif  