#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#ifdef _WIN32
#pragma once
#endif

#include "vgui.h"
#include "Frame.h"

#ifdef MessageBox
#undef MessageBox
#endif

namespace vgui
{

	class MessageBox : public Frame
	{
		DECLARE_CLASS_SIMPLE(MessageBox, Frame);

	public:
		MessageBox(const char* title, const char* text, Panel* parent = NULL);
		MessageBox(const wchar_t* wszTitle, const wchar_t* wszText, Panel* parent = NULL);
		~MessageBox();

		virtual void DoModal(Frame* pFrameOver = NULL);

		virtual void ShowWindow(Frame* pFrameOver = NULL);

		virtual void SetCommand(const char* command);
		virtual void SetCommand(KeyValues* command);

		virtual void SetOKButtonVisible(bool state);

		virtual void SetOKButtonText(const char* buttonText);
		virtual void SetOKButtonText(const wchar_t* wszButtonText);

		void SetCancelButtonVisible(bool state);
		void SetCancelButtonText(const char* buttonText);
		void SetCancelButtonText(const wchar_t* wszButtonText);
		void SetCancelCommand(KeyValues* command);

		virtual void DisableCloseButton(bool state);

		virtual void OnCommand(const char* pCommand);

		void ShowMessageBoxOverCursor(bool bEnable);

	protected:
		virtual void PerformLayout();
		virtual void ApplySchemeSettings(IScheme* pScheme);

	protected:
		Button* m_pOkButton;
		Button* m_pCancelButton;
		Label* m_pMessageLabel;

	private:
		MESSAGE_FUNC(OnShutdownRequest, "ShutdownRequest");

		void Init();

		KeyValues* m_OkCommand;
		KeyValues* m_CancelCommand;
		vgui::Frame* m_pFrameOver;
		bool m_bNoAutoClose : 1;
		bool m_bShowMessageBoxOverCursor : 1;
	};

}   


#endif  