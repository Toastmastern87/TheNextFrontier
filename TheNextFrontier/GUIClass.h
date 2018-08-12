#pragma once

#include "D3DClass.h"
#include <WICTextureLoader.h>
#include "ShaderManagerClass.h"
#include <vector>
#include "GeometryClass.h";

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

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();
	bool Render(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX);

	void AddBFSPopUpGUI(ID3D11Device*, float, float, float, float);

private:
	bool InitializeBaseGUI(ID3D11Device*, int, int);
	void ShutdownBuffers();
	bool RenderGUI(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX);

	bool LoadGameTimeTexture(ID3D11Device*);
	bool LoadPopUpBaseTexture(ID3D11Device*);

private:
	std::vector<ID3D11Buffer*> mVertexBuffers, mIndexBuffers;
	std::vector<unsigned long> mVertexCounts, mIndexCounts;
	std::vector<ID3D11Resource*> mResources;
	std::vector<ID3D11ShaderResourceView*> mResourceViews;

	XMMATRIX mWorldMatrix;
};