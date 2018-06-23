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

bool FrustumClass::Initialize(CameraClass* camera, PositionClass* position) 
{
	mCamera = camera;
	mPosition = position;

	return true;
}

bool FrustumClass::ConstructFrustum(float farPlane, float nearPlane, float aspectRatio, XMMATRIX projectionMatrix, XMMATRIX viewMatrix) 
{
	//NEW WAY
	float normHalfWidth;
	float nearPlaneWidth, nearPlaneHeight, farPlaneWidth, farPlaneHeight;

	normHalfWidth = tanf(mCamera->GetFOV() * (XM_PI / 180.0f));

	nearPlaneWidth = normHalfWidth * nearPlane;
	nearPlaneHeight = nearPlaneWidth / aspectRatio;
	farPlaneWidth = normHalfWidth * farPlane;
	farPlaneHeight = farPlaneWidth / aspectRatio;


	//OLD WAY
	float zMin, r;
	float length;
	XMFLOAT4X4 projectionMatrixXMFLOAT, viewMatrixXMFLOAT;
	XMMATRIX matrix;
	XMFLOAT4X4 matrixXMFLOAT;
	XMVECTOR tempVector;

	XMStoreFloat4x4(&projectionMatrixXMFLOAT, projectionMatrix);
	XMStoreFloat4x4(&viewMatrixXMFLOAT, viewMatrix);

	zMin = -projectionMatrixXMFLOAT._43 / projectionMatrixXMFLOAT._33;
	r = farPlane / (farPlane - zMin);
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

VolumeCheck FrustumClass::CheckTriangle(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3) 
{
	int i;
	XMVECTOR tempPlaneVector;
	VolumeCheck ret = VolumeCheck::CONTAINS;

	for (i = 0; i < 6; i++)
	{
		tempPlaneVector = XMLoadFloat4(&mPlane[i]);

		int rejects = 0;

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), XMLoadFloat3(&p1))) < 0.0f)
		{
			rejects++;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), XMLoadFloat3(&p2))) < 0.0f)
		{
			rejects++;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), XMLoadFloat3(&p3))) < 0.0f)
		{
			rejects++;
		}

		if (rejects >= 3)
		{
			return VolumeCheck::OUTSIDE;
		}
		else if (rejects > 0)
		{
			ret = VolumeCheck::INTERSECT;
		}
	}

	return ret;
}

VolumeCheck FrustumClass::CheckTriangleVolume(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, float height)
{
	int i;
	XMVECTOR tempPlaneVector;
	VolumeCheck ret = VolumeCheck::CONTAINS;

	for (i = 0; i < 6; i++)
	{
		tempPlaneVector = XMLoadFloat4(&mPlane[i]);

		int rejects = 0;

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), XMLoadFloat3(&p1))) < 0.0f)
		{
			rejects++;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), XMLoadFloat3(&p2))) < 0.0f)
		{
			rejects++;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), XMLoadFloat3(&p3))) < 0.0f)
		{
			rejects++;
		}

		if (rejects >= 3)
		{
			if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), (XMLoadFloat3(&p1) * height))) < 0.0f)
			{
				rejects++;
			}

			if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), (XMLoadFloat3(&p2) * height))) < 0.0f)
			{
				rejects++;
			}

			if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&mPlane[i]), (XMLoadFloat3(&p3) * height))) < 0.0f)
			{
				rejects++;
			}

			if (rejects >= 6)
			{
				return VolumeCheck::OUTSIDE;
			}
			else 
			{
				return VolumeCheck::INTERSECT;
			}
		}
		else if (rejects > 0)
		{
			ret = VolumeCheck::INTERSECT;
		}
	}

	return ret;
}

float FrustumClass::GetFOV() 
{
	return mCamera->GetFOV();
}