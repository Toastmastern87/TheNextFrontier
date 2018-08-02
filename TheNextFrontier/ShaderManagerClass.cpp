#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	mMarsShader = 0;
	mFontShader = 0;
	mMousePointerShader = 0;
	mGUIShader = 0;
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

	mMarsAtmosphereShader = new MarsAtmosphereShaderClass;
	if (!mMarsAtmosphereShader)
	{
		return false;
	}

	result = mMarsAtmosphereShader->Initialize(device, hwnd);
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

	mMousePointerShader = new MousePointerShaderClass();
	if (!mMousePointerShader)
	{
		return false;
	}

	result = mMousePointerShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	mGUIShader = new GUIShaderClass();
	if (!mGUIShader)
	{
		return false;
	}

	result = mGUIShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	mStarBoxShader = new StarBoxShaderClass();
	if (!mStarBoxShader)
	{
		return false;
	}

	result = mStarBoxShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ShaderManagerClass::Shutdown()
{
	if (mStarBoxShader)
	{
		mStarBoxShader->Shutdown();
		delete mStarBoxShader;
		mStarBoxShader = 0;
	}

	if (mGUIShader)
	{
		mGUIShader->Shutdown();
		delete mGUIShader;
		mGUIShader = 0;
	}

	if (mMousePointerShader)
	{
		mMousePointerShader->Shutdown();
		delete mMousePointerShader;
		mMousePointerShader = 0;
	}

	if (mFontShader)
	{
		mFontShader->Shutdown();
		delete mFontShader;
		mFontShader = 0;
	}

	if (mMarsAtmosphereShader)
	{
		mMarsAtmosphereShader->Shutdown();
		delete mMarsAtmosphereShader;
		mMarsAtmosphereShader = 0;
	}

	if (mMarsShader)
	{
		mMarsShader->Shutdown();
		delete mMarsShader;
		mMarsShader = 0;
	}

	return;
}

bool ShaderManagerClass::RenderMarsShader(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX inverseWorldMatrix, XMMATRIX rotationMatrix, float marsRadius, float marsMaxHeight, float marsMinHeight, vector<float> distanceLUT, XMFLOAT3 cameraPos, ID3D11ShaderResourceView* heightMap, ID3D11ShaderResourceView* heightMapDetail2, ID3D11ShaderResourceView* colorMap, ID3D11ShaderResourceView* detailAreaTextureX, ID3D11ShaderResourceView* detailAreaTextureY, ID3D11ShaderResourceView* detailAreaTextureWH, ID3D11ShaderResourceView* craterHeightMapTexture, XMFLOAT3 lightDirection, XMFLOAT4 lightDiffuseColor, float patchDelta, bool insideAtmosphere, float distanceFromOrigo)
{
	return mMarsShader->Render(deviceContext, indexCount, instanceCount, worldMatrix, viewMatrix, projectionMatrix, inverseWorldMatrix, rotationMatrix, marsRadius, marsMaxHeight, marsMinHeight, distanceLUT, cameraPos, heightMap, heightMapDetail2, colorMap, detailAreaTextureX, detailAreaTextureY, detailAreaTextureWH, craterHeightMapTexture, lightDirection, lightDiffuseColor, patchDelta, insideAtmosphere, distanceFromOrigo);
}

bool ShaderManagerClass::RenderMarsAtmosphereShader(ID3D11DeviceContext* deviceContext, int indexCount, int instanceCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX rotationMatrix, float marsAtmosphereRadius, bool insideAtmosphere, float marsRadius, float distanceFromOrigo, XMFLOAT3 cameraPos, XMFLOAT3 lightDirection)
{
	return mMarsAtmosphereShader->Render(deviceContext, indexCount, instanceCount, worldMatrix, viewMatrix, projectionMatrix, rotationMatrix, marsAtmosphereRadius, insideAtmosphere, marsRadius, distanceFromOrigo, cameraPos, lightDirection);
}

bool ShaderManagerClass::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return mFontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}

bool ShaderManagerClass::RenderMousePointerShader(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView *texture)
{
	return mMousePointerShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManagerClass::RenderGUIShader(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView *texture)
{
	return mGUIShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManagerClass::RenderStarBoxShader(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* starBoxTexture)
{
	return mStarBoxShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, starBoxTexture);
}