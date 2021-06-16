#ifndef IVGUI_H
#define IVGUI_H

#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "VGUI.h"

#include "IAppSystem.h"

class KeyValues;

namespace vgui
{

	typedef unsigned long HPanel;
	typedef int HContext;

	enum
	{
		DEFAULT_VGUI_CONTEXT = ((vgui::HContext)~0)
	};

	typedef unsigned long HPanel;

	class IVGui : public IAppSystem
	{
	public:
		virtual void Start() = 0;

		virtual void Stop() = 0;

		virtual bool IsRunning() = 0;

		virtual void RunFrame() = 0;

		virtual void ShutdownMessage(unsigned int shutdownID) = 0;

		virtual VPANEL AllocPanel() = 0;
		virtual void FreePanel(VPANEL panel) = 0;

		virtual void DPrintf(PRINTF_FORMAT_STRING const char* format, ...) = 0;
		virtual void DPrintf2(PRINTF_FORMAT_STRING const char* format, ...) = 0;
		virtual void SpewAllActivePanelNames() = 0;

		virtual HPanel PanelToHandle(VPANEL panel) = 0;
		virtual VPANEL HandleToPanel(HPanel index) = 0;
		virtual void MarkPanelForDeletion(VPANEL panel) = 0;

		virtual void AddTickSignal(VPANEL panel, int intervalMilliseconds = 0) = 0;
		virtual void RemoveTickSignal(VPANEL panel) = 0;

		virtual void PostMessage(VPANEL target, KeyValues* params, VPANEL from, float delaySeconds = 0.0f) = 0;

		virtual HContext CreateContext() = 0;
		virtual void DestroyContext(HContext context) = 0;

		virtual void AssociatePanelWithContext(HContext context, VPANEL pRoot) = 0;

		virtual void ActivateContext(HContext context) = 0;

		virtual void SetSleep(bool state) = 0;

		virtual bool GetShouldVGuiControlSleep() = 0;

		virtual void SetVRMode(bool bVRMode) = 0;
		virtual bool GetVRMode() = 0;

		virtual void AddTickSignalToHead(VPANEL panel, int intervalMilliseconds = 0) = 0;
	};

#define VGUI_IVGUI_INTERFACE_VERSION "VGUI_ivgui008"

};


#endif  