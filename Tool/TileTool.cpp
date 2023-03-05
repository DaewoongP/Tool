// TileTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TileTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"


// CTileTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTileTool, CDialog)

CTileTool::CTileTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TILETOOL, pParent)
{
	// 초기값 설정
	m_strTile = L"Tile0";
}

CTileTool::~CTileTool()
{
}

void CTileTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TILE_SPLIT, m_TileSplit);
	DDX_Control(pDX, IDC_TILE_COLL_RADIO1, m_CollRadio[0]);
	DDX_Control(pDX, IDC_TILE_COLL_RADIO2, m_CollRadio[1]);
	DDX_Control(pDX, IDC_TILE_LB, m_TileListBox);
}


BEGIN_MESSAGE_MAP(CTileTool, CDialog)
	ON_BN_CLICKED(IDC_TILE_SPLIT, &CTileTool::OnBnClickedTileSplit)
	ON_BN_CLICKED(IDC_TILE_SAVE_BTN, &CTileTool::OnSaveBtnClicked)
	ON_LBN_SELCHANGE(IDC_TILE_LB, &CTileTool::OnTileListBoxClicked)
	ON_BN_CLICKED(IDC_TILE_SAVE_BTN2, &CTileTool::OnDeleteBtnClicked)
	ON_BN_CLICKED(IDOK, &CTileTool::OnTileSelectBtnClicked)
END_MESSAGE_MAP()


// CTileTool 메시지 처리기입니다.


BOOL CTileTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	FileFinder();

	return TRUE;
}


void CTileTool::OnBnClickedTileSplit()
{
	CMenu menu;
	// 팝업 메뉴를 생성한다.
	menu.CreatePopupMenu();
	
	CString str;
	for (int i = 0; i < CFileInfo::DirFileCnt(L"../Texture/Tile"); ++i) {
		str.Format(_T("Tile%d"), i);
		// 팝업 메뉴에 메뉴를 추가한다.
		menu.AppendMenu(MF_STRING, 20000 + i, (LPCTSTR)str);
	}

	// 스플릿버튼의 위치값
	RECT rc = {};
	GetDlgItem(IDC_TILE_SPLIT)->GetWindowRect(&rc);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rc.left, rc.top + 25, this);

	// 생성된 팝업 메뉴를 삭제한다.
	menu.DestroyMenu();
}


BOOL CTileTool::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0)
	{
		int id = LPARAM(wParam);
		TRACE(_T("id: %d\n"), id);

		// 스플릿 버튼 메뉴의 아이디 값에 따른 이벤트 처리
		if (id >= 20000 && id < 20000 + CFileInfo::DirFileCnt(L"../Texture/Tile"))
		{
			CString strNum;
			strNum.Format(_T("%d"), id);
			
			m_strTile.Format(_T("Tile"));

			strNum = strNum.Right(1);
			m_strTile += strNum;

			// 버튼 타이틀 변경
			m_TileSplit.SetWindowText(m_strTile);
			AfxMessageBox(_T("타일 : ") + m_strTile);
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}


void CTileTool::OnSaveBtnClicked()
{
	TILE	pTile;
	
	if (m_CollRadio[0].GetCheck())
		pTile.byOption = 0;

	if (m_CollRadio[1].GetCheck())
		pTile.byOption = 1;

	pTile.byDrawID = _ttoi(m_strTile.Right(1));

	CFileDialog		Dlg(FALSE,		// FALSE(다른 이름으로 저장) , TRUE(불러오기)
		L"dat",		// 기본 파일 확장자명(명시적으로 확장자명을 기입하지 않을 경우, 이 확장자명으로 자동 삽입)
		L"*.dat", // 대화상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 파일로 저장 시 경고 메세지 띄움)
		L"Data Files(*.dat)|*.dat||",  // 대화 상자에 표시될 파일 형식
		this); // 부모 윈도우의 주소

	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;


	// DoModal : 대화 상자를 실행
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 선택된 경로를 반환
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		
		WriteFile(hFile, &(pTile.byDrawID), sizeof(BYTE), &dwByte, nullptr);
		WriteFile(hFile, &(pTile.byOption), sizeof(BYTE), &dwByte, nullptr);

		CloseHandle(hFile);
	}
	// 저장후 리스트 갱신
	FileFinder();
}

void CTileTool::FileFinder()
{
	CFileFind			Find;
	BOOL		bContinue = Find.FindFile(L"../Data/*.*");
	m_TileListBox.ResetContent();
	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;
		if (Find.IsDirectory())
			continue;
		if (Find.IsSystem())
			continue;

		m_TileListBox.AddString(Find.GetFileTitle().GetString());
	}
}


void CTileTool::OnTileListBoxClicked()
{
	UpdateData(TRUE);
	TILE	pTile;
	// 리스트박스의 선택한 셀의 텍스트를 가져옴
	CString	strLoad;
	m_TileListBox.GetText(m_TileListBox.GetCurSel(), strLoad);
	
	CString strPath = L"../Data/";
	strPath += strLoad;
	strPath += L".dat";
	const TCHAR*	pGetPath = strPath.GetString();

	HANDLE hFile = CreateFile(pGetPath,
		GENERIC_READ,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;

	ReadFile(hFile, &(pTile.byDrawID), sizeof(BYTE), &dwByte, nullptr);
	ReadFile(hFile, &(pTile.byOption), sizeof(BYTE), &dwByte, nullptr);

	if (pTile.byOption == 0)
	{
		m_CollRadio[0].SetCheck(true);
		m_CollRadio[1].SetCheck(false);
	}
		
	if (pTile.byOption == 1)
	{
		m_CollRadio[0].SetCheck(false);
		m_CollRadio[1].SetCheck(true);
	}
	CString strTile = L"Tile";
	CString strNum;
	strNum.Format(_T("%d"), pTile.byDrawID);
	m_strTile = strTile + strNum;
	m_TileSplit.SetWindowText(m_strTile);
		
	CloseHandle(hFile);

	UpdateData(FALSE);
}


void CTileTool::OnDeleteBtnClicked()
{
	CFileFind Find;
	CString	strLoad;
	m_TileListBox.GetText(m_TileListBox.GetCurSel(), strLoad);

	CString strPath = L"../Data/";
	strPath += strLoad;
	strPath += L".dat";

	const TCHAR*	pGetPath = strPath.GetString();

	if (Find.FindFile(pGetPath))
	{
		if (DeleteFile(pGetPath) == TRUE)
		{
			CString strDel;
			strDel = m_strTile + L" 파일 삭제 완료";
			m_TileListBox.DeleteString(m_TileListBox.GetCurSel());
			AfxMessageBox(strDel);
		}
	}
}


void CTileTool::OnTileSelectBtnClicked()
{
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	
	if (m_CollRadio[0].GetCheck())
		pToolView->m_Tile.byOption = 0;
	else
		pToolView->m_Tile.byOption = 1;

	
	if (0 <= _ttoi(m_strTile.Right(1)))
		pToolView->m_Tile.byDrawID = _ttoi(m_strTile.Right(1));

	CDialog::OnOK();
}
