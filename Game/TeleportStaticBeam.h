#pragma once

#include "Animator.h"

class Animation;

class TeleportStaticBeam : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);

	PRIVATE virtual void OnAnimationEnd(const Animation* current) override;
	PRIVATE virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC void UseDownAnimation();
	PUBLIC void UseRightAnimation();
	PUBLIC void UseLeftAnimation();
	PUBLIC void SetBeamAngle(float angle);
	PUBLIC void SetBeamLockTime(float time);
	PUBLIC void SetBeamDuration(float duration);
	PUBLIC void SetAnimationTime(float time);

	PRIVATE void CreateBeamLock();

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Animation* m_down_in;
	PRIVATE Animation* m_right_in;
	PRIVATE Animation* m_down_out;
	PRIVATE Animation* m_right_out;
	PRIVATE bool m_left;
	PRIVATE Animation* m_use;
	PRIVATE float m_beamAngle;
	PRIVATE float m_beamLockTime;
	PRIVATE float m_beamDuration;
	PRIVATE float m_halfTime;
};

