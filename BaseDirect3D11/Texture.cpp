#include "StdAfx.h"
#include "Texture.h"


CTexture::CTexture(ID3D11Device* pd3dDevice, std::wstring pSrcFile) : 
m_pd3dTexture(NULL)
{
	D3DX11CreateShaderResourceViewFromFile(
		pd3dDevice,
		pSrcFile.c_str(),
		0,
		0,
		&m_pd3dTexture,
		0);
}

CTexture::CTexture(ID3D11ShaderResourceView* pd3dTexture)
	: m_pd3dTexture(pd3dTexture)
{

}

CTexture::~CTexture(void)
{
}

void CTexture::OnDestroy(void)
{
	if( m_pd3dTexture )
	{
		m_pd3dTexture->Release();
		m_pd3dTexture = NULL;
	}
}

void CTexture::PSSet(ID3D11DeviceContext* pd3dDeviceContext, BYTE slot)
{
	pd3dDeviceContext->PSSetShaderResources(slot, 1, &m_pd3dTexture);
}