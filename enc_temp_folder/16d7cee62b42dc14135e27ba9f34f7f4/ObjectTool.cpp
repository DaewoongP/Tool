// ObjectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CObjectTool ��ȭ �����Դϴ�.

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


// CObjectTool �޽��� ó�����Դϴ�.


BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CObjectTool::OnObjectButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CObjectTool::OnLoadButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	//CFileFind : MFC���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind			Find;
	// ��� ���� ��� ������ ã�ƾ� �ϱ� ������ ��� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : ���� ���õ� ����� �ε����� ��ȯ
	int	iSelect = m_ListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	//GetText : �ε��� ���� �ش��ϴ� ���ڿ��� ����Ʈ �ڽ��κ��� ����
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
