#include "InputClass.h"

InputClass::InputClass()
{
	mDirectInput = 0;
	mKeyboard = 0;
	mMouse = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight, PositionClass *position, float frameTime)
{
	HRESULT result;

	mPosition = position;
	mFrameTime = frameTime;

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mMouseX = 0;
	mMouseY = 0;
	mMouseWheel = 0;
	mOldMouseWheel = 0;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, NULL);
	if (FAILED(result)) 
	{
		return false;
	}

	result = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = mKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = mKeyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	result = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	mF1Released = true;
	mF2Released = true;
	mF3Released = true;
	mF4Released = true;
	mF5Released = true;
	mLeftMouseButtonReleased = true;
	mRightMouseButtonReleased = true;

	return true;
}

void InputClass::Shutdown() 
{
	if(mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = 0;
	}

	if(mKeyboard)
	{
		mKeyboard->Unacquire();
		mKeyboard->Release();
		mKeyboard = 0;
	}

	if(mDirectInput)
	{
		mDirectInput->Release();
		mDirectInput = 0;
	}
}

bool InputClass::Frame(float frameTime)
{
	bool result;

	mFrameTime = frameTime;

	result = ReadKeyboard();
	if (!result) 
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard() 
{
	HRESULT result;
	
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

	result = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (LPVOID)&mKeyboardState);
	if (FAILED(result)) 
	{
		printf("LOST KEYBOARD!\r\n");
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = mMouse->GetDeviceState(sizeof(mMouseState), (LPVOID)&mMouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput() 
{
	if (mKeyboardState[DIK_UP] & 0x80)
	{
		printf("DIK_UP IS PRESSED!\r\n");
		ProcessKey(DIK_UP);
	}

	if (mKeyboardState[DIK_DOWN] & 0x80)
	{
		printf("DIK_DOWN IS PRESSED!\r\n");
		ProcessKey(DIK_DOWN);
	}

	if (mKeyboardState[DIK_RIGHT] & 0x80)
	{
		ProcessKey(DIK_RIGHT);
	}

	if (mKeyboardState[DIK_LEFT] & 0x80)
	{
		ProcessKey(DIK_LEFT);
	}

	mMouseX += mMouseState.lX;
	mMouseY += mMouseState.lY;
	mMouseWheel += mMouseState.lZ;

	if(mMouseX <= -(mScreenWidth / 2))
	{
		mMouseX = -(mScreenWidth / 2);
	}

	if (mMouseY <= -(mScreenHeight / 2))
	{
		mMouseY = -(mScreenHeight / 2);
	}

	// 33.72 is the width of the mouse texture on the screen
	if(mMouseX >= ((mScreenWidth / 2) - 33.72f))
	{
		mMouseX = ((mScreenWidth / 2) - 33.72f);
	}

	// 40 is the height of the mouse texture on the screen
	if (mMouseY >= ((mScreenHeight / 2) - 40))
	{
		mMouseY = ((mScreenHeight / 2) - 40);
	}
}

bool InputClass::IsEscapePressed() 
{
	if (mKeyboardState[DIK_ESCAPE] & 0x80) 
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY) 
{
	mouseX = mMouseX;
	mouseY = mMouseY;

	return;
}

int InputClass::GetMouseWheelLocation() 
{
	return mMouseWheel;
}

int InputClass::GetMouseWheelDelta()
{
	int ret;

	ret = mMouseWheel - mOldMouseWheel;

	mOldMouseWheel = mMouseWheel;

	return ret;
}

bool InputClass::IsF1Toggled()
{
	if (mKeyboardState[DIK_F1] & 0x80)
	{
		if(mF1Released)
		{
			mF1Released = false;
			return true;
		}
	}
	else 
	{
		mF1Released = true;
	}

	return false;
}

bool InputClass::IsF2Toggled()
{
	if (mKeyboardState[DIK_F2] & 0x80)
	{
		if (mF2Released)
		{
			mF2Released = false;
			return true;
		}
	}
	else
	{
		mF2Released = true;
	}

	return false;
}
bool InputClass::IsF3Toggled()
{
	if (mKeyboardState[DIK_F3] & 0x80)
	{
		if (mF3Released)
		{
			mF3Released = false;
			return true;
		}
	}
	else
	{
		mF3Released = true;
	}

	return false;
}

bool InputClass::IsF4Toggled()
{
	if (mKeyboardState[DIK_F4] & 0x80)
	{
		if (mF4Released)
		{
			mF4Released = false;
			return true;
		}
	}
	else
	{
		mF4Released = true;
	}

	return false;
}

bool InputClass::IsF5Toggled()
{
	if (mKeyboardState[DIK_F5] & 0x80)
	{
		if (mF5Released)
		{
			mF5Released = false;
			return true;
		}
	}
	else
	{
		mF5Released = true;
	}

	return false;
}

bool InputClass::IsPlusNmpPressed()
{
	if (mKeyboardState[DIK_ADD] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsMinusNmpPressed()
{
	if (mKeyboardState[DIK_SUBTRACT] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftMouseButtonClicked()
{
	if (mMouseState.rgbButtons[0] & 0x80)
	{
		if (mLeftMouseButtonReleased)
		{
			mLeftMouseButtonReleased = false;

			return true;
		}
	}
	else
	{
		mLeftMouseButtonReleased = true;
	}

	return false;
}

bool InputClass::IsRightMouseButtonClicked()
{
	if (mMouseState.rgbButtons[1] & 0x80)
	{
		if (mRightMouseButtonReleased)
		{
			mRightMouseButtonReleased = false;

			return true;
		}
	}
	else
	{
		mRightMouseButtonReleased = true;
	}

	return false;
}

void InputClass::ProcessKey(int key) 
{
	switch (key)
	{
	case DIK_UP:
		mPosition->mOrbitalAngleY += 1.0f * mFrameTime * (mPosition->GetDistanceFromOrigo() / 100000.0f);
		break;
	case DIK_DOWN:
		mPosition->mOrbitalAngleY -= 1.0f * mFrameTime * (mPosition->GetDistanceFromOrigo() / 100000.0f);
		break;
	}

	return;
}