#include "stdafx.h"
#include "DamageZone.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "DebugDraw.h"
#include "PlayerController.h"
#include "Enemy.h"
#include "TimeController.h"
#include "PlayerCamera.h"
#include "ExplosiveBarrel.h"
#include "SoundMgr.h"

void DamageZone::Awake()
{
	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Dynamic);
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);

	m_boxTrigger = AddComponent<BoxCollider>();
	m_boxTrigger->SetTriggerMode(true);
	m_boxTrigger->DrawDebug = DRAW_TRIGGER;
	m_boxTrigger->OnTriggerEnter += Function<void, Collider*>(this, &DamageZone::OnTriggerEnter);
	m_boxTrigger->SetEnable(false);

	m_circleTrigger = AddComponent<CircleCollider>();
	m_circleTrigger->SetTriggerMode(true);
	m_circleTrigger->DrawDebug = DRAW_TRIGGER;
	m_circleTrigger->OnTriggerEnter += Function<void, Collider*>(this, &DamageZone::OnTriggerEnter);
	m_circleTrigger->SetEnable(false);

	UseCircleTrigger();

	m_body->SetVelocity(Vec2(0.1f, 0.1f));
	m_pushPower = 400;
	m_wait = 0.0f;
	m_used = false;
	m_damageToPlayer = true;

	m_canBlock = false;

	m_playedExplosionSound = false;
}

void DamageZone::FixedUpdate()
{
	if (m_wait <= 0 && m_isExplosion && !m_playedExplosionSound)
	{
		m_playedExplosionSound = true;
		int soundCase = rand() % 3;
		switch (soundCase)
		{
			case 0:
				CSoundMgr::Get_Instance()->PlaySound(L"sound_explosion1.wav", CSoundMgr::CHANNELID::EXPLOSION);
				break;
			case 1:
				CSoundMgr::Get_Instance()->PlaySound(L"sound_explosion2.wav", CSoundMgr::CHANNELID::EXPLOSION);
				break;
			case 2:
				CSoundMgr::Get_Instance()->PlaySound(L"sound_explosion3.wav", CSoundMgr::CHANNELID::EXPLOSION);
				break;
		}
	}
	else if (m_wait > 0)
	{
		m_wait -= Time::GetFixedDeltaTime();
		return;
	}
	if (!m_used)
	{
		m_currentTrigger->SetEnable(true);
		m_used = true;
	}
	else
	{
		GetGameObject()->Destroy();
	}
}

void DamageZone::OnDestroy()
{
	m_boxTrigger->OnTriggerEnter -= Function<void, Collider*>(this, &DamageZone::OnTriggerEnter);
	m_circleTrigger->OnTriggerEnter -= Function<void, Collider*>(this, &DamageZone::OnTriggerEnter);
}

void DamageZone::OnTriggerEnter(Collider* collider)
{
	Body* body = collider->GetBody();
	if (m_damageToPlayer && body->GetGameObject()->GetTag() == TAG_PLAYER)
	{
		PlayerController* player = body->GetComponent<PlayerController>();
		if (!player) return;
		if (player->IsRolling()) return;

		Vec2 dir = (player->GetTransform()->GetPosition() - GetTransform()->GetPosition()).normalized();

		if (!m_damage)
			player->HardKnockback(dir * m_pushPower + m_adjustVelocity, this);
		else
			player->Damage(dir * m_pushPower + m_adjustVelocity, this);

		TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
		PlayerCamera::GetInstance()->Shake(dir, 0.2f, 600, 10);

		//GetGameObject()->Destroy();
	}
	else if (m_damageToEnemy && body->GetGameObject()->GetTag() == TAG_ENEMY)
	{
		Enemy* enemy = body->GetComponent<Enemy>();
		if (!enemy) return;
		if (enemy->IsHurt()) return;

		Vec2 dir = (enemy->GetTransform()->GetPosition() - GetTransform()->GetPosition()).normalized();

		bool dead = enemy->Damage(dir * m_pushPower + m_adjustVelocity, this, m_canBlock);

		//GetGameObject()->Destroy();
	}
	else if (m_isExplosion && body->GetGameObject()->GetTag() == TAG_EXPLOSIVE_BARREL)
	{
		ExplosiveBarrel* barrel = body->GetComponent<ExplosiveBarrel>();
		barrel->Explosion();
	}
}

void DamageZone::UseBoxTrigger()
{
	m_currentTrigger = m_boxTrigger;
}

void DamageZone::UseCircleTrigger()
{
	m_currentTrigger = m_circleTrigger;
}

void DamageZone::SetAdjustVelocity(const Vec2& velocity)
{
	m_adjustVelocity = velocity;
}

void DamageZone::SetPushPower(float power)
{
	m_pushPower = power;
}

void DamageZone::SetWait(float wait)
{
	m_wait = wait;
}

void DamageZone::SetDamage(bool enable)
{
	m_damage = enable;
}

void DamageZone::SetDamageToPlayer(bool enable)
{
	m_damageToPlayer = enable;
}

void DamageZone::SetDamageToEnemy(bool enable)
{
	m_damageToEnemy = enable;
}

void DamageZone::SetExplosion(bool enable)
{
	m_isExplosion = enable;
}

bool DamageZone::IsExplosion() const
{
	return m_isExplosion;
}

void DamageZone::SetCanBlock(bool enable)
{
	m_canBlock = enable;
}

bool DamageZone::IsCanBlock() const
{
	return m_canBlock;
}
