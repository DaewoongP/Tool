#pragma once
#include "ToolView.h"
class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	virtual HRESULT Initialize(void) PURE;
	virtual int Update(void) PURE;
	virtual void Late_Update(void) PURE;
	virtual void Release(void) PURE;
	virtual void Render(void) PURE;
	virtual void Mini_Render(void) PURE;

public:
	void		Set_View(CToolView* pMainView) { m_pMainView = pMainView; }
	void					Set_Render(bool _bRender) { m_bRender = _bRender; }
	void		Set_Pos(float fX, float fY)
	{
		m_tInfo.vPos.x = fX;
		m_tInfo.vPos.y = fY;
	}
	void		Set_Alpha(int _iAlpha)
	{
		m_iAlpha = _iAlpha;
	}
	void		Set_Installed() { m_bInstalled = true; }

	const bool&	Get_Installed() { return m_bInstalled; }
protected:
	CToolView*				m_pMainView;
	bool					m_bRender;
	INFO					m_tInfo;
	IMGPATH					m_tImgPath;
	VERTEX					m_vScroll;
	float					m_fReverseX;
	int						m_iAlpha;
	bool					m_bInstalled;
};