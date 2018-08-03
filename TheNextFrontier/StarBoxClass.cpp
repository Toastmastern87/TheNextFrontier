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

	result = LoadStarBoxTexture(device);
	if (!result)
	{
		return false;
	}

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

	mVertexCount = 14;
	mIndicesCount = 36;

	vertices = new StarBoxVertexType[mVertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].pos = XMFLOAT3(-20000.0f, 20000.0f, -20000.0f);
	vertices[0].uv = XMFLOAT2 (0.0f, 0.25f);

	vertices[1].pos = XMFLOAT3(-20000.0f, -20000.0f, -20000.0f);
	vertices[1].uv = XMFLOAT2(0.25f, 0.25f);

	vertices[2].pos = XMFLOAT3(20000.0f, 20000.0f, -20000.0f);
	vertices[2].uv = XMFLOAT2(0.0f, 0.5f);

	vertices[3].pos = XMFLOAT3(20000.0f, -20000.0f, -20000.0f);
	vertices[3].uv = XMFLOAT2(0.25f, 0.5f);

	vertices[4].pos = XMFLOAT3(-20000.0f, -20000.0f, 20000.0f);
	vertices[4].uv = XMFLOAT2(0.5f, 0.25f);

	vertices[5].pos = XMFLOAT3(20000.0f, -20000.0f, 20000.0f);
	vertices[5].uv = XMFLOAT2(0.5f, 0.5f);

	vertices[6].pos = XMFLOAT3(-20000.0f, 20000.0f, 20000.0f);
	vertices[6].uv = XMFLOAT2(0.75f, 0.25f);

	vertices[7].pos = XMFLOAT3(20000.0f, 20000.0f, 20000.0f);
	vertices[7].uv = XMFLOAT2(0.75f, 0.5f);
	
	vertices[8].pos = XMFLOAT3(-20000.0f, 20000.0f, -20000.0f);
	vertices[8].uv = XMFLOAT2(1.0f, 0.25f);

	vertices[9].pos = XMFLOAT3(20000.0f, 20000.0f, -20000.0f);
	vertices[9].uv = XMFLOAT2(1.0f, 0.5f);

	vertices[10].pos = XMFLOAT3(-20000.0f, 20000.0f, -20000.0f);
	vertices[10].uv = XMFLOAT2(0.25f, 0.0f);

	vertices[11].pos = XMFLOAT3(-20000.0f, 20000.0f, 20000.0f);
	vertices[11].uv = XMFLOAT2(0.5f, 0.0f);

	vertices[12].pos = XMFLOAT3(20000.0f, 20000.0f, -20000.0f);
	vertices[12].uv = XMFLOAT2(0.25f, 0.75f);

	vertices[13].pos = XMFLOAT3(20000.0f, 20000.0f, 20000.0f);
	vertices[13].uv = XMFLOAT2(0.5f, 0.75f);

	indices = new unsigned long[mIndicesCount]{
												0, 2, 1, 
												1, 2, 3,
												4, 5, 6, 
												5, 7, 6,
												6, 7, 8, 
												7, 9 ,8,
												1, 3, 4, 
												3, 5, 4,
												1, 11,10,
												1, 4, 11,
												3, 12, 5,
												5, 12, 13
											};
	if (!indices)
	{
		return false;
	}

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

bool StarBoxClass::LoadStarBoxTexture(ID3D11Device* device)
{
	HRESULT hResult;
	const wchar_t *fileName;

	fileName = L"../TheNextFrontier/Textures/StarBox.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mStarBoxResource, &mStarBoxResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* StarBoxClass::GetStarBoxTexture()
{
	return mStarBoxResourceView;
}