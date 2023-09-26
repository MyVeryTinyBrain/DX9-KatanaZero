#include "stdafx.h"
#include "EnemyGangster.h"
#include "EnemyGangsterAnimator.h"

void EnemyGangster::InitAnimator(EnemyAnimator** pAnimator)
{
    *pAnimator = AddComponent<EnemyGangsterAnimator>();
}

void EnemyGangster::InitSpeed(float* pWalkSpeed, float* pRunSpeed)
{
    *pWalkSpeed = 77;
    *pRunSpeed = 265;
}

void EnemyGangster::InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait)
{
    *pMeleeKill = false;
    *pHardMelee = false;
    *pMeleeRange = 40;
    *pMeleeWait = 0.1f;
}

void EnemyGangster::InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange)
{
    *pUse = true;
    *pLongRangeAttackRange = 410;
}

void EnemyGangster::InitShootDelay(float* pFirstShootDelay, float* pShootDelay)
{
    *pFirstShootDelay = 0.2f;
    *pShootDelay = 0.6f;
}

bool EnemyGangster::OnDamaged(Vec2 attackDir)
{
    return true;
}
