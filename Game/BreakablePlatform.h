#pragma once

class BreakablePlatform : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void BreakPlatform(const Vec2& velocity);
	PUBLIC bool IsShouldBreak() const;

	PRIVATE Sprite* m_spr;
	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_collider;
	PRIVATE bool m_destroyNextFrame;
};

