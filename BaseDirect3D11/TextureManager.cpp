#include "StdAfx.h"
#include "TextureManager.h"

CTextureManager* CTextureManager::m_pInstance = NULL;

CTextureManager::CTextureManager(void)
{
}

CTextureManager::~CTextureManager(void)
{
}

void CTextureManager::OnDestroy(void)
{
	for( auto iter = m_textureMap.begin();
		iter != m_textureMap.end();
		iter++ )
	{
		iter->second->OnDestroy();
	}

	for( auto iter = m_samplerMap.begin();
		iter != m_samplerMap.end();
		iter++ )
	{
		iter->second->OnDestroy();
	}

	delete m_pInstance;
	m_pInstance = NULL;
}

std::shared_ptr<CTexture> CTextureManager::CreateTextrue(ID3D11Device* pd3dDevice, std::wstring strFile)
{
	auto iter = m_textureMap.find(strFile);

	if ( iter == m_textureMap.end() )
	{
		m_textureMap[strFile] = std::shared_ptr<CTexture>( 
			new CTexture(pd3dDevice, strFile) );

		return m_textureMap[strFile];
	}
	else
	{
		if (iter->second == NULL)
		{
			m_textureMap[strFile] = std::shared_ptr<CTexture>( 
			new CTexture(pd3dDevice, strFile) );

			return m_textureMap[strFile];
		}
		else
		{
			return iter->second;
		}
	}
}

std::shared_ptr<CSampler> CTextureManager::CreateSampler(ID3D11Device* pd3dDevice, CSampler* pSampler)
{
	if(pSampler == NULL) return NULL;

	auto iter = m_samplerMap.find(typeid(*pSampler).name());

	if( iter == m_samplerMap.end()  )
	{
		pSampler->OnCraete(pd3dDevice);

		m_samplerMap[typeid(*pSampler).name()] = 
			std::shared_ptr<CSampler>(pSampler);

		return m_samplerMap[typeid(*pSampler).name()];
	}
	else
	{
		std::shared_ptr<CSampler>(pSampler);
		return iter->second;
	}
}
