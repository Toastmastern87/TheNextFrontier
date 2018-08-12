#include "MarsAtmosphereShaderClass.h"

MarsAtmosphereShaderClass::MarsAtmosphereShaderClass()
{
	mVertexFromSpaceShader = 0;
	mPixelFromSpaceShader = 0;
	mVertexFromAtmosphereShader = 0;
	mPixelFromAtmosphereShader = 0;
	mMatrixBuffer = 0;
	mAtmosphericScatteringVSBuffer = 0;
	mAtmosphericScatteringPSBuffer = 0;
	mLayout = 0;
}

MarsAtmosphereShaderClass::MarsAtmosphereShaderClass(const MarsAtmosphereShaderClass& other)
{
}

MarsAtmosphereShaderClass::~MarsAtmosphereShaderClass()
{
}

bool MarsAtmosphereShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, (WCHAR*)L"../TheNextFrontier/MarsAtmosphere.vs", (WCHAR*)L"../TheNextFrontier/MarsAtmosphere.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void MarsAtmosphereShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

bool MarsAtmosphereShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX rotationMatrix, float marsAtmosphereRadius, bool insideAtmosphere, float marsRadius, float distanceFromOrigo, XMFLOAT3 cameraPos, XMFLOAT3 lightDirection)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, rotationMatrix, marsAtmosphereRadius, marsRadius, distanceFromOrigo, cameraPos, lightDirection);
	if (!result)
	{
		return false;
	}

	RenderShaders(deviceContext, indexCount, instanceCount, insideAtmosphere);

	return true;
}

bool MarsAtmosphereShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc, atmosphericScatteringVSBufferDesc, atmosphericScatteringPSBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "MarsAtmosphereFromSpaceVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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

	result = D3DCompileFromFile(psFilename, NULL, NULL, "MarsAtmosphereFromSpacePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
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

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexFromSpaceShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelFromSpaceShader);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&vertexShaderBuffer, sizeof(vertexShaderBuffer));
	ZeroMemory(&pixelShaderBuffer, sizeof(pixelShaderBuffer));

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "MarsAtmosphereFromAtmosphereVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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

	result = D3DCompileFromFile(psFilename, NULL, NULL, "MarsAtmosphereFromAtmospherePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
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

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexFromAtmosphereShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelFromAtmosphereShader);
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

	polygonLayout[1].SemanticName = "TEXTUREID";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32_SINT;
	polygonLayout[1].InputSlot = 1;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[1].InstanceDataStepRate = 1;

	polygonLayout[2].SemanticName = "POSITION";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	polygonLayout[3].SemanticName = "POSITION";
	polygonLayout[3].SemanticIndex = 1;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate = 1;

	polygonLayout[4].SemanticName = "POSITION";
	polygonLayout[4].SemanticIndex = 2;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 1;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[4].InstanceDataStepRate = 1;
	
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

	atmosphericScatteringVSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	atmosphericScatteringVSBufferDesc.ByteWidth = sizeof(AtmosphericScatteringVSBufferType);
	atmosphericScatteringVSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	atmosphericScatteringVSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	atmosphericScatteringVSBufferDesc.MiscFlags = 0;
	atmosphericScatteringVSBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&atmosphericScatteringVSBufferDesc, NULL, &mAtmosphericScatteringVSBuffer);
	if (FAILED(result))
	{
		return false;
	}

	atmosphericScatteringPSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	atmosphericScatteringPSBufferDesc.ByteWidth = sizeof(AtmosphericScatteringPSBufferType);
	atmosphericScatteringPSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	atmosphericScatteringPSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	atmosphericScatteringPSBufferDesc.MiscFlags = 0;
	atmosphericScatteringPSBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&atmosphericScatteringPSBufferDesc, NULL, &mAtmosphericScatteringPSBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void MarsAtmosphereShaderClass::ShutdownShader()
{
	if (mAtmosphericScatteringVSBuffer)
	{
		mAtmosphericScatteringVSBuffer->Release();
		mAtmosphericScatteringVSBuffer = 0;
	}

	if (mAtmosphericScatteringPSBuffer)
	{
		mAtmosphericScatteringPSBuffer->Release();
		mAtmosphericScatteringPSBuffer = 0;
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

void MarsAtmosphereShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

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

bool MarsAtmosphereShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX rotationMatrix, float marsAtmosphereRadius, float marsRadius, float distanceFromOrigo, XMFLOAT3 cameraPos, XMFLOAT3 lightDirection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	AtmosphericScatteringVSBufferType *atmosphericScatteringVSDataPtr;
	AtmosphericScatteringPSBufferType *atmosphericScatteringPSDataPtr;
	float km, kr, eSun;

	km = 0.00000025f;
	kr = 0.0015f;
	eSun = 15.0f;

	unsigned int bufferNumber;

	float temp[32];

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
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
	dataPtr->marsAtmosphereRadius = marsAtmosphereRadius;

	deviceContext->Unmap(mMatrixBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(mappedResource));

	result = deviceContext->Map(mAtmosphericScatteringVSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	atmosphericScatteringVSDataPtr = (AtmosphericScatteringVSBufferType*)mappedResource.pData;

	atmosphericScatteringVSDataPtr->cameraPos = XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	atmosphericScatteringVSDataPtr->lightDirection = XMFLOAT4(lightDirection.x, lightDirection.y, lightDirection.z, 1.0f);
	atmosphericScatteringVSDataPtr->invWavelength = XMFLOAT4((1.0f / powf(0.625f, 4)), (1.0f / powf(0.570f, 4)), (1.0f / powf(0.475f, 4)), 1.0f);
	atmosphericScatteringVSDataPtr->cameraHeight = XMFLOAT4(distanceFromOrigo, distanceFromOrigo, distanceFromOrigo, distanceFromOrigo);
	atmosphericScatteringVSDataPtr->cameraHeight2 = XMFLOAT4((distanceFromOrigo * distanceFromOrigo), (distanceFromOrigo * distanceFromOrigo), (distanceFromOrigo * distanceFromOrigo), (distanceFromOrigo * distanceFromOrigo));
	atmosphericScatteringVSDataPtr->atmosphereRadius = XMFLOAT4(marsRadius * 1.025f, marsRadius * 1.025f, marsRadius * 1.025f, marsRadius * 1.025f);
	atmosphericScatteringVSDataPtr->atmosphereRadius2 = XMFLOAT4((marsRadius * 1.025f * marsRadius * 1.025f), (marsRadius * 1.025f * marsRadius * 1.025f), (marsRadius * 1.025f * marsRadius * 1.025f), (marsRadius * 1.025f * marsRadius * 1.025f));
	atmosphericScatteringVSDataPtr->marsRadius = XMFLOAT4(marsRadius, marsRadius, marsRadius, marsRadius);
	atmosphericScatteringVSDataPtr->krESun = XMFLOAT4((kr * eSun), (kr * eSun), (kr * eSun), (kr * eSun));
	atmosphericScatteringVSDataPtr->kmESun = XMFLOAT4((km * eSun), (km * eSun), (km * eSun), (km * eSun));
	atmosphericScatteringVSDataPtr->kr4PI = XMFLOAT4((kr * 4 * M_PI), (kr * 4 * M_PI), (kr * 4 * M_PI), (kr * 4 * M_PI));
	atmosphericScatteringVSDataPtr->km4PI = XMFLOAT4((km * 4 * M_PI), (km * 4 * M_PI), (km * 4 * M_PI), (km * 4 * M_PI));
	atmosphericScatteringVSDataPtr->scale = XMFLOAT4((1.0f / ((marsRadius * 1.025f) - marsRadius)), (1.0f / ((marsRadius * 1.025f) - marsRadius)), (1.0f / ((marsRadius * 1.025f) - marsRadius)), (1.0f / ((marsRadius * 1.025f) - marsRadius)));
	atmosphericScatteringVSDataPtr->scaleDepth = XMFLOAT4(0.25f, 0.25f, 0.25f, 0.25f);
	atmosphericScatteringVSDataPtr->scaleOverScaleDepth = XMFLOAT4(((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f), ((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f), ((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f), ((1.0f / ((marsRadius * 1.025f) - marsRadius)) / 0.25f));
	
	deviceContext->Unmap(mAtmosphericScatteringVSBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(mappedResource));

	result = deviceContext->Map(mAtmosphericScatteringPSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	atmosphericScatteringPSDataPtr = (AtmosphericScatteringPSBufferType*)mappedResource.pData;

	atmosphericScatteringPSDataPtr->rotationMatrix = rotationMatrix;
	atmosphericScatteringPSDataPtr->lightDirection = XMFLOAT4(lightDirection.x, lightDirection.y, lightDirection.z, 1.0f);
	atmosphericScatteringPSDataPtr->g = XMFLOAT4(-0.98f, -0.98f, -0.98f, -0.98f);

	deviceContext->Unmap(mAtmosphericScatteringPSBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mAtmosphericScatteringVSBuffer);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mAtmosphericScatteringPSBuffer);

	return true;
}

void MarsAtmosphereShaderClass::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, bool insideAtmosphere)
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

	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);

	return;
}