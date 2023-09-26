#include "stdafx.h"
#include "EnemyGrunt.h"
#include "EnemyGruntAnimator.h"

void EnemyGrunt::InitAnimator(EnemyAnimator** pAnimator)
{
	*pAnimator = AddComponent<EnemyGruntAnimator>();
}

void EnemyGrunt::InitSpeed(float* pWalkSpeed, float* pRunSpeed)
{
	*pWalkSpeed = 70;
	*pRunSpeed = 260;
}

void EnemyGrunt::InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait)
{
	*pMeleeKill = true;
	*pHardMelee = false;
	*pMeleeRange = 40;
	*pMeleeWait = 0.25f;
}

void EnemyGrunt::InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange)
{
	*pUse = false;
	*pLongRangeAttackRange = 0;
}

void EnemyGrunt::InitShootDelay(float* pFirstShootDelay, float* pShootDelay)
{
	*pFirstShootDelay = 0;
	*pShootDelay = 0;
}

bool EnemyGrunt::OnDamaged(Vec2 attackDir)
{
	return true;
}
