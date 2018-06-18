#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <DirectXMath.h>
using namespace DirectX;

class CameraClass 
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetFOV(float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMVECTOR GetLookAtVector();
	float GetFOV();

	void Render();
	XMMATRIX GetViewMatrix();

	void RenderBaseViewMatrix();
	void GetBaseViewMatrix(XMMATRIX&);
	bool CheckMovement();

private:
	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;
	float mOldPositionX, mOldPositionY, mOldPositionZ;
	float mOldRotationX, mOldRotationY, mOldRotationZ;
	XMMATRIX mViewMatrix, mBaseViewMatrix;
	XMVECTOR mLookAt;
	float mFOV;
};

#endif
