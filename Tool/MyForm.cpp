// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "MyTerrain.h"
#include "FileInfo.h"

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
	DDX_Control(pDX, IDC_MAIN_TREE, m_Tree);
	DDX_Control(pDX, IDC_MAIN_LB, m_ListBox);
	DDX_Control(pDX, IDC_FILE_PC, m_Picture);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_FILE_DETAIL_BTN, &CMyForm::OnDetailBtnClicked)
	ON_BN_CLICKED(IDC_GRID_CHECK, &CMyForm::OnGridCheckClicked)
	ON_LBN_SELCHANGE(IDC_MAIN_LB, &CMyForm::OnListBox)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MAIN_TREE, &CMyForm::OnTreeCtrl)
	ON_WM_DESTROY()
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

	CFileFind Find;
	TCHAR	szPath[MAX_PATH] = L"";

	// GetCurrentDirectory : 현재 프로젝트가 있는 디렉토리 경로를 얻어오는 함수

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Texture");
	wstring wstrPath = szPath; // 경로만들기.

	m_Tree.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);
	Make_Tree(wstrPath, m_Tree.GetRootItem());
}


void CMyForm::OnDetailBtnClicked()
{
	CString cstrClicked = m_Tree.GetItemText(m_Tree.GetSelectedItem());

	if (cstrClicked == TEXT("Object"))
	{
		if (nullptr == m_ObjTool.GetSafeHwnd())
			m_ObjTool.Create(IDD_OBJTOOL);

		m_ObjTool.ShowWindow(SW_SHOW);
	}
	else if (cstrClicked == TEXT("Tile"))
	{
		if (nullptr == m_TileTool.GetSafeHwnd())
			m_TileTool.Create(IDD_TILETOOL);

		m_TileTool.ShowWindow(SW_SHOW);
	}
	else if (cstrClicked == TEXT("Map"))
	{
		if (nullptr == m_MapTool.GetSafeHwnd())
			m_MapTool.Create(IDD_MAPTOOL);

		m_MapTool.ShowWindow(SW_SHOW);
	}
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

void CMyForm::Make_Tree(wstring & wstrPath, HTREEITEM parent)
{
	CFileFind Find;
	wstring		wstrFilePath = wstrPath + L"\\*.*";
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());
	
	while (bContinue)
	{
  		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		if (Find.IsDirectory())
		{
			CString cstr = Find.GetFileTitle();
			HTREEITEM curTree = m_Tree.InsertItem(cstr, 0, 0, parent, TVI_LAST);
			wstring curFilePath = wstrPath + L"\\" + cstr.GetString();
			Make_Tree(curFilePath, curTree);
		}
	}
}

void CMyForm::Make_Path(wstring & wstrOut, HTREEITEM curTree)
{
	wstring wstrText = m_Tree.GetItemText(curTree).GetString();

	if (m_Tree.GetRootItem() == curTree)
	{
		wstrOut += L"\\" + wstrText;
		return;
	}

	Make_Path(wstrOut, m_Tree.GetParentItem(curTree));
	wstrOut += L"\\" + wstrText;
}

void CMyForm::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	int iSelect = m_ListBox.GetCurSel();
	CString cstrSelFile;
	m_ListBox.GetText(iSelect, cstrSelFile);
	wstring wstrImgPath = m_wstrCurDir + L"\\" + cstrSelFile.GetString() + L".png";
	TCHAR szPath[MAX_PATH];
	lstrcpy(szPath, wstrImgPath.c_str());

	CImage PngImage;
	CRect rect;
	m_Picture.GetClientRect(rect);
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_Picture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	//PngImage.Draw(dc->m_hDC, rect);
	PngImage.Load(szPath);
	PngImage.StretchBlt(dc->m_hDC, rect);

	ReleaseDC(dc);//DC 해제
	//m_Picture.SetBitmap(PngImage);
	
	UpdateData(FALSE);
}


void CMyForm::OnTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	wstring wstrPath = szPath;
	Make_Path(wstrPath, m_Tree.GetSelectedItem());
	m_wstrCurDir = wstrPath;

	m_ListBox.ResetContent();

	int FileCnt = CFileInfo::DirFileCnt(szPath);

	//CFileFind : MFC에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind			Find;

	// 경로 상의 모든 파일을 찾아야 하기 때문에 경로 수정
	wstring		wstrFilePath = wstrPath + L"\\*.*";
	
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());
	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
			continue;

		else if (Find.IsSystem())
			continue;

		TCHAR szPathBuf[MAX_STR] = L"";
		lstrcpy(szPathBuf, Find.GetFilePath().GetString());
		wstring		wstrFileName = Find.GetFileTitle().GetString();

		m_ListBox.AddString(wstrFileName.c_str());
	}
}


void CMyForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMyForm::OnGridCheckClicked()
{
	// üũ �ڽ��� Ŭ���ϸ� �� üũ�ڽ� ���¿� ��� ToolView�� MiniView�� ���ڸ� ONOFF�� ���
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
