#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace DirectX;
using namespace std;
using namespace Assimp;

class ObjLoaderClass
{
public:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 bitangent;

		VertexType(XMFLOAT3 position, XMFLOAT2 uvCoords)
		{
			pos = position;
			uv = uvCoords;
		}

		VertexType(XMFLOAT3 position) 
		{
			pos = position;
		}
	};

	struct Object
	{
		vector<VertexType> vertices;
		vector<unsigned long> indices;
	};

public:
	ObjLoaderClass();
	ObjLoaderClass(const ObjLoaderClass&);
	~ObjLoaderClass();

	static Object LoadObject(char*);

};