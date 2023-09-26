#include "stdafx.h"
#include "PlayerPusher.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "DebugDraw.h"
#include "PlayerController.h"
#include "Enemy.h"
#include "SoundMgr.h"

void PlayerPusher::Awake()
{
	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Dynamic);
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);
	m_body->GetGameObject()->SetTag(TAG_DAMAGABLE_TO_PLAYER);

	GameObject* triggerObject = GameObject::CreateChild(GetGameObject());
	m_trigger = triggerObject->AddComponent<BoxCollider>();
	m_trigger->SetWidth(1);
	m_trigger->SetHeight(1);
	m_trigger->SetTriggerMode(true);
	m_trigger->DrawDebug = DRAW_TRIGGER;
	m_trigger->OnTriggerEnter += Function<void, Collider*>(this, &PlayerPusher::OnTriggerEnter);
	m_trigger->SetEnable(false);

	m_body->SetVelocity(Vec2(0.1f, 0));

	m_wait = 0.05f;
	m_used = false;
}

void PlayerPusher::BeforeFixedStep()
{
	if (m_stickedEnemy && (m_stickedEnemy->IsHurt() || m_stickedEnemy->IsGroggy()))
	{
		GetGameObject()->Destroy();
	}
}

void PlayerPusher::FixedUpdate()
{
	if (m_wait > 0)
	{
		m_wait -= Time::GetFixedDeltaTime();
		return;
	}
	if (!m_used)
	{
		m_trigger->SetEnable(true);
		m_used = true;
	}
	else
	{
		GetGameObject()->Destroy();
	}
}

void PlayerPusher::LateUpdate()
{
	if (m_stickedEnemy && m_stickedEnemy->IsHurt())
	{
		GetGameObject()->Destroy();
	}
}

void PlayerPusher::OnDestroy()
{
	m_trigger->OnTriggerEnter -= Function<void, Collider*>(this, &PlayerPusher::OnTriggerEnter);
	DetachGameObject();
}

void PlayerPusher::OnTriggerEnter(Collider* collider)
{
	if (GetGameObject()->IsShouldDestroy()) return;

	Body* body = collider->GetBody();
	if (body->GetGameObject()->GetTag() == TAG_PLAYER)
	{
		PlayerController* player = body->GetComponent<PlayerController>();
		if (!player) return;
		if (player->IsRolling()) return;
		float hd = GetTransform()->GetScale().x;
		if (hd < 0) hd = -1;
		else hd = +1;
		Vec2 dir(hd, 1);
		dir.Normalize();
		// 이미 맞았는데 또 맞으면 뒤집니다.
		if (!m_damage && !player->IsHurt())
			player->HardKnockback(dir * 300, this);
		else
			player->Damage(dir * 300, this);
		GetGameObject()->Destroy();

		CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_punch_hit.wav", CSoundMgr::CHANNELID::ENEMY_MELEE_HIT);
	}
}

void PlayerPusher::OnDestroyStickedObject(Object* stickedEnemy)
{
	DetachGameObject();
}

void PlayerPusher::SetDamage(bool enable)
{
	m_damage = enable;
}

void PlayerPusher::StickToGameObject(Enemy* stickedEnemy)
{
	DetachGameObject();
	AttachGameObject(stickedEnemy);
}

void PlayerPusher::SetWait(float wait)
{
	m_wait = wait;
}

void PlayerPusher::DetachGameObject()
{
	if (!m_stickedEnemy) return;
	m_stickedEnemy->OnDestoryCallback -= Function<void, Object*>(this, &PlayerPusher::OnDestroyStickedObject);
	m_stickedEnemy = nullptr;
}

void PlayerPusher::AttachGameObject(Enemy* stickedEnemy)
{
	if (!stickedEnemy) return;
	m_stickedEnemy = stickedEnemy;
	m_stickedEnemy->OnDestoryCallback += Function<void, Object*>(this, &PlayerPusher::OnDestroyStickedObject);
}
