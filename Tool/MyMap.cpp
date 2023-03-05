#include "stdafx.h"
#include "MyMap.h"

CMyMap::CMyMap()
	:m_pMainView(nullptr)
{
}


CMyMap::~CMyMap()
{
	Release();
}

HRESULT CMyMap::Initialize(void)
{

	return S_OK;
}

int CMyMap::Update(void)
{
	return 0;
}

void CMyMap::Late_Update(void)
{
}

void CMyMap::Release(void)
{
}

void CMyMap::Render()
{
	RECT		rc;

	//GetClientRect : ���� Ŭ���̾�Ʈ ������ rect ������ ����
	GetClientRect(m_pMainView->m_hWnd, &rc);

	const TEXINFO*	pTexInfo = TEXTURE->Get_Texture(m_pMapName, m_pMapName, 0);
	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX		matWorld, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		-m_pMainView->GetScrollPos(0),
		-m_pMainView->GetScrollPos(1),
		0.f);
	matWorld = matScale * matTrans;

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX, fY);
	
	DEVICE->Get_Sprite()->SetTransform(&matWorld);
	
	DEVICE->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, // ����� �̹��� ������ ���� RECT ����ü ������ �ּ�(NULL�� ��� �̹����� 0, 0�������� ���)
		&D3DXVECTOR3(0.f, 0.f, 0.f), // ����� �̹����� �߽� �࿡ ���� VECTOR3 ����ü ������ �ּ�(NULL�� ��� 0, 0�� �߽� ��ǥ)
		nullptr, // ��ġ ��ǥ�� ���� VECTOR3 ����ü ������ �ּ�(NULL�� ��� ��ũ���� 0,0 �·� ���)
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyMap::Mini_Render(void)
{
	RECT		rc;

	//GetClientRect : ���� Ŭ���̾�Ʈ ������ rect ������ ����
	GetClientRect(m_pMainView->m_hWnd, &rc);

	const TEXINFO*	pTexInfo = TEXTURE->Get_Texture(m_pMapName, m_pMapName, 0);
	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX		matWorld, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		-m_pMainView->GetScrollPos(0),
		-m_pMainView->GetScrollPos(1),
		0.f);
	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, 0.3f, 0.3f);

	DEVICE->Get_Sprite()->SetTransform(&matWorld);

	DEVICE->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, // ����� �̹��� ������ ���� RECT ����ü ������ �ּ�(NULL�� ��� �̹����� 0, 0�������� ���)
		&D3DXVECTOR3(0.f, 0.f, 0.f), // ����� �̹����� �߽� �࿡ ���� VECTOR3 ����ü ������ �ּ�(NULL�� ��� 0, 0�� �߽� ��ǥ)
		nullptr, // ��ġ ��ǥ�� ���� VECTOR3 ����ü ������ �ּ�(NULL�� ��� ��ũ���� 0,0 �·� ���)
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyMap::Set_Ratio(D3DXMATRIX * pOut, const float & fRatioX, const float & fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}

void CMyMap::Set_Name(const TCHAR* path, const TCHAR * _Name)
{
	lstrcpy(m_pMapName, _Name);
	TEXTURE->Insert_Texture(path, TEX_SINGLE, m_pMapName, m_pMapName, 0);
}
