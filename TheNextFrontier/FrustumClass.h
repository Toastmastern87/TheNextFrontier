#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_

#include <DirectXMath.h>
using namespace DirectX;

class FrustumClass 
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	bool ConstructFrustum(float, XMMATRIX, XMMATRIX);
	bool CheckTriangle(XMFLOAT3, XMFLOAT3, XMFLOAT3);

private:
	XMFLOAT4 mPlane[6];
};

#endif
