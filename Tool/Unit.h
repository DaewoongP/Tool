#pragma once
#include "Obj.h"
class CUnit :
	public CObj
{
public:
	CUnit();
	virtual ~CUnit();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Release(void) override;
	virtual void Render(void) override;
	virtual void Mini_Render(void) override;

public:
	UNITDATA				m_tUnitData;
};