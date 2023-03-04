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

	// GetDeviceCaps : ��ġ�� ���� ������ ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�

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

	// CreateDevice : �׷��� ī�带 ������ ��ü�� �����ϴ� �Լ�
	//CreateDevice(� �׷��� ī�带 ������ ���ΰ�, � ������ ���� ��ġ�� ������ ���ΰ�, ��ġ�� ����� ������ �ڵ�, ���� ���, ���ȯ��, �����Ǵ� �İ�ü �ּ�)

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}
	// ��������Ʈ �� ��ü

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// ��Ʈ �� ��ü

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.CharSet = HANGEUL_CHARSET;
	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	//  ���� �� ��ü

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
	m_pDevice->Clear(0,			// ��Ʈ�� ����
					nullptr,	// ��Ʒ �迭�� �ּ�(nullptr�� ��� ��ü ������ �ǹ�)
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
					D3DCOLOR_ARGB(255, 0, 0, 255), // ����� �ؽ�ó�� ����
					1.f, // ����(z����) ���� �ʱ�ȭ ��
					0); // ���ٽ� ���� �ʱ�ȭ ��

	m_pDevice->BeginScene();

	// ���� �׽�Ʈ�� ��ȿ�� ��Ȳ���� ���� ������ �����ϴ� �ɼ�
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pLine->Begin();
}

void CDevice::Render_End(HWND hWnd)
{
	m_pLine->End();
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

	// 1, 2�� ���� : D3DSWAPEFFECT_COPY�� �ۼ����� ���� �� nullptr
	// 3�� ���� : ���� ��ų ��� â�� �ڵ� �� 
	// 4�� ���� : D3DSWAPEFFECT_COPY�� �ۼ����� ���� �� nullptr
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

	//D3DSWAPEFFECT_DISCARD : ���� ü�� ���,
	//D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ����ϴ� ���,
	//D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���� ���,

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;

	// â ���� ��ü ȭ�� ����
	d3dpp.Windowed = true; // â ���
	
	// �ڵ������� ���� / ���ٽ� ���۸� ����� �����ϱ� ���ϸ� true
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ��üȭ�� ��ȯ �� ����Ǵ� ������
	// â ��� �ÿ��� ��ġ�� �ƴ� �ü���� ����� ������� ����
	// �׷��� ��ü ȭ�� ��� �� �׷��� ��ġ�� ����� ������� ����

	// ���� ������� ������� ���� �˾Ƽ� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// ������� �ÿ��� ���� ����

	// D3DPRESENT_INTERVAL_IMMEDIATE : ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ������ �������� �˾Ƽ� ����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
