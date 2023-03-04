#pragma once
#include "afxwin.h"
#include "MapTool.h"
// CMyForm 폼 뷰입니다.
#include "ObjectTool.h"
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
	afx_msg void OnObjectTool();

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnGridCheckBox();
	afx_msg void OnMapTool();
public:
	CFont				m_Font;
	CButton				m_GridCheck;
	CMapTool			m_MapTool;
	CObjectTool			m_ObjectTool;
};


