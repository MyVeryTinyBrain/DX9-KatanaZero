#pragma once

class DamageZone : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnTriggerEnter(Collider* collider);

	PUBLIC void UseBoxTrigger();
	PUBLIC void UseCircleTrigger();

	PUBLIC void SetAdjustVelocity(const Vec2& velocity);
	PUBLIC void SetPushPower(float power);
	PUBLIC void SetWait(float wait);
	PUBLIC void SetDamage(bool enable);
	PUBLIC void SetDamageToPlayer(bool enable);
	PUBLIC void SetDamageToEnemy(bool enable);
	PUBLIC void SetExplosion(bool enable);
	PUBLIC bool IsExplosion() const;
	PUBLIC void SetCanBlock(bool enable);
	PUBLIC bool IsCanBlock() const;

	PRIVATE Body* m_body;
	PRIVATE Collider* m_currentTrigger;
	PRIVATE BoxCollider* m_boxTrigger;
	PRIVATE CircleCollider* m_circleTrigger;

	PRIVATE Vec2 m_adjustVelocity;
	PRIVATE float m_pushPower;
	PRIVATE float m_wait;
	PRIVATE bool m_used;
	PRIVATE bool m_damage;
	PRIVATE bool m_damageToEnemy;
	PRIVATE bool m_damageToPlayer;
	PRIVATE bool m_isExplosion;
	PRIVATE bool m_canBlock;

	PRIVATE bool m_playedExplosionSound;
};

