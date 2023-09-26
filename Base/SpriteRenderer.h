#pragma once

#include "Component.h"

class Sprite;

class SpriteRenderer : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetSprite(Sprite* sprite);
	PUBLIC Sprite* GetSprite() const;
	PUBLIC void SetColor(const Color& color);
	PUBLIC const Color& GetColor() const;
	PUBLIC void SetRenderOrder(int order);
	PUBLIC void SetAdditiveBlending(bool enable);
	PUBLIC bool IsAdditiveBlending() const;
	PUBLIC void SetSelfDeleteSprite(bool enable);

	PRIVATE Sprite* m_sprite;
	PRIVATE Color m_color;
	PRIVATE bool m_additiveBlending;
};

