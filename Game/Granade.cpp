#include "stdafx.h"
#include "Granade.h"
#include "StaticEffect.h"
#include "GameRenderOrder.h"
#include "GameLayer.h"
#include "DamageZone.h"
#include "StaticEffect.h"
#include "DebugDraw.h"
#include "TrailRenderer.h"
#include "PlayerCamera.h"
#include "SoundMgr.h"

void Granade::Awake()
{
	GetGameObject()->SetName(L"test");

	auto grndObj = GameObject::CreateChild(GetGameObject());
	grndObj->SetName(L"test0");
	grndObj->GetTransform()->SetScale(Vec2::one() * 0.5f);
	m_sprGranade = new Sprite(L"../Texture/object/used/granade/spr_granade.png");
	m_granadeRenderer = grndObj->AddComponent<SpriteRenderer>();
	m_granadeRenderer->SetRenderOrder(RENDER_ORDER_FRONTOBJECT);
	m_granadeRenderer->SetSprite(m_sprGranade);

	auto lightObj = GameObject::CreateChild(GetGameObject());
	lightObj->SetName(L"test1");
	lightObj->GetTransform()->SetScale(Vec2::one() * 0.5f);
	m_sprLight = new Sprite(L"../Texture/object/used/granade/spr_granade_light.png");
	m_lightRenderer = lightObj->AddComponent<SpriteRenderer>();
	m_lightRenderer->SetRenderOrder(RENDER_ORDER_EFFECT_BACK);
	m_lightRenderer->SetSprite(m_sprLight);
	m_lightRenderer->SetColor(Color(1, 1, 1, 0.75f));

	auto rangeObj = GameObject::CreateChild(GetGameObject());
	rangeObj->SetName(L"test2");
	rangeObj->GetTransform()->SetScale(Vec2::zero());
	m_sprRange = new Sprite(L"../Texture/object/used/granade/spr_granade_range1.png");
	m_rangeRenderer = rangeObj->AddComponent<SpriteRenderer>();
	m_rangeRenderer->SetRenderOrder(RENDER_ORDER_VISUALIZE_EFFECT);
	m_rangeRenderer->SetSprite(m_sprRange);

	m_body = AddComponent<Body>();
	m_body->SetLayerIndex(LAYERINDEX_GROUND_OBJECT);
	m_body->SetGravityScale(1.0f);
	m_body->SetLinearDamping(0.2f);

	m_collider = AddComponent<CircleCollider>();
	m_collider->SetRadius(5);
	m_collider->SetRestitution(1.0f);
	m_collider->DrawDebug = DRAW_OBJECT;

	m_startVelocity = Vec2(0.707f, 0.707f) * 500;
	m_rangeRadius = 90;

	m_readyToPlayWarningSound = true;
}

void Granade::Start()
{
	m_body->SetVelocity(m_startVelocity);
}

void Granade::Update()
{
	constexpr float activeTime = 2.0f;
	if (m_elapsed > activeTime)
	{
		m_body->SetEnable(false);
		m_activeElapsed += Time::GetDeltaTime();
	}
	else
	{
		if (m_trailCounter <= 0)
		{
			auto trail = TrailRenderer::Create(m_granadeRenderer);
			trail->SetBeginColor(Color(1, 0, 0, 0.5f));
			trail->SetEndColor(Color(1, 0, 0, 0));
			trail->SetScaleMode(true);
			trail->SetDuration(0.45f);
			m_trailCounter = 0.005f;
		}
		else m_trailCounter -= Time::GetDeltaTime();

		m_elapsed += Time::GetDeltaTime();
	}
	if (m_elapsed < activeTime) return;

	constexpr float explosionTime = 1.0f;
	float percent = m_activeElapsed / explosionTime;
	if (percent > 1.0f) percent = 1.0f;
	float newPercent = 1 - powf(percent - 1, 4);
	if (newPercent < 0.05f) newPercent = 0.05f;

	float scaleRatio = m_rangeRadius / (m_sprRange->GetSize().x * 0.5f);
	m_rangeRenderer->GetTransform()->SetScale(newPercent * scaleRatio * Vec2::one());
	m_rangeRenderer->GetTransform()->SetAngle(newPercent * 180);
	m_rangeRenderer->SetColor(Color(1, 1, 1, 1 - powf(percent, 4)));

	if (m_readyToPlayWarningSound && fabsf(m_activeElapsed - explosionTime) < 0.6f)
	{
		m_readyToPlayWarningSound = false;
		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressbomb_armed_01.wav", CSoundMgr::CHANNELID::BOSS_PROJECTILE);
	}

	if (m_activeElapsed > explosionTime)
	{
		Vec2 center = GetTransform()->GetPosition();
		for (int i = 0; i < 20; ++i)
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
		damage->SetAdjustVelocity(Vec2::up() * 400);
		damage->SetExplosion(true);

		PlayerCamera::GetInstance()->Shake(Vec2::Direction(float(rand() % 360)), 0.2f, 600, 10);

		GetGameObject()->Destroy();
	}
}

void Granade::OnDestroy()
{
	SAFE_DELETE(m_sprGranade);
	SAFE_DELETE(m_sprLight);
	SAFE_DELETE(m_sprRange);
}

void Granade::SetVelocity(const Vec2& velocity)
{
	m_startVelocity = velocity;
}
