#pragma once

class CameraController;
class Background;
class Parallax;
class CameraBorderBox;

class EditorManager : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static EditorManager* GetInstance();

	PUBLIC void SetFocus(bool focus);
	PUBLIC bool IsFocus() const;

	PUBLIC void SaveAs(const wstring& path);
	PUBLIC void Open(const wstring& path);
	PUBLIC const wstring& GetCurrentPath() const;
	PUBLIC void New();

	PUBLIC Background* GetFrontBackground() const;
	PUBLIC Background* GetMiddleBackground() const;
	PUBLIC Background* GetBackBackground() const;
	PUBLIC Parallax* GetFrontParallax() const;
	PUBLIC Parallax* GetMiddleParallax() const;
	PUBLIC Parallax* GetBackParallax() const;
	PUBLIC CameraBorderBox* GetCameraBorderBox() const;

	PRIVATE static EditorManager* g_instance;
	PRIVATE bool m_focus;
	PRIVATE CameraController* m_camController;
	PRIVATE Background* m_background[3];
	PRIVATE Parallax* m_parallax[3];
	PRIVATE CameraBorderBox* m_cameraBorderBox;

	PRIVATE wstring m_currentPath;
};

