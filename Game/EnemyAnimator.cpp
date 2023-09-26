#include "stdafx.h"
#include "EnemyAnimator.h"
#include "Animation.h"

void EnemyAnimator::Awake()
{ 
	Animator::Awake();
	ZERO_MEM(m_aim);
	m_aimAdjustY = 0;
	InitAnimations();

	// 할당된 에임 스프라이트의 갯수 확인
	for (int i = 0; i < 19; ++i)
	{
		if (m_aim[i] == nullptr)
		{
			m_aimCount = i;
			break;
		}
	}
	if (m_aimCount == 0 && m_aim[18] != nullptr) m_aimCount = 19;

	SetDefaultAnimation(m_idle);
}

void EnemyAnimator::Update()
{
	Animator::Update();

	Animation* current = GetCurrentAnimation();

	if (current == m_hurtfly && m_hasGround) 
	{
		PlayHurtGround();
	}

	if (IsPlayingHurt()) return;
	if (IsPlayingGroggy()) return;
	if (current == m_turn) return;
	if (current == m_melee) return;
}

void EnemyAnimator::LateUpdate()
{
	Animator::LateUpdate();
}

void EnemyAnimator::BeforeRender()
{
	Animator::BeforeRender();

	if (m_aimming)
	{
		m_renderer->SetSprite(m_aim[m_aimIndex]);
		m_renderer->GetTransform()->SetLocalPosition(Vec2(0, m_aimAdjustY));
	}
}

void EnemyAnimator::OnDestroy()
{
	for (auto& aim : m_aim)
		SAFE_DELETE(aim);
}

void EnemyAnimator::OnAnimationEnd(const Animation* current)
{
	if (current == m_hurtground)
	{
		SetTransition(false);
	}
	if (current == m_turn)
	{
		int test = 0;
	}
}

void EnemyAnimator::OnAnimationChange(const Animation* current, Animation** next)
{
}

void EnemyAnimator::SetHorizontal(EnemyAnimatorHorizontal h)
{
	if (h == m_horizontal) return;

	m_horizontal = h;

	Animation* current = GetCurrentAnimation();
	bool overlap = (current == m_idle || current == m_walk || current == m_run);

	switch (m_horizontal)
	{
		case EnemyAnimatorHorizontal::NOTHING:
			SetDefaultAnimation(m_idle, overlap);
			break;
		case EnemyAnimatorHorizontal::WALK:
			SetDefaultAnimation(m_walk, overlap);
			break;
		case EnemyAnimatorHorizontal::RUN:
			SetDefaultAnimation(m_run, overlap);
			break;
	}
}

void EnemyAnimator::PlayUseDoor()
{
	PlayAnimation(m_usedoor);
}

void EnemyAnimator::PlayTurn()
{
	PlayAnimation(m_turn);
}

void EnemyAnimator::OverlapPlayTurn()
{
	PlayAnimation(m_turn, true);
}

void EnemyAnimator::PlayHurtFly()
{
	PlayAnimation(m_hurtfly);
}

void EnemyAnimator::PlayHurtGround()
{
	PlayAnimation(m_hurtground);
}

void EnemyAnimator::PlayGroggy()
{
	PlayAnimation(m_groggy);
}

void EnemyAnimator::PlayMelee()
{
	PlayAnimation(m_melee);
}

void EnemyAnimator::SetGroundState(bool hasGround)
{
	m_hasGround = hasGround;
}

void EnemyAnimator::SetAimming(bool enable)
{
	if (m_aimming == enable) return;
	m_aimming = enable;
	if (enable)
		m_renderer->GetTransform()->SetLocalPosition(Vec2(0, m_aimAdjustY));
	else if(!enable && GetCurrentAnimation())
		m_renderer->GetTransform()->SetLocalPosition(GetCurrentAnimation()->GetAdjust());
}

void EnemyAnimator::SetAimAngle(float angle)
{
	if (angle < -90) angle = -90;
	if (angle > +90) angle = +90;
	float percent = (angle + 90) / 180.0f;
	float fIndex = percent * (m_aimCount - 1);
	m_aimIndex = size_t(fIndex);
}

bool EnemyAnimator::IsPlayingIdle() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_idle;
}

bool EnemyAnimator::IsPlayingMove() const
{
	Animation* current = GetCurrentAnimation();
	return 
		current == m_walk ||
		current == m_run;
}

bool EnemyAnimator::IsPlayingUseDoor() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_usedoor;
}

bool EnemyAnimator::IsPlayingTurn() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_turn;
}

bool EnemyAnimator::IsPlayingHurt() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_hurtfly ||
		current == m_hurtground;
}

bool EnemyAnimator::IsPlayingHurtGround() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_hurtground;
}

bool EnemyAnimator::IsPlayingGroggy() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_groggy;
}

bool EnemyAnimator::IsPlayingMelee() const
{
	Animation* current = GetCurrentAnimation();
	return
		current == m_melee;
}

bool EnemyAnimator::IsAimming() const
{
	return m_aimming;
}

bool EnemyAnimator::CanMove() const
{
	bool cantmove =
		IsPlayingUseDoor() ||
		IsPlayingTurn() ||
		IsPlayingHurt() ||
		IsPlayingGroggy() ||
		IsPlayingMelee() ||
		IsAimming()
		;
	return !cantmove;
}

bool EnemyAnimator::IsDead() const
{
	return GetCurrentAnimation() == m_hurtground && GetPercent() >= 1.0f;
}
