#include "MarsShaderClass.h"

MarsShaderClass::MarsShaderClass()
{
	mVertexFromSpaceShader = 0;
	mPixelFromSpaceShader = 0;
	mMatrixBuffer = 0;
	mLayout = 0;
	mLightBuffer = 0;
	mHeightBuffer = 0;
	mSampleStateHeight = 0;
	mMorphBuffer = 0;
	mAtmosphericScatteringBuffer = 0;
}

MarsShaderClass::MarsShaderClass(const MarsShaderClass& other)
{
}

MarsShaderClass::~MarsShaderClass()
{
}

bool MarsShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void MarsShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

bool MarsShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX inverserWorldMatrix, XMMATRIX rotationMatrix, float marsRadius, float marsMaxHeight, float marsMinHeight, vector<float> distanceLUT, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* heightTexture, ID3D11ShaderResourceView* heightDetail2Texture, ID3D11ShaderResourceView* colorMap, ID3D11ShaderResourceView* detailAreaTextureX, ID3D11ShaderResourceView* detailAreaTextureY, ID3D11ShaderResourceView* detailAreaTextureWH, ID3D11ShaderResourceView* craterHeightMapTexture, XMFLOAT3 lightDirection, XMFLOAT4 lightDiffuseColor, float patchDelta, bool insideAtmosphere, float distanceFromOrigo)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, inverserWorldMatrix, rotationMatrix, marsRadius,  marsMaxHeight, marsMinHeight, distanceLUT, cameraPos, heightTexture, heightDetail2Texture, colorMap, detailAreaTextureX, detailAreaTextureY, detailAreaTextureWH, craterHeightMapTexture, lightDirection, lightDiffuseColor, patchDelta, distanceFromOrigo);
	if (!result)
	{
		return false;
	}

	RenderShaders(deviceContext, indexCount, instanceCount, insideAtmosphere);

	return true;
}

bool MarsShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd)
{
	HRESULT hResult;
	ID3DBlob* errorMessage;
	ID3DBlob* vertexShaderFromSpaceBuffer, *vertexShaderFromAtmosphereBuffer;
	ID3D10Blob* pixelShaderFromSpaceBuffer, *pixelShaderFromAtmosphereBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[6];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc, morphBufferDesc, heightBufferDesc, lightBufferDesc, atmosphericScatteringBufferDesc;
	D3D11_SAMPLER_DESC samplerDescHeight;
	
	hResult = D3DReadFileToBlob(L"../x64/Debug/MarsFromSpace_vs.cso", &vertexShaderFromSpaceBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = D3DReadFileToBlob(L"../x64/Debug/MarsFromAtmosphere_vs.cso", &vertexShaderFromAtmosphereBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreateVertexShader(vertexShaderFromSpaceBuffer->GetBufferPointer(), vertexShaderFromSpaceBuffer->GetBufferSize(), nullptr, &mVertexFromSpaceShader);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreateVertexShader(vertexShaderFromAtmosphereBuffer->GetBufferPointer(), vertexShaderFromAtmosphereBuffer->GetBufferSize(), nullptr, &mVertexFromAtmosphereShader);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = D3DReadFileToBlob(L"../x64/Debug/MarsFromSpace_ps.cso", &pixelShaderFromSpaceBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = D3DReadFileToBlob(L"../x64/Debug/MarsFromAtmosphere_ps.cso", &pixelShaderFromAtmosphereBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreatePixelShader(pixelShaderFromSpaceBuffer->GetBufferPointer(), pixelShaderFromSpaceBuffer->GetBufferSize(), NULL, &mPixelFromSpaceShader);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreatePixelShader(pixelShaderFromAtmosphereBuffer->GetBufferPointer(), pixelShaderFromAtmosphereBuffer->GetBufferSize(), NULL, &mPixelFromAtmosphereShader);
	if (FAILED(hResult))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "TEXCOORD";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 1;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXTUREID";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32_SINT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = 0;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	polygonLayout[3].SemanticName = "POSITION";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate = 1;

	polygonLayout[4].SemanticName = "POSITION";
	polygonLayout[4].SemanticIndex = 1;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 1;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[4].InstanceDataStepRate = 1;

	polygonLayout[5].SemanticName = "POSITION";
	polygonLayout[5].SemanticIndex = 2;
	polygonLayout[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[5].InputSlot = 1;
	polygonLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[5].InstanceDataStepRate = 1;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hResult = device->CreateInputLayout(polygonLayout, numElements, vertexShaderFromSpaceBuffer->GetBufferPointer(), vertexShaderFromSpaceBuffer->GetBufferSize(), &mLayout);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreateInputLayout(polygonLayout, numElements, vertexShaderFromAtmosphereBuffer->GetBufferPointer(), vertexShaderFromAtmosphereBuffer->GetBufferSize(), &mLayout);
	if (FAILED(hResult))
	{
		return false;
	}

	vertexShaderFromSpaceBuffer->Release();
	vertexShaderFromSpaceBuffer = 0;
	vertexShaderFromAtmosphereBuffer->Release();
	vertexShaderFromAtmosphereBuffer = 0;

	pixelShaderFromSpaceBuffer->Release();
	pixelShaderFromSpaceBuffer = 0;
	pixelShaderFromAtmosphereBuffer->Release();
	pixelShaderFromAtmosphereBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	morphBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	morphBufferDesc.ByteWidth = sizeof(MorphBufferType);
	morphBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	morphBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	morphBufferDesc.MiscFlags = 0;
	morphBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&morphBufferDesc, NULL, &mMorphBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	heightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	heightBufferDesc.ByteWidth = sizeof(MarsHeightBufferType);
	heightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	heightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	heightBufferDesc.MiscFlags = 0;
	heightBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&heightBufferDesc, NULL, &mHeightBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	samplerDescHeight.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescHeight.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescHeight.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescHeight.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescHeight.MipLODBias = 0.0f;
	samplerDescHeight.MaxAnisotropy = 1;
	samplerDescHeight.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescHeight.BorderColor[0] = 0;
	samplerDescHeight.BorderColor[1] = 0;
	samplerDescHeight.BorderColor[2] = 0;
	samplerDescHeight.BorderColor[3] = 0;
	samplerDescHeight.MinLOD = 0;
	samplerDescHeight.MaxLOD = D3D11_FLOAT32_MAX;

	hResult = device->CreateSamplerState(&samplerDescHeight, &mSampleStateHeight);
	if (FAILED(hResult))
	{
		return false;
	}

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&lightBufferDesc, NULL, &mLightBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	atmosphericScatteringBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	atmosphericScatteringBufferDesc.ByteWidth = sizeof(AtmosphericScatteringBufferType);
	atmosphericScatteringBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	atmosphericScatteringBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	atmosphericScatteringBufferDesc.MiscFlags = 0;
	atmosphericScatteringBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&atmosphericScatteringBufferDesc, NULL, &mAtmosphericScatteringBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

void MarsShaderClass::ShutdownShader()
{
	if (mAtmosphericScatteringBuffer) 
	{
		mAtmosphericScatteringBuffer->Release();
		mAtmosphericScatteringBuffer = 0;
	}

	if (mLightBuffer)
	{
		mLightBuffer->Release();
		mLightBuffer = 0;
	}

	if (mSampleStateHeight)
	{
		mSampleStateHeight->Release();
		mSampleStateHeight = 0;
	}

	if (mHeightBuffer)
	{
		mHeightBuffer->Release();
		mHeightBuffer = 0;
	}

	if (mMorphBuffer)
	{
		mMorphBuffer->Release();
		mMorphBuffer = 0;
	}

	if (mMatrixBuffer)
	{
		mMatrixBuffer->Release();
		mMatrixBuffer = 0;
	}

	if (mLayout)
	{
		mLayout->Release();
		mLayout = 0;
	}

	if (mPixelFromSpaceShader)
	{
		mPixelFromSpaceShader->Release();
		mPixelFromSpaceShader = 0;
	}

	if (mVertexFromSpaceShader)
	{
		mVertexFromSpaceShader->Release();
		mVertexFromSpaceShader = 0;
	}

	if (mPixelFromAtmosphereShader)
	{
		mPixelFromAtmosphereShader->Release();
		mPixelFromAtmosphereShader = 0;
	}

	if (mVertexFromAtmosphereShader)
	{
		mVertexFromAtmosphereShader->Release();
		mVertexFromAtmosphereShader = 0;
	}

	return;
}

bool MarsShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX inverseWorldMatrix, XMMATRIX rotationMatrix, float marsRadius, float marsMaxHeight, float marsMinHeight, vector<float> distanceLUT, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* heightTexture, ID3D11ShaderResourceView* heightDetail2Texture, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* detailAreaTextureX, ID3D11ShaderResourceView* detailAreaTextureY, ID3D11ShaderResourceView* detailAreaTextureWH, ID3D11ShaderResourceView* craterHeightMapTexture, XMFLOAT3 lightDirection, XMFLOAT4 lightDiffuseColor, float patchDelta, float distanceFromOrigo)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	MorphBufferType *morphDataPtr;
	MarsHeightBufferType *heightDataPtr;
	LightBufferType *lightDataPtr;
	AtmosphericScatteringBufferType *atmosphericScatteringDataPtr;
	float km, kr, eSun;

	km = 0.0f;
	kr = 0.004f;
	eSun = 10.0f;

	unsigned int bufferNumber;

	float temp[32];

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	inverseWorldMatrix = XMMatrixTranspose(inverseWorldMatrix);
	rotationMatrix = XMMatrixTranspose(rotationMatrix);

	result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->rotationMatrix = rotationMatrix;
	dataPtr->marsRadius = marsRadius;

	deviceContext->Unmap(mMatrixBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(mappedResource));

	result = deviceContext->Map(mMorphBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	morphDataPtr = (MorphBufferType*)mappedResource.pData;

	morphDataPtr->cameraPos = XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

	for (int i = 0; i < distanceLUT.size(); i++) 
	{
		morphDataPtr->distanceLUT[i] = XMFLOAT4(distanceLUT[i], distanceLUT[i], distanceLUT[i], distanceLUT[i]);
	}

	morphDataPtr->morphRange = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);

	deviceContext->Unmap(mMorphBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(mappedResource));

	result = deviceContext->Map(mHeightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	heightDataPtr = (MarsHeightBufferType*)mappedResource.pData;

	heightDataPtr->marsMaxHeight = XMFLOAT4(marsMaxHeight, marsMaxHeight, marsMaxHeight, marsMaxHeight);
	heightDataPtr->marsMinHeight = XMFLOAT4(marsMinHeight, marsMinHeight, marsMinHeight, marsMinHeight);

	deviceContext->Unmap(mHeightBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(mappedResource));

	result = deviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	lightDataPtr = (LightBufferType*)mappedResource.pData;

	lightDataPtr->lightDirection = XMFLOAT4(lightDirection.x, lightDirection.y, lightDirection.z, 1.0f);
	lightDataPtr->diffuseColor = lightDiffuseColor;
	lightDataPtr->patchDelta = XMFLOAT4(patchDelta, patchDelta, patchDelta, patchDelta);
	lightDataPtr->rotationMatrix = rotationMatrix;

	deviceContext->Unmap(mLightBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(mappedResource));

	result = deviceContext->Map(mAtmosphericScatteringBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	atmosphericScatteringDataPtr = (AtmosphericScatteringBufferType*)mappedResource.pData;

	atmosphericScatteringDataPtr->lightDirection = XMFLOAT4(lightDirection.x, lightDirection.y, lightDirection.z, 1.0f);
	atmosphericScatteringDataPtr->invWavelength = XMFLOAT4((1.0f / powf(0.625f, 4)), (1.0f / powf(0.570f, 4)), (1.0f / powf(0.475f, 4)), 1.0f);
	atmosphericScatteringDataPtr->cameraHeight = XMFLOAT4(distanceFromOrigo, distanceFromOrigo, distanceFromOrigo, distanceFromOrigo);
	atmosphericScatteringDataPtr->atmosphereRadius = XMFLOAT4(marsRadius * 1.025f, marsRadius * 1.025f, marsRadius * 1.025f, marsRadius * 1.025f);
	atmosphericScatteringDataPtr->kr = XMFLOAT4(kr, kr, kr, kr);
	atmosphericScatteringDataPtr->km = XMFLOAT4(km, km, km, km);
	atmosphericScatteringDataPtr->eSun = XMFLOAT4(eSun, eSun, eSun, eSun);
	atmosphericScatteringDataPtr->scale = XMFLOAT4((1.0f / ((marsRadius * 1.025f) - marsRadius)), (1.0f / ((marsRadius * 1.025f) - marsRadius)), (1.0f / ((marsRadius * 1.025f) - marsRadius)), (1.0f / ((marsRadius * 1.025f) - marsRadius)));
	atmosphericScatteringDataPtr->scaleDepth = XMFLOAT4(0.25f, 0.25f, 0.25f, 0.25f);
	atmosphericScatteringDataPtr->scaleOverScaleDepth = XMFLOAT4(((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f), ((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f), ((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f), ((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f));

	deviceContext->Unmap(mAtmosphericScatteringBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMorphBuffer);

	bufferNumber = 2;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mHeightBuffer);

	bufferNumber = 3;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mAtmosphericScatteringBuffer);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	deviceContext->VSSetShaderResources(0, 1, &heightTexture);
	deviceContext->VSSetShaderResources(1, 1, &heightDetail2Texture);
	deviceContext->VSSetShaderResources(2, 1, &detailAreaTextureX);
	deviceContext->VSSetShaderResources(3, 1, &craterHeightMapTexture);
	deviceContext->VSSetShaderResources(4, 1, &detailAreaTextureY);
	deviceContext->VSSetShaderResources(5, 1, &detailAreaTextureWH);

	deviceContext->PSSetShaderResources(0, 1, &heightTexture);
	deviceContext->PSSetShaderResources(1, 1, &heightDetail2Texture);
	deviceContext->PSSetShaderResources(2, 1, &colorTexture);
	deviceContext->PSSetShaderResources(3, 1, &detailAreaTextureX);
	deviceContext->PSSetShaderResources(4, 1, &craterHeightMapTexture);
	deviceContext->PSSetShaderResources(5, 1, &detailAreaTextureY);
	deviceContext->PSSetShaderResources(6, 1, &detailAreaTextureWH);

	return true;
}

void MarsShaderClass::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, bool insideAtmosphere)
{
	deviceContext->IASetInputLayout(mLayout);

	if (insideAtmosphere) 
	{
		deviceContext->VSSetShader(mVertexFromAtmosphereShader, NULL, 0);
		deviceContext->PSSetShader(mPixelFromAtmosphereShader, NULL, 0);
	}
	else 
	{
		deviceContext->VSSetShader(mVertexFromSpaceShader, NULL, 0);
		deviceContext->PSSetShader(mPixelFromSpaceShader, NULL, 0);
	}

	deviceContext->VSSetSamplers(0, 1, &mSampleStateHeight);
	deviceContext->PSSetSamplers(0, 1, &mSampleStateHeight);

	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);

	return;
}