#include "editor_stdafx.h"
#include "EditorSprite.h"

std::vector<EditorSprite*> EditorSprite::sprites;

void EditorSprite::Awake()
{
	TransformableRect::Awake();

	sprites.push_back(this);
}

void EditorSprite::Start()
{
	TransformableRect::Start();
}

void EditorSprite::OnDestroy()
{
	TransformableRect::OnDestroy();

	auto it = std::find(sprites.begin(), sprites.end(), this);
	if (it == sprites.end()) return;
	sprites.erase(it);
}

Json::Value EditorSprite::ToJson() const
{
	USES_CONVERSION;
	string key(W2A(m_key.c_str()));

	Json::Value value;
	/*string*/	value["type_name"] = "sprite";
	/*string*/	value["key"] = key;
	/*float*/	value["position_x"] = GetTransform()->GetPosition().x;
	/*float*/	value["position_y"] = GetTransform()->GetPosition().y;
	/*float*/	value["scale_x"] = GetTransform()->GetScale().x;
	/*float*/	value["scale_y"] = GetTransform()->GetScale().y;

	return value;
}
