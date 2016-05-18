#pragma once
#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

#include "..\Common\PEMacro.h"
#include <dinput.h>

PE_BEGIN

enum E_InputDevice
{
	eID_SysKeyboard = 0,
	eID_SysMouse = 1,
};

struct InputKeys
{
	static const short Up = DIK_UP;
	static const short Down = DIK_DOWN;
	static const short Left = DIK_LEFT;
	static const short Right = DIK_RIGHT;
};

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void Init(void *hWnd);
	/**
	 @brief 更新所有设备的输入状态
	 */
	void UpdateInputState();

	/**
	 @brief 获取键盘状态buffer
	 */
	bool GetKeyDownState(short key);

private:
	HWND mHWnd;

	IDirectInput8		*mDirectInput;
	IDirectInputDevice8	*mKeyboard;
	
	// 默认键盘的数据
	unsigned long mKeyBufferSize;
	byte *mKeyStateBuffer;
};

PE_END

#endif