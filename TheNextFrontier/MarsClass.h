#pragma once

#define _USE_MATH_DEFINES

const bool HD = false;

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <math.h>
#include <fstream>
#include "FrustumClass.h"
#include "PositionClass.h"
#include "TextureClass.h"
#include <WICTextureLoader.h>
#include "CImg.h"
using namespace DirectX;
using namespace std;
using namespace cimg_library;

class MarsClass
{
public:
	struct MarsVertexType
	{
		XMFLOAT2 pos;
		XMFLOAT2 morph;

		MarsVertexType(XMFLOAT2 position, XMFLOAT2 morphing)
		{
			pos = position;
			morph = morphing;
		}
	};

	struct MarsCellType
	{
		int level;
		XMFLOAT3 a;
		XMFLOAT3 r;
		XMFLOAT3 s;

		MarsCellType(int Level, XMFLOAT3 A, XMFLOAT3 R, XMFLOAT3 S)
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

	enum NextTriangle
	{
		CULL,
		LEAF,
		SPLIT,
		SPLITCULL
	};

public:
	MarsClass();
	MarsClass(const MarsClass&);
	~MarsClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, FrustumClass*, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetInstanceCount();
	int GetMarsVerticesCount();
	float GetMarsRadius();
	float GetMarsMaxHeight();
	float GetMarsMinHeight();
	float GetMarsPatchDelta();
	vector<float> GetDistanceLUT();
	ID3D11ShaderResourceView* GetHeightMap();

	bool UpdateMars(ID3D11DeviceContext*, FrustumClass*, PositionClass*);

private:
	bool InitializeBuffers(ID3D11Device*);
	bool InitializeIcosphere();
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadHeightMapTexture(ID3D11Device*);

	float GetVectorLength(XMFLOAT3);
	float GetVectorDistance(XMFLOAT3, XMFLOAT3);

	vector<XMFLOAT3> GetIcosadronPositions(int);
	vector<int> GetIcosadronIndices();
	void GenerateCells();
	void GenerateCellGeometry();
	void GenerateHeightMultiLUT();
	bool MapCells(ID3D11DeviceContext*);

	void RecursiveTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3, short, bool);
	NextTriangle CheckTriangleSplit(XMFLOAT3, XMFLOAT3, XMFLOAT3, short, bool);

private:
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer, *mInstanceBuffer;

	vector<MarsVertexType> mMarsCellVertices;

	vector<long int> mMarsCellIndices;
	FrustumClass* mFrustum;

	float mMarsRadius;
	float mMarsMaxHeight;
	float mMarsMinHeight;
	int screenWidth;

	vector<TriangleType> mIcosphere;
	vector<MarsCellType> mMarsCells;
	vector<vector<int>> mHeightData;

	vector<float> mDistanceLUT;
	vector<float> mHeightMultiLUT;

	int mMaxSubdivisionLevel;
	int mMaxCellLevel;
	float mMaxTriangleSize, mDelta;
	int mScreenWidth;

	PositionClass* mPosition;
	ID3D11Resource *mHeightMapResource;
	ID3D11ShaderResourceView* mHeightMapResourceView;
};