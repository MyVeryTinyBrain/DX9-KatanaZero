#pragma once

class ExplosiveBarrel : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void Explosion();

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Sprite* m_spr;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_trigger;

	PRIVATE float m_rangeRadius;
	PRIVATE bool m_readyToDamage;
	PRIVATE float m_damageWait;
	PRIVATE bool m_destroyNextFrame;
};

