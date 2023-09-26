#pragma once

class WinLoop;

class editor_main
{
	PUBLIC static void Initialize();
	PUBLIC static void Release();
	PUBLIC static void ManualLoop();
	PUBLIC static void TranslateMessage(MSG* pMsg);

	PUBLIC static bool IsReady();

	PRIVATE static WinLoop* winLoop;
};

