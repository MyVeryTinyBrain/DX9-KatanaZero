#pragma once

#include "Animator.h"

class Animation;

class PlayerAnimator : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PROTECTED virtual void OnAnimationEnd(const Animation* current) override;
	PROTECTED virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC void SetHorizontalControl(bool enable);
	PUBLIC void SetGroundState(bool hasGround);
	PUBLIC void SetVerticalVelocity(float verticalVelocity);
	PUBLIC void PlayAttack();
	PUBLIC void PlayRoll();
	PUBLIC void SetCrouch(bool enable);
	PUBLIC void PlayFlip();
	PUBLIC void SetWallGrab(bool enable);
	PUBLIC void PlayHurt(bool canWake);
	PUBLIC void PlayHurtCover();
	PUBLIC void PlayDoorOpen();

	PUBLIC bool IsPlayingHorizontalMoveOnGround() const;
	PUBLIC bool IsPlayingToIdle() const;
	PUBLIC bool IsPlayingToRun() const;
	PUBLIC bool IsPlayingAttack() const;
	PUBLIC bool IsPlayingRoll() const;
	PUBLIC bool IsPlayingWallGrab() const;
	PUBLIC bool IsPlayingFlip() const;
	PUBLIC bool IsPlayingHurt() const;
	PUBLIC bool IsPlayingHurtFly() const;
	PUBLIC bool IsPlayingHurtGroundLoop() const;
	PUBLIC bool IsPlayingDoorOpen() const;
	PUBLIC bool CanMove() const;
	PUBLIC bool CanAttack() const;
	PUBLIC bool GetGroundState() const;
	PUBLIC bool GetHorizontaControlState() const;
	PUBLIC bool IsCrouch() const;

	PRIVATE Animation* m_idle;
	PRIVATE Animation* m_idle_to_run;
	PRIVATE Animation* m_run;
	PRIVATE Animation* m_run_to_idle;
	PRIVATE Animation* m_jump;
	PRIVATE Animation* m_fall;
	PRIVATE Animation* m_attack;
	PRIVATE Animation* m_roll;
	PRIVATE Animation* m_crouch_pre;
	PRIVATE Animation* m_crouch;
	PRIVATE Animation* m_crouch_post;
	PRIVATE Animation* m_crouch_walk;
	PRIVATE Animation* m_wallgrab;
	PRIVATE Animation* m_flip;
	PRIVATE Animation* m_hurtfly_begin;
	PRIVATE Animation* m_hurtfly_loop;
	PRIVATE Animation* m_hurtground_begin;
	PRIVATE Animation* m_hurtground_loop;
	PRIVATE Animation* m_hurtground_cover;
	PRIVATE Animation* m_dooropen;

	PRIVATE bool m_horizontalControl;
	PRIVATE bool m_hasGround;
	PRIVATE float m_verticalVelocity;
	PRIVATE bool m_crouching;
	PRIVATE bool m_wallGrabbing;
	PRIVATE bool m_canWake;

	PUBLIC Delegate<void> OnEndPlayHurtflyGround;
};

