
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MyTerrain.h"
#include "MiniView.h"
#include "MyMap.h"
#include "Unit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

HWND	g_hWnd;

CToolView::CToolView()
	:m_pTerrain(nullptr), m_pMap(nullptr), m_iTileX(TILEX), m_iTileY(TILEY)
	, m_ePickMod(PICK_END)
{
	m_Tile.byDrawID = 0;
	m_Tile.byOption = 1;
}

CToolView::~CToolView()
{
}
#pragma region 필요없음

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG

#pragma endregion 필요없음

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILECX * m_iTileX, TILECY * m_iTileY / 2));

	RECT	rcWnd{};
	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->GetWindowRect(&rcWnd);
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT rcMainView{};
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	g_hWnd = m_hWnd;
	
	if (FAILED(DEVICE->InitDevice()))
	{
		AfxMessageBox(L"Device Create Failed");
		return;
	}
	
	m_pTerrain = new CMyTerrain;

	if (FAILED(m_pTerrain->Initialize()))
	{
		AfxMessageBox(L"MyTerrain Create Failed");
		return;
	}

	m_pTerrain->Set_MainView(this);

	m_pMap = new CMyMap;

	if (FAILED(m_pMap->Initialize()))
	{
		AfxMessageBox(L"MyMap Create Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Read_ImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"Image Load failed");
		return;
	}

	m_pMap->Set_MainView(this);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DEVICE->Render_Begin();

	m_pMap->Render();
	m_pTerrain->Render();
	
	for (auto& Unit : m_vecUnit)
	{
		Unit->Set_View(this);
		Unit->Render();
	}
		
	
	DEVICE->Render_End();
}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();
	Safe_Delete(m_pMap);
	Safe_Delete(m_pTerrain);
	for_each(m_vecUnit.begin(), m_vecUnit.end(), Safe_Delete<CObj*>);
	TEXTURE->Destroy_Instance();
	DEVICE->Destroy_Instance();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateData(TRUE);
	CScrollView::OnLButtonDown(nFlags, point);
	
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	switch (m_ePickMod)
	{
	case PICK_OBJ:
	{
		m_vecUnit.back()->Set_Alpha(255);
		m_vecUnit.back()->Set_Installed();
		CObj* pUnit = new CUnit;
		pUnit->Initialize();
		pUnit->Set_Pos(::Get_Mouse().x + GetScrollPos(0), ::Get_Mouse().y + GetScrollPos(1));
		m_vecUnit.push_back(pUnit);
	}
		break;
	case PICK_TILE:
		m_pTerrain->TileChange(D3DXVECTOR3(float(point.x) + GetScrollPos(0),
			float(point.y) + GetScrollPos(1), 0.f), m_Tile.byDrawID, m_Tile.byOption);
		break;
	default:
		break;
	}
	
	Invalidate(FALSE);

	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);
	UpdateData(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnRButtonDown(nFlags, point);
	
	if (PICK_OBJ == m_ePickMod)
	{
		auto iter = m_vecUnit.end() - 1;
		Safe_Delete(m_vecUnit.back());
		m_vecUnit.erase(iter);
		m_ePickMod = PICK_END;
	}

	Invalidate(FALSE);
		
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);
	
	switch (m_ePickMod)
	{
	case PICK_OBJ:
		if (m_vecUnit.empty())
			break;
		if (true == m_vecUnit.back()->Get_Installed())
			break;

		m_vecUnit.back()->Set_Pos(Get_Mouse().x, Get_Mouse().y);
		break;
	case PICK_TILE:
		break;
	default:
		break;
	}

	Invalidate(FALSE);
}
