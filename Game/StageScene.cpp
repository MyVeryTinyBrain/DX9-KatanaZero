#include "stdafx.h"
#include "StageScene.h"

#include "UI.h"
#include "TimeController.h"
#include "SceneChangeEffect.h"

#include "DebugDraw.h"
#include "GameRenderOrder.h"
#include "GameLayer.h"
#include "StageMap.h"

#include "StaticBackground.h"
#include "ParallaxBackground.h"

#include "PlayerController.h"
#include "Door.h"
#include "StaticSprite.h"

#include "Enemy.h"
#include "EnemyGangster.h"
#include "EnemyGrunt.h"
#include "EnemyPomp.h"
#include "EnemyShieldCop.h"
#include "EnemyShotgun.h"

#include "PlayerCamera.h"
#include "Trigger.h"
#include "NextSceneTriggerCallback.h"

#include "Boss.h"
#include "Headhunter.h"
#include "Turret.h"
#include "WallTurrets.h"

#include "Recorder.h"

#include "SojuBottle.h"
#include "BearBottle.h"
#include "KitchenKnife.h"
#include "ExplosiveBottle.h"
#include "Bust.h"
#include "Lamp.h"
#include "SmokeGranade.h"

#include "ExplosiveBarrel.h"

#include "BreakablePlatform.h"

#include "EndArrow.h"

#include <fstream>
#include <atlconv.h>

void StageScene::OnLoad(Scene* prevScene)
{
	auto cameraObject = new GameObject;
	PlayerCamera* playerCam = cameraObject->AddComponent<PlayerCamera>();

	Physics::SetGravity(Vec2(0, -1000));
	LayerManager::GetInstance()->Reset();
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_GROUND, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_PLATFORM, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_DOOR, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_BREAKABLE_PLATFORM, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_TRIGGER, true);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_GROUND, true);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_DOOR, true);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_BREAKABLE_PLATFORM, true);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_TRIGGER, true);
	LayerManager::SetCollision(LAYERINDEX_GROUND_OBJECT, LAYERINDEX_GROUND, true);
	LayerManager::SetCollision(LAYERINDEX_GROUND_OBJECT, LAYERINDEX_GROUND_OBJECT, false);

	Time::SetTimeScale(1.0f);
	Time::SetFixedTimeScale(1.0f);

	auto uiObj = new GameObject;
	uiObj->AddComponent<UI>();

	auto timeControllerObj = new GameObject;
	timeControllerObj->AddComponent<TimeController>();

	auto sceneChangeEffObj = new GameObject;
	sceneChangeEffObj->AddComponent<SceneChangeEffect>();
	SceneChangeEffect::GetInstance()->OnFadeIn += Function<void>(this, &StageScene::OnFadeIn);
	SceneChangeEffect::GetInstance()->OnFadeOut += Function<void>(this, &StageScene::OnFadeOut);
	if (m_useFadeOutEffect)
	{
		SceneChangeEffect::GetInstance()->SetDirection(m_fadeOutDirection);
		SceneChangeEffect::GetInstance()->FadeOut(m_fadeOutDuration, m_fadeOutDelay);
	}

	auto recorderObj = new GameObject;
	recorderObj->AddComponent<Recorder>();

	auto endArrowObj = new GameObject;
	endArrowObj->AddComponent<EndArrow>();

	StageMap::GetInstance()->Clear();

	LoadStageJson(StagePath());

	auto endTrigger = Trigger::FindWithName(L"End");
	if (endTrigger)
	{
		endTrigger->SetCallback(new NextSceneTriggerCallback);
	}
}

void StageScene::OnChange(Scene* nextScene)
{
	SceneChangeEffect::GetInstance()->OnFadeIn -= Function<void>(this, &StageScene::OnFadeIn);
	SceneChangeEffect::GetInstance()->OnFadeOut -= Function<void>(this, &StageScene::OnFadeOut);
}

void StageScene::OnFadeIn()
{
	if (m_usePlayback)
	{
		SceneChangeEffect::GetInstance()->FadeOut(m_fadeOutDuration, m_fadeOutDelay);
		Recorder::GetInstance()->Play();
		Recorder::GetInstance()->SetCanChangeNextSceneWhenPlay(true);
		m_usePlayback = false;
		m_useFadeInEffect = true;
	}
	else
	{
		SceneManager::ChangeScene(NextScene());
	}
}

void StageScene::OnFadeOut()
{
}

void StageScene::LoadStageJson(const wstring& path)
{
	Json::Value root;
	std::ifstream ifs;
	ifs.open(path);

	if (!ifs.is_open())
	{
		MESSAGE_BOX(L"Stage open failed: file does not exist");
		return;
	}

	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs))
	{
		ifs.close();
		MESSAGE_BOX(L"Stage open failed: faield to parse json");
		return;
	}

	ifs.close();

	auto to_wstring = [](const string& str)
	{
		USES_CONVERSION;
		return wstring(A2W(str.c_str()));
	};

	std::vector<Json::Value> teleport_doors;
	std::vector<Json::Value> enemy_stands;
	std::vector<Json::Value> enemy_rallies;

	for (size_t i = 0; i < root.size(); ++i)
	{
		Json::Value item = root[int(i)];

		string json_type = item["json_type"].asString();

		if (json_type == "background")
		{
			string order = item["order"].asString();

			auto o = new GameObject;

			auto b = o->AddComponent<StaticBackground>();
			b->SetScale(item["scale"].asFloat());
			b->SetSprite(to_wstring(item["key"].asString()));
			b->SetEmptyColor(Color(item["red"].asFloat(), item["green"].asFloat(), item["blue"].asFloat(), 1.0f));
			b->GetTransform()->SetPosition(Vec2(item["adjust_x"].asFloat(), item["adjust_y"].asFloat()));
			if (order == "front") b->SetOrder(RENDER_ORDER_BG_FRONT);
			else if (order == "middle") b->SetOrder(RENDER_ORDER_BG_MIDDLE);
			else if (order == "back") b->SetOrder(RENDER_ORDER_BG_BACK);
		}
		else if (json_type == "box_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));

			auto b = o->AddComponent<Body>();
			b->SetType(BodyType::Static);
			b->SetLayerIndex(LAYERINDEX_GROUND);

			auto c = o->AddComponent<BoxCollider>();
			c->SetWidth(item["width"].asInt());
			c->SetHeight(item["height"].asInt());
			c->SetFriction(0.5f);
			c->DrawDebug = DebugDrawTrue;

			if (item["collider_type"].asString() == "ground") StageMap::GetInstance()->InsertGroundCollider(c);
		}
		else if (json_type == "left_triangle_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()), 0.0f, Vec2(-1, 1));

			auto b = o->AddComponent<Body>();
			b->SetType(BodyType::Static);
			b->SetLayerIndex(LAYERINDEX_GROUND);

			auto c = o->AddComponent<TriangleCollider>();
			c->SetWidth(item["width"].asInt());
			c->SetHeight(item["height"].asInt());
			c->SetFriction(0.5f);
			c->DrawDebug = DebugDrawTrue;

			if (item["collider_type"].asString() == "ground") StageMap::GetInstance()->InsertStairCollider(c);
		}
		else if (json_type == "line_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));

			auto b = o->AddComponent<Body>();
			b->SetType(BodyType::Static);
			b->SetLayerIndex(LAYERINDEX_PLATFORM);

			auto c = o->AddComponent<LineCollider>();
			c->SetWidth(item["width"].asInt());
			c->SetFriction(0.5f);
			c->DrawDebug = DebugDrawTrue;

			if (item["collider_type"].asString() == "ground") StageMap::GetInstance()->InsertPlatformCollider(c);
		}
		else if (json_type == "right_triangle_collider")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));

			auto b = o->AddComponent<Body>();
			b->SetType(BodyType::Static);
			b->SetLayerIndex(LAYERINDEX_GROUND);

			auto c = o->AddComponent<TriangleCollider>();
			c->SetWidth(item["width"].asInt());
			c->SetHeight(item["height"].asInt());
			c->SetFriction(0.5f);
			c->DrawDebug = DebugDrawTrue;

			if (item["collider_type"].asString() == "ground") StageMap::GetInstance()->InsertStairCollider(c);
		}
		else if (json_type == "parallax")
		{
			string order = item["order"].asString();

			auto o = new GameObject;

			auto parallax = o->AddComponent<ParallaxBackground>();
			parallax->SetSprite(to_wstring(item["key"].asString()));
			parallax->SetBase(Vec2(item["base_x"].asFloat(), item["base_y"].asFloat()));
			parallax->SetParallaxScale(Vec2(item["parallax_x"].asFloat(), item["parallax_y"].asFloat()));
			parallax->SetScale(item["scale"].asFloat());
			if (order == "front") parallax->SetOrder(RENDER_ORDER_PARALLAX_FRONT);
			else if (order == "middle") parallax->SetOrder(RENDER_ORDER_PARALLAX_MIDDLE);
			else if (order == "back") parallax->SetOrder(RENDER_ORDER_PARALLAX_BACK);
		}
		else if (json_type == "camera_border")
		{
			auto playerCamera = PlayerCamera::GetInstance();
			Vec2 borderCenter = Vec2(item["position_x"].asFloat(), item["position_y"].asFloat());
			Vec2 size = Vec2(item["width"].asFloat(), item["height"].asFloat());
			Vec2 hs = size * 0.5f;
			float left = borderCenter.x - hs.x;
			float right = borderCenter.x + hs.x;
			float bottom = borderCenter.y - hs.y;
			float top = borderCenter.y + hs.y;
			playerCamera->m_border = FRect(left, top, right, bottom);
		}
		else if (json_type == "trigger")
		{
			auto o = new GameObject(Vec2(item["position_x"].asFloat(), item["position_y"].asFloat()));
			o->SetName(to_wstring(item["name"].asString()));
			auto trigger = o->AddComponent<Trigger>();
			trigger->SetSize(item["width"].asFloat(), item["height"].asFloat());
		}
		else if (json_type == "object")
		{
			Vec2 pos = Vec2(item["position_x"].asFloat(), item["position_y"].asFloat());
			float hd = item["horizontal_direction"].asFloat();
			string key = item["key"].asString();
			string object_type = item["object_type"].asString();

			if (object_type == "player")
			{
				auto o = new GameObject(pos);

				auto player = o->AddComponent<PlayerController>();
				player->SetDirection(hd);
			}
			else if (object_type == "door")
			{
				auto o = new GameObject(pos, 0.0f, Vec2(hd, 1.0f));

				auto door = o->AddComponent<Door>();
				if (key == "door_club") door->SetType(Door::Type::CLUB);
				else if (key == "door_door") door->SetType(Door::Type::DOOR);
				else if (key == "door_iron") door->SetType(Door::Type::IRON);
				else if (key == "door_mansion") door->SetType(Door::Type::MANSION);
				else continue;
			}
			else if (object_type == "teleport_door")
			{
				teleport_doors.push_back(item);
			}
			else if (object_type == "enemy_stand")
			{
				enemy_stands.push_back(item);
			}
			else if (object_type == "enemy_rally")
			{
				enemy_rallies.push_back(item);
			}
			else if (object_type == "boss")
			{
				auto o = new GameObject(pos, 0.0f);
				Boss* boss = nullptr;

				if (key == "headhunter")
				{
					auto headHunter = o->AddComponent<Headhunter>();
					boss = headHunter;
				}

				if (boss)
				{
					boss->SetDirection(hd);
				}
			}
			else if (object_type == "turret")
			{
				auto o = new GameObject(pos, 0, Vec2(hd, 1));
				Turret* turret = o->AddComponent<Turret>();
			}
			else if (object_type == "wall_turret")
			{
				auto o = new GameObject(pos, 0, Vec2(hd, 1));
				WallTurrets* turret = o->AddComponent<WallTurrets>();
			}
			else if (object_type == "throw_object")
			{
				auto o = new GameObject(pos, 0, Vec2(hd, 1));
				if (key == "soju_bottle") o->AddComponent<SojuBottle>();
				else if (key == "bear_bottle") o->AddComponent<BearBottle>();
				else if (key == "kitchen_knife_down") o->AddComponent<KitchenKnife>();
				else if (key == "kitchen_knife_up")
				{
					o->AddComponent<KitchenKnife>();
					o->GetTransform()->SetScale(Vec2(hd, -1));
				}
				else if (key == "kitchen_knife_right")
				{
					o->AddComponent<KitchenKnife>();
					if (hd > 0) o->GetTransform()->SetAngle(90);
					else o->GetTransform()->SetAngle(-90);
				}
				else if (key == "explosive_bottle") o->AddComponent<ExplosiveBottle>();
				else if (key == "bust") o->AddComponent<Bust>();
				else if (key == "lamp") o->AddComponent<Lamp>();
				else if (key == "smoke_granade") o->AddComponent<SmokeGranade>();
			}
			else if (object_type == "breakable_platform")
			{
				auto o = new GameObject(pos, 0, Vec2(hd, 1));
				o->AddComponent<BreakablePlatform>();
			}
			else if (object_type == "explosive_barrel")
			{
				auto o = new GameObject(pos, 0, Vec2(hd, 1));
				o->AddComponent<ExplosiveBarrel>();
			}
		}
	}

	for (auto& item : teleport_doors)
	{
		string key = item["key"].asString();

		Vec2 pos0 = Vec2(item["door_0_x"].asFloat(), item["door_0_y"].asFloat());
		Vec2 pos1 = Vec2(item["door_1_x"].asFloat(), item["door_1_y"].asFloat());
		auto o0 = new GameObject(pos0);
		auto o1 = new GameObject(pos1);

		wstring doorSprite;
		if (key == "teleport_door_club") doorSprite = L"../Texture/object/used/teleport_door/teleport_door_club.png";
		else if (key == "teleport_door_door") doorSprite = L"../Texture/object/used/teleport_door/teleport_door_door.png";
		else if (key == "teleport_door_iron") doorSprite = L"../Texture/object/used/teleport_door/teleport_door_iron.png";
		else if (key == "teleport_door_mansion") doorSprite = L"../Texture/object/used/teleport_door/teleport_door_mansion.png";
		else continue;
		auto doorRender0 = o0->AddComponent<StaticSprite>();
		auto doorRender1 = o1->AddComponent<StaticSprite>();
		doorRender0->SetSprite(doorSprite);
		doorRender1->SetSprite(doorSprite);

		StageMap::GetInstance()->InsertDoorPair(pos0, pos1);
	}

	StageMap::GetInstance()->BuildEdges();

	for (auto& item : enemy_stands)
	{
		Vec2 pos = Vec2(item["position_x"].asFloat(), item["position_y"].asFloat());
		float hd = item["horizontal_direction"].asFloat();
		string key = item["key"].asString();
		auto o = new GameObject(pos);

		Enemy* enemy = nullptr;
		if (key == "gangster") enemy = o->AddComponent<EnemyGangster>();
		else if (key == "grunt") enemy = o->AddComponent<EnemyGrunt>();
		else if (key == "pomp") enemy = o->AddComponent<EnemyPomp>();
		else if (key == "shieldcop") enemy = o->AddComponent<EnemyShieldCop>();
		else if (key == "shotgun") enemy = o->AddComponent<EnemyShotgun>();
		else continue;

		enemy->SetHorizontalDirection(hd);
		enemy->SetStandingPoint(pos);
	}

	for (auto& item : enemy_rallies)
	{
		Vec2 pos = Vec2(item["position_x"].asFloat(), item["position_y"].asFloat());
		float hd = item["horizontal_direction"].asFloat();
		string key = item["key"].asString();
		auto o = new GameObject(pos);

		Enemy* enemy = nullptr;
		if (key == "gangster") enemy = o->AddComponent<EnemyGangster>();
		else if (key == "grunt") enemy = o->AddComponent<EnemyGrunt>();
		else if (key == "pomp") enemy = o->AddComponent<EnemyPomp>();
		else if (key == "shieldcop") enemy = o->AddComponent<EnemyShieldCop>();
		else if (key == "shotgun") enemy = o->AddComponent<EnemyShotgun>();
		else continue;

		Vec2 rally0 = Vec2(item["rally_0_x"].asFloat(), item["rally_0_y"].asFloat());
		Vec2 rally1 = Vec2(item["rally_1_x"].asFloat(), item["rally_1_y"].asFloat());
		enemy->SetRallyPoints(rally0, rally1);
	}
}

StageScene::StageScene()
{
	m_fadeOutDelay = 0.1f;
	m_fadeInDelay = 0.0f;
	m_fadeOutDuration = 0.5f;
	m_fadeInDuration = 0.5f;

	UseFadeInEffect(SceneChangeEffectDirection::LEFT);
	UseFadeOutEffect(SceneChangeEffectDirection::RIGHT);
}

void StageScene::ChangeToNextScene()
{
	if (m_useFadeInEffect)
	{
		SceneChangeEffect::GetInstance()->FadeIn(m_fadeInDuration, m_fadeInDelay);
	}
	else
	{
		OnFadeIn();
	}
}

void StageScene::UseFadeOutEffect(const SceneChangeEffectDirection& direction)
{
	m_fadeOutDirection = direction;
	m_useFadeOutEffect = true;
}

void StageScene::UseFadeInEffect(const SceneChangeEffectDirection& direction)
{
	m_fadeInDirection = direction;
	m_useFadeInEffect = true;
}

void StageScene::DontUseFadeOutEffect()
{
	m_useFadeOutEffect = false;
}

void StageScene::DontUseFadeInEffect()
{
	m_useFadeInEffect = false;
}

void StageScene::UsePlayback()
{
	m_usePlayback = true;
}

void StageScene::DontUsePlayback()
{
	m_usePlayback = false;
}
