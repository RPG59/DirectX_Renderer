#pragma once
#include <DirectXMath.h>
#include "DXBuffer.h"
using namespace DirectX;

class Renderable2D
{
private:
	XMFLOAT3 m_Position;
	XMFLOAT2 m_Size;
	DXBuffer* m_pVertexBuffer;


public:
	Renderable2D(XMFLOAT3 position, XMFLOAT2 size, LPVOID bufferPointer, SIZE_T bufferSize)
		:m_Position(position), m_Size(size)
	{
		float vertices[] = {
			position.x, position.y, position.z, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f,
			position.x, position.y + size.y, position.z, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f,
			position.x + size.x, position.y + size.y, position.z, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f,
			position.x + size.x, position.y, position.z, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f
		};

		m_pVertexBuffer = new DXBuffer(ARRAYSIZE(vertices) * sizeof(float));
		m_pVertexBuffer->SetLayout(bufferPointer, bufferSize);
		m_pVertexBuffer->Resize(vertices);
	}

	void BindVAO()
	{
		m_pVertexBuffer->Bind();
	}

	virtual void submit()
	{

	}

};

