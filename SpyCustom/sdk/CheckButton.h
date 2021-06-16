#ifndef CHECKBUTTON_H
#define CHECKBUTTON_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "ToggleButton.h"
#include "TextImage.h"

namespace vgui
{

	class TextImage;

	class CheckImage : public TextImage
	{
	public:
		CheckImage(CheckButton* CheckButton) : TextImage("g")
		{
			_CheckButton = CheckButton;

			SetSize(20, 13);
		}

		virtual void Paint();

		virtual void SetColor(Color color)
		{
			_borderColor1 = color;
			_borderColor2 = color;
			_checkColor = color;
		}

		Color _borderColor1;
		Color _borderColor2;
		Color _checkColor;

		Color _bgColor;

	private:
		CheckButton* _CheckButton;
	};

	class CheckButton : public ToggleButton
	{
		DECLARE_CLASS_SIMPLE(CheckButton, ToggleButton);

	public:
		CheckButton(Panel* parent, const char* panelName, const char* text);
		~CheckButton();

		virtual void SetSelected(bool state);

		virtual void SetCheckButtonCheckable(bool state);
		virtual bool IsCheckButtonCheckable() const { return m_bCheckButtonCheckable; }

		Color GetDisabledFgColor() { return _disabledFgColor; }
		Color GetDisabledBgColor() { return _disabledBgColor; }

		CheckImage* GetCheckImage() { return _checkBoxImage; }

		virtual void SetHighlightColor(Color fgColor);

	protected:
		virtual void ApplySchemeSettings(IScheme* pScheme);
		MESSAGE_FUNC_PTR(OnCheckButtonChecked, "CheckButtonChecked", panel);
		virtual Color GetButtonFgColor();

		virtual IBorder* GetBorder(bool depressed, bool armed, bool selected, bool keyfocus);


	private:
		enum { CHECK_INSET = 6 };
		bool m_bCheckButtonCheckable;
		CheckImage* _checkBoxImage;
		Color _disabledFgColor;
		Color _disabledBgColor;
		Color _highlightFgColor;
	};

}   

#endif  