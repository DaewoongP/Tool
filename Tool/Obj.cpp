#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	:m_pMainView(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_vScroll, sizeof(VERTEX));
}


CObj::~CObj()
{
}
