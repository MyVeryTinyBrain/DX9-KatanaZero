#include "stdafx.h"
#include "SwordEffect.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "Door.h"
#include "DebugDraw.h"
#include "GameRenderOrder.h"
#include "Enemy.h"
#include "PlayerController.h"
#include "TimeController.h"
#include "PlayerCamera.h"
#include "HitLineEffect.h"
#include "Boss.h"
#include "Turret.h"
#include "BreakablePlatform.h"
#include "SoundMgr.h"
#include "EnemyShieldCop.h"
#include "StaticEffect.h"

void SwordEffect::Awake()
{
	m_stickedGameObject = nullptr;

	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_EFFECT_SWORDEFFECT);
	m_slash = AnimationManager::GetAnimation(L"../Animation/effect/slash.txt");

	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Dynamic);
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);
	m_body->GetGameObject()->SetTag(TAG_DAMAGABLE_TO_ENEMY);

	GameObject* triggerObject = GameObject::CreateChild(GetGameObject());
	triggerObject->GetTransform()->SetLocalPosition(Vec2(20.0f, 0));
	m_trigger = triggerObject->AddComponent<BoxCollider>();
	m_trigger->SetWidth(45);
	m_trigger->SetHeight(30);
	m_trigger->SetTriggerMode(true);
	m_trigger->DrawDebug = DRAW_TRIGGER;
	m_trigger->OnTriggerEnter += Function<void, Collider*>(this, &SwordEffect::OnTriggerEnter);

	Animator::Awake();

	PlayAnimation(m_slash);

	int soundCase = rand() % 3;
	switch (soundCase)
	{
		case 0:
			CSoundMgr::Get_Instance()->PlaySound(L"sound_player_slash_1.wav", CSoundMgr::CHANNELID::SWORDEFFECT);
			break;
		case 1:
			CSoundMgr::Get_Instance()->PlaySound(L"sound_player_slash_2.wav", CSoundMgr::CHANNELID::SWORDEFFECT);
			break;
		case 2:
			CSoundMgr::Get_Instance()->PlaySound(L"sound_player_slash_3.wav", CSoundMgr::CHANNELID::SWORDEFFECT);
			break;
	}
}

void SwordEffect::Update()
{
	Animator::Update();

	if (m_longTake)
	{
		if (GetPercent() > 0.5f)
			m_trigger->SetEnable(false);
	}
	else
	{
		if (GetPercent() > 0.99f)
			m_trigger->SetEnable(false);
	}

	if (m_stickedGameObject)
	{
		Transform* target = m_stickedGameObject->GetTransform();
		Transform* self = GetTransform();
		self->SetPosition(target->GetPosition());
	}
}

void SwordEffect::OnDestroy()
{
	m_trigger->OnTriggerEnter -= Function<void, Collider*>(this, &SwordEffect::OnTriggerEnter);
	DetachGameObject();
}

void SwordEffect::OnAnimationEnd(const Animation* current)
{
	GetGameObject()->Destroy();
}

void SwordEffect::OnAnimationChange(const Animation* current, Animation** next)
{
}

void SwordEffect::OnTriggerEnter(Collider* collider)
{
	Body* body = collider->GetBody();

	if (body->GetGameObject()->GetTag() == TAG_DOOR)
	{
		Door* door = body->GetComponent<Door>();
		if (!door) return;
		if (std::find(m_damaged.begin(), m_damaged.end(), body) != m_damaged.end()) return;
		float hd = door->GetTransform()->GetPosition().x - GetTransform()->GetPosition().x;
		door->Open(hd);
		m_damaged.push_back(body);

		PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.1f, 300, 10);
	}
	else if (body->GetGameObject()->GetTag() == TAG_ENEMY)
	{
		Enemy* enemy = body->GetComponent<Enemy>();
		if (!enemy) return;
		if (std::find(m_damaged.begin(), m_damaged.end(), body) != m_damaged.end()) return;
		if (enemy->IsHurt()) return;
		bool damaged = enemy->Damage(GetTransform()->GetRight() * 400, this, true);
		if (damaged)
		{
			TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
			PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
			HitLineEffect::Create(body->GetTransform()->GetPosition(), GetTransform()->GetRight());

			int soundCase = rand() % 2;
			switch (soundCase)
			{
				case 0:
					CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
					break;
				case 1:
					CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_02.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
					break;
			}
		}
		else
		{
			PlayerController::GetInstance()->Knockback(-GetTransform()->GetRight() * 300, this);

			auto effect = StaticEffect::Create(L"../Animation/effect/bulletreflect.txt", enemy->GetTransform()->GetPosition());
			effect->SetRenderOrder(RENDER_ORDER_EFFECT_BULLET_REFLECT);

			if (dynamic_cast<EnemyShieldCop*>(enemy))
				CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_swordshield.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
			else 
				CSoundMgr::Get_Instance()->PlaySound(L"sound_swordclash.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
		}
		m_damaged.push_back(body);
	}
	else if (body->GetGameObject()->GetTag() == TAG_BOSS)
	{
		Boss* boss = body->GetComponent<Boss>();
		if (!boss) return;
		if (std::find(m_damaged.begin(), m_damaged.end(), body) != m_damaged.end()) return;
		if (!boss->CanTakeDamage()) return;
		bool damaged = boss->TakeDamage(GetTransform()->GetRight() * 400, this, true);
		if (!damaged) PlayerController::GetInstance()->Knockback(-GetTransform()->GetRight() * 300, this);
		if (damaged)
		{
			TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
			PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
			HitLineEffect::Create(body->GetTransform()->GetPosition(), GetTransform()->GetRight());

			int soundCase = rand() % 2;
			switch (soundCase)
			{
				case 0:
					CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
					break;
				case 1:
					CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_02.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
					break;
			}
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sound_swordclash.wav", CSoundMgr::CHANNELID::SWORDEFFECT_HIT);
		}
		m_damaged.push_back(body);
	}
	else if (body->GetGameObject()->GetTag() == TAG_TURRET)
	{
		Turret* turret = body->GetComponent<Turret>();
		if (!turret) return;
		if (!turret->Damage()) return;

		TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
		PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
		HitLineEffect::Create(turret->GetHeadTransform()->GetPosition(), GetTransform()->GetRight());

		m_damaged.push_back(body);
	}
	else if (body->GetGameObject()->GetTag() == TAG_BREAKABLE_PLATFORM)
	{
		BreakablePlatform* platform = body->GetComponent<BreakablePlatform>();
		platform->BreakPlatform(GetTransform()->GetRight() * 300);
	}
}

void SwordEffect::OnDestroyStickedObject(Object* stickedObject)
{
	DetachGameObject();
}

void SwordEffect::StickToGameObject(GameObject* gameObject)
{
	DetachGameObject();
	AttachGameObject(gameObject);
}

BoxCollider* SwordEffect::GetTrigger() const
{
	return m_trigger;
}

void SwordEffect::DetachGameObject()
{
	if (!m_stickedGameObject) return;
	m_stickedGameObject->OnDestoryCallback -= Function<void, Object*>(this, &SwordEffect::OnDestroyStickedObject);
	m_stickedGameObject = nullptr;
}

void SwordEffect::AttachGameObject(GameObject* gameObject)
{
	if (!gameObject) return;
	m_stickedGameObject = gameObject;
	m_stickedGameObject->OnDestoryCallback += Function<void, Object*>(this, &SwordEffect::OnDestroyStickedObject);
}
