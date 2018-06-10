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
	return (int)mMarsMesh.indices.size();
}

bool MarsClass::InitializeBuffers(ID3D11Device* device)
{
	vector<VertexType> icosphere;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	XMFLOAT4 color;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	float ratio = (float)((1.0f + sqrt(5.0f)) / 2.0f);
	float scale = 10.0f / sqrt(ratio * ratio + 1.0f);

	ratio *= scale;

	icosphere.push_back(VertexType(ratio, 0.0f, -scale, color));
	icosphere.push_back(VertexType(-ratio, 0.0f, -scale, color));
	icosphere.push_back(VertexType(ratio, 0.0f, scale, color));
	icosphere.push_back(VertexType(-ratio, 0.0f, scale, color));

	icosphere.push_back(VertexType(0.0f, -scale, ratio, color));
	icosphere.push_back(VertexType(0.0f, -scale, -ratio, color));
	icosphere.push_back(VertexType(0.0f, scale, ratio, color));
	icosphere.push_back(VertexType(0.0f, scale, -ratio, color));

	icosphere.push_back(VertexType(-scale, ratio, 0.0f, color));
	icosphere.push_back(VertexType(-scale, -ratio, 0.0f, color));
	icosphere.push_back(VertexType(scale, ratio, 0.0f, color));
	icosphere.push_back(VertexType(scale, -ratio, 0.0f, color));

	RecursiveTriangle(icosphere[1], icosphere[3], icosphere[8], 0);
	RecursiveTriangle(icosphere[1], icosphere[3], icosphere[9], 0);
	RecursiveTriangle(icosphere[0], icosphere[2], icosphere[10], 0);
	RecursiveTriangle(icosphere[0], icosphere[2], icosphere[11], 0);

	RecursiveTriangle(icosphere[5], icosphere[7], icosphere[0], 0);
	RecursiveTriangle(icosphere[5], icosphere[7], icosphere[1], 0);
	RecursiveTriangle(icosphere[4], icosphere[6], icosphere[2], 0);
	RecursiveTriangle(icosphere[4], icosphere[6], icosphere[3], 0);

	RecursiveTriangle(icosphere[9], icosphere[11], icosphere[4], 0);
	RecursiveTriangle(icosphere[9], icosphere[11], icosphere[5], 0);
	RecursiveTriangle(icosphere[8], icosphere[10], icosphere[6], 0);
	RecursiveTriangle(icosphere[8], icosphere[10], icosphere[7], 0);

	RecursiveTriangle(icosphere[1], icosphere[7], icosphere[8], 0);
	RecursiveTriangle(icosphere[1], icosphere[5], icosphere[9], 0);
	RecursiveTriangle(icosphere[0], icosphere[7], icosphere[10], 0);
	RecursiveTriangle(icosphere[0], icosphere[5], icosphere[11], 0);

	RecursiveTriangle(icosphere[3], icosphere[6], icosphere[8], 0);
	RecursiveTriangle(icosphere[3], icosphere[4], icosphere[9], 0);
	RecursiveTriangle(icosphere[2], icosphere[6], icosphere[10], 0);
	RecursiveTriangle(icosphere[2], icosphere[4], icosphere[11], 0);


	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (int)mMarsMesh.vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &mMarsMesh.vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result)) 
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (int)mMarsMesh.indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &mMarsMesh.indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
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

	visible = true;

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