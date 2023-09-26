#include "stdafx.h"
#include "HeadhunterBeamPattern.h"
#include "TeleportStaticBeam.h"
#include "TeleportRotateBeam.h"
#include "Trigger.h"

void HeadhunterBeamPattern::Awake()
{
	m_steps[0] = [&]() { Step0(); };
	m_steps[1] = [&]() { Step1(); };
	m_steps[2] = [&]() { Step2(); };
	m_steps[3] = [&]() { Step3(); };
	m_steps[4] = [&]() { Step4(); };
	m_steps[5] = [&]() { Step5(); };
	m_steps[6] = [&]() { Step6(); };
	m_steps[7] = [&]() { Step7(); };
	m_steps[8] = [&]() { Step8(); };
	m_steps[9] = [&]() { Step9(); };
	m_steps[10] = [&]() { Step10(); };
	m_steps[11] = [&]() { Step11(); };
	m_steps[12] = [&]() { Step12(); };
	m_steps[13] = [&]() { Step13(); };
	m_steps[14] = [&]() { Step14(); };
	m_steps[15] = [&]() { Step15(); };
	m_steps[16] = [&]() { Step16(); };
	m_steps[17] = [&]() { Step17(); };
	m_steps[18] = [&]() { Step18(); };
	m_steps[19] = [&]() { Step19(); };
	m_steps[20] = [&]() { Step20(); };
	m_steps[21] = [&]() { Step21(); };
	m_steps[22] = [&]() { Step22(); };
	m_steps[23] = [&]() { Step23(); };
	m_steps[24] = [&]() { Step24(); };
	m_steps[25] = [&]() { StepLast(); };

	Trigger::FindWithName(L"p0")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p1")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p2")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p3")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p4")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p5")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p6")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p7")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p8")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p9")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p10")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p11")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p12")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p13")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p14")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p15")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p16")->GetBody()->SetEnable(false);
	Trigger::FindWithName(L"p17")->GetBody()->SetEnable(false);

	m_points[0] = Trigger::FindWithName(L"p0")->GetTransform()->GetPosition();
	m_points[1] = Trigger::FindWithName(L"p1")->GetTransform()->GetPosition();
	m_points[2] = Trigger::FindWithName(L"p2")->GetTransform()->GetPosition();
	m_points[3] = Trigger::FindWithName(L"p3")->GetTransform()->GetPosition();
	m_points[4] = Trigger::FindWithName(L"p4")->GetTransform()->GetPosition();
	m_points[5] = Trigger::FindWithName(L"p5")->GetTransform()->GetPosition();
	m_points[6] = Trigger::FindWithName(L"p6")->GetTransform()->GetPosition();
	m_points[7] = Trigger::FindWithName(L"p7")->GetTransform()->GetPosition();
	m_points[8] = Trigger::FindWithName(L"p8")->GetTransform()->GetPosition();
	m_points[9] = Trigger::FindWithName(L"p9")->GetTransform()->GetPosition();
	m_points[10] = Trigger::FindWithName(L"p10")->GetTransform()->GetPosition();
	m_points[11] = Trigger::FindWithName(L"p11")->GetTransform()->GetPosition();
	m_points[12] = Trigger::FindWithName(L"p12")->GetTransform()->GetPosition();
	m_points[13] = Trigger::FindWithName(L"p13")->GetTransform()->GetPosition();
	m_points[14] = Trigger::FindWithName(L"p14")->GetTransform()->GetPosition();
	m_points[15] = Trigger::FindWithName(L"p15")->GetTransform()->GetPosition();
	m_points[16] = Trigger::FindWithName(L"p16")->GetTransform()->GetPosition();
	m_points[17] = Trigger::FindWithName(L"p17")->GetTransform()->GetPosition();

	m_step = 0;
	m_elapsed = 0;
}

void HeadhunterBeamPattern::Update()
{
	while (m_step < STEP_MAX && m_elapsed > m_stepTime[m_step])
	{
		m_elapsed -= m_stepTime[m_step];
		m_steps[m_step]();
		++m_step;
	}

	m_elapsed += Time::GetDeltaTime();
}

void HeadhunterBeamPattern::Step0()
{
	CreateDownBeam(m_points[0]);
}

void HeadhunterBeamPattern::Step1()
{
	CreateDownBeam(m_points[1]);
}

void HeadhunterBeamPattern::Step2()
{
	CreateDownBeam(m_points[2]);
}

void HeadhunterBeamPattern::Step3()
{
	CreateDownBeam(m_points[3]);
}

void HeadhunterBeamPattern::Step4()
{
	CreateSweepFromLeftToRight(m_points[4]);
}

void HeadhunterBeamPattern::Step5()
{
	CreateSweepFromRightToLeft(m_points[5]);
}

void HeadhunterBeamPattern::Step6()
{
	auto beam = CreateDownBeam(m_points[6]);
	beam->SetBeamAngle(240);
}

void HeadhunterBeamPattern::Step7()
{
	auto beam = CreateDownBeam(m_points[7]);
	beam->SetBeamAngle(300);
}

void HeadhunterBeamPattern::Step8()
{
	CreateDownBeam(m_points[11]);
}

void HeadhunterBeamPattern::Step9()
{
	CreateDownBeam(m_points[8]);
}

void HeadhunterBeamPattern::Step10()
{
	CreateDownBeam(m_points[9]);
}

void HeadhunterBeamPattern::Step11()
{
	CreateDownBeam(m_points[10]);
}

void HeadhunterBeamPattern::Step12()
{
	CreateDownBeam(m_points[12]);
}

void HeadhunterBeamPattern::Step13()
{
	CreateSweepFromRightToLeft(m_points[9]);
}

void HeadhunterBeamPattern::Step14()
{
	CreateBeamFromRightToLeft(m_points[17]);
}

void HeadhunterBeamPattern::Step15()
{
	CreateDownBeam(m_points[0]);
}

void HeadhunterBeamPattern::Step16()
{
	CreateDownBeam(m_points[1]);
}

void HeadhunterBeamPattern::Step17()
{
	CreateDownBeam(m_points[2]);
}

void HeadhunterBeamPattern::Step18()
{
	CreateDownBeam(m_points[3]);
}

void HeadhunterBeamPattern::Step19()
{
	auto beam = CreateBeamFromLeftToRight(m_points[13]);
	beam->SetBeamDuration(0.1f);
	beam->SetBeamLockTime(0.35f);
}

void HeadhunterBeamPattern::Step20()
{
	auto beam = CreateBeamFromRightToLeft(m_points[14]);
	beam->SetBeamDuration(0.1f);
	beam->SetBeamLockTime(0.35f);
}

void HeadhunterBeamPattern::Step21()
{
	auto beam = CreateBeamFromLeftToRight(m_points[15]);
	beam->SetBeamDuration(0.1f);
	beam->SetBeamLockTime(0.35f);
}

void HeadhunterBeamPattern::Step22()
{
	auto beam = CreateBeamFromRightToLeft(m_points[16]);
	beam->SetBeamDuration(0.1f);
	beam->SetBeamLockTime(0.35f);
}

void HeadhunterBeamPattern::Step23()
{
	CreateSweepFromRightToLeft(m_points[9]);
}

void HeadhunterBeamPattern::Step24()
{
	CreateBeamFromLeftToRight(m_points[13]);
}

void HeadhunterBeamPattern::StepLast()
{
	OnEndPattern();
	GetGameObject()->Destroy();
}

TeleportStaticBeam* HeadhunterBeamPattern::CreateDownBeam(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportStaticBeam>();
	beam->UseDownAnimation();
	beam->SetBeamAngle(270);
	beam->SetBeamLockTime(0.5f);
	beam->SetBeamDuration(0.2f);
	beam->SetAnimationTime(0.25f);
	return beam;
}

void HeadhunterBeamPattern::CreateSweepFromRightToLeft(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportRotateBeam>();
	beam->SetFromDirection(+1.0f);
	beam->SetLockTime(0.15f);
	beam->SetSweepTime(0.5f);
}

void HeadhunterBeamPattern::CreateSweepFromLeftToRight(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportRotateBeam>();
	beam->SetFromDirection(-1.0f);
	beam->SetLockTime(0.15f);
	beam->SetSweepTime(0.5f);
}

TeleportStaticBeam* HeadhunterBeamPattern::CreateBeamFromLeftToRight(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportStaticBeam>();
	beam->UseDownAnimation();
	beam->SetBeamAngle(0);
	beam->UseRightAnimation();
	beam->SetBeamLockTime(0.5f);
	beam->SetBeamDuration(0.2f);
	beam->SetAnimationTime(0.35f);
	return beam;
}

TeleportStaticBeam* HeadhunterBeamPattern::CreateBeamFromRightToLeft(const Vec2& p)
{
	auto o = new GameObject(p);
	auto beam = o->AddComponent<TeleportStaticBeam>();
	beam->UseDownAnimation();
	beam->SetBeamAngle(180);
	beam->UseLeftAnimation();
	beam->SetBeamLockTime(0.5f);
	beam->SetBeamDuration(0.2f);
	beam->SetAnimationTime(0.35f);
	return beam;
}
