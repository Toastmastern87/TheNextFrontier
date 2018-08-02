#include "UniverseClass.h"

UniverseClass::UniverseClass()
{
	mUI = 0;
	mCamera = 0;
	mPosition = 0;
	mMars = 0;
	mMarsAtmosphere = 0;
	mFrustum = 0;
	mSunlight = 0;
	mGameTime = 0;
	mGUI = 0;
	mStarBox = 0;

	mSpeedIncreased = false;
	mSpeedDecreased = false;

	mWireframe = false;
	mDisplayUI = true;
	mRenderAtmosphere = true;
	mRenderMars = true;
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
	mScreenNear = screenNear;

	mStarBox = new StarBoxClass();
	if (!mStarBox)
	{
		return false;
	}

	result = mStarBox->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Star Box object", L"Error", MB_OK);
		return false;
	}

	mGUI = new GUIClass();
	if (!mGUI)
	{
		return false;
	}

	result = mGUI->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Game GUI object", L"Error", MB_OK);
		return false;
	}

	mGameTime = new GameTimeClass();
	if (!mGameTime)
	{
		return false;
	}

	mGameTime->Initialize();

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
	mSunlight->SetDirection(-1.0f, 0.0f, 0.0f);

	mPosition = new PositionClass;
	if (!mPosition)
	{
		return false;
	}

	mPosition->SetPosition(3030.0f, 851.0f, 1433.0f);
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

	mMarsAtmosphere = new MarsAtmosphereClass;
	if (!mMarsAtmosphere)
	{
		return false;
	}

	result = mMarsAtmosphere->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), 3389.5f);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Mars Atmosphere object.", L"Error", MB_OK);

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

	return true;
}

void UniverseClass::Shutdown()
{
	if (mStarBox)
	{
		mStarBox->Shutdown();
		delete mStarBox;
		mStarBox = 0;
	}

	if (mGUI)
	{
		mGUI->Shutdown();
		delete mGUI;
		mGUI = 0;
	}

	if (mMousePointer)
	{
		mMousePointer->Shutdown();
		delete mMousePointer;
		mMousePointer = 0;
	}

	if (mMarsAtmosphere)
	{
		mMarsAtmosphere->Shutdown();
		delete mMarsAtmosphere;
		mMarsAtmosphere = 0;
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

	if (mGameTime)
	{
		delete mGameTime;
		mGameTime = 0;
	}

	return;
}

bool UniverseClass::Frame(HWND hwnd, D3DClass* direct3D, InputClass* input, ShaderManagerClass* shaderManager, float frameTime, int fps)
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ, altitude, numberOfVertices;
	int mouseX, mouseY;

	numberOfVertices = 0;

	mGameTime->Frame();

	mSunlight->CalculateDayNightCycle(mGameTime->GetGameTimeMS(), mGameTime->GetGameTimeSecs());

	HandleMovementInput(input, frameTime);

	mPosition->GetPosition(posX, posY, posZ);
	mPosition->GetRotation(rotX, rotY, rotZ);

	input->GetMouseLocation(mouseX, mouseY);

	result = mMousePointer->UpdateMousePointerPos(direct3D->GetDeviceContext(), mouseX, (-1 * mouseY));
	if (!result)
	{
		return false;
	}

	if (mRenderMars) 
	{
		numberOfVertices += (mMars->GetMarsVerticesCount() * mMars->GetInstanceCount());
	}

	if (mRenderAtmosphere)
	{
		numberOfVertices += (mMarsAtmosphere->GetMarsVerticesCount() * mMarsAtmosphere->GetInstanceCount());
	}

	if (mMars) 
	{
		altitude = mPosition->GetDistanceFromOrigo() - mMars->GetHeightAtPos(mPosition->GetPositionXMFLOAT3());

		result = mUI->Frame(hwnd, direct3D->GetDeviceContext(), fps, posX, posY, posZ, rotX, rotY, rotZ, numberOfVertices, altitude, mPosition->GetDistanceFromOrigo(), mMars->GetHeightAtPos(mPosition->GetPositionXMFLOAT3()), mGameTime->GetGameTimeSecs(), mGameTime->GetGameTimeMins(), mGameTime->GetGameTimeHours(), mGameTime->GetGameTimeDays(), mGameTime->GetGameTimeMarsYears());
		if (!result)
		{
			return false;
		}
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
	int mouseX, mouseY, mouseDelta;

	mPosition->SetFrameTime(frameTime);

	input->GetMouseLocation(mouseX, mouseY);
	input->GetMouseWheelLocation();
	mouseDelta = input->GetMouseWheelDelta();

	keyDown = input->IsLeftPressed();
	mPosition->OrbitLeft(keyDown);

	keyDown = input->IsRightPressed();
	mPosition->OrbitRight(keyDown);

	keyDown = input->IsUpPressed();
	mPosition->OrbitNorth(keyDown);

	keyDown = input->IsDownPressed();
	mPosition->OrbitSouth(keyDown);

	keyDown = input->IsPlusNmpPressed();
	if (keyDown && !mSpeedIncreased)
	{
		mGameTime->IncreaseGameSpeed();

		mSpeedIncreased = true;
	}
	else if (!keyDown && mSpeedIncreased)
	{
		mSpeedIncreased = false;
	}

	keyDown = input->IsMinusNmpPressed();
	if (keyDown && !mSpeedDecreased)
	{
		mGameTime->DecreaseGameSpeed();

		mSpeedDecreased = true;
	}
	else if (!keyDown && mSpeedDecreased)
	{
		mSpeedDecreased = false;
	}

	if (mMars)
	{
		if (mPosition->GetDistanceFromOrigo() < mPosition->MAXDISTANCEFROMORIGO) 
		{
			mPosition->ZoomOut(mouseDelta, mMars->GetMarsRadius());
		}

		if (!mPosition->MaxZoom())
		{
			mPosition->ZoomIn(mouseDelta, mMars->GetMarsRadius(), mMars->GetHeightAtPos(mPosition->GetPositionXMFLOAT3()));
		}

		if (mPosition->MaxZoom())
		{
			mPosition->CheckAltitude(mMars->GetHeightAtPos(mPosition->GetPositionXMFLOAT3()));
		}
	}

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

	if(input->IsF3Toggled())
	{
		mRenderAtmosphere = !mRenderAtmosphere;
	}

	if (input->IsF4Toggled())
	{
		mRenderMars = !mRenderMars;
	}

	return;
}

bool UniverseClass::Render(D3DClass* direct3D, ShaderManagerClass* shaderManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix, inverseWorldMatrix, rotationMatrix;
	bool result;

	mCamera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	inverseWorldMatrix = direct3D->GetInverseWorldMatrix();
	viewMatrix = mCamera->GetViewMatrix();
	projectionMatrix = direct3D->GetProjectionMatrix();
	mCamera->GetBaseViewMatrix(baseViewMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);
	rotationMatrix = mSunlight->GetSunlightRotation();

	if (mCamera->CheckMovement()) 
	{
		mFrustum->ConstructFrustum(mScreenDepth, mScreenNear, direct3D->GetAspectRation(), projectionMatrix, viewMatrix, worldMatrix);
	}

	mMars->UpdateMars(direct3D->GetDeviceContext(), mFrustum, mPosition);
	mMarsAtmosphere->UpdateMarsAtmosphere(direct3D->GetDeviceContext(), mPosition);

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (mWireframe)
	{
		direct3D->EnableWireframe();
	}

	mStarBox->Render(direct3D->GetDeviceContext());
	result = shaderManager->RenderStarBoxShader(direct3D->GetDeviceContext(), mStarBox->GetIndexCount(), worldMatrix, baseViewMatrix, projectionMatrix, mStarBox->GetStarBoxTexture());

	if (mRenderMars)
	{
		mMars->Render(direct3D->GetDeviceContext());
		result = shaderManager->RenderMarsShader(direct3D->GetDeviceContext(), mMars->GetIndexCount(), mMars->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, inverseWorldMatrix, rotationMatrix, mMars->GetMarsRadius(), mMars->GetMarsMaxHeight(), mMars->GetMarsMinHeight(), mMars->GetDistanceLUT(), mPosition->GetPositionXMFLOAT3(), mMars->GetHeightMap(), mMars->GetHeightMapDetail2(), mMars->GetColorMap(), mMars->GetDetailAreaMapX(), mMars->GetDetailAreaMapY(), mMars->GetDetailAreaMapWH(), mMars->GetCraterHeightMap(), mSunlight->GetDirection(), mSunlight->GetDiffuseColor(), mMars->GetMarsPatchDelta(), mPosition->CheckIfInsideAtmosphere(mMarsAtmosphere->GetAtmosphereHeight(), mMars->GetMarsRadius(), mPosition->GetDistanceFromOrigo()), mPosition->GetDistanceFromOrigo());
		if (!result)
		{
			return false;
		}
	}

	if (mRenderAtmosphere) 
	{
		if (mWireframe) 
		{
			direct3D->EnableFrontCullingWireframe();
		}
		else 
		{
			direct3D->TurnOnFrontCulling();
		}

		mMarsAtmosphere->Render(direct3D->GetDeviceContext());
		result = shaderManager->RenderMarsAtmosphereShader(direct3D->GetDeviceContext(), mMarsAtmosphere->GetIndexCount(), mMarsAtmosphere->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, rotationMatrix, (mMarsAtmosphere->GetAtmosphereHeight() + mMars->GetMarsRadius()), mPosition->CheckIfInsideAtmosphere(mMarsAtmosphere->GetAtmosphereHeight(), mMars->GetMarsRadius(), mPosition->GetDistanceFromOrigo()), mMars->GetMarsRadius(), mPosition->GetDistanceFromOrigo(), mCamera->GetPosition(), mSunlight->GetDirection());
		if (!result)
		{
			return false;
		}
	
		if (mWireframe)
		{
			direct3D->DisableFrontCullingWireframe();
		}
		else
		{
			direct3D->TurnOffFrontCulling();
		}
	}

	if (mWireframe)
	{
		direct3D->DisableWireframe();
	}

	direct3D->TurnZBufferOff();
	direct3D->EnableAlphaBlending();

	if (mDisplayUI)
	{
		result = mGUI->Render(direct3D, shaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

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