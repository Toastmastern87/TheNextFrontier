#include "LightClass.h"

LightClass::LightClass()
{
	mSunRotateAngle = -2.0f;
}

LightClass::LightClass(const LightClass& other)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	mAmbientColor = XMFLOAT4(red, green, blue, alpha);

	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	mDiffuseColor = XMFLOAT4(red, green, blue, alpha);

	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	mDirection = XMFLOAT3(x, y, z);

	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);

	return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return mAmbientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return mDiffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return mDirection;
}

XMFLOAT3 LightClass::GetPosition()
{
	return mPosition;
}

void LightClass::CalculateDayNightCycle(int gameTimeMS, int gameTimeSec)
{
	int timeDiff;

	timeDiff = 0;

	if (mSunRotateAngle > (2 * M_PI))
	{
		mSunRotateAngle -= (2 * M_PI);
	}

	if (mOldGameTimeMS > gameTimeMS)
	{
		timeDiff += (1000 - mOldGameTimeMS) + gameTimeMS;
	}
	else
	{
		timeDiff += gameTimeMS - mOldGameTimeMS;

		if (mOldGameTimeSec > gameTimeSec)
		{
			timeDiff += ((60 - mOldGameTimeSec) + gameTimeSec) * 1000;
		}
		else
		{
			timeDiff += (gameTimeSec - mOldGameTimeSec) * 1000;
		}
	}

	mOldGameTimeMS = gameTimeMS;
	mOldGameTimeSec = gameTimeSec;

	mSunRotateAngle += timeDiff * SUNROTATESPEED;

	mRotationMatrix = XMMATRIX(cosf(-mSunRotateAngle), 0.0f, sinf(-mSunRotateAngle), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinf(-mSunRotateAngle), 0.0f, cosf(-mSunRotateAngle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return;
}

XMMATRIX LightClass::GetSunlightRotation()
{
	return mRotationMatrix;
}