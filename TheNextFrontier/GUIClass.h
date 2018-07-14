#pragma once

#include "D3DClass.h"
#include <WICTextureLoader.h>
#include "ShaderManagerClass.h"
using namespace std;

class GUIClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};


public:
	GUIClass();
	GUIClass(const GUIClass&);
	~GUIClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int);
	void Shutdown();
	bool Render(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeGUI(ID3D11Device*, ID3D11DeviceContext*, int, int);
	void ShutdownBuffers();
	bool RenderGUI(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX);

	bool LoadGameTimeTexture(ID3D11Device*);

private:
	ID3D11Buffer * mVertexBuffer, *mIndexBuffer;
	int mVertexCount, mIndexCount;
	ID3D11Resource *mGameTimeResource;
	ID3D11ShaderResourceView *mGameTimeResourceView;
};