#pragma once

class Screen
{
	DECLARE_SINGLETON(Screen)

	PUBLIC void Initialize(HINSTANCE hInstance, LONG width, LONG height);
	PUBLIC void Initialize(HWND hWnd, LONG width, LONG height);
	PUBLIC void UpdateSize(LONG width, LONG height);
	PRIVATE void UpdateMatrices();

	PUBLIC static void SetSize(LONG width, LONG height);

	PUBLIC static HWND GetHWND();
	PUBLIC static LONG GetWidth();
	PUBLIC static LONG GetHeight();
	PUBLIC static const Mat4& GetCameraToScreenMatrix();
	PUBLIC static const Mat4& GetScreenToCameraMatrix();

	PRIVATE HWND m_hWnd;
	PRIVATE LONG m_width;
	PRIVATE LONG m_height;
	PRIVATE Mat4 m_cameraToScreen;
	PRIVATE Mat4 m_screenToCamera;
};

