#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

#include "Mesh.h"

class CGameObject
{
protected:
	CMesh* m_pMesh;

	D3DXMATRIX m_d3dxmtxWorld;
	
	D3DXVECTOR3 m_d3dxvPitchYawRoll;
public:
	CGameObject(void);
	virtual ~CGameObject(void);

	D3DXMATRIX* GetWorldMatrix(void);

	virtual void Render(ID3D11DeviceContext* pDeviceContext);

	void SetMesh(CMesh* m_pMesh);

	void SetPosition(float fx, float fy, float fz);

	void CGameObject::Animate(float fElapsedTime);
};

