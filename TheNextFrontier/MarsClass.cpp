#include "MarsClass.h"

MarsClass::MarsClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
}

MarsClass::MarsClass(const MarsClass& other)
{
}

MarsClass::~MarsClass()
{
}

bool MarsClass::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeBuffers(device);
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
	return mIndexCount;
}

bool MarsClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, j, terrainWidth, terrainHeight, index;
	XMFLOAT4 color;
	float positionX, positionZ;

	terrainWidth = 256;
	terrainHeight = 256;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	mVertexCount = 12;

	mIndexCount = 60;
	
	vertices = new VertexType[mVertexCount];
	if (!vertices) 
	{
		return false;
	}

	indices = new unsigned long[mIndexCount];
	if (!indices) 
	{
		return false;
	}

	index = 0;

	float t = (float)((1.0f + sqrt(5.0f)) / 2.0f);

	vertices[index].position = XMFLOAT3(-1.0f, t, 0.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(1.0f, t, 0.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(1.0f, t, 0.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(-1.0f, -t, 0.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(0.0f, -1.0f, t);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(0.0f, 1.0f, t);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(0.0f, -1.0f, -t);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(0.0f, 1.0f, -t);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(t, 0.0f, -1.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(t, 0.0f, 1.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(-t, 0.0f, -1.0f);
	vertices[index].color = color;
	index++;

	vertices[index].position = XMFLOAT3(-t, 0.0f, 1.0f);
	vertices[index].color = color;
	index++;

	index = 0;

	indices[index] = 0;
	index++;
	indices[index] = 11;
	index++;
	indices[index] = 5;
	index++;

	indices[index] = 0;
	index++;
	indices[index] = 5;
	index++;
	indices[index] = 1;
	index++;

	indices[index] = 0;
	index++;
	indices[index] = 1;
	index++;
	indices[index] = 7;
	index++;

	indices[index] = 0;
	index++;
	indices[index] = 7;
	index++;
	indices[index] = 10;
	index++;

	indices[index] = 0;
	index++;
	indices[index] = 1;
	index++;
	indices[index] = 11;
	index++;

	indices[index] = 1;
	index++;
	indices[index] = 5;
	index++;
	indices[index] = 9;
	index++;

	indices[index] = 5;
	index++;
	indices[index] = 11;
	index++;
	indices[index] = 4;
	index++;

	indices[index] = 11;
	index++;
	indices[index] = 10;
	index++;
	indices[index] = 2;
	index++;

	indices[index] = 10;
	index++;
	indices[index] = 7;
	index++;
	indices[index] = 6;
	index++;

	indices[index] = 7;
	index++;
	indices[index] = 1;
	index++;
	indices[index] = 8;
	index++;

	indices[index] = 3;
	index++;
	indices[index] = 9;
	index++;
	indices[index] = 4;
	index++;

	indices[index] = 3;
	index++;
	indices[index] = 4;
	index++;
	indices[index] = 2;
	index++;

	indices[index] = 3;
	index++;
	indices[index] = 2;
	index++;
	indices[index] = 6;
	index++;

	indices[index] = 3;
	index++;
	indices[index] = 6;
	index++;
	indices[index] = 8;
	index++;

	indices[index] = 3;
	index++;
	indices[index] = 8;
	index++;
	indices[index] = 9;
	index++;

	indices[index] = 4;
	index++;
	indices[index] = 9;
	index++;
	indices[index] = 5;
	index++;

	indices[index] = 2;
	index++;
	indices[index] = 4;
	index++;
	indices[index] = 11;
	index++;

	indices[index] = 6;
	index++;
	indices[index] = 2;
	index++;
	indices[index] = 10;
	index++;

	indices[index] = 8;
	index++;
	indices[index] = 6;
	index++;
	indices[index] = 7;
	index++;

	indices[index] = 9;
	index++;
	indices[index] = 8;
	index++;
	indices[index] = 1;
	index++;

	//for(j = 0; j < (terrainHeight - 1); j++)
	//{
	//	for (i = 0; i < (terrainHeight - 1); i++)
	//	{
	//		positionX = (float)i;
	//		positionZ = (float)(j + 1);

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)(i + 1);
	//		positionZ = (float)(j + 1);

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)(i + 1);
	//		positionZ = (float)(j + 1);

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)(i + 1);
	//		positionZ = (float)j;

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)(i + 1);
	//		positionZ = (float)j;

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)i;
	//		positionZ = (float)j;

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)i;
	//		positionZ = (float)j;

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		positionX = (float)i;
	//		positionZ = (float)(j + 1);

	//		vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].color = color;
	//		indices[index] = index;
	//		index++;
	//	}
	//}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
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

	delete[] vertices;
	vertices = 0;
	
	delete[] indices;
	indices = 0;

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
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1,	&mVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}