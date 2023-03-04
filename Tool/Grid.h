#pragma once
class CGrid
{
public:
	CGrid();
	~CGrid();
public:
	static void Render_Grid();
	static void Draw_Line(VERTEX _vLeft, VERTEX _vRight, D3DCOLOR _Color);
private:
	// 0번에 left위치, 1번에 right 위치
	D3DXVECTOR2		m_vLinePoint[2];
};

