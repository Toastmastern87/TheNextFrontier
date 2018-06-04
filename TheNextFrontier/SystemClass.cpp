#include "SystemClass.h"

SystemClass::SystemClass()
{
	mInput = 0;
	mGraphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize() 
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	mInput = new InputClass();
	if (!mInput) 
	{
		return false;
	}

	mInput->Initialize();

	mGraphics = new GraphicsClass();
	if (!mGraphics) 
	{
		return false;
	}

	result = mGraphics->Initialize(screenWidth, screenHeight, mHWND);
	if (!result) 
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown() 
{
	if (mGraphics) 
	{
		mGraphics->Shutdown();
		delete mGraphics;
		mGraphics = 0;
	}

	if (mInput)
	{
		delete mInput;
		mInput = 0;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run() 
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) 
		{
			done = true;
		}
		else 
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
		
	return;
}

bool SystemClass::Frame() 
{
	bool result;

	if (mInput->IsKeyDown(VK_ESCAPE)) 
	{
		return false;
	}

	result = mGraphics->Frame();
	if (!result) 
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) 
{
	switch (umsg) 
	{
		case WM_KEYDOWN:
		{
			mInput->KeyDown((unsigned int)wparam);

			return 0;
		}
		
		case WM_KEYUP: 
		{
			mInput->KeyUp((unsigned int)wparam);

			return 0;
		}

		default: 
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) 
{
	WNDCLASSEX wc;
	DEVMODE	dmScreenSettings;
	int posX, posY;

	applicationHandle = this;

	mHInstance = GetModuleHandle(NULL);

	mApplicationName = L"The Next Frontier";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULLSCREEN) 
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else 
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	mHWND = CreateWindowEx(WS_EX_APPWINDOW,  mApplicationName, mApplicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, mHInstance, NULL);

	ShowWindow(mHWND, SW_SHOW);
	SetForegroundWindow(mHWND);
	SetFocus(mHWND);

	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(mHWND);
	mHWND = NULL;

	UnregisterClass(mApplicationName, mHInstance);
	mHInstance = NULL;

	applicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage) 
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return applicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}