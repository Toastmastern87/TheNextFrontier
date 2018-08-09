#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include "ObjLoaderClass.h"
#include <WICTextureLoader.h>
#include "TargetBoxClass.h"
#include <DirectXCollision.h>
using namespace DirectX;
using namespace std;

class BFSClass
{
public:
	BFSClass();
	BFSClass(const BFSClass&);
	~BFSClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, XMFLOAT3, XMFLOAT3, XMFLOAT3, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndicesCount();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetTargetBoxWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetNormalMap();

	TargetBoxClass* GetTargetBox();
	XMFLOAT3* GetBoundingOrientBoxCorners();
	bool IsTargeted();
	void CheckRayIntersection(XMVECTOR, XMVECTOR, bool, bool);
	void Untarget();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*);
	bool LoadNormalMap(ID3D11Device*);

private:
	int mIndexCount;
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	XMMATRIX mPositionMatrix, mScaleMatrix, mRotationMatrix;
	XMFLOAT3 mPosition, mScale, mRotation;

	ID3D11Resource *mTextureResource, *mNormalMapResource;
	ID3D11ShaderResourceView *mTextureResourceView, *mNormalMapResourceView;

	bool mTargeted;
	TargetBoxClass* mTargetBox;
	BoundingOrientedBox mBoundingOrientedBox;

	char* shipName;
	float fuelLeft;
};