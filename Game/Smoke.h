#pragma once

#include "Animator.h"

class Animation;

class Smoke : public Animator
{
	virtual void OnAnimationEnd(const Animation* current) override;
	virtual void OnAnimationChange(const Animation* current, Animation** next) override;
};

