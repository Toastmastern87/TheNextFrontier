#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
using namespace DirectX;
using namespace std;

class MarsShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		float marsRadius;
	};

	struct MorphBufferType
	{
		XMFLOAT4 distanceLUT[32];
		XMFLOAT4 cameraPos;
		XMFLOAT4 morphRange;
	};

	struct MarsHeightBufferType
	{
		XMFLOAT4 marsMaxHeight;
		XMFLOAT4 marsMinHeight;
	};

public:
	MarsShaderClass();
	MarsShaderClass(const MarsShaderClass&);
	~MarsShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*);
	void RenderShaders(ID3D11DeviceContext*, int, int);

private:
	ID3D11VertexShader * mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer *mMatrixBuffer, *mMorphBuffer, *mHeightBuffer;
	ID3D11SamplerState *mSampleState;
};
