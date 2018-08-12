#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;

class BFSShaderClass
{
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 lightDirection;
		XMFLOAT4 diffuseColor;
		XMMATRIX rotationMatrix;
	};

public:
	BFSShaderClass();
	BFSShaderClass(const BFSShaderClass&);
	~BFSShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader * mVertexShader;
	ID3D11PixelShader *mPixelShader;
	ID3D11InputLayout *mLayout;
	ID3D11Buffer *mConstantBuffer, *mLightBuffer;
	ID3D11SamplerState *mSampleState;
};