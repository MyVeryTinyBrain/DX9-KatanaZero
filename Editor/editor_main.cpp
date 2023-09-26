#include "editor_stdafx.h"
#include "editor_main.h"
#include "EditorScene.h"

WinLoop* editor_main::winLoop = nullptr;

void editor_main::Initialize()
{
	winLoop = new WinLoop;
	SceneManager::ChangeScene(new EditorScene);
}

void editor_main::Release()
{
	SAFE_DELETE(winLoop);
}

void editor_main::ManualLoop()
{
	if (!GraphicDevice::GetInstance()->IsInitialized()) return;
	winLoop->ManualLoop();
}

void editor_main::TranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
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
			Input::GetInstance()->AddWheelDelta(HIWORD(pMsg->wParam));
			break;
		case WM_KEYDOWN:
			Input::GetInstance()->SetKeyDown((unsigned char)pMsg->wParam);
			break;
		case WM_KEYUP:
			Input::GetInstance()->SetKeyUp((unsigned char)pMsg->wParam);
			break;
		case WM_SIZE:
			//Screen::GetInstance()->UpdateSize(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
			//GraphicDevice::GetInstance()->Reset(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam), Screen::GetHWND());
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
}

bool editor_main::IsReady()
{
	return winLoop != nullptr;
}
