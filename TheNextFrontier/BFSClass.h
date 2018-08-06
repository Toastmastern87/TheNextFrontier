#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include "ObjLoaderClass.h"
#include <WICTextureLoader.h>
using namespace DirectX;
using namespace std;

class BFSClass
{
public:
	BFSClass();
	BFSClass(const BFSClass&);
	~BFSClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, XMFLOAT3, XMFLOAT3, XMFLOAT3, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	XMMATRIX GetPositionMatrix();
	XMMATRIX GetScaleMatrix();
	XMMATRIX GetRotationMatrix();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*);

private:
	int mIndexCount;
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	XMMATRIX mPositionMatrix, mScaleMatrix, mRotationMatrix;
	XMFLOAT3 mPosition, mScale, mRotation;

	ID3D11Resource *mTextureResource;
	ID3D11ShaderResourceView *mTextureResourceView;
};