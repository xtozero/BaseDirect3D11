#include "StdAfx.h"
#include "Ray.h"


CRay::CRay(void)
{
}


CRay::~CRay(void)
{
}

bool CRay::IsCollision(const CBoundingObject* boundingObj)
{
	if( typeid(CBoundingSphere) == typeid(*boundingObj) )
	{
		const CBoundingSphere* pBoundingSph = static_cast<const CBoundingSphere*>(boundingObj);

		const D3DXVECTOR3& d3dxvBoundingOrigin = pBoundingSph->GetOrigin();

		float fProjLenth = D3DXVec3Dot(&(d3dxvBoundingOrigin - m_d3dxvOrigin), &m_d3dxvDirection);

		D3DXVECTOR3 d3dxvProjPoint = m_d3dxvOrigin + m_d3dxvDirection * fProjLenth;

		return boundingObj->IsCollision(d3dxvProjPoint);
	}
	return false;
}