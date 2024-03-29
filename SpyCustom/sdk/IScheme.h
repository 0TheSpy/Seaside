#ifndef ISCHEME_H
#define ISCHEME_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "interface.h"

class Color;
class KeyValues;

namespace vgui
{

	typedef unsigned long HScheme;
	typedef unsigned long HTexture;

	class IBorder;
	class IImage;

	class IScheme : public IBaseInterface
	{
	public:
		virtual const char* GetResourceString(const char* stringName) = 0;

		virtual IBorder* GetBorder(const char* borderName) = 0;

		virtual HFont GetFont(const char* fontName, bool proportional = false) = 0;

		virtual char const* GetFontName(const HFont& font) = 0;

		virtual Color GetColor(const char* colorName, Color defaultColor) = 0;

		virtual int GetBorderCount() const = 0;

		virtual IBorder* GetBorderAtIndex(int iIndex) = 0;

		virtual int GetFontCount() const = 0;

		virtual HFont GetFontAtIndex(int iIndex) = 0;

		virtual const KeyValues* GetColorData() const = 0;
	};



	class ISchemeManager : public IBaseInterface
	{
	public:
		virtual HScheme LoadSchemeFromFile(const char* fileName, const char* tag) = 0;

		virtual void ReloadSchemes() = 0;

		virtual void ReloadFonts() = 0;

		virtual HScheme GetDefaultScheme() = 0;

		virtual HScheme GetScheme(const char* tag) = 0;

		virtual IImage* GetImage(const char* imageName, bool hardwareFiltered) = 0;
		virtual HTexture GetImageID(const char* imageName, bool hardwareFiltered) = 0;

		virtual IScheme* GetIScheme(HScheme scheme) = 0;

		virtual void Shutdown(bool full = true) = 0;

		virtual int GetProportionalScaledValue(int normalizedValue) = 0;
		virtual int GetProportionalNormalizedValue(int scaledValue) = 0;

		virtual HScheme LoadSchemeFromFileEx(VPANEL sizingPanel, const char* fileName, const char* tag) = 0;
		virtual int GetProportionalScaledValueEx(HScheme scheme, int normalizedValue) = 0;
		virtual int GetProportionalNormalizedValueEx(HScheme scheme, int scaledValue) = 0;

		virtual bool DeleteImage(const char* pImageName) = 0;
	};

#define VGUI_SCHEME_INTERFACE_VERSION "VGUI_Scheme010"


}   


#endif  