// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "ToolView.h"
#include "MyTerrain.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_GridCheck);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_CHECK1, &CMyForm::OnGridCheckBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnObjectTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Font.CreatePointFont(100, L"고딕");
}


void CMyForm::OnGridCheckBox()
{
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	if (!m_GridCheck.GetCheck())
		dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Set_Render(false);
	else
		dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Set_Render(true);

	pToolView->Invalidate(FALSE);

	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);
}


void CMyForm::OnMapTool()
{
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);

	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnObjectTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_ObjectTool.GetSafeHwnd())
		m_ObjectTool.Create(IDD_OBJECTTOOL);

	m_ObjectTool.ShowWindow(SW_SHOW);	// 창 모양으로 출력
}
