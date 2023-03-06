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
#include "MyMap.h"
#include "Unit.h"
// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
	, m_iTileX(TILEX)
	, m_iTileY(TILEY)
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
	DDX_Control(pDX, IDC_FILE_DETAIL_BTN, m_DetailBtn);
	DDX_Text(pDX, IDC_TILE_SIZEX, m_iTileX);
	DDX_Text(pDX, IDC_TILE_SIZEY, m_iTileY);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_FILE_DETAIL_BTN, &CMyForm::OnDetailBtnClicked)
	ON_BN_CLICKED(IDC_GRID_CHECK, &CMyForm::OnGridCheckClicked)
	ON_LBN_SELCHANGE(IDC_MAIN_LB, &CMyForm::OnListBox)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MAIN_TREE, &CMyForm::OnTreeCtrl)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TILE_SIZE_BTN, &CMyForm::OnTileXYBtn)
	ON_LBN_DBLCLK(IDC_MAIN_LB, &CMyForm::OnListBoxDoubleClick)
	ON_BN_CLICKED(IDC_SAVE_ALL_BTN, &CMyForm::OnBnClickedSaveAllBtn)
	ON_BN_CLICKED(IDC_LOAD_ALL_BTN, &CMyForm::OnBnClickedLoadAllBtn)
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
	UpdateData(TRUE);
	
	int iSelect = m_ListBox.GetCurSel();

	DrawPictureControl(iSelect);

	DrawMap();

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

	CString cstrClicked = m_Tree.GetItemText(m_Tree.GetSelectedItem());
	if (cstrClicked == TEXT("Object") || 
		cstrClicked == TEXT("Tile") ||
		cstrClicked == TEXT("Map"))
	{
		if (cstrClicked == TEXT("Object"))
		{
			CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
			pToolView->m_ePickMod = PICK_OBJ;
		}
		cstrClicked = cstrClicked + L" Detail";
		m_DetailBtn.SetWindowText(cstrClicked);
	}
}

void CMyForm::DrawPictureControl(int _iSelect)
{
	CString cstrSelFile;
	m_ListBox.GetText(_iSelect, cstrSelFile);
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
}

void CMyForm::DrawMap()
{
	if (m_Tree.GetItemText(m_Tree.GetSelectedItem()) != _T("Stage"))
		return;

	m_ListBox.GetText(m_ListBox.GetCurSel(), m_strMapName);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	CFileInfo::ConvertRelativePath(szPath);
	PathRemoveFileSpec(szPath);
	m_wstrPath = szPath;
	Make_Path(m_wstrPath, m_Tree.GetSelectedItem());

	m_wstrPath = m_wstrPath + L"\\" + m_strMapName + L".png";

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	dynamic_cast<CMyMap*>(pToolView->m_pMap)->Set_Name(m_wstrPath.c_str(), m_strMapName);

	pToolView->Invalidate(FALSE);
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);
}

void CMyForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMyForm::OnTileXYBtn()
{
	UpdateData(TRUE);
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	pToolView->m_iTileX = this->m_iTileX;
	pToolView->m_iTileY = this->m_iTileY;
	dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Set_TileCnt(this->m_iTileX, this->m_iTileY);
	dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Release();
	dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Initialize();
	UpdateData(FALSE);

	pToolView->Invalidate(FALSE);
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);
}



void CMyForm::OnListBoxDoubleClick()
{
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (pToolView->m_ePickMod)
	{
	case PICK_OBJ:
	{
		CUnit* pUnit = new CUnit;
		pUnit->Initialize();
		pToolView->m_vecUnit.push_back(pUnit);
	}
		break;
	case PICK_TILE:
		break;
	default:
		break;
	}
}

void CMyForm::OnBnClickedSaveAllBtn()
{
	UpdateData(TRUE);
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	vector<TILE*>	vecTile = dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Get_vecTile();
	vector<CObj*>	vecObj = pToolView->m_vecUnit;

	CFileDialog		Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;
		int		iTileCnt = m_iTileX * m_iTileY;
		WriteFile(hFile, &m_iTileX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &m_iTileY, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iTileCnt, sizeof(int), &dwByte, nullptr);
		for (auto& Tile : vecTile)
		{
			WriteFile(hFile, &(Tile->vPos), sizeof(VERTEX), &dwByte, nullptr);
			WriteFile(hFile, &(Tile->vSize), sizeof(VERTEX), &dwByte, nullptr);
			WriteFile(hFile, &(Tile->byDrawID), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(Tile->byOption), sizeof(BYTE), &dwByte, nullptr);
		}
		// Unit List 돌면서 저장
		int		iUnitCnt = vecObj.size();
		WriteFile(hFile, &(iUnitCnt), sizeof(int), &dwByte, nullptr);
		for (auto& Obj : vecObj)
		{
			CUnit* Unit = dynamic_cast<CUnit*>(Obj);
			WriteFile(hFile, &(Unit->m_tUnitData.bCollision), sizeof(bool), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_tUnitData.iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_tUnitData.iHp), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_tUnitData.iLayer), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_tUnitData.iType), sizeof(int), &dwByte, nullptr);
			dwStrByte = 0;
			TCHAR*	Name = Unit->m_tUnitData.strName.GetBuffer(
				Unit->m_tUnitData.strName.GetLength());
			dwStrByte = sizeof(TCHAR) * (lstrlen(Name) + 1);
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, Name, dwStrByte, &dwByte, nullptr);
			Unit->m_tUnitData.strName.ReleaseBuffer();

			WriteFile(hFile, &(Unit->m_tImgPath.iCount), sizeof(int), &dwByte, nullptr);
			dwStrByte = 0;
			dwStrByte = sizeof(TCHAR) * (lstrlen(Unit->m_tImgPath.wstrObjKey.c_str()) + 1);
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, Unit->m_tImgPath.wstrObjKey.c_str(), dwStrByte, &dwByte, nullptr);
			dwStrByte = 0;
			dwStrByte = sizeof(TCHAR) * (lstrlen(Unit->m_tImgPath.wstrPath.c_str()) + 1);
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, Unit->m_tImgPath.wstrPath.c_str(), dwStrByte, &dwByte, nullptr);
			dwStrByte = 0; 
			dwStrByte = sizeof(TCHAR) * (lstrlen(Unit->m_tImgPath.wstrStateKey.c_str()) + 1);
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, Unit->m_tImgPath.wstrStateKey.c_str(), dwStrByte, &dwByte, nullptr);

			WriteFile(hFile, &(Unit->m_tInfo), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_vScroll), sizeof(VERTEX), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_fReverseX), sizeof(float), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_iAlpha), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(Unit->m_bInstalled), sizeof(bool), &dwByte, nullptr);
		}

		// Map name 저장
		dwStrByte = 0;
		dwStrByte = sizeof(TCHAR) * (lstrlen(m_strMapName) + 1);
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, m_strMapName, dwStrByte, &dwByte, nullptr);
		// Map Path 저장
		dwStrByte = 0;
		TCHAR pPath[MAX_STR] = L"";
		lstrcpy(pPath, m_wstrPath.c_str());
		dwStrByte = sizeof(TCHAR) * (lstrlen(pPath) + 1);
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, &pPath, dwStrByte, &dwByte, nullptr);

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CMyForm::OnBnClickedLoadAllBtn()
{
	UpdateData(TRUE);
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	CFileDialog		Dlg(TRUE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath,
			GENERIC_READ,
			0,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;
		UNITDATA	tData{};
		// 불러온 타일을 넣어줄 벡터
		vector<TILE*>	vecTile;
		vector<CObj*>	vecUnit;
		// 저장된 타일의 개수
		int iTileCnt = 0;
		// 맨앞에 타일의 개수가 저장되어 있으므로 그걸 가져옴
		ReadFile(hFile, &m_iTileX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &m_iTileY, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iTileCnt, sizeof(int), &dwByte, nullptr);
		dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Set_TileCnt(this->m_iTileX, this->m_iTileY);
		if (0 == dwByte)
			return;
		// 타일의 개수만큼만 반복
		for (int i = 0; i < iTileCnt; ++i)
		{
			TILE* Tile = new TILE;
			ReadFile(hFile, &(Tile->vPos), sizeof(VERTEX), &dwByte, nullptr);
			ReadFile(hFile, &(Tile->vSize), sizeof(VERTEX), &dwByte, nullptr);
			ReadFile(hFile, &(Tile->byDrawID), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(Tile->byOption), sizeof(BYTE), &dwByte, nullptr);
			if (0 == dwByte)
			{
				delete Tile;
				Tile = nullptr;
				break;
			}
			vecTile.push_back(Tile);
		}
		//타일 벡터를 터레인 클래스에 대입
		dynamic_cast<CMyTerrain*>(pToolView->m_pTerrain)->Load_vecTile(vecTile);

		// 유닛도 마찬가지로 for로 돌면서 대입
		int iUnitCnt = 0;
		ReadFile(hFile, &(iUnitCnt), sizeof(int), &dwByte, nullptr);
		if (0 == dwByte)
			return;
		for (auto& iter : pToolView->m_vecUnit)
		{
			Safe_Delete(iter);
		}
		for (int i = 0; i < iUnitCnt; ++i)
		{
			CObj* pUnit = new CUnit;
			CUnit* Unit = dynamic_cast<CUnit*>(pUnit);
			ReadFile(hFile, &(Unit->m_tUnitData.bCollision), sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_tUnitData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_tUnitData.iHp), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_tUnitData.iLayer), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_tUnitData.iType), sizeof(int), &dwByte, nullptr);

			dwStrByte = 0;
			TCHAR  pName[MAX_STR] = L"";
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
			Unit->m_tUnitData.strName = pName;

			ReadFile(hFile, &(Unit->m_tImgPath.iCount), sizeof(int), &dwByte, nullptr);

			dwStrByte = 0;
			TCHAR  objKey[MAX_STR] = L"";
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			ReadFile(hFile, objKey, dwStrByte, &dwByte, nullptr);
			Unit->m_tImgPath.wstrObjKey = objKey;

			dwStrByte = 0;
			TCHAR  objPath[MAX_STR] = L"";
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			ReadFile(hFile, objPath, dwStrByte, &dwByte, nullptr);
			Unit->m_tImgPath.wstrPath = objPath;

			dwStrByte = 0;
			TCHAR  objState[MAX_STR] = L"";
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			ReadFile(hFile, objState, dwStrByte, &dwByte, nullptr);
			Unit->m_tImgPath.wstrStateKey = objState;

			ReadFile(hFile, &(Unit->m_tInfo), sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_vScroll), sizeof(VERTEX), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_fReverseX), sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_iAlpha), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(Unit->m_bInstalled), sizeof(bool), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete pUnit;
				pUnit = nullptr;
				break;
			}
			vecUnit.push_back(pUnit);
		}
		pToolView->m_vecUnit = vecUnit;
		// 맵 로드
		dwStrByte = 0;
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, m_strMapName, dwStrByte, &dwByte, nullptr);
		dwStrByte = 0;
		TCHAR  pPath[MAX_STR] = L"";
		m_wstrPath = pPath;
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, pPath, dwStrByte, &dwByte, nullptr);

		dynamic_cast<CMyMap*>(pToolView->m_pMap)->Set_Name(pPath, m_strMapName);
		
		CloseHandle(hFile);
	}

	UpdateData(FALSE);

	pToolView->Invalidate(FALSE);
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);
}

