#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;
using namespace std;

class ObjLoaderClass
{
public:
	struct Object
	{
		vector<XMFLOAT3> vertices;
	};

public:
	ObjLoaderClass();
	ObjLoaderClass(const ObjLoaderClass&);
	~ObjLoaderClass();

	static Object LoadObject();

};