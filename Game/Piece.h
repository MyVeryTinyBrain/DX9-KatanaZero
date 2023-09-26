#pragma once

class Piece : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetSprite(const wstring& key);
	PUBLIC void SetVelocity(const Vec2& velocity);
	PUBLIC void SetPower(const Vec2& direction, float power, float angleRange);
	PUBLIC void SetTorque(float torque);
	PUBLIC Body* GetBody() const;
	PUBLIC void SetColliderSize(const Vec2& size);

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_collider;
	PRIVATE float m_elapsed;
};

