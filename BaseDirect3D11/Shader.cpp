#include "StdAfx.h"
#include "Shader.h"


CShader::CShader(void) : m_pd3dVertexShader(NULL), m_pd3dPixelShader(NULL), m_pd3dInputLayout(NULL)
{
}


CShader::~CShader(void)
{
}

bool CShader::CreateVertexShaderFromFile(ID3D11Device* pd3dDevice, _TCHAR* pszFileName, 
	LPCSTR pszShaderName, LPCSTR pszShaderModel, D3D11_INPUT_ELEMENT_DESC* d3dInputDesc, UINT nInputElements)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* d3dVertexShaderBlob = NULL;
	ID3DBlob* d3dErrorBlob = NULL;

	if ( SUCCEEDED(D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName,
		pszShaderModel, dwShaderFlags, 0, NULL, &d3dVertexShaderBlob, &d3dErrorBlob, NULL)) )
	{
		pd3dDevice->CreateVertexShader(d3dVertexShaderBlob->GetBufferPointer(), d3dVertexShaderBlob->GetBufferSize(), NULL, &m_pd3dVertexShader);
		pd3dDevice->CreateInputLayout(d3dInputDesc, nInputElements, d3dVertexShaderBlob->GetBufferPointer(), d3dVertexShaderBlob->GetBufferSize(), &m_pd3dInputLayout);
		d3dVertexShaderBlob->Release();
		return true;
	}
	else
	{
		d3dErrorBlob->Release();
		return false;
	}
}

bool CShader::CreatePixelShaderFromFile(ID3D11Device* pd3dDevice, _TCHAR* pszFileName, 
	LPCSTR pszShaderName, LPCSTR pszShaderModel)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* d3dPixelShaderBlob = NULL;
	ID3DBlob* d3dErrorBlob = NULL;

	if ( SUCCEEDED(D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName,
		pszShaderModel, dwShaderFlags, 0, NULL, &d3dPixelShaderBlob, &d3dErrorBlob, NULL)) )
	{
		pd3dDevice->CreatePixelShader(d3dPixelShaderBlob->GetBufferPointer(), d3dPixelShaderBlob->GetBufferSize(), NULL, &m_pd3dPixelShader);
		d3dPixelShaderBlob->Release();
		return true;
	}
	else
	{
		d3dErrorBlob->Release();
		return false;
	}
}

void CShader::OnDestory(void)
{
	if( m_pd3dPixelShader )
	{
		m_pd3dPixelShader->Release();
		m_pd3dPixelShader = NULL;
	}

	if( m_pd3dInputLayout )
	{
		m_pd3dInputLayout->Release();
		m_pd3dInputLayout = NULL;
	}

	if( m_pd3dVertexShader )
	{
		m_pd3dVertexShader->Release();
		m_pd3dVertexShader = NULL;
	}
}

void CShader::SetShader(ID3D11DeviceContext* pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dInputLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
}

bool CTextureShader::CreateShader(ID3D11Device* pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dinputElement[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	if ( CreateVertexShaderFromFile(pd3dDevice, L".\\TextureShader.fx", "VSTexture", "vs_5_0", d3dinputElement, sizeof(d3dinputElement) / sizeof(D3D11_INPUT_ELEMENT_DESC)) )
	{
		//Do Nothing
	}
	else
	{
		return false;
	}
	if ( CreatePixelShaderFromFile(pd3dDevice, L".\\TextureShader.fx", "PSTexture", "ps_5_0") )
	{
		//Do Nothing
	}
	else
	{
		return false;
	}

	return true;
}

bool CSSDecalShader::CreateShader(ID3D11Device* pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dinputElement[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	if ( CreateVertexShaderFromFile(pd3dDevice, L".\\SSDecalShader.fx", "VSSSDecal", "vs_5_0", d3dinputElement, sizeof(d3dinputElement) / sizeof(D3D11_INPUT_ELEMENT_DESC)) )
	{
		//Do Nothing
	}
	else
	{
		return false;
	}
	if ( CreatePixelShaderFromFile(pd3dDevice, L".\\SSDecalShader.fx", "PSSSDecal", "ps_5_0") )
	{
		//Do Nothing
	}
	else
	{
		return false;
	}

	return true;
}