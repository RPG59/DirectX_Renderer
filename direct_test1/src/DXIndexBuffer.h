#pragma once

#include <d3d11.h>

class DXIndexBuffer
{
private:
	ID3D11Buffer* m_Handle;
	UINT32 m_Count;
public:
	DXIndexBuffer(UINT16* data, UINT32 count);
	~DXIndexBuffer();

	void Bind();
	void Unbind();

	inline UINT32 GetCount() const { return m_Count; };
};
