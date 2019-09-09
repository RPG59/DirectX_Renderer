#include "D3DContext.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

D3DContext* D3DContext::currentContext = nullptr;

D3DContext::D3DContext(void* deviceContext)
{
	Init((HWND)deviceContext);
}

D3DContext::~D3DContext()
{
	delete currentContext;
}

void D3DContext::Init(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;



	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 8;                              // how many multisamples
	scd.SampleDesc.Quality = 16;
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_SwapChain,
		&m_Device,
		NULL,
		&m_DevContext);

	UINT levels;
	m_Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		8,
		&levels
		);


	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

	// use the back buffer address to create the render target
	m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBuffer);
	pBackBuffer->Release();

	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	memset(&depthStencilDesc, NULL, sizeof(depthStencilDesc));
	depthStencilDesc.Width = 640;
	depthStencilDesc.Height = 480;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DYNAMIC;
	depthStencilDesc.SampleDesc.Count = 8;
	depthStencilDesc.SampleDesc.Quality = 16;

	hr = m_Device->CreateTexture2D(&depthStencilDesc, nullptr, &m_DepthStencilBuffer);
	hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, nullptr, &m_DepthStencilView);

	// set the render target as the back buffer
	m_DevContext->OMSetRenderTargets(1, &m_BackBuffer, m_DepthStencilView);
	/*
		1. number of render targets to bind to the pipeline
		2. the array of render targets to bind the pipeline
		3. the depth-stencil view to bind the pipeline
	*/


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = .0f;
	viewport.MaxDepth = 1.f;

	m_DevContext->RSSetViewports(1, &viewport);

}


