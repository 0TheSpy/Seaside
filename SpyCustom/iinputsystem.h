#ifndef IINPUTSYSTEM_H
#define IINPUTSYSTEM_H
#ifdef _WIN32
#pragma once
#endif

#include "platform.h"
#include "IAppSystem.h"

#include "InputEnums.h"
#include "ButtonCode.h"
#include "AnalogCode.h"

#define INPUTSYSTEM_INTERFACE_VERSION	"InputSystemVersion001"
abstract_class IInputSystem : public IAppSystem
{
public:
	virtual void AttachToWindow(void* hWnd) = 0;
	virtual void DetachFromWindow() = 0;

	virtual void EnableInput(bool bEnable) = 0;

	virtual void EnableMessagePump(bool bEnable) = 0;

	virtual void PollInputState() = 0;

	virtual int GetPollTick() const = 0;

	virtual bool IsButtonDown(ButtonCode_t code) const = 0;

	virtual int GetButtonPressedTick(ButtonCode_t code) const = 0;
	virtual int GetButtonReleasedTick(ButtonCode_t code) const = 0;

	virtual int GetAnalogValue(AnalogCode_t code) const = 0;

	virtual int GetAnalogDelta(AnalogCode_t code) const = 0;

	virtual int GetEventCount() const = 0;
	virtual const InputEvent_t* GetEventData() const = 0;

	virtual void PostUserEvent(const InputEvent_t& event) = 0;

	virtual int GetJoystickCount() const = 0;

	virtual void EnableJoystickInput(int nJoystick, bool bEnable) = 0;

	virtual void EnableJoystickDiagonalPOV(int nJoystick, bool bEnable) = 0;

	virtual void SampleDevices(void) = 0;

	virtual void SetRumble(float fLeftMotor, float fRightMotor, int userId = INVALID_USER_ID) = 0;
	virtual void StopRumble(void) = 0;

	virtual void ResetInputState() = 0;

	virtual void SetPrimaryUserId(int userId) = 0;

	virtual const char* ButtonCodeToString(ButtonCode_t code) const = 0;
	virtual const char* AnalogCodeToString(AnalogCode_t code) const = 0;
	virtual ButtonCode_t StringToButtonCode(const char* pString) const = 0;
	virtual AnalogCode_t StringToAnalogCode(const char* pString) const = 0;

	virtual void SleepUntilInput(int nMaxSleepTimeMS = -1) = 0;

	virtual ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey) const = 0;
	virtual int ButtonCodeToVirtualKey(ButtonCode_t code) const = 0;
	virtual ButtonCode_t ScanCodeToButtonCode(int lParam) const = 0;

	virtual int GetPollCount() const = 0;

	virtual void SetCursorPosition(int x, int y) = 0;

	virtual void* GetHapticsInterfaceAddress() const = 0;

	virtual void SetNovintPure(bool bPure) = 0;

	virtual bool GetRawMouseAccumulators(int& accumX, int& accumY) = 0;

	virtual void SetConsoleTextMode(bool bConsoleTextMode) = 0;
};


#endif  