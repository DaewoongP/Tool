// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
	for_each(m_MapPngImg.begin(), m_MapPngImg.end(), [](auto& Pair)
	{
		Pair.second->Destroy();
		Safe_Delete(Pair.second);
	});
	m_MapPngImg.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_MapListBox);
	DDX_Control(pDX, IDC_MAP_PICTURE, m_MapPicture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnListBoxSelected)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	SelectListBox_Update();

	return TRUE; 
}

void CMapTool::SelectListBox_Update()
{
	wstring		wstrPath = L"../Texture/Stage/*.*";

	CFileFind	Find;
	BOOL		bContinue = Find.FindFile(wstrPath.c_str());
	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		CImage*		pPngImg = new CImage;
		pPngImg->Load(Find.GetFilePath());	// �ش� ����� �̹����� �ε�

		m_MapPngImg.insert({ Find.GetFileTitle(), pPngImg });
		m_MapListBox.AddString(Find.GetFileTitle());
	}
}


void CMapTool::OnListBoxSelected()
{
	CString		strFindName;
	int	iSelect = m_MapListBox.GetCurSel();
	m_MapListBox.GetText(iSelect, strFindName);
}
