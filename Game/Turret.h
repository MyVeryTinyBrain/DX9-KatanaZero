#pragma once

class Gauge;

class Turret : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC bool Damage();
	PUBLIC Transform* GetHeadTransform() const;

	PRIVATE Sprite* m_sprHead;
	PRIVATE Sprite* m_sprBody;
	PRIVATE SpriteRenderer* m_headRenderer;
	PRIVATE SpriteRenderer* m_bodyRenderer;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_trigger;
	PRIVATE bool m_damaged;

	PRIVATE float m_shootAgainDelay;
	PRIVATE float m_shootAgainCounter;
	PRIVATE bool m_prevDetect;

	PRIVATE int m_shootCount;
	PRIVATE float m_firstShootDelay;
	PRIVATE float m_shootDelay;
	PRIVATE float m_shootCounter;

	PRIVATE Gauge* m_gauge;
};

