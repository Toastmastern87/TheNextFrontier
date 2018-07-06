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
	mZoomInSpeed = 0.0f;
	mZoomOutSpeed = 0.0f;
	mOrbitAngleXZ = 0.0f;
	mOrbitAngleY = 0.0f;
	mLookUpSpeed = 0.0f;
	mLookDownSpeed = 0.0f;
	mMaxZoomSpeed = 0.0f;
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

void PositionClass::OrbitNorth(bool keyDown) 
{
	float altitude = GetDistanceFromOrigo();

	if (keyDown) 
	{
		mOrbitAngleY += mFrameTime * 0.1;

		if (mOrbitAngleY > (2 * XM_PI))
		{
			mOrbitAngleY -= (2 * XM_PI);
		}

		mPositionX += (cosf(mOrbitAngleXZ) * altitude * cosf(mOrbitAngleY)) - mPositionX;
		mPositionY += (sinf(mOrbitAngleY) * altitude) - mPositionY;
		mPositionZ += (sinf(mOrbitAngleXZ) * altitude * cosf(mOrbitAngleY)) - mPositionZ;
	}

	return;
}

void PositionClass::OrbitSouth(bool keyDown)
{
	float altitude = GetDistanceFromOrigo();

	if (keyDown)
	{
		mOrbitAngleY -= mFrameTime * 0.1;

		if (mOrbitAngleY < 0.0f)
		{
			mOrbitAngleY += (2 * XM_PI);
		}

		mPositionX += (cosf(mOrbitAngleXZ) * altitude * cosf(mOrbitAngleY)) - mPositionX;
		mPositionY += (sinf(mOrbitAngleY) * altitude) - mPositionY;
		mPositionZ += (sinf(mOrbitAngleXZ) * altitude * cosf(mOrbitAngleY)) - mPositionZ;
	}

	return;
}

void PositionClass::ZoomOut(int mouseWheelDelta, float marsRadius)
{
	float maxSpeed; 
	XMVECTOR posVector;
	XMVECTOR posVectorNorm;

	posVector = XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ));
	posVectorNorm = XMVector3Normalize(posVector);
	maxSpeed = powf(log10f(GetDistanceFromOrigo() - marsRadius), 5.0f) * 5.0f;

	// GetDistanceFromOrigo() < MAXDISTANCEFROMORIGO shouldn't actually be happening here since it's also checked in
	// UniverseClass but it doesn't hurt to double check
	if (mouseWheelDelta < 0 && GetDistanceFromOrigo() < MAXDISTANCEFROMORIGO)
	{
		mZoomOutSpeed = powf(log10f(GetDistanceFromOrigo() - marsRadius), 5.0f) * 5.0f;

		if (mZoomOutSpeed > maxSpeed)
		{
			mZoomOutSpeed = maxSpeed;
		}
	}
	else
	{
		mZoomOutSpeed -= mFrameTime * mZoomOutSpeed * 4.0f;

		if (mZoomOutSpeed < 0.0f)
		{
			mZoomOutSpeed = 0.0f;
		}
	}

	// Updates the position values
	if (GetDistanceFromOrigo() < MAXDISTANCEFROMORIGO)
	{
		XMVECTOR movementDelta = posVectorNorm * mFrameTime * mZoomOutSpeed;

		mPositionX += XMVectorGetX(movementDelta);
		mPositionY += XMVectorGetY(movementDelta);
		mPositionZ += XMVectorGetZ(movementDelta);
	}

	// Check if max zoom is achieved and compensate position for that
	if (GetDistanceFromOrigo() > MAXDISTANCEFROMORIGO)
	{
		XMVECTOR maxPosVector = posVectorNorm * MAXDISTANCEFROMORIGO;

		mPositionX = XMVectorGetX(maxPosVector);
		mPositionY = XMVectorGetY(maxPosVector);
		mPositionZ = XMVectorGetZ(maxPosVector);
	}

	return;
}

void PositionClass::ZoomIn(int mouseWheelDelta, float marsRadius)
{
	ofstream fOut;

	fOut.open("Debug.txt", ios::out | ios::app);

	float maxSpeed;
	XMVECTOR posVector;
	XMVECTOR posVectorNorm;

	posVector = XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ));
	posVectorNorm = XMVector3Normalize(posVector);
	maxSpeed = powf(log10f(GetDistanceFromOrigo() - marsRadius), 5.0f) * 5.0f;

	fOut << "mouseWheelDelta: ";
	fOut << mouseWheelDelta;
	fOut << "\r\n";
	fOut << "GetDistanceFromOrigo(): ";
	fOut << GetDistanceFromOrigo();
	fOut << "\r\n";
	fOut << "\r\n";

	// GetDistanceFromOrigo() > MINDISTANCEFROMORIGO shouldn't actually be happening here since it's also checked in
	// UniverseClass but it doesn't hurt to double check
	if (mouseWheelDelta > 0 && GetDistanceFromOrigo() > MINDISTANCEFROMORIGO)
	{
		fOut << "mouseWheelDelta: ";
		fOut << mouseWheelDelta;
		fOut << "\r\n";
		fOut << "\r\n";

		mZoomInSpeed = powf(log10f(GetDistanceFromOrigo() - marsRadius), 5.0f) * 5.0f;

		if (mZoomInSpeed > maxSpeed)
		{
			mZoomInSpeed = maxSpeed;
		}
	}
	else
	{
		mZoomInSpeed -= mFrameTime * mZoomInSpeed * 4.0f;

		if (mZoomInSpeed < 0.0f)
		{
			mZoomInSpeed = 0.0f;
		}
	}

	// Updates the position values
	if (GetDistanceFromOrigo() > MINDISTANCEFROMORIGO)
	{
		XMVECTOR movementDelta = posVectorNorm * mFrameTime * mZoomInSpeed;

		mPositionX -= XMVectorGetX(movementDelta);
		mPositionY -= XMVectorGetY(movementDelta);
		mPositionZ -= XMVectorGetZ(movementDelta);
	}

	// Check if max zoom is achieved and compensate position for that
	if (GetDistanceFromOrigo() < MINDISTANCEFROMORIGO)
	{
		XMVECTOR minPosVector = posVectorNorm * MINDISTANCEFROMORIGO;

		mPositionX = XMVectorGetX(minPosVector);
		mPositionY = XMVectorGetY(minPosVector);
		mPositionZ = XMVectorGetZ(minPosVector);
	}

	fOut.close();

	return;

	////ofstream fOut;
	//float maxZoomSpeed, zoomAmount, positionDelta;

	////fOut.open("Debug.txt", ios::out | ios::app);

	////fOut << "mouseWheelDelta inside position: ";
	////fOut << mouseWheelDelta;
	////fOut << "\r\n";

	////fOut.close();

	//ofstream fOut;

	//fOut.open("Debug.txt", ios::out | ios::app);

	//float logMinAlt = log10f(MINALTITUDE);
	//float logMaxAlt = log10f(MAXALTITUDE);

	//float logAlt = logMinAlt + (logMaxAlt - logMinAlt) * ((GetDistanceFromOrigo() - 3389.5f) / (6605.0f - 1.0f));

	//float alt = expf(logAlt);

	//mMaxZoomSpeed = powf(3.0f, (GetDistanceFromOrigo() - 3389.5f - MINALTITUDE) / 700.0f);

	//fOut << "\r\n";
	//fOut << "altitude: ";
	//fOut << (GetDistanceFromOrigo() - 3389.5f);
	//fOut << "\r\n";
	//fOut << "mMaxZoomSpeed: ";
	//fOut << mMaxZoomSpeed;
	//fOut << "\r\n";
	//fOut << "mZoomInSpeed: ";
	//fOut << mZoomInSpeed;

	//if (mouseWheelDelta > 0)
	//{
	//	zoomAmount = mouseWheelDelta * mMaxZoomSpeed;

	//	mZoomInSpeed += mFrameTime * mMaxZoomSpeed;

	//	fOut << "\r\n";
	//	fOut << "mMaxZoomSpeed at 10km Altitude: ";
	//	fOut << powf((3399.5f - 3389.5f - MINALTITUDE) / 1000.0f, 3.0f);
	//	fOut << "\r\n";
	//	fOut << "mMaxZoomSpeed at 6610km Altitude: ";
	//	fOut << powf((10000.0f - 3389.5f - MINALTITUDE) / 1000.0f, 3.0f);
	//	fOut << "\r\n";
	//	fOut << "mMaxZoomSpeed at 5000km Altitude: ";
	//	fOut << powf((8390.0f - 3389.5f - MINALTITUDE) / 1000.0f, 3.0f);
	//	fOut << "\r\n";
	//	fOut << "mMaxZoomSpeed at 3305km Altitude: ";
	//	fOut << powf((6695.0f - 3389.5f - MINALTITUDE) / 1000.0f, 3.0f);
	//	fOut << "\r\n";
	//	fOut << "mMaxZoomSpeed at 1695km Altitude: ";
	//	fOut << powf((5000.0f - 3389.5f - MINALTITUDE) / 1000.0f, 3.0f);
	//	fOut << "\r\n";
	//	fOut << "mouseWheelDelta: ";
	//	fOut << mouseWheelDelta;
	//	fOut << "\r\n";
	//	fOut << "mFrameTime: ";
	//	fOut << mFrameTime;
	//	fOut << "\r\n";
	//	fOut << "mZoomInSpeed: ";
	//	fOut << mZoomInSpeed;
	//	fOut << "\r\n";
	//	fOut << "mFrameTime * mMaxZoomSpeed: ";
	//	fOut << mFrameTime * mMaxZoomSpeed;
	//	fOut << "\r\n";
	//	fOut << "\r\n";
	//	fOut << "\r\n";

	//	//fOut.open("Debug.txt", ios::out | ios::app);

	//	//fOut << "mouseWheelDelta: ";
	//	//fOut << mouseWheelDelta;
	//	//fOut << "\r\n";

	//	//fOut.close();

	//	//mZoomInSpeed = mFrameTime * alt * 50.0f;


	//}
	//else
	//{
	//	mZoomInSpeed -= mFrameTime * mZoomInSpeed * 2.5f;

	//	if (mZoomInSpeed < 0.0f)
	//	{
	//		mZoomInSpeed = 0.0f;
	//	}
	//}

	//if (mZoomInSpeed > mMaxZoomSpeed)
	//{
	//	fOut << "\r\n";
	//	fOut << "High zoom speed detected, setting mZoomInSpeed to: ";
	//	fOut << mMaxZoomSpeed;
	//	fOut << "\r\n";
	//	fOut << "\r\n";

	//	mZoomInSpeed = mMaxZoomSpeed;
	//}

	////fOut << "\r\n";
	////fOut << "mZoomInSpeed: ";
	////fOut << mZoomInSpeed;
	////fOut << "\r\n";

	//fOut.close();

	//float totPos = fabsf(mPositionX) + fabsf(mPositionY) + fabsf(mPositionZ);

	//if (mPositionX >= 0) 
	//{
	//	mPositionX -= (mPositionX / totPos) * mZoomInSpeed;
	//}
	//else 
	//{
	//	mPositionX += (mPositionX / totPos) * mZoomInSpeed;
	//}

	//if (mPositionY >= 0)
	//{
	//	mPositionY -= (mPositionY / totPos) * mZoomInSpeed;
	//}
	//else
	//{
	//	mPositionY += (mPositionY / totPos) * mZoomInSpeed;
	//}

	//if (mPositionZ >= 0)
	//{
	//	mPositionZ -= (mPositionZ / totPos) * mZoomInSpeed;
	//}
	//else
	//{
	//	mPositionZ += (mPositionZ / totPos) * mZoomInSpeed;
	//}

	//return;
}

void PositionClass::OrbitLeft(bool keyDown)
{
	float distanceFromOrigoXZPlane = sqrt(mPositionX * mPositionX + mPositionZ * mPositionZ);

	if (keyDown)
	{
		mOrbitAngleXZ -= mFrameTime * 0.1;

		if (mOrbitAngleXZ < 0.0f)
		{
			mOrbitAngleXZ += (2 * XM_PI);
		}

		mPositionX += (cosf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionX;
		mPositionZ += (sinf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionZ;
	}

	return;
}

void PositionClass::OrbitRight(bool keyDown)
{
	float distanceFromOrigoXZPlane = sqrt(mPositionX * mPositionX + mPositionZ * mPositionZ);

	if (keyDown)
	{
		mOrbitAngleXZ += mFrameTime * 0.1;

		if (mOrbitAngleXZ > (2 * XM_PI))
		{
			mOrbitAngleXZ -= (2 * XM_PI);
		}

		mPositionX += (cosf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionX;
		mPositionZ += (sinf(mOrbitAngleXZ) * distanceFromOrigoXZPlane) - mPositionZ;
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

XMFLOAT3 PositionClass::GetPositionXMFLOAT3() 
{
	return XMFLOAT3(mPositionX, mPositionY, mPositionZ);
}

float PositionClass::GetDistanceFromOrigo()
{
	return (sqrtf((mPositionX * mPositionX) + (mPositionY * mPositionY) + (mPositionZ * mPositionZ)));
}