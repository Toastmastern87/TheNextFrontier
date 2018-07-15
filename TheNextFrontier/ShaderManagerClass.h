#pragma once

#include "D3DClass.h"
#include "MarsShaderClass.h"
#include "MarsAtmosphereShaderClass.h"
#include "FontShaderClass.h"
#include "MousePointerShaderClass.h"
#include "GUIShaderClass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderMarsShader(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, float);
	bool RenderMarsAtmosphereShader(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, float);
	bool RenderFontShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	bool RenderMousePointerShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	bool RenderGUIShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	MarsShaderClass * mMarsShader;
	MarsAtmosphereShaderClass* mMarsAtmosphereShader;
	FontShaderClass* mFontShader;
	MousePointerShaderClass* mMousePointerShader;
	GUIShaderClass* mGUIShader;
};