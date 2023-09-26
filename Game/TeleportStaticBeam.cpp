#include "stdafx.h"
#include "TeleportStaticBeam.h"
#include "AnimationManager.h"
#include "StaticEffect.h"
#include "BeamLock.h"
#include "GameRenderOrder.h"
#include "Animation.h"
#include "SoundMgr.h"

void TeleportStaticBeam::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_ENEMY);

	Animator::Awake();

	SetAdjust(false);

	m_down_in = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_teleport_down_in.txt");
	m_right_in = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_teleport_ground_in.txt");
	m_down_out = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_teleport_down_out.txt");
	m_right_out = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_teleport_ground_out.txt");
	m_use = m_down_in;

	m_beamAngle = 270.0f;
	m_beamLockTime = 0.5f;
	m_beamDuration = 0.15f;

	SetAnimationTime(0.2f);

	int soundCase = rand() % 3;
	switch (soundCase)
	{
		case 0:
			CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_appear_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
			break;
		case 1:
			CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_appear_02.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
			break;
		case 2:
			CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_appear_03.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
			break;
	}
}

void TeleportStaticBeam::Start()
{
	Animator::Start();

	PlayAnimation(m_use);

	if (m_use == m_right_in && m_left)
	{
		GetTransform()->SetScale(Vec2(-1, 1));
	}
}

void TeleportStaticBeam::Update()
{
	auto current = GetCurrentAnimation();
	if (current)
		SetSpeed(current->GetMaximumTime() / m_halfTime);

	Animator::Update();
}

void TeleportStaticBeam::OnAnimationEnd(const Animation* current)
{
	Vec2 pos = GetTransform()->GetPosition();

	if (current == m_down_in)
	{
		PlayAnimation(m_down_out);
		CreateBeamLock();
	}
	if (current == m_right_in)
	{
		PlayAnimation(m_right_out);
		CreateBeamLock();
	}
	if (current == m_down_out || current == m_right_out)
	{
		GetGameObject()->Destroy();
	}
}

void TeleportStaticBeam::OnAnimationChange(const Animation* current, Animation** next)
{
}

void TeleportStaticBeam::UseDownAnimation()
{
	m_use = m_down_in;
	m_left = false;
}

void TeleportStaticBeam::UseRightAnimation()
{
	m_use = m_right_in;
	m_left = false;
}

void TeleportStaticBeam::UseLeftAnimation()
{
	m_use = m_right_in;
	m_left = true;
}

void TeleportStaticBeam::SetBeamAngle(float angle)
{
	m_beamAngle = angle;
}

void TeleportStaticBeam::SetBeamLockTime(float time)
{
	m_beamLockTime = time;
}

void TeleportStaticBeam::SetBeamDuration(float duration)
{
	m_beamDuration = duration;
}

void TeleportStaticBeam::SetAnimationTime(float time)
{
	m_halfTime = time * 0.5f;
}

void TeleportStaticBeam::CreateBeamLock()
{
	auto beamLockObj = new GameObject(GetTransform()->GetPosition(), m_beamAngle);
	auto beamLock = beamLockObj->AddComponent<BeamLock>();
	beamLock->SetLockTime(m_beamLockTime);
	beamLock->SetBeamDuration(m_beamDuration);
}
