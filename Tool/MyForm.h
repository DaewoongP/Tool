#pragma once
// CMyForm �� ���Դϴ�.
#include "MapTool.h"
#include "TileTool.h"
#include "ObjTool.h"
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDetailBtnClicked();
public:
	CFont				m_Font;
	CMapTool			m_MapTool;
	CTileTool			m_TileTool;
	CObjTool			m_ObjTool;
};


