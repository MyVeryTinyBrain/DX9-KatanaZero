#pragma once

#include "SceneChangeEffectDirection.h"

class SceneChangeEffect : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static SceneChangeEffect* GetInstance();
	PUBLIC void SetCounts(size_t xCount, size_t yCount);
	PUBLIC void SetDirection(SceneChangeEffectDirection direction);
	PUBLIC void SetScaleFactor(float factor);

	PUBLIC void FadeIn(float duration, float delay = 0.1f);
	PUBLIC void FadeOut(float duration, float delay = 0.1f);
	PUBLIC void FadeInImediatley();
	PUBLIC void FadeOutImediatley();

	PUBLIC bool IsFading() const;

	PRIVATE int m_xCount;
	PRIVATE int m_yCount;

	PRIVATE bool m_fading;
	PRIVATE float m_duration;
	PRIVATE float m_elapsed;
	PRIVATE float m_delayCounter;
	PRIVATE bool m_fadeIn;

	PRIVATE Color m_color;
	PRIVATE SceneChangeEffectDirection m_direction;
	PRIVATE float m_scaleFactor;

	PRIVATE Sprite* m_changeSprite;
	PRIVATE static SceneChangeEffect* g_instance;

	PUBLIC Delegate<void> OnFadeOut;
	PUBLIC Delegate<void> OnFadeIn;
};

