#pragma once

#include "Enemy.h"

class EnemyShieldCop : public Enemy
{
	virtual void InitAnimator(EnemyAnimator** pAnimator) override;
	virtual void InitSpeed(float* pWalkSpeed, float* pRunSpeed) override;
	PROTECTED virtual void InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait) override;
	PROTECTED virtual void InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange) override;
	PROTECTED virtual void InitShootDelay(float* pFirstShootDelay, float* pShootDelay) override;
	PROTECTED virtual bool OnDamaged(Vec2 attackDir) override;
};

