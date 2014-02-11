#include "StdAfx.h"
#include "BoundingObject.h"


CBoundingObject::CBoundingObject(void)
{
}

CBoundingObject::~CBoundingObject(void)
{
}

CBoundingSphere::CBoundingSphere(void) : m_d3dxvOrigin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_fHalfLenth(0.0f)
{
}

CBoundingSphere::~CBoundingSphere(void)
{
}

bool CBoundingSphere::Create(BYTE* pVertices, BYTE nElementSize, BYTE nElements)
{
	if( pVertices == NULL )
	{
		return false;
	}

	float fLenth = 0; // 길이가 마이너스가 나오는 경우는 없으니...

	for(int i = 0; i < nElements; i++)
	{
		D3DXVECTOR3* d3dxvPosition = (D3DXVECTOR3*)(pVertices + i * nElementSize);

		float fLenthSq = D3DXVec3LengthSq(d3dxvPosition);

		if( fLenthSq > fLenth )
		{
			fLenth = fLenthSq;
		}
	}

	m_fHalfLenth = sqrtf( fLenth );
	return true;
}

bool CBoundingSphere::IsCollision(CBoundingObject* pBoundingObj) const
{
	if( pBoundingObj == NULL )
	{
		return false;
	}
	//우선은 경계구만 지원
	if( typeid(CBoundingSphere) == typeid(*pBoundingObj) )
	{
		CBoundingSphere* pOther = static_cast<CBoundingSphere*>(pBoundingObj);  

		const float& fOtherHalfLenth = pOther->GetHalfLenth();
		const D3DXVECTOR3& d3dxvOtherOrigin = pOther->GetOrigin();

		D3DXVECTOR3 d3dxvToOrigin = d3dxvOtherOrigin - m_d3dxvOrigin;
		float fTotalHalfLenth = fOtherHalfLenth + m_fHalfLenth;

		if( D3DXVec3LengthSq(&d3dxvToOrigin) <= fTotalHalfLenth * fTotalHalfLenth )
		{
			return true;
		}
	}

	return false;
}

bool CBoundingSphere::IsCollision(const D3DXVECTOR3& d3dxvPoint) const
{
	float fLenthSq = D3DXVec3LengthSq( &(m_d3dxvOrigin - d3dxvPoint) );
	if( fLenthSq <= m_fHalfLenth * m_fHalfLenth )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CBoundingSphere::Update(const D3DXMATRIX& d3dxMatirx)
{
	m_d3dxvOrigin.x = d3dxMatirx._41;
	m_d3dxvOrigin.y = d3dxMatirx._42;
	m_d3dxvOrigin.z = d3dxMatirx._43;
}