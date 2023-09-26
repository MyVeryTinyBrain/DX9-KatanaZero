#pragma once

class StaticSprite : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetSprite(const wstring& textureKey);
	PUBLIC SpriteRenderer* GetRenderer() const;

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;
};

