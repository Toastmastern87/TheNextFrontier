#include "ObjLoaderClass.h"

ObjLoaderClass::ObjLoaderClass()
{
}

ObjLoaderClass::ObjLoaderClass(const ObjLoaderClass& other)
{
}

ObjLoaderClass::~ObjLoaderClass()
{
}

ObjLoaderClass::Object ObjLoaderClass::LoadObject(char* filename)
{
	char input;
	unsigned long tempLong;
	Object obj;
	ifstream objFileStream;
	XMFLOAT3 tempXMFLOAT3;
	XMFLOAT2 tempXMFLOAT2;
	int uvIndex = 0;
	Importer imp;

	const aiScene *scene = imp.ReadFile(filename, aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices);

	aiMesh *mesh = scene->mMeshes[0];

	//ofstream fOut;

	//fOut.open("Debug.txt", ios::out | ios::app);

	for (int i = 0; i < mesh->mNumVertices; i++) 
	{
		obj.vertices.push_back(VertexType(XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)));
		//fOut << "BFS.vertices[";
		//fOut << i;
		//fOut << "] x: ";
		//fOut << mesh->mVertices[i].x;
		//fOut << " y: ";
		//fOut << mesh->mVertices[i].y;
		//fOut << " z: ";
		//fOut << mesh->mVertices[i].z;
		//fOut << "\r\n";
	}

	//fOut << "\r\n";

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			obj.indices.push_back(mesh->mFaces[i].mIndices[j]);
			//fOut << "mesh->mFaces[";
			//fOut << i;
			//fOut << "].mIndices[";
			//fOut << j;
			//fOut << "]: ";
			//fOut << mesh->mFaces[i].mIndices[j];
			//fOut << "\r\n";
		}
	}


	//fOut << "\r\n";

	//for (int i = 0; i < mesh->mNumVertices; i++)
	//{
	//	fOut << "BFS.uv[";
	//	fOut << i;
	//	fOut << "] x: ";
	//	fOut << mesh->mTextureCoords[0][i].x;
	//	fOut << " y: ";
	//	fOut << mesh->mTextureCoords[0][i].y;
	//	fOut << "\r\n";
	//}

	//fOut.close();

	return obj;
}