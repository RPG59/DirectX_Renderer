#pragma once
#include <d3d11.h>

class D3DContext
{
private:
	IDXGISwapChain* m_SwapChain;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DevContext;
	ID3D11RenderTargetView* m_BackBuffer;

	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilView* m_DepthStencilView;

public: 
	static D3DContext* currentContext;

public:
	D3DContext(void* deviceContext);
	~D3DContext();
	void Init(HWND hWnd);

	inline static ID3D11Device* GetDevice() { return D3DContext::currentContext->m_Device; }
	inline static IDXGISwapChain* GetSwapChain() { return D3DContext::currentContext->m_SwapChain; }
	inline static ID3D11DeviceContext* GetDevContext() { return D3DContext::currentContext->m_DevContext; }
	inline static ID3D11RenderTargetView* GetBackBuffer() { return D3DContext::currentContext->m_BackBuffer; }
};

