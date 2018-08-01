#include "StarBoxClass.h"

StarBoxClass::StarBoxClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
}

StarBoxClass::StarBoxClass(const StarBoxClass& other)
{
}

StarBoxClass::~StarBoxClass()
{
}

bool StarBoxClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void StarBoxClass::Shutdown() 
{
	ShutdownBuffers();

	return;
}

void StarBoxClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool StarBoxClass::InitializeBuffers(ID3D11Device* device)
{
	StarBoxVertexType* vertices;
	unsigned long* indices;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	HRESULT result;

	mVertexCount = 8;
	mIndicesCount = 36;

	vertices = new StarBoxVertexType[mVertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[mIndicesCount];
	if (!indices)
	{
		return false;
	}

	vertices[0].pos = XMFLOAT3(-20000.0f, -20000.0f, 20000.0f);
	vertices[1].pos = XMFLOAT3(20000.0f, -20000.0f, 20000.0f);
	vertices[2].pos = XMFLOAT3(20000.0f, -20000.0f, -20000.0f);
	vertices[3].pos = XMFLOAT3(-20000.0f, -20000.0f, -20000.0f);

	vertices[4].pos = XMFLOAT3(-20000.0f, 20000.0f, 20000.0f);
	vertices[5].pos = XMFLOAT3(20000.0f, 20000.0f, 20000.0f);
	vertices[6].pos = XMFLOAT3(20000.0f, 20000.0f, -20000.0f);
	vertices[7].pos = XMFLOAT3(-20000.0f, 20000.0f, -20000.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;

	indices[9] = 6;
	indices[10] = 7;
	indices[11] = 4;
	
	indices[12] = 0;
	indices[13] = 4;
	indices[14] = 5;

	indices[15] = 0;
	indices[16] = 1;
	indices[17] = 5;

	indices[18] = 1;
	indices[19] = 2;
	indices[20] = 6;

	indices[21] = 6;
	indices[22] = 5;
	indices[23] = 1;

	indices[24] = 2;
	indices[25] = 6;
	indices[26] = 3;

	indices[27] = 3;
	indices[28] = 7;
	indices[29] = 6;

	indices[30] = 0;
	indices[31] = 3;
	indices[32] = 7;

	indices[33] = 7;
	indices[34] = 4;
	indices[35] = 0;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(StarBoxVertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void StarBoxClass::ShutdownBuffers()
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

void StarBoxClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* bufferPointer;

	stride = sizeof(StarBoxVertexType);
	offset = 0;

	bufferPointer = mVertexBuffer;

	deviceContext->IASetVertexBuffers(0, 1, &bufferPointer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

int StarBoxClass::GetIndexCount() 
{
	return mIndicesCount;
}