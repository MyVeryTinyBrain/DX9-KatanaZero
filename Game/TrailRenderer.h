#pragma once

class TrailRenderer : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static TrailRenderer* Create(SpriteRenderer* renderer);
	PUBLIC static TrailRenderer* Create(const Vec2& position, Sprite* sprite, int order);

	PUBLIC void SetSprite(SpriteRenderer* renderer);
	PUBLIC void ResetElapsed();
	PUBLIC void SetDuration(float duration);
	PUBLIC void SetBeginColor(const Color& color);
	PUBLIC void SetEndColor(const Color& color);
	PUBLIC void SetTimeScaleMode(bool enable);
	PUBLIC void SetAdditiveBlending(bool enable);
	PUBLIC void SetRenderOrder(int order);
	PUBLIC void SetScaleMode(bool enable);

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE float m_elapsed;
	PRIVATE float m_duration;
	PRIVATE Color m_begColor;
	PRIVATE Color m_endColor;
	PRIVATE bool m_useTimeScale;
	PRIVATE bool m_useScale;
	PRIVATE Vec2 m_firstScale;
};

