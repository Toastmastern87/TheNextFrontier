#pragma once

#include <DirectXMath.h>
#include <vector>
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
	void Transform(XMMATRIX);
	XMVECTOR nearA;
	XMVECTOR nearB;
	XMVECTOR nearC;
	XMVECTOR nearD;
	XMVECTOR farA;
	XMVECTOR farB;
	XMVECTOR farC;
	XMVECTOR farD;
};

struct Plane 
{
	XMVECTOR n, d;

	Plane(XMVECTOR a, XMVECTOR b, XMVECTOR c)
	{
		d = a;
		n = XMVector3Normalize(XMVector3Cross(-b + a, c - a));
	}
};

class FrustumClass 
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	bool Initialize(CameraClass*, PositionClass*);
	bool ConstructFrustum(float, float, float, XMMATRIX, XMMATRIX, XMMATRIX);
	VolumeCheck CheckTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3);
	VolumeCheck CheckTriangleVolume(XMFLOAT3, XMFLOAT3, XMFLOAT3, float);
	VolumeCheck CheckTriangleVolume(XMVECTOR, XMVECTOR, XMVECTOR, float);

	float GetFOV();

private:
	XMFLOAT4 mPlane[6];
	vector<Plane> mPlanes;
	CameraClass* mCamera;
	PositionClass* mPosition;
	FrustumCorners mCorners;
};