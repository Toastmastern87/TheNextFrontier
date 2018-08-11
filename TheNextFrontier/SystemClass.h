#pragma once

#define WIN32_LEAN_AND_MEAN

#include "ApplicationClass.h"
#include <fstream>

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR mApplicationName;
	HINSTANCE mHInstance;
	HWND mHWND;

	ApplicationClass* mApplication;

	RAWINPUTDEVICE mNewKeyboard;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* applicationHandle = 0;