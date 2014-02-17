#include "StdAfx.h"
#include "Camera.h"


CCamera::CCamera(void) : m_d3dxvRight(1.0f, 0.0f, 0.0f),
	m_d3dxvUp(0.0f, 1.0f, 0.0f), m_d3dxvLook(0.0f, 0.0f, 1.0f),
	m_d3dxvPosition(0.0f, 0.0f, -2.0f), m_d3dxvLookAtWorld(0.0f, 0.0f, 0.0f),
	m_pd3dViewMatrixBuffer(NULL), m_pd3dProjMatrixBuffer(NULL),
	m_pd3dViewports(NULL), m_nViewports(0)
{
	D3DXMatrixIdentity(&m_d3dxmtxView);
	D3DXMatrixIdentity(&m_d3dxmtxProj);
}


CCamera::~CCamera(void)
{
}

void CCamera::CreateViewport(int nSlot, float minZ, float maxZ, 
	float fClientHeight, float fClientWidth, float fTopLeftX, float fTopLeftY)
{
	D3D11_VIEWPORT d3dViewport;

	d3dViewport.TopLeftX = fTopLeftX;
	d3dViewport.TopLeftY = fTopLeftY;
	d3dViewport.Height = fClientHeight;
	d3dViewport.Width = fClientWidth;
	d3dViewport.MinDepth = minZ;
	d3dViewport.MaxDepth = maxZ;

	if( nSlot >= m_nViewports )
	{
		D3D11_VIEWPORT* pViewports = new D3D11_VIEWPORT[m_nViewports + 1];

		for( int i = 0; i < m_nViewports; i++ )
		{
			pViewports[i] = m_pd3dViewports[i];
		}

		delete []m_pd3dViewports;
		m_pd3dViewports = pViewports;

		m_pd3dViewports[m_nViewports] = d3dViewport;
		++m_nViewports;
	}
	else
	{
		m_pd3dViewports[nSlot] = d3dViewport;
	}
}

void CCamera::SetViewports(ID3D11DeviceContext* pd3dDeviceContext)
{
	pd3dDeviceContext->RSSetViewports(m_nViewports, m_pd3dViewports);
}

void CCamera::CreatePerspectiveMatrix(float fFov, float fAspectRatio, float fZNear, float fZFar)
{
	D3DXMatrixPerspectiveFovLH(
		&m_d3dxmtxProj,
		static_cast<float>( D3DXToRadian(fFov) ),
		fAspectRatio,
		fZNear,
		fZFar);
}

void CCamera::CreateViewMatrix(void)
{
	m_d3dxmtxView._11 = m_d3dxvRight.x; m_d3dxmtxView._12 = m_d3dxvUp.x; m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y; m_d3dxmtxView._22 = m_d3dxvUp.y; m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z; m_d3dxmtxView._32 = m_d3dxvUp.z; m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = -D3DXVec3Dot(&m_d3dxvRight, &m_d3dxvPosition);
	m_d3dxmtxView._42 = -D3DXVec3Dot(&m_d3dxvUp, &m_d3dxvPosition);
	m_d3dxmtxView._43 = -D3DXVec3Dot(&m_d3dxvLook, &m_d3dxvPosition);
}

void CCamera::CreateViewMatirxLookAtWorld(void)
{
	D3DXMatrixLookAtLH(
		&m_d3dxmtxView,
		&m_d3dxvPosition,
		&m_d3dxvLookAtWorld,
		&m_d3dxvUp);
}

void CCamera::RegenerateViewMatrix(void)
{
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
    D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
    D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
    D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
    D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	m_d3dxmtxView._11 = m_d3dxvRight.x; m_d3dxmtxView._12 = m_d3dxvUp.x; m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y; m_d3dxmtxView._22 = m_d3dxvUp.y; m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z; m_d3dxmtxView._32 = m_d3dxvUp.z; m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = - D3DXVec3Dot(&m_d3dxvRight, &m_d3dxvPosition);
	m_d3dxmtxView._42 = - D3DXVec3Dot(&m_d3dxvUp, &m_d3dxvPosition);
	m_d3dxmtxView._43 = - D3DXVec3Dot(&m_d3dxvLook, &m_d3dxvPosition);
}

void CCamera::OnDestory(void)
{
	if( m_pd3dViewports )
	{
		delete []m_pd3dViewports;
		m_pd3dViewports = NULL;
	}
	if( m_pd3dViewMatrixBuffer )
	{
		m_pd3dViewMatrixBuffer->Release();
		m_pd3dViewMatrixBuffer = NULL;
	}
	if( m_pd3dProjMatrixBuffer )
	{
		m_pd3dProjMatrixBuffer->Release();
		m_pd3dProjMatrixBuffer = NULL;
	}
}

void CCamera::CreateViewAndProjBuffers(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC d3d11BufferDesc;

	::ZeroMemory(&d3d11BufferDesc, sizeof(d3d11BufferDesc));

	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3d11BufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3d11BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	pd3dDevice->CreateBuffer(&d3d11BufferDesc, NULL, &m_pd3dViewMatrixBuffer);

	pd3dDevice->CreateBuffer(&d3d11BufferDesc, NULL, &m_pd3dProjMatrixBuffer);
}

void CCamera::UpdateViewMatrix(ID3D11DeviceContext* pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(
		m_pd3dViewMatrixBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&d3dMappedResource);

	D3DXMATRIX* pMatrix = (D3DXMATRIX*)d3dMappedResource.pData;
	D3DXMatrixTranspose(pMatrix, &m_d3dxmtxView);

	pd3dDeviceContext->Unmap(m_pd3dViewMatrixBuffer, 0);

	pd3dDeviceContext->VSSetConstantBuffers(1, 1, &m_pd3dViewMatrixBuffer);
}

void CCamera::UpdateProjMatrix(ID3D11DeviceContext* pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(
		m_pd3dProjMatrixBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&d3dMappedResource);

	D3DXMATRIX* pMatrix = (D3DXMATRIX*)d3dMappedResource.pData;
	D3DXMatrixTranspose(pMatrix, &m_d3dxmtxProj);

	pd3dDeviceContext->Unmap(m_pd3dProjMatrixBuffer, 0);

	pd3dDeviceContext->VSSetConstantBuffers(2, 1, &m_pd3dProjMatrixBuffer);
}