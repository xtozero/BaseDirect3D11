#include "StdAfx.h"
#include "Mesh.h"


CMesh::CMesh(void) : m_nReferance(0), m_d3dTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_pd3dVertexBuffer(NULL), m_pVertices(NULL), m_nVertices(0), m_nStride(0), m_nOffset(0),
	m_pd3dIndexBuffer(NULL), m_nIndices(0), m_nIndexStride(0), m_nIndexOffset(0), m_pTexture(NULL),
	m_pSampler(NULL)
{
	for( int i = 0; i < MAX_BOUNDING_OBJ; i++ )
	{
		m_pBoundingObj[i] = NULL;
	}
}


CMesh::~CMesh(void)
{
}

void CMesh::AddRef()
{
	++m_nReferance;
}

void CMesh::ReleaseRef()
{
	if ( --m_nReferance <= 0 )
	{
		Destroy();
	}
}

void CMesh::SetTexture(std::shared_ptr<CTexture> pTexture)
{
	m_pTexture = pTexture;
}

void CMesh::SetSampler(std::shared_ptr<CSampler> pSampler)
{
	m_pSampler = pSampler;
}

void CMesh::CreateBoundingObj(void)
{
	m_pBoundingObj[BOUDING_OBJ_ENUM::BOUNDING_SPHERE] = new CBoundingSphere();
	if( m_pBoundingObj[BOUDING_OBJ_ENUM::BOUNDING_SPHERE]->Create( reinterpret_cast<BYTE*>(m_pVertices), m_nStride, m_nVertices ) )
	{
		//Do Nothing
	}
	else
	{
		delete m_pBoundingObj[BOUDING_OBJ_ENUM::BOUNDING_SPHERE];
		m_pBoundingObj[BOUDING_OBJ_ENUM::BOUNDING_SPHERE] = NULL;
	}
}

void CMesh::Render(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->IASetPrimitiveTopology(m_d3dTopology);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	pDeviceContext->RSSetState(NULL);
	pDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, m_nIndexOffset);

	if( m_pSampler )
	{
		m_pSampler->PSSet(pDeviceContext, 0);
	}
	if( m_pTexture )
	{
		m_pTexture->PSSet(pDeviceContext, 0);
	}

	pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
}

void CMesh::Destroy()
{
	if( m_pd3dIndexBuffer )
	{
		m_pd3dIndexBuffer->Release();
		m_pd3dIndexBuffer = NULL;
	}

	if( m_pd3dVertexBuffer )
	{
		m_pd3dVertexBuffer->Release();
		m_pd3dVertexBuffer = NULL;
	}

	if( m_pVertices )
	{
		delete []m_pVertices;
		m_pVertices = NULL;
	}

	for( int i = 0; i < MAX_BOUNDING_OBJ; i++ )
	{
		if( m_pBoundingObj[i] )
		{
			delete m_pBoundingObj[i];
			m_pBoundingObj[i] = NULL;
		}
	}
}

CPlaneMesh::CPlaneMesh(ID3D11Device* pDevice, D3DXVECTOR2 d3dxvSize)
{
	m_d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_nVertices = 4;
	m_nStride = sizeof(CVertex);

	m_pVertices = new CVertex[m_nVertices];

	m_pVertices[0] = CVertex(D3DXVECTOR3(-d3dxvSize.x, d3dxvSize.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_pVertices[1] = CVertex(D3DXVECTOR3(d3dxvSize.x, d3dxvSize.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_pVertices[2] = CVertex(D3DXVECTOR3(d3dxvSize.x, -d3dxvSize.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_pVertices[3] = CVertex(D3DXVECTOR3(-d3dxvSize.x, -d3dxvSize.y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	D3D11_BUFFER_DESC bufferDesc;
	::ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = m_nStride * m_nVertices;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA bufferData;
	::ZeroMemory(&bufferData, sizeof(bufferData));

	bufferData.pSysMem = m_pVertices;

	HRESULT hResult = pDevice->CreateBuffer(&bufferDesc, &bufferData, &m_pd3dVertexBuffer);

	m_nIndices = 4;
	m_nIndexStride = sizeof(WORD);

	WORD indices[4];
	indices[0] = 3;
	indices[1] = 0;
	indices[2] = 2;
	indices[3] = 1;

	::ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = m_nIndexStride * m_nIndices;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	::ZeroMemory(&bufferData, sizeof(bufferData));

	bufferData.pSysMem = indices;

	hResult = pDevice->CreateBuffer(&bufferDesc, &bufferData, &m_pd3dIndexBuffer);
	CreateBoundingObj();
}

CPlaneMesh::~CPlaneMesh(void)
{

}

void CPlaneMesh::Render(ID3D11DeviceContext* pDeviceContext)
{
	CMesh::Render(pDeviceContext);
}

CCubeMesh::CCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor)
{
	m_nVertices = 24;
	m_nStride = sizeof(CVertex);
	m_nOffset = 0;

	m_d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

    m_pVertices = new CVertex[m_nVertices];

	//¾Õ¸é
	m_pVertices[0] = CVertex(D3DXVECTOR3(-fx, fy, -fz), d3dxColor, D3DXVECTOR2(0.0, 0.0));
	m_pVertices[1] = CVertex(D3DXVECTOR3(fx, fy, -fz), d3dxColor, D3DXVECTOR2(1.0, 0.0));
	m_pVertices[2] = CVertex(D3DXVECTOR3(fx, -fy, -fz), d3dxColor, D3DXVECTOR2(1.0, 1.0));
	m_pVertices[3] = CVertex(D3DXVECTOR3(-fx, -fy, -fz), d3dxColor, D3DXVECTOR2(0.0, 1.0));

	//À­¸é
	m_pVertices[4] = CVertex(D3DXVECTOR3(-fx, fy, fz), d3dxColor, D3DXVECTOR2(0.0, 0.0));
	m_pVertices[5] = CVertex(D3DXVECTOR3(fx, fy, fz), d3dxColor, D3DXVECTOR2(1.0, 0.0));
	m_pVertices[6] = CVertex(D3DXVECTOR3(fx, fy, -fz), d3dxColor, D3DXVECTOR2(1.0, 1.0));
	m_pVertices[7] = CVertex(D3DXVECTOR3(-fx, fy, -fz), d3dxColor, D3DXVECTOR2(0.0, 1.0));

	//¾Æ·§¸é
	m_pVertices[8] = CVertex(D3DXVECTOR3(-fx, -fy, -fz), d3dxColor, D3DXVECTOR2(0.0, 0.0));
	m_pVertices[9] = CVertex(D3DXVECTOR3(fx, -fy, -fz), d3dxColor, D3DXVECTOR2(1.0, 0.0));
	m_pVertices[10] = CVertex(D3DXVECTOR3(fx, -fy, fz), d3dxColor, D3DXVECTOR2(1.0, 1.0));
	m_pVertices[11] = CVertex(D3DXVECTOR3(-fx, -fy, fz), d3dxColor, D3DXVECTOR2(0.0, 1.0));

	//¿À¸¥ÂÊ ¸é
	m_pVertices[12] = CVertex(D3DXVECTOR3(fx, fy, -fz), d3dxColor, D3DXVECTOR2(0.0, 0.0));
	m_pVertices[13] = CVertex(D3DXVECTOR3(fx, fy, fz), d3dxColor, D3DXVECTOR2(1.0, 0.0));
	m_pVertices[14] = CVertex(D3DXVECTOR3(fx, -fy, fz), d3dxColor, D3DXVECTOR2(1.0, 1.0));
	m_pVertices[15] = CVertex(D3DXVECTOR3(fx, -fy, -fz), d3dxColor, D3DXVECTOR2(0.0, 1.0));

	//¿ÞÂÊ ¸é
	m_pVertices[16] = CVertex(D3DXVECTOR3(-fx, fy, fz), d3dxColor, D3DXVECTOR2(0.0, 0.0));
	m_pVertices[17] = CVertex(D3DXVECTOR3(-fx, fy, -fz), d3dxColor, D3DXVECTOR2(1.0, 0.0));
	m_pVertices[18] = CVertex(D3DXVECTOR3(-fx, -fy, -fz), d3dxColor, D3DXVECTOR2(1.0, 1.0));
	m_pVertices[19] = CVertex(D3DXVECTOR3(-fx, -fy, fz), d3dxColor, D3DXVECTOR2(0.0, 1.0));

	//µÞ¸é
	m_pVertices[20] = CVertex(D3DXVECTOR3(fx, fy, fz), d3dxColor, D3DXVECTOR2(0.0, 0.0));
	m_pVertices[21] = CVertex(D3DXVECTOR3(-fx, fy, fz), d3dxColor, D3DXVECTOR2(1.0, 0.0));
	m_pVertices[22] = CVertex(D3DXVECTOR3(-fx, -fy, fz), d3dxColor, D3DXVECTOR2(1.0, 1.0));
	m_pVertices[23] = CVertex(D3DXVECTOR3(fx, -fy, fz), d3dxColor, D3DXVECTOR2(0.0, 1.0));

	D3D11_BUFFER_DESC d3dBufferDesc;
	::ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    d3dBufferDesc.ByteWidth = sizeof(CVertex) * m_nVertices;
    d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA d3dBufferData;
	::ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    d3dBufferData.pSysMem = m_pVertices;

    HRESULT hResult = pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	m_nIndices = 36;

    WORD pIndices[36];
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
	pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;

	pIndices[6] = 4; pIndices[7] = 5; pIndices[8] = 6;
	pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 7;

	pIndices[12] = 8; pIndices[13] = 9; pIndices[14] = 10;
	pIndices[15] = 8; pIndices[16] = 10; pIndices[17] = 11;

	pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 14;
	pIndices[21] = 12; pIndices[22] = 14; pIndices[23] = 15;

	pIndices[24] = 16; pIndices[25] = 17; pIndices[26] = 18;
	pIndices[27] = 16; pIndices[28] = 18; pIndices[29] = 19;

	pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 22;
	pIndices[33] = 20; pIndices[34] = 22; pIndices[35] = 23;

	::ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    d3dBufferDesc.ByteWidth = sizeof(WORD) * m_nIndices;        
    d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	::ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    d3dBufferData.pSysMem = pIndices;

    hResult = pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);
	CreateBoundingObj();
}

void CCubeMesh::Render(ID3D11DeviceContext* pDeviceContext)
{
	CMesh::Render(pDeviceContext);
}

CCubeMesh::~CCubeMesh(void)
{

}