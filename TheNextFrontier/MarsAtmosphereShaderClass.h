#pragma once

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
		float marsAtmosphereRadius;
	};

	struct AtmosphericScatteringBufferType
	{
		XMFLOAT4 lightDirection;
		XMFLOAT4 invWavelength;
		XMFLOAT4 cameraHeight;
		XMFLOAT4 cameraHeight2;
		XMFLOAT4 atmosphereRadius;
		XMFLOAT4 atmosphereRadius2;
		XMFLOAT4 marsRadius2;
		XMFLOAT4 krESun;
		XMFLOAT4 kmESun;
		XMFLOAT4 kr4PI;
		XMFLOAT4 km4PI;
		XMFLOAT4 scale;
		XMFLOAT4 scaleDepth;
		XMFLOAT4 scaleOverScaleDepth;
	};

public:
	MarsAtmosphereShaderClass();
	MarsAtmosphereShaderClass(const MarsAtmosphereShaderClass&);
	~MarsAtmosphereShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, float);
	void RenderShaders(ID3D11DeviceContext*, int, int);

private:
	ID3D11VertexShader * mVertexFromSpaceShader, *mVertexFromAtmosphereShader;
	ID3D11PixelShader *mPixelFromSpaceShader, *mPixelFromAtmosphereShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer *mMatrixBuffer, *mAtmosphericScatteringBuffer;
};