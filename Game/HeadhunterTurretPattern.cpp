#include "stdafx.h"
#include "HeadhunterTurretPattern.h"
#include "Trigger.h"
#include "WallTurrets.h"
#include "TeleportRotateBeam.h"

void HeadhunterTurretPattern::Awake()
{
	m_steps[0] = [&]() { Step0(); };
	m_steps[1] = [&]() { Step1(); };
	m_steps[2] = [&]() { StepLast(); };

	m_air = Trigger::FindWithName(L"AirY")->GetTransform()->GetPosition();

	m_step = 0;
	m_elapsed = 0;
}

void HeadhunterTurretPattern::Update()
{
	while (m_step < STEP_MAX && m_elapsed > m_stepTime[m_step])
	{
		m_elapsed -= m_stepTime[m_step];
		m_steps[m_step]();
		++m_step;
	}

	m_elapsed += Time::GetDeltaTime();
}

void HeadhunterTurretPattern::Step0()
{
	for (auto& wallTurrets : WallTurrets::g_wallTurrets)
	{
		wallTurrets->SetEnable(true);
	}
}

void HeadhunterTurretPattern::Step1()
{
	CreateSweepFromRightToLeft(m_air);
}

void HeadhunterTurretPattern::StepLast()
{
	OnEndPattern();
	GetGameObject()->Destroy();
}

void HeadhunterTurretPattern::CreateSweepFromRightToLeft(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportRotateBeam>();
	beam->SetFromDirection(+1.0f);
	beam->SetLockTime(0.15f);
	beam->SetSweepTime(0.5f);
}

void HeadhunterTurretPattern::CreateSweepFromLeftToRight(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportRotateBeam>();
	beam->SetFromDirection(-1.0f);
	beam->SetLockTime(0.15f);
	beam->SetSweepTime(0.5f);
}
