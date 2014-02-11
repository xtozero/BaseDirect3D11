#include "StdAfx.h"
#include "Sampler.h"


CSampler::CSampler(void) : m_pd3dSamplerState(NULL)
{
}


CSampler::~CSampler(void)
{
}

void CSampler::OnCraete(ID3D11Device* pd3dDevice)
{
	D3D11_SAMPLER_DESC d3d11SamplerDesc;

	::ZeroMemory(&d3d11SamplerDesc, sizeof(d3d11SamplerDesc));

	d3d11SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3d11SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3d11SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3d11SamplerDesc.BorderColor[0] = 1.0f;
	d3d11SamplerDesc.BorderColor[1] = 1.0f;
	d3d11SamplerDesc.BorderColor[2] = 1.0f;
	d3d11SamplerDesc.BorderColor[3] = 1.0f;
	d3d11SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3d11SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3d11SamplerDesc.MaxLOD = FLT_MAX;
	d3d11SamplerDesc.MinLOD = -FLT_MIN;

	pd3dDevice->CreateSamplerState(&d3d11SamplerDesc, &m_pd3dSamplerState);
}

void CSampler::OnDestroy(void)
{
	if(m_pd3dSamplerState)
	{
		m_pd3dSamplerState->Release();
		m_pd3dSamplerState = NULL;
	}
}

void CSampler::PSSet(ID3D11DeviceContext* pd3dDeviceContext, BYTE slot)
{
	pd3dDeviceContext->PSSetSamplers(slot, 1, &m_pd3dSamplerState);
}