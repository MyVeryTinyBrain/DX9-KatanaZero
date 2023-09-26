#pragma once

class TimeController : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static TimeController* GetInstance();
	PUBLIC void SetScale(float scale);
	PUBLIC void SetSubScale(float scale, float time);
	PUBLIC void Stop();

	PUBLIC float GetCurrentScale() const;
	PUBLIC bool IsSlowMode() const;
	PUBLIC bool IsStop() const;

	PRIVATE static TimeController* g_instance;

	PRIVATE float m_scale;
	PRIVATE float m_currentScale;

	PRIVATE float m_subScaleTimer;
	PRIVATE float m_subScale;
	PRIVATE bool m_stop;

	PRIVATE Sprite* m_sprBlack;
};

