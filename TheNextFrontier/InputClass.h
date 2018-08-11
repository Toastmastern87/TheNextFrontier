#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <fstream>
#include "PositionClass.h"
using namespace std;

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	int GetMouseWheelLocation();
	int GetMouseWheelDelta();

	bool IsAPressed();
	bool IsZPressed();
	bool IsPlusNmpPressed();
	bool IsMinusNmpPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();

	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF3Toggled();
	bool IsF4Toggled();
	bool IsF5Toggled();

	bool IsLeftMouseButtonClicked();
	bool IsRightMouseButtonClicked();

	void OrbitMovement(PositionClass*, float);

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

	int mKey;
};