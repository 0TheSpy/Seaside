#ifndef BUTTON_H
#define BUTTON_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "Dar.h"
#include "Color.h"
#include "Label.h"
#include "MouseCode.h"

namespace vgui
{

	class Button : public Label
	{
		DECLARE_CLASS_SIMPLE(Button, Label);

	public:
		Button(Panel* parent, const char* panelName, const char* text, Panel* pActionSignalTarget = NULL, const char* pCmd = NULL);
		Button(Panel* parent, const char* panelName, const wchar_t* text, Panel* pActionSignalTarget = NULL, const char* pCmd = NULL);
		~Button();
	private:
		void Init();
	public:
		virtual void SetArmed(bool state);
		virtual bool IsArmed(void);

		virtual bool IsDepressed();
		virtual void ForceDepressed(bool state);
		virtual void RecalculateDepressedState(void);

		virtual void SetSelected(bool state);
		virtual bool IsSelected(void);

		virtual void SetBlink(bool state);
		virtual bool IsBlinking(void);

		virtual void SetUseCaptureMouse(bool state);
		virtual bool IsUseCaptureMouseEnabled(void);

		MESSAGE_FUNC(DoClick, "PressButton");
		MESSAGE_FUNC(OnHotkey, "Hotkey")
		{
			DoClick();
		}

		virtual void SetMouseClickEnabled(MouseCode code, bool state);
		virtual bool IsMouseClickEnabled(MouseCode code);
		enum ActivationType_t
		{
			ACTIVATE_ONPRESSEDANDRELEASED,	   
			ACTIVATE_ONPRESSED,				    
			ACTIVATE_ONRELEASED,			  
		};
		virtual void SetButtonActivationType(ActivationType_t activationType);

		virtual void FireActionSignal(void);
		virtual void PerformLayout();

		virtual bool RequestInfo(KeyValues* data);

		virtual bool CanBeDefaultButton(void);

		MESSAGE_FUNC_INT(SetAsDefaultButton, "SetAsDefaultButton", state);
		MESSAGE_FUNC_INT(SetAsCurrentDefaultButton, "SetAsCurrentDefaultButton", state);

		virtual void OnSetFocus();
		virtual void OnKillFocus();

		virtual void SetButtonBorderEnabled(bool state);

		virtual void SetDefaultColor(Color fgColor, Color bgColor);
		virtual void SetArmedColor(Color fgColor, Color bgColor);
		virtual void SetSelectedColor(Color fgColor, Color bgColor);
		virtual void SetDepressedColor(Color fgColor, Color bgColor);
		virtual void SetBlinkColor(Color fgColor);

		virtual Color GetButtonFgColor();
		virtual Color GetButtonBgColor();

		Color		  GetButtonDefaultFgColor() { return _defaultFgColor; }
		Color		  GetButtonDefaultBgColor() { return _defaultBgColor; }

		Color		  GetButtonArmedFgColor() { return _armedFgColor; }
		Color		  GetButtonArmedBgColor() { return _armedBgColor; }

		Color		  GetButtonSelectedFgColor() { return _selectedFgColor; }
		Color		  GetButtonSelectedBgColor() { return _selectedBgColor; }

		Color		  GetButtonDepressedFgColor() { return _depressedFgColor; }
		Color		  GetButtonDepressedBgColor() { return _depressedBgColor; }

		virtual void SetDefaultBorder(IBorder* border);
		virtual void SetDepressedBorder(IBorder* border);
		virtual void SetKeyFocusBorder(IBorder* border);

		virtual void SetCommand(const char* command);
		virtual void SetCommand(KeyValues* message);

		void SetArmedSound(const char* sound);
		void SetDepressedSound(const char* sound);
		void SetReleasedSound(const char* sound);

		virtual void OnCursorEntered();
		virtual void OnCursorExited();
		virtual void SizeToContents();

		virtual KeyValues* GetCommand();

		bool IsDrawingFocusBox();
		void DrawFocusBox(bool bEnable);

		bool ShouldPaint() { return _paint; }
		void SetShouldPaint(bool paint) { _paint = paint; }

		virtual void ApplySettings(KeyValues* inResourceData);
		virtual void NavigateTo();
		virtual void NavigateFrom();

	protected:
		virtual void DrawFocusBorder(int tx0, int ty0, int tx1, int ty1);

		virtual void Paint(void);
		virtual IBorder* GetBorder(bool depressed, bool armed, bool selected, bool keyfocus);

		virtual void ApplySchemeSettings(IScheme* pScheme);
		MESSAGE_FUNC_INT(OnSetState, "SetState", state);

		virtual void OnMousePressed(MouseCode code);
		virtual void OnMouseDoublePressed(MouseCode code);
		virtual void OnMouseReleased(MouseCode code);
		virtual void OnKeyCodePressed(KeyCode code);
		virtual void OnKeyCodeReleased(KeyCode code);

		virtual void GetSettings(KeyValues* outResourceData);
		virtual const char* GetDescription(void);

		KeyValues* GetActionMessage();
		void PlayButtonReleasedSound();

	protected:
		enum ButtonFlags_t
		{
			ARMED = 0x0001,
			DEPRESSED = 0x0002,
			FORCE_DEPRESSED = 0x0004,
			BUTTON_BORDER_ENABLED = 0x0008,
			USE_CAPTURE_MOUSE = 0x0010,
			BUTTON_KEY_DOWN = 0x0020,
			DEFAULT_BUTTON = 0x0040,
			SELECTED = 0x0080,
			DRAW_FOCUS_BOX = 0x0100,
			BLINK = 0x0200,
			ALL_FLAGS = 0xFFFF,
		};

		CUtlFlags< unsigned short > _buttonFlags;	  
		int                _mouseClickMask;
		KeyValues* _actionMessage;
		ActivationType_t   _activationType;

		IBorder* _defaultBorder;
		IBorder* _depressedBorder;
		IBorder* _keyFocusBorder;

		Color			   _defaultFgColor, _defaultBgColor;
		Color			   _armedFgColor, _armedBgColor;
		Color			   _selectedFgColor, _selectedBgColor;
		Color              _depressedFgColor, _depressedBgColor;
		Color              _keyboardFocusColor;
		Color			   _blinkFgColor;

		bool				_paint;

		unsigned short	   m_sArmedSoundName, m_sDepressedSoundName, m_sReleasedSoundName;
		bool m_bSelectionStateSaved;
		bool m_bStaySelectedOnClick;
	};

}   

#endif  