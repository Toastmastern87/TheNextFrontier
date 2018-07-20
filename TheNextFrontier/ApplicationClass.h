#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

const bool FULLSCREEN = true;
const bool VSYNCENABLED = false;
const float SCREENDEPTH = 30000.0f;
const float SCREENEAR = 0.1f;

#include "InputClass.h"
#include "D3DClass.h"
#include "ShaderManagerClass.h"
#include "TimerClass.h"
#include "FPSClass.h"
#include "UniverseClass.h"
#include "tiffio.h"
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown(); 
	bool Frame(HWND);

private:
	InputClass* mInput;
	D3DClass* mDirect3D;
	ShaderManagerClass* mShaderManager;
	TimerClass* mTimer;
	FPSClass* mFPS;
	UniverseClass* mUniverse;
};

#endif