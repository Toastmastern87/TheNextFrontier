#pragma once

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

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetPosition();

private:
	XMFLOAT4 mAmbientColor;
	XMFLOAT4 mDiffuseColor;
	XMFLOAT3 mDirection;
	XMFLOAT3 mPosition;
};