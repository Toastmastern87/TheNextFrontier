#pragma once

#define _USE_MATH_DEFINES

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
using namespace DirectX;
using namespace std;

class MarsAtmosphereShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX rotationMatrix;
		float marsAtmosphereRadius;
	};

	struct AtmosphericScatteringVSBufferType
	{
		XMFLOAT4 cameraPos;
		XMFLOAT4 lightDirection;
		XMFLOAT4 invWavelength;
		XMFLOAT4 cameraHeight;
		XMFLOAT4 cameraHeight2;
		XMFLOAT4 atmosphereRadius;
		XMFLOAT4 atmosphereRadius2;
		XMFLOAT4 marsRadius;
		XMFLOAT4 krESun;
		XMFLOAT4 kmESun;
		XMFLOAT4 kr4PI;
		XMFLOAT4 km4PI;
		XMFLOAT4 scale;
		XMFLOAT4 scaleDepth;
		XMFLOAT4 scaleOverScaleDepth;
	};

	struct AtmosphericScatteringPSBufferType
	{
		XMMATRIX rotationMatrix;
		XMFLOAT4 lightDirection;
		XMFLOAT4 g;
	};

public:
	MarsAtmosphereShaderClass();
	MarsAtmosphereShaderClass(const MarsAtmosphereShaderClass&);
	~MarsAtmosphereShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, bool, float, float, XMFLOAT3, XMFLOAT3);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, XMFLOAT3, XMFLOAT3);
	void RenderShaders(ID3D11DeviceContext*, int, int, bool);

private:
	ID3D11VertexShader * mVertexFromSpaceShader, *mVertexFromAtmosphereShader;
	ID3D11PixelShader *mPixelFromSpaceShader, *mPixelFromAtmosphereShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer *mMatrixBuffer, *mAtmosphericScatteringVSBuffer, *mAtmosphericScatteringPSBuffer;
};