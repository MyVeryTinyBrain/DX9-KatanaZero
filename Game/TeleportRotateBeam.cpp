#include "stdafx.h"
#include "TeleportRotateBeam.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "BeamLock.h"
#include "Beam.h"
#include "GameRenderOrder.h"
#include "SoundMgr.h"

#define MAX_ANGLE_RANGE (180.0f)

void TeleportRotateBeam::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_ENEMY);

	Animator::Awake();

	SetAdjust(false);

	m_rifle_sweep_teleport_in = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_sweep_teleport_in.txt");
	m_rifle_sweep_teleport_in_wait = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_sweep_teleport_in_wait.txt");
	m_rifle_sweep = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_sweep.txt");
	m_rifle_sweep_teleport_out = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_sweep_teleport_out.txt");
	
	PlayAnimation(m_rifle_sweep_teleport_in);

	m_fromDirection = 1.0f;
	m_angleRange = 180;
	m_beginLength = 35;

	m_lockTime = 0.3f;
	m_sweepTime = 0.5f;

	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressbeam_circle_01.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_2);
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

void TeleportRotateBeam::Start()
{
	Animator::Start();

	GetTransform()->SetScale(Vec2(m_fromDirection, 1.0f));
}

void TeleportRotateBeam::Update()
{
	Animator::Update();
}

void TeleportRotateBeam::LateUpdate()
{
	Animator::LateUpdate();

	float currentAngle = GetCurrentAngle();
	float currentPercent = GetPercent();

	if (m_beam)
	{
		m_beam->GetTransform()->SetAngle(GetCurrentAngle());
		m_beam->GetTransform()->SetPosition(GetBeginPosition(GetCurrentAngle()));
	}

	if (!m_beam && GetCurrentAnimation() == m_rifle_sweep && currentPercent >= GetStartPercent())
	{
		auto o = new GameObject(GetBeginPosition(GetStartAngle()), GetStartAngle());
		m_beam = o->AddComponent<Beam>();
		m_beam->SetDuration(m_sweepTime);
		m_beam->OnDestoryCallback += Function<void, Object*>(this, &TeleportRotateBeam::OnDestroyBeam);
		m_beam->m_soundType = Beam::SoundType::NOPLAY;
	}
}

void TeleportRotateBeam::OnDestroy()
{
	if (m_beam)
	{
		m_beam->OnDestoryCallback -= Function<void, Object*>(this, &TeleportRotateBeam::OnDestroyBeam);
		m_beam = nullptr;
	}
}

void TeleportRotateBeam::OnAnimationEnd(const Animation* current)
{
	if (current == m_rifle_sweep_teleport_in)
	{
		auto o = new GameObject(GetBeginPosition(GetStartAngle()), GetStartAngle());
		auto beamLock = o->AddComponent<BeamLock>();
		beamLock->SetLockTime(m_lockTime);
		beamLock->Cancel();

		if (m_lockTime <= 0)
		{
			PlaySweep();
		}
		else
		{
			SetSpeed(m_rifle_sweep_teleport_in_wait->GetMaximumTime() / m_lockTime);
			PlayAnimation(m_rifle_sweep_teleport_in_wait);
		}
	}
	if (current == m_rifle_sweep_teleport_in_wait)
	{
		PlaySweep();
	}
	if (current == m_rifle_sweep)
	{
		PlayAnimation(m_rifle_sweep_teleport_out);
	}
	if (current == m_rifle_sweep_teleport_out)
	{
		GetGameObject()->Destroy();
	}
}

void TeleportRotateBeam::OnAnimationChange(const Animation* current, Animation** next)
{
}

void TeleportRotateBeam::OnCreatedBeam(Beam* beam)
{
	m_beam = beam;
	m_beam->OnDestoryCallback += Function<void, Object*>(this, &TeleportRotateBeam::OnDestroyBeam);
}

void TeleportRotateBeam::OnDestroyBeam(Object* object)
{
	if (m_beam && m_beam == object)
	{
		m_beam->OnDestoryCallback -= Function<void, Object*>(this, &TeleportRotateBeam::OnDestroyBeam);
		m_beam = nullptr;
	}
}

void TeleportRotateBeam::SetFromDirection(float hd)
{
	m_fromDirection = hd;
}

void TeleportRotateBeam::SetAngleRange(float angleRange)
{
	m_angleRange = angleRange;
}

void TeleportRotateBeam::SetLockTime(float time)
{
	m_lockTime = time;
}

void TeleportRotateBeam::SetSweepTime(float time)
{
	m_sweepTime = time;
}

float TeleportRotateBeam::GetStartAngle() const
{
	return 270 + m_angleRange * 0.5f * m_fromDirection;
}

float TeleportRotateBeam::GetEndAngle() const
{
	return 270 - m_angleRange * 0.5f * m_fromDirection;
}

Vec2 TeleportRotateBeam::GetBeginPosition(float angle)
{
	return GetTransform()->GetPosition() + Vec2::Direction(angle * DEG2RAD) * m_beginLength;
}

float TeleportRotateBeam::GetStartPercent() const
{
	float startPercent = -1.0f / 360.0f * m_angleRange + 0.5f;
	return startPercent;
}

float TeleportRotateBeam::GetEndPercent() const
{
	float startPercent = GetStartPercent();
	return 1.0f - startPercent;
}

float TeleportRotateBeam::GetCurrentAngle() const
{
	if (GetCurrentAnimation() == m_rifle_sweep_teleport_out) return GetEndAngle();
	else if (GetCurrentAnimation() == nullptr) return GetEndAngle();
	else if (GetCurrentAnimation() != m_rifle_sweep) return GetStartAngle();

	float percent = GetPercent();
	float startAngle = GetStartAngle();

	float startPercent = GetStartPercent();
	float endPercent = GetEndPercent();
	float deltaPercent = endPercent - startPercent;
	percent = percent;
	if (percent < startPercent) percent = startPercent;
	else if (percent > endPercent) percent = endPercent;
	float newPercent = (percent - startPercent) / deltaPercent;

	float adjustAngle = newPercent * m_angleRange;
	float angle = startAngle - adjustAngle * m_fromDirection;
	return angle;

}

void TeleportRotateBeam::PlaySweep()
{
	if (m_sweepTime <= 0) m_sweepTime = 0.01f;
	SetSpeed(m_rifle_sweep->GetMaximumTime() / m_sweepTime);
	PlayAnimation(m_rifle_sweep);
}
