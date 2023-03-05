#pragma once
#include "afxwin.h"


// CTileTool 대화 상자입니다.

class CTileTool : public CDialog
{
	DECLARE_DYNAMIC(CTileTool)

public:
	CTileTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTileTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnTileListBoxClicked();
	afx_msg void OnBnClickedTileSplit();
	afx_msg void OnSaveBtnClicked();
	afx_msg void OnDeleteBtnClicked();
	afx_msg void OnTileSelectBtnClicked();
	DECLARE_MESSAGE_MAP()
private:
	void FileFinder();
public:
	CSplitButton		m_TileSplit;
	CString				m_strTile;
	CButton				m_CollRadio[2];
	CListBox			m_TileListBox;
};
