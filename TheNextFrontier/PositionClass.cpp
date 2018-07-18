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

	mMaxZoom = false;
}

PositionClass::PositionClass(const PositionClass& other) 
{
}

PositionClass::~PositionClass() 
{
}

void PositionClass::SetPosition(float x, float y, float z) 
{
	float hypXZ;
	float hypXY;

	mPositionX = x;
	mPositionY = y;
	mPositionZ = z;

	hypXZ = sqrtf(mPositionX * mPositionX + mPositionZ * mPositionZ);
	hypXY = sqrtf(mPositionX * mPositionX + mPositionY * mPositionY);

	mOrbitAngleXZ = asinf(mPositionZ / hypXZ);

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

		mMaxZoom = false;
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

void PositionClass::ZoomIn(int mouseWheelDelta, float marsRadius, float heightAtPos)
{
	float maxSpeed, altitude;
	XMVECTOR posVector, posVectorNorm;

	mAltitude = XMVectorGetX(XMVector3Length(XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ)))) - heightAtPos;

	posVector = XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ));
	posVectorNorm = XMVector3Normalize(posVector);
	maxSpeed = powf(log10f(GetDistanceFromOrigo() - marsRadius), 5.0f) * 5.0f;

	// GetDistanceFromOrigo() > MINDISTANCEFROMORIGO shouldn't actually be happening here since it's also checked in
	// UniverseClass but it doesn't hurt to double check
	if (mouseWheelDelta > 0 && mAltitude > MINALTITUDE)
	{
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
	if (mAltitude > MINALTITUDE)
	{
		XMVECTOR movementDelta = posVectorNorm * mFrameTime * mZoomInSpeed;

		mPositionX -= XMVectorGetX(movementDelta);
		mPositionY -= XMVectorGetY(movementDelta);
		mPositionZ -= XMVectorGetZ(movementDelta);

		// Rechecking altitude after update
		mAltitude = XMVectorGetX(XMVector3Length(XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ)))) - heightAtPos;

		mMaxZoom = false;
	}

	// Check if min zoom is achieved and compensate position for that
	if (mAltitude < MINALTITUDE)
	{
		float deltaAlt = MINALTITUDE - mAltitude;

		XMVECTOR deltaPosVector = posVectorNorm * deltaAlt;

		mPositionX += XMVectorGetX(deltaPosVector);
		mPositionY += XMVectorGetY(deltaPosVector);
		mPositionZ += XMVectorGetZ(deltaPosVector);

		// Rechecking altitude after update
		mAltitude = XMVectorGetX(XMVector3Length(XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ)))) - heightAtPos;

		mZoomInSpeed = 0;

		mMaxZoom = true;
	}

	return;
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

void PositionClass::CheckAltitude(float heightAtPos) 
{
	XMVECTOR posVector, posVectorNorm, deltaPosVector;

	posVector = XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ));
	posVectorNorm = XMVector3Normalize(posVector);

	mAltitude = XMVectorGetX(XMVector3Length(XMLoadFloat3(new XMFLOAT3(mPositionX, mPositionY, mPositionZ)))) - heightAtPos;

	if (mMaxZoom) 
	{
		float deltaAlt;

		//Go Lower
		if (mAltitude > (MINALTITUDE + 0.01f))
		{
			deltaAlt = mAltitude - MINALTITUDE;

			deltaPosVector = posVectorNorm * deltaAlt;

			mPositionX -= XMVectorGetX(deltaPosVector);
			mPositionY -= XMVectorGetY(deltaPosVector);
			mPositionZ -= XMVectorGetZ(deltaPosVector);
		}
		
		//Go higher
		else if (mAltitude < (MINALTITUDE - 0.01f))
		{
			deltaAlt = MINALTITUDE - mAltitude;

			deltaPosVector = posVectorNorm * deltaAlt;

			mPositionX += XMVectorGetX(deltaPosVector);
			mPositionY += XMVectorGetY(deltaPosVector);
			mPositionZ += XMVectorGetZ(deltaPosVector);
		}
	}
}

bool PositionClass::MaxZoom()
{
	return mMaxZoom;
}

bool PositionClass::CheckIfInsideAtmosphere(float atmosphereHeight, float marsRadius, float altitude)
{
	if (altitude < (marsRadius + atmosphereHeight)) 
	{
		return true;
	}

	return false;
}