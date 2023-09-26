#pragma once

class HeadhunterTurretPattern : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);

	PRIVATE void Step0();
	PRIVATE void Step1();
	PRIVATE void StepLast();

	PRIVATE void CreateSweepFromRightToLeft(const Vec2& p);
	PRIVATE void CreateSweepFromLeftToRight(const Vec2& p);

	PRIVATE enum { STEP_MAX = 3 };
	PRIVATE std::function<void()> m_steps[STEP_MAX];
	PRIVATE float m_stepTime[STEP_MAX] = {
		0.6f,
		2.5f,
		1.05f
	};
	PRIVATE Vec2 m_air;

	PRIVATE int m_step;
	PRIVATE float m_elapsed;

	PUBLIC Delegate<void> OnEndPattern;
};

