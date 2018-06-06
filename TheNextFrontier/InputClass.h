#ifndef _INPUTCLASS_H_

#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

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

	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsAPressed();
	bool IsZPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();

	bool IsF1Toggled();
	bool IsF2Toggled();

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
	int mMouseX, mMouseY;

	bool mF1Released;
	bool mF2Released;
};

#endif