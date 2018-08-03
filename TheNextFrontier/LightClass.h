#pragma once

#define _USE_MATH_DEFINES

const float SUNROTATESPEED = (2.0f * 3.141592f) / 88775000.0f;

#include <directxmath.h>
using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);

	void CalculateDayNightCycle(int, int);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetPosition();
	XMMATRIX GetSunlightRotation();
	XMMATRIX GetSunlightOppositeRotation();

private:
	XMFLOAT4 mAmbientColor;
	XMFLOAT4 mDiffuseColor;
	XMFLOAT3 mDirection;
	XMFLOAT3 mPosition;

	XMMATRIX mRotationMatrix;
	float mSunRotateAngle;
	int mOldGameTimeMS, mOldGameTimeSec;
};