#pragma once

class CameraController : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(LateUpdate);

	PUBLIC void Reset();

	PRIVATE void Pick();
	PRIVATE void Put();
	PRIVATE void Zoom();

	PRIVATE bool m_pick;
	PRIVATE Vec2 m_prevMousePos;
	PRIVATE float m_scale;
};

