#ifndef LISTPANEL_H
#define LISTPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "utllinkedlist.h"
#include "utlvector.h"
#include "utlrbtree.h"
#include "VGUI.h"
#include "Panel.h"

class KeyValues;

namespace vgui
{

	class ScrollBar;
	class TextImage;
	class ImagePanel;
	class Label;
	class Button;
	class IDraggerEvent;
	class FastSortListPanelItem;

	class ListPanelItem
	{
	public:
		ListPanelItem() :
			kv(0),
			userData(0),
			m_pDragData(0),
			m_bImage(false),
			m_nImageIndex(-1),
			m_nImageIndexSelected(-1),
			m_pIcon(0)
		{
		}

		KeyValues* kv;
		unsigned int 	userData;
		KeyValues* m_pDragData;
		bool			m_bImage;
		int				m_nImageIndex;
		int				m_nImageIndexSelected;
		IImage* m_pIcon;
	};

	typedef int __cdecl SortFunc(
		ListPanel* pPanel,
		const ListPanelItem& item1,
		const ListPanelItem& item2);

	class ListPanel : public Panel
	{
		DECLARE_CLASS_SIMPLE(ListPanel, Panel);

	public:
		ListPanel(Panel* parent, const char* panelName);
		~ListPanel();

		enum ColumnFlags_e
		{
			COLUMN_FIXEDSIZE = 0x01,          
			COLUMN_RESIZEWITHWINDOW = 0x02,            
			COLUMN_IMAGE = 0x04,	                 
			COLUMN_HIDDEN = 0x08,	     
			COLUMN_UNHIDABLE = 0x10,	   
		};

		virtual void AddColumnHeader(int index, const char* columnName, const char* columnText, int startingWidth, int minWidth, int maxWidth, int columnFlags = 0);
		virtual void AddColumnHeader(int index, const char* columnName, const char* columnText, int width, int columnFlags = 0);

		virtual void RemoveColumn(int column);	   
		virtual int  FindColumn(const char* columnName);
		virtual void SetColumnHeaderHeight(int height);
		virtual void SetColumnHeaderText(int column, const char* text);
		virtual void SetColumnHeaderText(int column, wchar_t* text);
		virtual void SetColumnHeaderImage(int column, int imageListIndex);
		virtual void SetColumnHeaderTooltip(int column, const char* tooltipText);
		virtual void SetColumnTextAlignment(int column, int align);

		virtual int GetNumColumnHeaders() const;
		virtual bool GetColumnHeaderText(int index, char* pOut, int maxLen);

		virtual void SetSortFunc(int column, SortFunc* func);
		virtual void SetSortColumn(int column);
		virtual void SortList(void);
		virtual void SetColumnSortable(int column, bool sortable);
		virtual void SetColumnVisible(int column, bool visible);
		int GetSortColumn() const;

		virtual void SetAllowUserModificationOfColumns(bool allowed);

		virtual int AddItem(const KeyValues* data, unsigned int userData, bool bScrollToItem, bool bSortOnAdd);                   
		void SetItemDragData(int itemID, const KeyValues* data);                          
		virtual int	GetItemCount(void);			      
		virtual int GetItem(const char* itemName);	          
		virtual KeyValues* GetItem(int itemID);        
		virtual int GetItemCurrentRow(int itemID);		         
		virtual int GetItemIDFromRow(int currentRow);			     
		virtual unsigned int GetItemUserData(int itemID);
		virtual ListPanelItem* GetItemData(int itemID);
		virtual void SetUserData(int itemID, unsigned int userData);
		virtual int GetItemIDFromUserData(unsigned int userData);
		virtual void ApplyItemChanges(int itemID);               
		virtual void RemoveItem(int itemID);              
		virtual void RereadAllItems();        

		virtual void RemoveAll();		           
		virtual void DeleteAllItems();	   

		virtual void GetCellText(int itemID, int column, OUT_Z_BYTECAP(bufferSizeInBytes) wchar_t* buffer, int bufferSizeInBytes);         
		virtual IImage* GetCellImage(int itemID, int column);            

		virtual int FirstItem() const;
		virtual int NextItem(int iItem) const;

		virtual int InvalidItemID() const;
		virtual bool IsValidItemID(int itemID);

		virtual void SetItemVisible(int itemID, bool state);
		virtual void SetItemDisabled(int itemID, bool state);
		bool IsItemVisible(int itemID);

		virtual void SetFont(HFont font);

		virtual void SetImageList(ImageList* imageList, bool deleteImageListWhenDone);

		virtual int GetSelectedItemsCount();

		virtual int GetSelectedItem(int selectionIndex);

		virtual void ClearSelectedItems();

		virtual bool IsItemSelected(int itemID);

		virtual void AddSelectedItem(int itemID);

		virtual void SetSingleSelectedItem(int itemID);

		virtual int GetSelectedColumn();

		virtual void SetSelectIndividualCells(bool state);

		void SetMultiselectEnabled(bool bState);
		bool IsMultiselectEnabled() const;

		virtual void SetSelectedCell(int row, int column);

		virtual bool GetCellAtPos(int x, int y, int& row, int& column);	                 
		virtual bool GetCellBounds(int row, int column, int& x, int& y, int& wide, int& tall);

		virtual void SetEmptyListText(const char* text);
		virtual void SetEmptyListText(const wchar_t* text);

		void ResetScrollBar();

		virtual void OnCreateDragData(KeyValues* msg);

		void		SetIgnoreDoubleClick(bool state);

		virtual void EnterEditMode(int itemID, int column, vgui::Panel* editPanel);

		virtual void LeaveEditMode();

		virtual bool IsInEditMode();

		MESSAGE_FUNC_INT(ResizeColumnToContents, "ResizeColumnToContents", column);

#ifdef _X360
		virtual void NavigateTo();
#endif
		int m_nUserConfigFileVersion;

	protected:
		virtual Panel* GetCellRenderer(int row, int column);

		virtual void OnMouseWheeled(int delta);
		virtual void OnSizeChanged(int wide, int tall);
		virtual void PerformLayout();
		virtual void Paint();
		virtual void PaintBackground();
		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual void OnMousePressed(MouseCode code);
		virtual void OnMouseDoublePressed(MouseCode code);
#ifdef _X360
		virtual void OnKeyCodePressed(KeyCode code);
#else
		virtual void OnKeyCodePressed(KeyCode code);
#endif
		MESSAGE_FUNC(OnSliderMoved, "ScrollBarSliderMoved");
		MESSAGE_FUNC_INT_INT(OnColumnResized, "ColumnResized", column, delta);
		MESSAGE_FUNC_INT(OnSetSortColumn, "SetSortColumn", column);
		MESSAGE_FUNC(OpenColumnChoiceMenu, "OpenColumnChoiceMenu");
		MESSAGE_FUNC_INT(OnToggleColumnVisible, "ToggleColumnVisible", col);
		virtual float GetRowsPerPage();
		virtual int GetStartItem();

		virtual void ApplyUserConfigSettings(KeyValues* userConfig);
		virtual void GetUserConfigSettings(KeyValues* userConfig);
		virtual bool HasUserConfigSettings();

	public:
		virtual void SetSortColumnEx(int iPrimarySortColumn, int iSecondarySortColumn, bool bSortAscending);
		void GetSortColumnEx(int& iPrimarySortColumn, int& iSecondarySortColumn, bool& bSortAscending) const;

	private:
		void CleanupItem(FastSortListPanelItem* data);

		void IndexItem(int itemID);

		void UpdateSelection(vgui::MouseCode code, int x, int y, int row, int column);

		void HandleMultiSelection(int itemID, int row, int column);

		void HandleAddSelection(int itemID, int row, int column);

		struct IndexItem_t
		{
			ListPanelItem* dataItem;
			int duplicateIndex;
		};
		typedef CUtlRBTree<IndexItem_t, int> IndexRBTree_t;

		struct column_t
		{
			Button* m_pHeader;
			int	m_iMinWidth;
			int	m_iMaxWidth;
			bool m_bResizesWithWindow;
			Panel* m_pResizer;
			SortFunc* m_pSortFunc;
			bool m_bTypeIsText;
			bool m_bHidden;
			bool m_bUnhidable;
			IndexRBTree_t m_SortedTree;
			int m_nContentAlignment;
		};

		CUtlLinkedList<column_t, unsigned char> 		m_ColumnsData;

		CUtlVector<unsigned char>						m_ColumnsHistory;

		CUtlVector<unsigned char>						m_CurrentColumns;

		int				    m_iColumnDraggerMoved;         
		int					m_lastBarWidth;

		CUtlLinkedList<FastSortListPanelItem*, int>		m_DataItems;
		CUtlVector<int>									m_VisibleItems;

		int 				m_iSortColumn;
		int 				m_iSortColumnSecondary;

		void 				ResortColumnRBTree(int col);
		static bool 		RBTreeLessFunc(vgui::ListPanel::IndexItem_t& item1, vgui::ListPanel::IndexItem_t& item2);

		TextImage* m_pTextImage;    
		ImagePanel* m_pImagePanel;    
		Label* m_pLabel;	     
		ScrollBar* m_hbar;
		ScrollBar* m_vbar;

		int				m_iSelectedColumn;

		bool 			m_bNeedsSort : 1;
		bool 			m_bSortAscending : 1;
		bool 			m_bSortAscendingSecondary : 1;
		bool			m_bCanSelectIndividualCells : 1;
		bool			m_bShiftHeldDown : 1;
		bool			m_bMultiselectEnabled : 1;
		bool			m_bAllowUserAddDeleteColumns : 1;
		bool 			m_bDeleteImageListWhenDone : 1;
		bool			m_bIgnoreDoubleClick : 1;

		int				m_iHeaderHeight;
		int 			m_iRowHeight;

		CUtlVector<int> 	m_SelectedItems;		    
		int					m_LastItemSelected;	         

		int 		m_iTableStartX;
		int	 		m_iTableStartY;

		Color 		m_LabelFgColor;
		Color		m_DisabledColor;
		Color 		m_SelectionFgColor;
		Color		m_DisabledSelectionFgColor;

		ImageList* m_pImageList;
		TextImage* m_pEmptyListText;

		PHandle		m_hEditModePanel;
		int			m_iEditModeItemID;
		int			m_iEditModeColumn;

		void ResetColumnHeaderCommands();
	};

}

#endif  