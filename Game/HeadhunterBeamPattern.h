#pragma once

class TeleportStaticBeam;

class HeadhunterBeamPattern : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);

	PRIVATE void Step0();
	PRIVATE void Step1();
	PRIVATE void Step2();
	PRIVATE void Step3();

	PRIVATE void Step4();
	PRIVATE void Step5();

	PRIVATE void Step6();
	PRIVATE void Step7();

	PRIVATE void Step8();
	PRIVATE void Step9();
	PRIVATE void Step10();
	PRIVATE void Step11();
	PRIVATE void Step12();

	PRIVATE void Step13();

	PRIVATE void Step14();

	PRIVATE void Step15();
	PRIVATE void Step16();
	PRIVATE void Step17();
	PRIVATE void Step18();

	PRIVATE void Step19();
	PRIVATE void Step20();
	PRIVATE void Step21();
	PRIVATE void Step22();

	PRIVATE void Step23();

	PRIVATE void Step24();

	PRIVATE void StepLast();

	PRIVATE TeleportStaticBeam* CreateDownBeam(const Vec2& p);
	PRIVATE void CreateSweepFromRightToLeft(const Vec2& p);
	PRIVATE void CreateSweepFromLeftToRight(const Vec2& p);
	PRIVATE TeleportStaticBeam* CreateBeamFromLeftToRight(const Vec2& p);
	PRIVATE TeleportStaticBeam* CreateBeamFromRightToLeft(const Vec2& p);

	// 0, 1, 2, 3: 왼쪽 오른쪽 왼쪽 오른쪽 순서로 쏘는 레이저
	// 4, 5: 왼쪽 오른쪽에서 스윕
	// 6, 7: 오른쪽 왼쪽에서 240도, 300도로 쏘는 레이저
	// 8, 9, 10, 11, 12: 왼쪽부터 오른쪽으로 이동하며 아래로 쏘는 레이저
	// 13: 중간에서 180도 스윕하는 레이저
	// 14: 바닥 오른쪽에서 쏘는 레이저
	 
	// 15, 16, 17, 18: 왼쪽 오른쪽 왼쪽 오른쪽 순서로 쏘는 레이저
	// 19, 20, 21, 22: 바닥에서 왼쪽 오른쪽 왼쪽 오른쏙 순서로 쏘는 레이저
	// 23: 중간에서 180도 스윕하는 레이저
	// 24: 바닥 왼쪽에서 쏘는 레이저
	// 25: 끝

	PRIVATE enum { STEP_MAX = 26 };
	PRIVATE std::function<void()> m_steps[STEP_MAX];
	PRIVATE float m_stepTime[STEP_MAX] = {
		0.6f, 0.25f, 0.25f, 0.25f, // 0~3
		0.5f, 1.0f, // 4~5
		0.5f, 0.2f, // 6~7
		0.2f, 0.2f, 0.2f, 0.2f, 0.2f, // 8~12
		0.4f, // 13
		0.6f, // 14

		0.4f, 0.25f, 0.25f, 0.25f, // 15~18
		0.2f, 0.2f, 0.2f, 0.2f, // 19~22
		0.7f, // 23
		0.6f, // 24

		0.8f, // 25: end
	};
	PRIVATE enum { POINT_MAX = 18 };
	PRIVATE Vec2 m_points[POINT_MAX];

	PRIVATE int m_step;
	PRIVATE float m_elapsed;

	PUBLIC Delegate<void> OnEndPattern;
};

