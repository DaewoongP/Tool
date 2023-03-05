#pragma once

typedef D3DXVECTOR3	  VERTEX;
typedef D3DXMATRIX    MATRIX;

#define		OBJ_NOEVENT		0
#define		OBJ_DEAD		1

#define		WINCX		800
#define		WINCY		600

#define		VK_MAX			0xff

#define		SAFE_DELETE(p)	if(p) { delete p; p = nullptr;}

#define		TILECX			94
#define		TILECY			48

#define		TILEX			45
#define		TILEY			30

#define		MIN_STR			64
#define		MAX_STR			256	
		

#define ERR_MSG(message)								\
::MessageBox(nullptr, message, L"error", MB_OK)

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static void		  Destroy_Instance();				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
void ClassName::Destroy_Instance()						\
{														\
	if(nullptr != m_pInstance)							\
	{													\
		delete m_pInstance;								\
		m_pInstance = nullptr;							\
	}													\
}

extern HWND		g_hWnd;

// ����� ���ÿ� �ʱ�ȭ �Ұ���, ���� ����� ���������� �ڷ���, ��������� ��ġ�ϴ� ���¿����� ��� ����

#define		DEVICE	CDevice::Get_Instance()
#define		TEXTURE	CTextureMgr::Get_Instance()
#define		KEY		CKeyMgr::Get_Instance()