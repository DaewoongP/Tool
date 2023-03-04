#pragma once

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite() { return m_pSprite; }
	LPD3DXFONT				Get_Font() { return m_pFont; }
public:
	HRESULT			InitDevice();
	void			Render_Begin();
	void			Render_End(HWND hWnd = nullptr);
	void			Release(void);

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
};

