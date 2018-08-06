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

			// UV coords
			if (input == 't')
			{
				objFileStream.get(input);

				// Read in the vertices.
				if (input == ' ')
				{
					objFileStream >> tempXMFLOAT2.x >> tempXMFLOAT2.y;

					tempXMFLOAT2.y = 1.0f - tempXMFLOAT2.y;

					obj.uv.push_back(tempXMFLOAT2);
				}
			}
		}

		//Indices
		if (input == 'f')
		{
			objFileStream.get(input);

			// Read in the indices.
			if (input == ' ')
			{
				objFileStream >> tempLong;

				objFileStream.get(input);
				
				// Test if the indices is 2 numbered
				if (input != '/') 
				{
					obj.indices.push_back((tempLong * 10) + input - 1);
				}
				else 
				{
					obj.indices.push_back(tempLong - 1);
				}

				// Loop until the next indice arrives
				objFileStream.get(input);
				while (input != ' ') 
				{
					objFileStream.get(input);
				}

				objFileStream >> tempLong;

				objFileStream.get(input);

				// Test if the indices is 2 numbered
				if (input != '/')
				{
					obj.indices.push_back((tempLong * 10) + input - 1);
				}
				else
				{
					obj.indices.push_back(tempLong - 1);
				}

				// Loop until the next indice arrives
				objFileStream.get(input);
				while (input != ' ')
				{
					objFileStream.get(input);
				}

				objFileStream >> tempLong;

				objFileStream.get(input);

				// Test if the indices is 2 numbered
				if (input != '/')
				{
					obj.indices.push_back((tempLong * 10) + input - 1);
				}
				else
				{
					obj.indices.push_back(tempLong - 1);
				}
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