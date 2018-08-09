#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <WICTextureLoader.h>
#include "GeometryClass.h"
using namespace DirectX;
using namespace std;

class StarBoxClass
{
public:
	StarBoxClass();
	StarBoxClass(const StarBoxClass&);
	~StarBoxClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetStarBoxTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadStarBoxTexture(ID3D11Device*);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	vector<GeometryClass::VertexType3D> mVertices;
	vector<int> mIndices;

	ID3D11Resource *mStarBoxResource;
	ID3D11ShaderResourceView *mStarBoxResourceView;
};