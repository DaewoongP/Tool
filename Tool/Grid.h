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
	// 0���� left��ġ, 1���� right ��ġ
	D3DXVECTOR2		m_vLinePoint[2];
};

