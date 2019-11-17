#include "DXBuffer.h"
#include "D3DContext.h"

struct VERTEX1 { FLOAT X, Y, Z;  float Color[4]; };
VERTEX1 OurVertices1[] =
{
	{0.45f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
	{0.45f, -0.5, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
	{-0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}
};

DXBuffer::DXBuffer(size_t size)
{

	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.ByteWidth = size;
}

void DXBuffer::Bind()
{
	UINT32 stride = 28 + 8; //////TOOTOTOTODODODOD!!!!	tetst only!!!!
	UINT32 offset = 0;

	D3DContext::GetDevContext()->IASetInputLayout(m_InputLayout);
	D3DContext::GetDevContext()->IASetVertexBuffers(0, 1, &m_BufferHandle, &stride, &offset);
}

//void DXBuffer::SetData(UINT32 size, const void* data)
//{
//	//GetPointerInternal();
//	//memcpy(m_MappedSubresouce.pData, data, size);
//}

void* DXBuffer::GetPointerInternal()
{
	D3DContext::GetDevContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedSubresouce);
	return m_MappedSubresouce.pData;
}

void DXBuffer::ReleasePointer()
{
	D3DContext::GetDevContext()->Unmap(m_BufferHandle, NULL);
}

void DXBuffer::SetLayout(LPVOID bufferPointer, SIZE_T bufferSize)
{
	//TODO !
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	D3DContext::GetDevice()->CreateInputLayout(ied, 3, bufferPointer, bufferSize, &m_InputLayout);
}

void DXBuffer::Resize(float* pVertices)
{
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//m_Size = bufferSize;
	//m_BufferDesc.ByteWidth = bufferSize;

	D3DContext::GetDevice()->CreateBuffer(&m_BufferDesc, &InitData, &m_BufferHandle);
}



