#include "D3DContext.h"
#include <Dxgi.h>

#pragma comment(lib, "DXGI.lib")

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define DEBUG_MODE

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
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	HRESULT hr;
	// create a device, device context and swap chain using the information in the scd struct
	IDXGIFactory* fooFactory;
	IDXGIAdapter* fooAdapter;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&fooFactory);
	//fooFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_MINIMUM_POWER, __uuidof(IDXGIAdapter4), (void**)& fooAdapter);
	fooFactory->EnumAdapters(2, &fooAdapter);
	DXGI_ADAPTER_DESC fooAdapterDesc;
	fooAdapter->GetDesc(&fooAdapterDesc);

	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&m_Device,
		NULL,
		&m_DevContext
	);

	hr = fooFactory->CreateSwapChain(m_Device, &scd, &m_SwapChain);


	UINT levels;
	m_Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&levels
	);

//////////////////////////////////////////////
#ifdef DEBUG_MODE
	//IDXGIDevice4* pDXGIDevice = nullptr;
	//hr = m_Device->QueryInterface(__uuidof(IDXGIDevice4), (void**)& pDXGIDevice);

	//IDXGIAdapter* pDXGIAdapter = nullptr;
	//IDXGIAdapter4* pdxgiAdapter4 = nullptr;
	//hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
	//hr = pDXGIAdapter->QueryInterface(__uuidof(IDXGIAdapter4), (void**)& pdxgiAdapter4);

	//IDXGIFactory7* pIDXGIFactory = nullptr;
	//pdxgiAdapter4->GetParent(__uuidof(IDXGIFactory7), (void**)& pIDXGIFactory);

	//IDXGIOutput* pOutput = NULL;
	//IDXGIOutput6* pOutput6 = NULL;
	//DXGI_OUTPUT_DESC1 pOD1;
	//hr = pDXGIAdapter->EnumOutputs(0, &pOutput);
	//hr = pOutput->QueryInterface(__uuidof(IDXGIOutput6), (void**)&pOutput6);
	//hr = pOutput6->GetDesc1(&pOD1);
	//DXGI_ADAPTER_DESC3 pAdapterDesc3;
	//hr = pdxgiAdapter4->GetDesc3(&pAdapterDesc3);
	//IDXGIAdapter* testAdapter = nullptr;
	//hr = pIDXGIFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_MINIMUM_POWER, __uuidof(IDXGIAdapter), (void**)(&testAdapter));
	//DXGI_ADAPTER_DESC testAdapterDesc;
	//hr = testAdapter->GetDesc(&testAdapterDesc);




#endif
///////////////////////////////////


	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = NULL;
	DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//// Get the number of elements
	//hr = pOutput->GetDisplayModeList(format, 0, &numModes, NULL);

	//displayModes = new DXGI_MODE_DESC[numModes];

	//// Get the list
	//hr = pOutput->GetDisplayModeList(format, 0, &numModes, displayModes);

	//DXGI_ADAPTER_DESC adapterDesc;
	//pDXGIAdapter->GetDesc(&adapterDesc);

	// use the back buffer address to create the render target
	m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBuffer);
	pBackBuffer->Release();

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


