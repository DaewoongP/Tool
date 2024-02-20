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

		// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� ���� ä���ִ� �Լ�
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
			0, // �Ϲ����� �ؽ�ó ������ �ǹ�(D3DUSAGE_RENDERTARGET �� ������ ��� ȭ�� �ؽ�ó ����)
			pTexInfo->tImgInfo.Format, D3DPOOL_MANAGED,
			D3DX_DEFAULT, // �̹��� ���͸� ���(���͸��̶� �̹��� Ȯ��, ��� �� �ȼ��� ��� ä�� ���ΰ��� ���� �ɼ�)
			D3DX_DEFAULT,  // �Ӹ��� �̿��� �̹��� ���͸� ���
			0,				// ������ ����, 0�� �÷�Ű ���� ��ȿȭ
			nullptr,		// �̹��� ���� ���
			nullptr,		// �ȷ�Ʈ �̹����� ���� ó��
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
