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
		XMFLOAT3 biTangent;

		VertexType(XMFLOAT3 position, XMFLOAT2 uvCoords, XMFLOAT3 nor, XMFLOAT3 tan, XMFLOAT3 biTan)
		{
			pos = position;
			uv = uvCoords;
			normal = nor;
			tangent = tan;
			biTangent = biTan;
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