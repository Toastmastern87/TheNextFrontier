#include "BFSClass.h"

BFSClass::BFSClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;

	mTargetBox = 0;
}

BFSClass::BFSClass(const BFSClass& other)
{
}

BFSClass::~BFSClass()
{
}

bool BFSClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT3 startPosition, XMFLOAT3 scale, XMFLOAT3 rotation, float rotationRadians)
{
	bool result;

	mPosition = startPosition;
	mScale = scale;
	mRotation = rotation;

	mBoundingOrientedBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));

	mTargetBox = new TargetBoxClass();
	if (!mTargetBox) 
	{
		return false;
	}

	result = mTargetBox->Initialize(device, deviceContext);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	mPositionMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition));
	mScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&mScale));
	mRotationMatrix = XMMatrixRotationNormal(XMLoadFloat3(&mRotation), rotationRadians);

	mBoundingOrientedBox.Transform(mBoundingOrientedBox, mScaleMatrix);
	mBoundingOrientedBox.Transform(mBoundingOrientedBox, mRotationMatrix);
	mBoundingOrientedBox.Transform(mBoundingOrientedBox, mPositionMatrix);

	//Debugging
	//XMFLOAT3 *corners = new XMFLOAT3(1.0f, 1.0f, 1.0f);
	//mBoundingOrientedBox.GetCorners(corners);

	//ofstream fOut;

	//fOut.open("Debug.txt", ios::out | ios::app);

	//for (int i = 0; i < 8; i++) 
	//{
	//	fOut << "corner[";
	//	fOut << i;
	//	fOut << "] x: ";
	//	fOut << corners[i].x;
	//	fOut << " y: ";
	//	fOut << corners[i].y;
	//	fOut << " z: ";
	//	fOut << corners[i].z;
	//	fOut << "\r\n";
	//}

	//fOut.close();

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
	
	//ofstream fOut;

	//fOut.open("Debug.txt", ios::out | ios::app);

	BFS = ObjLoaderClass::LoadObject((char*)"../TheNextFrontier/Objects/CylinderMeshTest.obj");

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

	//for (int i = 0; i < BFS.uv.size(); i++)
	//{
	//	fOut << "BFS.uvs[";
	//	fOut << i;
	//	fOut << "] x: ";
	//	fOut << BFS.uv[i].x;
	//	fOut << " y: ";
	//	fOut << BFS.uv[i].y;
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
	if (mTargetBox)
	{
		mTargetBox->Shutdown();
	}

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

bool BFSClass::LoadTexture(ID3D11Device* device)
{
	HRESULT hResult;
	const wchar_t *fileName;

	fileName = L"../TheNextFrontier/Textures/BFS.tif";

	hResult = CreateWICTextureFromFile(device, fileName, &mTextureResource, &mTextureResourceView);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

int BFSClass::GetIndicesCount() 
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

XMFLOAT3 BFSClass::GetScale() 
{
	return mScale;
}

XMMATRIX BFSClass::GetRotationMatrix()
{
	return mRotationMatrix;
}

ID3D11ShaderResourceView* BFSClass::GetTexture()
{
	return mTextureResourceView;
}

TargetBoxClass* BFSClass::GetTargetBox()
{
	return mTargetBox;
}

bool BFSClass::IsPicked() 
{
	return mPicked;
}

void BFSClass::CheckRayIntersection(XMVECTOR origin, XMVECTOR dir)
{
	float dist;

	mPicked = mBoundingOrientedBox.Intersects(origin, XMVector3Normalize(dir), dist);

	return;
}

XMFLOAT3* BFSClass::GetBoundingOrientBoxCorners() 
{
	XMFLOAT3 *retCorners = NULL;
	mBoundingOrientedBox.GetCorners(retCorners);

	return retCorners;
}