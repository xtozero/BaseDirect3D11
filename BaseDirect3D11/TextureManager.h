#pragma once
#include <map>
#include <string>
#include <D3D11.h>
#include <typeinfo>
#include "Texture.h"
#include "Sampler.h"

class CTextureManager
{
private:
	static CTextureManager* m_pInstance;

	std::map<std::wstring, std::shared_ptr<CTexture>> m_textureMap;
	std::map<const char*, std::shared_ptr<CSampler>> m_samplerMap;
public:
	static CTextureManager* GetInstance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CTextureManager();
			return m_pInstance;
		}
		else
		{
			return m_pInstance;
		}
	}

	CTextureManager(void);
	~CTextureManager(void);

	void OnDestroy(void);

	std::shared_ptr<CTexture> CreateTextrue(ID3D11Device* pd3dDevice, std::wstring strFile);
	std::shared_ptr<CSampler> CreateSampler(ID3D11Device* pd3dDevice, CSampler* pSampler);
};

