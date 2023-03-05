// TileTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TileTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CTileTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTileTool, CDialog)

CTileTool::CTileTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TILETOOL, pParent)
{
}

CTileTool::~CTileTool()
{
}

void CTileTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TILE_SPLIT, m_TileSplit);
}


BEGIN_MESSAGE_MAP(CTileTool, CDialog)
	ON_BN_CLICKED(IDC_TILE_SPLIT, &CTileTool::OnBnClickedTileSplit)
END_MESSAGE_MAP()


// CTileTool 메시지 처리기입니다.


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
			CString strTile;
			strTile.Format(_T("Tile"));

			strNum = strNum.Right(2);
			strTile += strNum;
			m_TileSplit.SetWindowText(strTile);
			AfxMessageBox(_T("타일 : ") + strTile);
		}
		
	}
	return CDialog::OnCommand(wParam, lParam);
}
