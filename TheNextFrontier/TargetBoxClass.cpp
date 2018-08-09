#include "TargetBoxClass.h"

TargetBoxClass::TargetBoxClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
}

TargetBoxClass::TargetBoxClass(const TargetBoxClass& other)
{
}

TargetBoxClass::~TargetBoxClass()
{
}

bool TargetBoxClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TargetBoxClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void TargetBoxClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool TargetBoxClass::InitializeBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	HRESULT result;

	mVertices = GeometryClass::GetCubeVertices(1.0f);

	mIndices = GeometryClass::GetCubeIndices();

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GeometryClass::VertexType3D) * mVertices.size();
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

void TargetBoxClass::ShutdownBuffers()
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

void TargetBoxClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* bufferPointer;

	stride = sizeof(GeometryClass::VertexType3D);
	offset = 0;

	bufferPointer = mVertexBuffer;

	deviceContext->IASetVertexBuffers(0, 1, &bufferPointer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

int TargetBoxClass::GetIndicesCount()
{
	return mIndices.size();
}

bool TargetBoxClass::LoadTexture(ID3D11Device* device)
{
	HRESULT hResult;
	const wchar_t *fileName;

	fileName = L"../TheNextFrontier/Textures/TargetBox.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mTextureResource, &mTextureResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* TargetBoxClass::GetTexture()
{
	return mTextureResourceView;
}
