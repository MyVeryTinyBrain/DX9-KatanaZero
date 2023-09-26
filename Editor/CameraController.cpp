#include "editor_stdafx.h"
#include "CameraController.h"
#include "EditorManager.h"

void CameraController::Awake()
{
	Reset();
}

void CameraController::LateUpdate()
{
	if (Input::GetKey(Key::Space) && Input::GetKeyDown(Key::LeftMouse)) Pick();
	if (!EditorManager::GetInstance()->IsFocus() || !Input::GetKey(Key::Space) || !Input::GetKey(Key::LeftMouse)) Put();

	if (m_pick)
	{
		Vec2 mousePos = CCamera2D::GetInstance()->GetMousePositionInScreen();
		Vec2 delta = mousePos - m_prevMousePos;
		delta.x *= -1;
		Vec2 camPos = CCamera2D::GetInstance()->GetPosition();
		CCamera2D::GetInstance()->SetPosition(camPos + delta * m_scale);
		m_prevMousePos = mousePos;
	}

	Zoom();
}

void CameraController::Reset()
{
	CCamera2D::GetInstance()->SetPosition(Vec2::zero());
	m_pick = false;
	m_scale = 1;
}

void CameraController::Pick()
{
	if (m_pick) return;
	m_pick = true;
	m_prevMousePos = CCamera2D::GetInstance()->GetMousePositionInScreen();
}

void CameraController::Put()
{
	if (!m_pick) return;
	m_pick = false;
}

void CameraController::Zoom()
{
	float scrollDelta = Input::GetMouseWheelDelta();
	if (scrollDelta > 0) m_scale -= 0.1f;
	if (scrollDelta < 0) m_scale += 0.1f;
	if (m_scale < 0.1f) m_scale = 0.1f;
	CCamera2D::GetInstance()->SetScale(Vec2::one() * m_scale);
}
