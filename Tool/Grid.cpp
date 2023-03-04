#include "stdafx.h"
#include "Grid.h"


CGrid::CGrid()
{
	ZeroMemory(&m_vLinePoint, sizeof(D3DXVECTOR2) * 2);
}


CGrid::~CGrid()
{
}

void CGrid::Render_Grid()
{
	
}

void CGrid::Draw_Line(VERTEX _vLeft, VERTEX _vRight, D3DCOLOR _Color)
{
	D3DXVECTOR2 vLinePoint[2] = {};
	vLinePoint[0] = { _vLeft.x, _vLeft.y };
	vLinePoint[1] = { _vRight.x, _vRight.y };

	DEVICE->Get_Line()->SetWidth(2.f);
	DEVICE->Get_Line()->Draw(vLinePoint, 2, _Color);
}
