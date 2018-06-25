#include "MarsClass.h"

MarsClass::MarsClass()
{
	mVertexBuffer = 0;
	mInstanceBuffer = 0;
	mIndexBuffer = 0;
	mFrustum = 0;
	mHeightMapResource = 0;
	mHeightMapResourceView = 0;
}

MarsClass::MarsClass(const MarsClass& other)
{
}

MarsClass::~MarsClass()
{
}

bool MarsClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FrustumClass* frustum, int screenWidth)
{
	bool result;

	mFrustum = frustum;
	mMarsRadius = 3389.5f;
	mMarsMaxHeight = 21.229f;
	mMarsMinHeight = -8.2f;

	mMaxTriangleSize = 400.0f;
	mMaxSubdivisionLevel = 22;
	mMaxCellLevel = 4;
	mScreenWidth = screenWidth;

	GenerateCellGeometry();

	result = LoadHeightMapTexture(device, deviceContext);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = InitializeIcosphere();
	if (!result)
	{
		return false;
	}

	return true;
}

void MarsClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void MarsClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

int MarsClass::GetIndexCount()
{
	return (int)mMarsCellIndices.size();
}

int MarsClass::GetMarsVerticesCount()
{
	return (int)mMarsCellVertices.size();
}

int MarsClass::GetInstanceCount()
{
	return (int)mMarsCells.size();
}

bool MarsClass::InitializeBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(MarsVertexType) * mMarsCellVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &mMarsCellVertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = (sizeof(MarsCellType) * 20000);
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&instanceBufferDesc, NULL, &mInstanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mMarsCellIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &mMarsCellIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool MarsClass::MapCells(ID3D11DeviceContext* deviceContext)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE resource;

	// Mapping the cell to the GPU
	hResult = deviceContext->Map(mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (hResult == S_OK)
	{
		memcpy(resource.pData, &mMarsCells[0], (sizeof(MarsCellType) * mMarsCells.size()));
		deviceContext->Unmap(mInstanceBuffer, 0);
	}

	return true;
}

void MarsClass::GenerateCellGeometry()
{
	float delta;

	mMarsCellVertices.clear();
	mMarsCellIndices.clear();

	int mRC = 1 + (int)pow(2, mMaxCellLevel);

	delta = 1 / (float)(mRC - 1);

	int rowIndex = 0;
	int nextIndex = 0;

	for (int row = 0; row < mRC; row++)
	{
		int numCols = mRC - row;

		nextIndex += numCols;

		for (int column = 0; column < numCols; column++)
		{
			//calc position
			XMFLOAT2 pos = XMFLOAT2(column / (float)(mRC - 1), row / (float)(mRC - 1));

			//calc morph factor
			XMFLOAT2 morph = XMFLOAT2(0.0f, 0.0f);
			if (row % 2 == 0)
			{
				if (column % 2 == 1) 
				{
					morph = XMFLOAT2(-delta, 0);
				} 
			}
			else
			{
				if (column % 2 == 0) 
				{ 
					morph = XMFLOAT2(0, delta); 
				}
				else 
				{ 
					morph = XMFLOAT2(delta, -delta);
				}
			}

			//create vertex
			mMarsCellVertices.push_back(MarsVertexType(pos, morph));

			//calc index
			if (row < mRC - 1 && column < numCols - 1)
			{
				mMarsCellIndices.push_back(rowIndex + column);
				mMarsCellIndices.push_back(nextIndex + column);
				mMarsCellIndices.push_back(1 + rowIndex + column);

				if (column < numCols - 2)
				{
					mMarsCellIndices.push_back(nextIndex + column);
					mMarsCellIndices.push_back(1 + nextIndex + column);
					mMarsCellIndices.push_back(1 + rowIndex + column);
				}
			}
		}

		rowIndex = nextIndex;
	}

	return;
}

bool MarsClass::InitializeIcosphere()
{
	vector<XMFLOAT3> icoPositions;
	vector<int> icoIndices;

	icoPositions = GetIcosadronPositions(mMarsRadius);
	icoIndices = GetIcosadronIndices();

	for (size_t i = 0; i < icoIndices.size(); i += 3)
	{
		mIcosphere.push_back(TriangleType(icoPositions[icoIndices[i]], icoPositions[icoIndices[i + 1]], icoPositions[icoIndices[i + 2]], nullptr, (short)0));
	}

	return true;
}

bool MarsClass::UpdateMars(ID3D11DeviceContext* deviceContext, FrustumClass* frustum, PositionClass* position)
{
	mFrustum = frustum;
	mPosition = position;

	GenerateHeightMultiLUT();

	GenerateCells();

	if ((int)mMarsCells.size() > 0) {
		MapCells(deviceContext);
	}

	return true;
}

void MarsClass::ShutdownBuffers()
{
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}

	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}

	return;
}

void MarsClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(MarsVertexType);
	strides[1] = sizeof(MarsCellType);
	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = mVertexBuffer;
	bufferPointers[1] = mInstanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void MarsClass::GenerateCells()
{
	float vectorDistance;
	float frac;

	mDistanceLUT.clear();

	vectorDistance = GetVectorDistance(mIcosphere[0].a , mIcosphere[0].b);
	frac = tanf((mMaxTriangleSize * (mFrustum->GetFOV() * (M_PI / 180.0f))) / mScreenWidth);

	for (int level = 0; level < mMaxSubdivisionLevel; level++)
	{
		mDistanceLUT.push_back(vectorDistance / frac);
		vectorDistance *= 0.5f;
	}

	mMarsCells.clear();

	for (auto triangle : mIcosphere)
	{
		RecursiveTriangle(triangle.a, triangle.b, triangle.c, triangle.level, true);
	}
}

void MarsClass::GenerateHeightMultiLUT() 
{
	XMVECTOR a, b, c, center;
	float normMaxHeight;

	mHeightMultiLUT.clear();

	a = XMLoadFloat3(&mIcosphere[0].a);
	b = XMLoadFloat3(&mIcosphere[0].b);
	c = XMLoadFloat3(&mIcosphere[0].c);

	center = (a + b + c) / 3;
	center = center * mMarsRadius / XMVector3Length(center);

	mHeightMultiLUT.push_back(1 / XMVectorGetX(XMVector3Dot(XMVector3Normalize(a), XMVector3Normalize(center))));

	normMaxHeight = 3.0f / mMarsRadius;

	for (int i = 1; i < mMaxSubdivisionLevel; i++) 
	{
		XMVECTOR A, B;

		A = b + ((c - b) * 0.5f);
		B = c + ((a - c) * 0.5f);
		c = a + ((b - a) * 0.5f);

		a = A * mMarsRadius / XMVector3Length(A);
		b = B * mMarsRadius / XMVector3Length(B);
		c = c * mMarsRadius / XMVector3Length(c);

		mHeightMultiLUT.push_back(1 / (XMVectorGetX(XMVector3Dot(XMVector3Normalize(a), XMVector3Normalize(center))) + normMaxHeight));
	}

	return;
}

float MarsClass::GetVectorLength(XMFLOAT3 vector)
{
	float length;

	length = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	return length;
}

float MarsClass::GetVectorDistance(XMFLOAT3 vector1, XMFLOAT3 vector2)
{
	float distance;

	distance = sqrt(((vector1.x - vector2.x) * (vector1.x - vector2.x)) + ((vector1.y - vector2.y) * (vector1.y - vector2.y)) + ((vector1.z - vector2.z) * (vector1.z - vector2.z)));

	return distance;
}

vector<XMFLOAT3> MarsClass::GetIcosadronPositions(int radius)
{
	vector<XMFLOAT3> ret;

	float ratio = (float)((1.0f + sqrt(5.0f)) / 2.0f);
	float scale = mMarsRadius / sqrt(ratio * ratio + 1.0f);

	ratio *= scale;

	ret.push_back(XMFLOAT3(ratio, 0.0f, -scale));
	ret.push_back(XMFLOAT3(-ratio, 0.0f, -scale));
	ret.push_back(XMFLOAT3(ratio, 0.0f, scale));
	ret.push_back(XMFLOAT3(-ratio, 0.0f, scale));

	ret.push_back(XMFLOAT3(0.0f, -scale, ratio));
	ret.push_back(XMFLOAT3(0.0f, -scale, -ratio));
	ret.push_back(XMFLOAT3(0.0f, scale, ratio));
	ret.push_back(XMFLOAT3(0.0f, scale, -ratio));

	ret.push_back(XMFLOAT3(-scale, ratio, 0.0f));
	ret.push_back(XMFLOAT3(-scale, -ratio, 0.0f));
	ret.push_back(XMFLOAT3(scale, ratio, 0.0f));
	ret.push_back(XMFLOAT3(scale, -ratio, 0.0f));

	return ret;
}

vector<int> MarsClass::GetIcosadronIndices()
{
	vector<int> ret
	{
		1, 3, 8,
		1, 3, 9,
		0, 2, 10,
		0, 2, 11,

		5, 7, 0,
		5, 7, 1,
		4, 6, 2,
		4, 6, 3,

		9, 11, 4,
		9, 11, 5,
		8, 10, 6,
		8, 10, 7,

		1, 7, 8,
		1, 5, 9,
		0, 7, 10,
		0, 5, 11,

		3, 6, 8,
		3, 4, 9,
		2, 6, 10,
		2, 4, 11
	};

	return ret;
}

MarsClass::NextTriangle MarsClass::CheckTriangleSplit(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, short level, bool frustumCull)
{
	float aDistance, bDistance, cDistance;
	float dot;
	XMFLOAT3 center, position, centerPositionSubtraction;
	XMVECTOR centerNormalized, centerPositionSubtractionNormalized;

	//Backface Culling
	center = XMFLOAT3(((a.x + b.x + c.x) / 3), ((a.y + b.y + c.y) / 3), ((a.z + b.z + c.z) / 3));
	position = mPosition->GetPositionXMFLOAT3();

	centerNormalized = XMVector3Normalize(XMLoadFloat3(&center));
	centerPositionSubtraction = XMFLOAT3((center.x - position.x), (center.y - position.y), (center.z - position.z));
	centerPositionSubtractionNormalized = XMVector3Normalize(XMLoadFloat3(&centerPositionSubtraction));

	dot = XMVectorGetX(XMVector3Dot(centerNormalized, centerPositionSubtractionNormalized));

	if (dot > 0.5f)
	{
		return NextTriangle::CULL;
	}

	//Frustum Culling
	if (frustumCull)
	{
		VolumeCheck intersect = mFrustum->CheckTriangleVolume(a, b, c, mHeightMultiLUT[level]);
		//VolumeCheck intersect = mFrustum->CheckTriangle(a, b, c);
		if (intersect == VolumeCheck::OUTSIDE) 
		{
			return NextTriangle::CULL;
		}

		// All children are also inside the frustum, so we stop checking the frustum culling
		if (intersect == VolumeCheck::CONTAINS) 
		{
			if (level > mMaxSubdivisionLevel)
			{
				return NextTriangle::LEAF;
			}

			aDistance = GetVectorDistance(a, mPosition->GetPositionXMFLOAT3());
			bDistance = GetVectorDistance(b, mPosition->GetPositionXMFLOAT3());
			cDistance = GetVectorDistance(c, mPosition->GetPositionXMFLOAT3());

			if (fminf(aDistance, fminf(bDistance, cDistance)) < mDistanceLUT[level])
			{
				return NextTriangle::SPLIT;
			}

			return NextTriangle::LEAF;
		}
	}

	if (level > mMaxSubdivisionLevel)
	{
		return NextTriangle::LEAF;
	}

	// Check distance between triangle vertex and the position of the camera
	aDistance = GetVectorDistance(a, mPosition->GetPositionXMFLOAT3());
	bDistance = GetVectorDistance(b, mPosition->GetPositionXMFLOAT3());
	cDistance = GetVectorDistance(c, mPosition->GetPositionXMFLOAT3());

	if(fminf(aDistance, fminf(bDistance, cDistance)) < mDistanceLUT[level])
	{
		return NextTriangle::SPLITCULL;
	}

	return NextTriangle::LEAF;
}

void MarsClass::RecursiveTriangle(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, short level, bool frustumCull)
{
	XMFLOAT3 A, B, C;
	NextTriangle nextTriangle;

	nextTriangle = CheckTriangleSplit(a, b, c, level, frustumCull);

	if (nextTriangle == NextTriangle::CULL)
	{
		return;
	}

	//Check if the triangle is inside the frustum
	if (nextTriangle == NextTriangle::SPLIT || nextTriangle == NextTriangle::SPLITCULL) {
		int nLevel;

		float lengthA;
		float lengthB;
		float lengthC;

		XMStoreFloat3(&A, XMLoadFloat3(&b) + XMVectorScale((XMLoadFloat3(&c) - XMLoadFloat3(&b)), 0.5f));
		XMStoreFloat3(&B, XMLoadFloat3(&c) + XMVectorScale((XMLoadFloat3(&a) - XMLoadFloat3(&c)), 0.5f));
		XMStoreFloat3(&C, XMLoadFloat3(&a) + XMVectorScale((XMLoadFloat3(&b) - XMLoadFloat3(&a)), 0.5f));

		lengthA = XMVectorGetX(XMVector3Length(XMLoadFloat3(&A)));
		lengthB = XMVectorGetX(XMVector3Length(XMLoadFloat3(&B)));
		lengthC = XMVectorGetX(XMVector3Length(XMLoadFloat3(&C)));

		XMStoreFloat3(&A, (XMLoadFloat3(&A) * (mMarsRadius / lengthA)));
		XMStoreFloat3(&B, (XMLoadFloat3(&B) * (mMarsRadius / lengthB)));
		XMStoreFloat3(&C, (XMLoadFloat3(&C) * (mMarsRadius / lengthC)));

		nLevel = level + 1;

		RecursiveTriangle(a, B, C, nLevel, nextTriangle == NextTriangle::SPLITCULL);
		RecursiveTriangle(A, b, C, nLevel, nextTriangle == NextTriangle::SPLITCULL);
		RecursiveTriangle(A, B, c, nLevel, nextTriangle == NextTriangle::SPLITCULL);
		RecursiveTriangle(A, B, C, nLevel, nextTriangle == NextTriangle::SPLITCULL);
	}
	else
	{
		XMFLOAT3 secondCorner;
		XMFLOAT3 thirdCorner;

		XMStoreFloat3(&secondCorner, XMLoadFloat3(&b) - XMLoadFloat3(&a));
		XMStoreFloat3(&thirdCorner, XMLoadFloat3(&c) - XMLoadFloat3(&a));

		mMarsCells.push_back(MarsCellType(level, a, secondCorner, thirdCorner));
	}

	return;
}

float MarsClass::GetMarsRadius()
{
	return mMarsRadius;
}

float MarsClass::GetMarsMaxHeight()
{
	return mMarsMaxHeight;
}

float MarsClass::GetMarsMinHeight()
{
	return mMarsMinHeight;
}

vector<float> MarsClass::GetDistanceLUT()
{
	return mDistanceLUT;
}

bool MarsClass::LoadHeightMapTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	HRESULT hResult;
	const wchar_t* fileName = L"../TheNextFrontier/MarsHeightMap46K.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mHeightMapResource, &mHeightMapResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

bool MarsClass::LoadNormalMapTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	HRESULT hResult;
	const wchar_t* fileName = L"../TheNextFrontier/MarsNormalMap8K.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mHeightMapResource, &mHeightMapResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* MarsClass::GetHeightMap() 
{
	return mHeightMapResourceView;
}