#pragma once

class Animation;

class Animator abstract : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(BeforeRender);

	// 루프 설정이 없는 애니메이션이 끝날 때 호출됩니다.
	// 여기에서 다음 애니메이션을 설정합니다.
	// 만약 다음 애니메이션을 설정하지 않은 경우 기본 애니메이션으로 전환됩니다.
	PROTECTED virtual void OnAnimationEnd(const Animation* current) = 0;

	// PlayAnimation() 함수에 의해 애니메이션이 변경될 때 호출됩니다.
	// PlayAnimation() 함수는 직접 호출 외에도 루프 설정이 없는 애니메이션이 종료될 때
	// 기본 애니메이션으로 전환되면서도 호출됩니다.
	PROTECTED virtual void OnAnimationChange(const Animation* current, Animation** next) = 0;

	PUBLIC void PlayAnimation(Animation* animation, bool overlap = false);
	PUBLIC void SetDefaultAnimation(Animation* defaultAnimation, bool overlap = true);
	PUBLIC Animation* GetCurrentAnimation() const;
	PUBLIC Animation* GetDefaultAnimation() const;
	PUBLIC void AutoConnectToSpriteRenderer();
	PUBLIC void PauseAnimation();
	PUBLIC void ResumeAnimation();
	PUBLIC void PlayDefaultAnimation(bool overlap = false);
	PUBLIC void SetSpeed(float speed);
	PUBLIC float GetSpeed() const;
	PUBLIC float GetElapsed() const;
	PUBLIC float GetPercent() const;
	PUBLIC int GetFrameIndex() const;
	PUBLIC void SetTransition(bool transition);
	PUBLIC void SetAdjust(bool enable);

	PROTECTED SpriteRenderer* m_renderer;
	PRIVATE float m_elapsed;
	PRIVATE Animation* m_current;
	PRIVATE Animation* m_default;
	PRIVATE bool m_pause;
	PRIVATE float m_speed;
	PRIVATE bool m_transition;
	PRIVATE bool m_adjust;
};

