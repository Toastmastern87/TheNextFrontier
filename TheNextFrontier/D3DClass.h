#ifndef _D3DCLASS_H_

#define _D3DCLASS_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class D3DClass 
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	XMMATRIX GetProjectionMatrix();
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);
	float GetAspectRation();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnCulling();
	void TurnOffCulling();

	void EnableAlphaBlending();
	void EnableAlphaToCoverageBlending();
	void DisableAlphaBlending();

	void EnableWireframe();
	void DisableWireframe();

private:
	bool mVSyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	float mAspectRatio;
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;
	ID3D11RasterizerState* mRasterStateNoCulling;
	ID3D11RasterizerState* mRasterStateWireframe;
	XMMATRIX mProjectionMatrix;
	XMMATRIX mWorldMatrix;
	XMMATRIX mOrthoMatrix;
	ID3D11DepthStencilState* mDepthDisabledStencilState;
	ID3D11BlendState* mAlphaEnableBlendingState;
	ID3D11BlendState* mAlphaDisableBlendingState;
	ID3D11BlendState* mAlphaEnableBlendingState2;
};

#endif