#include "DXIndexBuffer.h"
#include "D3DContext.h"

DXIndexBuffer::DXIndexBuffer(UINT16* data, UINT32 count)
	: m_Count(count)
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT16) * count;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = data;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;
	HRESULT hr = D3DContext::GetDevice()->CreateBuffer(&ibd, &ibData, &m_Handle);
	if (FAILED(hr)) {
		DebugBreak();
	}
		
}

DXIndexBuffer::~DXIndexBuffer()
{

}

void DXIndexBuffer::Bind()
{
	D3DContext::GetDevContext()->IASetIndexBuffer(m_Handle, DXGI_FORMAT_R16_UINT, 0);
}

void DXIndexBuffer::Unbind()
{

}

