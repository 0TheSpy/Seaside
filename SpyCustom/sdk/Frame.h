#ifndef VGUI_FRAME_H
#define VGUI_FRAME_H

#ifdef _WIN32
#pragma once
#endif

#include "vgui.h"
#include "Dar.h"
#include "Panel.h"
#include "EditablePanel.h"
#include "FocusNavGroup.h"

namespace vgui
{

	class FrameButton;
	class FrameSystemButton;

	class Frame : public EditablePanel
	{
		DECLARE_CLASS_SIMPLE(Frame, EditablePanel);

	public:
		Frame(Panel* parent, const char* panelName, bool showTaskbarIcon = true, bool bPopup = true);
		virtual ~Frame();

		virtual void SetTitle(const char* title, bool surfaceTitle);
		virtual void SetTitle(const wchar_t* title, bool surfaceTitle);

		virtual void Activate();

		virtual void ActivateMinimized();

		MESSAGE_FUNC(Close, "Close");
		MESSAGE_FUNC(CloseModal, "CloseModal");

		virtual void SetDeleteSelfOnClose(bool state);

		virtual void MoveToCenterOfScreen();

		virtual void SetMoveable(bool state);
		virtual bool IsMoveable();

		virtual void SetSizeable(bool state);
		virtual bool IsSizeable();
		virtual void SetMenuButtonVisible(bool state);
		void SetMenuButtonResponsive(bool state);

		virtual void SetMinimizeButtonVisible(bool state);
		virtual void SetMaximizeButtonVisible(bool state);
		virtual void SetMinimizeToSysTrayButtonVisible(bool state);

		virtual void SetCloseButtonVisible(bool state);

		virtual bool IsMinimized();
		virtual void FlashWindow();
		virtual void FlashWindowStop();
		virtual void OnCommand(const char* command);

		virtual Menu* GetSysMenu();
		virtual void SetSysMenu(Menu* menu);

		void SetImages(const char* pEnabledImage, const char* pDisabledImage = NULL);

		virtual void SetTitleBarVisible(bool state);

		virtual void SetClipToParent(bool state);
		virtual bool GetClipToParent() const;

		virtual void SetSmallCaption(bool state);
		virtual bool IsSmallCaption() const;

		virtual int GetDraggerSize();
		virtual int GetCornerSize();
		virtual int GetBottomRightSize();
		virtual int GetCaptionHeight();

		virtual void LoadControlSettings(const char* dialogResourceName, const char* pathID = NULL, KeyValues* pPreloadedKeyValues = NULL, KeyValues* pConditions = NULL);

		void SetChainKeysToParent(bool state);
		bool CanChainKeysToParent() const;

		virtual void DoModal();

		void PlaceUnderCursor();

		void DisableFadeEffect(void);

		void SetFadeEffectDisableOverride(bool disabled);

		virtual void OnGripPanelMoved(int nNewX, int nNewY, int nNewW, int nNewH);
		virtual void OnGripPanelMoveFinished() {}

	protected:
		virtual void OnMousePressed(MouseCode code);
		virtual void OnKeyCodeTyped(KeyCode code);
		virtual void OnKeyTyped(wchar_t unichar);
		virtual void OnKeyCodeReleased(KeyCode code);
		virtual void OnKeyFocusTicked();
		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual void PerformLayout();
		virtual void OnClose();
		virtual void OnFinishedClose();
		MESSAGE_FUNC(OnMinimize, "Minimize");
		virtual void OnMinimizeToSysTray();
		MESSAGE_FUNC(OnCloseFrameButtonPressed, "CloseFrameButtonPressed");
		virtual void OnChildAdded(VPANEL child);
		virtual void ApplySettings(KeyValues* inResourceData);
		virtual void GetSettings(KeyValues* outResourceData);
		virtual const char* GetDescription(void);

		virtual bool GetDefaultScreenPosition(int& x, int& y, int& wide, int& tall);

		virtual void PaintBackground();

		virtual void OnThink();

		virtual void OnScreenSizeChanged(int iOldWide, int iOldTall);

		virtual void GetClientArea(int& x, int& y, int& wide, int& tall);

		virtual void ApplyUserConfigSettings(KeyValues* userConfig);

		virtual void GetUserConfigSettings(KeyValues* userConfig);

		virtual bool HasUserConfigSettings();

		virtual void GetSizerClientArea(int& x, int& y, int& wide, int& tall);

	private:
		MESSAGE_FUNC_CHARPTR(InternalSetTitle, "SetTitle", text);
		MESSAGE_FUNC(InternalFlashWindow, "FlashWindow");
		MESSAGE_FUNC_PARAMS(OnDialogVariablesChanged, "DialogVariables", dialogVariables);

		void SetupResizeCursors();
		void LayoutProportional(FrameButton* bt);
		void FinishClose();
		void OnFrameFocusChanged(bool bHasFocus);

		Color		_titleBarBgColor;
		Color		_titleBarDisabledBgColor;
		Color		_titleBarFgColor;
		Color		_titleBarDisabledFgColor;
		Color		m_InFocusBgColor;
		Color		m_OutOfFocusBgColor;
		TextImage* _title;

#if !defined( _GAMECONSOLE )
		Panel* _topGrip;
		Panel* _bottomGrip;
		Panel* _leftGrip;
		Panel* _rightGrip;
		Panel* _topLeftGrip;
		Panel* _topRightGrip;
		Panel* _bottomLeftGrip;
		Panel* _bottomRightGrip;
		Panel* _captionGrip;
		FrameButton* _minimizeButton;
		FrameButton* _maximizeButton;
		FrameButton* _minimizeToSysTrayButton;
		FrameButton* _closeButton;
		FrameSystemButton* _menuButton;
		Menu* _sysMenu;
#endif

		float	m_flTransitionEffectTime;
		float	 m_flFocusTransitionEffectTime;
		int		m_iClientInsetX;
		int		m_iClientInsetY;
		int		m_iTitleTextInsetX;
		int		m_nGripperWidth;
		VPANEL	m_hPreviousModal;
		HFont	m_hCustomTitleFont;

		bool	_sizeable : 1;
		bool	_moveable : 1;
		bool	 m_bHasFocus : 1;
		bool	_flashWindow : 1;
		bool	_nextFlashState : 1;
		bool	_drawTitleBar : 1;
		bool	m_bPreviouslyVisible : 1;
		bool	m_bFadingOut : 1;
		bool	m_bDeleteSelfOnClose : 1;
		bool	m_bDisableFadeEffect : 1;
		bool	m_bClipToParent : 1;
		bool	m_bSmallCaption : 1;
		bool	m_bChainKeysToParent : 1;
		bool	m_bPrimed : 1;
		bool	m_iClientInsetXOverridden : 1;

		CPanelAnimationVarAliasType(int, m_iTitleTextInsetXOverride, "titletextinsetX", "0", "proportional_int");
		CPanelAnimationVarAliasType(int, m_iTitleTextInsetYOverride, "titletextinsetY", "0", "proportional_int");
	};

}   

#endif  