#pragma once

#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

class GeometryClass
{
public:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;

		VertexType(XMFLOAT3 position, XMFLOAT2 uvCoords)
		{
			pos = position;
			uv = uvCoords;
		}

		VertexType()
		{
		}
	};

public:
	GeometryClass();
	GeometryClass(const GeometryClass&);
	~GeometryClass();

	static vector<XMFLOAT3> GetIcosadronVertices(float);
	static vector<int> GetIcosadronIndices();

	static vector<VertexType> GetCubeVertices(float);
	static vector<int> GetCubeIndices();
};