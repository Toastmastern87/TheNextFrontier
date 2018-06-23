#pragma once

#include <DirectXMath.h>
#include "CameraClass.h"
#include "PositionClass.h"
using namespace DirectX;

enum class VolumeCheck 
{
	OUTSIDE,
	INTERSECT,
	CONTAINS
};

struct FrustumCorners
{
	//void Transform(XMFLOAT4X4 space);
	XMVECTOR nearA;
	XMVECTOR nearB;
	XMVECTOR nearC;
	XMVECTOR nearD;
	XMVECTOR farA;
	XMVECTOR farB;
	XMVECTOR farC;
	XMVECTOR farD;
};

class FrustumClass 
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	bool Initialize(CameraClass*, PositionClass*);
	bool ConstructFrustum(float, float, float, XMMATRIX, XMMATRIX);
	VolumeCheck CheckTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3);
	VolumeCheck CheckTriangleVolume(XMFLOAT3, XMFLOAT3, XMFLOAT3, float);

	float GetFOV();

private:
	XMFLOAT4 mPlane[6];
	CameraClass* mCamera;
	PositionClass* mPosition;
};