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
	mOrbitAngle = 0.0f;
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
	float radians;

	if (keyDown) 
	{
		mForwardSpeed += mFrameTime * 100.0f;

		if (mForwardSpeed > (mFrameTime * 500.0f)) 
		{
			mForwardSpeed = mFrameTime * 500.0f;
		}
	}
	else 
	{
		mForwardSpeed -= mFrameTime * 0.5f;

		if (mForwardSpeed < 0.0f)
		{
			mForwardSpeed = 0.0f;
		}
	}

	radians = mRotationY * 0.0174532925f;

	mPositionX += sinf(radians) * mForwardSpeed;
	mPositionZ += cosf(radians) * mForwardSpeed;

	return;
}

void PositionClass::MoveBackward(bool keyDown)
{
	float radians;

	if (keyDown)
	{
		mBackwardSpeed += mFrameTime * 1.0f;

		if (mBackwardSpeed > (mFrameTime * 50.0f))
		{
			mBackwardSpeed = mFrameTime * 50.0f;
		}
	}
	else
	{
		mBackwardSpeed -= mFrameTime * 0.5f;

		if (mBackwardSpeed < 0.0f)
		{
			mBackwardSpeed = 0.0f;
		}
	}

	radians = mRotationY * 0.0174532925f;

	mPositionX -= sinf(radians) * mBackwardSpeed;
	mPositionZ -= cosf(radians) * mBackwardSpeed;

	return;
}

void PositionClass::MoveUpward(bool keyDown)
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

void PositionClass::MoveDownward(bool keyDown)
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
	float altitude = GetAltitude();

	if (keyDown)
	{
		mOrbitAngle -= mFrameTime * 0.1;
	}

	if (mOrbitAngle < 0.0f)
	{
		mOrbitAngle += (2 * XM_PI);
	}

	mPositionX += cosf(mOrbitAngle) * (3389.5f + altitude) - mPositionX;
	mPositionZ += sinf(mOrbitAngle) * (3389.5f + altitude) - mPositionZ;

	return;
}

void PositionClass::OrbitRight(bool keyDown)
{
	float altitude = GetAltitude();

	if (keyDown)
	{
		mOrbitAngle += mFrameTime * 0.1;
	}

	if (mOrbitAngle > (2 * XM_PI))
	{
		mOrbitAngle -= (2 * XM_PI);
	}

	mPositionX += cosf(mOrbitAngle) * (3389.5f + altitude) - mPositionX;
	mPositionZ += sinf(mOrbitAngle) * (3389.5f + altitude) - mPositionZ;

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