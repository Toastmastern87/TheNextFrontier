#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "GeometryClass.h"
#include <WICTextureLoader.h>
using namespace DirectX;

class TargetBoxClass
{
public:
	TargetBoxClass();
	TargetBoxClass(const TargetBoxClass&);
	~TargetBoxClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();

	int GetIndicesCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*);

private:
	ID3D11Buffer * mVertexBuffer, *mIndexBuffer;
	std::vector<GeometryClass::VertexType> mVertices;
	std::vector<int> mIndices;

	ID3D11Resource *mTextureResource;
	ID3D11ShaderResourceView *mTextureResourceView;
};