#include "StdAfx.h"
#include "Direct3D11.h"
#include "TextureManager.h"

CDirect3D11::CDirect3D11(HWND hWnd) : m_pd3dDevice(NULL), m_pd3dDeviceContext(NULL), 
	m_pdxgiSwapChain(NULL), m_hWnd(hWnd), m_pd3dDSResouceView(NULL)
{
}


CDirect3D11::~CDirect3D11(void)
{
	
}

bool CDirect3D11::CreateDeviceAndSwapChain()
{
	D3D_DRIVER_TYPE d3dDriverType[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_WARP,
	};

	UINT flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL d3dFeatureLevel[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	RECT windowRect;
	GetClientRect(m_hWnd, &windowRect);

	m_nClientWidth = windowRect.right - windowRect.left;
	m_nClientHeight = windowRect.bottom - windowRect.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));

	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.Height = m_nClientHeight;
	dxgiSwapChainDesc.BufferDesc.Width = m_nClientWidth;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.Flags = 0;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = true;

	for(int i = 0; i < sizeof(d3dDriverType) / sizeof(D3D_DRIVER_TYPE); i++)
	{
		if( SUCCEEDED(D3D11CreateDeviceAndSwapChain(
			NULL, 
			d3dDriverType[i], 
			NULL, 
			flag, 
			d3dFeatureLevel, 
			sizeof(d3dFeatureLevel) / sizeof(D3D_FEATURE_LEVEL), 
			D3D11_SDK_VERSION,
			&dxgiSwapChainDesc,
			&m_pdxgiSwapChain,
			&m_pd3dDevice,
			&m_d3dSelectedFeature,
			&m_pd3dDeviceContext)) )
		{
			return true;
		}	
	}

	MessageBox(m_hWnd, L"D3D11 Device 생성 실패", L"실패", MB_OK);
	return false;
}

bool CDirect3D11::CreateRenderTargetView()
{
	ID3D11Texture2D* d3dBackBuffer;

	if ( SUCCEEDED(m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dBackBuffer)) )
	{
		if ( SUCCEEDED(m_pd3dDevice->CreateRenderTargetView(d3dBackBuffer, NULL, &m_pd3dRenderTargetView)) )
		{
			d3dBackBuffer->Release();
			return true;
		}
		else
		{
			MessageBox(m_hWnd, L"BackBuffer RenderTargetView 생성 실패", L"실패", MB_OK);
			return false;
		}
	}
	else
	{
		MessageBox(m_hWnd, L"BackBuffer 획득 실패", L"실패", MB_OK);
		return false;
	}
}

bool CDirect3D11::CreateDepthStencilBufferAndView(bool bCreateShaderResouce)
{
	D3D11_TEXTURE2D_DESC d3dDepthStencilDesc;

	::ZeroMemory(&d3dDepthStencilDesc, sizeof(d3dDepthStencilDesc));

	d3dDepthStencilDesc.ArraySize = 1;
	d3dDepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	d3dDepthStencilDesc.CPUAccessFlags = 0;
	d3dDepthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	d3dDepthStencilDesc.Width = m_nClientWidth;
	d3dDepthStencilDesc.Height = m_nClientHeight;
	d3dDepthStencilDesc.MipLevels = 1;
	d3dDepthStencilDesc.MiscFlags = 0;
	d3dDepthStencilDesc.SampleDesc.Count = 1;
	d3dDepthStencilDesc.SampleDesc.Quality = 0;
	d3dDepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	if( SUCCEEDED(m_pd3dDevice->CreateTexture2D(&d3dDepthStencilDesc, NULL, &m_d3dDepthStencilBuffer)) )
	{
		//Do nothing
	}
	else
	{
		MessageBox(m_hWnd, L"DepthStencil 버퍼 생성 실패", L"실패", MB_OK);
		return false;
	}

	if( bCreateShaderResouce )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc,sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		if( SUCCEEDED( m_pd3dDevice->CreateShaderResourceView(m_d3dDepthStencilBuffer, &srvDesc, &m_pd3dDSResouceView) ) )
		{
			//Do nothing
		}
		else
		{
			MessageBox(m_hWnd, L"Depth Stencil ShaderResourceView 생성 실패", L"실패", MB_OK);
			return false;
		}
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	::ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	if( SUCCEEDED(m_pd3dDevice->CreateDepthStencilView(m_d3dDepthStencilBuffer, &dsvDesc, &m_pd3dDepthStencilView)) )
	{
		return true;
	}
	else
	{
		MessageBox(m_hWnd, L"Depth Stencil View 생성 실패", L"실패", MB_OK);
		return false;
	}
}

bool CDirect3D11::StartUp()
{
	if( CreateDeviceAndSwapChain() )
	{
		//Do nothing
	}
	else
	{
		return false;
	}
	if( CreateRenderTargetView() )
	{
		//Do nothing
	}
	else
	{
		return false;
	}
	if ( CreateDepthStencilBufferAndView(true) )
	{
		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
	}
	else
	{
		return false;
	}
	if ( BuildScene() )
	{
		return true;
	}
	{
		OnDestroy();
		return false;
	}
}

void CDirect3D11::OnDestroy()
{
	DestroyScene();

	CTextureManager::GetInstance()->OnDestroy();

	if( m_pd3dDeviceContext )
	{
		m_pd3dDeviceContext->ClearState();
	}

	if( m_pd3dDepthStencilView )
	{
		m_pd3dDepthStencilView->Release();
		m_pd3dDepthStencilView = NULL;
	}

	if ( m_pd3dDSResouceView )
	{
		m_pd3dDSResouceView->Release();
		m_pd3dDSResouceView = NULL;
	}

	if( m_d3dDepthStencilBuffer )
	{
		m_d3dDepthStencilBuffer->Release();
		m_d3dDepthStencilBuffer = NULL;
	}

	if( m_pd3dRenderTargetView )
	{
		m_pd3dRenderTargetView->Release();
		m_pd3dRenderTargetView = NULL;
	}

	if( m_pdxgiSwapChain )
	{
		m_pdxgiSwapChain->Release();
		m_pdxgiSwapChain = NULL;
	}

	if( m_pd3dDeviceContext )
	{
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = NULL;
	}

	if( m_pd3dDevice )
	{
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}

bool CDirect3D11::BuildScene()
{
	pCamera = new CCamera();

	pCamera->CreateViewport( 0, 0.0f, 1.0f, (float)m_nClientHeight, (float)m_nClientWidth);

	pCamera->SetViewports( m_pd3dDeviceContext );

	pCamera->CreatePerspectiveMatrix(60.0f, (float)m_nClientWidth / m_nClientHeight, 1.0f, 1000.0f);

	pCamera->CreateViewMatirxLookAtWorld();

	pCamera->CreateViewAndProjBuffers(m_pd3dDevice);

	pCamera->UpdateProjMatrix(m_pd3dDeviceContext);

	pMesh = new CCubeMesh(m_pd3dDevice, 2.0f, 2.0f, 2.0f, 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	pMesh->SetTexture(CTextureManager::GetInstance()->CreateTextrue(
		m_pd3dDevice, L".\\cloth.png"));

	pMesh->SetSampler(CTextureManager::GetInstance()->CreateSampler(
		m_pd3dDevice, new CSampler()));

	Object.SetMesh(pMesh);

	Object.SetPosition(0.0f, 0.0f, 0.0f);

	pDepthBufferPlane = new CPlaneMesh(m_pd3dDevice, D3DXVECTOR2(0.4f, 0.3f));

	pDepthBufferPlane->SetTexture( 
		std::shared_ptr<CTexture>(new CTexture(m_pd3dDSResouceView)) );

	pDepthBufferPlane->SetSampler(CTextureManager::GetInstance()->CreateSampler(
		m_pd3dDevice, new CSampler()));

	DepthBufferObject.SetMesh(pDepthBufferPlane);

	DepthBufferObject.SetPosition(1.25f, 0.9f, 0.0f);

	pSSDecalMesh = new CCubeMesh(m_pd3dDevice, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	pSSDecalMesh->SetTexture(CTextureManager::GetInstance()->CreateTextrue(
		m_pd3dDevice, L".\\UV_mapper.png"));

	pSSDecalMesh->SetSampler(CTextureManager::GetInstance()->CreateSampler(
		m_pd3dDevice, new CSampler()));

	SSDecalObject.SetMesh(pSSDecalMesh);

	SSDecalObject.SetPosition(0.0f, 0.0f, -1.0f);

	pWorldConstantBuffer = new CConstantBuffer<D3DXMATRIX>();

	if ( pWorldConstantBuffer->OnCreate(m_pd3dDevice) )
	{
		//Do Nothing
	}
	else
	{
		MessageBox(m_hWnd, L"상수버퍼 생성 실패", L"실패", MB_OK);
		return false;
	}

	pShader = new CTextureShader();

	if ( pShader->CreateShader(m_pd3dDevice) )
	{
		//Do Nothing
	}
	else
	{
		MessageBox(m_hWnd, L"셰이더 생성 실패", L"실패", MB_OK);
		return false;
	}

	pSSDecalShader = new CSSDecalShader();

	if ( pSSDecalShader->CreateShader(m_pd3dDevice) )
	{
		//Do Nothing
	}
	else
	{
		MessageBox(m_hWnd, L"데칼 셰이더 생성 실패", L"실패", MB_OK);
		return false;
	}
	return true;
}

void CDirect3D11::DestroyScene()
{
	if ( pMesh )
	{
		pMesh->ReleaseRef();

		delete pMesh;

		pMesh = NULL;
	}

	if ( pDepthBufferPlane )
	{
		pDepthBufferPlane->ReleaseRef();

		delete pDepthBufferPlane;

		pDepthBufferPlane = NULL;
	}

	if ( pSSDecalMesh )
	{
		pSSDecalMesh->ReleaseRef();

		delete pSSDecalMesh;

		pSSDecalMesh = NULL;
	}

	if ( pShader )
	{
		pShader->OnDestory();

		delete pShader;

		pShader = NULL;
	}

	if ( pSSDecalShader )
	{
		pSSDecalShader->OnDestory();

		delete pSSDecalShader;

		pSSDecalShader = NULL;
	}

	if ( pCamera )
	{
		pCamera->OnDestory();

		delete pCamera;

		pCamera = NULL;
	}

	if ( pWorldConstantBuffer )
	{
		pWorldConstantBuffer->OnDestory();

		delete pWorldConstantBuffer;

		pWorldConstantBuffer = NULL;
	}
}

void CDirect3D11::RenderScene()
{
	pShader->SetShader(m_pd3dDeviceContext);

	pCamera->UpdateViewMatrix(m_pd3dDeviceContext);

	Object.Animate(0.0000026f);

	pWorldConstantBuffer->Update(m_pd3dDeviceContext, Object.GetWorldMatrix(), 0);
	Object.Render(m_pd3dDeviceContext);

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView,NULL);
	pWorldConstantBuffer->Update(m_pd3dDeviceContext, DepthBufferObject.GetWorldMatrix(), 0);
	DepthBufferObject.Render(m_pd3dDeviceContext);
	
	m_pd3dDeviceContext->PSSetShaderResources(1, 1, &m_pd3dDSResouceView);
	pSSDecalShader->SetShader(m_pd3dDeviceContext);
	
	pWorldConstantBuffer->Update(m_pd3dDeviceContext, SSDecalObject.GetWorldMatrix(), 0);
	SSDecalObject.Render(m_pd3dDeviceContext);

	ID3D11ShaderResourceView* pInit[1] = {NULL};
	m_pd3dDeviceContext->PSSetShaderResources(1, 1, pInit);

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);


}

void CDirect3D11::FrameAdvance()
{
	float clearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f};

	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, clearColor);

	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	RenderScene();

	m_pdxgiSwapChain->Present(0, 0);
}