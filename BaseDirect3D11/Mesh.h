#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <memory>
#include "Texture.h"
#include "Sampler.h"
#include "BoundingObject.h"

const int MAX_BOUNDING_OBJ = 1;
namespace BOUDING_OBJ_ENUM
{
	enum BOUNDING_OBJ
	{
		BOUNDING_SPHERE = 0,
		AABB_BOUNDING_BOX,
		OBB_BOUNDING_BOX,
		END,
	};
}

class CVertex
{
private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXCOLOR	m_d3dxColor;
	D3DXVECTOR2 m_d3dxvTexCoord;

public:
	CVertex()
	{

	}
	CVertex(D3DXVECTOR3& d3dxvPosition, D3DXCOLOR& d3dxColor, D3DXVECTOR2& d3dxvTexCoord) 
		: m_d3dxvPosition(d3dxvPosition), m_d3dxColor(d3dxColor), m_d3dxvTexCoord(d3dxvTexCoord)
	{

	}
	CVertex(CVertex& vertex) : m_d3dxvPosition(vertex.m_d3dxvPosition), m_d3dxColor(vertex.m_d3dxColor), m_d3dxvTexCoord(vertex.m_d3dxvTexCoord)
	{

	}
	~CVertex()
	{

	}
};

class CMesh
{
protected:
	int m_nReferance;

	D3D11_PRIMITIVE_TOPOLOGY m_d3dTopology;

	ID3D11Buffer *m_pd3dVertexBuffer;
	CVertex* m_pVertices;
	UINT m_nVertices;
	UINT m_nStride;
	UINT m_nOffset;

	ID3D11Buffer *m_pd3dIndexBuffer;
	UINT m_nIndices;
	UINT m_nIndexStride;
	UINT m_nIndexOffset;

	CBoundingObject* m_pBoundingObj[MAX_BOUNDING_OBJ];

	///////////////////////////////////////////////////////////////
	//테스트용

	std::shared_ptr<CTexture> m_pTexture;
	std::shared_ptr<CSampler> m_pSampler;
	
	///////////////////////////////////////////////////////////////
public:
	CMesh(void);
	virtual ~CMesh(void);

	void AddRef();
	void ReleaseRef();

	void SetTexture(std::shared_ptr<CTexture> pTexture);
	void SetSampler(std::shared_ptr<CSampler> pSampler);
	void CreateBoundingObj(void); 

	virtual void Render(ID3D11DeviceContext* pDeviceContext);
	virtual void Destroy();
};

class CPlaneMesh : public CMesh
{
public:
	CPlaneMesh(ID3D11Device* pDevice, D3DXVECTOR2 d3dxvSize);
	virtual ~CPlaneMesh(void);

	virtual void Render(ID3D11DeviceContext* pDeviceContext);
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(ID3D11Device *pd3dDevice, float fWidth=2.0f, float fHeight=2.0f, float fDepth=2.0f, D3DXCOLOR d3dxColor=D3DXCOLOR(1.0f,1.0f,0.0f,0.0f));
	virtual ~CCubeMesh(void);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext = false);
};