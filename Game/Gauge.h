#pragma once

class Gauge : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetPercent(float percent);
	PUBLIC void Show();
	PUBLIC void Hide();

	PRIVATE float m_percent;
	PRIVATE Sprite* m_sprBack;
	PRIVATE Sprite* m_sprFront;
	PRIVATE bool m_render;
};

