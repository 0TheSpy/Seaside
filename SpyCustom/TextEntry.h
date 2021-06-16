#ifndef TEXTENTRY_H
#define TEXTENTRY_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "Color.h"
#include "Panel.h"
#include "Label.h"
#include "ListPanel.h"

#include "utlvector.h"

namespace vgui
{

	class TextEntry : public Panel
	{
		DECLARE_CLASS_SIMPLE(TextEntry, Panel);

	public:
		TextEntry(Panel* parent, const char* panelName);
		virtual ~TextEntry();

		virtual void SetText(const wchar_t* wszText);
		virtual void SetText(const char* text);
		virtual void GetText(OUT_Z_BYTECAP(bufLenInBytes) char* buf, int bufLenInBytes);
		virtual void GetText(OUT_Z_BYTECAP(bufLenInBytes) wchar_t* buf, int bufLenInBytes);
		virtual int GetTextLength() const;
		virtual bool IsTextFullySelected() const;

		virtual void GotoLeft();		     
		virtual void GotoRight();		     
		virtual void GotoUp();			     
		virtual void GotoDown();		     
		virtual void GotoWordRight();	       
		virtual void GotoWordLeft();	       
		virtual void GotoFirstOfLine();	        
		virtual void GotoEndOfLine();	        
		virtual void GotoTextStart();	      
		virtual void GotoTextEnd();		      

		virtual void InsertChar(wchar_t ch);
		virtual void InsertString(const char* text);
		virtual void InsertString(const wchar_t* wszText);
		virtual void Backspace();
		virtual void Delete();
		virtual void SelectNone();
		virtual void OpenEditMenu();
		MESSAGE_FUNC(CutSelected, "DoCutSelected");
		MESSAGE_FUNC(CopySelected, "DoCopySelected");
		MESSAGE_FUNC(Paste, "DoPaste");

		MESSAGE_FUNC_INT(LanguageChanged, "DoLanguageChanged", handle);
		MESSAGE_FUNC_INT(ConversionModeChanged, "DoConversionModeChanged", handle);
		MESSAGE_FUNC_INT(SentenceModeChanged, "DoSentenceModeChanged", handle);

		MESSAGE_FUNC_WCHARPTR(CompositionString, "DoCompositionString", string);

		MESSAGE_FUNC(ShowIMECandidates, "DoShowIMECandidates");
		MESSAGE_FUNC(HideIMECandidates, "DoHideIMECandidates");
		MESSAGE_FUNC(UpdateIMECandidates, "DoUpdateIMECandidates");

		virtual void DeleteSelected();
		virtual void Undo();
		virtual void SaveUndoState();
		virtual void SetFont(HFont font);
		virtual void SetTextHidden(bool bHideText);
		virtual void SetEditable(bool state);
		virtual bool IsEditable();
		virtual void SetEnabled(bool state);
		virtual void MoveCursor(int line, int pixelsAcross);

		virtual void SetDisabledBgColor(Color col);

		virtual void SetMultiline(bool state);
		virtual bool IsMultiline();

		virtual void SetVerticalScrollbar(bool state);

		virtual void SetCatchEnterKey(bool state);

		virtual void SendNewLine(bool send);

		virtual void SetMaximumCharCount(int maxChars);
		virtual int GetMaximumCharCount();
		virtual void SetAutoProgressOnHittingCharLimit(bool state);

		virtual void SetWrap(bool wrap);

		virtual void RecalculateLineBreaks();
		virtual void LayoutVerticalScrollBarSlider();

		virtual bool RequestInfo(KeyValues* outputData);

		void SetToFullHeight();

		void SetToFullWidth();

		int GetNumLines();

		void SelectAllText(bool bResetCursorPos);
		void SelectNoText();
		void SelectAllOnFirstFocus(bool status);
		void SetDrawWidth(int width);            
		int GetDrawWidth();
		void SetHorizontalScrolling(bool status);       

		void SetAllowNonAsciiCharacters(bool state);

		void SetAllowNumericInputOnly(bool state);

		void SetDrawLanguageIDAtLeft(bool state);

		virtual bool GetDropContextMenu(Menu* menu, CUtlVector< KeyValues* >& data);
		virtual bool IsDroppable(CUtlVector< KeyValues* >& data);
		virtual void OnPanelDropped(CUtlVector< KeyValues* >& data);
		virtual Panel* GetDragPanel();
		virtual void OnCreateDragData(KeyValues* msg);

		void SelectAllOnFocusAlways(bool status);
		void SetSelectionTextColor(const Color& clr);
		void SetSelectionBgColor(const Color& clr);
		void SetSelectionUnfocusedBgColor(const Color& clr);

		void SetUseFallbackFont(bool bState, HFont hFallback);

	protected:
		virtual void ResetCursorBlink();
		virtual void PerformLayout();        
		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual void PaintBackground();
		virtual int  DrawChar(wchar_t ch, HFont font, int index, int x, int y);
		virtual bool DrawCursor(int x, int y);

		virtual void SetCharAt(wchar_t ch, int index);           
		virtual void ApplySettings(KeyValues* inResourceData);
		virtual void GetSettings(KeyValues* outResourceData);
		virtual const char* GetDescription(void);
		virtual void FireActionSignal();
		virtual bool GetSelectedRange(int& cx0, int& cx1);
		virtual void CursorToPixelSpace(int cursorPos, int& cx, int& cy);
		virtual int  PixelToCursorSpace(int cx, int cy);
		virtual void AddAnotherLine(int& cx, int& cy);
		virtual int  GetYStart();        

		virtual bool SelectCheck(bool fromMouse = false);	         
		MESSAGE_FUNC_WCHARPTR(OnSetText, "SetText", text);
		MESSAGE_FUNC(OnSliderMoved, "ScrollBarSliderMoved");      
		virtual void OnKillFocus();
		virtual void OnMouseWheeled(int delta);	     
		virtual void OnKeyCodePressed(KeyCode code);    
		virtual void OnKeyCodeTyped(KeyCode code);	   
		virtual	void OnKeyTyped(wchar_t unichar);	   

		virtual void OnCursorMoved(int x, int y);           
		virtual void OnMousePressed(MouseCode code);      
		virtual void OnMouseDoublePressed(MouseCode code);
		virtual void OnMouseTriplePressed(MouseCode code);
		virtual void OnMouseReleased(MouseCode code);	     

		virtual void OnKeyFocusTicked();       
		virtual void OnMouseFocusTicked();       
		virtual void OnCursorEntered();	     
		virtual void OnCursorExited();	     

		virtual void OnMouseCaptureLost();
		virtual void OnSizeChanged(int newWide, int newTall);

		virtual int GetStartDrawIndex(int& lineBreakIndexIndex);

	public:
		virtual float GetValueAsFloat();
		virtual int GetValueAsInt();

	protected:
		void ScrollRight();        
		void ScrollLeft();      
		bool IsCursorOffRightSideOfWindow(int cursorPos);          
		bool IsCursorOffLeftSideOfWindow(int cursorPos);          
		void ScrollLeftForResize();

		void OnSetFocus();
		void OnChangeIME(bool forward);

		bool NeedsEllipses(HFont font, int* pIndex);

	private:
		MESSAGE_FUNC_INT(OnSetState, "SetState", state);
		int GetCurrentLineStart();
		int GetCurrentLineEnd();
		bool IsLineBreak(int index);
		int GetCursorLine();
		void MoveScrollBar(int delta);
		void CalcBreakIndex();   
		void CreateEditMenu();    

	public:
		Menu* GetEditMenu();    

	private:
		void	FlipToLastIME();

	public:
		virtual void GetTextRange(wchar_t* buf, int from, int numchars);	            
		virtual void GetTextRange(char* buf, int from, int numchars);	            

	private:

		CUtlVector<wchar_t> m_TextStream;		           
		CUtlVector<wchar_t> m_UndoTextStream;	         
		CUtlVector<int>		m_LineBreaks;		             

		int                _cursorPos;		          
		bool               _cursorIsAtEnd;
		bool               _putCursorAtEnd;
		int				   _undoCursorPos;	         
		bool               _cursorBlink;	      
		bool               _hideText;		        
		bool			   _editable;		      
		bool			   _mouseSelection;	         
		bool			   _mouseDragSelection;               
		int				   _mouseSelectCursorStart;	         
		long               _cursorNextBlinkTime;       
		int                _cursorBlinkRate;	      
		int                _select[2];	             
		int				   _pixelsIndent;
		int				   _charCount;
		int				   _maxCharCount;             
		HFont              _font;		          
		HFont			   _smallfont;
		bool			   _dataChanged;          
		bool			   _multiline;	            
		bool			   _verticalScrollbar;        
		ScrollBar* _vertScrollBar;		         
		Color			   _cursorColor;	       
		Color			   _disabledFgColor;
		Color			   _disabledBgColor;
		Color			   _selectionColor;
		Color			   _selectionTextColor;	      
		Color			   _defaultSelectionBG2Color;
		int				   _currentStartLine;        
		int				   _currentStartIndex;       
		bool			   _horizScrollingAllowed;	       
		Color			   _focusEdgeColor;
		bool		       _catchEnterKey;
		bool			   _wrap;
		bool			   _sendNewLines;
		int				   _drawWidth;

		Menu* m_pEditMenu;  

		int				   _recalculateBreaksIndex;          	
		bool			   _selectAllOnFirstFocus : 1;          
		bool				_selectAllOnFocusAlways : 1;
		bool			   _firstFocusStatus;           
		bool				m_bAllowNumericInputOnly;
		bool				m_bAllowNonAsciiCharacters;
		bool				m_bAutoProgressOnHittingCharLimit;

		enum
		{
			MAX_COMPOSITION_STRING = 256,
		};

		wchar_t				m_szComposition[MAX_COMPOSITION_STRING];
		Menu* m_pIMECandidates;
		int					m_hPreviousIME;
		bool				m_bDrawLanguageIDAtLeft;
		int					m_nLangInset;

		bool				m_bUseFallbackFont : 1;
		HFont				m_hFallbackFont;
	};

}

#endif  