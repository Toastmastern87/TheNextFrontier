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
	char input, input2;
	unsigned long tempLong;
	Object obj;
	ifstream objFileStream;
	XMFLOAT3 tempXMFLOAT3;

	objFileStream.open(filename);

	objFileStream.get(input);
	while (!objFileStream.eof())
	{
		//Vertices
		if (input == 'v')
		{
			objFileStream.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				objFileStream >> tempXMFLOAT3.x >> tempXMFLOAT3.y >> tempXMFLOAT3.z;

				tempXMFLOAT3.z *= -1.0f;

				obj.vertices.push_back(tempXMFLOAT3);
			}
		}

		//Indices
		if (input == 'f')
		{
			objFileStream.get(input);

			// Read in the indices.
			if (input == ' ')
			{
				objFileStream >> tempLong >> input2 >> input2 >> input2;
				obj.indices.push_back(tempLong);

				objFileStream >> tempLong >> input2 >> input2 >> input2;
				obj.indices.push_back(tempLong);

				objFileStream >> tempLong >> input2 >> input2 >> input2;
				obj.indices.push_back(tempLong);
			}
		}

		while(input != '\n')
		{
			objFileStream.get(input);
		}

		objFileStream.get(input);
	}

	objFileStream.close();

	return obj;
}