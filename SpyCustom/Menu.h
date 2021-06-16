#ifndef MENU_H
#define MENU_H

#ifdef _WIN32
#pragma once
#endif

#include "Panel.h"
#include "Label.h"
#include "utllinkedlist.h"
#include "utlvector.h"

namespace vgui
{

	class MenuItem;
	class ScrollBar;
	class MenuSeparator;

	class Menu : public Panel
	{
		DECLARE_CLASS_SIMPLE(Menu, Panel);
		friend class MenuItem;
	public:
		enum MenuDirection_e
		{
			LEFT,
			RIGHT,
			UP,
			DOWN,
			CURSOR,	        
			ALIGN_WITH_PARENT,        
		};

		Menu(Panel* parent, const char* panelName);
		~Menu();

		static void PlaceContextMenu(Panel* parent, Menu* menu);
		static void OnInternalMousePressed(Panel* other, MouseCode code);

		virtual void PositionRelativeToPanel(Panel* reference, MenuDirection_e direction, int nAdditionalYOffset = 0, bool showMenu = false);

		virtual int AddMenuItem(const char* itemName, const char* itemText, const char* command, Panel* target, const KeyValues* userData = NULL);
		virtual int AddMenuItem(const char* itemName, const wchar_t* wszItemText, const char* command, Panel* target, const KeyValues* userData = NULL);

		virtual int AddMenuItem(const char* itemName, const char* itemText, KeyValues* message, Panel* target, const KeyValues* userData = NULL);
		virtual int AddMenuItem(const char* itemName, const wchar_t* wszItemText, KeyValues* message, Panel* target, const KeyValues* userData = NULL);

		virtual int AddMenuItem(const char* itemText, const char* command, Panel* target, const KeyValues* userData = NULL);
		virtual int AddMenuItem(const char* itemText, KeyValues* message, Panel* target, const KeyValues* userData = NULL);
		virtual int AddMenuItem(const char* itemText, Panel* target, const KeyValues* userData = NULL);

		virtual int AddCheckableMenuItem(const char* itemName, const char* itemText, const char* command, Panel* target, const KeyValues* userData = NULL);
		virtual int AddCheckableMenuItem(const char* itemName, const wchar_t* wszItemText, const char* command, Panel* target, const KeyValues* userData = NULL);

		virtual int AddCheckableMenuItem(const char* itemName, const char* itemText, KeyValues* message, Panel* target, const KeyValues* userData = NULL);
		virtual int AddCheckableMenuItem(const char* itemName, const wchar_t* wszItemText, KeyValues* message, Panel* target, const KeyValues* userData = NULL);

		virtual int AddCheckableMenuItem(const char* itemText, const char* command, Panel* target, const KeyValues* userData = NULL);
		virtual int AddCheckableMenuItem(const char* itemText, KeyValues* message, Panel* target, const KeyValues* userData = NULL);
		virtual int AddCheckableMenuItem(const char* itemText, Panel* target, const KeyValues* userData = NULL);

		virtual int AddCascadingMenuItem(const char* itemName, const char* itemText, const char* command, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);
		virtual int AddCascadingMenuItem(const char* itemName, const wchar_t* wszItemText, const char* command, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);

		virtual int AddCascadingMenuItem(const char* itemName, const char* itemText, KeyValues* message, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);
		virtual int AddCascadingMenuItem(const char* itemName, const wchar_t* wszItemText, KeyValues* message, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);

		virtual int AddCascadingMenuItem(const char* itemText, const char* command, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);
		virtual int AddCascadingMenuItem(const char* itemText, KeyValues* message, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);
		virtual int AddCascadingMenuItem(const char* itemText, Panel* target, Menu* cascadeMenu, const KeyValues* userData = NULL);

		virtual int AddMenuItem(MenuItem* panel);

		virtual void AddSeparator();
		virtual void AddSeparatorAfterItem(int itemID);

		virtual void UpdateMenuItem(int itemID, const char* itemText, KeyValues* message, const KeyValues* userData = NULL);
		virtual void UpdateMenuItem(int itemID, const wchar_t* wszItemText, KeyValues* message, const KeyValues* userData = NULL);

		virtual void MoveMenuItem(int itemID, int moveBeforeThisItemID);

		virtual bool IsValidMenuID(int itemID);
		virtual int GetInvalidMenuID();

		KeyValues* GetItemUserData(int itemID);
		void GetItemText(int itemID, wchar_t* text, int bufLenInBytes);
		void GetItemText(int itemID, char* text, int bufLenInBytes);

		virtual void SetItemEnabled(const char* itemName, bool state);
		virtual void SetItemEnabled(int itemID, bool state);
		virtual void SetItemVisible(const char* itemName, bool visible);
		virtual void SetItemVisible(int itemID, bool visible);

		void DeleteItem(int itemID);

		void DeleteAllItems();

		virtual void SetFixedWidth(int width);

		void SetContentAlignment(Label::Alignment alignment);

		virtual void SetMenuItemHeight(int itemHeight);
		virtual int  GetMenuItemHeight() const;

		virtual void SetNumberOfVisibleItems(int numItems);

		void EnableUseMenuManager(bool bUseMenuManager);

		virtual void PerformLayout(void);

		virtual void SetBorder(class IBorder* border);
		virtual void ApplySchemeSettings(IScheme* pScheme);

		enum MenuTypeAheadMode
		{
			COMPAT_MODE = 0,
			HOT_KEY_MODE,
			TYPE_AHEAD_MODE,
		};
		virtual void SetTypeAheadMode(MenuTypeAheadMode mode);
		virtual int GetTypeAheadMode();

		virtual void OnKeyTyped(wchar_t unichar);
		virtual void OnKeyCodeTyped(KeyCode code);

		virtual void SetVisible(bool state);

		virtual void ActivateItem(int itemID);
		virtual void SilentActivateItem(int itemID);         
		virtual void ActivateItemByRow(int row);
		virtual int GetActiveItem();		          

		virtual int GetItemCount();

		virtual int GetMenuID(int index);

		int GetCurrentlyVisibleItemsCount();

		MenuItem* GetMenuItem(int itemID);
		void CloseOtherMenus(MenuItem* item);
		virtual void OnKillFocus();

		int GetMenuMode();
		enum MenuMode
		{
			MOUSE = 0,
			KEYBOARD,
		};

		void SetCurrentlyHighlightedItem(int itemID);
		int GetCurrentlyHighlightedItem();
		void ClearCurrentlyHighlightedItem();

		void SetMenuItemChecked(int itemID, bool state);
		bool IsChecked(int index);      


		void SetMinimumWidth(int width);
		int  GetMinimumWidth();

		virtual void SetFgColor(Color newColor);
		virtual void SetBgColor(Color newColor);

		virtual void SetFont(HFont font);

		void SetCurrentKeyBinding(int itemID, char const* hotkey);

		void ForceCalculateWidth();

		void SetUseFallbackFont(bool bState, HFont hFallback);

	protected:
		int AddMenuItemCharCommand(MenuItem* item, const char* command, Panel* target, const KeyValues* userData);
		int AddMenuItemKeyValuesCommand(MenuItem* item, KeyValues* message, Panel* target, const KeyValues* userData);

		virtual void OnCommand(const char* command);
		MESSAGE_FUNC_PTR(OnMenuItemSelected, "MenuItemSelected", panel);
		virtual void AddScrollBar();
		virtual void RemoveScrollBar();
		MESSAGE_FUNC(OnSliderMoved, "ScrollBarSliderMoved");
		virtual void Paint();
		virtual void LayoutMenuBorder();
		virtual void MakeItemsVisibleInScrollRange(int maxVisibleItems, int nNumPixelsAvailable);
		virtual void OnMouseWheeled(int delta);
		virtual void OnHotKey(wchar_t unichar);
		virtual void OnTypeAhead(wchar_t unichar);

		int	CountVisibleItems();
		void ComputeWorkspaceSize(int& workWide, int& workTall);
		int ComputeFullMenuHeightWithInsets();

		void CalculateWidth();

		void LayoutScrollBar();
		void PositionCascadingMenu();
		void SizeMenuItems();
		void OnCursorMoved(int x, int y);
		void OnKeyCodePressed(KeyCode code);
		void OnMenuClose();
		MESSAGE_FUNC(OnKeyModeSet, "KeyModeSet");

		void SetCurrentlySelectedItem(MenuItem* item);
		void SetCurrentlySelectedItem(int itemID);
		MESSAGE_FUNC_INT(OnCursorEnteredMenuItem, "CursorEnteredMenuItem", VPanel);
		MESSAGE_FUNC_INT(OnCursorExitedMenuItem, "CursorExitedMenuItem", VPanel);

		void MoveAlongMenuItemList(int direction, int loopCount);

		enum
		{
			DEFAULT_MENU_ITEM_HEIGHT = 22,       
			MENU_UP = -1,            
			MENU_DOWN = 1
		};

#ifdef DBGFLAG_VALIDATE
		virtual void Validate(CValidator& validator, char* pchName);
#endif  

	private:
		MenuItem* GetParentMenuItem();

		int 			m_iMenuItemHeight;
		int 			m_iFixedWidth;
		int 			m_iMinimumWidth;               
		int 			m_iNumVisibleLines;	          
		ScrollBar* m_pScroller;

		CUtlLinkedList<MenuItem*, int> 	m_MenuItems;

		CUtlVector<int>					m_VisibleSortedItems;
		CUtlVector<int>					m_SortedItems;		    
		CUtlVector<int>					m_Separators;                 
		CUtlVector<MenuSeparator*>		m_SeparatorPanels;

		bool 			_sizedForScrollBar : 1;          
		bool			m_bUseFallbackFont : 1;
		bool 			_recalculateWidth : 1;
		bool			m_bUseMenuManager : 1;

		int 			_menuWide;
		int 			m_iCurrentlySelectedItemID;
		int 			m_iInputMode;
		int 			m_iCheckImageWidth;            
		int 			m_iProportionalScrollBarSize;
		Label::Alignment	m_Alignment;
		Color 			_borderDark;
		int 			m_iActivatedItem;
		HFont			m_hItemFont;
		HFont			m_hFallbackItemFont;

#define			TYPEAHEAD_BUFSIZE 256
		MenuTypeAheadMode m_eTypeAheadMode;
		wchar_t			m_szTypeAheadBuf[TYPEAHEAD_BUFSIZE];
		int				m_iNumTypeAheadChars;
		double			m_fLastTypeAheadTime;
	};

}   

#endif  