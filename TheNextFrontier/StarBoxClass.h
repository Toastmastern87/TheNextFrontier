#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;
using namespace std;

class StarBoxClass
{
public:
	struct StarBoxVertexType
	{
		XMFLOAT3 pos;

		StarBoxVertexType(XMFLOAT3 position)
		{
			pos = position;
		}

		StarBoxVertexType() 
		{
		}
	};

public:
	StarBoxClass();
	StarBoxClass(const StarBoxClass&);
	~StarBoxClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	int mVertexCount, mIndicesCount;
};