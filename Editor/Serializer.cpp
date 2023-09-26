#include "editor_stdafx.h"
#include "Serializer.h"

#include "EditorManager.h"
#include "Background.h"
#include "Parallax.h"
#include "EditorBoxCollider.h"
#include "EditorLeftTriangleCollider.h"
#include "EditorRightTriangleCollider.h"
#include "EditorLineCollider.h"
#include "EditorObject.h"
#include "EditorEnemy.h"
#include "TeleportDoor.h"
#include "CameraBorderBox.h"
#include "TriggerBox.h"

#include <fstream>

void Serializer::Serialize(const wstring& path)
{
	std::vector<Json::Value> values;

	auto ToJson = [](IJsonConvert* from) -> Json::Value
	{
		Json::Value item = from->ToJson();
		item["json_type"] = from->JsonType();
		return item;
	};

	values.push_back(ToJson(EditorManager::GetInstance()->GetFrontBackground()));
	values.push_back(ToJson(EditorManager::GetInstance()->GetMiddleBackground()));
	values.push_back(ToJson(EditorManager::GetInstance()->GetBackBackground()));

	values.push_back(ToJson(EditorManager::GetInstance()->GetFrontParallax()));
	values.push_back(ToJson(EditorManager::GetInstance()->GetMiddleParallax()));
	values.push_back(ToJson(EditorManager::GetInstance()->GetBackParallax()));

	values.push_back(ToJson(EditorManager::GetInstance()->GetCameraBorderBox()));

	for (auto& trigger : TriggerBox::triggers)
		values.push_back(ToJson(trigger));

	for (auto& collider : EditorCollider::colliders)
		values.push_back(ToJson(collider));

	for (auto& obj : EditorObject::objects)
		values.push_back(ToJson(obj));

	Json::Value root;
	for (size_t i = 0; i < values.size(); ++i)
		root[int(i)] = values[i];

	USES_CONVERSION;
	string s_path(W2A(path.c_str()));
	std::ofstream ofs;
	ofs.open(s_path);
	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &ofs);
	ofs.close();

	MESSAGE_BOX(L"Save complete");
}

bool Serializer::Deserialize(const wstring& path)
{
	Json::Value root;
	std::ifstream ifs;
	ifs.open(path);

	if (!ifs.is_open())
	{
		MESSAGE_BOX(L"Load failed: file does not exist");
		return false;
	}

	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs))
	{
		ifs.close();
		MESSAGE_BOX(L"Load failed: faield to parse json");
		return false;
	}

	auto to_wstring = [](const string& str)
	{
		USES_CONVERSION;
		return wstring(A2W(str.c_str()));
	};

	EditorManager::GetInstance()->New();

	for (size_t i = 0; i < root.size(); ++i)
	{
		Json::Value item = root[int(i)];

		string json_type = item["json_type"].asString();

		if (json_type == "background")
		{
			string order = item["order"].asString();
			Background* bg = nullptr;
			if (order == "front") bg = EditorManager::GetInstance()->GetFrontBackground();
			if (order == "middle") bg = EditorManager::GetInstance()->GetMiddleBackground();
			if (order == "back") bg = EditorManager::GetInstance()->GetBackBackground();
			bg->SetSprite(to_wstring(item["key"].asString()));
			bg->SetScale(item["scale"].asFloat());
			bg->SetEmptyColor(Color(item["red"].asFloat(), item["green"].asFloat(), item["blue"].asFloat(), 1.0f));
			bg->GetTransform()->SetPosition(Vec2(item["adjust_x"].asFloat(), item["adjust_y"].asFloat()));
		}
		else if (json_type == "box_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			auto c = o->AddComponent<EditorBoxCollider>();
			c->type = (item["collider_type"].asString() == "wall" ? COLLIDER_WALL : COLLIDER_GROUND);
			c->SetSize(item["width"].asInt(), item["height"].asInt());
		}
		else if (json_type == "left_triangle_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			auto c = o->AddComponent<EditorLeftTriangleCollider>();
			c->type = (item["collider_type"].asString() == "wall" ? COLLIDER_WALL : COLLIDER_GROUND);
			c->SetSize(item["width"].asInt(), item["height"].asInt());
		}
		else if (json_type == "line_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			auto c = o->AddComponent<EditorLineCollider>();
			c->type = (item["collider_type"].asString() == "wall" ? COLLIDER_WALL : COLLIDER_GROUND);
			c->SetSize(item["width"].asInt(), item["height"].asInt());
		}
		else if (json_type == "right_triangle_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			auto c = o->AddComponent<EditorRightTriangleCollider>();
			c->type = (item["collider_type"].asString() == "wall" ? COLLIDER_WALL : COLLIDER_GROUND);
			c->SetSize(item["width"].asInt(), item["height"].asInt());
		}
		else if (json_type == "parallax")
		{
			string order = item["order"].asString();
			Parallax* parallax = nullptr;
			if (order == "front") parallax = EditorManager::GetInstance()->GetFrontParallax();
			if (order == "middle") parallax = EditorManager::GetInstance()->GetMiddleParallax();
			if (order == "back") parallax = EditorManager::GetInstance()->GetBackParallax();
			parallax->SetSprite(to_wstring(item["key"].asString()));
			parallax->SetBase(Vec2(item["base_x"].asFloat(), item["base_y"].asFloat()));
			parallax->SetParallaxScale(Vec2(item["parallax_x"].asFloat(), item["parallax_y"].asFloat()));
			parallax->SetScale(item["scale"].asFloat());
		}
		else if (json_type == "camera_border")
		{
			CameraBorderBox* camBorder = EditorManager::GetInstance()->GetCameraBorderBox();
			camBorder->GetTransform()->SetPosition(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			camBorder->SetSize(item["width"].asInt(), item["height"].asInt());
		}
		else if (json_type == "trigger")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			TriggerBox* trigger = o->AddComponent<TriggerBox>();
			trigger->SetSize(item["width"].asInt(), item["height"].asInt());
			trigger->m_name = to_wstring(item["name"].asString());
		}
		else if (json_type == "object")
		{
			EditorObject* o = EditorObject::Create(
				item["key"].asString(),
				item["object_type"].asString(),
				item["sprite"].asString(),
				Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()),
				item["horizontal_direction"].asFloat());
			
			string object_type = item["object_type"].asString();

			if (object_type == "enemy_stand")
			{
				EditorEnemy* e = static_cast<EditorEnemy*>(o);
				e->SetRally(false);
			}
			else if (object_type == "enemy_rally")
			{
				EditorEnemy* e = static_cast<EditorEnemy*>(o);
				e->SetRally(true);
				e->GetRallyPointA()->GetTransform()->SetPosition(Vec2(item["rally_0_x"].asFloat(), item["rally_0_y"].asFloat()));
				e->GetRallyPointB()->GetTransform()->SetPosition(Vec2(item["rally_1_x"].asFloat(), item["rally_1_y"].asFloat()));
			}
			else if (object_type == "teleport_door")
			{
				TeleportDoor* tpdoor = static_cast<TeleportDoor*>(o);
				tpdoor->GetDoorPointA()->GetTransform()->SetPosition(Vec2(item["door_0_x"].asFloat(), item["door_0_y"].asFloat()));
				tpdoor->GetDoorPointB()->GetTransform()->SetPosition(Vec2(item["door_1_x"].asFloat(), item["door_1_y"].asFloat()));
			}
		}
	}

	ifs.close();

	MESSAGE_BOX(L"Load complete");

	return true;
}
