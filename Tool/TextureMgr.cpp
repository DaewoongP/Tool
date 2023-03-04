#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto	iter = find_if(m_MapTex.begin(), m_MapTex.end(),
		[&](auto& Pair)->bool
	{
		if (pObjKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_MapTex.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCount);
}


HRESULT CTextureMgr::Insert_Texture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_MapTex.begin(), m_MapTex.end(), 
	[&](auto& Pair)->bool 
	{
		if (pObjKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_MapTex.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCount)))
		{
			AfxMessageBox(pFilePath);
			return E_FAIL;
		}

		m_MapTex.insert({ pObjKey, pTexture });
	}
	
	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_MapTex.begin(), m_MapTex.end(), [](auto& MyPair) { Safe_Delete(MyPair.second);});
	m_MapTex.clear();
}
