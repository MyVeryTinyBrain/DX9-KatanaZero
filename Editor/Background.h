#pragma once

#include "IJsonConvert.h"

class Background : public Component, public IJsonConvert
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC virtual Json::Value ToJson() const override;
	PUBLIC virtual string JsonType() const override;

	PUBLIC void SetOrder(int order);
	PUBLIC int GetOrder() const;

	PUBLIC void SetSprite(const wstring& key);
	PUBLIC const wstring& GetKey() const;

	PUBLIC void SetScale(float scale);
	PUBLIC float GetScale() const;
	PUBLIC Vec2 GetScaledSize() const;
	PUBLIC bool HasSprite() const;

	PUBLIC void SetEmptyColor(const Color& color);
	PUBLIC const Color& GetEmptyColor();

	PRIVATE float m_scale;
	PRIVATE wstring m_key;
	PRIVATE Color m_emptyColor;

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;
};

