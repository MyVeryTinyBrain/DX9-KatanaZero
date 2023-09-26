#include "stdafx.h"
#include "ExplosiveBarrel.h"
#include "StaticEffect.h"
#include "DamageZone.h"
#include "PlayerCamera.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "GameRenderOrder.h"
#include "DebugDraw.h"

void ExplosiveBarrel::Awake()
{
	GetGameObject()->SetTag(TAG_EXPLOSIVE_BARREL);

	m_spr = new Sprite(L"../Texture/object/used/explosive_barrel/spr_explosive_barrel.png");
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetSprite(m_spr);
	m_renderer->SetRenderOrder(RENDER_ORDER_EXPLOSIVE_BARREL);

	m_body = AddComponent<Body>();
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);

	auto o = GameObject::CreateChild(GetGameObject());
	m_trigger = o->AddComponent<BoxCollider>();
	m_trigger->SetTriggerMode(true);
	m_trigger->SetWidth(21);
	m_trigger->SetHeight(27);
	m_trigger->DrawDebug = DRAW_OBJECT;

	m_rangeRadius = 70;
}

void ExplosiveBarrel::Update()
{
	if (m_destroyNextFrame)
	{
		GetGameObject()->Destroy();
		return;
	}

	if (!m_destroyNextFrame && m_readyToDamage)
	{
		if (m_damageWait < 0)
		{
			m_destroyNextFrame = true;

			Vec2 center = GetTransform()->GetPosition();
			for (int i = 0; i < 10; ++i)
			{
				float angle = float(rand() % 360);
				float dist = fmodf(float(rand()), m_rangeRadius) + 10;
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
		else
		{
			m_damageWait -= Time::GetDeltaTime();
		}
	}
}

void ExplosiveBarrel::OnDestroy()
{
	SAFE_DELETE(m_spr);
}

void ExplosiveBarrel::Explosion()
{
	m_readyToDamage = true;
	m_damageWait = 0.05f;
}
