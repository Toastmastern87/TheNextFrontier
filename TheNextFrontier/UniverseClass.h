#ifndef _UNIVERSECLASS_H_
#define _UNIVERSECLASS_H_

#include "D3DClass.h"
#include "InputClass.h"
#include "ShaderManagerClass.h"
#include "TimerClass.h"
#include "UIClass.h"
#include "CameraClass.h"
#include "PositionClass.h"
#include "MarsClass.h"
#include "FrustumClass.h"

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
	void HandleMovementInput(InputClass*, float);
	bool Render(D3DClass*, ShaderManagerClass*);

private:
	UIClass * mUI;
	CameraClass* mCamera;
	PositionClass* mPosition;
	MarsClass* mMars;
	FrustumClass* mFrustum;
	bool mDisplayUI, mWireframe;
	float mScreenDepth, mScreenNear;
};

#endif
