#ifndef _UICLASS_H_
#define _UICLASS_H_

#include "TextClass.h"

class UIClass 
{
public:
	UIClass();
	UIClass(const UIClass&);
	~UIClass();

	bool Initialize(HWND, D3DClass*, int, int);
	void Shutdown();

	bool Frame(HWND, ID3D11DeviceContext*, int, float, float, float, float, float, float, int);
	bool Render(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool UpdateFPSString(HWND, ID3D11DeviceContext*, int);
	bool UpdatePositionStrings(HWND, ID3D11DeviceContext*, float, float, float, float, float, float);
	bool UpdateVerticesString(HWND, ID3D11DeviceContext*, int);

private:
	FontClass* mFont1;
	TextClass *mFPSString, *mVideoStrings, *mPositionStrings, *mVerticesString;
	int mPreviousFPS;
	int mPreviousPosition[6];
};

#endif
