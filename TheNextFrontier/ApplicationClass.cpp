#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	mInput = 0;
	mDirect3D = 0;
	mTimer = 0;
	mFPS = 0;
	mShaderManager = 0;
	mUniverse = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	mDirect3D = new D3DClass;
	if (!mDirect3D)
	{
		return false;
	}

	result = mDirect3D->Initialize(screenWidth, screenHeight, VSYNCENABLED, hwnd, FULLSCREEN, SCREENDEPTH, SCREENEAR);
	if (!result) 
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	mShaderManager = new ShaderManagerClass;
	if (!mShaderManager) 
	{
		return false;
	}

	result = mShaderManager->Initialize(mDirect3D->GetDevice(), hwnd);
	if (!result) 
	{
		MessageBox(hwnd, L"Could not initialize ShaderManager object", L"Error", MB_OK);
		return false;
	}

	mTimer = new TimerClass;
	if (!mTimer)
	{
		return false;
	}

	result = mTimer->Initalize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Timer object", L"Error", MB_OK);
		return false;
	}

	mFPS = new FPSClass;
	if (!mFPS)
	{
		return false;
	}

	mFPS->Initialize();

	mUniverse = new UniverseClass;
	if (!mUniverse) 
	{
		return false;
	}

	result = mUniverse->Initialize(mDirect3D, hwnd, screenWidth, screenHeight, SCREENDEPTH, SCREENEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the universe object", L"Error", MB_OK);
		return false;
	}

	mInput = new InputClass;
	if (!mInput)
	{
		return false;
	}

	result = mInput->Initialize(hInstance, hwnd, screenWidth, screenHeight, mUniverse->GetPosition(), mTimer->GetTime());
	if (!result)
	{
		MessageBox(hwnd, L"Couldn't initiate the input object", L"Error", MB_OK);
		return false;
	}

	mKeys.fill(0);

	return true;
}

void ApplicationClass::Shutdown()
{
	if (mUniverse)
	{
		mUniverse->Shutdown();
		delete mUniverse;
		mUniverse = 0;
	}

	if (mFPS)
	{
		delete mFPS;
		mFPS = 0;
	}

	if (mTimer) 
	{
		delete mTimer;
		mTimer = 0;
	}

	if (mShaderManager) 
	{
		mShaderManager->Shutdown();
		delete mShaderManager;
		mShaderManager = 0;
	}

	if (mDirect3D)
	{
		mDirect3D->Shutdown();
		delete mDirect3D;
		mDirect3D = 0;
	}

	if (mInput)
	{
		mInput->Shutdown();
		delete mInput;
		mInput = 0;
	}

	return;
}

bool ApplicationClass::Frame(HWND hwnd)
{
	bool result;

	mFPS->Frame();
	mTimer->Frame();

	result = mInput->Frame(mTimer->GetTime(), mKeys);
	if (!result) 
	{
		return false;
	}

	if (mInput->IsEscapePressed() == true) 
	{
		return false;
	}

	result = mUniverse->Frame(hwnd, mDirect3D, mInput, mShaderManager, mTimer->GetTime(), mFPS->GetFPS());
	if (!result)
	{
		return false;
	}

	return true;
}