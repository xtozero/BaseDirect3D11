#pragma once

#include <D3DX10math.h>
#include <typeinfo>
#include "BoundingObject.h"

class CRay
{
private:
	D3DXVECTOR3 m_d3dxvOrigin;
	D3DXVECTOR3 m_d3dxvDirection;
public:
	CRay(void);
	~CRay(void);

	bool IsCollision(const CBoundingObject* boundingObj);
};

