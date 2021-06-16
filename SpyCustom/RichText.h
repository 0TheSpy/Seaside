#ifndef RICHTEXT_H
#define RICHTEXT_H

#ifdef _WIN32
#pragma once
#endif

#include "Panel.h"
#include "utlvector.h"

namespace vgui
{

	class ClickPanel;

	class RichText : public Panel
	{
		DECLARE_CLASS_SIMPLE(RichText, Panel);

	public:
		RichText(Panel* parent, const char* panelName);
		~RichText();

		virtual void SetText(const char* text);
		virtual void SetText(const wchar_t* text);
		void GetText(int offset, OUT_Z_BYTECAP(bufLenInBytes) wchar_t* buf, int bufLenInBytes);
		void GetText(int offset, OUT_Z_BYTECAP(bufLenInBytes) char* pch, int bufLenInBytes);

		void SetFont(HFont font);

		void InsertChar(wchar_t ch);
		void InsertString(const char* text);
		void InsertString(const wchar_t* wszText);

		void SelectNone();
		void SelectAllText();
		void SelectNoText();
		MESSAGE_FUNC(CutSelected, "DoCutSelected");
		MESSAGE_FUNC(CopySelected, "DoCopySelected");

		void SetPanelInteractive(bool bInteractive) { m_bInteractive = bInteractive; }

		void SetUnusedScrollbarInvisible(bool bInvis) { m_bUnusedScrollbarInvis = bInvis; }

		void GotoTextStart();	      
		void GotoTextEnd();	      

		void SetVerticalScrollbar(bool state);
		void SetMaximumCharCount(int maxChars);

		void InsertColorChange(Color col);
		void InsertIndentChange(int pixelsIndent);
		void InsertClickableTextStart(const char* pchClickAction = NULL);
		void InsertClickableTextEnd();
		void InsertPossibleURLString(const char* text, Color URLTextColor, Color normalTextColor);

		void InsertFade(float flSustain, float flLength);

		void ResetAllFades(bool bHold, bool bOnlyExpired = false, float flNewSustain = -1.0f);

		void SetToFullHeight();
		int GetNumLines();


		virtual bool RequestInfo(KeyValues* outputData);
		virtual void SetFgColor(Color color);
		virtual void SetDrawOffsets(int ofsx, int ofsy);
		bool IsScrollbarVisible();

		void SetURLClickedHandler(Panel* pPanelToHandleClickMsg);

		void SetUnderlineFont(HFont font);

		bool IsAllTextAlphaZero() const;
		bool HasText() const;

		void SetDrawTextOnly();

	protected:
		virtual void OnThink();
		virtual void PerformLayout();        
		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual void Paint();

		virtual void ApplySettings(KeyValues* inResourceData);
		virtual void GetSettings(KeyValues* outResourceData);
		virtual const char* GetDescription(void);
		MESSAGE_FUNC_WCHARPTR(OnSetText, "SetText", text);
		MESSAGE_FUNC(OnSliderMoved, "ScrollBarSliderMoved");      
		virtual void OnKillFocus();
		virtual void OnMouseWheeled(int delta);	     
		virtual void OnKeyCodeTyped(KeyCode code);	   

		MESSAGE_FUNC_INT(OnClickPanel, "ClickPanel", index);

		virtual void OnCursorMoved(int x, int y);           
		virtual void OnMousePressed(MouseCode code);      
		virtual void OnMouseDoublePressed(MouseCode code);
		virtual void OnMouseReleased(MouseCode code);	     

		virtual void OnMouseFocusTicked();       
		virtual void OnCursorEntered();	     
		virtual void OnCursorExited();	     

		virtual void OnMouseCaptureLost();
		virtual void OnSizeChanged(int newWide, int newTall);
		virtual void OnSetFocus();

		int ParseTextStringForUrls(const char* text, int startPos, char* pchURLText, int cchURLText, char* pchURL, int cchURL, bool& clickable);
		virtual void OnTextClicked(const wchar_t* text);

#ifdef DBGFLAG_VALIDATE
		virtual void Validate(CValidator& validator, char* pchName);
#endif  

	protected:
		ScrollBar* _vertScrollBar;	       

	private:
		int GetLineHeight();
		HFont GetDefaultFont();

		const wchar_t* ResolveLocalizedTextAndVariables(char const* pchLookup, OUT_Z_BYTECAP(outbufsizeinbytes) wchar_t* outbuf, size_t outbufsizeinbytes);
		void CheckRecalcLineBreaks();

		void GotoWordRight();	       
		void GotoWordLeft();	       

		void TruncateTextStream();
		bool GetSelectedRange(int& cx0, int& cx1);
		void CursorToPixelSpace(int cursorPos, int& cx, int& cy);
		int PixelToCursorSpace(int cx, int cy);
		void AddAnotherLine(int& cx, int& cy);
		void RecalculateDefaultState(int startIndex);

		void LayoutVerticalScrollBarSlider();
		void OpenEditMenu();
		void FinishingURL(int x, int y);
		int GetStartDrawIndex(int& lineBreakIndexIndex);
		int GetCursorLine();
		int GetClickableTextIndexStart(int startIndex);
		void CreateEditMenu();    

		MESSAGE_FUNC_INT(MoveScrollBar, "MoveScrollBar", delta);
		MESSAGE_FUNC_INT(MoveScrollBarDirect, "MoveScrollBarDirect", delta);

		void InvalidateLineBreakStream();
		void RecalculateLineBreaks();

		struct TFade
		{
			float flFadeStartTime;
			float flFadeLength;
			float flFadeSustain;
			int  iOriginalAlpha;
		};

		struct TFormatStream
		{
			Color color;
			int pixelsIndent;
			bool textClickable;
			CUtlSymbol m_sClickableTextAction;

			TFade fade;

			int textStreamIndex;
		};

		bool m_bResetFades;
		bool m_bInteractive;
		bool m_bUnusedScrollbarInvis;
		bool m_bAllTextAlphaIsZero;

		CUtlVector<wchar_t>   m_TextStream;		           
		CUtlVector<int>	   m_LineBreaks;		             
		CUtlVector<TFormatStream> m_FormatStream;	    

		bool m_bRecalcLineBreaks;

		int	_recalculateBreaksIndex;			         	
		bool			   _invalidateVerticalScrollbarSlider;
		int                _cursorPos;			          
		bool			   _mouseSelection;		         
		bool			   _mouseDragSelection;	              
		int                _select[2];			             
		int				   _pixelsIndent;
		int				   _maxCharCount;		           
		HFont              _font;				       
		HFont			   m_hFontUnderline;
		Color			   _selectionColor;
		Color			   _selectionTextColor;	     
		bool			   _currentTextClickable;
		CUtlVector<ClickPanel*>  _clickableTextPanels;
		int				   _clickableTextIndex;
		Color				_defaultTextColor;
		int					_drawOffsetX;
		int					_drawOffsetY;

		Panel* m_pInterior;
		PHandle				m_hPanelToHandleClickingURLs;


		Menu* m_pEditMenu;		  

		char* m_pszInitialText;	  

		bool _recalcSavedRenderState;

		struct TRenderState
		{
			int x, y;

			Color textColor;
			int pixelsIndent;
			bool textClickable;

			int formatStreamIndex;
		};
		TRenderState m_CachedRenderState;	        

		bool UpdateRenderState(int textStreamPos, TRenderState& renderState);
		void CalculateFade(TRenderState& renderState);

		void GenerateRenderStateForTextStreamIndex(int textStreamIndex, TRenderState& renderState);
		int FindFormatStreamIndexForTextStreamPos(int textStreamIndex);

		int DrawString(int iFirst, int iLast, TRenderState& renderState, HFont font);
	};

}   


#endif  