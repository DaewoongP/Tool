#pragma once
#include "ToolView.h"
class CMyMap
{
public:
	CMyMap();
	~CMyMap();
public:
	HRESULT Initialize(void);
	int Update(void);
	void Late_Update(void);
	void Release(void);
	void Render();
	void Mini_Render(void);

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, const float& fRatioX, const float& fRatioY);
	void		Set_Name(const TCHAR* path, const TCHAR* _Name);

private:
	CToolView*				m_pMainView;
	TCHAR					m_pMapName[MAX_STR];
};
