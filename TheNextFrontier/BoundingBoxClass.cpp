#include "BoundingBoxClass.h"

BoundingBoxClass::BoundingBoxClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
}

BoundingBoxClass::BoundingBoxClass(const BoundingBoxClass& other)
{
}

BoundingBoxClass::~BoundingBoxClass()
{
}

bool BoundingBoxClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void BoundingBoxClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void BoundingBoxClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool BoundingBoxClass::InitializeBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	HRESULT result;

	mVertices = GeometryClass::GetCubeVertices(1.0f);

	mIndices = GeometryClass::GetCubeIndices();

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GeometryClass::VertexType) * mVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &mVertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * mIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &mIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void BoundingBoxClass::ShutdownBuffers()
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

void BoundingBoxClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* bufferPointer;

	stride = sizeof(GeometryClass::VertexType);
	offset = 0;

	bufferPointer = mVertexBuffer;

	deviceContext->IASetVertexBuffers(0, 1, &bufferPointer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}

int BoundingBoxClass::GetIndexCount()
{
	return mIndices.size();
}