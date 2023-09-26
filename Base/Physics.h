#pragma once

#include "Box2dStruct.h"

class b2World;
class b2Body;
class PhysicsDraw;
class Collider;
class ContactListener;
class ContactFilter;

class Physics
{
	DECLARE_SINGLETON(Physics)

	PUBLIC void Step(float dt);
	PUBLIC void Draw();

	PUBLIC static void SetGravity(Vec2 gravity);
	PUBLIC static Vec2 GetGravity();
	PUBLIC static size_t QueryAABB(const AABB& aabb, std::vector<Collider*>* pColliders);
	PUBLIC static size_t QueryAABB(const AABB& aabb, std::vector<Collider*>* pColliders, layer_t layerMask);
	PUBLIC static bool RaycastStraight(const Vec2& p1, const Vec2& p2, RaycastResult* pResult);
	PUBLIC static bool RaycastStraight(const Vec2& p1, const Vec2& p2, RaycastResult* pResult, layer_t layerMask);
	PUBLIC static bool Raycast(const Vec2& p1, const Vec2& p2, RaycastResult* pResult);
	PUBLIC static bool Raycast(const Vec2& p1, const Vec2& p2, RaycastResult* pResult, layer_t layerMask);
	PUBLIC static size_t RaycastAll(const Vec2& p1, const Vec2& p2, std::vector<RaycastResult>* pResults);
	PUBLIC static size_t RaycastAll(const Vec2& p1, const Vec2& p2, std::vector<RaycastResult>* pResults, layer_t layerMask);
	PUBLIC static Collider* OverlapPoint(const Vec2& p);
	PUBLIC static Collider* OverlapPoint(const Vec2& p, layer_t layerMask);
	PUBLIC static size_t OverlapPointAll(const Vec2& p, std::vector<Collider*>* pColliders);
	PUBLIC static size_t OverlapPointAll(const Vec2& p, std::vector<Collider*>* pColliders, layer_t layerMask);

	PUBLIC b2World* GetWorld() const;
	PUBLIC ContactListener* GetContactListener() const;
	PUBLIC ContactFilter* GetContactFilter() const;

	PRIVATE b2World* m_world;
	PRIVATE PhysicsDraw* m_draw;
	PRIVATE ContactListener* m_contactListener;
	PRIVATE ContactFilter* m_contactFilter;
};

