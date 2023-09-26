#pragma once

#include "Animator.h"

class Animation;

class WallTurrets : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnEnable);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE virtual void OnAnimationEnd(const Animation* current) override;
	PRIVATE virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PRIVATE Transform* CreateTurret(const Vec2& pos);

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Animation* m_wallOpen;
	PRIVATE bool m_opened;
	PRIVATE bool m_ready;
	PRIVATE bool m_enabled;

	PUBLIC static std::vector<WallTurrets*> g_wallTurrets;
};

