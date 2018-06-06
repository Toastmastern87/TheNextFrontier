#ifndef _TIMERCLASS_H_

#define _TIMERCLASS_H_

#include <Windows.h>

class TimerClass 
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initalize();
	void Frame();

	float GetTime();

	void StartTimer();
	void StopTimer();
	int GetTiming();

private:
	float mFrequency;
	INT64 mStartTime;
	float mFrameTime;
	INT64 mBeginTime, mEndTime;
};

#endif
