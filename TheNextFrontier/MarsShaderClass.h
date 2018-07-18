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
		XMMATRIX inverseWorld;
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

	struct LightBufferType
	{
		XMFLOAT4 lightDirection;
		XMFLOAT4 diffuseColor;
		XMFLOAT4 patchDelta;
		XMMATRIX rotationMatrix;
	};

public:
	MarsShaderClass();
	MarsShaderClass(const MarsShaderClass&);
	~MarsShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, float, bool);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, float);
	void RenderShaders(ID3D11DeviceContext*, int, int, bool);

private:
	ID3D11VertexShader *mVertexFromSpaceShader, *mVertexFromAtmosphereShader;
	ID3D11PixelShader *mPixelFromSpaceShader, *mPixelFromAtmosphereShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer *mMatrixBuffer, *mMorphBuffer, *mHeightBuffer, *mLightBuffer;
	ID3D11SamplerState *mSampleStateHeight;
};
