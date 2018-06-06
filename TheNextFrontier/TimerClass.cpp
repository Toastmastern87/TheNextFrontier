#include "TimerClass.h"

TimerClass::TimerClass() 
{
}

TimerClass::TimerClass(const TimerClass& other) 
{
}

TimerClass::~TimerClass() 
{
}

bool TimerClass::Initalize() 
{
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		return false;
	}

	mFrequency = (float)frequency;

	QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);

	return true;
}

void TimerClass::Frame() 
{
	INT64 currentTime;
	INT64 elapsedTicks;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	elapsedTicks = currentTime - mStartTime;

	mFrameTime = (float)elapsedTicks / mFrequency;

	mStartTime = currentTime;

	return;
}

float TimerClass::GetTime() 
{
	return mFrameTime;
}

void TimerClass::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mBeginTime);

	return;
}

void TimerClass::StopTimer() 
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mEndTime);

	return;
}

int TimerClass::GetTiming() 
{
	float elapsedTicks;
	INT64 frequency;
	float milliSeconds;

	elapsedTicks = (float)(mEndTime - mBeginTime);

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	milliSeconds = (elapsedTicks / (float)frequency) * 1000.0f;

	return (int)milliSeconds;
}