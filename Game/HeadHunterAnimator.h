#pragma once

#include "Animator.h"

class Animation;

class HeadHunterAnimator : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE virtual void OnAnimationEnd(const Animation* current) override;
	PRIVATE virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC void SetGround(bool ground);
	PUBLIC void PlayAimming();
	PUBLIC void PlayStopAimming();
	PUBLIC void SetAimAngle(float angle);
	PUBLIC void PlayHurt();
	PUBLIC void PlayRoll();
	PUBLIC void PlayJump();
	PUBLIC void PlayJumpImmediately();
	PUBLIC void PlayWallGrab();
	PUBLIC void PlayWallJump();
	PUBLIC void PlayGranadeTakeout();
	PUBLIC void PlayGranadeShoot();
	PUBLIC void PlayGranadePutback();
	PUBLIC void PlayDash();
	PUBLIC void PlayDashImmediately();
	PUBLIC void PlayFastDash();
	PUBLIC void SetShouldDie();

	PUBLIC bool IsPlayingIdle() const;
	PUBLIC bool IsPlayingHurt() const;
	PUBLIC bool IsPlayingHurtLand() const;
	PUBLIC bool IsPlayingRoll() const;
	PUBLIC bool IsPlayingJumpReady() const;
	PUBLIC bool IsPlayingJumpLoop() const;
	PUBLIC bool IsPlayingWallGrab() const;
	PUBLIC bool IsPlayingAimming() const;
	PUBLIC bool IsPlayingDashReady() const;
	PUBLIC bool IsPlayingFastDashReady() const;
	PUBLIC bool IsPlayingAnimationAboutJump() const;
	PUBLIC bool IsPlayingWallJump() const;
	PUBLIC bool IsPlayingAnimationAboutGranade() const;
	PUBLIC bool IsPlayingAnimationAboutAimming() const;
	PUBLIC bool IsPlayingJumpLand() const;
	PUBLIC bool IsPlayingDie() const;

	PUBLIC float GetWallJumpPercent() const;

	PRIVATE Animation* m_idle;
	PRIVATE Animation* m_hurtfly_begin;
	PRIVATE Animation* m_hurtfly_loop;
	PRIVATE Animation* m_hurtfly_land;
	PRIVATE Animation* m_jump_ready;
	PRIVATE Animation* m_jump_loop;
	PRIVATE Animation* m_jump_land;
	PRIVATE Animation* m_wallgrab_begin;
	PRIVATE Animation* m_wallgrab_loop;
	PRIVATE Animation* m_wallJump;
	PRIVATE Animation* m_wallJump_loop;;
	PRIVATE Animation* m_roll;
	PRIVATE Animation* m_rifle_aim_begin;
	PRIVATE Animation* m_rifle_aim_loop;
	PRIVATE Animation* m_rifle_aim_end;
	PRIVATE Sprite* m_aim[19];
	PRIVATE Animation* m_granade_begin;
	PRIVATE Animation* m_granade_shoot;
	PRIVATE Animation* m_granade_end;
	PRIVATE Animation* m_dash_ready;
	PRIVATE Animation* m_dash_end;
	PRIVATE Animation* m_dash_fastready;
	PRIVATE Animation* m_die_land;
	PRIVATE Animation* m_die_land_loop;

	PRIVATE bool m_ground;
	PRIVATE int m_aimIndex;
	PRIVATE bool m_readyToGranadeShoot;
	PRIVATE bool m_shouldDie;

	PUBLIC Delegate<void> OnPlayEndHurtFlyLand;
	PUBLIC Delegate<void> OnPlayJump;
	PUBLIC Delegate<void> OnPlayEndRifleReady;
	PUBLIC Delegate<void> OnPlayEndGranadeReady;
	PUBLIC Delegate<void> OnPlayEndGranadeShoot;
	PUBLIC Delegate<void> OnGranadeShootFrame;
	PUBLIC Delegate<void> OnPlayEndDashReady;
	PUBLIC Delegate<void> OnPlayEndRoll;
};

