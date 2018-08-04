#pragma once

#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

class GeometryClass
{
public:
	GeometryClass();
	GeometryClass(const GeometryClass&);
	~GeometryClass();

	static vector<XMFLOAT3> GetIcosadronVertices(float);
	static vector<int> GetIcosadronIndices();
};