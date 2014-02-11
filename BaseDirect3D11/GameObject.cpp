#include "StdAfx.h"
#include "GameObject.h"


CGameObject::CGameObject(void) : m_pMesh(NULL), m_d3dxvPitchYawRoll(0.0f, 0.0f, 0.0f)
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
}


CGameObject::~CGameObject(void)
{
}

D3DXMATRIX* CGameObject::GetWorldMatrix(void)
{
	return &m_d3dxmtxWorld;
}

void CGameObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	if( m_pMesh )
	{
		m_pMesh->Render(pDeviceContext);
	}
}

void CGameObject::SetMesh(CMesh* pMesh)
{
	if ( m_pMesh )
		m_pMesh->ReleaseRef();
	m_pMesh = pMesh;
	m_pMesh->AddRef();
}

void CGameObject::SetPosition(float fx, float fy, float fz)
{
	m_d3dxmtxWorld._41 = fx;
	m_d3dxmtxWorld._42 = fy;
	m_d3dxmtxWorld._43 = fz;
}

void CGameObject::Animate(float fElapsedTime)
{
	m_d3dxvPitchYawRoll.y += fElapsedTime * 30;
	D3DXMatrixRotationY(&m_d3dxmtxWorld, m_d3dxvPitchYawRoll.y);
}