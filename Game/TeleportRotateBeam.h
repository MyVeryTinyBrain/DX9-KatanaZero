#pragma once

#include "Animator.h"

class Animation;
class Beam;

class TeleportRotateBeam : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE virtual void OnAnimationEnd(const Animation* current) override;
	PRIVATE virtual void OnAnimationChange(const Animation* current, Animation** next) override;
	PRIVATE void OnCreatedBeam(Beam* beam);
	PRIVATE void OnDestroyBeam(Object* object);

	PUBLIC void SetFromDirection(float hd);
	PUBLIC void SetAngleRange(float angleRange);
	PUBLIC void SetLockTime(float time);
	PUBLIC void SetSweepTime(float time);

	PRIVATE float GetStartAngle() const;
	PRIVATE float GetEndAngle() const;
	PRIVATE Vec2 GetBeginPosition(float angle);
	PRIVATE float GetStartPercent() const;
	PRIVATE float GetEndPercent() const;
	PRIVATE float GetCurrentAngle() const;

	PRIVATE void PlaySweep();

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Animation* m_rifle_sweep_teleport_in;
	PRIVATE Animation* m_rifle_sweep_teleport_in_wait;
	PRIVATE Animation* m_rifle_sweep;
	PRIVATE Animation* m_rifle_sweep_teleport_out;

	PRIVATE Beam* m_beam;

	PRIVATE float m_fromDirection;
	PRIVATE float m_angleRange;
	PRIVATE float m_beginLength;

	PRIVATE float m_lockTime;
	PRIVATE float m_sweepTime;
};

