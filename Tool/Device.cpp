#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr)
	, m_pDevice(nullptr)
	, m_pSprite(nullptr)
	, m_pFont(nullptr)
	, m_pLine(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::InitDevice()
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// CreateDevice : 그래픽 카드를 제어할 객체를 생성하는 함수
	//CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보를 토대로 장치에 접근할 것인가, 장치를 사용할 윈도우 핸들, 동작 방식, 사용환경, 생성되는 컴객체 주소)

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}
	// 스프라이트 컴 객체

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// 폰트 컴 객체

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.CharSet = HANGEUL_CHARSET;
	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	//  라인 컴 객체

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		AfxMessageBox(L"D3DXCreateLine Failed");
		return E_FAIL;
	}

	m_pLine->SetWidth(2.f);
		
	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,			// 렉트의 개수
					nullptr,	// 렉틔 배열의 주소(nullptr인 경우 전체 영역을 의미)
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
					D3DCOLOR_ARGB(255, 0, 0, 255), // 백버퍼 텍스처의 색상
					1.f, // 깊이(z버퍼) 버퍼 초기화 값
					0); // 스텐실 버퍼 초기화 값

	m_pDevice->BeginScene();

	// 알파 테스트가 유효한 상황에서 알파 블랜딩을 지원하는 옵션
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pLine->Begin();
}

void CDevice::Render_End(HWND hWnd)
{
	m_pLine->End();
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

	// 1, 2번 인자 : D3DSWAPEFFECT_COPY로 작성하지 않은 한 nullptr
	// 3번 인자 : 적용 시킬 대상 창의 핸들 값 
	// 4번 인자 : D3DSWAPEFFECT_COPY로 작성하지 않은 한 nullptr
}

void CDevice::Release(void)
{
	Safe_Release(m_pLine);
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	//D3DSWAPEFFECT_DISCARD : 스왑 체인 방식,
	//D3DSWAPEFFECT_FLIP : 버퍼 하나를 뒤집으면서 사용하는 방식,
	//D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사 방식,

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;

	// 창 모드냐 전체 화면 모드냐
	d3dpp.Windowed = true; // 창 모드
	
	// 자동적으로 깊이 / 스텐실 버퍼를 만들고 관리하길 원하면 true
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면 전환 시 적용되는 변수들
	// 창 모드 시에는 장치가 아닌 운영체제가 모니터 재생률을 관리
	// 그러나 전체 화면 모드 시 그래픽 장치가 모니터 재생률을 관리

	// 현재 모니터의 재생률을 토대로 알아서 수행
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 재생률과 시연의 간격 설정

	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격으로 알아서 결정
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
