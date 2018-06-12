#ifndef _MARSCLASS_H_
#define _MARSCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include "FrustumClass.h"
using namespace DirectX;
using namespace std;

class MarsClass
{
private:
	struct VertexType 
	{
		XMFLOAT3 position;
		XMFLOAT4 color;

		VertexType() 
		{
		}

		VertexType(float x, float y, float z, XMFLOAT4 c) 
		{
			position.x = x;
			position.y = y;
			position.z = z;

			color = c;
		}

		VertexType(float x, float y, float z)
		{
			position.x = x;
			position.y = y;
			position.z = z;

			color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		VertexType operator+(VertexType a)
		{
			return VertexType(a.position.x + position.x, a.position.y + position.y, a.position.z + position.z, color);
		}

		VertexType operator*(float factor)
		{
			return VertexType(position.x * factor, position.y * factor, position.z * factor, color);
		}

		VertexType operator-(VertexType a)
		{
			return VertexType(position.x - a.position.x, position.y - a.position.y, position.z - a.position.z, color);
		}
	};

	struct MeshGeometry3D
	{
		vector<VertexType> vertices;
		vector<unsigned long> indices;
	};

public:
	MarsClass();
	MarsClass(const MarsClass&);
	~MarsClass();

	bool Initialize(ID3D11Device*, FrustumClass*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetVerticesCount();

	bool UpdateVertexBuffer(ID3D11DeviceContext*, FrustumClass*);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void RecursiveTriangle(VertexType, VertexType, VertexType, short level);
	float GetVertexTypeLength(VertexType);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	MeshGeometry3D mMarsMesh;
	FrustumClass* mFrustum;
	vector<VertexType> mIcosphere;
};

#endif 