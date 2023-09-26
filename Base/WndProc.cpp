#include "base_stdafx.h"
#include "WndProc.h"
#include "Input.h"
#include "Screen.h"
#include "GraphicDevice.h"

LRESULT CALLBACK WndProc::Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
			Input::GetInstance()->SetKeyDown((unsigned char)Key::LeftMouse);
			break;
		case WM_RBUTTONDOWN:
			Input::GetInstance()->SetKeyDown((unsigned char)Key::RightMouse);
			break;
		case WM_MBUTTONDOWN:
			Input::GetInstance()->SetKeyDown((unsigned char)Key::WhileMouse);
			break;
		case WM_LBUTTONUP:
			Input::GetInstance()->SetKeyUp((unsigned char)Key::LeftMouse);
			break;
		case WM_RBUTTONUP:
			Input::GetInstance()->SetKeyUp((unsigned char)Key::RightMouse);
			break;
		case WM_MBUTTONUP:
			Input::GetInstance()->SetKeyUp((unsigned char)Key::WhileMouse);
			break;
		case WM_MOUSEWHEEL:
			Input::GetInstance()->AddWheelDelta(HIWORD(wParam));
			break;
		case WM_KEYDOWN:
			Input::GetInstance()->SetKeyDown((unsigned char)wParam);
			break;
		case WM_KEYUP:
			Input::GetInstance()->SetKeyUp((unsigned char)wParam);
			break;
		case WM_SIZE:
			Screen::GetInstance()->UpdateSize(LOWORD(lParam), HIWORD(lParam));
			GraphicDevice::GetInstance()->Reset(LOWORD(lParam), HIWORD(lParam), Screen::GetHWND());
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}