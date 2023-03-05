// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
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
	DDX_Control(pDX, IDC_GRID_CHECK, m_GirdRender);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_FILE_DETAIL_BTN, &CMyForm::OnDetailBtnClicked)
	ON_BN_CLICKED(IDC_GRID_CHECK, &CMyForm::OnGridCheckClicked)
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
	// 격자 ONOFF 체크박스 처음 상태를 true로 설정
	m_GirdRender.SetCheck(true);
}


void CMyForm::OnDetailBtnClicked()
{
	// switch 문에 따라 처리 
	// Obj폴더를 누럴ㅆ다
	/*switch (key)
	{
	case L"Obj":
		if (nullptr == m_ObjTool.GetSafeHwnd())
			m_ObjTool.Create(IDD_OBJTOOL);

		m_ObjTool.ShowWindow(SW_SHOW);
		break;
	case L"Tile":
		if (nullptr == m_ObjTool.GetSafeHwnd())
			m_ObjTool.Create(IDD_OBJTOOL);

		m_ObjTool.ShowWindow(SW_SHOW);
		break;
	case L"Map":
		if (nullptr == m_ObjTool.GetSafeHwnd())
			m_ObjTool.Create(IDD_OBJTOOL);

		m_ObjTool.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}*/
}


void CMyForm::OnGridCheckClicked()
{
	// 체크 박스를 클릭하면 그 체크박스 상태에 따라 ToolView와 MiniView의 격자를 ONOFF로 설정
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	
	if (m_GirdRender.GetCheck())
		dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Set_Render(true);
	else
		dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Set_Render(false);
	pToolView->Invalidate(FALSE);
	pMiniView->Invalidate(FALSE);
}
