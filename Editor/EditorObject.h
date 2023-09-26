#pragma once

#include "TransformableRect.h"
#include "IJsonConvert.h"

class EditorObject : public TransformableRect, public IJsonConvert
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC virtual Json::Value ToJson() const override;
	PUBLIC virtual string JsonType() const override;

	PUBLIC void SetSprite(const wstring& key);
	PUBLIC void SetKey(const wstring& key);
	PUBLIC void SetHorizontalDirection(float hd);
	
	PUBLIC const wstring& GetKey() const;
	PUBLIC const wstring& GetTypeName() const;
	PUBLIC float GetHorizontalDirection() const;

	PROTECTED wstring m_key;
	PROTECTED wstring m_object_type;
	PROTECTED float m_horizontal_direction;
	PROTECTED wstring m_sprite_key;
	
	PROTECTED Sprite* m_sprite;
	PROTECTED SpriteRenderer* m_renderer;

	PUBLIC static EditorObject* Create(const Json::Value& object_type, const SelectParameters& selParam);
	PUBLIC static EditorObject* Create(
		const string& key,
		const string& object_type,
		const string& sprite,
		const Vec2& position,
		float horizontal_direction);
	PUBLIC static std::vector<EditorObject*> objects;
};

