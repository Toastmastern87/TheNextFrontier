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

bool GUIClass::Initialize(ID3D11Device *device, int screenWidth, int screenHeight)
{
	bool result;

	result = InitializeBaseGUI(device, screenWidth, screenHeight);
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

bool GUIClass::Render(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	result = RenderGUI(direct3D, shaderManager, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GUIClass::InitializeBaseGUI(ID3D11Device *device, int screenWidth, int screenHeight)
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

	vertices = GeometryClass::GetPlaneVertices(((screenWidth / 2.0f) - 272.0f), ((screenHeight / 2.0f)), 272.0f, 63.0f);
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

bool GUIClass::RenderGUI(D3DClass *direct3D, ShaderManagerClass *shaderManager, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	mWorldMatrix = XMMatrixIdentity();

	for (int i = 0; i < mVertexBuffers.size(); i++) 
	{
		direct3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffers[i], &stride, &offset);
		direct3D->GetDeviceContext()->IASetIndexBuffer(mIndexBuffers[i], DXGI_FORMAT_R32_UINT, 0);
		direct3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		shaderManager->RenderGUIShader(direct3D->GetDeviceContext(), mIndexCounts[i], mWorldMatrix, viewMatrix, orthoMatrix, mResourceViews[i]);
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

void GUIClass::AddBFSPopUpGUI(ID3D11Device *device, float popUpStartPosX, float popUpStartY, float popUpWidth, float popUpHeight)
{
	vector<GeometryClass::VertexType> vertices;
	vector<int> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hResult;
	bool result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	result = LoadPopUpBaseTexture(device);
	if (!result)
	{
		return;
	}

	vertices = GeometryClass::GetPlaneVertices(popUpStartPosX, popUpStartY, popUpWidth, popUpHeight);
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
		return;
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
		return;
	}

	return;
}