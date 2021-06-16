#ifndef VGUI_BUILDGROUP_H
#define VGUI_BUILDGROUP_H

#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "utlsymbol.h"
#include "vgui.h"
#include "Dar.h"
#include "Cursor.h"
#include "IScheme.h"
#include "Controls.h"
#include "PHandle.h"
#include "utlhandletable.h"

class KeyValues;

namespace vgui
{

	class BuildGroup
	{
		DECLARE_HANDLES(BuildGroup, 20);

	public:
		BuildGroup(Panel* parentPanel, Panel* contextPanel);
		~BuildGroup();

		virtual void SetEnabled(bool state);

		virtual bool IsEnabled();

		virtual Panel* GetCurrentPanel();

		virtual void LoadControlSettings(const char* controlResourceName, const char* pathID = NULL, KeyValues* pPreloadedKeyValues = NULL, KeyValues* pConditions = NULL);

		void ReloadControlSettings();

		void ChangeControlSettingsFile(const char* controlResourceName);

		virtual bool SaveControlSettings();

		virtual void ApplySettings(KeyValues* resourceData);

		virtual void GetSettings(KeyValues* resourceData);

		virtual void RemoveSettings();

		void GetNewFieldName(char* newFieldName, int newFieldNameSize, Panel* newPanel);

		Panel* FieldNameTaken(const char* fieldName);

		Panel* NewControl(KeyValues* controlKeys, int x = 0, int y = 0);
		Panel* NewControl(const char* name, int x = 0, int y = 0);

		virtual void SetContextPanel(Panel* contextPanel);

		virtual Panel* GetContextPanel();

		CUtlVector<PHandle>* GetPanelList();

		virtual const char* GetResourceName(void) { return m_pResourceName; }

		void PanelAdded(Panel* panel);
		void PanelRemoved(Panel* panel);

		virtual bool MousePressed(MouseCode code, Panel* panel);
		virtual bool MouseReleased(MouseCode code, Panel* panel);

		virtual CUtlVector<PHandle>* GetControlGroup();

		virtual void ToggleRulerDisplay();

		virtual void SetRulerLabelsVisible(bool state);

		virtual bool HasRulersOn();

		virtual void DrawRulers();

		void RegisterControlSettingsFile(const char* controlResourceName, const char* pathID = NULL);

		int GetRegisteredControlSettingsFileCount();
		const char* GetRegisteredControlSettingsFileByIndex(int index);

		KeyValues* GetDialogVariables();

		void ProcessConditionalKeys(KeyValues* pDat, KeyValues* pConditions);

	protected:
		virtual bool CursorMoved(int x, int y, Panel* panel);
		virtual bool MouseDoublePressed(MouseCode code, Panel* panel);
		virtual bool KeyCodeTyped(KeyCode code, Panel* panel);
		virtual bool KeyCodeReleased(KeyCode code, Panel* panel);
		virtual void ApplySchemeSettings(IScheme* pScheme);
		virtual bool KeyTyped(wchar_t unichar, Panel* panel);

		virtual HCursor GetCursor(Panel* panel);

	private:
		void ApplySnap(Panel* panel);
		Panel* CreateBuildDialog();
		void ActivateBuildDialog();
		void DeleteAllControlsCreatedByControlSettingsFile();

		bool      _enabled;
		int       _snapX;
		int       _snapY;
		HCursor   _cursor_sizenwse;
		HCursor   _cursor_sizenesw;
		HCursor   _cursor_sizewe;
		HCursor   _cursor_sizens;
		HCursor   _cursor_sizeall;
		bool      _dragging;
		MouseCode _dragMouseCode;
		int       _dragStartPanelPos[2];
		int       _dragStartCursorPos[2];
		int		  _dragStartPanelSize[2];
		Panel* _currentPanel;
		CUtlVector<PHandle> _panelDar;
		char* m_pResourceName;
		char* m_pResourcePathID;
		PHandle	 m_hBuildDialog;
		Panel* m_pBuildContext;               
		Panel* m_pParentPanel;         
		CUtlVector<PHandle> _controlGroup;   
		CUtlVector<int> _groupDeltaX;	             
		CUtlVector<int> _groupDeltaY;	             
		Label* _rulerNumber[4];        
		bool	_showRulers;	      
		CUtlVector<CUtlSymbol> m_RegisteredControlSettingsFiles;

		friend class Panel;
	};


	typedef CUtlHandle<BuildGroup> HBuildGroup;


}   

#endif  