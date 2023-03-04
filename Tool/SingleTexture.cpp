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

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� ���� ä���ִ� �Լ�
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
		0, // �Ϲ����� �ؽ�ó ������ �ǹ�(D3DUSAGE_RENDERTARGET �� ������ ��� ȭ�� �ؽ�ó ����)
		m_pTexInfo->tImgInfo.Format, D3DPOOL_MANAGED,
		D3DX_DEFAULT, // �̹��� ���͸� ���(���͸��̶� �̹��� Ȯ��, ��� �� �ȼ��� ��� ä�� ���ΰ��� ���� �ɼ�)
		D3DX_DEFAULT,  // �Ӹ��� �̿��� �̹��� ���͸� ���
		0,				// ������ ����, 0�� �÷�Ű ���� ��ȿȭ
		nullptr,		// �̹��� ���� ���
		nullptr,		// �ȷ�Ʈ �̹����� ���� ó��
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
