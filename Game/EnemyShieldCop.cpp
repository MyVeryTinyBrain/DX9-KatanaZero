#include "stdafx.h"
#include "EnemyShieldCop.h"
#include "EnemyShieldCopAnimator.h"

void EnemyShieldCop::InitAnimator(EnemyAnimator** pAnimator)
{
	*pAnimator = AddComponent<EnemyShieldCopAnimator>();
}

void EnemyShieldCop::InitSpeed(float* pWalkSpeed, float* pRunSpeed)
{
	*pWalkSpeed = 70;
	*pRunSpeed = 250;
}

void EnemyShieldCop::InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait)
{
	*pMeleeKill = false;
	*pHardMelee = true;
	*pMeleeRange = 40;
	*pMeleeWait = 0.1f;
}

void EnemyShieldCop::InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange)
{
	*pUse = true;
	*pLongRangeAttackRange = 250;
}

void EnemyShieldCop::InitShootDelay(float* pFirstShootDelay, float* pShootDelay)
{
	*pFirstShootDelay = 0.6f;
	*pShootDelay = 0.8f;
}

bool EnemyShieldCop::OnDamaged(Vec2 attackDir)
{
	if (GetState() != Enemy::State::TRACE) return true;

	float hd;
	if (attackDir.x < 0) hd = -1;
	else hd = +1;
	if (m_animator->IsPlayingTurn()) true;
	else if(GetHorizontalDirection() != hd) return false;
	return true;
}
