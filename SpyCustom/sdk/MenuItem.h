#ifndef MENUITEM_H
#define MENUITEM_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "Button.h"
#include "Menu.h"

namespace vgui
{

	class IBorder;
	class TextImage;
	class Menu;
	class Image;

	class MenuItem : public Button
	{
		DECLARE_CLASS_SIMPLE(MenuItem, Button);

	public:
		MenuItem(Menu* parent, const char* panelName, const char* text, Menu* cascadeMenu = NULL, bool checkable = false);
		MenuItem(Menu* parent, const char* panelName, const wchar_t* wszText, Menu* cascadeMenu = NULL, bool checkable = false);
		~MenuItem();

		virtual void Paint();

		virtual void FireActionSignal();

		virtual bool CanBeDefaultButton(void);

		void OnCursorEntered();
		void OnCursorExited();

		void CloseCascadeMenu();

		MESSAGE_FUNC(OnKillFocus, "MenuClose");

		bool HasMenu();

		void SetTextImageSize(int wide, int tall);

		void GetTextImageSize(int& wide, int& tall);

		void GetArrowImageSize(int& wide, int& tall);

		void GetCheckImageSize(int& wide, int& tall);

		Menu* GetMenu();

		virtual void PerformLayout();

		void OnCursorMoved(int x, int y);

		MESSAGE_FUNC(ArmItem, "ArmItem");
		MESSAGE_FUNC(DisarmItem, "DisarmItem");

		bool IsItemArmed();

		void OpenCascadeMenu();

		bool IsCheckable();
		bool IsChecked();

		void SetChecked(bool state);

		KeyValues* GetUserData();
		void SetUserData(const KeyValues* kv);

		int GetActiveItem() { if (m_pCascadeMenu) { return m_pCascadeMenu->GetActiveItem(); } else { return 0; } }

		Menu* GetParentMenu();

		void SetCurrentKeyBinding(char const* keyName);

		virtual void GetContentSize(int& cw, int& ch);

	protected:
		void OnKeyCodeReleased(KeyCode code);
		void OnMenuClose();
		MESSAGE_FUNC(OnKeyModeSet, "KeyModeSet");

		virtual void Init(void);
		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual IBorder* GetBorder(bool depressed, bool armed, bool selected, bool keyfocus);

	private:
		enum { CHECK_INSET = 6 };
		Menu* m_pCascadeMenu;           
		bool m_bCheckable;                      
		bool m_bChecked;             
		TextImage* m_pCascadeArrow;              
		Image* m_pCheck;             
		TextImage* m_pBlankCheck;                

		TextImage* m_pCurrentKeyBinding;             

		KeyValues* m_pUserData;

	};

}   

#endif  