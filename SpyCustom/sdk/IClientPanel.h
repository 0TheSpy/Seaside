#ifndef ICLIENTPANEL_H
#define ICLIENTPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"

#ifdef GetClassName
#undef GetClassName
#endif

class KeyValues;

namespace vgui
{

	class Panel;
	class SurfaceBase;

	enum EInterfaceID
	{
		ICLIENTPANEL_STANDARD_INTERFACE = 0,
	};

	class IClientPanel
	{
	public:
		virtual VPANEL GetVPanel() = 0;

		virtual void Think() = 0;
		virtual void PerformApplySchemeSettings() = 0;
		virtual void PaintTraverse(bool forceRepaint, bool allowForce) = 0;
		virtual void Repaint() = 0;
		virtual VPANEL IsWithinTraverse(int x, int y, bool traversePopups) = 0;
		virtual void GetInset(int& top, int& left, int& right, int& bottom) = 0;
		virtual void GetClipRect(int& x0, int& y0, int& x1, int& y1) = 0;
		virtual void OnChildAdded(VPANEL child) = 0;
		virtual void OnSizeChanged(int newWide, int newTall) = 0;

		virtual void InternalFocusChanged(bool lost) = 0;
		virtual bool RequestInfo(KeyValues* outputData) = 0;
		virtual void RequestFocus(int direction) = 0;
		virtual bool RequestFocusPrev(VPANEL existingPanel) = 0;
		virtual bool RequestFocusNext(VPANEL existingPanel) = 0;
		virtual void OnMessage(const KeyValues* params, VPANEL ifromPanel) = 0;
		virtual VPANEL GetCurrentKeyFocus() = 0;
		virtual int GetTabPosition() = 0;

		virtual const char* GetName() = 0;
		virtual const char* GetClassName() = 0;

		virtual HScheme GetScheme() = 0;
		virtual bool IsProportional() = 0;
		virtual bool IsAutoDeleteSet() = 0;
		virtual void DeletePanel() = 0;

		virtual void* QueryInterface(EInterfaceID id) = 0;

		virtual Panel* GetPanel() = 0;

		virtual const char* GetModuleName() = 0;

		virtual void OnTick() = 0;
	};

}   


#endif  