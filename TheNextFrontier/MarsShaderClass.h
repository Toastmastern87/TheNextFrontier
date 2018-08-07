#pragma once

#define _USE_MATH_DEFINES
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

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
		XMMATRIX rotationMatrix;
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

	struct AtmosphericScatteringBufferType
	{
		XMFLOAT4 lightDirection;
		XMFLOAT4 invWavelength;
		XMFLOAT4 cameraHeight;
		XMFLOAT4 atmosphereRadius;
		XMFLOAT4 kr;
		XMFLOAT4 km;
		XMFLOAT4 eSun;
		XMFLOAT4 scale;
		XMFLOAT4 scaleDepth;
		XMFLOAT4 scaleOverScaleDepth;
	};

public:
	MarsShaderClass();
	MarsShaderClass(const MarsShaderClass&);
	~MarsShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, float, bool, float);

private:
	bool InitializeShader(ID3D11Device*, HWND);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, float, float);
	void RenderShaders(ID3D11DeviceContext*, int, int, bool);

private:
	ID3D11VertexShader *mVertexFromSpaceShader, *mVertexFromAtmosphereShader;
	ID3D11PixelShader *mPixelFromSpaceShader, *mPixelFromAtmosphereShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer *mMatrixBuffer, *mMorphBuffer, *mHeightBuffer, *mLightBuffer, *mAtmosphericScatteringBuffer;
	ID3D11SamplerState *mSampleStateHeight;
};