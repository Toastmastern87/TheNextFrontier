#include "BFSClass.h"

BFSClass::BFSClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
}

BFSClass::BFSClass(const BFSClass& other)
{
}

BFSClass::~BFSClass()
{
}

bool BFSClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT3 startPosition, XMFLOAT3 scale)
{
	bool result;

	mPosition = startPosition;
	mScale = scale;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	mPositionMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition));
	mScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&mScale));

	return true;
}

void BFSClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void BFSClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool BFSClass::InitializeBuffers(ID3D11Device *device)
{
	ObjLoaderClass::Object BFS;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hResult;
	bool result;
	
	BFS = ObjLoaderClass::LoadObject((char*)"../TheNextFrontier/Objects/CubeMeshTest.obj");

	//ofstream fOut;

	//fOut.open("Debug.txt", ios::out | ios::app);

	//for (int i = 0; i < BFS.vertices.size(); i++) 
	//{
	//	fOut << "BFS.vertices[";
	//	fOut << i;
	//	fOut << "] x: ";
	//	fOut << BFS.vertices[i].x;
	//	fOut << " y: ";
	//	fOut << BFS.vertices[i].y;
	//	fOut << " z: ";
	//	fOut << BFS.vertices[i].z;
	//	fOut << "\r\n";
	//}

	//fOut << "\r\n";

	//for (int i = 0; i < BFS.indices.size(); i++)
	//{
	//	fOut << "BFS.indices[";
	//	fOut << i;
	//	fOut << "]: ";
	//	fOut << BFS.indices[i];
	//	fOut << "\r\n";
	//}

	//fOut.close();

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * BFS.vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &BFS.vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * BFS.indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &BFS.indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	mIndexCount = BFS.indices.size();

	return true;
}

void BFSClass::ShutdownBuffers()
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

void BFSClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(XMFLOAT3);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

int BFSClass::GetIndexCount() 
{
	return mIndexCount;
}

XMMATRIX BFSClass::GetPositionMatrix() 
{
	return mPositionMatrix;
}

XMMATRIX BFSClass::GetScaleMatrix() 
{
	return mScaleMatrix;
}