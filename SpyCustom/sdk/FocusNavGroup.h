#ifndef FOCUSNAVGROUP_H
#define FOCUSNAVGROUP_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "PHandle.h"

namespace vgui
{

	class Panel;

	class FocusNavGroup
	{
	public:
		FocusNavGroup(Panel* panel);
		~FocusNavGroup();
		virtual Panel* GetDefaultPanel();	          

		virtual void SetDefaultButton(Panel* panel);	          
		virtual VPANEL GetDefaultButton();				                    
		virtual VPANEL GetCurrentDefaultButton();			        
		virtual Panel* FindPanelByHotkey(wchar_t key);		          
		virtual bool RequestFocusPrev(VPANEL panel = NULL);                  
		virtual bool RequestFocusNext(VPANEL panel = NULL);

		virtual Panel* GetCurrentFocus();
		virtual VPANEL SetCurrentFocus(VPANEL panel, VPANEL defaultPanel);      

		virtual void SetFocusTopLevel(bool state);

		virtual void SetCurrentDefaultButton(VPANEL panel, bool sendCurrentDefaultButtonMessage = true);
	private:
		bool CanButtonBeDefault(VPANEL panel);

		VPanelHandle _defaultButton;
		VPanelHandle _currentDefaultButton;
		VPanelHandle _currentFocus;

		Panel* _mainPanel;
		bool _topLevelFocus;
	};

}   

#endif  