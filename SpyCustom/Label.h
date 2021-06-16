#ifndef LABEL_H
#define LABEL_H

#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "VGUI.h"
#include "Panel.h"
#include "PHandle.h"

namespace vgui
{

	class Label : public Panel
	{
		DECLARE_CLASS_SIMPLE(Label, Panel);

	public:
		Label(Panel* parent, const char* panelName, const char* text);
		Label(Panel* parent, const char* panelName, const wchar_t* wszText);
		~Label();

	public:
		virtual void SetText(const char* tokenName);

		virtual void SetText(const wchar_t* unicodeString, bool bClearUnlocalizedSymbol = false);

		virtual void GetText(OUT_Z_BYTECAP(bufferLen) char* textOut, int bufferLen);
		virtual void GetText(OUT_Z_BYTECAP(bufLenInBytes) wchar_t* textOut, int bufLenInBytes);

		virtual void GetContentSize(int& wide, int& tall);

		enum Alignment
		{
			a_northwest = 0,
			a_north,
			a_northeast,
			a_west,
			a_center,
			a_east,
			a_southwest,
			a_south,
			a_southeast,
		};

		virtual void SetContentAlignment(Alignment alignment);
		virtual void SetEnabled(bool state);
		virtual void SetTextInset(int xInset, int yInset);
		virtual void GetTextInset(int* xInset, int* yInset);

		virtual void SetFgColor(Color color);
		virtual Color GetFgColor();

		virtual void SetDisabledFgColor1(Color color);
		virtual void SetDisabledFgColor2(Color color);
		virtual Color GetDisabledFgColor1();
		virtual Color GetDisabledFgColor2();

		enum EColorState
		{
			CS_NORMAL,
			CS_DULL,
			CS_BRIGHT,
		};
		virtual void SetTextColorState(EColorState state);

		virtual void SetFont(HFont font);
		virtual HFont GetFont();

		virtual Panel* HasHotkey(wchar_t key);
		virtual void SetHotkey(wchar_t key);
		virtual wchar_t GetHotKey();

		virtual void SetAssociatedControl(Panel* control);

		virtual int AddImage(IImage* image, int preOffset);          
		virtual void SetImageAtIndex(int index, IImage* image, int preOffset);
		virtual void SetImagePreOffset(int index, int preOffset);          
		virtual IImage* GetImageAtIndex(int index);
		virtual int GetImageCount();
		virtual void ClearImages();
		virtual void ResetToSimpleTextImage();
		virtual void SetImageBounds(int index, int x, int width);

		virtual TextImage* GetTextImage();

		virtual int SetTextImageIndex(int newIndex);

		virtual bool RequestInfo(KeyValues* outputData);
		virtual void SizeToContents();

		enum Padding
		{
			Content = 8,
		};

		void SetWrap(bool bWrap);
		void SetCenterWrap(bool bWrap);

		void SetAllCaps(bool bAllCaps);

	protected:
		virtual void PerformLayout();
		virtual wchar_t CalculateHotkey(const char* text);
		virtual wchar_t CalculateHotkey(const wchar_t* text);
		virtual void ComputeAlignment(int& tx0, int& ty0, int& tx1, int& ty1);
		virtual void Paint();
		MESSAGE_FUNC_PARAMS(OnSetText, "SetText", params);
		virtual void DrawDashedLine(int x0, int y0, int x1, int y1, int dashLen, int gapLen);
		virtual void OnRequestFocus(VPANEL subFocus, VPANEL defaultPanel);
		MESSAGE_FUNC(OnHotkeyPressed, "Hotkey");
		virtual void OnMousePressed(MouseCode code);
		virtual void OnSizeChanged(int wide, int tall);

		virtual void EnsureImageCapacity(int maxIndex);

		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual void GetSettings(KeyValues* outResourceData);
		virtual void ApplySettings(KeyValues* inResourceData);
		virtual const char* GetDescription(void);

		MESSAGE_FUNC_PARAMS(OnDialogVariablesChanged, "DialogVariables", dialogVariables);

		void HandleAutoSizing(void);

	private:
		void Init();

		Alignment  _contentAlignment;
		TextImage* _textImage;           
		struct TImageInfo
		{
			IImage* image;
			short offset;
			short xpos;
			short width;
		};
		CUtlVector<TImageInfo> _imageDar;

		int		   _textInset[2];
		Color      _disabledFgColor1;
		Color      _disabledFgColor2;
		Color	   _associateColor;
		int		   _textImageIndex;	          
		EColorState _textColorState;

		PHandle		_associate;
		char* _associateName;

		char* _fontOverrideName;

		wchar_t	   _hotkey;		      

		bool	m_bWrap;
		bool	m_bCenterWrap;
		bool	m_bAllCaps;
		bool	m_bAutoWideToContents;
		bool	m_bAutoWideDirty;
		bool	m_bUseProportionalInsets;

	};

}   

#endif  