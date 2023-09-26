#pragma once

class Enemy;

class PlayerPusher : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(BeforeFixedStep);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnTriggerEnter(Collider* collider);
	PRIVATE void OnDestroyStickedObject(Object* stickedEnemy);

	PUBLIC void SetDamage(bool enable);
	PUBLIC void StickToGameObject(Enemy* stickedEnemy);
	PUBLIC void SetWait(float wait);

	PRIVATE void DetachGameObject();
	PRIVATE void AttachGameObject(Enemy* stickedEnemy);

	PRIVATE Enemy* m_stickedEnemy;

	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_trigger;

	PRIVATE float m_wait;
	PRIVATE bool m_used;
	PRIVATE bool m_damage;
};

