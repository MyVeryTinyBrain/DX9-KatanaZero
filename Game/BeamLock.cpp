#include "stdafx.h"
#include "BeamLock.h"
#include "Beam.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "GameRenderOrder.h"
#include "SoundMgr.h"

void BeamLock::Awake()
{
	auto rendObj = GameObject::CreateChild(GetGameObject());
	m_renderer = rendObj->AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_EFFECT_FRONT);

	Animator::Awake();

	SetAdjust(false);

	m_animation = AnimationManager::GetAnimation(L"../Animation/effect/prelaser.txt");
	PlayAnimation(m_animation);

	SetLockTime(0.5f);
	SetBeamDuration(0.5f);
	SetBeamLength(600);
	SetBeamWidth(15);
	SetLockWidth(1);

	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_lockon_01.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_0);
}

void BeamLock::BeforeRender()
{
	Animator::BeforeRender();

	Sprite* spr = nullptr;
	GetCurrentAnimation()->TimeOf(GetElapsed(), &spr);

	if (!spr) return;

	Vec2 sprSize = spr->GetSize();
	float w = sprSize.x;
	float hw = w * 0.5f;
	float h = sprSize.y;

	// 준비 레이저가 지정한 길이만큼 그려지도록 설정합니다.
	float localScaleX = m_beamLength / w;
	// 준비 레이저가 지정한 굵기만큼 그려지도록 설정합니다.
	float localScaleY = m_lockWidth / h;
	m_renderer->GetTransform()->SetScale(Vec2(localScaleX, localScaleY));

	// 준비 레이저가 시작지점부터 나아가도록 설정합니다.
	Vec2 localPos = Vec2::right() * hw * localScaleX;
	m_renderer->GetTransform()->SetLocalPosition(localPos);
}

void BeamLock::OnAnimationEnd(const Animation* current)
{
	if (!m_cancel)
	{
		auto o = new GameObject(GetTransform()->GetPosition(), GetTransform()->GetAngle());
		auto beam = o->AddComponent<Beam>();
		beam->SetDuration(m_beamDuration);
		beam->SetLength(m_beamLength);
		beam->SetWidth(m_beamWidth);
		OnCreatedBeam(beam);
	}

	GetGameObject()->Destroy();
}

void BeamLock::OnAnimationChange(const Animation* current, Animation** next)
{
}

void BeamLock::SetLockTime(float time)
{
	if (time < 0) time = 0.01f;
	float animMaxTime = m_animation->GetMaximumTime();
	float speed = animMaxTime / time;
	SetSpeed(speed);
}

void BeamLock::SetLockWidth(float width)
{
	m_lockWidth = width;
}

void BeamLock::SetBeamDuration(float duration)
{
	m_beamDuration = duration;
}

void BeamLock::SetBeamLength(float length)
{
	m_beamLength = length;
}

void BeamLock::SetBeamWidth(float width)
{
	m_beamWidth = width;
}

void BeamLock::Cancel()
{
	m_cancel = true;
}
