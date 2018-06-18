#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_

#include <DirectXMath.h>
#include "CameraClass.h"
using namespace DirectX;

class FrustumClass 
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	bool Initialize(CameraClass*);
	bool ConstructFrustum(float, XMMATRIX, XMMATRIX);
	bool CheckTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3);

	float GetFOV();

private:
	XMFLOAT4 mPlane[6];
	CameraClass* mCamera;
};

#endif
