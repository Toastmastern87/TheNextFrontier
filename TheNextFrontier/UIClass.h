#ifndef _UICLASS_H_

#define _UICLASS_H_

#include "TextClass.h"

class UIClass 
{
public:
	UIClass();
	UIClass(const UIClass&);
	~UIClass();

	bool Initialize(D3DClass*, int, int);
	void Shutdown();

	bool Frame(ID3D11DeviceContext*, int, float, float, float, float, float, float);
	bool Render(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool UpdateFPSString(ID3D11DeviceContext*, int);
	bool UpdatePositionStrings(ID3D11DeviceContext*, float, float, float, float, float, float);

private:
	FontClass* mFont1;
	TextClass *mFPSString, *mVideoStrings, *mPositionStrings;
	int mPreviousFPS;
	int mPreviousPosition[6];
};

#endif
