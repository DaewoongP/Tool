#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath, 
										const TCHAR * pStateKey, 
										const int & iCount )
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 값을 채워주는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		AfxMessageBox(pFilePath);
		SAFE_DELETE(m_pTexInfo);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(DEVICE->Get_Device(), pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0, // 일반적인 텍스처 형식을 의미(D3DUSAGE_RENDERTARGET 을 설정할 경우 화면 텍스처 생성)
		m_pTexInfo->tImgInfo.Format, D3DPOOL_MANAGED,
		D3DX_DEFAULT, // 이미지 필터링 방식(필터링이란 이미지 확대, 축소 시 픽셀을 어떻게 채울 것인가에 대한 옵션)
		D3DX_DEFAULT,  // 밉맵을 이용한 이미지 필터링 방식
		0,				// 제거할 색상, 0을 컬러키 값을 무효화
		nullptr,		// 이미지 정보 기록
		nullptr,		// 팔레트 이미지에 대한 처리
		&(m_pTexInfo->pTexture))))
	{
		AfxMessageBox(L"Single Texture Load Failed");
		SAFE_DELETE(m_pTexInfo);
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	Safe_Delete(m_pTexInfo);
}
