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

	// 0, 1, 2, 3: ���� ������ ���� ������ ������ ��� ������
	// 4, 5: ���� �����ʿ��� ����
	// 6, 7: ������ ���ʿ��� 240��, 300���� ��� ������
	// 8, 9, 10, 11, 12: ���ʺ��� ���������� �̵��ϸ� �Ʒ��� ��� ������
	// 13: �߰����� 180�� �����ϴ� ������
	// 14: �ٴ� �����ʿ��� ��� ������
	 
	// 15, 16, 17, 18: ���� ������ ���� ������ ������ ��� ������
	// 19, 20, 21, 22: �ٴڿ��� ���� ������ ���� ������ ������ ��� ������
	// 23: �߰����� 180�� �����ϴ� ������
	// 24: �ٴ� ���ʿ��� ��� ������
	// 25: ��

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

