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
protected:
	CToolView*				m_pMainView;
	INFO					m_tInfo;
	VERTEX					m_vScroll;
};

