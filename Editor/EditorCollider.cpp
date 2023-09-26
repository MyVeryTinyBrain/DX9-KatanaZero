#include "editor_stdafx.h"
#include "EditorCollider.h"

std::vector<EditorCollider*> EditorCollider::colliders;

void EditorCollider::Awake()
{
	TransformableRect::Awake();

	type = COLLIDER_GROUND;

	colliders.push_back(this);
}

void EditorCollider::Start()
{
	TransformableRect::Start();

	auto text = AddComponent<TextRenderer>();
	if (type == COLLIDER_WALL) text->SetText(L"Wall");
	if (type == COLLIDER_GROUND) text->SetText(L"Ground");
}

void EditorCollider::Render()
{
	TransformableRect::Render();
}

void EditorCollider::OnDestroy()
{
	TransformableRect::OnDestroy();

	auto it = std::find(colliders.begin(), colliders.end(), this);
	if (it == colliders.end()) return;
	colliders.erase(it);
}

Json::Value EditorCollider::ToJson() const
{
	Json::Value value;
	/*string*/	value["collider_type"] = (type == COLLIDER_WALL ? "wall" : "ground");
	/*float*/	value["position_x"] = GetTransform()->GetPosition().x;
	/*float*/	value["position_y"] = GetTransform()->GetPosition().y;
	/*int*/		value["width"] = GetWidth();
	/*int*/		value["height"] = GetHeight();

	return value;
}
