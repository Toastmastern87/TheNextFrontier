#pragma once

#define DIRECTINPUT_VERSION 0x0800

#define KEY_W 2

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <fstream>
#include "PositionClass.h"
#include <array>
using namespace std;

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int, PositionClass*, float);
	void Shutdown();
	bool Frame(float);

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	int GetMouseWheelLocation();
	int GetMouseWheelDelta();

	bool IsPlusNmpPressed();
	bool IsMinusNmpPressed();

	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF3Toggled();
	bool IsF4Toggled();
	bool IsF5Toggled();

	bool IsLeftMouseButtonClicked();
	bool IsRightMouseButtonClicked();

	void ProcessKey(int);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mKeyboard;
	IDirectInputDevice8* mMouse;

	unsigned char mKeyboardState[256];
	DIMOUSESTATE mMouseState;

	int mScreenWidth, mScreenHeight;
	int mMouseX, mMouseY, mMouseWheel, mOldMouseWheel;

	bool mF1Released, mF2Released, mF3Released, mF4Released, mF5Released, mLeftMouseButtonReleased, mRightMouseButtonReleased;

	PositionClass* mPosition;
	float mFrameTime;
};