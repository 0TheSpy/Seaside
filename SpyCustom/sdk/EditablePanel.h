#ifndef EDITABLEPANEL_H
#define EDITABLEPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "Panel.h"
#include "FocusNavGroup.h"

namespace vgui
{

	class EditablePanel : public Panel
	{
		DECLARE_CLASS_SIMPLE(EditablePanel, Panel);

	public:
		EditablePanel(Panel* parent, const char* panelName);
		EditablePanel(Panel* parent, const char* panelName, HScheme hScheme);

		virtual ~EditablePanel();

		virtual void LoadControlSettings(const char* dialogResourceName, const char* pathID = NULL, KeyValues* pPreloadedKeyValues = NULL, KeyValues* pConditions = NULL);
		virtual void ApplySettings(KeyValues* inResourceData);

		virtual void PerformLayout();

		virtual void LoadUserConfig(const char* configName, int dialogID = 0);
		virtual void SaveUserConfig();

		virtual void LoadControlSettingsAndUserConfig(const char* dialogResourceName, int dialogID = 0);

		virtual void ActivateBuildMode();

		virtual BuildGroup* GetBuildGroup();

		virtual Panel* CreateControlByName(const char* controlName);

		virtual void SetControlString(const char* controlName, const char* string);
		virtual void SetControlString(const char* controlName, const wchar_t* string);
		virtual void SetControlInt(const char* controlName, int state);
		virtual int GetControlInt(const char* controlName, int defaultState);
		virtual const char* GetControlString(const char* controlName, const char* defaultString = "");
		virtual void GetControlString(const char* controlName, char* buf, int bufSize, const char* defaultString = "");
		virtual void SetControlEnabled(const char* controlName, bool enabled);
		virtual void SetControlVisible(const char* controlName, bool visible);

		virtual void SetDialogVariable(const char* varName, const char* value);
		virtual void SetDialogVariable(const char* varName, const wchar_t* value);
		virtual void SetDialogVariable(const char* varName, int value);
		virtual void SetDialogVariable(const char* varName, float value);

		virtual void RequestFocus(int direction = 0);
		virtual bool RequestFocusNext(VPANEL panel);
		virtual bool RequestFocusPrev(VPANEL panel);
		virtual void OnSetFocus();
		virtual void OnRequestFocus(VPANEL subFocus, VPANEL defaultPanel);
		virtual VPANEL GetCurrentKeyFocus();
		virtual Panel* HasHotkey(wchar_t key);

		virtual void OnKeyCodePressed(KeyCode code);

		virtual bool RequestInfo(KeyValues* data);
		virtual void RegisterControlSettingsFile(const char* dialogResourceName, const char* pathID = NULL);

		KeyValues* GetDialogVariables();

	protected:
		virtual void PaintBackground();

		virtual FocusNavGroup& GetFocusNavGroup();

		MESSAGE_FUNC_HANDLE(OnDefaultButtonSet, "DefaultButtonSet", button);
		MESSAGE_FUNC_HANDLE(OnCurrentDefaultButtonSet, "CurrentDefaultButtonSet", button);
		MESSAGE_FUNC(OnFindDefaultButton, "FindDefaultButton");

		virtual void OnChildAdded(VPANEL child);
		virtual void OnSizeChanged(int wide, int tall);
		virtual void OnClose();

		virtual void ApplyUserConfigSettings(KeyValues* userConfig);

		virtual void GetUserConfigSettings(KeyValues* userConfig);

	private:
		void ForceSubPanelsToUpdateWithNewDialogVariables();

		BuildGroup* _buildGroup;
		FocusNavGroup m_NavGroup;
		KeyValues* m_pDialogVariables;

		char* m_pszConfigName;
		int m_iConfigID;
		bool m_bShouldSkipAutoResize;
	};

}   

#endif  