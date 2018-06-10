#include "D3DClass.h"

D3DClass::D3DClass() 
{
	mSwapChain = 0;
	mDevice = 0;
	mDeviceContext = 0;
	mRenderTargetView = 0;
	mDepthStencilBuffer = 0;
	mDepthStencilState = 0;
	mDepthStencilView = 0;
	mRasterState = 0;
	mRasterStateNoCulling = 0;
	mRasterStateWireframe = 0;
	mDepthDisabledStencilState = 0;
	mAlphaEnableBlendingState = 0;
	mAlphaDisableBlendingState = 0;
	mAlphaEnableBlendingState2 = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vSync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDesc;

	mVSyncEnabled = vSync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) 
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	for (i = 0; i < numModes; i++) 
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth) 
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (mVSyncEnabled) 
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else 
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullScreen) 
	{
		swapChainDesc.Windowed = false;
	}
	else 
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, NULL, &mDeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}
	
	result = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(result))
	{
		return false;
	}

	mDeviceContext->RSSetState(mRasterState);

	rasterDesc.CullMode = D3D11_CULL_NONE;

	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterStateNoCulling);
	if (FAILED(result))
	{
		return false;
	}

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterStateWireframe);
	if (FAILED(result))
	{
		return false;
	}

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	
	mDeviceContext->RSSetViewports(1, &viewport);

	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	mProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	mWorldMatrix = XMMatrixIdentity();

	mOrthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &mDepthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = mDevice->CreateBlendState(&blendStateDesc, &mAlphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	blendStateDesc.RenderTarget[0].BlendEnable = false;
	blendStateDesc.AlphaToCoverageEnable = false;

	result = mDevice->CreateBlendState(&blendStateDesc, &mAlphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	blendStateDesc.AlphaToCoverageEnable = true;
	blendStateDesc.IndependentBlendEnable = false;
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = mDevice->CreateBlendState(&blendStateDesc, &mAlphaEnableBlendingState2);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void D3DClass::Shutdown() 
{
	if (mSwapChain) 
	{
		mSwapChain->SetFullscreenState(false, NULL);
	}

	if (mAlphaEnableBlendingState2)
	{
		mAlphaEnableBlendingState2->Release();
		mAlphaEnableBlendingState2 = 0;
	}

	if (mAlphaDisableBlendingState)
	{
		mAlphaDisableBlendingState->Release();
		mAlphaDisableBlendingState = 0;
	}

	if (mAlphaEnableBlendingState)
	{
		mAlphaEnableBlendingState->Release();
		mAlphaEnableBlendingState = 0;
	}

	if (mDepthDisabledStencilState)
	{
		mDepthDisabledStencilState->Release();
		mDepthDisabledStencilState = 0;
	}

	if (mRasterStateWireframe)
	{
		mRasterStateWireframe->Release();
		mRasterStateWireframe = 0;
	}

	if (mRasterStateNoCulling)
	{
		mRasterStateNoCulling->Release();
		mRasterStateNoCulling = 0;
	}

	if (mRasterState) 
	{
		mRasterState->Release();
		mRasterState = 0;
	}

	if (mDepthStencilView) 
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}

	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}


	if (mRenderTargetView) 
	{
		mRenderTargetView->Release();
		mRenderTargetView = 0;
	}

	if (mDeviceContext) 
	{
		mDeviceContext->Release();
		mDeviceContext = 0;
	}

	if (mDevice) 
	{
		mDevice->Release();
		mDevice = 0;
	}

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}

	return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha) 
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);

	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene() 
{
	if (mVSyncEnabled) 
	{
		mSwapChain->Present(1, 0);
	}
	else 
	{
		mSwapChain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3DClass::GetDevice() 
{
	return mDevice;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext() 
{
	return mDeviceContext;
}

XMMATRIX D3DClass::GetProjectionMatrix()
{
	return mProjectionMatrix;
}

void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = mWorldMatrix;

	return;
}

void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = mOrthoMatrix;

	return;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory) 
{
	strcpy_s(cardName, 128, mVideoCardDescription);
	memory = mVideoCardMemory;
	
	return;
}

void D3DClass::TurnZBufferOn() 
{
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	return;
}

void D3DClass::TurnZBufferOff()
{
	mDeviceContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);

	return;
}

void D3DClass::EnableAlphaBlending() 
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	mDeviceContext->OMSetBlendState(mAlphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3DClass::DisableAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	mDeviceContext->OMSetBlendState(mAlphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3DClass::TurnOnCulling() 
{
	mDeviceContext->RSSetState(mRasterState);

	return;
}

void D3DClass::TurnOffCulling()
{
	mDeviceContext->RSSetState(mRasterStateNoCulling);

	return;
}

void D3DClass::EnableAlphaToCoverageBlending() 
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	mDeviceContext->OMSetBlendState(mAlphaEnableBlendingState2, blendFactor, 0xffffffff);

	return;
}

void D3DClass::EnableWireframe()
{
	mDeviceContext->RSSetState(mRasterStateWireframe);

	return;
}

void D3DClass::DisableWireframe()
{
	mDeviceContext->RSSetState(mRasterState);

	return;
}