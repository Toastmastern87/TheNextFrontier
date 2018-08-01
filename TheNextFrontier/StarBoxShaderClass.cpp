#include "StarBoxShaderClass.h"

StarBoxShaderClass::StarBoxShaderClass()
{
	mVertexShader = 0;
	mPixelShader = 0;
	mLayout = 0;
	mConstantBuffer = 0;
	mSampleState = 0;
}

StarBoxShaderClass::StarBoxShaderClass(const StarBoxShaderClass& other)
{
}

StarBoxShaderClass::~StarBoxShaderClass()
{
}

bool StarBoxShaderClass::Initialize(ID3D11Device *device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void StarBoxShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

bool StarBoxShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool StarBoxShaderClass::InitializeShader(ID3D11Device *device, HWND hwnd)
{
	HRESULT hResult;
	ID3D10Blob *errorMessage;
	ID3D10Blob *vertexShaderBuffer;
	ID3D10Blob *pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	hResult = D3DReadFileToBlob(L"../x64/Debug/StarBox_vs.cso", &vertexShaderBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = D3DReadFileToBlob(L"../x64/Debug/StarBox_ps.cso", &pixelShaderBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
	if (FAILED(hResult))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hResult = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(hResult))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&constantBufferDesc, NULL, &mConstantBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hResult = device->CreateSamplerState(&samplerDesc, &mSampleState);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

void StarBoxShaderClass::ShutdownShader()
{
	if (mSampleState)
	{
		mSampleState->Release();
		mSampleState = 0;
	}

	if (mConstantBuffer)
	{
		mConstantBuffer->Release();
		mConstantBuffer = 0;
	}

	if (mLayout)
	{
		mLayout->Release();
		mLayout = 0;
	}

	if (mPixelShader)
	{
		mPixelShader->Release();
		mPixelShader = 0;
	}

	if (mVertexShader)
	{
		mVertexShader->Release();
		mVertexShader = 0;
	}

	return;
}

bool StarBoxShaderClass::SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBufferType *dataPtr;
	unsigned int bufferNumber;

	result = deviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (ConstantBufferType*)mappedResource.pData;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(mConstantBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mConstantBuffer);

	return true;
}

void StarBoxShaderClass::RenderShader(ID3D11DeviceContext *deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(mLayout);

	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &mSampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}