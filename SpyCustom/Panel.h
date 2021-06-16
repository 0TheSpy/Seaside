#ifndef PANEL_H
#define PANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "utlflags.h"
#include "vgui.h"
#include "Dar.h"
#include "MessageMap.h"
#if defined( VGUI_USEKEYBINDINGMAPS )
#include "KeyBindingMap.h"
#endif
#include "IClientPanel.h"
#include "IScheme.h"
#include "Controls.h"
#include "PHandle.h"
#include "PanelAnimationVar.h"
#include "color.h"
#include "keyvalues.h"
#include "ikeyvaluessystem.h"
#include "utlsymbol.h"
#include "BuildGroup.h"
#include "dmxelement.h"

#ifdef PostMessage
#undef PostMessage
#endif

#ifdef SetCursor
#undef SetCursor
#endif


class CUtlBuffer;
struct DmxElementUnpackStructure_t;

namespace vgui
{

#if !defined( _GAMECONSOLE )
#define VGUI_USEDRAGDROP 1
#endif

#if defined( VGUI_USEKEYBINDINGMAPS )
	struct PanelKeyBindingMap;
#endif
	template< class T >
	inline T* SETUP_PANEL(T* panel)
	{
		panel->MakeReadyForUse();
		return panel;
	}

#define CREATE_PANEL(type, parent, name) (SETUP_PANEL(new type(parent, name)))

#if defined( VGUI_USEDRAGDROP )
	struct DragDrop_t;
	class Menu;
#endif



	class Panel;

	struct SizerAddArgs_t
	{
		SizerAddArgs_t()
		{
			m_flExpandFactor = 0.0f;
			m_nPadding = 5;
			m_bMinorExpand = true;
			m_nMinX = -1;
			m_nMinY = -1;
			m_bIgnoreMemberMin = false;
		}

		SizerAddArgs_t& Expand(float flExpandFactor) { m_flExpandFactor = flExpandFactor; return *this; }
		SizerAddArgs_t& Padding(int nPadding) { m_nPadding = nPadding; return *this; }
		SizerAddArgs_t& MinorExpand(bool bMinorExpand) { m_bMinorExpand = bMinorExpand; return *this; }
		SizerAddArgs_t& MinSize(int nMinX, int nMinY) { m_nMinX = nMinX; m_nMinY = nMinY; return *this; }
		SizerAddArgs_t& MinX(int nMinX) { m_nMinX = nMinX; return *this; }
		SizerAddArgs_t& MinY(int nMinY) { m_nMinY = nMinY; return *this; }

		SizerAddArgs_t& IgnoreMemberMin(bool bIgnoreMemberMin = true) { m_bIgnoreMemberMin = bIgnoreMemberMin; return *this; }

		SizerAddArgs_t& FixedSize(int nX, int nY)
		{
			IgnoreMemberMin(true);
			MinSize(nX, nY);
			Expand(0.f);
			MinorExpand(false);
			return *this;
		}

		float m_flExpandFactor;
		int m_nPadding;
		bool m_bMinorExpand;
		int m_nMinX;
		int m_nMinY;
		bool m_bIgnoreMemberMin;
	};


	enum SizerLayoutDirection_t
	{
		ESLD_HORIZONTAL,     
		ESLD_VERTICAL	     
	};

	enum SizerElementType_t
	{
		ESET_SIZER,
		ESET_PANEL,
		ESET_SPACER,
	};

	class CSizerBase
	{
	public:
		CSizerBase();
		virtual ~CSizerBase();

		int GetElementCount() { return m_Members.Count(); }
		SizerElementType_t GetElementType(int i);
		Panel* GetPanel(int i);

		void SetElementArgs(int nIndex, const SizerAddArgs_t& args) { m_Members[nIndex].Fill(args); }

		void InsertPanel(int nIndex, Panel* pPanel, const SizerAddArgs_t& args);
		void InsertSizer(int nIndex, CSizerBase* pSizer, const SizerAddArgs_t& args);
		void InsertSpacer(int nIndex, const SizerAddArgs_t& args);

		void AddPanel(Panel* pPanel, const SizerAddArgs_t& args) { InsertPanel(GetElementCount(), pPanel, args); }
		void AddSizer(CSizerBase* pSizer, const SizerAddArgs_t& args) { InsertSizer(GetElementCount(), pSizer, args); }
		void AddSpacer(const SizerAddArgs_t& args) { InsertSpacer(GetElementCount(), args); }

		void RemoveElement(int i, bool bDelete);
		void RemoveAllMembers(bool bDelete);

		void GetMinSize(int& OutX, int& OutY);

		void RecursiveInvalidateCachedSize();

		virtual void DoLayout(int BaseX, int BaseY, int SizeX, int SizeY) = 0;
		virtual void CalculateSize() = 0;

	protected:
		class CSizerMember
		{
			friend class CSizerBase;         

		public:
			SizerElementType_t GetElementType() const;
			Panel* GetPanel() const;

			void GetMemberMinSize(int& OutX, int& OutY);
			void RecursiveInvalidateCachedSize();
			void Place(int BaseX, int BaseY, int SizeX, int SizeY);

			float GetExpandFactor() { return m_flExpandFactor; }
			bool GetMinorExpand() { return m_bMinorExpand; }

			void DiscardOwnedSizer();

			bool IsVisible();

			void Fill(const SizerAddArgs_t& args);

		private:
			void RecursiveRemove(bool bDelete);

			Panel* m_pPanel;
			CSizerBase* m_pSizer;

			int m_nPadding;              
			float m_flExpandFactor;
			bool m_bMinorExpand;
			bool m_bIgnoreMemberMin;
			int m_nMinX;
			int m_nMinY;
		};

		CUtlVector<CSizerMember> m_Members;
		int m_nMinXSize;
		int m_nMinYSize;
	};

	inline int SizerMajorAxis(SizerLayoutDirection_t Dir, int X, int Y) { return (Dir == ESLD_HORIZONTAL) ? X : Y; }
	inline int SizerMinorAxis(SizerLayoutDirection_t Dir, int X, int Y) { return (Dir == ESLD_VERTICAL) ? X : Y; }
	inline int SizerXAxis(SizerLayoutDirection_t Dir, int MajorAxis, int MinorAxis) { return (Dir == ESLD_HORIZONTAL) ? MajorAxis : MinorAxis; }
	inline int SizerYAxis(SizerLayoutDirection_t Dir, int MajorAxis, int MinorAxis) { return (Dir == ESLD_VERTICAL) ? MajorAxis : MinorAxis; }

	class CBoxSizer : public CSizerBase
	{
	public:
		CBoxSizer(SizerLayoutDirection_t LayoutDirection);

		virtual void CalculateSize();
		virtual void DoLayout(int BaseX, int BaseY, int SizeX, int SizeY);

	protected:
		SizerLayoutDirection_t m_LayoutDirection;
	};


	struct OverridableColorEntry
	{
		char const* name() { return m_pszScriptName; }

		char const* m_pszScriptName;
		Color* m_pColor;
		Color		m_colFromScript;
		UtlSymId_t	m_sColorNameFromScript;
		bool		m_bOverridden;
	};

#define REGISTER_COLOR_AS_OVERRIDABLE( name, scriptname )			\
	AddToOverridableColors( &name, scriptname );


#define DECLARE_VGUI_UNPACK()	\
	DECLARE_DMXELEMENT_UNPACK()	\
	private: \
		static DmxElementUnpackStructure_t *s_pUnpackParams; \
	public:	 \
		virtual const DmxElementUnpackStructure_t* GetUnpackStructure() const { return s_pUnpackParams; }

#define DECLARE_VGUI_UNPACK_NAMESPACE( _namespace ) \
	template <typename T> friend DmxElementUnpackStructure_t *DmxElementUnpackInit##_namespace(T *); \
	private: \
		static DmxElementUnpackStructure_t *s_pUnpackParams; \
	public:	 \
		virtual const DmxElementUnpackStructure_t* GetUnpackStructure() const { return s_pUnpackParams; }

#define BEGIN_VGUI_UNPACK( _structName ) BEGIN_DMXELEMENT_UNPACK( _structName )
#define END_VGUI_UNPACK( _structName ) \
	END_DMXELEMENT_UNPACK( _structName, s_pUnpackParams ) \
 	DmxElementUnpackStructure_t *_structName::s_pUnpackParams = _structName##_UnpackInit::s_pUnpack; 

#define BEGIN_VGUI_UNPACK_NAMESPACE( _nameSpace, _structName ) BEGIN_DMXELEMENT_UNPACK_NAMESPACE( _nameSpace, _structName )
#define END_VGUI_UNPACK_NAMESPACE( _nameSpace, _structName ) \
	END_DMXELEMENT_UNPACK_NAMESPACE( _nameSpace, _structName, s_pUnpackParams ) \
 	DmxElementUnpackStructure_t *_structName::s_pUnpackParams = _namespace##_structName##_UnpackInit::s_pUnpack; 


	class IPanelAnimationPropertyConverter
	{
	public:
		virtual void GetData(Panel* panel, KeyValues* kv, PanelAnimationMapEntry* entry) = 0;
		virtual void SetData(Panel* panel, KeyValues* kv, PanelAnimationMapEntry* entry) = 0;
		virtual void InitFromDefault(Panel* panel, PanelAnimationMapEntry* entry) = 0;
	};

#if defined( VGUI_USEKEYBINDINGMAPS )
	enum KeyBindingContextHandle_t
	{
		INVALID_KEYBINDINGCONTEXT_HANDLE = 0xffffffff,
	};
#endif

#define PANEL_ROUND_CORNER_TOP_LEFT		(1 << 0)
#define PANEL_ROUND_CORNER_TOP_RIGHT	(1 << 1)
#define PANEL_ROUND_CORNER_BOTTOM_LEFT	(1 << 2)
#define PANEL_ROUND_CORNER_BOTTOM_RIGHT (1 << 3)
#define PANEL_ROUND_CORNER_ALL			PANEL_ROUND_CORNER_TOP_LEFT | PANEL_ROUND_CORNER_TOP_RIGHT | PANEL_ROUND_CORNER_BOTTOM_LEFT | PANEL_ROUND_CORNER_BOTTOM_RIGHT
	class Panel : public IClientPanel
	{
		DECLARE_CLASS_SIMPLE_NOBASE(Panel);
		DECLARE_DMXELEMENT_UNPACK_NAMESPACE(vgui);

	public:
		static void InitPropertyConverters(void);
		static void AddPropertyConverter(char const* typeName, IPanelAnimationPropertyConverter* converter);

		Panel();
		Panel(Panel* parent);
		Panel(Panel* parent, const char* panelName);
		Panel(Panel* parent, const char* panelName, HScheme scheme);

		virtual ~Panel();

		virtual VPANEL GetVPanel() { return _vpanel; }
		HPanel ToHandle() const;


		void SetName(const char* panelName);             
		const char* GetName();		        
		const char* GetClassName();             

		void MakeReadyForUse();                      

		void SetPos(int x, int y);		            
		void GetPos(int& x, int& y);		     
		void SetSize(int wide, int tall);	    
		void GetSize(int& wide, int& tall);	    
		void SetBounds(int x, int y, int wide, int tall);		   
		void GetBounds(int& x, int& y, int& wide, int& tall);	   
		int  GetWide();	    
		void SetWide(int wide);	    
		int  GetTall();	    
		void SetTall(int tall);	    
		void SetMinimumSize(int wide, int tall);		        
		void GetMinimumSize(int& wide, int& tall);	    
		bool IsBuildModeEditable();	      
		void SetBuildModeEditable(bool state);     
		bool IsBuildModeDeletable();      
		void SetBuildModeDeletable(bool state);	   
		bool IsBuildModeActive();	       
		void SetZPos(int z);	           
		int  GetZPos(void);
		void SetAlpha(int alpha);	          
		int GetAlpha();	    

		virtual void SetVisible(bool state);
		virtual bool IsVisible();
		virtual bool IsFullyVisible();		      

		virtual VPANEL IsWithinTraverse(int x, int y, bool traversePopups);	          
		MESSAGE_FUNC(Repaint, "Repaint");							        
		virtual void PostMessage(VPANEL target, KeyValues* message, float delaySeconds = 0.0f);

		bool IsWithin(int x, int y);   
		void LocalToScreen(int& x, int& y);
		void ScreenToLocal(int& x, int& y);
		void ParentLocalToScreen(int& x, int& y);
		void MakePopup(bool showTaskbarIcon = true, bool disabled = false);		               
		virtual void OnMove();

		virtual Panel* GetParent();
		virtual VPANEL GetVParent();
		virtual void SetParent(Panel* newParent);
		virtual void SetParent(VPANEL newParent);
		virtual bool HasParent(VPANEL potentialParent);

		int GetChildCount();
		Panel* GetChild(int index);
		int FindChildIndexByName(const char* childName);
		Panel* FindChildByName(const char* childName, bool recurseDown = false);
		Panel* FindSiblingByName(const char* siblingName);
		void CallParentFunction(KeyValues* message);

		virtual bool LookupElementBounds(const char* elementName, int& x, int& y, int& wide, int& tall) { return false; }

		virtual void SetAutoDelete(bool state);		            
		virtual bool IsAutoDeleteSet();
		virtual void DeletePanel();				        

		virtual void AddActionSignalTarget(Panel* messageTarget);
		virtual void AddActionSignalTarget(VPANEL messageTarget);
		virtual void RemoveActionSignalTarget(Panel* oldTarget);
		virtual void PostActionSignal(KeyValues* message);			       
		virtual bool RequestInfoFromChild(const char* childName, KeyValues* outputData);
		virtual void PostMessageToChild(const char* childName, KeyValues* messsage);
		virtual void PostMessage(Panel* target, KeyValues* message, float delaySeconds = 0.0f);
		virtual bool RequestInfo(KeyValues* outputData);				                       
		virtual bool SetInfo(KeyValues* inputData);						            
		virtual void SetSilentMode(bool bSilent);						              

		virtual void InstallMouseHandler(Panel* pHandler);	            

		virtual void   SetEnabled(bool state);
		virtual bool   IsEnabled();
		virtual bool   IsPopup();	         
		virtual void   GetClipRect(int& x0, int& y0, int& x1, int& y1);
		virtual void   MoveToFront();

		enum PinCorner_e
		{
			PIN_TOPLEFT = 0,
			PIN_TOPRIGHT,
			PIN_BOTTOMLEFT,
			PIN_BOTTOMRIGHT,
			PIN_NO,

			PIN_CENTER_TOP,
			PIN_CENTER_RIGHT,
			PIN_CENTER_BOTTOM,
			PIN_CENTER_LEFT,
		};

		enum AutoResize_e
		{
			AUTORESIZE_NO = 0,
			AUTORESIZE_RIGHT,
			AUTORESIZE_DOWN,
			AUTORESIZE_DOWNANDRIGHT,
		};

		void SetPinCorner(PinCorner_e pinCorner, int nOffsetX, int nOffsetY);

		void SetAutoResize(PinCorner_e pinCorner, AutoResize_e resizeDir, int nPinOffsetX, int nPinOffsetY, int nUnpinnedCornerOffsetX, int nUnpinnedCornerOffsetY);

		AutoResize_e GetAutoResize();
		PinCorner_e GetPinCorner();

		void GetPinOffset(int& dx, int& dy);
		void GetResizeOffset(int& dx, int& dy);

		void PinToSibling(const char* pszSibling, PinCorner_e pinOurCorner, PinCorner_e pinSibling);
		void UpdateSiblingPin(void);

		virtual void SetBgColor(Color color);
		virtual void SetFgColor(Color color);
		virtual Color GetBgColor();
		virtual Color GetFgColor();

		virtual void SetCursor(HCursor cursor);
		virtual HCursor GetCursor();
		virtual void RequestFocus(int direction = 0);
		virtual bool HasFocus();
		virtual void InvalidateLayout(bool layoutNow = false, bool reloadScheme = false);
		virtual bool RequestFocusPrev(VPANEL panel = NULL);
		virtual bool RequestFocusNext(VPANEL panel = NULL);
		virtual void   SetTabPosition(int position);
		virtual int    GetTabPosition();
		virtual void SetBorder(IBorder* border);
		virtual IBorder* GetBorder();
		virtual void SetPaintBorderEnabled(bool state);
		virtual void SetPaintBackgroundEnabled(bool state);
		virtual void SetPaintEnabled(bool state);
		virtual void SetPostChildPaintEnabled(bool state);
		virtual void SetPaintBackgroundType(int type);                    
		virtual void GetInset(int& left, int& top, int& right, int& bottom);
		virtual void GetPaintSize(int& wide, int& tall);
		virtual void SetBuildGroup(BuildGroup* buildGroup);
		virtual bool IsBuildGroupEnabled();
		virtual bool IsCursorNone();
		virtual bool IsCursorOver();		          
		virtual void MarkForDeletion();		       
		virtual bool IsLayoutInvalid();		       
		virtual Panel* HasHotkey(wchar_t key);			       
		virtual bool IsOpaque();
		bool IsRightAligned();		             
		bool IsBottomAligned();		             
		bool IsPercentage();		           

		virtual HScheme GetScheme();
		virtual void SetScheme(const char* tag);
		virtual void SetScheme(HScheme scheme);
		virtual Color GetSchemeColor(const char* keyName, IScheme* pScheme);
		virtual Color GetSchemeColor(const char* keyName, Color defaultColor, IScheme* pScheme);

		virtual void ApplySchemeSettings(IScheme* pScheme);

		virtual void ApplySettings(KeyValues* inResourceData);
		virtual void OnUnserialized(CDmxElement* pElement);

		virtual void GetSettings(KeyValues* outResourceData);

		virtual const char* GetDescription();

		virtual const char* GetModuleName();

		virtual void ApplyUserConfigSettings(KeyValues* userConfig);

		virtual void GetUserConfigSettings(KeyValues* userConfig);

		virtual bool HasUserConfigSettings();

		virtual void OnMessage(const KeyValues* params, VPANEL fromPanel);	        
		MESSAGE_FUNC_CHARPTR(OnCommand, "Command", command);	       
		MESSAGE_FUNC(OnMouseCaptureLost, "MouseCaptureLost");	       
		MESSAGE_FUNC(OnSetFocus, "SetFocus");			        
		MESSAGE_FUNC(OnKillFocus, "KillFocus");		        
		MESSAGE_FUNC(OnDelete, "Delete");				            
		virtual void OnThink();							           
		virtual void OnChildAdded(VPANEL child);		          
		virtual void OnSizeChanged(int newWide, int newTall);	          

		virtual void OnTick();

		MESSAGE_FUNC_INT_INT(OnCursorMoved, "OnCursorMoved", x, y);
		virtual void OnCursorEntered();
		virtual void OnCursorExited();
		virtual void OnMousePressed(MouseCode code);
		virtual void OnMouseDoublePressed(MouseCode code);
		virtual void OnMouseReleased(MouseCode code);
		virtual void OnMouseWheeled(int delta);

		virtual void SetTriplePressAllowed(bool state);
		virtual bool IsTriplePressAllowed() const;
		virtual void OnMouseTriplePressed(MouseCode code);

		static char const* KeyCodeToString(KeyCode code);
		static wchar_t const* KeyCodeToDisplayString(KeyCode code);
		static wchar_t const* KeyCodeModifiersToDisplayString(KeyCode code, int modifiers);  

		static KeyCode		StringToKeyCode(char const* str);
#if defined( VGUI_USEKEYBINDINGMAPS )
		static KeyBindingContextHandle_t   CreateKeyBindingsContext(char const* filename, char const* pathID = 0);
		virtual void		SetKeyBindingsContext(KeyBindingContextHandle_t handle);
		virtual KeyBindingContextHandle_t	GetKeyBindingsContext() const;
		virtual bool		IsValidKeyBindingsContext() const;

		static int			GetPanelsWithKeyBindingsCount(KeyBindingContextHandle_t handle);
		static Panel* GetPanelWithKeyBindings(KeyBindingContextHandle_t handle, int index);

		static void			RevertKeyBindings(KeyBindingContextHandle_t handle);

		static void			ReloadKeyBindings(KeyBindingContextHandle_t handle);
		static void			SaveKeyBindings(KeyBindingContextHandle_t handle);
		static void			SaveKeyBindingsToFile(KeyBindingContextHandle_t handle, char const* filename, char const* pathID = 0);
		static void			LoadKeyBindings(KeyBindingContextHandle_t handle);
		static void			LoadKeyBindingsForOnePanel(KeyBindingContextHandle_t handle, Panel* panelOfInterest);

		virtual bool		IsKeyRebound(KeyCode code, int modifiers);
		virtual bool		IsKeyOverridden(KeyCode code, int modifiers);

		virtual void		AddKeyBinding(char const* bindingName, int keycode, int modifiers);

		KeyBindingMap_t* LookupBinding(char const* bindingName);
		KeyBindingMap_t* LookupBindingByKeyCode(KeyCode code, int modifiers);
		void				LookupBoundKeys(char const* bindingName, CUtlVector< BoundKey_t* >& list);
		BoundKey_t* LookupDefaultKey(char const* bindingName);
		PanelKeyBindingMap* LookupMapForBinding(char const* bindingName);

		int				GetKeyMappingCount();

		void			RevertKeyBindingsToDefault();
		void			RemoveAllKeyBindings();
		void			ReloadKeyBindings();
		virtual void	EditKeyBindings();

		void			SaveKeyBindingsToBuffer(int level, CUtlBuffer& buf);
		bool			ParseKeyBindings(KeyValues* kv);

		virtual char const* GetKeyBindingsFile() const;
		virtual char const* GetKeyBindingsFilePathID() const;

		void			SetAllowKeyBindingChainToParent(bool state);
		bool			IsKeyBindingChainToParentAllowed() const;
#endif  

		virtual void OnKeyCodePressed(KeyCode code);
		virtual void OnKeyCodeTyped(KeyCode code);
		virtual void OnKeyTyped(wchar_t unichar);
		virtual void OnKeyCodeReleased(KeyCode code);
		virtual void OnKeyFocusTicked();       

		MESSAGE_FUNC(OnMouseFocusTicked, "OnMouseFocusTicked");

		virtual void PaintBackground();
		virtual void Paint();
		virtual void PaintBorder();
		virtual void PaintBuildOverlay();		        
		virtual void PostChildPaint();
		virtual void PerformLayout();

		DECLARE_PANELMAP();

		virtual VPANEL GetCurrentKeyFocus();

		BaseTooltip* GetTooltip();
		void	SetTooltip(BaseTooltip* pToolTip, const char* pszText);

		virtual bool IsProportional() { return _flags.IsFlagSet(IS_PROPORTIONAL); }
		virtual void SetProportional(bool state);

		virtual void SetMouseInputEnabled(bool state);
		virtual void SetKeyBoardInputEnabled(bool state);
		virtual bool IsMouseInputEnabled();
		virtual bool IsKeyBoardInputEnabled();

		void		DisableMouseInputForThisPanel(bool bDisable);
		bool		IsMouseInputDisabledForThisPanel() const;

		virtual void DrawTexturedBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha);
		virtual void DrawBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha, bool hollow = false);
		virtual void DrawBoxFade(int x, int y, int wide, int tall, Color color, float normalizedAlpha, unsigned int alpha0, unsigned int alpha1, bool bHorizontal, bool hollow = false);
		virtual void DrawHollowBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha);
		virtual void DrawHollowBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha, int cornerWide, int cornerTall);

		unsigned char GetRoundedCorners() { return m_roundedCorners; }
		void SetRoundedCorners(unsigned char cornerFlags) { m_roundedCorners = cornerFlags; }
		bool ShouldDrawTopLeftCornerRounded() { return 0 != (m_roundedCorners & PANEL_ROUND_CORNER_TOP_LEFT); }
		bool ShouldDrawTopRightCornerRounded() { return 0 != (m_roundedCorners & PANEL_ROUND_CORNER_TOP_RIGHT); }
		bool ShouldDrawBottomLeftCornerRounded() { return 0 != (m_roundedCorners & PANEL_ROUND_CORNER_BOTTOM_LEFT); }
		bool ShouldDrawBottomRightCornerRounded() { return 0 != (m_roundedCorners & PANEL_ROUND_CORNER_BOTTOM_RIGHT); }

		virtual void SetDragEnabled(bool enabled);
		virtual bool IsDragEnabled() const;

		virtual void SetShowDragHelper(bool enabled);

		virtual void OnDragFailed(CUtlVector< KeyValues* >& msglist);

		virtual void SetBlockDragChaining(bool block);
		virtual bool IsBlockingDragChaining() const;

		virtual int GetDragStartTolerance() const;
		virtual void SetDragSTartTolerance(int nTolerance);

		virtual void SetDropEnabled(bool enabled, float m_flHoverContextTime = 0.0f);
		virtual bool IsDropEnabled() const;

		virtual bool GetDropContextMenu(Menu* menu, CUtlVector< KeyValues* >& msglist);
		virtual void OnDropContextHoverShow(CUtlVector< KeyValues* >& msglist);
		virtual void OnDropContextHoverHide(CUtlVector< KeyValues* >& msglist);

#if defined( VGUI_USEDRAGDROP )
		virtual DragDrop_t* GetDragDropInfo();
#endif
		virtual void OnGetAdditionalDragPanels(CUtlVector< Panel* >& dragabbles);

		virtual void OnCreateDragData(KeyValues* msg);
		virtual bool IsDroppable(CUtlVector< KeyValues* >& msglist);

		virtual void OnDraggablePanelPaint();
		virtual void OnDroppablePanelPaint(CUtlVector< KeyValues* >& msglist, CUtlVector< Panel* >& dragPanels);

		virtual void OnPanelDropped(CUtlVector< KeyValues* >& msglist);

		virtual void OnPanelEnteredDroppablePanel(CUtlVector< KeyValues* >& msglist);
		virtual void OnPanelExitedDroppablePanel(CUtlVector< KeyValues* >& msglist);

		virtual Panel* GetDropTarget(CUtlVector< KeyValues* >& msglist);
		virtual Panel* GetDragPanel();
		virtual bool	IsBeingDragged();
		virtual HCursor GetDropCursor(CUtlVector< KeyValues* >& msglist);
		virtual HCursor GetDragFailCursor(CUtlVector< KeyValues* >& msglist) { return dc_no; }

		Color GetDropFrameColor();
		Color GetDragFrameColor();

		virtual bool	CanStartDragging(int startx, int starty, int mx, int my);

		virtual void FillRectSkippingPanel(const Color clr, int x, int y, int w, int h, Panel* skipPanel);

		virtual int	GetPaintBackgroundType();
		virtual void GetCornerTextureSize(int& w, int& h);

		bool		IsChildOfModalSubTree();
		bool		IsChildOfSurfaceModalPanel();

		bool		ShouldHandleInputMessage();

		virtual void SetSkipChildDuringPainting(Panel* child);

		void		SetStartDragWhenMouseExitsPanel(bool state);
		bool		IsStartDragWhenMouseExitsPanel() const;

		void		SetMessageContextId_R(int nContextID);

		void		PostMessageToAllSiblings(KeyValues* msg, float delaySeconds = 0.0f);
		template< class S >
		void		PostMessageToAllSiblingsOfType(KeyValues* msg, float delaySeconds = 0.0f);

		void		SetConsoleStylePanel(bool bConsoleStyle);
		bool		IsConsoleStylePanel() const;

		enum NAV_DIRECTION { ND_UP, ND_DOWN, ND_LEFT, ND_RIGHT, ND_BACK, ND_NONE };
		virtual Panel* NavigateUp();
		virtual Panel* NavigateDown();
		virtual Panel* NavigateLeft();
		virtual Panel* NavigateRight();
		virtual void NavigateTo();
		virtual void NavigateFrom();
		virtual void NavigateToChild(Panel* pNavigateTo);  

		Panel* SetNavUp(Panel* navUp);
		Panel* SetNavDown(Panel* navDown);
		Panel* SetNavLeft(Panel* navLeft);
		Panel* SetNavRight(Panel* navRight);
		NAV_DIRECTION GetLastNavDirection();
		MESSAGE_FUNC_CHARPTR(OnNavigateTo, "OnNavigateTo", panelName);
		MESSAGE_FUNC_CHARPTR(OnNavigateFrom, "OnNavigateFrom", panelName);

	protected:

		virtual void OnStartDragging();
		virtual void OnContinueDragging();
		virtual void OnFinishDragging(bool mousereleased, MouseCode code, bool aborted = false);

		virtual void DragDropStartDragging();

		virtual void GetDragData(CUtlVector< KeyValues* >& list);
		virtual void CreateDragData();

		virtual void PaintTraverse(bool Repaint, bool allowForce = true);

	protected:
		MESSAGE_FUNC_ENUM_ENUM(OnRequestFocus, "OnRequestFocus", VPANEL, subFocus, VPANEL, defaultPanel);
		MESSAGE_FUNC_INT_INT(OnScreenSizeChanged, "OnScreenSizeChanged", oldwide, oldtall);
		virtual void* QueryInterface(EInterfaceID id);

		void AddToOverridableColors(Color* pColor, char const* scriptname)
		{
			int iIdx = m_OverridableColorEntries.AddToTail();
			m_OverridableColorEntries[iIdx].m_pszScriptName = scriptname;
			m_OverridableColorEntries[iIdx].m_pColor = pColor;
			m_OverridableColorEntries[iIdx].m_bOverridden = false;
		}

		void ApplyOverridableColors(IScheme* pScheme);
		void SetOverridableColor(Color* pColor, const Color& newColor);

	protected:
		void SetNavUp(const char* controlName);
		void SetNavDown(const char* controlName);
		void SetNavLeft(const char* controlName);
		void SetNavRight(const char* controlName);

	public:
		Panel* GetNavUp(Panel* first = NULL);
		Panel* GetNavDown(Panel* first = NULL);
		Panel* GetNavLeft(Panel* first = NULL);
		Panel* GetNavRight(Panel* first = NULL);

		inline void SetWorldPositionCurrentFrame(bool bWorldPositionCurrentFrame) { m_bWorldPositionCurrentFrame = bWorldPositionCurrentFrame; }
		inline bool GetWorldPositionCurrentFrame() { return m_bWorldPositionCurrentFrame; }

	protected:
		Panel* GetNavUpPanel();
		Panel* GetNavDownPanel();
		Panel* GetNavLeftPanel();
		Panel* GetNavRightPanel();

		bool m_PassUnhandledInput;
		NAV_DIRECTION m_LastNavDirection;

		void InternalInitDefaultValues(PanelAnimationMap* map);


	private:
		enum BuildModeFlags_t
		{
			BUILDMODE_EDITABLE = 0x01,
			BUILDMODE_DELETABLE = 0x02,
			BUILDMODE_SAVE_XPOS_RIGHTALIGNED = 0x04,
			BUILDMODE_SAVE_XPOS_CENTERALIGNED = 0x08,
			BUILDMODE_SAVE_YPOS_BOTTOMALIGNED = 0x10,
			BUILDMODE_SAVE_YPOS_CENTERALIGNED = 0x20,
			BUILDMODE_SAVE_WIDE_FULL = 0x40,
			BUILDMODE_SAVE_TALL_FULL = 0x80,
			BUILDMODE_SAVE_PROPORTIONAL_TO_PARENT = 0x100,
			BUILDMODE_SAVE_PERCENTAGE = 0x200,
		};

		enum PanelFlags_t
		{
			MARKED_FOR_DELETION = 0x0001,
			NEEDS_REPAINT = 0x0002,
			PAINT_BORDER_ENABLED = 0x0004,
			PAINT_BACKGROUND_ENABLED = 0x0008,
			PAINT_ENABLED = 0x0010,
			POST_CHILD_PAINT_ENABLED = 0x0020,
			AUTODELETE_ENABLED = 0x0040,
			NEEDS_LAYOUT = 0x0080,
			NEEDS_SCHEME_UPDATE = 0x0100,
			NEEDS_DEFAULT_SETTINGS_APPLIED = 0x0200,
#if defined( VGUI_USEKEYBINDINGMAPS )
			ALLOW_CHAIN_KEYBINDING_TO_PARENT = 0x0400,
#endif
			IN_PERFORM_LAYOUT = 0x0800,
			IS_PROPORTIONAL = 0x1000,
			TRIPLE_PRESS_ALLOWED = 0x2000,
			DRAG_REQUIRES_PANEL_EXIT = 0x4000,
			IS_MOUSE_DISABLED_FOR_THIS_PANEL_ONLY = 0x8000,
			ALL_FLAGS = 0xFFFF,
		};

		virtual Panel* GetPanel() { return this; }

		void Think();
		void PerformApplySchemeSettings();

		void InternalPerformLayout();
		void InternalSetCursor();

		MESSAGE_FUNC_INT_INT(InternalCursorMoved, "CursorMoved", xpos, ypos);
		MESSAGE_FUNC(InternalCursorEntered, "CursorEntered");
		MESSAGE_FUNC(InternalCursorExited, "CursorExited");

		MESSAGE_FUNC_INT(InternalMousePressed, "MousePressed", code);
		MESSAGE_FUNC_INT(InternalMouseDoublePressed, "MouseDoublePressed", code);
		MESSAGE_FUNC_INT(InternalMouseTriplePressed, "MouseTriplePressed", code);
		MESSAGE_FUNC_INT(InternalMouseReleased, "MouseReleased", code);
		MESSAGE_FUNC_INT(InternalMouseWheeled, "MouseWheeled", delta);
		MESSAGE_FUNC_INT(InternalKeyCodePressed, "KeyCodePressed", code);
		MESSAGE_FUNC_INT(InternalKeyCodeTyped, "KeyCodeTyped", code);
		MESSAGE_FUNC_INT(InternalKeyTyped, "KeyTyped", unichar);
		MESSAGE_FUNC_INT(InternalKeyCodeReleased, "KeyCodeReleased", code);

		MESSAGE_FUNC(InternalKeyFocusTicked, "KeyFocusTicked");
		MESSAGE_FUNC(InternalMouseFocusTicked, "MouseFocusTicked");

		MESSAGE_FUNC(InternalInvalidateLayout, "Invalidate");

		MESSAGE_FUNC(InternalMove, "Move");
		virtual void InternalFocusChanged(bool lost);	      

		void Init(int x, int y, int wide, int tall);
		void PreparePanelMap(PanelMap_t* panelMap);

		bool InternalRequestInfo(PanelAnimationMap* map, KeyValues* outputData);
		bool InternalSetInfo(PanelAnimationMap* map, KeyValues* inputData);

		PanelAnimationMapEntry* FindPanelAnimationEntry(char const* scriptname, PanelAnimationMap* map);

		void InternalApplySettings(PanelAnimationMap* map, KeyValues* inResourceData);

		void ApplyAutoResizeSettings(KeyValues* inResourceData);

		void FindDropTargetPanel_R(CUtlVector< VPANEL >& panelList, int x, int y, VPANEL check);
		Panel* FindDropTargetPanel();

		int GetProportionalScaledValue(int rootTall, int normalizedValue);

#if defined( VGUI_USEDRAGDROP )
		DragDrop_t* m_pDragDrop;
		Color			m_clrDragFrame;
		Color			m_clrDropFrame;
#endif

		BaseTooltip* m_pTooltips;
		bool			m_bToolTipOverridden;

		PHandle			m_SkipChild;
		long			m_lLastDoublePressTime;
		HFont			m_infoFont;	         

#if defined( VGUI_USEKEYBINDINGMAPS )
		KeyBindingContextHandle_t m_hKeyBindingsContext;
#endif

		VPANEL			_vpanel;	     
		CUtlString		_panelName;		            
		IBorder* _border;

		CUtlFlags< unsigned short > _flags;	  
		Dar<HPanel>		_actionSignalTargetDar;	          

		CUtlVector<OverridableColorEntry>	m_OverridableColorEntries;

		Color			_fgColor;		  
		Color			_bgColor;		  

		HBuildGroup		_buildGroup;

		short			m_nPinDeltaX;		         
		short			m_nPinDeltaY;
		short			m_nResizeDeltaX;	         
		short			m_nResizeDeltaY;

		HCursor			_cursor;
		unsigned short	_buildModeFlags;            

		byte			_pinCorner : 4;	          
		byte			_autoResizeDirection : 4;          

		GCC_DIAG_PUSH_OFF(overflow)

			DECLARE_DMXELEMENT_BITFIELD(_pinCorner, byte, Panel)
		DECLARE_DMXELEMENT_BITFIELD(_autoResizeDirection, byte, Panel)

		GCC_DIAG_POP()

			unsigned char	_tabPosition;		       
		HScheme			 m_iScheme;       

		bool			m_bIsDMXSerialized : 1;      
		bool			m_bUseSchemeColors : 1;        
		bool			m_bIsSilent : 1;     
		bool			m_bIsConsoleStylePanel : 1;

		DECLARE_DMXELEMENT_BITFIELD(m_bUseSchemeColors, bool, Panel)
		DECLARE_DMXELEMENT_BITFIELD(m_bIsSilent, bool, Panel)

		char* _pinToSibling;				         
		byte			_pinToSiblingCorner;		         
		byte			_pinCornerToSibling;		           
		PHandle			m_pinSibling;

		char* _tooltipText;		            

		PHandle			m_hMouseEventHandler;

		bool			m_bWorldPositionCurrentFrame;		                           
		CUtlSymbol		m_sBorderName;

	protected:
		CUtlString	m_sNavUpName;
		PHandle		m_NavUp;

		CUtlString m_sNavDownName;
		PHandle m_NavDown;

		CUtlString m_sNavLeftName;
		PHandle m_NavLeft;

		CUtlString m_sNavRightName;
		PHandle m_NavRight;
	protected:
		static int s_NavLock;

	private:

		CPanelAnimationVar(float, m_flAlpha, "alpha", "255");

		CPanelAnimationVar(int, m_nPaintBackgroundType, "PaintBackgroundType", "0");
		CPanelAnimationVarAliasType(int, m_nBgTextureId1, "Texture1", "vgui/hud/800corner1", "textureid");
		CPanelAnimationVarAliasType(int, m_nBgTextureId2, "Texture2", "vgui/hud/800corner2", "textureid");
		CPanelAnimationVarAliasType(int, m_nBgTextureId3, "Texture3", "vgui/hud/800corner3", "textureid");
		CPanelAnimationVarAliasType(int, m_nBgTextureId4, "Texture4", "vgui/hud/800corner4", "textureid");

		unsigned char m_roundedCorners;
		friend class BuildGroup;
		friend class BuildModeDialog;
		friend class PHandle;

		void OnOldMessage(KeyValues* params, VPANEL ifromPanel);

	public:

		virtual void GetSizerMinimumSize(int& wide, int& tall);
		virtual void GetSizerClientArea(int& x, int& y, int& wide, int& tall);
		CSizerBase* GetSizer();
		void SetSizer(CSizerBase* pSizer);

	protected:

		CSizerBase* m_pSizer;
	};

	inline void Panel::DisableMouseInputForThisPanel(bool bDisable)
	{
		_flags.SetFlag(IS_MOUSE_DISABLED_FOR_THIS_PANEL_ONLY, bDisable);
	}

	inline bool	Panel::IsMouseInputDisabledForThisPanel() const
	{
		return _flags.IsFlagSet(IS_MOUSE_DISABLED_FOR_THIS_PANEL_ONLY);
	}

	template< class S >
	inline void Panel::PostMessageToAllSiblingsOfType(KeyValues* msg, float delaySeconds  )
	{
		Panel* parent = GetParent();
		if (parent)
		{
			int nChildCount = parent->GetChildCount();
			for (int i = 0; i < nChildCount; ++i)
			{
				Panel* sibling = parent->GetChild(i);
				if (sibling == this)
					continue;
				if (dynamic_cast<S*>(sibling))
				{
					PostMessage(sibling->GetVPanel(), msg->MakeCopy(), delaySeconds);
				}
			}
		}

		msg->deleteThis();
	}

}   


#endif  