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

	static vector<XMFLOAT3> GetIcosadron(int);
	static vector<int> GetIcosadronIndices();
};