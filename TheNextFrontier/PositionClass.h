#pragma once


#include <math.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class PositionClass 
{
public:
	const float MINALTITUDE = 10.0f;
	const float MAXALTITUDE = 6615.0f;
	const float MAXDISTANCEFROMORIGO = 10000.0f;
	const float ONEMOUSEWHEELTURN = 120.0f;

public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	XMFLOAT3 GetPositionXMFLOAT3();
	void GetRotation(float&, float&, float&);
	float GetDistanceFromOrigo();

	void SetFrameTime(float);

	void OrbitNorth(bool);
	void OrbitSouth(bool);
	void ZoomOut(int);
	void ZoomIn(int);
	void OrbitLeft(bool);
	void OrbitRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

private:
	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;

	float mFrameTime;

	float mForwardSpeed, mBackwardSpeed;
	float mZoomInSpeed, mZoomOutSpeed;
	float mOrbitAngleXZ, mOrbitAngleY;
	float mLookUpSpeed, mLookDownSpeed, mMaxZoomSpeed;
};
