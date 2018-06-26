#pragma once

#include "D3DClass.h"
#include "MarsShaderClass.h"
#include "FontShaderClass.h"
#include "MousePointerShaderClass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderMarsShader(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, XMFLOAT4, XMFLOAT4, float, ID3D11ShaderResourceView*);
	bool RenderFontShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	bool RenderMousePointerShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	MarsShaderClass * mMarsShader;
	FontShaderClass* mFontShader;
	MousePointerShaderClass* mMousePointerShader;
};