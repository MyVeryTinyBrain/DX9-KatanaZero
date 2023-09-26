#include "editor_stdafx.h"
#include "EditorObject.h"
#include "EditorEnemy.h"
#include "TeleportDoor.h"

std::vector<EditorObject*> EditorObject::objects;

void EditorObject::Awake()
{
	TransformableRect::Awake();

	m_horizontal_direction = 1;

	m_renderer = AddComponent<SpriteRenderer>();

	m_useLeft = false;
	m_useTop = false;
	m_useRight = false;
	m_useBottom = false;

	objects.push_back(this);
}

void EditorObject::Start()
{
	TransformableRect::Start();
}

void EditorObject::OnDestroy()
{
	TransformableRect::OnDestroy();

	SAFE_DELETE(m_sprite);

	auto it = std::find(objects.begin(), objects.end(), this);
	if (it == objects.end()) return;
	objects.erase(it);
}

Json::Value EditorObject::ToJson() const
{
	USES_CONVERSION;
	string key(W2A(m_key.c_str()));
	string obj_type(W2A(m_object_type.c_str()));
	string spr_key(W2A(m_sprite_key.c_str()));

	Json::Value value;
	/*string*/  value["object_type"] = obj_type;
	/*string*/	value["key"] = key;
	/*float*/	value["position_x"] = GetTransform()->GetPosition().x;
	/*float*/	value["position_y"] = GetTransform()->GetPosition().y;
	/*float*/	value["horizontal_direction"] = m_horizontal_direction;
	/*float*/	value["sprite"] = spr_key;

	return value;
}

string EditorObject::JsonType() const
{
	return "object";
}

void EditorObject::SetSprite(const wstring& key)
{
	SAFE_DELETE(m_sprite);

	m_sprite = new Sprite(key);
	m_renderer->SetSprite(m_sprite);

	m_sprite_key = key;
}

void EditorObject::SetKey(const wstring& key)
{
	m_key = key;
}

void EditorObject::SetHorizontalDirection(float hd)
{
	if (hd < 0) hd = -1;
	else hd = +1;
	m_horizontal_direction = hd;
	GetTransform()->SetScale(Vec2(hd, 1));
}

const wstring& EditorObject::GetKey() const
{
	return m_key;
}

const wstring& EditorObject::GetTypeName() const
{
	return m_object_type;
}

float EditorObject::GetHorizontalDirection() const
{
	return m_horizontal_direction;
}

EditorObject* EditorObject::Create(const Json::Value& json, const SelectParameters& selParam)
{
	string s_key = json["key"].asString();
	string s_object_type = json["object_type"].asString();
	string s_sprite = json["sprite"].asString() + ".png";
	const Vec2& pos = CCamera2D::GetInstance()->GetPosition();

	return Create(s_key, s_object_type, s_sprite, pos, selParam.horizontalDirection);
}

EditorObject* EditorObject::Create(const string& key, const string& object_type, const string& sprite, const Vec2& position, float horizontal_direction)
{
	USES_CONVERSION;
	wstring w_key(A2W(key.c_str()));
	wstring w_object_type(A2W(object_type.c_str()));
	wstring w_sprite(A2W(sprite.c_str()));

	auto obj = new GameObject(position);
	EditorObject* editorObj = nullptr;

	if (w_object_type == L"player")
	{
		auto it = std::find_if(objects.begin(), objects.end(), [&](const EditorObject* o) { return o->m_object_type == L"player"; });
		if (it != objects.end())
		{
			obj->Destroy();
			return nullptr;
		}
		editorObj = obj->AddComponent<EditorObject>();
	}
	else if (w_object_type == L"enemy_stand")
	{
		EditorEnemy* enemy = obj->AddComponent<EditorEnemy>();
		enemy->SetRally(false);
		editorObj = enemy;
	}
	else if (w_object_type == L"enemy_rally")
	{
		EditorEnemy* enemy = obj->AddComponent<EditorEnemy>();
		enemy->SetRally(true);
		editorObj = enemy;
	}
	else if (w_object_type == L"teleport_door")
	{
		TeleportDoor* tpdoor = obj->AddComponent<TeleportDoor>();
		editorObj = tpdoor;
	}
	else
	{
		editorObj = obj->AddComponent<EditorObject>();
	}

	editorObj->SetSprite(w_sprite);
	editorObj->m_key = w_key;
	editorObj->m_object_type = w_object_type;

	editorObj->SetHorizontalDirection(horizontal_direction);

	return editorObj;
}
