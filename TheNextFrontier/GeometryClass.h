#pragma once

#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

class GeometryClass
{
public:
	struct VertexType3D
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;

		VertexType3D(XMFLOAT3 position, XMFLOAT2 uvCoords)
		{
			pos = position;
			uv = uvCoords;
		}

		VertexType3D()
		{
		}
	};

public:
	GeometryClass();
	GeometryClass(const GeometryClass&);
	~GeometryClass();

	static vector<XMFLOAT3> GetIcosadronVertices(float);
	static vector<int> GetIcosadronIndices();

	static vector<VertexType3D> GetCubeVertices(float);
	static vector<int> GetCubeIndices();
};