#include "stdafx.h"
#include "Turret.h"
#include "GameRenderOrder.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "DebugDraw.h"
#include "PlayerController.h"
#include "StaticEffect.h"
#include "Bullet.h"
#include "Gauge.h"
#include "SoundMgr.h"

void Turret::Awake()
{
	GetGameObject()->SetTag(TAG_TURRET);

	m_sprHead = new Sprite(L"../Texture/object/used/turret_head/spr_floor_turret_top_0.png");
	m_sprBody = new Sprite(L"../Texture/object/used/turret_floor/spr_floor_turret_1.png");
	auto headObj = GameObject::CreateChild(GetGameObject());
	headObj->GetTransform()->SetLocalPosition(Vec2(-2, 24));
	auto bodyObj = GameObject::CreateChild(GetGameObject());
	m_headRenderer = headObj->AddComponent<SpriteRenderer>();
	m_bodyRenderer = bodyObj->AddComponent<SpriteRenderer>();
	m_headRenderer->SetSprite(m_sprHead);
	m_bodyRenderer->SetSprite(m_sprBody);
	m_headRenderer->SetRenderOrder(RENDER_ORDER_TURRET_HEAD);
	m_bodyRenderer->SetRenderOrder(RENDER_ORDER_TURRET_BODY);

	m_body = AddComponent<Body>();
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);
	auto triggerObj = GameObject::CreateChild(GetGameObject());
	triggerObj->GetTransform()->SetLocalPosition(Vec2(10, 15));
	m_trigger = triggerObj->AddComponent<BoxCollider>();
	m_trigger->SetTriggerMode(true);
	m_trigger->SetWidth(50);
	m_trigger->SetHeight(36);
	m_trigger->DrawDebug = DRAW_OBJECT;

	m_shootAgainDelay = 1.5f;
	m_firstShootDelay = 0.5f;
	m_shootDelay = 0.1f;

	auto o = GameObject::CreateChild(GetGameObject());
	o->GetTransform()->SetLocalPosition(Vec2(8, 40));
	m_gauge = o->AddComponent<Gauge>();
}

void Turret::Update()
{
	float targetAngle = 0;

	Vec2 beg = m_headRenderer->GetTransform()->GetPosition();
	Vec2 end = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 relVec = end - beg;
	Vec2 dir = relVec.normalized();

	bool detect = false;

	RaycastResult res;
	if (!Physics::Raycast(beg, end, &res, LAYER_GROUND | LAYER_DOOR | LAYER_BREAKABLE_PLATFORM))
	{
		Vec2 angleDir = dir;
		angleDir.x *= m_headRenderer->GetTransform()->GetScale().x;
		angleDir.y *= m_headRenderer->GetTransform()->GetScale().x;
		targetAngle = Vec2::Angle180(angleDir);
		if (fabsf(targetAngle) > 90)
		{
			targetAngle = 0;
		}
		else
		{
			detect = true;
		}
	}

	float curAngle = m_headRenderer->GetTransform()->GetAngle();
	float lerpAngle = curAngle + (targetAngle - curAngle) * Time::GetDeltaTime() * 5;

	if (m_shootAgainCounter <= 0)
	{
		// On detect and shoot
		if (detect)
		{
			m_shootAgainCounter = m_shootAgainDelay;
			if (!m_prevDetect)
				m_shootCounter = m_firstShootDelay;
			else
				m_shootCounter = 0;
			m_shootCount = 2;
		}
	}
	else
	{
		m_shootAgainCounter -= Time::GetDeltaTime();

		// On active
		if (m_shootAgainCounter <= 0)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"sound_turret_activate.wav", CSoundMgr::CHANNELID::TURRET);
		}
	}

	if (m_shootCount > 0)
	{
		// real shoot
		if (m_shootCounter <= 0)
		{
			Vec2 bulletDir = Vec2::Direction(lerpAngle * DEG2RAD) * GetTransform()->GetScale().x;
			Vec2 bulletBeg = beg + bulletDir * 50;

			Bullet::Create(
				bulletBeg, 
				bulletDir,
				1200, 
				true);

			m_shootCounter = m_shootDelay;
			--m_shootCount;

			int soundCase = rand() % 2;
			switch (soundCase)
			{
				case 0:
					CSoundMgr::Get_Instance()->PlaySound(L"sound_gun_fire_1.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
					break;
				case 1:
					CSoundMgr::Get_Instance()->PlaySound(L"sound_gun_fire_2.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
					break;
			}
		}
		else
		{
			m_shootCounter -= Time::GetDeltaTime();
		}
	}

	m_headRenderer->GetTransform()->SetAngle(lerpAngle);

	m_gauge->SetPercent(1 - m_shootAgainCounter / m_shootAgainDelay);
	if (m_shootAgainCounter <= 0) m_gauge->Hide();
	else m_gauge->Show();

	m_prevDetect = detect;
}

void Turret::OnDestroy()
{
	SAFE_DELETE(m_sprHead);
	SAFE_DELETE(m_sprBody);
}

bool Turret::Damage()
{
	if (m_damaged) return false;

	auto destroyAnimation = StaticEffect::Create(
		L"../Animation/object/turret/turret_destroy.txt", 
		GetTransform()->GetPosition(),
		0.0f,
		GetTransform()->GetScale());
	destroyAnimation->SetRenderOrder(RENDER_ORDER_TURRET_BODY);
	destroyAnimation->DontDestroyOnEndOfAnimation();
	GetGameObject()->Destroy();

	CSoundMgr::Get_Instance()->PlaySound(L"sound_turret_death.wav", CSoundMgr::CHANNELID::TURRET);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_turret_overheat.wav", CSoundMgr::CHANNELID::TURRET_EFFECT);

	m_damaged = true;
	return true;
}

Transform* Turret::GetHeadTransform() const
{
	return m_headRenderer->GetTransform();
}
