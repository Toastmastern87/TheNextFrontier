#pragma once

#include "D3DClass.h"
#include "MarsShaderClass.h"
#include "MarsAtmosphereShaderClass.h"
#include "FontShaderClass.h"
#include "MousePointerShaderClass.h"
#include "GUIShaderClass.h"
#include "StarBoxShaderClass.h"
#include "BFSShaderClass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderMarsShader(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, float, float, vector<float>, XMFLOAT3, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, float, bool, float);
	bool RenderMarsAtmosphereShader(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, float, bool, float, float, XMFLOAT3, XMFLOAT3);
	bool RenderFontShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	bool RenderMousePointerShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	bool RenderGUIShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	bool RenderStarBoxShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	bool RenderBFSShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	MarsShaderClass * mMarsShader;
	MarsAtmosphereShaderClass* mMarsAtmosphereShader;
	FontShaderClass* mFontShader;
	MousePointerShaderClass* mMousePointerShader;
	GUIShaderClass* mGUIShader;
	StarBoxShaderClass *mStarBoxShader;
	BFSShaderClass *mBFSShader;
};