#pragma once

class HitLineEffect : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static HitLineEffect* Create(const Vec2& position, const Vec2& right);

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE float m_speed;
	PRIVATE float m_elapsed;

	// 색상의 변화 주기입니다.
	// 이 값이 0.1이라면 0.2초 동안 색상이 두 번 바뀝니다.
	PRIVATE float m_waveform;
};

