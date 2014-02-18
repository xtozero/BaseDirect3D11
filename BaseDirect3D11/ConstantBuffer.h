#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <typeinfo>

template <class Element>
class CConstantBuffer
{
private:
	ID3D11Buffer* m_pd3dConstantBuffer;

	int			m_nElementSize;
	int			m_nElementCount;
public:
	explicit CConstantBuffer(int nElementCount = 1) : m_pd3dConstantBuffer(NULL), m_nElementCount(nElementCount)
	{
		m_nElementSize = sizeof(Element);
	}

	virtual ~CConstantBuffer(void)
	{

	}

	void OnDestory(void)
	{
		if( m_pd3dConstantBuffer )
		{
			m_pd3dConstantBuffer->Release();
			m_pd3dConstantBuffer = NULL;
		}
	}

	bool OnCreate(ID3D11Device* pd3dDevice)
	{
		D3D11_BUFFER_DESC d3d11BufferDesc;
		::ZeroMemory(&d3d11BufferDesc, sizeof(d3d11BufferDesc));

		d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d3d11BufferDesc.ByteWidth = m_nElementSize * m_nElementCount;
		d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		d3d11BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		if( SUCCEEDED(pd3dDevice->CreateBuffer(&d3d11BufferDesc, NULL, &m_pd3dConstantBuffer)) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Update(ID3D11DeviceContext* pd3dDeviceContext, Element* pElement, int slot)
	{
		D3D11_MAPPED_SUBRESOURCE d3dMappedResurce;

		pd3dDeviceContext->Map(
			m_pd3dConstantBuffer, 
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&d3dMappedResurce);

		Element* pValue = (Element*)d3dMappedResurce.pData;

		if( strcmp(typeid(*pElement).name(), "struct D3DXMATRIX") == 0 )
		{
			for( int i = 0; i < m_nElementCount; i++ )
			{
				D3DXMatrixTranspose(pValue + (sizeof(Element) *i), pElement + (sizeof(Element) *i));
			}
		}
		else
		{
			_memccpy(pValue, pElement, m_nElementCount, m_nElementSize);
		}

		pd3dDeviceContext->Unmap(m_pd3dConstantBuffer, 0);

		pd3dDeviceContext->VSSetConstantBuffers(slot, 1, &m_pd3dConstantBuffer);
		pd3dDeviceContext->PSSetConstantBuffers(slot, 1, &m_pd3dConstantBuffer);
	}
};

