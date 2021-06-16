#ifndef INPUTENUMS_H
#define INPUTENUMS_H
#ifdef _WIN32
#pragma once
#endif

#define MAX_BUTTONSAMPLE			32768

#if !defined( _X360 )
#define INVALID_USER_ID		-1
#else
#define INVALID_USER_ID		XBX_INVALID_USER_ID
#endif

enum
{
	MAX_JOYSTICKS = 1,
	MOUSE_BUTTON_COUNT = 5,
	MAX_NOVINT_DEVICES = 2,
};

#if defined( LINUX )
enum JoystickAxis_t
{
	JOY_AXIS_X = 0,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_U,
	JOY_AXIS_R,
	JOY_AXIS_V,
	MAX_JOYSTICK_AXES,
};
#else
enum JoystickAxis_t
{
	JOY_AXIS_X = 0,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_R,
	JOY_AXIS_U,
	JOY_AXIS_V,
	MAX_JOYSTICK_AXES,
};
#endif



enum
{
	MS_WM_XBUTTONDOWN = 0x020B,
	MS_WM_XBUTTONUP = 0x020C,
	MS_WM_XBUTTONDBLCLK = 0x020D,
	MS_MK_BUTTON4 = 0x0020,
	MS_MK_BUTTON5 = 0x0040,
};

enum InputEventType_t
{
	IE_ButtonPressed = 0,	    
	IE_ButtonReleased,		    
	IE_ButtonDoubleClicked,	    
	IE_AnalogValueChanged,	        

	IE_FirstSystemEvent = 100,
	IE_Quit = IE_FirstSystemEvent,
	IE_ControllerInserted,	     
	IE_ControllerUnplugged,	     

	IE_FirstVguiEvent = 1000,	          
	IE_FirstAppEvent = 2000,
};

struct InputEvent_t
{
	int m_nType;				      
	int m_nTick;				      
	int m_nData;				          
	int m_nData2;				          
	int m_nData3;				          
};

#endif  