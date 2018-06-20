#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	mColorShader = 0;
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

	mColorShader = new ColorShaderClass;
	if (!mColorShader)
	{
		return false;
	}

	result = mColorShader->Initialize(device, hwnd);
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

	if (mColorShader)
	{
		mColorShader->Shutdown();
		delete mColorShader;
		mColorShader = 0;
	}

	return;
}

bool ShaderManagerClass::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float marsRadius, vector<float> distanceLUT, XMFLOAT3 cameraPos)
{
	return mColorShader->Render(deviceContext, indexCount, instanceCount, worldMatrix, viewMatrix, projectionMatrix, marsRadius, distanceLUT, cameraPos);
}

bool ShaderManagerClass::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return mFontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}