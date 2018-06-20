#include "UniverseClass.h"

UniverseClass::UniverseClass()
{
	mUI = 0;
	mCamera = 0;
	mPosition = 0;
	mMars = 0;
	mFrustum = 0;
}

UniverseClass::UniverseClass(const UniverseClass& other)
{
}

UniverseClass::~UniverseClass()
{
}

bool UniverseClass::Initialize(D3DClass* direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	bool result;

	mScreenDepth = screenDepth;

	mUI = new UIClass;
	if (!mUI)
	{
		return false;
	}

	result = mUI->Initialize(hwnd, direct3D, screenHeight, screenWidth);
	if (!result)
	{
		return false;
	}

	mCamera = new CameraClass;
	if (!mCamera)
	{
		return false;
	}

	mCamera->SetPosition(0.0f, 0.0f, -10.0f);
	mCamera->SetFOV(45.0f);
	mCamera->Render();
	mCamera->RenderBaseViewMatrix();

	mPosition = new PositionClass;
	if (!mPosition)
	{
		return false;
	}

	mPosition->SetPosition(6000.0f, 8500.0f, 0.0f);
	mPosition->SetRotation(56.0f, 269.0f, 16.0f);

	mFrustum = new FrustumClass;
	if (!mFrustum)
	{
		return false;
	}

	result = mFrustum->Initialize(mCamera);
	if (!result)
	{
		return false;
	}

	mMars = new MarsClass;
	if (!mMars)
	{
		return false;
	}

	result = mMars->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), mFrustum, screenWidth);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Mars object.", L"Error", MB_OK);

		return false;
	}

	mDisplayUI = true;
	mWireframe = true;

	return true;
}

void UniverseClass::Shutdown()
{
	if (mMars)
	{
		mMars->Shutdown();
		delete mMars;
		mMars = 0;
	}

	if (mPosition)
	{
		delete mPosition;
		mPosition = 0;
	}

	if (mCamera)
	{
		delete mCamera;
		mCamera = 0;
	}

	if (mUI)
	{
		mUI->Shutdown();
		delete mUI;
		mUI = 0;
	}

	return;
}

bool UniverseClass::Frame(HWND hwnd, D3DClass* direct3D, InputClass* input, ShaderManagerClass* shaderManager, float frameTime, int fps)
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	HandleMovementInput(input, frameTime);

	mPosition->GetPosition(posX, posY, posZ);
	mPosition->GetRotation(rotX, rotY, rotZ);

	result = mUI->Frame(hwnd, direct3D->GetDeviceContext(), fps, posX, posY, posZ, rotX, rotY, rotZ, (mMars->GetMarsVerticesCount() * mMars->GetInstanceCount()), mPosition->GetAltitude());
	if (!result)
	{
		return false;
	}

	result = Render(direct3D, shaderManager);
	if (!result) {
		return false;
	}

	return true;
}

void UniverseClass::HandleMovementInput(InputClass* input, float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	mPosition->SetFrameTime(frameTime);

	keyDown = input->IsLeftPressed();
	mPosition->TurnLeft(keyDown);

	keyDown = input->IsRightPressed();
	mPosition->TurnRight(keyDown);

	keyDown = input->IsUpPressed();
	mPosition->MoveForward(keyDown);

	keyDown = input->IsDownPressed();
	mPosition->MoveBackward(keyDown);

	keyDown = input->IsAPressed();
	mPosition->MoveUpward(keyDown);

	keyDown = input->IsZPressed();
	mPosition->MoveDownward(keyDown);

	keyDown = input->IsPgUpPressed();
	mPosition->LookUpward(keyDown);

	keyDown = input->IsPgDownPressed();
	mPosition->LookDownward(keyDown);

	mPosition->GetPosition(posX, posY, posZ);
	mPosition->GetRotation(rotX, rotY, rotZ);

	mCamera->SetPosition(posX, posY, posZ);
	mCamera->SetRotation(rotX, rotY, rotZ);

	if (input->IsF1Toggled())
	{
		mDisplayUI = !mDisplayUI;
	}

	if (input->IsF2Toggled())
	{
		mWireframe = !mWireframe;
	}

	return;
}

bool UniverseClass::Render(D3DClass* direct3D, ShaderManagerClass* shaderManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;

	mCamera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	viewMatrix = mCamera->GetViewMatrix();
	projectionMatrix = direct3D->GetProjectionMatrix();
	mCamera->GetBaseViewMatrix(baseViewMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	//if (mCamera->CheckMovement()) 
	//{
	mFrustum->ConstructFrustum(mScreenDepth, projectionMatrix, viewMatrix);

	mMars->UpdateMars(direct3D->GetDeviceContext(), mFrustum, mPosition);
	//}

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (mWireframe)
	{
		direct3D->EnableWireframe();
	}

	mMars->Render(direct3D->GetDeviceContext());

	result = shaderManager->RenderColorShader(direct3D->GetDeviceContext(), mMars->GetIndexCount(), mMars->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, mMars->GetMarsRadius(), mMars->GetDistanceLUT(), mPosition->GetPositionXMFLOAT3());
	if (!result)
	{
		return false;
	}

	if (mWireframe)
	{
		direct3D->DisableWireframe();
	}

	if (mDisplayUI)
	{
		result = mUI->Render(direct3D, shaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	direct3D->EndScene();

	return true;
}