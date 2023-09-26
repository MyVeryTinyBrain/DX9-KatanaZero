#pragma once

#include "TransformableRect.h"
#include "IJsonConvert.h"

class EditorSprite : public TransformableRect, public IJsonConvert
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(OnDestroy);
	PROTECTED virtual Json::Value ToJson() const override;

	PRIVATE wstring m_key;
	PRIVATE Sprite* m_sprite;
	PRIVATE SpriteRenderer* m_renderer;

	PUBLIC static std::vector<EditorSprite*> sprites;
};

