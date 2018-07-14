#include "CameraClass.h"

CameraClass::CameraClass() 
{
	mPositionX = 0.0f;
	mPositionY = 0.0f;
	mPositionZ = 0.0f;

	mRotationX = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;

	mOldPositionX = 0.0f;
	mOldPositionY = 0.0f;
	mOldPositionZ = 0.0f;

	mOldRotationX = 0.0f;
	mOldRotationY = 0.0f;
	mOldRotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other) 
{
}

CameraClass::~CameraClass() 
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	mPositionX = x;
	mPositionY = y;
	mPositionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z) 
{
	mRotationX = x;
	mRotationY = y;
	mRotationZ = z;
}

XMFLOAT3 CameraClass::GetPosition() 
{
	return XMFLOAT3(mPositionX, mPositionY, mPositionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(mRotationX, mRotationY, mRotationZ);
}

void CameraClass::Render() 
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR positionVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	mUp = XMLoadFloat3(&up);

	position.x = mPositionX;
	position.y = mPositionY;
	position.z = mPositionZ;

	positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	mLookAt = XMLoadFloat3(&lookAt);

	pitch = mRotationX * 0.0174532925f;
	yaw = mRotationY * 0.0174532925f;
	roll = mRotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	mLookAt = XMVector3TransformCoord(mLookAt, rotationMatrix);
	mUp = XMVector3TransformCoord(mUp, rotationMatrix);

	mLookAt = XMVectorAdd(positionVector, mLookAt);

	mViewMatrix = XMMatrixLookAtLH(positionVector, mLookAt, mUp);

	return;
}

XMMATRIX CameraClass::GetViewMatrix()
{
	return mViewMatrix;

}

void CameraClass::RenderBaseViewMatrix()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	position.x = mPositionX;
	position.y = mPositionY;
	position.z = mPositionZ;

	positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	mLookAt = XMLoadFloat3(&lookAt);

	pitch = mRotationX * 0.0174532925f;
	yaw = mRotationY * 0.0174532925f;
	roll = mRotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	mLookAt = XMVector3TransformCoord(mLookAt, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	mLookAt = XMVectorAdd(positionVector, mLookAt);

	mBaseViewMatrix = XMMatrixLookAtLH(positionVector, mLookAt, upVector);

	return;
}

void CameraClass::GetBaseViewMatrix(XMMATRIX& baseViewMatrix)
{
	baseViewMatrix = mBaseViewMatrix;

	return;
}

XMVECTOR CameraClass::GetLookAtVector() 
{
	return mLookAt;
}

XMVECTOR CameraClass::GetUpVector()
{
	return mUp;
}

XMVECTOR CameraClass::GetRightVector()
{
	return XMVector3Cross(mUp, mLookAt);
}

bool CameraClass::CheckMovement() 
{
	if ((mOldPositionX != mPositionX) || (mOldPositionY != mPositionY) || (mOldPositionZ != mPositionZ) || (mOldRotationX != mRotationX) || (mOldRotationY != mRotationY) || (mOldRotationZ != mRotationZ))
	{
		mOldPositionX = mPositionX;
		mOldPositionY = mPositionY;
		mOldPositionZ = mPositionZ;
		mOldRotationX = mRotationX;
		mOldRotationY = mRotationY;
		mOldRotationZ = mRotationZ;

		return true;
	}

	mOldPositionX = mPositionX;
	mOldPositionY = mPositionY;
	mOldPositionZ = mPositionZ;
	mOldRotationX = mRotationX;
	mOldRotationY = mRotationY;
	mOldRotationZ = mRotationZ;

	return false;
}

void CameraClass::SetFOV(float fov) 
{
	mFOV = fov;

	return;
}

float CameraClass::GetFOV() 
{
	return mFOV;
}