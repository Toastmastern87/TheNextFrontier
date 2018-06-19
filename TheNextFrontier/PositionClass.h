#pragma once

#include <math.h>
#include <DirectXMath.h>
using namespace DirectX;

class PositionClass 
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	XMFLOAT3 GetPositionXMFLOAT3();
	void GetRotation(float&, float&, float&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

private:
	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;

	float mFrameTime;

	float mForwardSpeed, mBackwardSpeed;
	float mUpwardSpeed, mDownwardSpeed;
	float mLeftTurnSpeed, mRightTurnSpeed;
	float mLookUpSpeed, mLookDownSpeed;
};
