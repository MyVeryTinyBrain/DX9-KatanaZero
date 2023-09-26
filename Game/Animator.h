#pragma once

class Animation;

class Animator abstract : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(BeforeRender);

	// ���� ������ ���� �ִϸ��̼��� ���� �� ȣ��˴ϴ�.
	// ���⿡�� ���� �ִϸ��̼��� �����մϴ�.
	// ���� ���� �ִϸ��̼��� �������� ���� ��� �⺻ �ִϸ��̼����� ��ȯ�˴ϴ�.
	PROTECTED virtual void OnAnimationEnd(const Animation* current) = 0;

	// PlayAnimation() �Լ��� ���� �ִϸ��̼��� ����� �� ȣ��˴ϴ�.
	// PlayAnimation() �Լ��� ���� ȣ�� �ܿ��� ���� ������ ���� �ִϸ��̼��� ����� ��
	// �⺻ �ִϸ��̼����� ��ȯ�Ǹ鼭�� ȣ��˴ϴ�.
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

