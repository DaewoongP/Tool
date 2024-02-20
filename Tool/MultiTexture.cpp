#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto	iter = find_if(m_MapMultiTex.begin(), m_MapMultiTex.end(), [&](auto& Pair)
	{
		if (pStateKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_MapMultiTex.end())
		return nullptr;
	
	return iter->second[iCount];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	TCHAR	szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 값을 채워주는 함수
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			AfxMessageBox(szFullPath);
			SAFE_DELETE(pTexInfo);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(DEVICE->Get_Device(), 
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, // 일반적인 텍스처 형식을 의미(D3DUSAGE_RENDERTARGET 을 설정할 경우 화면 텍스처 생성)
			pTexInfo->tImgInfo.Format, D3DPOOL_MANAGED,
			D3DX_DEFAULT, // 이미지 필터링 방식(필터링이란 이미지 확대, 축소 시 픽셀을 어떻게 채울 것인가에 대한 옵션)
			D3DX_DEFAULT,  // 밉맵을 이용한 이미지 필터링 방식
			0,				// 제거할 색상, 0을 컬러키 값을 무효화
			nullptr,		// 이미지 정보 기록
			nullptr,		// 팔레트 이미지에 대한 처리
			&(pTexInfo->pTexture))))
		{
			AfxMessageBox(L"Single Texture Load Failed");
			SAFE_DELETE(pTexInfo);
			return E_FAIL;
		}

		m_MapMultiTex[pStateKey].push_back(pTexInfo);
	}
	
	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_MapMultiTex.begin(), m_MapMultiTex.end(), [](auto& Pair)
	{
		for_each(Pair.second.begin(), Pair.second.end(), Safe_Delete<TEXINFO*>);
		Pair.second.clear();
	});

	m_MapMultiTex.clear();
}
