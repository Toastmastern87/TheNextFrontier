#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "GeometryClass.h"
using namespace DirectX;
using namespace std;

class TargetBoxClass
{
public:
	TargetBoxClass();
	TargetBoxClass(const TargetBoxClass&);
	~TargetBoxClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndicesCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer * mVertexBuffer, *mIndexBuffer;
	vector<GeometryClass::VertexType> mVertices;
	vector<int> mIndices;
};