#include "stdafx.h"
#include "ExplosiveBottle.h"
#include "DamageZone.h"
#include "StaticEffect.h"
#include "PlayerCamera.h"

void ExplosiveBottle::Awake()
{
	ThrowObject::Awake();

	m_damageToEnemy = false;
    m_canCrashWithBullet = true;
	m_useBreakEffect = false;

	m_rangeRadius = 60;
}

void ExplosiveBottle::OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType)
{
	Vec2 center = GetTransform()->GetPosition();
	for (int i = 0; i < 10; ++i)
	{
		float angle = float(rand() % 360);
		float dist = fmodf(float(rand()), m_rangeRadius) + 0;
		auto effect = StaticEffect::Create(
			L"../Animation/effect/explosion.txt",
			center + Vec2::Direction(angle) * dist, true);
		effect->SetWait(float(rand() % 10) * 0.015f);
	}

	auto o = new GameObject(GetTransform()->GetPosition());
	o->GetTransform()->SetScale(Vec2::one() * m_rangeRadius * 2.0f);
	auto damage = o->AddComponent<DamageZone>();
	damage->SetDamage(true);
	damage->SetDamageToEnemy(true);
	damage->SetDamageToPlayer(true);
	damage->SetAdjustVelocity(Vec2::up() * 400);
	damage->SetExplosion(true);

	PlayerCamera::GetInstance()->Shake(Vec2::Direction(float(rand() % 360)), 0.2f, 600, 10);
}

wstring ExplosiveBottle::SpriteKey()
{
	return L"../Texture/object/used/throw/spr_explosive_vial_0.png";
}
