#include "LightClass.h"

LightClass::LightClass()
{
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
