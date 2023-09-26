#include "stdafx.h"
#include "WallTurrets.h"
#include "AnimationManager.h"
#include "GameRenderOrder.h"
#include "StaticSprite.h"
#include "Turret.h"
#include "SoundMgr.h"

std::vector<WallTurrets*> WallTurrets::g_wallTurrets;

void WallTurrets::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_TURRET_WALL);

	Animator::Awake();

	SetAdjust(false);

	m_wallOpen = AnimationManager::GetAnimation(L"../Animation/object/turret/turret_wall_open.txt");

	PlayAnimation(m_wallOpen);

	g_wallTurrets.push_back(this);
}

void WallTurrets::Start()
{
	SetEnable(false);
	m_ready = true;
}

void WallTurrets::Update()
{
	Animator::Update();

	if (m_opened)
		GetGameObject()->Destroy();
}

void WallTurrets::OnEnable()
{
	if (!m_ready) return;
	if (m_enabled) return;
	m_enabled = true;
	CSoundMgr::Get_Instance()->PlaySound(L"sound_object_turrets_deploy_01.wav", CSoundMgr::CHANNELID::TURRET_EFFECT);
}

void WallTurrets::OnDestroy()
{
	auto it = std::find(g_wallTurrets.begin(), g_wallTurrets.end(), this);
	if (it == g_wallTurrets.end()) return;
	g_wallTurrets.erase(it);
}

void WallTurrets::OnAnimationEnd(const Animation* current)
{
	{
		auto o = new GameObject(GetTransform()->GetPosition(), GetTransform()->GetAngle(), GetTransform()->GetScale());
		auto spr = o->AddComponent<StaticSprite>();
		spr->SetSprite(L"../Texture/object/used/turret_wall/spr_bunker_turret_wall_open_20.png");
		spr->GetRenderer()->SetRenderOrder(RENDER_ORDER_TURRET_WALL);
	}
	{
		auto o = new GameObject(GetTransform()->GetPosition(), GetTransform()->GetAngle(), GetTransform()->GetScale());
		auto spr = o->AddComponent<StaticSprite>();
		spr->SetSprite(L"../Texture/object/used/turret_wall/spr_bunker_turret_wall_open_34.png");
		spr->GetRenderer()->SetRenderOrder(RENDER_ORDER_TURRET_FRAME);
	}

	m_opened = true;

	Vec2 center = GetTransform()->GetPosition();
	Vec2 right = GetTransform()->GetRight();
	CreateTurret(center + right * 10 + Vec2::up() * 18);
	CreateTurret(center + right * 10 + Vec2::down() * 31);
}

void WallTurrets::OnAnimationChange(const Animation* current, Animation** next)
{
}

Transform* WallTurrets::CreateTurret(const Vec2& pos)
{
	auto o = new GameObject(pos, GetTransform()->GetAngle(), GetTransform()->GetScale());
	return o->AddComponent<Turret>()->GetTransform();
}
