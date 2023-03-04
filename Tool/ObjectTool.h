#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CObjectTool 대화 상자입니다.

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnObjectButton();
	afx_msg void OnLoadButton();
	afx_msg void OnListBox();

public:
	CTreeCtrl m_Tree;
	CListBox m_ListBox;
	TCHAR	m_szCurPath[MAX_PATH];
	map<CString, CImage*>		m_MapPngImg;
	afx_msg void OnDestroy();
	CStatic m_Picture;
};
