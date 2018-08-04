#include "GeometryClass.h"

GeometryClass::GeometryClass()
{
}

GeometryClass::GeometryClass(const GeometryClass& other)
{
}

GeometryClass::~GeometryClass()
{
}

vector<XMFLOAT3> GeometryClass::GetIcosadronVertices(float radius)
{
	vector<XMFLOAT3> ret;

	float ratio = (float)((1.0f + sqrt(5.0f)) / 2.0f);
	float scale = radius / sqrt(ratio * ratio + 1.0f);

	ratio *= scale;

	ret.push_back(XMFLOAT3(ratio, 0.0f, -scale));
	ret.push_back(XMFLOAT3(-ratio, 0.0f, -scale));
	ret.push_back(XMFLOAT3(ratio, 0.0f, scale));
	ret.push_back(XMFLOAT3(-ratio, 0.0f, scale));

	ret.push_back(XMFLOAT3(0.0f, -scale, ratio));
	ret.push_back(XMFLOAT3(0.0f, -scale, -ratio));
	ret.push_back(XMFLOAT3(0.0f, scale, ratio));
	ret.push_back(XMFLOAT3(0.0f, scale, -ratio));

	ret.push_back(XMFLOAT3(-scale, ratio, 0.0f));
	ret.push_back(XMFLOAT3(-scale, -ratio, 0.0f));
	ret.push_back(XMFLOAT3(scale, ratio, 0.0f));
	ret.push_back(XMFLOAT3(scale, -ratio, 0.0f));

	return ret;
}

vector<int> GeometryClass::GetIcosadronIndices()
{
	vector<int> ret
	{
		1, 3, 8,
		3, 1, 9,
		2, 0, 10,
		0, 2, 11,

		5, 7, 0,
		7, 5, 1,
		6, 4, 2,
		4, 6, 3,

		9, 11, 4,
		11, 9, 5,
		10, 8, 6,
		8, 10, 7,

		7, 1, 8,
		1, 5, 9,
		0, 7, 10,
		5, 0, 11,

		3, 6, 8,
		4, 3, 9,
		6, 2, 10,
		2, 4, 11
	};

	return ret;
}