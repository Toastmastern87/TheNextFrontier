#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;

class PositionClass 
{
public:
	const float MINALTITUDE = 10.0f;
	const float MAXALTITUDE = 6615.0f;
	const float MAXDISTANCEFROMORIGO = 10000.0f;
	const float MINDISTANCEFROMORIGO = 3399.5f;
	const float ONEMOUSEWHEELTURN = 120.0f;

	float mOrbitalAngleY{ 0.0f }, mOrbitalAngleXZ{ 0.0f };

public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	XMFLOAT3 GetPositionXMFLOAT3();
	XMVECTOR GetPosition();
	void GetRotation(float&, float&, float&);
	float GetDistanceFromOrigo();
	void CheckAltitude(float);
	bool MaxZoom();

	void SetFrameTime(float);

	void ZoomOut(int, float);
	void ZoomIn(int, float, float);
	void LookUpward(bool);
	void LookDownward(bool);

	bool InsideAtmosphere(float, float, float);

	void UpdateOrbit();
	void CheckOrbitalAngles();

private:
	XMVECTOR mPosition, mRotation;

	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;

	float mFrameTime;

	float mForwardSpeed, mBackwardSpeed;
	float mZoomInSpeed, mZoomOutSpeed;
	float mAltitude;
	float mLookUpSpeed, mLookDownSpeed, mMaxZoomSpeed, mOrbitSpeed;

	bool mMaxZoom;
};