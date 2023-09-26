#include "base_stdafx.h"
#include "Screen.h"
#include "Resource.h"

#define INST (g_instance)
#define MAX_LOADSTRING 100

IMPLEMENT_SINGLETON(Screen)

IMPLEMENT_SINGLETON_CONSTRUCTOR(Screen)
{
	m_hWnd = 0;
	m_width = 0;
	m_height = 0;
}

IMPLEMENT_SINGLETON_DESTRUCTOR(Screen)
{
}

void Screen::Initialize(HINSTANCE hInstance, LONG width, LONG height)
{
	m_width = width;
	m_height = height;

	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BASE, szWindowClass, MAX_LOADSTRING);

	RECT rect = { 0, 0, Screen::GetWidth(), Screen::GetHeight() };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);
}

void Screen::Initialize(HWND hWnd, LONG width, LONG height)
{
	m_width = width;
	m_height = height;
	m_hWnd = hWnd;
}

void Screen::UpdateSize(LONG width, LONG height)
{
	m_width = width;
	m_height = height;
	UpdateMatrices();
}

void Screen::UpdateMatrices()
{
	float hw = float(m_width) * 0.5f;
	float hh = float(m_height) * 0.5f;
	m_cameraToScreen = Mat4(
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0, 
		hw, hh, 0,	1
	);
	m_screenToCamera = Mat4(
		1,		0,		0,		0,
		0,		1,		0,		0,
		0,		0,		1,		0,
		-hw,	-hh,	0,		1
	);
}

void Screen::SetSize(LONG width, LONG height)
{
	SetWindowPos(INST.m_hWnd, NULL, 0, 0, width, height, NULL);
	INST.m_width = width;
	INST.m_height = height;
	INST.UpdateMatrices();
}

HWND Screen::GetHWND()
{
	return INST.m_hWnd;
}

LONG Screen::GetWidth()
{
	return INST.m_width;
}

LONG Screen::GetHeight()
{
	return INST.m_height;
}

const Mat4& Screen::GetCameraToScreenMatrix()
{
	return INST.m_cameraToScreen;
}

const Mat4& Screen::GetScreenToCameraMatrix()
{
	return INST.m_screenToCamera;
}
