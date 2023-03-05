#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"


CMyTerrain::CMyTerrain()
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

		//GetClientRect : 현재 클라이언트 영역의 rect 정보를 얻어옴
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
			nullptr, // 출력할 이미지 영역에 대한 RECT 구조체 포인터 주소(NULL인 경우 이미지의 0, 0기준으로 출력)
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 VECTOR3 구조체 포인터 주소(NULL인 경우 0, 0이 중심 좌표)
			nullptr, // 위치 좌표에 대한 VECTOR3 구조체 포인터 주소(NULL인 경우 스크린의 0,0 좌료 출력)
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상 값, 0xffffffff를 매개 변수로 전달할 경우 텍스처의 원본 색상 유지

		swprintf_s(szBuf, L"%d", iIndex);

		DEVICE->Get_Font()->DrawTextW(DEVICE->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		++iIndex;
	}
}

void CMyTerrain::Mini_Render(void)
{
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
		if (Picking_Dot(vPos, index))
			return index;
	}

	return -1;
}

bool CMyTerrain::Picking(const D3DXVECTOR3 & vPos, const int& iIndex)
{
	//y = ax + b;


	float	fGradient[4] = {

		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)

	};

	D3DXVECTOR3		vPoint[4] = {

		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
	};

	//y = ax  + b

	//b = y - ax 

	float		fB[4] = {

		vPoint[0].y - fGradient[0] * vPoint[0].x,
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x
	};

	bool		bCheck[4]{ false };

	//0 == ax + b - y  // 선분 상에 있다
	//0 > ax + b - y   // 선분 위에 있다
	//0 < ax + b - y	// 선분 아래 있다

	// 12시 -> 3시
	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	// 3시 -> 6시
	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	// 6시 -> 9시
	if (0 >= fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	// 9시 -> 12시
	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;

	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CMyTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
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
		// 예각인 경우 타일 외부를 피킹한 것
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

