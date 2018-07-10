#pragma once


#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <fstream>
using namespace std;

class GameTimeClass
{
public:
	GameTimeClass();
	GameTimeClass(const GameTimeClass&);
	~GameTimeClass();

	void Initialize();
	void Frame();
	int GetGameTimeMS();
	int GetGameTimeSecs();
	int GetGameTimeMins();
	int GetGameTimeHours();
	int GetGameTimeDays();
	int GetGameTimeMarsYears();
	void IncreaseGameSpeed();
	void DecreaseGameSpeed();

private:
	int mGameTimeMS, mGameTimeSecs, mGameTimeMins, mGameTimeHours, mGameTimeDays, mGameTimeMarsYears, mSpeed;
	unsigned long mOldTime;
};