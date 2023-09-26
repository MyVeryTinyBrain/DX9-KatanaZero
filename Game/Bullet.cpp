#include "stdafx.h"
#include "Bullet.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "PlayerController.h"
#include "SwordEffect.h"
#include "Enemy.h"
#include "TrailRenderer.h"
#include "DebugDraw.h"
#include "TimeController.h"
#include "GameRenderOrder.h"
#include "PlayerCamera.h"
#include "HitLineEffect.h"
#include "StaticEffect.h"
#include "Boss.h"
#include "ThrowObject.h"
#include "SoundMgr.h"
#include "EnemyShieldCop.h"

wstring Bullet::g_bulletName = L"bullet";

void Bullet::Awake()
{
	GetGameObject()->SetTag(TAG_DAMAGABLE_TO_PLAYER);
	GetGameObject()->SetName(g_bulletName);

	auto spr = new Sprite(L"../Texture/object/used/bullet/spr_bullet_0.png");

	constexpr float bullet_width = 20;

	m_body = AddComponent<Body>();
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);
	m_body->SetGravityMode(false);
	m_body->SetContinuousMode(true);

	auto triggerObj = GameObject::CreateChild(GetGameObject());
	m_trigger = triggerObj->AddComponent<BoxCollider>();
	m_trigger->SetTriggerMode(true);
	m_trigger->SetWidth(bullet_width);
	m_trigger->SetHeight(2);
	m_trigger->DrawDebug = DebugDraw::DRAW_TRIGGER;
	m_trigger->OnTriggerEnter += Function<void, Collider*>(this, &Bullet::OnTriggerEnter);
	
	auto rendererObject = GameObject::CreateChild(GetGameObject());
	rendererObject->GetTransform()->SetScale(Vec2(bullet_width / spr->GetSize().x, 1.0f / spr->GetSize().y * 1.1f));
	m_renderer = rendererObject->AddComponent<SpriteRenderer>();
	m_renderer->SetSprite(spr);
	m_renderer->SetRenderOrder(RENDER_ORDER_BULLET);

	m_speed = 500;
	m_elapsed = 0;
}

void Bullet::FixedUpdate()
{
	if (m_elapsed > 2.0f)
	{
		GetGameObject()->Destroy();
		return;
	}

	Vec2 dir = GetTransform()->GetRight().normalized();
	Vec2 vel = dir * m_speed;
	m_body->SetVelocity(vel);

	m_elapsed += Time::GetFixedDeltaTime();
}

void Bullet::Update()
{
	TrailRenderer* trail = TrailRenderer::Create(m_renderer);
	trail->SetBeginColor(Color(1, 1, 1, 1));
	trail->SetEndColor(Color(0.75f, 1, 0.75f, 0));
	trail->SetDuration(0.05f);
	trail->SetTimeScaleMode(false);
}

void Bullet::OnDestroy()
{
	Sprite* sprite = m_renderer->GetSprite();
	SAFE_DELETE(sprite);
}

void Bullet::OnTriggerEnter(Collider* collider)
{
	Body* body = collider->GetBody();
	tag_t tag = body->GetGameObject()->GetTag();

	if (collider->GetBody()->GetLayerIndex() == LAYERINDEX_GROUND ||
		collider->GetBody()->GetLayerIndex() == LAYERINDEX_DOOR)
	{
		GetGameObject()->Destroy();
		return;
	}

	switch (tag)
	{
		case GameTag::TAG_DAMAGABLE_TO_ENEMY:
			{
				if (!m_fromEnemy) return;
				SwordEffect* swordEffect = body->GetComponent<SwordEffect>();
				if (!swordEffect) return;

				Vec2 xBasis = GetTransform()->GetRight();
				float angle = Quat::FromToRotation(Vec2::right(), -xBasis).GetEulerAngle().z;
				m_body->SetAngle(angle);

				GetGameObject()->SetTag(TAG_DAMAGABLE_TO_ENEMY);
				m_fromEnemy = false;

				TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
				PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
				auto effect = StaticEffect::Create(L"../Animation/effect/bulletreflect.txt", GetTransform()->GetPosition());
				effect->SetRenderOrder(RENDER_ORDER_EFFECT_BULLET_REFLECT);

				CSoundMgr::Get_Instance()->PlaySound(L"sound_slash_bullet.wav", CSoundMgr::CHANNELID::BULLET_HIT);
			}
			break;
		case GameTag::TAG_PLAYER:
			{
				if (!m_fromEnemy) return;
				if (PlayerController::GetInstance()->IsRolling()) return;

				float hd = GetTransform()->GetRight().x < 0 ? -1.0f : +1.0f;
				Vec2 dir = Vec2(hd, 1.0f).normalized();
				PlayerController::GetInstance()->Damage(dir * 400, this);

				GetGameObject()->Destroy();

				TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
				PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);

				CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_bullet.wav", CSoundMgr::CHANNELID::BULLET_HIT);
			}
			break;
		case GameTag::TAG_ENEMY:
			{
				if (m_fromEnemy) return;
				Enemy* enemy = body->GetComponent<Enemy>();
				if (!enemy) return;
				if (enemy->IsHurt()) return;
				bool dead = enemy->Damage(GetTransform()->GetRight() * 400, this, true);
				GetGameObject()->Destroy();

				TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
				PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
				
				if (dead)
				{
					HitLineEffect::Create(body->GetTransform()->GetPosition(), GetTransform()->GetRight());

					CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_bullet.wav", CSoundMgr::CHANNELID::BULLET_HIT);
				}
				else
				{
					if (dynamic_cast<EnemyShieldCop*>(enemy))
						CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_swordshield.wav", CSoundMgr::CHANNELID::BULLET_HIT);
					else
						CSoundMgr::Get_Instance()->PlaySound(L"sound_swordclash.wav", CSoundMgr::CHANNELID::BULLET_HIT);
				}
			}
			break;
		case GameTag::TAG_BOSS: 
			{
				if (m_fromEnemy) return;
				Boss* boss = body->GetComponent<Boss>();
				if (!boss) return;
				if (!boss->CanTakeDamage()) return;
				boss->TakeDamage(GetTransform()->GetRight() * 400, this, true);
				bool damage = boss->TakeDamage(GetTransform()->GetRight() * 400, this, true);
				GetGameObject()->Destroy();

				TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
				PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
				HitLineEffect::Create(body->GetTransform()->GetPosition(), GetTransform()->GetRight());

				if (damage)
				{
					CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_bullet.wav", CSoundMgr::CHANNELID::BULLET_HIT);
				}
			}
			break;
		case GameTag::TAG_THROW_OBJECT:
			{
				if (!m_fromEnemy) return;
				ThrowObject* to = body->GetComponent<ThrowObject>();
				if (!to) return;
				if (!to->CanCrashWithBullet()) return;
				to->Crash(m_body->GetVelocity());

				GetGameObject()->Destroy();

				CSoundMgr::Get_Instance()->PlaySound(L"sound_swordclash.wav", CSoundMgr::CHANNELID::BULLET_HIT);
			}
			break;
	}
}

Bullet* Bullet::Create(const Vec2& pos, const Vec2& right, float speed, bool fromEnemy)
{
	GameObject* object = new GameObject(pos, 0.0f);
	object->GetTransform()->SetRight(right);
	Bullet* bullet = object->AddComponent<Bullet>();
	bullet->m_speed = speed;
	bullet->m_fromEnemy = fromEnemy;
	bullet->m_body->SetVelocity(bullet->GetTransform()->GetRight() * speed);
	if(fromEnemy) object->SetTag(TAG_DAMAGABLE_TO_PLAYER);
	else object->SetTag(TAG_DAMAGABLE_TO_ENEMY);

	auto gunspark = StaticEffect::Create(L"../Animation/effect/gunspark.txt", pos, right, Vec2::one(), Vec2::zero(), Vec2::zero(), false, true);

	return bullet;
}

const wstring& Bullet::BulletName()
{
	return g_bulletName;
}
