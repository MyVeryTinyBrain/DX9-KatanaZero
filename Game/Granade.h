#pragma once

class Granade : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetVelocity(const Vec2& velocity);

	PRIVATE SpriteRenderer* m_granadeRenderer;
	PRIVATE Sprite* m_sprGranade;
	PRIVATE SpriteRenderer* m_lightRenderer;
	PRIVATE Sprite* m_sprLight;
	PRIVATE SpriteRenderer* m_rangeRenderer;
	PRIVATE Sprite* m_sprRange;
	PRIVATE Body* m_body;
	PRIVATE CircleCollider* m_collider;

	PRIVATE Vec2 m_startVelocity;
	PRIVATE float m_elapsed;
	PRIVATE float m_activeElapsed;
	PRIVATE float m_rangeRadius;
	PRIVATE float m_trailCounter;

	PRIVATE bool m_readyToPlayWarningSound;
};

