#pragma once

#include "D3DClass.h"
#include "InputClass.h"
#include "ShaderManagerClass.h"
#include "TimerClass.h"
#include "UIClass.h"
#include "CameraClass.h"
#include "PositionClass.h"
#include "MarsClass.h"
#include "FrustumClass.h"
#include "MousePointerClass.h"
#include "LightClass.h"
#include "GameTimeClass.h"
#include "GUIClass.h"
#include "MarsAtmosphereClass.h"
#include "StarBoxClass.h";
#include "BFSClass.h";

class UniverseClass
{
public:
	UniverseClass();
	UniverseClass(const UniverseClass& other);
	~UniverseClass();

	bool Initialize(D3DClass*, HWND, int, int, float, float);
	void Shutdown();
	bool Frame(HWND, D3DClass*, InputClass*, ShaderManagerClass*, float, int);

private:
	void HandleMovementInput(InputClass*, float, D3DClass*);
	bool Render(D3DClass*, ShaderManagerClass*);
	
private:
	UIClass * mUI;
	CameraClass* mCamera;
	PositionClass* mPosition;
	MarsClass* mMars;
	MarsAtmosphereClass* mMarsAtmosphere;
	FrustumClass* mFrustum;
	LightClass* mSunlight;
	bool mDisplayUI, mWireframe, mSpeedIncreased, mSpeedDecreased, mRenderAtmosphere, mRenderMars, mLeftMouseButtonClicked, mRightMouseButtonClicked, mRenderStarBox;
	float mScreenDepth, mScreenNear;
	int  mScreenWidth, mScreenHeight;
	MousePointerClass *mMousePointer;
	GameTimeClass *mGameTime;
	GUIClass *mGUI;
	StarBoxClass *mStarBox;
	BFSClass *mHeartOfGold;
};