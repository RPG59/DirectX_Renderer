#pragma once
#include <d3d11.h>

class DXBuffer
{
private:
	D3D11_BUFFER_DESC m_BufferDesc;
	D3D11_MAPPED_SUBRESOURCE m_MappedSubresouce;
	ID3D11Buffer* m_BufferHandle;
	ID3D11InputLayout* m_InputLayout;
	
	UINT32 m_Size = 0;
public:
	DXBuffer();
	void Resize(UINT32 bufferSize);
	void SetLayout(LPVOID bufferPointer, SIZE_T bufferSize);
	void Bind();
	void SetData(UINT32 size, const void* data);

	void* GetPointerInternal();
	void ReleasePointer();

};

