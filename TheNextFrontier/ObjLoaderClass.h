#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
using namespace DirectX;
using namespace std;

class ObjLoaderClass
{
public:
	struct Object
	{
		vector<XMFLOAT3> vertices;
		vector<XMFLOAT2> uv;
		vector<unsigned long> indices;
	};

public:
	ObjLoaderClass();
	ObjLoaderClass(const ObjLoaderClass&);
	~ObjLoaderClass();

	static Object LoadObject(char*);

};