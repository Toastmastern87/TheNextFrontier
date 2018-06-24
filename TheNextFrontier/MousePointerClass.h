#pragma once

#include "D3DClass.h"
#include "ShaderManagerClass.h"

class MousePointerClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	MousePointerClass();
	MousePointerClass(const MousePointerClass&);
	~MousePointerClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int);
	void Shutdown();
	bool Render(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

	bool UpdateMousePointerPos(ID3D11DeviceContext*, int, int);

private:
	bool InitializeMousePointer(ID3D11Device*, ID3D11DeviceContext*, int, int);
	void ShutdownBuffers();
	bool RenderMousePointer(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	ID3D11Buffer * mVertexBuffer, *mIndexBuffer;
	int mVertexCount, mIndexCount;
};