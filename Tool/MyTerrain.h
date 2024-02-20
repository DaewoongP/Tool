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
	void					Set_TileCnt(int _X, int _Y) { m_iTileX = _X; m_iTileY = _Y; }
	void					Load_vecTile(vector<TILE*> _vecTile)
	{ 
		this->Release();
		m_vecTile = _vecTile;
	}
public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, const float& fRatioX, const float& fRatioY);
public:
	void		TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption = 1);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	vector<TILE*>			m_vecTile;
	CToolView*				m_pMainView;
	bool					m_bRender;
	int						m_iTileX;
	int						m_iTileY;
};

