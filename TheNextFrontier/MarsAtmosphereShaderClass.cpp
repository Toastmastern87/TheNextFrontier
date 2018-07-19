#include "MarsAtmosphereShaderClass.h"

MarsAtmosphereShaderClass::MarsAtmosphereShaderClass()
{
	mVertexFromSpaceShader = 0;
	mPixelFromSpaceShader = 0;
	mVertexFromAtmosphereShader = 0;
	mPixelFromAtmosphereShader = 0;
	mMatrixBuffer = 0;
	mAtmosphericScatteringBuffer = 0;
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

bool MarsAtmosphereShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float marsAtmosphereRadius)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, marsAtmosphereRadius);
	if (!result)
	{
		return false;
	}

	RenderShaders(deviceContext, indexCount, instanceCount);

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
	D3D11_BUFFER_DESC matrixBufferDesc, atmosphericScatteringBufferDesc;;

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

	atmosphericScatteringBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	atmosphericScatteringBufferDesc.ByteWidth = sizeof(AtmosphericScatteringBufferType);
	atmosphericScatteringBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	atmosphericScatteringBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	atmosphericScatteringBufferDesc.MiscFlags = 0;
	atmosphericScatteringBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&atmosphericScatteringBufferDesc, NULL, &mAtmosphericScatteringBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void MarsAtmosphereShaderClass::ShutdownShader()
{
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

bool MarsAtmosphereShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float marsAtmosphereRadius)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;

	unsigned int bufferNumber;

	float temp[32];

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->marsAtmosphereRadius = marsAtmosphereRadius;

	deviceContext->Unmap(mMatrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	return true;
}

void MarsAtmosphereShaderClass::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount)
{
	deviceContext->IASetInputLayout(mLayout);

	deviceContext->VSSetShader(mVertexFromSpaceShader, NULL, 0);
	deviceContext->PSSetShader(mPixelFromSpaceShader, NULL, 0);

	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);

	return;
}