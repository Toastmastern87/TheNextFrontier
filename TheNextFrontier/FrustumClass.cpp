#include "FrustumClass.h"

FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass& other) 
{
}

FrustumClass::~FrustumClass() 
{
}

bool FrustumClass::Initialize(CameraClass* camera) 
{
	mCamera = camera;

	return true;
}

bool FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix) 
{
	float zMin, r;
	float length;
	XMFLOAT4X4 projectionMatrixXMFLOAT, viewMatrixXMFLOAT;
	XMMATRIX matrix;
	XMFLOAT4X4 matrixXMFLOAT;
	XMVECTOR tempVector;

	XMStoreFloat4x4(&projectionMatrixXMFLOAT, projectionMatrix);
	XMStoreFloat4x4(&viewMatrixXMFLOAT, viewMatrix);

	zMin = -projectionMatrixXMFLOAT._43 / projectionMatrixXMFLOAT._33;
	r = screenDepth / (screenDepth - zMin);
	projectionMatrixXMFLOAT._33 = r;
	projectionMatrixXMFLOAT._43 = -r * zMin;

	projectionMatrix = XMLoadFloat4x4(&projectionMatrixXMFLOAT);
	viewMatrix = XMLoadFloat4x4(&viewMatrixXMFLOAT);

	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	XMStoreFloat4x4(&matrixXMFLOAT, matrix);

	mPlane[0].x = matrixXMFLOAT._14 + matrixXMFLOAT._13;
	mPlane[0].y = matrixXMFLOAT._24 + matrixXMFLOAT._23;
	mPlane[0].z = matrixXMFLOAT._34 + matrixXMFLOAT._33;
	mPlane[0].w = matrixXMFLOAT._44 + matrixXMFLOAT._43;
	XMStoreFloat4(&mPlane[0], XMPlaneNormalize(XMLoadFloat4(&mPlane[0])));

	mPlane[1].x = matrixXMFLOAT._14 - matrixXMFLOAT._13;
	mPlane[1].y = matrixXMFLOAT._24 - matrixXMFLOAT._23;
	mPlane[1].z = matrixXMFLOAT._34 - matrixXMFLOAT._33;
	mPlane[1].w = matrixXMFLOAT._44 - matrixXMFLOAT._43;
	XMStoreFloat4(&mPlane[1], XMPlaneNormalize(XMLoadFloat4(&mPlane[1])));

	mPlane[2].x = matrixXMFLOAT._14 + matrixXMFLOAT._11;
	mPlane[2].y = matrixXMFLOAT._24 + matrixXMFLOAT._21;
	mPlane[2].z = matrixXMFLOAT._34 + matrixXMFLOAT._31;
	mPlane[2].w = matrixXMFLOAT._44 + matrixXMFLOAT._41;
	XMStoreFloat4(&mPlane[2], XMPlaneNormalize(XMLoadFloat4(&mPlane[2])));

	mPlane[3].x = matrixXMFLOAT._14 - matrixXMFLOAT._11;
	mPlane[3].y = matrixXMFLOAT._24 - matrixXMFLOAT._21;
	mPlane[3].z = matrixXMFLOAT._34 - matrixXMFLOAT._31;
	mPlane[3].w = matrixXMFLOAT._44 - matrixXMFLOAT._41;
	XMStoreFloat4(&mPlane[3], XMPlaneNormalize(XMLoadFloat4(&mPlane[3])));

	mPlane[4].x = matrixXMFLOAT._14 - matrixXMFLOAT._12;
	mPlane[4].y = matrixXMFLOAT._24 - matrixXMFLOAT._22;
	mPlane[4].z = matrixXMFLOAT._34 - matrixXMFLOAT._32;
	mPlane[4].w = matrixXMFLOAT._44 - matrixXMFLOAT._42;
	XMStoreFloat4(&mPlane[4], XMPlaneNormalize(XMLoadFloat4(&mPlane[4])));

	mPlane[5].x = matrixXMFLOAT._14 + matrixXMFLOAT._12;
	mPlane[5].y = matrixXMFLOAT._24 + matrixXMFLOAT._22;
	mPlane[5].z = matrixXMFLOAT._34 + matrixXMFLOAT._32;
	mPlane[5].w = matrixXMFLOAT._44 + matrixXMFLOAT._42;
	XMStoreFloat4(&mPlane[5], XMPlaneNormalize(XMLoadFloat4(&mPlane[5])));

	return true;
}

bool FrustumClass::CheckTriangle(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3) 
{
	int i;
	XMVECTOR tempPlaneVector;
	XMVECTOR tempP1Vector, tempP2Vector, tempP3Vector;

	tempP1Vector = XMLoadFloat3(&p1);
	tempP2Vector = XMLoadFloat3(&p2);
	tempP3Vector = XMLoadFloat3(&p3);

	for (i = 0; i < 6; i++)
	{
		tempPlaneVector = XMLoadFloat4(&mPlane[i]);

		if (XMVectorGetX(XMPlaneDotCoord(tempPlaneVector, tempP1Vector)) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(tempPlaneVector, tempP2Vector)) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(tempPlaneVector, tempP3Vector)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

float FrustumClass::GetFOV() 
{
	return mCamera->GetFOV();
}