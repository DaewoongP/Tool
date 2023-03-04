// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
{

}

CObjectTool::~CObjectTool()
{
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CONTROL, m_Tree);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectTool::OnObjectButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnLoadButton)
	ON_LBN_SELCHANGE(IDC_LIST2, &CObjectTool::OnListBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.


BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	vector<HTREEITEM> vecTree;
	for (size_t i = 0; i < 6; ++i)
		vecTree.push_back(HTREEITEM{});

	vecTree[0] = m_Tree.InsertItem(TEXT("Object"), 0, 0, TVI_ROOT, TVI_LAST);
	vecTree[1] = m_Tree.InsertItem(TEXT("Etc"), 0, 0, vecTree[0], TVI_LAST);
	vecTree[2] = m_Tree.InsertItem(TEXT("Fence"), 0, 0, vecTree[0], TVI_LAST);
	vecTree[3] = m_Tree.InsertItem(TEXT("Plant"), 0, 0, vecTree[0], TVI_LAST);
	vecTree[4] = m_Tree.InsertItem(TEXT("House"), 0, 0, vecTree[0], TVI_LAST);
	vecTree[5] = m_Tree.InsertItem(TEXT("Tree"), 0, 0, vecTree[0], TVI_LAST);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CObjectTool::OnObjectButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectTool::OnLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Texture\\Object\\");

	HTREEITEM item = m_Tree.GetSelectedItem();

	if (NULL == item)
		return;

	CString ItemStr = m_Tree.GetItemText(item);
	lstrcat(szPath, ItemStr);
	lstrcpy(m_szCurPath, szPath);
	wstring wstrPath = szPath;
	m_ListBox.ResetContent();
	
	int FileCnt = CFileInfo::DirFileCnt(szPath);

	//CFileFind : MFC에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind			Find;
	// 경로 상의 모든 파일을 찾아야 하기 때문에 경로 수정
	wstring		wstrFilePath = wstrPath + L"\\*.*";

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());
	while(bContinue)
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


void CObjectTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : 현재 선택된 목록의 인덱스를 반환
	int	iSelect = m_ListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	//GetText : 인덱스 값에 해당하는 문자열을 리스트 박스로부터 얻어옴
	m_ListBox.GetText(iSelect, strFindName);
	TCHAR	m_szCurFilePath[MAX_PATH] = L"";

	lstrcpy(m_szCurFilePath, m_szCurPath);
	lstrcat(m_szCurFilePath, L"\\");
	lstrcat(m_szCurFilePath, strFindName);
	auto	iter = m_MapPngImg.find(strFindName);
	
}

void CObjectTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
