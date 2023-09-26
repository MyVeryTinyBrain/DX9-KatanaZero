#pragma once

class Bullet : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnTriggerEnter(Collider* collider);

	PUBLIC static Bullet* Create(const Vec2& pos, const Vec2& right, float speed, bool fromEnemy);

	PRIVATE float m_speed;
	PRIVATE bool m_fromEnemy;
	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_trigger;
	PRIVATE float m_elapsed;

	PRIVATE static wstring g_bulletName;
	PUBLIC static const wstring& BulletName();
};

