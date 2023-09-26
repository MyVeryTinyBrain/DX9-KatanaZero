#include "base_stdafx.h"
#include "GraphicDevice.h"
#include "SpriteDevice.h"
#include "TextDevice.h"
#include "LineDevice.h"

#define INST (g_instance)

IMPLEMENT_SINGLETON(GraphicDevice)

IMPLEMENT_SINGLETON_CONSTRUCTOR(GraphicDevice)
{
	m_initialized = false;
	m_backgroundColor = Color(0, 0, 1, 1);
	m_SDK = NULL;
	m_device = NULL;
}

IMPLEMENT_SINGLETON_DESTRUCTOR(GraphicDevice)
{

}

LPDIRECT3D9 GraphicDevice::CreateSDK()
{
	return Direct3DCreate9(D3D_SDK_VERSION);
}

bool GraphicDevice::CreatePresentParameters(UINT width, UINT height, HWND hWnd, D3DPRESENT_PARAMETERS* pParam)
{
	D3DPRESENT_PARAMETERS pp{};
	pp.BackBufferWidth = width;
	pp.BackBufferHeight = height;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.BackBufferCount = 1;
	pp.MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE;	// ���ø��� ���� �ʽ��ϴ�.
	pp.MultiSampleQuality = 0;
	pp.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;			// ����� �ּ� ��ü ����� ����մϴ�.
	pp.hDeviceWindow = hWnd;
	pp.Windowed = true;												// â ��� �����Դϴ�.
	pp.EnableAutoDepthStencil = true;								// ���� ���ۿ� ���ٽ��� ����մϴ�.
	pp.AutoDepthStencilFormat = D3DFORMAT::D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ��üȭ�鿡���� ���ΰ�ħ �󵵸� �����մϴ�.
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// ����� ��ȯ�� ��� �ϵ��� �մϴ�

	*pParam = pp;
	return true;
}

LPDIRECT3DDEVICE9 GraphicDevice::CreateDevice(LPDIRECT3D9 SDK, UINT width, UINT height, HWND hWnd, bool windowed)
{
	HRESULT hr = S_OK;

	// ù ��° �ϵ������ ����� �����ɴϴ�.
	// D3DADAPTER_DEFAULT: ù ��° �׷���ī���Դϴ�.
	D3DCAPS9 caps{};
	if (FAILED(hr = SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE::D3DDEVTYPE_HAL, &caps)))
		return NULL;

	// ��ġ ������ �����ϴ� �÷��׸� �����մϴ�.
	DWORD vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// ��Ƽ�����带 ����մϴ�.
	vp |= D3DCREATE_MULTITHREADED;

	// ��ġ ���� �ɼ��� �����մϴ�.
	D3DPRESENT_PARAMETERS pp{};
	if (!CreatePresentParameters(width, height, hWnd, &pp))
		return NULL;

	// â��� �����Դϴ�.
	pp.Windowed = windowed;

	// ù ��° �׷���ī�带 �����ϴ� ��ġ�� �����մϴ�.
	LPDIRECT3DDEVICE9 device;
	if (FAILED(hr = SDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE::D3DDEVTYPE_HAL, hWnd, vp, &pp, &device)))
		return NULL;
	return device;
}

bool GraphicDevice::CreateInterafaces()
{
	if (!m_device) return false;

	m_spriteDevice = SpriteDevice::Create(m_device);
	if (!m_spriteDevice) goto ERROR_POINT;

	m_lineDevice = LineDevice::Create(m_device);
	if (!m_lineDevice) goto ERROR_POINT;

	m_textDevice = TextDevice::Create(m_spriteDevice);
	if (!m_textDevice) goto ERROR_POINT;

	return true;

ERROR_POINT:
	ReleaseInterfaces();
	return false;
}

bool GraphicDevice::ReleaseInterfaces()
{
	SAFE_DELETE(m_spriteDevice);
	SAFE_DELETE(m_lineDevice);
	SAFE_DELETE(m_textDevice);
	return true;
}

bool GraphicDevice::Initialize(UINT width, UINT height, HWND hWnd, bool windowed)
{
	m_SDK = CreateSDK();
	if (!m_SDK) return false;

	m_device = CreateDevice(m_SDK, width, height, hWnd, windowed);
	if (!m_device) return false;

	if (!CreateInterafaces())
		return false;

	m_initialized = true;
	m_rendering = false;
	m_windowed = windowed;

	return true;
}

bool GraphicDevice::Reset(UINT width, UINT height, HWND hWnd)
{
	if (!m_device) return false;
	if (width * height == 0) return false;

	if (!ReleaseInterfaces())
		return false;

	D3DPRESENT_PARAMETERS pp{};
	if (!CreatePresentParameters(width, height, hWnd, &pp))
		return false;

	pp.Windowed = m_windowed;
	
	if (FAILED(m_device->Reset(&pp)))
		return false;

	if (!CreateInterafaces())
		return false;

	return true;
}

void GraphicDevice::Release()
{
	ReleaseInterfaces();
	
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_SDK);
}

bool GraphicDevice::BeginRender()
{
	while (m_rendering);
	if (FAILED(m_device->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		m_backgroundColor,
		1.0f,
		0
	)))
		return false;
	if (FAILED(m_device->BeginScene()))
		return false;
	if (FAILED(m_spriteDevice->BeginRender()))
		return false;

	// �⺻ ���ø��� D3DTEXF_NONE �Դϴ�.
	// ��Ʈ�� ������ �ʵ��� POINT ���ø��� ����մϴ�.
	GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	// �⺻���� ����� ���� ����Դϴ�.
	GraphicDevice::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GraphicDevice::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GraphicDevice::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GraphicDevice::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_rendering = true;
	return true;
}

bool GraphicDevice::EndRender(HWND hWnd)
{
	if (FAILED(m_spriteDevice->EndRender()))
		return false;
	if (FAILED(m_device->EndScene()))
		return false;
	// hDestWindowOverride: NULL�� ����ϸ� ��ġ�� �����ߴ� ������(hWndDeviceWindow)�� �׸��ϴ�.
	//						NULL�� �ƴ϶�� �ش� �����쿡 �׸��ϴ�.
	// 1, 2, 4��° �Ķ����: SwapEffect�� D3DSWAPEFFECT_COPY�� �ƴ϶�� ��� NULL�� ä���� �մϴ�.
	if (FAILED(m_device->Present(NULL, NULL, hWnd, NULL)))
		return false;
	m_rendering = false;
	return true;
}

void GraphicDevice::SetBackgroundColor(const Color& color)
{
	INST.m_backgroundColor = color;
}

const Color& GraphicDevice::GetBackgroundColor()
{
	return INST.m_backgroundColor;
}

LPDIRECT3DDEVICE9 GraphicDevice::GetDevice()
{
	return INST.m_device;
}

SpriteDevice* GraphicDevice::GetSpriteDevice()
{
	return INST.m_spriteDevice;
}

TextDevice* GraphicDevice::GetTextDevice()
{
	return INST.m_textDevice;
}

LineDevice* GraphicDevice::GetLineDevice()
{
	return INST.m_lineDevice;
}

bool GraphicDevice::IsInitialized() const
{
	return m_initialized;
}
