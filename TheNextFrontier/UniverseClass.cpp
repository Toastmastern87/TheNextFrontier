#include "UniverseClass.h"

UniverseClass::UniverseClass()
{
	mUI = 0;
	mCamera = 0;
	mPosition = 0;
	mMars = 0;
	mFrustum = 0;
	mSunlight = 0;
}

UniverseClass::UniverseClass(const UniverseClass& other)
{
}

UniverseClass::~UniverseClass()
{
}

bool UniverseClass::Initialize(D3DClass* direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	bool result;

	mScreenDepth = screenDepth;
	mScreenDepth = screenNear;

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

	mSunlight = new LightClass();
	if (!mSunlight)
	{
		return false;
	}

	mSunlight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	mSunlight->SetDirection(1.0f, 0.0f, 0.0f, 1.0f);

	mPosition = new PositionClass;
	if (!mPosition)
	{
		return false;
	}

	mPosition->SetPosition(10000.0f, 0.0f, 0.0f);
	mPosition->SetRotation(0.0f, 270.0f, 0.0f);

	mFrustum = new FrustumClass;
	if (!mFrustum)
	{
		return false;
	}

	result = mFrustum->Initialize(mCamera, mPosition);
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

	mMousePointer = new MousePointerClass();
	if (!mMousePointer)
	{
		return false;
	}

	result = mMousePointer->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the mouse object", L"Error", MB_OK);
		return false;
	}

	mDisplayUI = true;
	mWireframe = true;

	return true;
}

void UniverseClass::Shutdown()
{
	if (mMousePointer)
	{
		mMousePointer->Shutdown();
		delete mMousePointer;
		mMousePointer = 0;
	}

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

	if (mSunlight)
	{
		delete mSunlight;
		mSunlight = 0;
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
	int mouseX, mouseY;

	HandleMovementInput(input, frameTime);

	mPosition->GetPosition(posX, posY, posZ);
	mPosition->GetRotation(rotX, rotY, rotZ);

	input->GetMouseLocation(mouseX, mouseY);

	result = mMousePointer->UpdateMousePointerPos(direct3D->GetDeviceContext(), mouseX, (-1 * mouseY));
	if (!result)
	{
		return false;
	}

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
	int mouseX, mouseY;

	mPosition->SetFrameTime(frameTime);

	input->GetMouseLocation(mouseX, mouseY);

	keyDown = input->IsLeftPressed();
	mPosition->OrbitLeft(keyDown);

	keyDown = input->IsRightPressed();
	mPosition->OrbitRight(keyDown);

	keyDown = input->IsUpPressed();
	mPosition->MoveForward(keyDown);

	keyDown = input->IsDownPressed();
	mPosition->MoveBackward(keyDown);

	keyDown = input->IsAPressed();
	mPosition->ZoomOut(keyDown);

	keyDown = input->IsZPressed();
	mPosition->ZoomIn(keyDown);

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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix, inverseWorldMatrix;
	bool result;

	mCamera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	inverseWorldMatrix = direct3D->GetInverseWorldMatrix();
	viewMatrix = mCamera->GetViewMatrix();
	projectionMatrix = direct3D->GetProjectionMatrix();
	mCamera->GetBaseViewMatrix(baseViewMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	if (mCamera->CheckMovement()) 
	{
		mFrustum->ConstructFrustum(mScreenDepth, mScreenNear, direct3D->GetAspectRation(), projectionMatrix, viewMatrix, worldMatrix);

		mMars->UpdateMars(direct3D->GetDeviceContext(), mFrustum, mPosition);
	}

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (mWireframe)
	{
		direct3D->EnableWireframe();
	}

	mMars->Render(direct3D->GetDeviceContext());

	result = shaderManager->RenderMarsShader(direct3D->GetDeviceContext(), mMars->GetIndexCount(), mMars->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, inverseWorldMatrix, mMars->GetMarsRadius(), mMars->GetMarsMaxHeight(), mMars->GetMarsMinHeight(), mMars->GetDistanceLUT(), mPosition->GetPositionXMFLOAT3(), mMars->GetHeightMap(), mSunlight->GetDirection(), mSunlight->GetDiffuseColor(), mMars->GetMarsPatchDelta());
	if (!result)
	{
		return false;
	}

	if (mWireframe)
	{
		direct3D->DisableWireframe();
	}

	direct3D->TurnZBufferOff();
	direct3D->EnableAlphaBlending();

	if (mDisplayUI)
	{
		result = mUI->Render(direct3D, shaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	result = mMousePointer->Render(direct3D, shaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	direct3D->DisableAlphaBlending();
	direct3D->TurnZBufferOn();

	direct3D->EndScene();

	return true;
}