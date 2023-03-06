#pragma once
#include "afxwin.h"
#include "PathFind.h"

// CObjTool 대화 상자입니다.

class CObjTool : public CDialog
{
	DECLARE_DYNAMIC(CObjTool)

public:
	CObjTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnAddBtn();
	afx_msg void OnDeleteBtn();
	afx_msg void OnEditBtn();
	afx_msg void OnSaveBtn();
	afx_msg void OnLoadBtn();
	afx_msg void OnLayerSpinbar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFindEdit();
	afx_msg void OnDestroy();
	afx_msg void OnObjListBox();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
	void SetAll(CString strFindName);

public:
	map<CString, UNITDATA*>		m_mapUnitData;
	CStatic m_Obj_Picture;
	CListBox m_Obj_ListBox;

	CComboBox m_Type_Combo;
	CString m_strName;
	int m_iLayer;
	int m_iHp;
	int m_iAttack;
	CButton m_Collision_Check;

	CString m_strFindName;
	afx_msg void OnBnClickedObjPathfind();
	CPathFind   m_PathFind;
};