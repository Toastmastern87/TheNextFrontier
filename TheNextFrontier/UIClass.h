#ifndef _UICLASS_H_
#define _UICLASS_H_

#define _USE_MATH_DEFINES

#include "TextClass.h"

class UIClass 
{
public:
	UIClass();
	UIClass(const UIClass&);
	~UIClass();

	bool Initialize(HWND, D3DClass*, int, int);
	void Shutdown();

	bool Frame(HWND, ID3D11DeviceContext*, int, float, float, float, float, float, float, int, float, float, float, int, int, int, int, int, float);
	bool Render(D3DClass*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool UpdateFPSString(HWND, ID3D11DeviceContext*, int);
	bool UpdatePositionStrings(HWND, ID3D11DeviceContext*, float, float, float, float, float, float);
	bool UpdateVerticesString(HWND, ID3D11DeviceContext*, int);
	bool UpdateAltitudeString(HWND, ID3D11DeviceContext*, float);
	bool UpdateDistFromOrigoString(HWND, ID3D11DeviceContext*, float);
	bool UpdateMarsHeightString(HWND, ID3D11DeviceContext*, float);
	bool UpdateGameTimeString(HWND, ID3D11DeviceContext*, int, int, int, int, int);
	bool UpdateOrbitalAngleYString(HWND, ID3D11DeviceContext*, float);

private:
	FontClass* mFont1;
	TextClass *mFPSString, *mVideoStrings, *mPositionStrings, *mVerticesString, *mAltitudeString, *mDistFromOrigoString, *mMarsHeightString, *mGameTimeString, *mOrbitalAngleYString;
	int mPreviousFPS;
	int mPreviousPosition[6];
	int mPreviousGameTimeSec;
};

#endif