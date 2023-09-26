#pragma once

class StaticBackground : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetSprite(const wstring& key);
	PUBLIC const wstring& GetKey() const;

	PUBLIC void SetScale(float scale);
	PUBLIC float GetScale() const;

	PUBLIC void SetEmptyColor(const Color& color);
	PUBLIC const Color& GetEmptyColor();

	PUBLIC void SetOrder(int order);
	PUBLIC int GetOrder() const;

	PRIVATE float m_scale;
	PRIVATE wstring m_key;
	PRIVATE Color m_emptyColor;

	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;

	PUBLIC static std::vector<StaticBackground*> g_backgrounds;
};

