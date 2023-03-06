#include "stdafx.h"
#include "Unit.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "TextureMgr.h"
#include "FileInfo.h"

CUnit::CUnit() : CObj()
{
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	// 깊은복사하려면 일케 하는거 아니야?
	m_tUnitData.strName = pMyForm->m_ObjTool.m_strName.GetString();
	m_tUnitData.iType = pMyForm->m_ObjTool.m_Type_Combo.GetCurSel();
	m_tUnitData.iLayer = pMyForm->m_ObjTool.m_iLayer;
	m_tUnitData.iAttack = pMyForm->m_ObjTool.m_iAttack;
	m_tUnitData.iHp = pMyForm->m_ObjTool.m_iHp;
	m_tUnitData.bCollision = pMyForm->m_ObjTool.m_Collision_Check.GetCheck();
}

CUnit::~CUnit()
{
}

HRESULT CUnit::Initialize(void)
{
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	HTREEITEM leaf = pMyForm->m_Tree.GetSelectedItem();
	HTREEITEM parent = pMyForm->m_Tree.GetParentItem(leaf);

	m_tImgPath.wstrObjKey = pMyForm->m_Tree.GetItemText(parent);
	m_tImgPath.wstrStateKey = pMyForm->m_Tree.GetItemText(leaf);

	int idx = pMyForm->m_ListBox.GetCurSel();
	CString strText;
	pMyForm->m_ListBox.GetText(idx, strText);

	m_tImgPath.iCount = CFileInfo::ExtractIntOfBackFromCString(strText);

	m_tInfo.vPos = { Get_Mouse().x, Get_Mouse().y, 0.f };
	m_iAlpha = 120;
	return E_NOTIMPL;
}

int CUnit::Update(void)
{
	return 0;
}

void CUnit::Late_Update(void)
{
}

void CUnit::Release(void)
{
}

void CUnit::Render(void)
{
	// 행렬을 변환하는 부분
	D3DXMATRIX		matWorld, matScale, matTrans;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f * m_fReverseX, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	matWorld = matScale * matTrans;
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	// 텍스쳐를 가져오는 부분
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(
		m_tImgPath.wstrObjKey.c_str(), m_tImgPath.wstrStateKey.c_str(), m_tImgPath.iCount);

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, // 출력할 이미지 영역에 대한 RECT 구조체 포인터 주소(NULL인 경우 이미지의 0, 0기준으로 출력)
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 VECTOR3 구조체 포인터 주소(NULL인 경우 0, 0이 중심 좌표)
		nullptr, // 위치 좌표에 대한 VECTOR3 구조체 포인터 주소(NULL인 경우 스크린의 0,0 좌료 출력)
		D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상 값, 0xffffffff를 매개 변수로 전달할 경우 텍스처의 원본 색상 유지
}

void CUnit::Mini_Render(void)
{
}
