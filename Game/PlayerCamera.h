#pragma once

#include "FRect.h"

class PlayerCamera : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static PlayerCamera* GetInstance();
	PUBLIC void Shake(const Vec2& dir, float duration, float power, float shakeCount);
	PUBLIC void Stop();

	PUBLIC Vec2 CalculateFitInBorderPosition(const Vec2& position, const Vec2& size);

	PRIVATE void TrackingMouse();
	PRIVATE void FitInBorder();
	PRIVATE void Shaking();

	PUBLIC FRect m_border;
	PUBLIC bool m_useScaledTimeForShake;

	PRIVATE float m_shakeCounter;
	PRIVATE float m_shakeDuration;
	PRIVATE Vec2 m_shakeDir;
	PRIVATE float m_shakePower;
	PRIVATE float m_shakeCount;

	PRIVATE bool m_stop;

	PRIVATE static PlayerCamera* g_instance;
};

