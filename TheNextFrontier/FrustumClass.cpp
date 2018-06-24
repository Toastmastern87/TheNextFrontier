#include "FrustumClass.h"

void FrustumCorners::Transform(XMMATRIX space)
{
	nearA = XMVector3Transform(nearA, space);
	nearB = XMVector3Transform(nearB, space);
	nearC = XMVector3Transform(nearC, space);
	nearD = XMVector3Transform(nearD, space);

	farA = XMVector3Transform(farA, space);
	farB = XMVector3Transform(farB, space);
	farC = XMVector3Transform(farC, space);
	farD = XMVector3Transform(farD, space);
}

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

bool FrustumClass::ConstructFrustum(float farPlane, float nearPlane, float aspectRatio, XMMATRIX projectionMatrix, XMMATRIX viewMatrix, XMMATRIX worldMatrix)
{
	//NEW WAY
	float normHalfWidth;
	float nearPlaneWidth, nearPlaneHeight, farPlaneWidth, farPlaneHeight;
	XMVECTOR nearCenter, farCenter;
	XMMATRIX inverseWorldMatrix;

	normHalfWidth = tanf(mCamera->GetFOV() * (XM_PI / 180.0f));

	nearPlaneWidth = normHalfWidth * nearPlane;
	nearPlaneHeight = nearPlaneWidth * aspectRatio;
	farPlaneWidth = normHalfWidth * farPlane;
	farPlaneHeight = farPlaneWidth * aspectRatio;

	nearCenter = XMLoadFloat3(&mPosition->GetPositionXMFLOAT3()) + mCamera->GetLookAtVector() * nearPlane;
	farCenter = XMLoadFloat3(&mPosition->GetPositionXMFLOAT3()) + mCamera->GetLookAtVector() * farPlane * 0.5f;

	//ofstream fOut;

	//fOut.open(Debug.txt, iosout  iosapp);
	//fOut  normHalfWidth;
	//fOut  normHalfWidth;
	//fOut  rn;
	//fOut  nearPlaneWidth;
	//fOut  nearPlaneWidth;
	//fOut  rn;
	//fOut  nearPlaneHeight;
	//fOut  nearPlaneHeight;
	//fOut  rn;
	//fOut  farPlaneWidth;
	//fOut  farPlaneWidth;
	//fOut  rn;
	//fOut  farPlaneHeight;
	//fOut  farPlaneHeight;
	//fOut  rn;
	//fOut  rn;

	//fOut.close();

	mCorners.nearA = nearCenter + mCamera->GetUpVector() * nearPlaneHeight - mCamera->GetRightVector() * nearPlaneWidth;
	mCorners.nearB = nearCenter + mCamera->GetUpVector() * nearPlaneHeight + mCamera->GetRightVector() * nearPlaneWidth;
	mCorners.nearC = nearCenter - mCamera->GetUpVector() * nearPlaneHeight - mCamera->GetRightVector() * nearPlaneWidth;
	mCorners.nearD = nearCenter - mCamera->GetUpVector() * nearPlaneHeight + mCamera->GetRightVector() * nearPlaneWidth;
	
	mCorners.farA = farCenter + mCamera->GetUpVector() * farPlaneHeight - mCamera->GetRightVector() * farPlaneWidth;
	mCorners.farB = farCenter + mCamera->GetUpVector() * farPlaneHeight + mCamera->GetRightVector() * farPlaneWidth;
	mCorners.farC = farCenter - mCamera->GetUpVector() * farPlaneHeight - mCamera->GetRightVector() * farPlaneWidth;
	mCorners.farD = farCenter - mCamera->GetUpVector() * farPlaneHeight + mCamera->GetRightVector() * farPlaneWidth;

	inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	mCorners.Transform(inverseWorldMatrix);

	mPlanes.clear();

	mPlanes.push_back(Plane(mCorners.nearA, mCorners.nearB, mCorners.nearC)); //Near
	mPlanes.push_back(Plane(mCorners.farB, mCorners.farA, mCorners.farD));// Far
	mPlanes.push_back(Plane(mCorners.farA, mCorners.nearA, mCorners.farC)); //Left
	mPlanes.push_back(Plane(mCorners.nearB, mCorners.farB, mCorners.nearD));// Right
	mPlanes.push_back(Plane(mCorners.farA, mCorners.farB, mCorners.nearA)); //Top
	mPlanes.push_back(Plane(mCorners.nearC, mCorners.nearD, mCorners.farC));// Bottom

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

	matrix = XMMatrixMultiply(XMMatrixMultiply(viewMatrix, worldMatrix), projectionMatrix);

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

VolumeCheck FrustumClass::CheckTriangleVolume(XMVECTOR p1, XMVECTOR p2, XMVECTOR p3, float height)
{
	VolumeCheck ret = VolumeCheck::CONTAINS;

	for (auto plane : mPlanes)
	{
		int rejects = 0;

		if (XMVectorGetX(XMVector3Dot(plane.n, (p1 - plane.d))) < 0.0f)
		{
			rejects++;
		}

		if (XMVectorGetX(XMVector3Dot(plane.n, (p2 - plane.d))) < 0.0f)
		{
			rejects++;
		}

		if (XMVectorGetX(XMVector3Dot(plane.n, (p3 - plane.d))) < 0.0f)
		{
			rejects++;
		}

		if (rejects = 3)
		{
			if (XMVectorGetX(XMVector3Dot(plane.n, ((p1 * height) - plane.d))) < 0.0f)
			{
				rejects++;
			}

			if (XMVectorGetX(XMVector3Dot(plane.n, ((p2 * height) - plane.d))) < 0.0f)
			{
				rejects++;
			}

			if (XMVectorGetX(XMVector3Dot(plane.n, ((p3 * height) - plane.d))) < 0.0f)
			{
				rejects++;
			}

			if (rejects = 6)
			{
				ret = VolumeCheck::OUTSIDE;
			}
			else
			{
				ret = VolumeCheck::INTERSECT;
			}
		}
		else if (rejects < 0)
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