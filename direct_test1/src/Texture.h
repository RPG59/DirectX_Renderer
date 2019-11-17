#pragma once
#include <d3d11.h>
#include <WICTextureLoader.h>
#include "D3DContext.h"




class Texture {
private:
	ID3D11Resource* m_Texture;
	ID3D11ShaderResourceView* m_TextureView;
	ID3D11SamplerState* m_SamplerState;

public:
	void createTexturne(const wchar_t* filename) {
		size_t maxSize = 50 * 1024 * 1024;
		HRESULT hr;

		hr = DirectX::CreateWICTextureFromFile(
			D3DContext::GetDevice(),
			filename,
			&m_Texture,
			&m_TextureView,
			maxSize
		);

		if (FAILED(hr)) {
			DebugBreak();
		}
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		m_TextureView->GetDesc(&desc);

		D3DContext::GetDevContext()->GenerateMips(m_TextureView);

		D3DContext::GetDevContext()->PSSetShaderResources(0, 1, &m_TextureView);

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = D3DContext::GetDevice()->CreateSamplerState(&samplerDesc, &m_SamplerState);
		if (FAILED(hr)) {
			DebugBreak();
		}

	}

	inline ID3D11SamplerState** getSamplerState() { return &m_SamplerState; }

};
