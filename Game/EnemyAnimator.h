#pragma once

#include "Animator.h"

class Animation;

enum class EnemyAnimatorHorizontal{NOTHING, WALK, RUN};

class EnemyAnimator : public Animator 
{
	PROTECTED virtual void InitAnimations() = 0;

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(OnDestroy);
	PROTECTED virtual void OnAnimationEnd(const Animation* current) override;
	PROTECTED virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC void SetHorizontal(EnemyAnimatorHorizontal h);
	PUBLIC void PlayUseDoor();
	PUBLIC void PlayTurn();
	PUBLIC void OverlapPlayTurn();
	PUBLIC void PlayHurtFly();
	PUBLIC void PlayHurtGround();
	PUBLIC void PlayGroggy();
	PUBLIC void PlayMelee();
	PUBLIC void SetGroundState(bool hasGround);
	PUBLIC void SetAimming(bool enable);
	PUBLIC void SetAimAngle(float angle);

	PUBLIC bool IsPlayingIdle() const;
	PUBLIC bool IsPlayingMove() const;
	PUBLIC bool IsPlayingUseDoor() const;
	PUBLIC bool IsPlayingTurn() const;
	PUBLIC bool IsPlayingHurt() const;
	PUBLIC bool IsPlayingHurtGround() const;
	PUBLIC bool IsPlayingGroggy() const;
	PUBLIC bool IsPlayingMelee() const;
	PUBLIC bool IsAimming() const;
	PUBLIC bool CanMove() const;
	PUBLIC bool IsDead() const;

	PROTECTED Animation* m_idle;
	PROTECTED Animation* m_walk;
	PROTECTED Animation* m_run;
	PROTECTED Animation* m_usedoor; // it is almost turn animation
	PROTECTED Animation* m_turn;
	PROTECTED Animation* m_hurtfly;
	PROTECTED Animation* m_hurtground;
	PROTECTED Animation* m_groggy; // it is almost play hurtground and reversed hurtground
	PROTECTED Animation* m_melee;
	PROTECTED Sprite* m_aim[19]; // -90 ~ +90
	PROTECTED float m_aimAdjustY;

	PRIVATE EnemyAnimatorHorizontal m_horizontal;
	PRIVATE bool m_hasGround;
	PRIVATE bool m_aimming;
	PRIVATE int m_aimIndex;
	PRIVATE int m_aimCount;
};

