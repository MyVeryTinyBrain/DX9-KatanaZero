#pragma once

#include "Animator.h"

class Beam;

class BeamLock : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(BeforeRender);
	virtual void OnAnimationEnd(const Animation* current) override;
	virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC void SetLockTime(float time);
	PUBLIC void SetLockWidth(float width);
	PUBLIC void SetBeamDuration(float duration);
	PUBLIC void SetBeamLength(float length);
	PUBLIC void SetBeamWidth(float width);
	PUBLIC void Cancel();

	PRIVATE Animation* m_animation;
	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE float m_lockWidth;
	PRIVATE float m_beamDuration;
	PRIVATE float m_beamLength;
	PRIVATE float m_beamWidth;
	PRIVATE bool m_cancel;

	PUBLIC Delegate<void, Beam*> OnCreatedBeam;
};

