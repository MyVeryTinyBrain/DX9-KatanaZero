#pragma once

class WndProc
{
public:
	static LRESULT CALLBACK Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};