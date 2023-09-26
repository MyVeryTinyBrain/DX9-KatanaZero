#include "stdafx.h"
#include "PrefabCreator.h"
#include "GameLayer.h"
#include "DebugDraw.h"
#include "Cursor.h"
#include "TimeController.h"

Camera* PrefabCreator::CreateCamera()
{
	GameObject* cameraObject = new GameObject(Vec2::zero(), 0, Vec2(0.4f, 0.4f));
	//GameObject* cameraObject = new GameObject(Vec2::zero(), 0, Vec2(1,1));
	Camera* camera = cameraObject->AddComponent<Camera>();
	return camera;
}

void PrefabCreator::InitalizePhysics()
{
	Physics::SetGravity(Vec2(0, -1000));
	LayerManager::GetInstance()->Reset();
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_GROUND, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_PLATFORM, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_DOOR, true);
	LayerManager::SetCollision(LAYERINDEX_CHARACTER, LAYERINDEX_TRIGGER, true);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_GROUND, false);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_DOOR, true);
	LayerManager::SetCollision(LAYERINDEX_TRIGGER, LAYERINDEX_TRIGGER, true);
}

Cursor* PrefabCreator::CreateCursor()
{
	GameObject* cursorObject = new GameObject;
	return cursorObject->AddComponent<Cursor>();
}

TimeController* PrefabCreator::CreateTimeController()
{
	GameObject* object = new GameObject;
	return object->AddComponent<TimeController>();
}

BoxCollider* PrefabCreator::CreateGround(const Vec2& position)
{
	GameObject* groundObject = new GameObject(position);
	Body* groundBody = groundObject->AddComponent<Body>();
	groundBody->SetType(BodyType::Static);
	groundBody->SetLayerIndex(0);
	BoxCollider* groundCollider = groundObject->AddComponent<BoxCollider>();
	groundCollider->DrawDebug = DRAW_STAGE;
	return groundCollider;
}

LineCollider* PrefabCreator::CreatePlatform(const Vec2& position)
{
	//GameObject* groundObject = new GameObject(position);
	//Body* groundBody = groundObject->AddComponent<Body>();
	//groundBody->SetType(BodyType::Static);
	//groundBody->SetLayerIndex(1);
	//BoxCollider* groundCollider = groundObject->AddComponent<BoxCollider>();
	//groundCollider->DrawDebug = DRAW_STAGE;
	//return groundCollider;

	GameObject* groundObject = new GameObject(position);
	Body* groundBody = groundObject->AddComponent<Body>();
	groundBody->SetType(BodyType::Static);
	groundBody->SetLayerIndex(1);
	LineCollider* groundCollider = groundObject->AddComponent<LineCollider>();
	groundCollider->DrawDebug = DRAW_STAGE;
	return groundCollider;
}

RightTriangleCollider* PrefabCreator::CreateStairs(const Vec2& position)
{
	GameObject* groundObject = new GameObject(position);
	Body* groundBody = groundObject->AddComponent<Body>();
	groundBody->SetType(BodyType::Static);
	groundBody->SetLayerIndex(0);
	RightTriangleCollider* groundCollider = groundObject->AddComponent<RightTriangleCollider>();
	groundCollider->DrawDebug = DRAW_STAGE;
	return groundCollider;
}
