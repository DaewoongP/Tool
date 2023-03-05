// ObjTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjTool.h"
#include "afxdialogex.h"


// CObjTool ��ȭ �����Դϴ�.

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
END_MESSAGE_MAP()


// CObjTool �޽��� ó�����Դϴ�.

BOOL CObjTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Type_Combo.AddString(L"Creature");
	m_Type_Combo.AddString(L"NonCreature");
	m_Type_Combo.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;  
}

void CObjTool::OnAddBtn()
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// �̹� Ű�� �����ϴ� ��� ����
	auto& iter = m_mapUnitData.find(m_strName);
	if (iter != m_mapUnitData.end())
		return;

	// ���� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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


	// �ε��� ���� �ش��ϴ� ����Ʈ �ڽ��� ���ڿ��� ����
	m_Obj_ListBox.DeleteString(iSelect);

	UpdateData(FALSE);
}


void CObjTool::OnEditBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CObjTool::OnSaveBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CObjTool::OnLoadBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CObjTool::OnLayerSpinbar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	SetAll(m_strFindName);
	m_Obj_ListBox.SetCurSel(m_Obj_ListBox.FindString(0, m_strFindName));

	UpdateData(FALSE);
}


void CObjTool::OnObjListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CString		strFindName;

	//GetCurSel : ���� ���õ� ����� �ε����� ��ȯ
	int	iSelect = m_Obj_ListBox.GetCurSel();

	if (-1 == iSelect)
		return;
	m_Obj_ListBox.GetText(iSelect, strFindName);

	SetAll(strFindName);

	UpdateData(FALSE);
}
