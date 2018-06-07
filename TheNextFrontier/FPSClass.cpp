#include "FPSClass.h"

FPSClass::FPSClass() 
{
}

FPSClass::FPSClass(const FPSClass& other) 
{
}

FPSClass::~FPSClass() 
{
}

void FPSClass::Initialize() 
{
	mFPS = 0;
	mCount = 0;
	mStartTime = timeGetTime();

	return;
}

void FPSClass::Frame() 
{
	mCount++;

	if (timeGetTime() >=  (mStartTime + 1000)) 
	{
		mFPS = mCount;
		mCount = 0;

		mStartTime = timeGetTime();
	}
}

int FPSClass::GetFPS() 
{
	return mFPS;
}