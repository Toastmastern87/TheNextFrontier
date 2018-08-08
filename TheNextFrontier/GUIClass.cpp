#include "GUIClass.h"

GUIClass::GUIClass()
{
}

GUIClass::GUIClass(const GUIClass& other)
{
}

GUIClass::~GUIClass()
{
}

bool GUIClass::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight)
{
	bool result;

	result = InitializeBaseGUI(device, deviceContext, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	return true;
}

void GUIClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

bool GUIClass::Render(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	result = RenderGUI(direct3D, shaderManager, worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GUIClass::InitializeBaseGUI(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight)
{
	VertexType *vertices, *verticesPtr;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hResult;
	bool result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Game time buffers is created first
	result = LoadGameTimeTexture(device);
	if (!result)
	{
		return false;
	}

	mVertexCount.push_back(6);
	mIndexCount.push_back(6);

	vertices = new VertexType[mVertexCount[mVertexCount.size() - 1]];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[mIndexCount[mIndexCount.size() - 1]];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * mVertexCount[mVertexCount.size() - 1]));

	for (int i = 0; i < mIndexCount[mIndexCount.size() - 1]; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount[mVertexCount.size() - 1];
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mVertexBuffer.push_back(nullptr);

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer[mVertexBuffer.size() - 1]);
	if (FAILED(hResult))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount[mIndexCount.size() - 1];
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	mIndexBuffer.push_back(nullptr);

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer[mIndexCount.size() - 1]);
	if (FAILED(hResult))
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * mVertexCount[mVertexCount.size() - 1]));

	vertices[0].position = XMFLOAT3(((screenWidth / 2) - 272), (screenHeight / 2), 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3((screenWidth / 2), ((screenHeight / 2) - 63), 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(((screenWidth / 2) - 272), ((screenHeight / 2) - 63), 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(((screenWidth / 2) - 272), (screenHeight / 2), 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3((screenWidth / 2), (screenHeight / 2), 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3((screenWidth / 2), ((screenHeight / 2) - 63), 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	hResult = deviceContext->Map(mVertexBuffer[mVertexBuffer.size() - 1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * mVertexCount[mVertexCount.size() - 1]));

	deviceContext->Unmap(mVertexBuffer[mVertexBuffer.size() - 1], 0);

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void GUIClass::ShutdownBuffers()
{
	for(int i = 0; i < mVertexBuffer.size(); i++)
	{
		if (mVertexBuffer[i])
		{
			mVertexBuffer[i]->Release();
			mVertexBuffer[i] = 0;
		}
	}

	for (int i = 0; i < mIndexBuffer.size(); i++)
	{
		if (mIndexBuffer[i])
		{
			mIndexBuffer[i]->Release();
			mIndexBuffer[i] = 0;
		}
	}
	return;
}

bool GUIClass::RenderGUI(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	for (int i = 0; i < mVertexBuffer.size(); i++) 
	{
		direct3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer[i], &stride, &offset);
		direct3D->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		direct3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shaderManager->RenderGUIShader(direct3D->GetDeviceContext(), mIndexCount[i], worldMatrix, viewMatrix, orthoMatrix, mGameTimeResourceView);
	}

	return true;
}

bool GUIClass::LoadGameTimeTexture(ID3D11Device* device)
{
	HRESULT hResult;
	const wchar_t *fileName;

	fileName = L"../TheNextFrontier/Textures/GameTimeGUI.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mGameTimeResource, &mGameTimeResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

bool GUIClass::LoadPopUpBaseTexture(ID3D11Device* device)
{
	HRESULT hResult;
	const wchar_t *fileName;

	fileName = L"../TheNextFrontier/Textures/PopUpBaseTexture.tif";

	//hResult = CreateWICTextureFromFile(device, fileName, &mGameTimeResource, &mGameTimeResourceView);
	//if (FAILED(hResult))
	//{
	//	return false;
	//}

	return true;
}