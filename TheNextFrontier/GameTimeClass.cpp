#include "GameTimeClass.h"

GameTimeClass::GameTimeClass()
{
}

GameTimeClass::GameTimeClass(const GameTimeClass& other)
{
}

GameTimeClass::~GameTimeClass()
{
}

void GameTimeClass::Initialize()
{
	mGameTimeMS = 0;
	mGameTimeSecs = 0;
	mGameTimeMins = 0;
	mGameTimeHours = 0;
	mGameTimeDays = 0;
	mGameTimeMarsYears = 0;

	mSpeed = 1;

	mOldTime = timeGetTime();

	return;
}

void GameTimeClass::Frame()
{
	mGameTimeMS += (timeGetTime() - mOldTime) * mSpeed;

	mOldTime = timeGetTime();

	if (mGameTimeMS >= 1000)
	{
		int numberOfSecs = mGameTimeMS / 1000;

		mGameTimeMS -= (numberOfSecs * 1000);

		mGameTimeSecs += numberOfSecs;
	}

	if (mGameTimeSecs >= 60)
	{
		mGameTimeSecs -= 60;

		mGameTimeMins++;
	}

	if (mGameTimeMins >= 60)
	{
		mGameTimeMins -= 60;

		mGameTimeHours++;
	}

	if ((mGameTimeHours >= 24) && (mGameTimeMins >= 35) && (mGameTimeSecs >= 39))
	{
		mGameTimeHours -= 24;

		mGameTimeDays++;
	}

	if (mGameTimeDays >= 687)
	{
		mGameTimeDays -= 687;

		mGameTimeMarsYears++;
	}

	return;
}

int GameTimeClass::GetGameTimeMS()
{
	return mGameTimeMS;
}

int GameTimeClass::GetGameTimeSecs()
{
	return mGameTimeSecs;
}

int GameTimeClass::GetGameTimeMins()
{
	return mGameTimeMins;
}

int GameTimeClass::GetGameTimeHours()
{
	return mGameTimeHours;
}

int GameTimeClass::GetGameTimeDays()
{
	return mGameTimeDays;
}

int GameTimeClass::GetGameTimeMarsYears()
{
	return mGameTimeMarsYears;
}

void GameTimeClass::IncreaseGameSpeed()
{
	if (mSpeed <= 100)
	{
		mSpeed *= 10;
	}
}

void GameTimeClass::DecreaseGameSpeed()
{
	if (mSpeed > 1)
	{
		mSpeed /= 10;
	}
}