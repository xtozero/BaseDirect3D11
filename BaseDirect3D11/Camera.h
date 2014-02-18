#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

struct ProjConstantBuffer
{
	D3DXMATRIX m_d3dxmtProj;
	D3DXMATRIX m_d3dxmtInvProj;
};

class CCamera
{
private:
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProj;

	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;
	D3DXVECTOR3 m_d3dxvPosition;

	D3DXVECTOR3 m_d3dxvLookAtWorld;

	ID3D11Buffer* m_pd3dViewMatrixBuffer;
	ID3D11Buffer* m_pd3dProjMatrixBuffer;

	D3D11_VIEWPORT* m_pd3dViewports;
	BYTE			m_nViewports;
public:
	CCamera(void);
	~CCamera(void);

	void CreateViewport(int nSlot, float minZ, float maxZ, 
		float fClientHeight, float fClientWidth, float fTopLeftX = 0.0f, float fTopLeftY = 0.0f);
	void SetViewports(ID3D11DeviceContext* pd3dDeviceContext);
	
	void CreatePerspectiveMatrix(float fFov, float fAspectRatio, float fZNear, float fZFar);
	void CreateViewMatrix(void);
	void CreateViewMatirxLookAtWorld(void);
	void RegenerateViewMatrix(void);

	void OnDestory(void);

	void CreateViewAndProjBuffers(ID3D11Device* pd3dDevice);

	void UpdateViewMatrix(ID3D11DeviceContext* pd3dDeviceContext);
	void UpdateProjMatrix(ID3D11DeviceContext* pd3dDeviceContext);
};

