#include "stdafx.h"
#include "EnemyShotgun.h"
#include "EnemyShotgunAnimator.h"

void EnemyShotgun::InitAnimator(EnemyAnimator** pAnimator)
{
	*pAnimator = AddComponent<EnemyShotgunAnimator>();
}

void EnemyShotgun::InitSpeed(float* pWalkSpeed, float* pRunSpeed)
{
	*pWalkSpeed = 70;
	*pRunSpeed = 250;
}

void EnemyShotgun::InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait)
{
	*pMeleeKill = false;
	*pHardMelee = false;
	*pMeleeRange = 0;
	*pMeleeWait = 0;
}

void EnemyShotgun::InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange)
{
	*pUse = true;
	*pLongRangeAttackRange = 275;
	m_bulletCount = 5;
}

void EnemyShotgun::InitShootDelay(float* pFirstShootDelay, float* pShootDelay)
{
	*pFirstShootDelay = 0.6f;
	*pShootDelay = 0.8f;
}

bool EnemyShotgun::OnDamaged(Vec2 attackDir)
{
	return true;
}
