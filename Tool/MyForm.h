#pragma once
// CMyForm 폼 뷰입니다.
#include "MapTool.h"
#include "TileTool.h"
#include "ObjTool.h"
#include "afxwin.h"
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDetailBtnClicked();
	afx_msg void OnGridCheckClicked();
public:
	CMapTool			m_MapTool;
	CTileTool			m_TileTool;
	CObjTool			m_ObjTool;
	CButton				m_GirdRender;
};


