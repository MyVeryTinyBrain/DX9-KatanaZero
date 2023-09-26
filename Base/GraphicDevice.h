#pragma once

class SpriteDevice;
class TextDevice;
class LineDevice;

class GraphicDevice
{
	DECLARE_SINGLETON(GraphicDevice)

	PRIVATE static LPDIRECT3D9 CreateSDK();
	PRIVATE static bool CreatePresentParameters(UINT width, UINT height, HWND hWnd, D3DPRESENT_PARAMETERS* pParam);
	PRIVATE static LPDIRECT3DDEVICE9 CreateDevice(LPDIRECT3D9 SDK, UINT width, UINT height, HWND hWnd, bool windowed);

	PRIVATE bool CreateInterafaces();
	PRIVATE bool ReleaseInterfaces();

	PUBLIC bool Initialize(UINT width, UINT height, HWND hWnd, bool windowed);
	PUBLIC bool Reset(UINT width, UINT height, HWND hWnd);
	PUBLIC void Release();

	PUBLIC bool BeginRender();
	PUBLIC bool EndRender(HWND hWnd = NULL);

	PUBLIC static void SetBackgroundColor(const Color& color);
	PUBLIC static const Color& GetBackgroundColor();
	PUBLIC static LPDIRECT3DDEVICE9 GetDevice();
	PUBLIC static SpriteDevice* GetSpriteDevice();
	PUBLIC static TextDevice* GetTextDevice();
	PUBLIC static LineDevice* GetLineDevice();

	PUBLIC bool IsInitialized() const;

	PRIVATE bool m_initialized;
	PRIVATE bool m_rendering;
	PRIVATE Color m_backgroundColor;
	PRIVATE LPDIRECT3D9 m_SDK;
	PRIVATE LPDIRECT3DDEVICE9 m_device;
	PRIVATE SpriteDevice* m_spriteDevice;
	PRIVATE TextDevice* m_textDevice;
	PRIVATE LineDevice* m_lineDevice;

	PRIVATE bool m_windowed;
};

