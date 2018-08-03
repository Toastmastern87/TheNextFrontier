#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include "ObjLoaderClass.h"
using namespace DirectX;
using namespace std;

class BFSClass
{
public:
	struct VertexType
	{
		XMFLOAT3 position;
	};

public:
	BFSClass();
	BFSClass(const BFSClass&);
	~BFSClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
};