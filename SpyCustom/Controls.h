#ifndef CONTROLS_H
#define CONTROLS_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "IPanel.h"
#include "IKeyValuesSystem.h"

#include "interface.h"
#include "MouseCode.h"
#include "KeyCode.h"
#include "tier3.h"


namespace vgui
{

	bool VGui_InitInterfacesList(const char* moduleName, CreateInterfaceFn* factoryList, int numFactories);

	const char* GetControlsModuleName();

	class IPanel;
	class IInput;
	class ISchemeManager;
	class ISurface;
	class ISystem;
	class IVGui;

	class AnalogBar;
	class AnimatingImagePanel;
	class AnimationController;
	class BuildModeDialog;
	class Button;
	class CheckButton;
	class CheckButtonList;
	class CircularProgressBar;
	template< class T >class CvarToggleCheckButton;
	class ComboBox;
	class DirectorySelectDialog;
	class Divider;
	class EditablePanel;
	class FileOpenDialog;
	class Frame;
	class GraphPanel;
	class HTML;
	class ImagePanel;
	class Label;
	class ListPanel;
	class ListViewPanel;
	class Menu;
	class MenuBar;
	class MenuButton;
	class MenuItem;
	class MessageBox;
	class Panel;
	class PanelListPanel;
	class ProgressBar;
	class ProgressBox;
	class PropertyDialog;
	class PropertyPage;
	class PropertySheet;
	class QueryBox;
	class RadioButton;
	class RichText;
	class ScalableImagePanel;
	class ScrollBar;
	class ScrollBarSlider;
	class SectionedListPanel;
	class Slider;
	class Splitter;
	class TextEntry;
	class ToggleButton;
	class BaseTooltip;
	class TextTooltip;
	class TreeView;
	class CTreeViewListControl;
	class URLLabel;
	class WizardPanel;
	class WizardSubPanel;

	class BuildGroup;
	class FocusNavGroup;
	class IBorder;
	class IImage;
	class Image;
	class ImageList;
	class TextImage;

}   

#define VGUI_HOTKEYS_ENABLED
#define USING_BUILD_FACTORY( className )				\
	extern className *g_##className##LinkerHack;		\
	className *g_##className##PullInModule = g_##className##LinkerHack;

#define USING_BUILD_FACTORY_ALIAS( className, factoryName )				\
	extern className *g_##factoryName##LinkerHack;		\
	className *g_##factoryName##PullInModule = g_##factoryName##LinkerHack;

#endif  