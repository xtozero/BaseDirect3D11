#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <string>

class CTexture
{
private:
	ID3D11ShaderResourceView* m_pd3dTexture;
public:
	explicit CTexture(ID3D11Device* pd3dDevice, std::wstring pSrcFile);
	explicit CTexture(ID3D11ShaderResourceView* pd3dTexture);
	~CTexture(void);

	void OnDestroy(void);
	
	void PSSet(ID3D11DeviceContext* pd3dDeviceContext,BYTE slot);
};

