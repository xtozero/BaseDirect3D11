#pragma once
#include <D3D11.h>
#include <string>

class CSampler
{
private:
	ID3D11SamplerState* m_pd3dSamplerState;
public:
	explicit CSampler(void);
	~CSampler(void);

	void OnCraete(ID3D11Device* pd3dDevice);
	void OnDestroy(void);

	void PSSet(ID3D11DeviceContext* pd3dDeviceContext, BYTE slot);
};

