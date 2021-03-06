#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <math.h>
#include <fstream>
#include "PositionClass.h"
#include "GeometryClass.h"
using namespace DirectX;

class MarsAtmosphereClass
{
public:
	struct MarsAtmosphereVertexType
	{
		XMFLOAT2 pos;

		MarsAtmosphereVertexType(XMFLOAT2 position)
		{
			pos = position;
		}
	};

	struct MarsAtmosphereCellType
	{
		int level;
		XMFLOAT3 a;
		XMFLOAT3 r;
		XMFLOAT3 s;

		MarsAtmosphereCellType(int Level, XMFLOAT3 A, XMFLOAT3 R, XMFLOAT3 S)
		{
			level = Level;
			a = A;
			r = R;
			s = S;
		}
	};

	struct TriangleType
	{
		XMFLOAT3 a, b, c;
		TriangleType* parent;
		short level;

		TriangleType()
		{
		}

		TriangleType(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C, TriangleType* Parent, short Level)
		{
			a = A;
			b = B;
			c = C;

			parent = Parent;

			level = Level;
		}
	};

public:
	MarsAtmosphereClass();
	MarsAtmosphereClass(const MarsAtmosphereClass&);
	~MarsAtmosphereClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetMarsVerticesCount();
	int GetInstanceCount();

	float GetAtmosphereHeight();

	void UpdateMarsAtmosphere(ID3D11DeviceContext*, PositionClass*);

private:
	bool InitializeBuffers(ID3D11Device*);
	bool InitializeIcosphere(float);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void GenerateCells();
	void GenerateCellGeometry();
	bool MapCells(ID3D11DeviceContext*);

	void RecursiveTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3, short, bool);

private:
	float mAtmosphereHeight, mMarsRadius;
	ID3D11Buffer * mVertexBuffer, *mIndexBuffer, *mInstanceBuffer;

	std::vector<MarsAtmosphereVertexType> mMarsAtmosphereCellVertices;
	std::vector<long int> mMarsAtmosphereCellIndices;
	std::vector<TriangleType> mIcosphere;
	std::vector<MarsAtmosphereCellType> mMarsAtmosphereCells;

	int mMaxSubdivisionLevel, mMaxCellLevel;

	PositionClass* mPosition;
};