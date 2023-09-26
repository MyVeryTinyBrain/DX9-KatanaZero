#pragma once

#include "Animator.h"

class Animation;

class StaticEffect : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(BeforeRender);
	PROTECTED virtual void OnAnimationEnd(const Animation* current) override;
	PROTECTED virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC static StaticEffect* Create(
		const wstring& animationKey,
		const Vec2& position,
		bool front = false);
	PUBLIC static StaticEffect* Create(
		const wstring& animationKey,
		const Vec2& position,
		const Vec2& direction,
		const Vec2& scale,
		const Vec2& velocity = Vec2::zero(),
		const Vec2& gravity = Vec2::zero(),
		bool autoAngle = false,
		bool front = false);
	PUBLIC static StaticEffect* Create(
		const wstring& animationKey,
		const Vec2& position,
		float angle,
		const Vec2& scale,
		const Vec2& velocity = Vec2::zero(),
		const Vec2& gravity = Vec2::zero(),
		bool autoAngle = false,
		bool front = false);

	PUBLIC void SetAlphaDestroy(bool enable);
	PUBLIC void SetColor(const Color& color);
	PUBLIC void SetVelocity(const Vec2& velocity);
	PUBLIC void SetGravity(const Vec2& gravity);
	PUBLIC void SetUseAutoAngle(bool enable);
	PUBLIC void SetRenderOrder(int order);
	PUBLIC void SetWait(float wait);
	PUBLIC void DontDestroyOnEndOfAnimation();

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Animation* m_animation;
	PRIVATE bool m_alphaDestroy;
	PRIVATE Vec2 m_velocity;
	PRIVATE Vec2 m_gravity;
	PRIVATE bool m_autoAngle;
	PRIVATE float m_wait;
	PRIVATE bool m_played;
	PRIVATE bool m_dontDestroy;
};

