#include "TextClass.h"

TextClass::TextClass() 
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
	mVertexBuffer2 = 0;
	mIndexBuffer2 = 0;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass() 
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int maxLength, bool shadow, FontClass* font, char* text, int positionX, int positionY, float red, float green, float blue)
{
	bool result;

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mMaxLength = maxLength;

	mShadow = shadow;

	result = InitializeSentence(device, deviceContext, font, text, positionX, positionY, red, green, blue);
	if (!result) 
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown() 
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

	if (mVertexBuffer2)
	{
		mVertexBuffer2->Release();
		mVertexBuffer2 = 0;
	}

	if (mIndexBuffer2)
	{
		mIndexBuffer2->Release();
		mIndexBuffer2 = 0;
	}

	return;
}

//void TextClass::Render(ID3D11DeviceContext* deviceContext, ShaderManagerClass* shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix, ID3D11ShaderResourceView* fontTexture)
//{
//	RenderSentence(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, fontTexture);
//
//	return;
//}

bool TextClass::InitializeSentence(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FontClass* font, char* text, int positionX, int positionY, float red, float green, float blue)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	mVertexCount = 6 * mMaxLength;
	mIndexCount = 6 * mMaxLength;

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

	for (i = 0; i < mIndexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
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

	if (mShadow)
	{
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer2);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer2);
		if (FAILED(result))
		{
			return false;
		}
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	result = UpdateSentence(deviceContext, font, text, positionX, positionY, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::UpdateSentence(ID3D11DeviceContext* deviceContext, FontClass* font, char* text, int positionX, int positionY, float red, float green, float blue) 
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* verticesPtr;
	HRESULT result;

	mPixelColor = XMFLOAT4(red, green, blue, 1.0f);

	numLetters = (int)strlen(text);

	if (numLetters > mMaxLength) 
	{
		return false;
	}

	vertices = new VertexType[mVertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * mVertexCount));

	drawX = (float)(((mScreenWidth / 2) * -1) + positionX);
	drawY = (float)((mScreenHeight / 2) - positionY);

	font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	result = deviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (void*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * mVertexCount));

	deviceContext->Unmap(mVertexBuffer, 0);

	if (mShadow) 
	{
		memset(vertices, 0, (sizeof(VertexType) * mVertexCount));

		drawX = (float)((((mScreenWidth / 2) * -1) + positionX) + 2);
		drawY = (float)(((mScreenHeight / 2) - positionY) - 2);

		font->BuildVertexArray((void*)vertices, text, drawX, drawY);

		result = deviceContext->Map(mVertexBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		verticesPtr = (void*)mappedResource.pData;

		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * mVertexCount));

		deviceContext->Unmap(mVertexBuffer2, 0);
	}

	delete[] vertices;
	vertices = 0;

	return true;
}

//void TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, ShaderManagerClass* shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix, ID3D11ShaderResourceView* fontTexture)
//{
//	unsigned int stride, offset;
//	XMFLOAT4 shadowColor;
//
//	stride = sizeof(VertexType);
//	offset = 0;
//
//	if (mShadow)
//	{
//		shadowColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//		deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer2, &stride, &offset);
//		deviceContext->IASetIndexBuffer(mIndexBuffer2, DXGI_FORMAT_R32_UINT, 0);
//		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		
//		ShaderManager->RenderFontShader(deviceContext, mIndexCount, worldMatrix, viewMatrix, orthoMatrix, fontTexture, shadowColor);
//	}
//
//	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
//	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	ShaderManager->RenderFontShader(deviceContext, mIndexCount, worldMatrix, viewMatrix, orthoMatrix, fontTexture, mPixelColor);
//
//	return;
//}