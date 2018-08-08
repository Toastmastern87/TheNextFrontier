#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class TargetBoxShaderClass
{
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX position;
		XMMATRIX scale;
		XMMATRIX rotation;
	};

public:
	TargetBoxShaderClass();
	TargetBoxShaderClass(const TargetBoxShaderClass&);
	~TargetBoxShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT3, XMMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT3, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader * mVertexShader;
	ID3D11PixelShader *mPixelShader;
	ID3D11InputLayout *mLayout;
	ID3D11Buffer *mConstantBuffer;
	ID3D11SamplerState *mSampleState;
};