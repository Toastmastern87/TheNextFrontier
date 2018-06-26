#include "MarsShaderClass.h"

MarsShaderClass::MarsShaderClass()
{
	mVertexShader = 0;
	mPixelShader = 0;
	mMatrixBuffer = 0;
	mLayout = 0;
	mLightBuffer = 0;
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

	result = InitializeShader(device, hwnd, (WCHAR*)L"../TheNextFrontier/Mars.vs", (WCHAR*)L"../TheNextFrontier/Mars.ps");
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

bool MarsShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX inverserWorldMatrix, float marsRadius, float marsMaxHeight, float marsMinHeight, vector<float> distanceLUT, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* heightTexture, XMFLOAT4 lightDirection, XMFLOAT4 lightDiffuseColor, float patchDelta, ID3D11ShaderResourceView* normalTexture)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, inverserWorldMatrix, marsRadius,  marsMaxHeight, marsMinHeight, distanceLUT, cameraPos, heightTexture, lightDirection, lightDiffuseColor, patchDelta, normalTexture);
	if (!result)
	{
		return false;
	}

	RenderShaders(deviceContext, indexCount, instanceCount);

	return true;
}

bool MarsShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[6];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc, morphBufferDesc, heightBufferDesc, lightBufferDesc;
	D3D11_SAMPLER_DESC samplerDescHeight, samplerDescNormal;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "MarsVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(psFilename, NULL, NULL, "MarsPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
	if (FAILED(result))
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

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	morphBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	morphBufferDesc.ByteWidth = sizeof(MorphBufferType);
	morphBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	morphBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	morphBufferDesc.MiscFlags = 0;
	morphBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&morphBufferDesc, NULL, &mMorphBuffer);
	if (FAILED(result))
	{
		return false;
	}

	heightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	heightBufferDesc.ByteWidth = sizeof(MarsHeightBufferType);
	heightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	heightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	heightBufferDesc.MiscFlags = 0;
	heightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&heightBufferDesc, NULL, &mHeightBuffer);
	if (FAILED(result))
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

	result = device->CreateSamplerState(&samplerDescHeight, &mSampleStateHeight);
	if (FAILED(result))
	{
		return false;
	}

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &mLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	samplerDescNormal.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescNormal.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescNormal.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescNormal.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescNormal.MipLODBias = 0.0f;
	samplerDescNormal.MaxAnisotropy = 1;
	samplerDescNormal.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescNormal.BorderColor[0] = 0;
	samplerDescNormal.BorderColor[1] = 0;
	samplerDescNormal.BorderColor[2] = 0;
	samplerDescNormal.BorderColor[3] = 0;
	samplerDescNormal.MinLOD = 0;
	samplerDescNormal.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDescNormal, &mSampleStateNormal);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void MarsShaderClass::ShutdownShader()
{
	if (mLightBuffer)
	{
		mLightBuffer->Release();
		mLightBuffer = 0;
	}

	if (mSampleStateNormal)
	{
		mSampleStateNormal->Release();
		mSampleStateNormal = 0;
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

void MarsShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("Shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader. check Shader-error.txt for message", shaderFilename, MB_OK);

	return;
}

bool MarsShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX inverseWorldMatrix, float marsRadius, float marsMaxHeight, float marsMinHeight, vector<float> distanceLUT, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* heightTexture, XMFLOAT4 lightDirection, XMFLOAT4 lightDiffuseColor, float patchDelta, ID3D11ShaderResourceView* normalTexture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	MorphBufferType *morphDataPtr;
	MarsHeightBufferType *heightDataPtr;
	LightBufferType *lightDataPtr;

	unsigned int bufferNumber;

	float temp[32];

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	inverseWorldMatrix = XMMatrixTranspose(inverseWorldMatrix);

	result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->inverseWorld = inverseWorldMatrix;
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

	result = deviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	lightDataPtr = (LightBufferType*)mappedResource.pData;

	lightDataPtr->lightDirection = lightDirection;
	lightDataPtr->diffuseColor = lightDiffuseColor;
	lightDataPtr->patchDelta = XMFLOAT4(patchDelta, patchDelta, patchDelta, patchDelta);

	deviceContext->Unmap(mLightBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMorphBuffer);

	bufferNumber = 2;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mHeightBuffer);

	bufferNumber = 3;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	deviceContext->VSSetShaderResources(0, 1, &heightTexture);

	deviceContext->PSSetShaderResources(0, 1, &normalTexture);

	return true;
}

void MarsShaderClass::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount)
{
	deviceContext->IASetInputLayout(mLayout);

	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);

	deviceContext->VSSetSamplers(0, 1, &mSampleStateHeight);

	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);

	return;
}