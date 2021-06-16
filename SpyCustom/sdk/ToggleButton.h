#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "Button.h"

namespace vgui
{

	class ToggleButton : public Button
	{
		DECLARE_CLASS_SIMPLE(ToggleButton, Button);

	public:
		ToggleButton(Panel* parent, const char* panelName, const char* text);

		virtual void DoClick();

	protected:
		virtual void OnMouseDoublePressed(MouseCode code);

		virtual Color GetButtonFgColor();
		virtual void ApplySchemeSettings(IScheme* pScheme);

		virtual bool CanBeDefaultButton(void);
		virtual void OnKeyCodePressed(KeyCode code);

	private:
		Color _selectedColor;
	};

}   

#endif  