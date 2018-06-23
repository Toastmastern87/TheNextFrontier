#include "PositionClass.h"

PositionClass::PositionClass() 
{
	mPositionX = 0.0f;
	mPositionY = 0.0f;
	mPositionZ = 0.0f;

	mRotationX = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;
	
	mFrameTime = 0.0f;

	mForwardSpeed = 0.0f;
	mBackwardSpeed = 0.0f;
	mUpwardSpeed = 0.0f;
	mDownwardSpeed = 0.0f;
	mOrbitAngleXZ = 0.0f;
	mOrbitAngleY = 0.0f;
	mLookUpSpeed = 0.0f;
	mLookDownSpeed = 0.0f;
}

PositionClass::PositionClass(const PositionClass& other) 
{
}

PositionClass::~PositionClass() 
{
}

void PositionClass::SetPosition(float x, float y, float z) 
{
	mPositionX = x;
	mPositionY = y;
	mPositionZ = z;

	return;
}

void PositionClass::SetRotation(float x, float y, float z)
{
	mRotationX = x;
	mRotationY = y;
	mRotationZ = z;

	return;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = mPositionX;
	y = mPositionY;
	z = mPositionZ;

	return;
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = mRotationX;
	y = mRotationY;
	z = mRotationZ;

	return;
}

void PositionClass::SetFrameTime(float time) 
{
	mFrameTime = time;
	return;
}

void PositionClass::MoveForward(bool keyDown) 
{
	float altitude = GetAltitude();

	if (keyDown) 
	{
		mOrbitAngleY += mFrameTime * 0.1;
	}

	if (mOrbitAngleY > (2 * XM_PI))
	{
		mOrbitAngleY -= (2 * XM_PI);
	}

	mPositionX += (cosf(mOrbitAngleXZ) * (3389.5f + altitude) * cosf(mOrbitAngleY)) - mPositionX;
	mPositionY += (sinf(mOrbitAngleY) * (3389.5f + altitude)) - mPositionY;
	mPositionZ += (sinf(mOrbitAngleXZ) * (3389.5f + altitude) * cosf(mOrbitAngleY)) - mPositionZ;

	return;
}

void PositionClass::MoveBackward(bool keyDown)
{
	float altitude = GetAltitude();

	if (keyDown)
	{
		mOrbitAngleY -= mFrameTime * 0.1;
	}

	if (mOrbitAngleY < 0.0f)
	{
		mOrbitAngleY += (2 * XM_PI);
	}

	mPositionX += (cosf(mOrbitAngleXZ) * (3389.5f + altitude) * cosf(mOrbitAngleY)) - mPositionX;
	mPositionY += (sinf(mOrbitAngleY) * (3389.5f + altitude)) - mPositionY;
	mPositionZ += (sinf(mOrbitAngleXZ) * (3389.5f + altitude) * cosf(mOrbitAngleY)) - mPositionZ;

	return;
}

void PositionClass::ZoomOut(bool keyDown)
{
	float maxSpeed;

	maxSpeed = exp(GetAltitude() / 25.0f);

	if (keyDown)
	{
		mUpwardSpeed += mFrameTime * 5.5f;

		if (mUpwardSpeed > (mFrameTime * maxSpeed))
		{
			mUpwardSpeed = mFrameTime * maxSpeed;
		}
	}
	else
	{
		mUpwardSpeed -= mFrameTime * maxSpeed * 0.5f;

		if (mUpwardSpeed < 0.0f)
		{
			mUpwardSpeed = 0.0f;
		}
	}

	float totPos = mPositionX + mPositionY + mPositionZ;

	mPositionX += (mPositionX / totPos) * mUpwardSpeed;
	mPositionY += (mPositionY / totPos) * mUpwardSpeed;
	mPositionZ += (mPositionZ / totPos) * mUpwardSpeed;

	return;
}

void PositionClass::ZoomIn(bool keyDown)
{
	float maxSpeed;

	maxSpeed = exp(GetAltitude() / 25.0f);

	if (keyDown)
	{
		mDownwardSpeed += mFrameTime * 5.5f;

		if (mDownwardSpeed > (mFrameTime * maxSpeed))
		{
			mDownwardSpeed = mFrameTime * maxSpeed;
		}
	}
	else
	{
		mDownwardSpeed -= mFrameTime * maxSpeed * 0.5f;

		if (mDownwardSpeed < 0.0f)
		{
			mDownwardSpeed = 0.0f;
		}
	}

	float totPos = mPositionX + mPositionY + mPositionZ;

	mPositionX -= (mPositionX / totPos) * mDownwardSpeed;
	mPositionY -= (mPositionY / totPos) * mDownwardSpeed;
	mPositionZ -= (mPositionZ / totPos) * mDownwardSpeed;

	return;
}

void PositionClass::OrbitLeft(bool keyDown)
{
	float distanceFromOrigoXZPlane = sqrt(mPositionX * mPositionX + mPositionZ * mPositionZ);

	if (keyDown)
	{
		mOrbitAngleXZ -= mFrameTime * 0.1;
	}

	if (mOrbitAngleXZ < 0.0f)
	{
		mOrbitAngleXZ += (2 * XM_PI);
	}

	mPositionX += (cosf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionX;
	mPositionZ += (sinf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionZ;

	return;
}

void PositionClass::OrbitRight(bool keyDown)
{
	float distanceFromOrigoXZPlane = sqrt(mPositionX * mPositionX + mPositionZ * mPositionZ);

	if (keyDown)
	{
		mOrbitAngleXZ += mFrameTime * 0.1;
	}

	if (mOrbitAngleXZ > (2 * XM_PI))
	{
		mOrbitAngleXZ -= (2 * XM_PI);
	}

	mPositionX += (cosf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionX;
	mPositionZ += (sinf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionZ;

	return;
}

void PositionClass::LookUpward(bool keyDown)
{
	if (keyDown)
	{
		mLookUpSpeed += mFrameTime * 7.5f;

		if (mLookUpSpeed > (mFrameTime * 75.0f))
		{
			mLookUpSpeed = mFrameTime * 75.0f;
		}
	}
	else
	{
		mLookUpSpeed -= mFrameTime * 2.0f;

		if (mLookUpSpeed < 0.0f)
		{
			mLookUpSpeed = 0.0f;
		}
	}

	mRotationX -= mLookUpSpeed;

	if (mRotationX > 90.0f)
	{
		mRotationX = 90.0f;
	}

	return;
}

void PositionClass::LookDownward(bool keyDown)
{
	if (keyDown)
	{
		mLookDownSpeed += mFrameTime * 7.5f;

		if (mLookDownSpeed > (mFrameTime * 75.0f))
		{
			mLookDownSpeed = mFrameTime * 75.0f;
		}
	}
	else
	{
		mLookDownSpeed -= mFrameTime * 2.0f;

		if (mLookDownSpeed < 0.0f)
		{
			mLookDownSpeed = 0.0f;
		}
	}

	mRotationX += mLookDownSpeed;

	if (mRotationX < -90.0f)
	{
		mRotationX = -90.0f;
	}

	return;
}

XMFLOAT3 PositionClass::GetPositionXMFLOAT3() 
{
	return XMFLOAT3(mPositionX, mPositionY, mPositionZ);
}

float PositionClass::GetAltitude()
{
	return (sqrtf((mPositionX * mPositionX) + (mPositionY * mPositionY) + (mPositionZ * mPositionZ)) - 3389.5f);
}