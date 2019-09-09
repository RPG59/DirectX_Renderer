#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#include "D3DContext.h"
#include "DXBuffer.h"
#include "DXIndexBuffer.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

struct VERTEX { FLOAT X, Y, Z;  float Color[4]; };
VERTEX OurVertices[] =
{
	{0.45, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
	{0.45f, -0.5, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
	{-0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}
};

struct VS_CONSTANT_BUFFER
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4 vSomeVectorThatMayBeNeededByASpecificShader;
	float fSomeFloatThatMayBeNeededByASpecificShader;
	float fTime;
	float fSomeFloatThatMayBeNeededByASpecificShader2;
	float fSomeFloatThatMayBeNeededByASpecificShader3;
};

//ID3D11Buffer* g_pIndexBuffer = NULL;
ID3D11Buffer* g_pConstantBuffer11 = NULL;

//DXBuffer* buffer = new DXBuffer();

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

using std::string;
ID3DBlob* Compile(const string source, const string profile, const string main);

//UINT16 indices[] = {
//	0, 1, 2,
//	2, 3, 1
//};

DXBuffer* buffer = new DXBuffer();
DXIndexBuffer* indexBuffer = nullptr;

ID3D11VertexShader* pVS;               // the pointer to the vertex shader
ID3D11PixelShader* pPS;                // the pointer to the pixel shader


void RenderFrame(void);     // renders a single frame
//void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);


	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Our First Direct3D Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	D3DContext::currentContext = new D3DContext(hWnd);

	InitPipeline();
	InitGraphics();




	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame();
	}

	// clean up DirectX and COM
	//CleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


void RenderFrame(void)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	D3DContext::GetDevContext()->ClearRenderTargetView(D3DContext::GetBackBuffer(), color);


	buffer->Bind();
	indexBuffer->Bind();
	//D3DContext::GetDevContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// select which primtive type we are using
	D3DContext::GetDevContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	D3DContext::GetDevContext()->DrawIndexed(6, 0, 0);
	//D3DContext::GetDevContext()->Draw(3, 0);

	// switch the back buffer and the front buffer
	HRESULT hr;
	hr = D3DContext::GetSwapChain()->Present(0, 0);
	if (FAILED(hr)) {
		DebugBreak();
	}
}


// this is the function that cleans up Direct3D and COM
//void CleanD3D(void)
//{
//	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
//
//	// close and release all existing COM objects
//	pLayout->Release();
//	pVS->Release();
//	pPS->Release();
//	pVBuffer->Release();
//	swapchain->Release();
//	backbuffer->Release();
//	dev->Release();
//	devcon->Release();
//}


void InitGraphics()
{
	UINT16 indices[] = { 0, 1, 2, 3, 0, 2 };
	buffer->Resize(sizeof(VERTEX) * 3);
	indexBuffer = new DXIndexBuffer(indices, 6);
	
	VS_CONSTANT_BUFFER VsConstData;
	VsConstData.vSomeVectorThatMayBeNeededByASpecificShader = XMFLOAT4(1, 2, 3, 4);
	VsConstData.fSomeFloatThatMayBeNeededByASpecificShader = 3.0f;
	VsConstData.fTime = 0.2f;
	VsConstData.fSomeFloatThatMayBeNeededByASpecificShader2 = 2.0f;
	VsConstData.fSomeFloatThatMayBeNeededByASpecificShader3 = 4.0f;

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &VsConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = D3DContext::GetDevice()->CreateBuffer(&cbDesc, &InitData, &g_pConstantBuffer11);
	if (FAILED(hr))
	{
		DebugBreak();
	}
	D3DContext::GetDevContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);





	//buffer->SetData(sizeof(OurVertices), OurVertices);



	 //Create indices.

	// Fill in a buffer description.
	//D3D11_BUFFER_DESC bufferDesc;
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof(unsigned int) * 6;
	//bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bufferDesc.CPUAccessFlags = 0;
	//bufferDesc.MiscFlags = 0;

	//// Define the resource data.
	//D3D11_SUBRESOURCE_DATA InitData;
	//InitData.pSysMem = indices;
	//InitData.SysMemPitch = 0;
	//InitData.SysMemSlicePitch = 0;

	//HRESULT hr;

	//// Create the buffer with the device.
	//hr = D3DContext::GetDevice()->CreateBuffer(&bufferDesc, &InitData, &g_pIndexBuffer);
	//if (FAILED(hr)) {
	//	DebugBreak();
	//}

	// Set the buffer.
	//D3DContext::GetDevContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

}


void InitPipeline()
{
	ID3DBlob* VS, * PS;
	const string path = "shaders.shader";
	std::ifstream ifs(path.c_str());
	std::string result((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	VS = Compile(result, "vs_4_0", "VShader");
	PS = Compile(result, "ps_4_0", "PShader");

	D3DContext::GetDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	D3DContext::GetDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	D3DContext::GetDevContext()->VSSetShader(pVS, 0, 0);
	D3DContext::GetDevContext()->PSSetShader(pPS, 0, 0);

	buffer->SetLayout(VS->GetBufferPointer(), VS->GetBufferSize());
}

ID3DBlob* Compile(const string source, const string profile, const string main)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr;

	hr = D3DCompile(source.c_str(), source.size(), NULL, NULL, NULL, main.c_str(), profile.c_str(), D3DCOMPILE_DEBUG, NULL, &shaderBlob, &errorBlob);

	if (errorBlob && errorBlob->GetBufferSize())
	{
		std::cout << "Shader compile Errors: " << std::endl << (const char*)errorBlob->GetBufferPointer() << std::endl;
		errorBlob->Release();
	}

	if (FAILED(hr)) {
		std::cout << "COMPILE ERROR!" << std::endl;
		return NULL;
	}
	return shaderBlob;
}
