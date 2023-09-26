#pragma once

class Enemy;

class EnemyDeadBody : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetDeadBody(Enemy* enemy);

	PRIVATE SpriteRenderer* m_renderer;
};

