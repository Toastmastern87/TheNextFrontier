#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	mDirect3D = 0;
	mCamera = 0;
	mModel = 0;
	mColorShader = 0;
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

	mCamera = new CameraClass();
	if (!mCamera)
	{
		return false;
	}

	mCamera->SetPosition(0.0f, 0.0f, -5.0f);

	mModel = new ModelClass();
	if (!mModel)
	{
		return false;
	}

	result = mModel->Initialize(mDirect3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	mColorShader = new ColorShaderClass();
	if (!mColorShader)
	{
		return false;
	}

	result = mColorShader->Initialize(mDirect3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (mColorShader)
	{
		mColorShader->Shutdown();
		delete mColorShader;
		mColorShader = 0;
	}

	if (mModel)
	{
		mModel->Shutdown();
		delete mModel;
		mModel = 0;
	}

	if (mCamera) 
	{
		delete mCamera;
		mCamera = 0;
	}

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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	mDirect3D->BeginScene(0.29f, 0.57f, 0.86f, 1.0f);

	mCamera->Render();

	mDirect3D->GetWorldMatrix(worldMatrix);
	mCamera->GetViewMatrix(viewMatrix);
	mDirect3D->GetProjectionMatrix(projectionMatrix);

	mModel->Render(mDirect3D->GetDeviceContext());

	result = mColorShader->Render(mDirect3D->GetDeviceContext(), mModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	mDirect3D->EndScene();

	return true;
}