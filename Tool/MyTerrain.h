#pragma once
#include "Obj.h"
class CMyTerrain : public CObj
{
public:
	CMyTerrain();
	virtual ~CMyTerrain();

public:
	HRESULT Initialize(void);
	int Update(void);
	void Late_Update(void);
	void Release(void);
	void Render();
	void Mini_Render(void);

public:
	vector<TILE*>&			Get_vecTile() { return m_vecTile; }
	void		Set_Render(bool _bRender) { m_bRender = _bRender; }
public:
	void		TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
private:
	void		Set_Ratio(D3DXMATRIX* pOut, const float& fRatioX, const float& fRatioY);
private:
	vector<TILE*>			m_vecTile;
	bool					m_bRender;
};

