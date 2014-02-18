#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "ConstantBuffer.h"

class CDirect3D11
{
private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;
	IDXGISwapChain* m_pdxgiSwapChain;

	D3D_FEATURE_LEVEL m_d3dSelectedFeature;

	ID3D11RenderTargetView* m_pd3dRenderTargetView;

	ID3D11Texture2D* m_d3dDepthStencilBuffer;
	ID3D11DepthStencilView* m_pd3dDepthStencilView;
	ID3D11ShaderResourceView* m_pd3dDSResouceView;

	HWND m_hWnd;
	UINT m_nClientWidth;
	UINT m_nClientHeight;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//테스트용 임시 변수 선언

	CMesh* pMesh;
	CMesh* pDepthBufferPlane;
	CMesh* pSSDecalMesh;
	CGameObject Object;
	CGameObject DepthBufferObject;
	CGameObject SSDecalObject;
	CShader* pShader;
	CShader* pSSDecalShader;
	CCamera* pCamera;
	CConstantBuffer<D3DXMATRIX>* pWorldConstantBuffer;

	/////////////////////////////////////////////////////////////////////////////////////////////
public:
	CDirect3D11(HWND hWnd);
	~CDirect3D11(void);

	bool CreateDeviceAndSwapChain();
	bool CreateRenderTargetView();
	bool CreateDepthStencilBufferAndView(bool bCreateShaderResouce = false);
	
	bool StartUp();

	void OnDestroy();

	bool BuildScene();
	void DestroyScene();
	void RenderScene();
	void FrameAdvance();
};

