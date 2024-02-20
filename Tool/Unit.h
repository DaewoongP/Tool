#pragma once
#include "Obj.h"
class CUnit :
	public CObj
{
public:
	CUnit();
	virtual ~CUnit();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Release(void) override;
	virtual void Render(void) override;
	virtual void Mini_Render(void) override;

public:
	UNITDATA				m_tUnitData;
};