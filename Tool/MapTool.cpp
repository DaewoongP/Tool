// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CMapTool 대화 상자입니다.

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


// CMapTool 메시지 처리기입니다.


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
		pPngImg->Load(Find.GetFilePath());	// 해당 경로의 이미지를 로드

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
