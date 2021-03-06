#include "MarsAtmosphereClass.h"

MarsAtmosphereClass::MarsAtmosphereClass()
{
	mVertexBuffer = 0;
	mInstanceBuffer = 0;
	mIndexBuffer = 0;
	mAtmosphereHeight = 3389.5f * 0.025f;
	mMarsRadius = 0;
}

MarsAtmosphereClass::MarsAtmosphereClass(const MarsAtmosphereClass& other)
{
}

MarsAtmosphereClass::~MarsAtmosphereClass()
{
}

bool MarsAtmosphereClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float marsRadius)
{
	bool result;
	mMaxSubdivisionLevel = 3;
	mMaxCellLevel = 2;
	
	mMarsRadius = marsRadius;

	GenerateCellGeometry();

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = InitializeIcosphere((mMarsRadius + mAtmosphereHeight));
	if (!result)
	{
		return false;
	}

	return true;
}

void MarsAtmosphereClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void MarsAtmosphereClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool MarsAtmosphereClass::InitializeBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(MarsAtmosphereVertexType) * mMarsAtmosphereCellVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &mMarsAtmosphereCellVertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = (sizeof(MarsAtmosphereCellType) * 20000);
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mMarsAtmosphereCellIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &mMarsAtmosphereCellIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool MarsAtmosphereClass::MapCells(ID3D11DeviceContext* deviceContext)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE resource;

	// Mapping the cell to the GPU
	hResult = deviceContext->Map(mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (hResult == S_OK)
	{
		memcpy(resource.pData, &mMarsAtmosphereCells[0], (sizeof(MarsAtmosphereCellType) * mMarsAtmosphereCells.size()));
		deviceContext->Unmap(mInstanceBuffer, 0);
	}

	return true;
}

void MarsAtmosphereClass::GenerateCellGeometry()
{
	mMarsAtmosphereCellVertices.clear();
	mMarsAtmosphereCellIndices.clear();

	//0 inside pow is number each cell is divided levels
	int mRC = 1 + (int)pow(2, mMaxCellLevel);

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

			//create vertex
			mMarsAtmosphereCellVertices.push_back(MarsAtmosphereVertexType(pos));

			//calc index
			if (row < mRC - 1 && column < numCols - 1)
			{
				mMarsAtmosphereCellIndices.push_back(rowIndex + column);
				mMarsAtmosphereCellIndices.push_back(nextIndex + column);
				mMarsAtmosphereCellIndices.push_back(1 + rowIndex + column);

				if (column < numCols - 2)
				{
					mMarsAtmosphereCellIndices.push_back(nextIndex + column);
					mMarsAtmosphereCellIndices.push_back(1 + nextIndex + column);
					mMarsAtmosphereCellIndices.push_back(1 + rowIndex + column);
				}
			}
		}

		rowIndex = nextIndex;
	}

	return;
}

bool MarsAtmosphereClass::InitializeIcosphere(float mMarsRadius)
{
	vector<XMFLOAT3> icoPositions;
	vector<int> icoIndices;

	icoPositions = GeometryClass::GetIcosadronVertices(mMarsRadius);
	icoIndices = GeometryClass::GetIcosadronIndices();

	for (size_t i = 0; i < icoIndices.size(); i += 3)
	{
		mIcosphere.push_back(TriangleType(icoPositions[icoIndices[i]], icoPositions[icoIndices[i + 1]], icoPositions[icoIndices[i + 2]], nullptr, (short)0));
	}

	return true;
}

void MarsAtmosphereClass::ShutdownBuffers()
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

void MarsAtmosphereClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(MarsAtmosphereVertexType);
	strides[1] = sizeof(MarsAtmosphereCellType);
	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = mVertexBuffer;
	bufferPointers[1] = mInstanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void MarsAtmosphereClass::GenerateCells()
{
	mMarsAtmosphereCells.clear();

	for (auto triangle : mIcosphere)
	{
		RecursiveTriangle(triangle.a, triangle.b, triangle.c, triangle.level, true);
	}
}

void MarsAtmosphereClass::UpdateMarsAtmosphere(ID3D11DeviceContext* deviceContext, PositionClass* position)
{
	mPosition = position;

	GenerateCells();
	MapCells(deviceContext);

	return;
}

void MarsAtmosphereClass::RecursiveTriangle(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, short level, bool frustumCull)
{
	XMFLOAT3 A, B, C;

	if (level < mMaxSubdivisionLevel) {
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

		XMStoreFloat3(&A, (XMLoadFloat3(&A) * ((mAtmosphereHeight + mMarsRadius) / lengthA)));
		XMStoreFloat3(&B, (XMLoadFloat3(&B) * ((mAtmosphereHeight + mMarsRadius) / lengthB)));
		XMStoreFloat3(&C, (XMLoadFloat3(&C) * ((mAtmosphereHeight + mMarsRadius) / lengthC)));

		nLevel = level + 1;

		RecursiveTriangle(a, B, C, nLevel, false);
		RecursiveTriangle(A, b, C, nLevel, false);
		RecursiveTriangle(A, B, c, nLevel, false);
		RecursiveTriangle(C, B, A, nLevel, false);
	}
	else
	{
		XMFLOAT3 secondCorner;
		XMFLOAT3 thirdCorner;

		XMStoreFloat3(&secondCorner, XMLoadFloat3(&b) - XMLoadFloat3(&a));
		XMStoreFloat3(&thirdCorner, XMLoadFloat3(&c) - XMLoadFloat3(&a));

		mMarsAtmosphereCells.push_back(MarsAtmosphereCellType(level, a, secondCorner, thirdCorner));
	}

	return;
}

float MarsAtmosphereClass::GetAtmosphereHeight()
{
	return mAtmosphereHeight;
}

int MarsAtmosphereClass::GetIndexCount()
{
	return (int)mMarsAtmosphereCellIndices.size();
}

int MarsAtmosphereClass::GetInstanceCount()
{
	return (int)mMarsAtmosphereCells.size();
}

int MarsAtmosphereClass::GetMarsVerticesCount()
{
	return (int)mMarsAtmosphereCellVertices.size();
}