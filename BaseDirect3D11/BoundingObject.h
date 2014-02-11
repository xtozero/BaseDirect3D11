#pragma once

#include <D3DX10math.h>
#include <typeinfo>

class CBoundingObject
{
	//타이머 구현 후 업데이트 마스킹 필요할 듯.
public:
	CBoundingObject(void);
	virtual ~CBoundingObject(void);

	virtual bool Create(BYTE* pVertices, BYTE nElementSize, BYTE nElements) = 0;
	virtual bool IsCollision(CBoundingObject* pBoundingObj) const = 0;
	virtual bool IsCollision(const D3DXVECTOR3& d3dxvPoint) const = 0;

	virtual void Update(const D3DXMATRIX& d3dxMatirx) = 0;
};

class CBoundingSphere : public CBoundingObject
{
private:
	D3DXVECTOR3	m_d3dxvOrigin;
	float		m_fHalfLenth;
public:
	CBoundingSphere(void);
	virtual ~CBoundingSphere(void);

	virtual bool Create(BYTE* pVertices, BYTE nElementSize, BYTE nElements);
	virtual bool IsCollision(CBoundingObject* pBoundingObj) const;
	virtual bool IsCollision(const D3DXVECTOR3& d3dxvPoint) const;

	virtual void Update(const D3DXMATRIX& d3dxMatirx);

	const D3DXVECTOR3& GetOrigin(void) const {return m_d3dxvOrigin;} 
	const float& GetHalfLenth(void) const {return m_fHalfLenth;} 
};
