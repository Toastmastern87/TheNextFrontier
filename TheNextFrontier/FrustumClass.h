#pragma once

#include <DirectXMath.h>
#include "CameraClass.h"
using namespace DirectX;

enum class VolumeCheck 
{
	OUTSIDE,
	INTERSECT,
	CONTAINS
};

class FrustumClass 
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	bool Initialize(CameraClass*);
	bool ConstructFrustum(float, XMMATRIX, XMMATRIX);
	VolumeCheck CheckTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3);

	float GetFOV();

private:
	XMFLOAT4 mPlane[6];
	CameraClass* mCamera;
};