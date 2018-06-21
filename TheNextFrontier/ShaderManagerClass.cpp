#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	mMarsShader = 0;
	mFontShader = 0;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other)
{
}

ShaderManagerClass::~ShaderManagerClass()
{
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	mMarsShader = new MarsShaderClass;
	if (!mMarsShader)
	{
		return false;
	}

	result = mMarsShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	mFontShader = new FontShaderClass;
	if (!mFontShader)
	{
		return false;
	}

	result = mFontShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ShaderManagerClass::Shutdown()
{
	if (mFontShader)
	{
		mFontShader->Shutdown();
		delete mFontShader;
		mFontShader = 0;
	}

	if (mMarsShader)
	{
		mMarsShader->Shutdown();
		delete mMarsShader;
		mMarsShader = 0;
	}

	return;
}

bool ShaderManagerClass::RenderMarsShader(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float marsRadius, float marsMaxHeight, float marsMinHeight, vector<float> distanceLUT, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* heightMap)
{
	return mMarsShader->Render(deviceContext, indexCount, instanceCount, worldMatrix, viewMatrix, projectionMatrix, marsRadius, marsMaxHeight, marsMinHeight, distanceLUT, cameraPos, heightMap);
}

bool ShaderManagerClass::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return mFontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}