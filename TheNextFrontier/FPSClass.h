#ifndef _FPSCLASS_H_

#define _FPSCLASS_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

class FPSClass 
{
public:
	FPSClass();
	FPSClass(const FPSClass&);
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFPS();

private:
	int mFPS, mCount;
	unsigned long mStartTime;
};

#endif 