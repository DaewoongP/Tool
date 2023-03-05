#pragma once
#include "ToolView.h"

class CMyTerrain
{
public:
	CMyTerrain();
	~CMyTerrain();

public:
	HRESULT Initialize(void);
	int Update(void);
	void Late_Update(void);
	void Release(void);
	void Render();
	void Mini_Render(void);

public:
	vector<TILE*>&			Get_vecTile() { return m_vecTile; }
	void					Set_Render(bool _bRender) { m_bRender = _bRender; }
public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, const float& fRatioX, const float& fRatioY);
public:
	void		TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	vector<TILE*>			m_vecTile;
	CToolView*				m_pMainView;
	bool					m_bRender;
};

