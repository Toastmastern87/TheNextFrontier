#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_

#include "D3DClass.h"
#include "MarsShaderClass.h"
#include "FontShaderClass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderMarsShader(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, float, vector<float>, XMFLOAT3);
	bool RenderFontShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	MarsShaderClass * mMarsShader;
	FontShaderClass* mFontShader;
};

#endif
