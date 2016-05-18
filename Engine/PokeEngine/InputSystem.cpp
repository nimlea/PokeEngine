#include "InputSystem.h"
#include "LogSystem.h"

#pragma comment(lib, "dinput8.lib")

PE_BEGIN

InputSystem::InputSystem()
:mHWnd(nullptr), mDirectInput(nullptr), mKeyboard(nullptr)
{
	mKeyStateBuffer = new byte[256];
	mKeyBufferSize = sizeof(byte)* 256;
	memset(mKeyStateBuffer, 0, mKeyBufferSize);
}

InputSystem::~InputSystem()
{
	if (mKeyStateBuffer != nullptr)
	{
		delete[] mKeyStateBuffer;
		mKeyStateBuffer = nullptr;
	}
}

void InputSystem::Init(void *hWnd)
{
	mHWnd = (HWND)hWnd;
	HINSTANCE hInst = GetModuleHandle(nullptr);
	// 创建DXinput
	HRESULT hr;
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,	IID_IDirectInput8, (void**)&mDirectInput, nullptr);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建Direct Input失败");
		return;
	}

	// 默认键盘device
	hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建Input System Keyboard设备失败");
		return;
	}
	mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	mKeyboard->SetCooperativeLevel(mHWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
}

void InputSystem::UpdateInputState()
{
	HRESULT hr;
	if (SUCCEEDED(mKeyboard->Acquire()))
	{
		hr = mKeyboard->GetDeviceState(mKeyBufferSize, mKeyStateBuffer);
		if (hr == DI_OK) {
		}
	}
}

bool InputSystem::GetKeyDownState(short key)
{
	if (mKeyStateBuffer[key] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

PE_END