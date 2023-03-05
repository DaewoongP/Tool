// TileTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "TileTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CTileTool ��ȭ �����Դϴ�.

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


// CTileTool �޽��� ó�����Դϴ�.


void CTileTool::OnBnClickedTileSplit()
{
	CMenu menu;
	// �˾� �޴��� �����Ѵ�.
	menu.CreatePopupMenu();
	
	CString str;
	for (int i = 0; i < CFileInfo::DirFileCnt(L"../Texture/Tile"); ++i) {
		str.Format(_T("Tile%d"), i);
		// �˾� �޴��� �޴��� �߰��Ѵ�.
		menu.AppendMenu(MF_STRING, 20000 + i, (LPCTSTR)str);
	}

	// ���ø���ư�� ��ġ��
	RECT rc = {};
	GetDlgItem(IDC_TILE_SPLIT)->GetWindowRect(&rc);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rc.left, rc.top + 25, this);

	// ������ �˾� �޴��� �����Ѵ�.
	menu.DestroyMenu();
}


BOOL CTileTool::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0)
	{
		int id = LPARAM(wParam);
		TRACE(_T("id: %d\n"), id);

		// ���ø� ��ư �޴��� ���̵� ���� ���� �̺�Ʈ ó��
		if (id >= 20000 && id < 20000 + CFileInfo::DirFileCnt(L"../Texture/Tile"))
		{
			CString strNum;
			strNum.Format(_T("%d"), id);
			CString strTile;
			strTile.Format(_T("Tile"));

			strNum = strNum.Right(2);
			strTile += strNum;
			m_TileSplit.SetWindowText(strTile);
			AfxMessageBox(_T("Ÿ�� : ") + strTile);
		}
		
	}
	return CDialog::OnCommand(wParam, lParam);
}
