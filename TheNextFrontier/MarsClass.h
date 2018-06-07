#ifndef _MARSCLASS_H_

#define _MARSCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class MarsClass
{
private:
	struct VertexType 
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	MarsClass();
	MarsClass(const MarsClass&);
	~MarsClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	int mVertexCount, mIndexCount;
};

#endif 