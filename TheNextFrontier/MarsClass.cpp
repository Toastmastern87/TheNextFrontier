#include "MarsClass.h"

MarsClass::MarsClass()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
	mFrustum = 0;
}

MarsClass::MarsClass(const MarsClass& other)
{
}

MarsClass::~MarsClass()
{
}

bool MarsClass::Initialize(ID3D11Device* device, FrustumClass* frustum)
{
	bool result;

	mFrustum = frustum;

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
	return (int)mMarsMesh.indices.size();
}

int MarsClass::GetVerticesCount()
{
	return (int)mMarsMesh.vertices.size();
}

bool MarsClass::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	XMFLOAT4 color;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	float ratio = (float)((1.0f + sqrt(5.0f)) / 2.0f);
	float scale = 10.0f / sqrt(ratio * ratio + 1.0f);

	ratio *= scale;

	mIcosphere.push_back(VertexType(ratio, 0.0f, -scale, color));
	mIcosphere.push_back(VertexType(-ratio, 0.0f, -scale, color));
	mIcosphere.push_back(VertexType(ratio, 0.0f, scale, color));
	mIcosphere.push_back(VertexType(-ratio, 0.0f, scale, color));

	mIcosphere.push_back(VertexType(0.0f, -scale, ratio, color));
	mIcosphere.push_back(VertexType(0.0f, -scale, -ratio, color));
	mIcosphere.push_back(VertexType(0.0f, scale, ratio, color));
	mIcosphere.push_back(VertexType(0.0f, scale, -ratio, color));

	mIcosphere.push_back(VertexType(-scale, ratio, 0.0f, color));
	mIcosphere.push_back(VertexType(-scale, -ratio, 0.0f, color));
	mIcosphere.push_back(VertexType(scale, ratio, 0.0f, color));
	mIcosphere.push_back(VertexType(scale, -ratio, 0.0f, color));

	RecursiveTriangle(mIcosphere[1], mIcosphere[3], mIcosphere[8], 0);
	RecursiveTriangle(mIcosphere[1], mIcosphere[3], mIcosphere[9], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[2], mIcosphere[10], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[2], mIcosphere[11], 0);

	RecursiveTriangle(mIcosphere[5], mIcosphere[7], mIcosphere[0], 0);
	RecursiveTriangle(mIcosphere[5], mIcosphere[7], mIcosphere[1], 0);
	RecursiveTriangle(mIcosphere[4], mIcosphere[6], mIcosphere[2], 0);
	RecursiveTriangle(mIcosphere[4], mIcosphere[6], mIcosphere[3], 0);

	RecursiveTriangle(mIcosphere[9], mIcosphere[11], mIcosphere[4], 0);
	RecursiveTriangle(mIcosphere[9], mIcosphere[11], mIcosphere[5], 0);
	RecursiveTriangle(mIcosphere[8], mIcosphere[10], mIcosphere[6], 0);
	RecursiveTriangle(mIcosphere[8], mIcosphere[10], mIcosphere[7], 0);

	RecursiveTriangle(mIcosphere[1], mIcosphere[7], mIcosphere[8], 0);
	RecursiveTriangle(mIcosphere[1], mIcosphere[5], mIcosphere[9], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[7], mIcosphere[10], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[5], mIcosphere[11], 0);

	RecursiveTriangle(mIcosphere[3], mIcosphere[6], mIcosphere[8], 0);
	RecursiveTriangle(mIcosphere[3], mIcosphere[4], mIcosphere[9], 0);
	RecursiveTriangle(mIcosphere[2], mIcosphere[6], mIcosphere[10], 0);
	RecursiveTriangle(mIcosphere[2], mIcosphere[4], mIcosphere[11], 0);

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = (sizeof(VertexType) * mMarsMesh.vertices.size()*10);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&vertexBufferDesc, NULL, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (int)mMarsMesh.indices.size()*10;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&indexBufferDesc, NULL, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool MarsClass::UpdateVertexBuffer(ID3D11DeviceContext* deviceContext, FrustumClass* frustum) 
{
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hResult;

	mFrustum = frustum;

	mMarsMesh.vertices.clear();
	mMarsMesh.indices.clear();

	RecursiveTriangle(mIcosphere[1], mIcosphere[3], mIcosphere[8], 0);
	RecursiveTriangle(mIcosphere[1], mIcosphere[3], mIcosphere[9], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[2], mIcosphere[10], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[2], mIcosphere[11], 0);

	RecursiveTriangle(mIcosphere[5], mIcosphere[7], mIcosphere[0], 0);
	RecursiveTriangle(mIcosphere[5], mIcosphere[7], mIcosphere[1], 0);
	RecursiveTriangle(mIcosphere[4], mIcosphere[6], mIcosphere[2], 0);
	RecursiveTriangle(mIcosphere[4], mIcosphere[6], mIcosphere[3], 0);

	RecursiveTriangle(mIcosphere[9], mIcosphere[11], mIcosphere[4], 0);
	RecursiveTriangle(mIcosphere[9], mIcosphere[11], mIcosphere[5], 0);
	RecursiveTriangle(mIcosphere[8], mIcosphere[10], mIcosphere[6], 0);
	RecursiveTriangle(mIcosphere[8], mIcosphere[10], mIcosphere[7], 0);

	RecursiveTriangle(mIcosphere[1], mIcosphere[7], mIcosphere[8], 0);
	RecursiveTriangle(mIcosphere[1], mIcosphere[5], mIcosphere[9], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[7], mIcosphere[10], 0);
	RecursiveTriangle(mIcosphere[0], mIcosphere[5], mIcosphere[11], 0);

	RecursiveTriangle(mIcosphere[3], mIcosphere[6], mIcosphere[8], 0);
	RecursiveTriangle(mIcosphere[3], mIcosphere[4], mIcosphere[9], 0);
	RecursiveTriangle(mIcosphere[2], mIcosphere[6], mIcosphere[10], 0);
	RecursiveTriangle(mIcosphere[2], mIcosphere[4], mIcosphere[11], 0);

	if ((int)mMarsMesh.vertices.size() > 0)
	{
		hResult = deviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if (hResult == S_OK)
		{
			memcpy(resource.pData, &mMarsMesh.vertices[0], (sizeof(VertexType) * mMarsMesh.vertices.size()));
			deviceContext->Unmap(mVertexBuffer, 0);
		}

		ZeroMemory(&resource, sizeof(resource));

		hResult = deviceContext->Map(mIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if (hResult == S_OK)
		{
			memcpy(resource.pData, &mMarsMesh.indices[0], (sizeof(unsigned long) * mMarsMesh.indices.size()));
			deviceContext->Unmap(mIndexBuffer, 0);
		}
	}

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

void MarsClass::RecursiveTriangle(VertexType a, VertexType b, VertexType c, short level)
{
	bool visible;

	//Check if the triangle is inside the frustum
	visible = mFrustum->CheckTriangle(XMFLOAT3(a.position.x, a.position.y, a.position.z), XMFLOAT3(b.position.x, b.position.y, b.position.z), XMFLOAT3(c.position.x, c.position.y, c.position.z));

	if (visible)
	{
		bool subdivide = level < 4;
		if (subdivide) 
		{
			int nLevel;

			nLevel = level + 1;
			
			VertexType A = b + ((c - b) * 0.5f);
			VertexType B = c + ((a - c) * 0.5f);
			VertexType C = a + ((b - a) * 0.5f);

			A = A * (10.0f / GetVertexTypeLength(A));
			B = B * (10.0f / GetVertexTypeLength(B));
			C = C * (10.0f / GetVertexTypeLength(C));

			RecursiveTriangle(a, B, C, nLevel);
			RecursiveTriangle(A, b, C, nLevel);
			RecursiveTriangle(A, B, c, nLevel);
			RecursiveTriangle(A, B, C, nLevel);
		}
		else 
		{
			mMarsMesh.vertices.push_back(a);
			mMarsMesh.indices.push_back(mMarsMesh.vertices.size() - 1);

			mMarsMesh.vertices.push_back(b);
			mMarsMesh.indices.push_back(mMarsMesh.vertices.size() - 1);

			mMarsMesh.vertices.push_back(c);
			mMarsMesh.indices.push_back(mMarsMesh.vertices.size() - 1);
		}
	}

	return;
}

float MarsClass::GetVertexTypeLength(VertexType vector)
{
	float length;

	length = sqrt((vector.position.x * vector.position.x) + (vector.position.y * vector.position.y) + (vector.position.z * vector.position.z));

	return length;
}