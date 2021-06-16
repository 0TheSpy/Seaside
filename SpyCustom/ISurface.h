#ifndef ISURFACE_H
#define ISURFACE_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "IHTML.h"    
#include "interface.h"
#include "imageformat.h"

#include "IAppSystem.h"
#include "vector2d.h"        
#include "ichromehtmlwrapper.h"

#include "IVguiMatInfo.h"

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef PlaySound
#undef PlaySound
#endif

class Color;
class ITexture;

namespace vgui
{

	class IImage;
	class Image;
	class Point;

	typedef unsigned long HCursor;
	typedef unsigned long HTexture;
	typedef unsigned long HFont;



	struct Vertex_t
	{
		Vertex_t() {}
		Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}
		void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}

		Vector2D	m_Position;
		Vector2D	m_TexCoord;
	};


	enum FontDrawType_t
	{
		FONT_DRAW_DEFAULT = 0,

		FONT_DRAW_NONADDITIVE,
		FONT_DRAW_ADDITIVE,

		FONT_DRAW_TYPE_COUNT = 2,
	};


	struct CharRenderInfo
	{
		int				x, y;
		Vertex_t* verts;
		int				textureId;
		int				abcA;
		int				abcB;
		int				abcC;
		int				fontTall;
		HFont			currentFont;
		FontDrawType_t	drawType;
		wchar_t			ch;

		bool			valid;
		bool			shouldclip;
	};


	struct IntRect
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};



	struct DrawTexturedRectParms_t
	{
		DrawTexturedRectParms_t()
		{
			s0 = t0 = 0;
			s1 = t1 = 1.0f;
			alpha_ul = alpha_ur = alpha_lr = alpha_ll = 255;
			angle = 0;
		}

		int x0;
		int	y0;
		int x1;
		int y1;

		float s0;
		float t0;
		float s1;
		float t1;

		unsigned char alpha_ul;
		unsigned char alpha_ur;
		unsigned char alpha_lr;
		unsigned char alpha_ll;

		float angle;
	};


	class ISurface : public IAppSystem
	{
	public:
		virtual void Shutdown() = 0;

		virtual void RunFrame() = 0;

		virtual VPANEL GetEmbeddedPanel() = 0;
		virtual void SetEmbeddedPanel(VPANEL pPanel) = 0;

		virtual void PushMakeCurrent(VPANEL panel, bool useInsets) = 0;
		virtual void PopMakeCurrent(VPANEL panel) = 0;

		virtual void DrawSetColor(int r, int g, int b, int a) = 0;
		virtual void DrawSetColor(Color col) = 0;

		virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
		virtual void DrawFilledRectArray(IntRect* pRects, int numRects) = 0;
		virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;

		virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
		virtual void DrawPolyLine(int* px, int* py, int numPoints) = 0;

		virtual void DrawSetApparentDepth(float depth) = 0; 
		virtual void DrawClearApparentDepth() = 0; 

		virtual void DrawSetTextFont(HFont font) = 0;
		virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
		virtual void DrawSetTextColor(Color col) = 0;
		virtual void DrawSetTextPos(int x, int y) = 0;
		virtual void DrawGetTextPos(int& x, int& y) = 0;
		virtual void DrawPrintText(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
		virtual void DrawUnicodeChar(wchar_t wch, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;

		virtual void DrawFlushText() = 0;		       
		virtual IHTML* CreateHTMLWindow(vgui::IHTMLEvents* events, VPANEL context) = 0;
		virtual void PaintHTMLWindow(vgui::IHTML* htmlwin) = 0;
		virtual void DeleteHTMLWindow(IHTML* htmlwin) = 0;

		enum ETextureFormat
		{
			eTextureFormat_RGBA,
			eTextureFormat_BGRA,
			eTextureFormat_BGRA_Opaque,                      
		};

		virtual int	 DrawGetTextureId(char const* filename) = 0;
		virtual bool DrawGetTextureFile(int id, char* filename, int maxlen) = 0;
		virtual void DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;
		virtual void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
		virtual void DrawSetTexture(int id) = 0;
		virtual bool DeleteTextureByID(int id) = 0;

		virtual void DrawGetTextureSize(int id, int& wide, int& tall) = 0; 
		virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0; 
		virtual bool IsTextureIDValid(int id) = 0; 

		virtual int CreateNewTextureID(bool procedural = false) = 0;

		virtual void GetScreenSize(int& wide, int& tall) = 0;
		virtual void SetAsTopMost(VPANEL panel, bool state) = 0;
		virtual void BringToFront(VPANEL panel) = 0;
		virtual void SetForegroundWindow(VPANEL panel) = 0;
		virtual void SetPanelVisible(VPANEL panel, bool state) = 0;
		virtual void SetMinimized(VPANEL panel, bool state) = 0;
		virtual bool IsMinimized(VPANEL panel) = 0;
		virtual void FlashWindow(VPANEL panel, bool state) = 0;
		virtual void SetTitle(VPANEL panel, const wchar_t* title) = 0;
		virtual void SetAsToolBar(VPANEL panel, bool state) = 0;		          

		virtual void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
		virtual void SwapBuffers(VPANEL panel) = 0;
		virtual void Invalidate(VPANEL panel) = 0;
		virtual void SetCursor(HCursor cursor) = 0;
		virtual bool IsCursorVisible() = 0;
		virtual void ApplyChanges() = 0;
		virtual bool IsWithin(int x, int y) = 0;
		virtual bool HasFocus() = 0;

		enum SurfaceFeature_e
		{
			ANTIALIASED_FONTS = 1,
			DROPSHADOW_FONTS = 2,
			ESCAPE_KEY = 3,
			OPENING_NEW_HTML_WINDOWS = 4,
			FRAME_MINIMIZE_MAXIMIZE = 5,
			OUTLINE_FONTS = 6,
			DIRECT_HWND_RENDER = 7,
		};
		virtual bool SupportsFeature(SurfaceFeature_e feature) = 0;

		virtual void RestrictPaintToSinglePanel(VPANEL panel) = 0;

		virtual void SetModalPanel(VPANEL) = 0;
		virtual VPANEL GetModalPanel() = 0;

		virtual void UnlockCursor() = 0;
		virtual void LockCursor() = 0;
		virtual void SetTranslateExtendedKeys(bool state) = 0;
		virtual VPANEL GetTopmostPopup() = 0;

		virtual void SetTopLevelFocus(VPANEL panel) = 0;

		virtual HFont CreateFont() = 0;

		enum EFontFlags
		{
			FONTFLAG_NONE,
			FONTFLAG_ITALIC = 0x001,
			FONTFLAG_UNDERLINE = 0x002,
			FONTFLAG_STRIKEOUT = 0x004,
			FONTFLAG_SYMBOL = 0x008,
			FONTFLAG_ANTIALIAS = 0x010,
			FONTFLAG_GAUSSIANBLUR = 0x020,
			FONTFLAG_ROTARY = 0x040,
			FONTFLAG_DROPSHADOW = 0x080,
			FONTFLAG_ADDITIVE = 0x100,
			FONTFLAG_OUTLINE = 0x200,
			FONTFLAG_CUSTOM = 0x400,		           
			FONTFLAG_BITMAP = 0x800,		      
		};

		virtual bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) = 0;

		virtual bool AddCustomFontFile(const char* fontName, const char* fontFileName) = 0;

		virtual int GetFontTall(HFont font) = 0;
		virtual int GetFontAscent(HFont font, wchar_t wch) = 0;
		virtual bool IsFontAdditive(HFont font) = 0;
		virtual void GetCharABCwide(HFont font, int ch, int& a, int& b, int& c) = 0;
		virtual int GetCharacterWidth(HFont font, int ch) = 0;
		virtual void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall) = 0;

		virtual VPANEL GetNotifyPanel() = 0;
		virtual void SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char* text) = 0;

		virtual void PlaySound(const char* fileName) = 0;

		virtual int GetPopupCount() = 0;
		virtual VPANEL GetPopup(int index) = 0;
		virtual bool ShouldPaintChildPanel(VPANEL childPanel) = 0;
		virtual bool RecreateContext(VPANEL panel) = 0;
		virtual void AddPanel(VPANEL panel) = 0;
		virtual void ReleasePanel(VPANEL panel) = 0;
		virtual void MovePopupToFront(VPANEL panel) = 0;
		virtual void MovePopupToBack(VPANEL panel) = 0;

		virtual void SolveTraverse(VPANEL panel, bool forceApplySchemeSettings = false) = 0;
		virtual void PaintTraverse(VPANEL panel) = 0;

		virtual void EnableMouseCapture(VPANEL panel, bool state) = 0;

		virtual void GetWorkspaceBounds(int& x, int& y, int& wide, int& tall) = 0;

		virtual void GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall) = 0;

		virtual void GetProportionalBase(int& width, int& height) = 0;

		virtual void CalculateMouseVisible() = 0;
		virtual bool NeedKBInput() = 0;

		virtual bool HasCursorPosFunctions() = 0;
		virtual void SurfaceGetCursorPos(int& x, int& y) = 0;
		virtual void SurfaceSetCursorPos(int x, int y) = 0;

		virtual void DrawTexturedLine(const Vertex_t& a, const Vertex_t& b) = 0;
		virtual void DrawOutlinedCircle(int x, int y, int radius, int segments) = 0;
		virtual void DrawTexturedPolyLine(const Vertex_t* p, int n) = 0;         
		virtual void DrawTexturedSubRect(int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1) = 0;
		virtual void DrawTexturedPolygon(int n, Vertex_t* pVertice, bool bClipVertices = true) = 0;
		virtual const wchar_t* GetTitle(VPANEL panel) = 0;
		virtual bool IsCursorLocked(void) const = 0;
		virtual void SetWorkspaceInsets(int left, int top, int right, int bottom) = 0;

		virtual void DrawWordBubble(int x0, int y0, int x1, int y1, int nBorderThickness, Color rgbaBackground, Color rgbaBorder,
			bool bPointer = false, int nPointerX = 0, int nPointerY = 0, int nPointerBaseThickness = 16) = 0; 


		virtual bool DrawGetUnicodeCharRenderInfo(wchar_t ch, CharRenderInfo& info) = 0;
		virtual void DrawRenderCharFromInfo(const CharRenderInfo& info) = 0;

		virtual void DrawSetAlphaMultiplier(float alpha   ) = 0;
		virtual float DrawGetAlphaMultiplier() = 0;

		virtual void SetAllowHTMLJavaScript(bool state) = 0;

		virtual void OnScreenSizeChanged(int nOldWidth, int nOldHeight) = 0;

		virtual vgui::HCursor CreateCursorFromFile(char const* curOrAniFile, char const* pPathID = 0) = 0;

		virtual IVguiMatInfo* DrawGetTextureMatInfoFactory(int id) = 0;

		virtual void PaintTraverseEx(VPANEL panel, bool paintPopups = false) = 0;

		virtual float GetZPos() const = 0;

		virtual void SetPanelForInput(VPANEL vpanel) = 0;
		virtual void DrawFilledRectFastFade(int x0, int y0, int x1, int y1, int fadeStartPt, int fadeEndPt, unsigned int alpha0, unsigned int alpha1, bool bHorizontal) = 0;
		virtual void DrawFilledRectFade(int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool bHorizontal) = 0;
		virtual void DrawSetTextureRGBAEx(int id, const unsigned char* rgba, int wide, int tall, ImageFormat imageFormat) = 0;
		virtual void DrawSetTextScale(float sx, float sy) = 0;
		virtual bool SetBitmapFontGlyphSet(HFont font, const char* windowsFontName, float scalex, float scaley, int flags) = 0;
		virtual bool AddBitmapFontFile(const char* fontFileName) = 0;
		virtual void SetBitmapFontName(const char* pName, const char* pFontFilename) = 0;
		virtual const char* GetBitmapFontName(const char* pName) = 0;
		virtual void ClearTemporaryFontCache(void) = 0;

		virtual IImage* GetIconImageForFullPath(char const* pFullPath) = 0;
		virtual void DrawUnicodeString(const wchar_t* pwString, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
		virtual void PrecacheFontCharacters(HFont font, const wchar_t* pCharacters) = 0;
		virtual const char* GetFontName(HFont font) = 0;
		virtual bool ForceScreenSizeOverride(bool bState, int wide, int tall) = 0;
		virtual bool ForceScreenPosOffset(bool bState, int x, int y) = 0;
		virtual void OffsetAbsPos(int& x, int& y) = 0;

		virtual void SetAbsPosForContext(int id, int x, int y) = 0; 
		virtual void GetAbsPosForContext(int id, int& x, int& y) = 0; 

		virtual void ResetFontCaches() = 0;

		virtual bool IsScreenSizeOverrideActive(void) = 0;
		virtual bool IsScreenPosOverrideActive(void) = 0;

		virtual void DestroyTextureID(int id) = 0;

		virtual int GetTextureNumFrames(int id) = 0;
		virtual void DrawSetTextureFrame(int id, int nFrame, unsigned int* pFrameCache) = 0;

		virtual void GetClipRect(int& x0, int& y0, int& x1, int& y1) = 0;
		virtual void SetClipRect(int x0, int y0, int x1, int y1) = 0;

		virtual void DrawTexturedRectEx(DrawTexturedRectParms_t* pDrawParms) = 0;

		virtual void GetKernedCharWidth(HFont font, wchar_t ch, wchar_t chBefore, wchar_t chAfter, float& wide, float& abcA, float& abcC) = 0;
		virtual void DrawUpdateRegionTextureRGBA(int nTextureID, int x, int y, const unsigned char* pchData, int wide, int tall, ImageFormat imageFormat) = 0;
		virtual bool BHTMLWindowNeedsPaint(IHTML* htmlwin) = 0;

		virtual void DrawSetTextureRGBALinear(int id, const unsigned char* rgba, int wide, int tall) = 0; 

		virtual const char* GetWebkitHTMLUserAgentString() = 0;

		virtual void* Deprecated_AccessChromeHTMLController() = 0;
	};

}

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface030"

#endif  