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
	mLeftTurnSpeed = 0.0f;
	mRightTurnSpeed = 0.0f;
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
		mForwardSpeed += mFrameTime * 1.0f;

		if (mForwardSpeed > (mFrameTime * 50.0f)) 
		{
			mForwardSpeed = mFrameTime * 50.0f;
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
	if (keyDown)
	{
		mUpwardSpeed += mFrameTime * 1.5f;

		if (mUpwardSpeed > (mFrameTime * 300.0f))
		{
			mUpwardSpeed = mFrameTime * 300.0f;
		}
	}
	else
	{
		mUpwardSpeed -= mFrameTime * 0.99f;

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
	if (keyDown)
	{
		mDownwardSpeed += mFrameTime * 1.5f;

		if (mDownwardSpeed > (mFrameTime * 300.0f))
		{
			mDownwardSpeed = mFrameTime * 300.0f;
		}
	}
	else
	{
		mDownwardSpeed -= mFrameTime * 0.99f;

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

void PositionClass::TurnLeft(bool keyDown)
{
	if (keyDown)
	{
		mLeftTurnSpeed += mFrameTime * 5.0f;

		if (mLeftTurnSpeed > (mFrameTime * 150.0f))
		{
			mLeftTurnSpeed = mFrameTime * 150.0f;
		}
	}
	else
	{
		mLeftTurnSpeed -= mFrameTime * 3.5f;

		if (mLeftTurnSpeed < 0.0f)
		{
			mLeftTurnSpeed = 0.0f;
		}
	}

	mRotationY -= mLeftTurnSpeed;

	if (mRotationY < 0.0f)
	{
		mRotationY += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keyDown)
{
	if (keyDown)
	{
		mRightTurnSpeed += mFrameTime * 5.0f;

		if (mRightTurnSpeed > (mFrameTime * 150.0f))
		{
			mRightTurnSpeed = mFrameTime * 150.0f;
		}
	}
	else
	{
		mRightTurnSpeed -= mFrameTime * 3.5f;

		if (mRightTurnSpeed < 0.0f)
		{
			mRightTurnSpeed = 0.0f;
		}
	}

	mRotationY += mRightTurnSpeed;

	if (mRotationY > 360.0f)
	{
		mRotationY -= 360.0f;
	}

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