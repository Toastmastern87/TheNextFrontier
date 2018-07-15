#include "MarsAtmosphereClass.h"

MarsAtmosphereClass::MarsAtmosphereClass()
{
	mVertexBuffer = 0;
	mInstanceBuffer = 0;
	mIndexBuffer = 0;
	mAtmosphereHeight = 100.3f;
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
	mMaxCellLevel = 3;

	GenerateCellGeometry();

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = InitializeIcosphere(marsRadius);
	if (!result)
	{
		return false;
	}

	MapCells(deviceContext);

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

	icoPositions = GetIcosadronPositions((mMarsRadius + mAtmosphereHeight));
	icoIndices = GetIcosadronIndices();

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
	float vectorDistance;
	float frac;

	mMarsAtmosphereCells.clear();

	for (auto triangle : mIcosphere)
	{
		mMarsAtmosphereCells.push_back(MarsAtmosphereCellType(triangle.level, triangle.a, triangle.b, triangle.c));
	}
}

vector<XMFLOAT3> MarsAtmosphereClass::GetIcosadronPositions(int radius)
{
	vector<XMFLOAT3> ret;

	float ratio = (float)((1.0f + sqrt(5.0f)) / 2.0f);
	float scale = radius / sqrt(ratio * ratio + 1.0f);

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

vector<int> MarsAtmosphereClass::GetIcosadronIndices()
{
	vector<int> ret
	{
		1, 3, 8,
		3, 1, 9,
		2, 0, 10,
		0, 2, 11,

		5, 7, 0,
		7, 5, 1,
		6, 4, 2,
		4, 6, 3,

		9, 11, 4,
		11, 9, 5,
		10, 8, 6,
		8, 10, 7,

		7, 1, 8,
		1, 5, 9,
		0, 7, 10,
		5, 0, 11,

		3, 6, 8,
		4, 3, 9,
		6, 2, 10,
		2, 4, 11
	};

	return ret;
}