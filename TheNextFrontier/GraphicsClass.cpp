#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	mDirect3D = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
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

	return true;
}

void GraphicsClass::Shutdown()
{
	if (mDirect3D) 
	{
		mDirect3D->Shutdown();
		delete mDirect3D;
		mDirect3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if (!result) 
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	mDirect3D->BeginScene(0.29f, 0.57f, 0.86f, 1.0f);

	mDirect3D->EndScene();

	return true;
}