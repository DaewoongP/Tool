// ObjTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjTool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "afxdialogex.h"


// CObjTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjTool, CDialog)

CObjTool::CObjTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJTOOL, pParent)
	, m_strName(_T(""))
	, m_iLayer(0)
	, m_iHp(0)
	, m_iAttack(0)
	, m_strFindName(_T(""))
{

}

CObjTool::~CObjTool()
{
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_TYPE_COMBO, m_Type_Combo);
	DDX_Control(pDX, IDC_OBJ_COLL_CHECK, m_Collision_Check);
	DDX_Control(pDX, IDC_OBJ_PICTURE, m_Obj_Picture);
	DDX_Control(pDX, IDC_OBJ_OBJLIST, m_Obj_ListBox);
	DDX_Text(pDX, IDC_OBJ_NAME_EDIT, m_strName);
	DDX_Text(pDX, IDC_OBJ_LAYER_EDIT, m_iLayer);
	DDX_Text(pDX, IDC_OBJ_HP_EDIT, m_iHp);
	DDX_Text(pDX, IDC_OBJ_ATTACK_EDIT, m_iAttack);
	DDX_Text(pDX, IDC_OBJ_FIND_EDIT, m_strFindName);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)


	ON_BN_CLICKED(IDC_OBJ_ADDBTN, &CObjTool::OnAddBtn)
	ON_BN_CLICKED(IDC_OBJ_DELETEBTN, &CObjTool::OnDeleteBtn)
	ON_BN_CLICKED(IDC_OBJ_EDITBTN, &CObjTool::OnEditBtn)
	ON_BN_CLICKED(IDC_OBJ_SAVEBTN, &CObjTool::OnSaveBtn)
	ON_BN_CLICKED(IDC_OBJ_LOADBTN, &CObjTool::OnLoadBtn)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OBJ_LAYER_SPIN, &CObjTool::OnLayerSpinbar)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_OBJ_FIND_EDIT, &CObjTool::OnFindEdit)
	ON_LBN_SELCHANGE(IDC_OBJ_OBJLIST, &CObjTool::OnObjListBox)
	ON_BN_CLICKED(IDOK, &CObjTool::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OBJ_PATHFIND, &CObjTool::OnBnClickedObjPathfind)
END_MESSAGE_MAP()


// CObjTool 메시지 처리기입니다.

BOOL CObjTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Type_Combo.AddString(L"Creature");
	m_Type_Combo.AddString(L"NonCreature");
	m_Type_Combo.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;
}

void CObjTool::OnAddBtn()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 이미 키가 존재하는 경우 종료
	auto& iter = m_mapUnitData.find(m_strName);
	if (iter != m_mapUnitData.end())
		return;

	// 유닛 생성
	UNITDATA* pUnit = new UNITDATA;

	pUnit->iType = m_Type_Combo.GetCurSel();
	pUnit->iLayer = m_iLayer;
	pUnit->iLayer = min(pUnit->iLayer, 10);
	pUnit->iLayer = max(pUnit->iLayer, 0);

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iHp = m_iHp;
	pUnit->bCollision = m_Collision_Check.GetCheck();

	m_mapUnitData.insert({ pUnit->strName , pUnit });

	m_Obj_ListBox.AddString(pUnit->strName);
	UpdateData(FALSE);
}

void CObjTool::OnDeleteBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString		strFindName;

	int	iSelect = m_Obj_ListBox.GetCurSel();
	if (LB_ERR == iSelect)
		return;

	m_Obj_ListBox.GetText(iSelect, strFindName);

	auto&	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);


	// 인덱스 값에 해당하는 리스트 박스의 문자열을 삭제
	m_Obj_ListBox.DeleteString(iSelect);

	UpdateData(FALSE);
}


void CObjTool::OnEditBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CObjTool::OnSaveBtn()
{
	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data\\Object\\Obj1.dat");

	// GetPathName : 선택된 경로를 반환

	const TCHAR*	pGetPath = szPath;

	HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
 		return;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;

	for (auto& MyPair : m_mapUnitData)
	{
		// key값 저장
		dwStrByte = sizeof(TCHAR) * (MyPair.first.GetLength() + 1);
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);

		// value 값 저장
		WriteFile(hFile, &(MyPair.second->iType), sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(MyPair.second->iLayer), sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(MyPair.second->iAttack), sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(MyPair.second->iHp), sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(MyPair.second->bCollision), sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}


void CObjTool::OnLoadBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	TCHAR	szPath[MAX_PATH] = L"";

	// GetCurrentDirectory : 현재 프로젝트가 있는 디렉토리 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data\\Object\\Obj1.dat");


	for (auto& MyPair : m_mapUnitData)
		delete MyPair.second;
	m_mapUnitData.clear();
	m_Obj_ListBox.ResetContent();

	CString		str = szPath;
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

	while (true)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		TCHAR*		pName = new TCHAR[dwStrByte];
		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

		// value
		ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
		{
			delete[]pName;
			pName = nullptr;

			break;
		}

		UNITDATA*	pUnit = new UNITDATA;
		pUnit->strName = pName;

		delete[]pName;
		pName = nullptr;

		pUnit->iType = tData.iType;
		pUnit->iLayer = tData.iLayer;
		pUnit->iAttack = tData.iAttack;
		pUnit->iHp = tData.iHp;
		pUnit->bCollision = tData.bCollision;

		m_mapUnitData.insert({ pUnit->strName, pUnit });
		m_Obj_ListBox.AddString(pUnit->strName);

	}

	CloseHandle(hFile);
}


void CObjTool::OnLayerSpinbar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_iLayer;
	}
	else
		--m_iLayer;

	SetDlgItemInt(IDC_OBJ_LAYER_SPIN, m_iLayer);

	m_iLayer = min(m_iLayer, 10);
	m_iLayer = max(m_iLayer, 0);
	*pResult = 0;

	UpdateData(FALSE);
}

void CObjTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), [](auto& Pair)
	{
		Safe_Delete(Pair.second);
	});
	m_mapUnitData.clear();
}

void CObjTool::SetAll(CString strFindName)
{
	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	CString str;

	m_Type_Combo.SetCurSel(iter->second->iType);
	m_strName = iter->second->strName;
	m_iLayer = iter->second->iLayer;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;
	m_Collision_Check.SetCheck(iter->second->bCollision);
}


void CObjTool::OnFindEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	SetAll(m_strFindName);
	m_Obj_ListBox.SetCurSel(m_Obj_ListBox.FindString(0, m_strFindName));

	UpdateData(FALSE);
}


void CObjTool::OnObjListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString		strFindName;

	//GetCurSel : 현재 선택된 목록의 인덱스를 반환
	int	iSelect = m_Obj_ListBox.GetCurSel();

	if (-1 == iSelect)
		return;
	m_Obj_ListBox.GetText(iSelect, strFindName);

	SetAll(strFindName);

	UpdateData(FALSE);
}


void CObjTool::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	pToolView->m_ePickMod = PICK_OBJ;
}

void CObjTool::OnBnClickedObjPathfind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);

	m_PathFind.ShowWindow(SW_SHOW);
}
