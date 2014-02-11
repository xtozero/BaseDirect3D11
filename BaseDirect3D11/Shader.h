#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>

class CShader
{
protected:
	ID3D11VertexShader* m_pd3dVertexShader;
	ID3D11PixelShader* m_pd3dPixelShader;

	ID3D11InputLayout* m_pd3dInputLayout;
public:
	CShader(void);
	~CShader(void);

	bool CreateVertexShaderFromFile(ID3D11Device* pd3dDevice, _TCHAR* pszFileName, 
		LPCSTR pszShaderName, LPCSTR pszShaderModel, D3D11_INPUT_ELEMENT_DESC* d3dInputDesc, UINT nInputElements);
	bool CreatePixelShaderFromFile(ID3D11Device* pd3dDevice, _TCHAR* pszFileName, 
		LPCSTR pszShaderName, LPCSTR pszShaderModel);
	
	virtual bool CreateShader(ID3D11Device* pd3dDevice) = 0;
	void OnDestory(void);

	void SetShader(ID3D11DeviceContext* pd3dDeviceContext);
};

class CTextureShader : public CShader
{
public:
	virtual bool CreateShader(ID3D11Device* pd3dDevice);
};