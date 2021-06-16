#ifndef VGUI_IINPUT_H
#define VGUI_IINPUT_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "interface.h"
#include "MouseCode.h"
#include "KeyCode.h"

namespace vgui
{

	class Cursor;
	typedef unsigned long HCursor;

#define VGUI_GCS_COMPREADSTR                 0x0001
#define VGUI_GCS_COMPREADATTR                0x0002
#define VGUI_GCS_COMPREADCLAUSE              0x0004
#define VGUI_GCS_COMPSTR                     0x0008
#define VGUI_GCS_COMPATTR                    0x0010
#define VGUI_GCS_COMPCLAUSE                  0x0020
#define VGUI_GCS_CURSORPOS                   0x0080
#define VGUI_GCS_DELTASTART                  0x0100
#define VGUI_GCS_RESULTREADSTR               0x0200
#define VGUI_GCS_RESULTREADCLAUSE            0x0400
#define VGUI_GCS_RESULTSTR                   0x0800
#define VGUI_GCS_RESULTCLAUSE                0x1000
#define VGUI_CS_INSERTCHAR                   0x2000
#define VGUI_CS_NOMOVECARET                  0x4000

#define MESSAGE_CURSOR_POS -1
#define MESSAGE_CURRENT_KEYFOCUS -2


	class IInput : public IBaseInterface
	{
	public:
		virtual void SetMouseFocus(VPANEL newMouseFocus) = 0;
		virtual void SetMouseCapture(VPANEL panel) = 0;

		virtual void GetKeyCodeText(KeyCode code, OUT_Z_BYTECAP(buflen) char* buf, int buflen) = 0;

		virtual VPANEL GetFocus() = 0;
		virtual VPANEL GetCalculatedFocus() = 0;          
		virtual VPANEL GetMouseOver() = 0;		           

		virtual void SetCursorPos(int x, int y) = 0;
		virtual void GetCursorPos(int& x, int& y) = 0;
		virtual bool WasMousePressed(MouseCode code) = 0;
		virtual bool WasMouseDoublePressed(MouseCode code) = 0;
		virtual bool IsMouseDown(MouseCode code) = 0;

		virtual void SetCursorOveride(HCursor cursor) = 0;
		virtual HCursor GetCursorOveride() = 0;

		virtual bool WasMouseReleased(MouseCode code) = 0;
		virtual bool WasKeyPressed(KeyCode code) = 0;
		virtual bool IsKeyDown(KeyCode code) = 0;
		virtual bool WasKeyTyped(KeyCode code) = 0;
		virtual bool WasKeyReleased(KeyCode code) = 0;

		virtual VPANEL GetAppModalSurface() = 0;
		virtual void SetAppModalSurface(VPANEL panel) = 0;
		virtual void ReleaseAppModalSurface() = 0;

		virtual void GetCursorPosition(int& x, int& y) = 0;

		virtual void SetIMEWindow(void* hwnd) = 0;
		virtual void* GetIMEWindow() = 0;

		virtual void OnChangeIME(bool forward) = 0;
		virtual int  GetCurrentIMEHandle() = 0;
		virtual int  GetEnglishIMEHandle() = 0;

		virtual void GetIMELanguageName(OUT_Z_BYTECAP(unicodeBufferSizeInBytes) wchar_t* buf, int unicodeBufferSizeInBytes) = 0;
		virtual void GetIMELanguageShortCode(OUT_Z_BYTECAP(unicodeBufferSizeInBytes) wchar_t* buf, int unicodeBufferSizeInBytes) = 0;

		struct LanguageItem
		{
			wchar_t		shortname[4];
			wchar_t		menuname[128];
			int			handleValue;
			bool		active;        
		};

		struct ConversionModeItem
		{
			wchar_t		menuname[128];
			int			handleValue;
			bool		active;         
		};

		struct SentenceModeItem
		{
			wchar_t		menuname[128];
			int			handleValue;
			bool		active;         
		};

		virtual int	 GetIMELanguageList(LanguageItem* dest, int destcount) = 0;
		virtual int	 GetIMEConversionModes(ConversionModeItem* dest, int destcount) = 0;
		virtual int	 GetIMESentenceModes(SentenceModeItem* dest, int destcount) = 0;

		virtual void OnChangeIMEByHandle(int handleValue) = 0;
		virtual void OnChangeIMEConversionModeByHandle(int handleValue) = 0;
		virtual void OnChangeIMESentenceModeByHandle(int handleValue) = 0;

		virtual void OnInputLanguageChanged() = 0;
		virtual void OnIMEStartComposition() = 0;
		virtual void OnIMEComposition(int flags) = 0;
		virtual void OnIMEEndComposition() = 0;

		virtual void OnIMEShowCandidates() = 0;
		virtual void OnIMEChangeCandidates() = 0;
		virtual void OnIMECloseCandidates() = 0;
		virtual void OnIMERecomputeModes() = 0;

		virtual int  GetCandidateListCount() = 0;
		virtual void GetCandidate(int num, OUT_Z_BYTECAP(destSizeBytes) wchar_t* dest, int destSizeBytes) = 0;
		virtual int  GetCandidateListSelectedItem() = 0;
		virtual int  GetCandidateListPageSize() = 0;
		virtual int  GetCandidateListPageStart() = 0;

		virtual void SetCandidateWindowPos(int x, int y) = 0;

		virtual bool GetShouldInvertCompositionString() = 0;
		virtual bool CandidateListStartsAtOne() = 0;

		virtual void SetCandidateListPageStart(int start) = 0;

		virtual void SetMouseCaptureEx(VPANEL panel, MouseCode captureStartMouseCode) = 0;

		virtual void RegisterKeyCodeUnhandledListener(VPANEL panel) = 0;
		virtual void UnregisterKeyCodeUnhandledListener(VPANEL panel) = 0;

		virtual void OnKeyCodeUnhandled(int keyCode) = 0;

		virtual void	SetModalSubTree(VPANEL subTree, VPANEL unhandledMouseClickListener, bool restrictMessagesToSubTree = true) = 0;
		virtual void	ReleaseModalSubTree() = 0;
		virtual VPANEL	GetModalSubTree() = 0;

		virtual void	SetModalSubTreeReceiveMessages(bool state) = 0;
		virtual bool	ShouldModalSubTreeReceiveMessages() const = 0;

		virtual VPANEL 	GetMouseCapture() = 0;
	};

#define VGUI_INPUT_INTERFACE_VERSION "VGUI_Input005"

}   


#endif  