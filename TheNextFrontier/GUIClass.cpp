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
	vector<GeometryClass::VertexType> vertices;
	vector<int> indices;
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

	vertices = GeometryClass::GetPlaneVertices(272.0f, 63.0f, ((screenWidth / 2.0f) - 272.0f), ((screenHeight / 2.0f) - 63.0f));
	mVertexCounts.push_back(vertices.size());

	indices = GeometryClass::GetPlaneIndices();
	mIndexCounts.push_back(indices.size());

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GeometryClass::VertexType) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mVertexBuffers.push_back(nullptr);

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffers[mVertexBuffers.size() - 1]);
	if (FAILED(hResult))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCounts[mIndexCounts.size() - 1];
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	mIndexBuffers.push_back(nullptr);

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffers[mIndexCounts.size() - 1]);
	if (FAILED(hResult))
	{
		return false;
	}

	//vertices[0].position = XMFLOAT3(((screenWidth / 2) - 272), (screenHeight / 2), 0.0f);
	//vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	//vertices[1].position = XMFLOAT3((screenWidth / 2), ((screenHeight / 2) - 63), 0.0f);
	//vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	//vertices[2].position = XMFLOAT3(((screenWidth / 2) - 272), ((screenHeight / 2) - 63), 0.0f);
	//vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	//vertices[3].position = XMFLOAT3((screenWidth / 2), (screenHeight / 2), 0.0f);
	//vertices[3].texture = XMFLOAT2(1.0f, 0.0f);

	//hResult = deviceContext->Map(mVertexBuffers[mVertexBuffers.size() - 1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(hResult))
	//{
	//	return false;
	//}

	return true;
}

void GUIClass::ShutdownBuffers()
{
	for(int i = 0; i < mVertexBuffers.size(); i++)
	{
		if (mVertexBuffers[i])
		{
			mVertexBuffers[i]->Release();
			mVertexBuffers[i] = 0;
		}
	}

	for (int i = 0; i < mIndexBuffers.size(); i++)
	{
		if (mIndexBuffers[i])
		{
			mIndexBuffers[i]->Release();
			mIndexBuffers[i] = 0;
		}
	}
	return;
}

bool GUIClass::RenderGUI(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	for (int i = 0; i < mVertexBuffers.size(); i++) 
	{
		direct3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffers[i], &stride, &offset);
		direct3D->GetDeviceContext()->IASetIndexBuffer(mIndexBuffers[i], DXGI_FORMAT_R32_UINT, 0);
		direct3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		shaderManager->RenderGUIShader(direct3D->GetDeviceContext(), mIndexCounts[i], worldMatrix, viewMatrix, orthoMatrix, mResourceViews[i]);
	}

	return true;
}

bool GUIClass::LoadGameTimeTexture(ID3D11Device* device)
{
	HRESULT hResult;
	const wchar_t *fileName;

	fileName = L"../TheNextFrontier/Textures/GameTimeGUI.tif";

	mResources.push_back(nullptr);
	mResourceViews.push_back(nullptr);

	hResult = CreateWICTextureFromFile(device, fileName, &mResources[mResources.size() - 1], &mResourceViews[mResourceViews.size() - 1]);
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

	mResources.push_back(nullptr);
	mResourceViews.push_back(nullptr);

	hResult = CreateWICTextureFromFile(device, fileName, &mResources[mResources.size() - 1], &mResourceViews[mResourceViews.size() - 1]);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}