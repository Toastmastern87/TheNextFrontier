#ifndef _TEXTCLASS_H_

#define _TEXTCLASS_H_

#include "FontClass.h"
#include "ShaderManagerClass.h"

class TextClass 
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*, int, int, int, bool, FontClass*, char*, int, int, float, float, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

	bool UpdateSentence(HWND, ID3D11DeviceContext*, FontClass*, char*, int, int, float, float, float);

private:
	bool InitializeSentence(HWND, ID3D11Device*, ID3D11DeviceContext*, FontClass*, char*, int, int, float, float, float);
	void RenderSentence(ID3D11DeviceContext*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer, *mVertexBuffer2, *mIndexBuffer2;
	int mScreenWidth, mScreenHeight, mMaxLength, mVertexCount, mIndexCount;
	bool mShadow;
	XMFLOAT4 mPixelColor;
};

#endif
