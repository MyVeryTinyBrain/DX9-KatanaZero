#pragma once

#include "IJsonConvert.h"

class Parallax : public Component, public IJsonConvert
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC virtual Json::Value ToJson() const override;
	PUBLIC virtual string JsonType() const override;

	PUBLIC void SetBase(const Vec2& base);
	PUBLIC void SetParallaxScale(const Vec2& scale);
	PUBLIC void SetScale(float scale);
	PUBLIC void SetSprite(const wstring& key);
	PUBLIC const Vec2& GetBase() const;
	PUBLIC const Vec2& GetParallaxScale() const;
	PUBLIC float GetScale() const;
	PUBLIC const wstring& GetKey() const;

	PUBLIC void SetOrder(int order);

	PRIVATE Vec2 m_base;
	PRIVATE Vec2 m_parallaxScale;
	PRIVATE float m_scale;
	PRIVATE wstring m_key;

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;
};

