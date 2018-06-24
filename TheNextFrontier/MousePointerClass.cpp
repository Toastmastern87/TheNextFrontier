#include "MousePointerClass.h"

MousePointerClass::MousePointerClass()
{
}

MousePointerClass::MousePointerClass(const MousePointerClass& other)
{
}

MousePointerClass::~MousePointerClass()
{
}

bool MousePointerClass::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int mouseX, int mouseY)
{
	bool result;

	result = InitializeMousePointer(device, deviceContext, mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	return true;
}

void MousePointerClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

bool MousePointerClass::Render(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	result = RenderMousePointer(direct3D, shaderManager, worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool MousePointerClass::InitializeMousePointer(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int mouseX, int mouseY)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hResult;
	bool result;

	mVertexCount = 6;
	mIndexCount = 6;

	result = LoadMousePointerTexture(device, deviceContext);
	if (!result) 
	{
		return false;
	}

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

	memset(vertices, 0, (sizeof(VertexType) * mVertexCount));

	for (int i = 0; i < mIndexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(hResult))
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

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	result = UpdateMousePointerPos(deviceContext, mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	return true;
}

void MousePointerClass::ShutdownBuffers()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}

	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}

	return;
}

bool MousePointerClass::UpdateMousePointerPos(ID3D11DeviceContext *deviceContext, int mouseX, int mouseY)
{
	HRESULT result;
	VertexType *vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType *verticesPtr;

	vertices = new VertexType[mVertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * mVertexCount));

	vertices[0].position = XMFLOAT3(mouseX, mouseY, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3((mouseX + 40), (mouseY - 40), 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(mouseX, (mouseY - 40), 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(mouseX, mouseY, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3((mouseX + 40), mouseY, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3((mouseX + 40), (mouseY - 40), 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	result = deviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * mVertexCount));

	deviceContext->Unmap(mVertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

bool MousePointerClass::RenderMousePointer(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	direct3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	direct3D->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	direct3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shaderManager->RenderMousePointerShader(direct3D->GetDeviceContext(), mIndexCount, worldMatrix, viewMatrix, orthoMatrix, mMousePointerResourceView);

	return true;
}

bool MousePointerClass::LoadMousePointerTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	HRESULT hResult;
	const wchar_t* fileName = L"../TheNextFrontier/MousePointer.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mMousePointerResource, &mMousePointerResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}