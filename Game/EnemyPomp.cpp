#include "stdafx.h"
#include "EnemyPomp.h"
#include "EnemyPompAnimator.h"

void EnemyPomp::InitAnimator(EnemyAnimator** pAnimator)
{
    *pAnimator = AddComponent<EnemyPompAnimator>();
}

void EnemyPomp::InitSpeed(float* pWalkSpeed, float* pRunSpeed)
{
    *pWalkSpeed = 77;
    *pRunSpeed = 270;
}

void EnemyPomp::InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait)
{
    *pMeleeKill = true;
    *pHardMelee = false;
    *pMeleeRange = 40;
    *pMeleeWait = 0.25f;
}

void EnemyPomp::InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange)
{
    *pUse = false;
    *pLongRangeAttackRange = 0;
}

void EnemyPomp::InitShootDelay(float* pFirstShootDelay, float* pShootDelay)
{
    *pFirstShootDelay = 0;
    *pShootDelay = 0;
}

bool EnemyPomp::OnDamaged(Vec2 attackDir)
{
    if (GetState() != Enemy::State::TRACE) return true;
    float hd;
    if (attackDir.x < 0) hd = -1;
    else hd = +1;
    if (GetHorizontalDirection() != hd && m_animator->IsPlayingMelee()) return false;
    return true;
}
