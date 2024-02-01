// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
	Clear_InfoList();
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_PATH_LIST1, &CPathFind::OnListBox)
	ON_BN_CLICKED(IDC_PATH_SAVEBTN, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_PATH_LOADBTN, &CPathFind::OnLoadData)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_PATH_CLEAR_BUTTON, &CPathFind::OnBnClickedPathClearButton)
END_MESSAGE_MAP()


// CPathFind �޽��� ó�����Դϴ�.


void CPathFind::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPathFind::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	wofstream		fout;
	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())
	{
		for (auto& ImgPath : m_PathInfoList)
		{
			fout << ImgPath->wstrObjKey << L"|" << ImgPath->wstrStateKey << L"|"
				<< ImgPath->iCount << L"|" << ImgPath->wstrPath << endl;
		}

		fout.close();
	}

	//�������� �⺻ ���α׷��� �����Ű�� �Լ�
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFind::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	wifstream		fin;
	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR		szObjKey[MAX_STR] = L"";
		TCHAR		szStateKey[MAX_STR] = L"";
		TCHAR		szCount[MAX_STR] = L"";
		TCHAR		szPath[MAX_PATH] = L"";

		wstring		wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_ListBox.AddString(wstrCombined.c_str());
			m_PathInfoList.push_back(new IMGPATH(szObjKey, szStateKey, szPath, stoi(szCount)));
		}

		fin.close();

	}

	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	m_ListBox.ResetContent();
	
	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathInfoList);
	}

	wstring	wstrCombined = L"";
	TCHAR	szBuf[MAX_STR] = L"";

	for (auto& ImgPath : m_PathInfoList)
	{
		// ������ �����ڵ� ���ڿ��� ��ȯ
		_itow_s(ImgPath->iCount, szBuf, 10);

		wstrCombined = ImgPath->wstrObjKey + L"|" + ImgPath->wstrStateKey + L"|" + szBuf + L"|" + ImgPath->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);
}

void CPathFind::OnBnClickedPathClearButton()
{
	UpdateData(TRUE);

	Clear_InfoList();
	m_ListBox.ResetContent();

	UpdateData(FALSE);
}

void CPathFind::Clear_InfoList()
{
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), CDeleteObj());
	m_PathInfoList.clear();
}
