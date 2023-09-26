#pragma once

#include "Animator.h"

class Animation;

class DownArrow : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	PRIVATE virtual void OnAnimationEnd(const Animation* current) override;
	PRIVATE virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PROTECTED SpriteRenderer* m_renderer;
	PROTECTED float m_elapsed;
	PRIVATE Animation* m_animation;
};

