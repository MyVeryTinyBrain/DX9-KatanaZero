#pragma once

#include "IRecorderbleRendererCallback.h"

class ParallaxBackground : public Component, public IRecorderbleRendererCallback
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetBase(const Vec2& base);
	PUBLIC void SetParallaxScale(const Vec2& scale);
	PUBLIC void SetScale(float scale);
	PUBLIC void SetSprite(const wstring& key);
	PUBLIC const Vec2& GetBase() const;
	PUBLIC const Vec2& GetParallaxScale() const;
	PUBLIC float GetScale() const;
	PUBLIC const wstring& GetKey() const;

	PUBLIC Vec2 GetParallaxPosition(const Vec2& camPos);

	PUBLIC void SetOrder(int order);

	PRIVATE Vec2 m_base;
	PRIVATE Vec2 m_parallaxScale;
	PRIVATE float m_scale;
	PRIVATE wstring m_key;

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;

	PRIVATE virtual Vec2 GetRecordPosition() override;
	PRIVATE virtual float GetRecordAngle() override;
	PRIVATE virtual Vec2 GetRecordScale() override;
};

