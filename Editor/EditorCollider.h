#pragma once

#include "TransformableRect.h"
#include "IJsonConvert.h"

class EditorCollider : public TransformableRect, public IJsonConvert
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC Json::Value ToJson() const override;
	PROTECTED virtual string JsonType() const override = 0;

	PUBLIC ColliderType type;

	PUBLIC static std::vector<EditorCollider*> colliders;
};

