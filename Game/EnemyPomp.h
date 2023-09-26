#pragma once

#include "Enemy.h"

class EnemyPomp : public Enemy
{
	virtual void InitAnimator(EnemyAnimator** pAnimator) override;
	virtual void InitSpeed(float* pWalkSpeed, float* pRunSpeed) override;
	virtual void InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait) override;
	virtual void InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange) override;
	virtual void InitShootDelay(float* pFirstShootDelay, float* pShootDelay) override;
	virtual bool OnDamaged(Vec2 attackDir) override;
};

