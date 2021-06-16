#ifndef TEXTIMAGE_H
#define TEXTIMAGE_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "ILocalize.h"
#include "Image.h"

#include "utlvector.h"
#include "UtlSortVector.h"

class KeyValues;

namespace vgui
{

	struct label_colorchange_t
	{
		Color	color;
		int		textStreamIndex;
	};

	class CColorChangeListLess
	{
	public:
		bool Less(const label_colorchange_t& src1, const label_colorchange_t& src2, void* pCtx)
		{
			if (src1.textStreamIndex < src2.textStreamIndex)
				return true;

			return false;
		}
	};

	class TextImage : public Image
	{
	public:
		TextImage(const char* text);
		TextImage(const wchar_t* wszText);
		~TextImage();

	public:
		virtual void SetText(const char* text);
		virtual void SetText(const wchar_t* text, bool bClearUnlocalizedSymbol = false);
		virtual void GetText(char* buffer, int bufferSize);
		virtual void GetText(wchar_t* buffer, int bufferLength);
		virtual void GetUnlocalizedText(char* buffer, int bufferSize);
		virtual StringIndex_t GetUnlocalizedTextSymbol();

		virtual void SetFont(vgui::HFont font);
		virtual vgui::HFont GetFont();

		void SetDrawWidth(int width);
		void GetDrawWidth(int& width);

		void ResizeImageToContent();
		void ResizeImageToContentMaxWidth(int nMaxWidth);

		virtual void SetSize(int wide, int tall);

		virtual void GetContentSize(int& wide, int& tall);

		virtual void Paint();

		void SetWrap(bool bWrap);
		void RecalculateNewLinePositions();

		void SetUseFallbackFont(bool bState, HFont hFallback);

		void SetAllCaps(bool bAllCaps);

		void SetCenterWrap(bool bWrap);
		void RecalculateCenterWrapIndents();

		const wchar_t* GetUText(void) { return _utext; }

		void AddColorChange(Color col, int iTextStreamIndex);
		void SetColorChangeStream(CUtlSortVector<label_colorchange_t, CColorChangeListLess>* pUtlVecStream);
		void ClearColorChangeStream(void) { m_ColorChangeStream.Purge(); }

		const wchar_t* GetEllipsesPosition(void) const { return m_pwszEllipsesPosition; }
		bool IsWrapping() const { return m_LineBreaks.Count() != 0; }

	protected:
		void SizeText(wchar_t* tempText, int stringLength);
		virtual void GetTextSize(int& wide, int& tall);

	private:
		void RecalculateEllipsesPosition();

		wchar_t* _utext;	     
		short _textBufferLen;	     
		short _textLen;		     
		vgui::HFont _font;	     
		vgui::HFont _fallbackFont;
		int _drawWidth;		            
		StringIndex_t _unlocalizedTextSymbol;	         
		wchar_t* m_pwszEllipsesPosition;

		bool m_bRecalculateTruncation : 1;
		bool m_bWrap : 1;
		bool m_bUseFallbackFont : 1;
		bool m_bRenderUsingFallbackFont : 1;
		bool m_bAllCaps : 1;
		CUtlVector<wchar_t*>		m_LineBreaks;		             

		bool m_bWrapCenter;								          
		CUtlVector<int>				m_LineXIndent;		          

		CUtlSortVector<label_colorchange_t, CColorChangeListLess>		m_ColorChangeStream;
	};

}   

#endif  