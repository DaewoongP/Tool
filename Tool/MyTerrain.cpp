#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"


CMyTerrain::CMyTerrain()
	:m_bRender(false)
{
	m_vecTile.reserve(TILEX * TILEY);
}

CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(TEXTURE->Insert_Texture(L"../Texture/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 5)))
	{
		AfxMessageBox(L"Tile Texture Insert Failed");
		return E_FAIL;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float	fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);
			float	fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX, fY , 0.f };
			pTile->vSize = { (float)TILECX , (float)TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}
int CMyTerrain::Update(void)
{

	return 0;
}

void CMyTerrain::Late_Update(void)
{

}

void CMyTerrain::Render()
{
	if (!m_bRender)
		return;
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	for (auto pTile : m_vecTile)
	{
		D3DXMATRIX		matWorld, matScale, matTrans;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x - m_pMainView->GetScrollPos(0),
			pTile->vPos.y - m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		RECT		rc;

		//GetClientRect : ���� Ŭ���̾�Ʈ ������ rect ������ ����
		GetClientRect(m_pMainView->m_hWnd, &rc);

		float fX = WINCX / float(rc.right - rc.left);
		float fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX, fY);

		const TEXINFO*	pTexInfo = TEXTURE->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);
		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		DEVICE->Get_Sprite()->SetTransform(&matWorld);

		DEVICE->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, // ����� �̹��� ������ ���� RECT ����ü ������ �ּ�(NULL�� ��� �̹����� 0, 0�������� ���)
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� VECTOR3 ����ü ������ �ּ�(NULL�� ��� 0, 0�� �߽� ��ǥ)
			nullptr, // ��ġ ��ǥ�� ���� VECTOR3 ����ü ������ �ּ�(NULL�� ��� ��ũ���� 0,0 �·� ���)
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� ���� �̹����� ���� ���� ��, 0xffffffff�� �Ű� ������ ������ ��� �ؽ�ó�� ���� ���� ����

		swprintf_s(szBuf, L"%d", iIndex);

		DEVICE->Get_Font()->DrawTextW(DEVICE->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		++iIndex;
	}
}

void CMyTerrain::Mini_Render(void)
{
	if (!m_bRender)
		return;
	D3DXMATRIX		matWorld, matScale, matTrans;

	for (auto pTile : m_vecTile)
	{
		D3DXMATRIX		matWorld, matScale, matTrans;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x,
			pTile->vPos.y,
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.315f, 0.608f);

		const TEXINFO*	pTexInfo = TEXTURE->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);
		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		DEVICE->Get_Sprite()->SetTransform(&matWorld);

		DEVICE->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMyTerrain::Set_Ratio(D3DXMATRIX * pOut, const float & fRatioX, const float & fRatioY)
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

void CMyTerrain::TileChange(const D3DXVECTOR3 & vPos, const BYTE & byDrawID)
{
	int	iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CMyTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking(vPos, index))
			return index;
	}

	return -1;
}

bool CMyTerrain::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	D3DXVECTOR3		vPoint[4] = {

		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
	};

	D3DXVECTOR3		vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4] = {

		{ -vDir[0].y , vDir[0].x, 0.f },
		{ -vDir[1].y , vDir[1].x, 0.f },
		{ -vDir[2].y , vDir[2].x, 0.f },
		{ -vDir[3].y , vDir[3].x, 0.f },

	};

	D3DXVECTOR3		vMouseDir[4] = {

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// ������ ��� Ÿ�� �ܺθ� ��ŷ�� ��
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}
