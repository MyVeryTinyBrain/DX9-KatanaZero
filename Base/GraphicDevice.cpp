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
	pp.MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE;	// 샘플링을 하지 않습니다.
	pp.MultiSampleQuality = 0;
	pp.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;			// 백버퍼 주소 교체 방식을 사용합니다.
	pp.hDeviceWindow = hWnd;
	pp.Windowed = true;												// 창 모드 설정입니다.
	pp.EnableAutoDepthStencil = true;								// 깊이 버퍼와 스텐실을 사용합니다.
	pp.AutoDepthStencilFormat = D3DFORMAT::D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// 전체화면에서의 새로고침 빈도를 설정합니다.
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// 백버퍼 교환을 즉시 하도록 합니다

	*pParam = pp;
	return true;
}

LPDIRECT3DDEVICE9 GraphicDevice::CreateDevice(LPDIRECT3D9 SDK, UINT width, UINT height, HWND hWnd, bool windowed)
{
	HRESULT hr = S_OK;

	// 첫 번째 하드웨어의 기능을 가져옵니다.
	// D3DADAPTER_DEFAULT: 첫 번째 그래픽카드입니다.
	D3DCAPS9 caps{};
	if (FAILED(hr = SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE::D3DDEVTYPE_HAL, &caps)))
		return NULL;

	// 장치 동작을 제어하는 플래그를 설정합니다.
	DWORD vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// 멀티스레드를 사용합니다.
	vp |= D3DCREATE_MULTITHREADED;

	// 장치 생성 옵션을 설정합니다.
	D3DPRESENT_PARAMETERS pp{};
	if (!CreatePresentParameters(width, height, hWnd, &pp))
		return NULL;

	// 창모드 설정입니다.
	pp.Windowed = windowed;

	// 첫 번째 그래픽카드를 제어하는 장치를 생성합니다.
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

	// 기본 샘플링은 D3DTEXF_NONE 입니다.
	// 도트가 깨지지 않도록 POINT 샘플링을 사용합니다.
	GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	// 기본으로 적용된 블렌딩 모드입니다.
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
	// hDestWindowOverride: NULL을 사용하면 장치에 지정했던 윈도우(hWndDeviceWindow)에 그립니다.
	//						NULL이 아니라면 해당 윈도우에 그립니다.
	// 1, 2, 4번째 파라미터: SwapEffect가 D3DSWAPEFFECT_COPY가 아니라면 모두 NULL로 채워야 합니다.
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
