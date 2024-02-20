#include "stdafx.h"
#include "Obj.h"
#include "ObjTool.h"

CObj::CObj()
	:m_pMainView(nullptr), m_fReverseX(1.f), m_iAlpha(255), m_bInstalled(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_vScroll, sizeof(VERTEX));


}

CObj::~CObj()
{
}