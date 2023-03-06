#pragma once
#include "MapTool.h"
#include "TileTool.h"
#include "ObjTool.h"
#include "afxwin.h"
#include "afxcmn.h"

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
	afx_msg void OnListBox();
	afx_msg void OnTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnTileXYBtn();
	afx_msg void OnBnClickedSaveAllBtn();
	afx_msg void OnBnClickedLoadAllBtn();
private:
	void Make_Tree(wstring& wstrPath, HTREEITEM parent);
	void Make_Path(wstring& wstrOut, HTREEITEM curTree);
	void DrawPictureControl(int _iSelect);
	void DrawMap();
	
public:
	CMapTool				m_MapTool;
	CTileTool				m_TileTool;
	CObjTool				m_ObjTool;
	CButton					m_GirdRender;
	CTreeCtrl				m_Tree;
	CListBox				m_ListBox;
	wstring					m_wstrCurDir;
	CStatic					m_Picture;
	CButton					m_DetailBtn;
	int						m_iTileX;
	int						m_iTileY;

	TCHAR					m_strMapName[MAX_STR] = L"";
	wstring					m_wstrPath = L"";
};