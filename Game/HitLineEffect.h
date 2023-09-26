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

	// ������ ��ȭ �ֱ��Դϴ�.
	// �� ���� 0.1�̶�� 0.2�� ���� ������ �� �� �ٲ�ϴ�.
	PRIVATE float m_waveform;
};

