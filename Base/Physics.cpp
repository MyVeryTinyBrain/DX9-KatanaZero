#include "base_stdafx.h"
#include "Physics.h"
#include "Box2dHeader.h"
#include "PhysicsDraw.h"
#include "RaycastCallback.h"
#include "RaycastAllCallback.h"
#include "AABBQueryCallback.h"
#include "Collider.h"
#include "ContactListener.h"
#include "ContactFilter.h"

#define INST (g_instance)

IMPLEMENT_SINGLETON(Physics)

IMPLEMENT_SINGLETON_CONSTRUCTOR(Physics)
{
	m_world = new b2World(b2Vec2(0, -9.81f));
	//m_world->SetAutoClearForces(true);
	//m_world->SetContinuousPhysics(true);
	//m_world->SetSubStepping(false);
	//m_world->SetWarmStarting(true);

	m_draw = new PhysicsDraw;
	m_draw->SetFlags(b2Draw::e_shapeBit);
	m_world->SetDebugDraw(m_draw);

	m_contactListener = new ContactListener;
	m_world->SetContactListener(m_contactListener);

	m_contactFilter = new ContactFilter;
	m_world->SetContactFilter(m_contactFilter);
}

IMPLEMENT_SINGLETON_DESTRUCTOR(Physics)
{
	SAFE_DELETE(m_contactListener);
	SAFE_DELETE(m_contactFilter);
	SAFE_DELETE(m_draw);
	SAFE_DELETE(m_world);
}

void Physics::Step(float dt)
{
	m_world->Step(dt, 10, 10);
}

void Physics::Draw()
{
#ifdef _DEBUG
	m_world->DebugDraw();
#endif
}

void Physics::SetGravity(Vec2 gravity)
{
	INST.m_world->SetGravity(VEC2_TO_B2VEC2(gravity));
}

Vec2 Physics::GetGravity()
{
	return B2VEC2_TO_VEC2(INST.m_world->GetGravity());
}

size_t Physics::QueryAABB(const AABB& aabb, std::vector<Collider*>* pColliders)
{
	return QueryAABB(aabb, pColliders, 0xFF);
}

size_t Physics::QueryAABB(const AABB& aabb, std::vector<Collider*>* pColliders, layer_t layerMask)
{
	AABBQueryCallback callback(layerMask);
	b2AABB b2aabb;
	b2aabb.upperBound = VEC2_TO_B2VEC2(aabb.rightTop);
	b2aabb.lowerBound = VEC2_TO_B2VEC2(aabb.leftBottom);
	INST.m_world->QueryAABB(&callback, b2aabb);
	callback.MoveColliders(pColliders);
	return pColliders->size();
}

bool Physics::RaycastStraight(const Vec2& p1, const Vec2& p2, RaycastResult* pResult)
{
	return RaycastStraight(p1, p2, pResult, 0xFF);
}

bool Physics::RaycastStraight(const Vec2& p1, const Vec2& p2, RaycastResult* pResult, layer_t layerMask)
{
	RaycastCallback callback(layerMask);
	INST.m_world->RayCast(&callback, VEC2_TO_B2VEC2(p1), VEC2_TO_B2VEC2(p2));
	const RaycastResult& result = callback.GetResult();
	*pResult = result;
	return callback.HasCollision();
}

bool Physics::Raycast(const Vec2& p1, const Vec2& p2, RaycastResult* pResult)
{
	return Raycast(p1, p2, pResult, 0xFF);
}

bool Physics::Raycast(const Vec2& p1, const Vec2& p2, RaycastResult* pResult, layer_t layerMask)
{
	// box2d 레이캐스트는 시작점을 포함하는 도형에 대해서는 레이캐스트를 하지 않습니다.
	// 따라서 정방향과 역방향으로 레이캐스트를 하고 짧은 충돌점을 가진 결과를 반환합니다.
	// 쉽게 말해서 가장 가까운 충돌을 반환합니다.

	RaycastCallback callback1(layerMask);
	INST.m_world->RayCast(&callback1, VEC2_TO_B2VEC2(p1), VEC2_TO_B2VEC2(p2));
	const RaycastResult& result1 = callback1.GetResult();

	RaycastCallback callback2(layerMask);
	INST.m_world->RayCast(&callback2, VEC2_TO_B2VEC2(p2), VEC2_TO_B2VEC2(p1));
	const RaycastResult& result2 = callback2.GetResult();

	if (callback1.HasCollision() && callback2.HasCollision())
	{
		float fraction1 = callback1.GetFraction();
		float fraction2 = 1 - callback2.GetFraction();
		if (fraction1 < fraction2) *pResult = result1;
		else *pResult = result2;
		return true;
	}
	if (callback1.HasCollision())
	{
		*pResult = result1;
		return true;
	}
	if (callback2.HasCollision())
	{
		*pResult = result2;
		return true;
	}
	return false;
}

size_t Physics::RaycastAll(const Vec2& p1, const Vec2& p2, std::vector<RaycastResult>* pResults)
{
	return RaycastAll(p1, p2, pResults, 0xFF);
}

size_t Physics::RaycastAll(const Vec2& p1, const Vec2& p2, std::vector<RaycastResult>* pResults, layer_t layerMask)
{
	// 벡터에 담긴 결과들은 충돌 거리에 따라 정렬되지 않았습니다.
	// 필요하다면 직접 정렬해야 합니다.

	// 정방향으로 모든 콜라이더에 대해서 레이캐스트를 합니다.
	RaycastAllCallback callback1(layerMask);
	INST.m_world->RayCast(&callback1, VEC2_TO_B2VEC2(p1), VEC2_TO_B2VEC2(p2));
	callback1.MoveResults(pResults);

	// 역방향으로 모든 콜라이더에 대해서 레이캐스트를 합니다.
	std::vector<RaycastResult> result2(layerMask);
	RaycastAllCallback callback2;
	INST.m_world->RayCast(&callback2, VEC2_TO_B2VEC2(p2), VEC2_TO_B2VEC2(p1));
	callback2.MoveResults(&result2);

	// 역방향 레이캐스트 결과 중에서
	// 정방향 레이캐스트 결과에 포함되지 않은 결과들만
	// 벡터에 추가합니다.
	auto Contains = [&](const RaycastResult& rr)
	{
		auto it = std::find_if(
			pResults->begin(), pResults->end(),
			[&](const RaycastResult& current)
			{
				return rr.collider == current.collider;
			});
		return it != pResults->end();
	};
	for (auto& rr : result2)
	{
		if (!Contains(rr))
		{
			pResults->push_back(rr);
		}
	}

	return pResults->size();
}

Collider* Physics::OverlapPoint(const Vec2& p)
{
	return OverlapPoint(p, 0xFF);
}

Collider* Physics::OverlapPoint(const Vec2& p, layer_t layerMask)
{
	std::vector<Collider*> colliders;
	AABB aabb(p.y + 0.5f, p.x + 0.5f, p.y - 0.5f, p.x - 0.5f);
	size_t count = QueryAABB(aabb, &colliders, layerMask);
	if (count == 0) return nullptr;

	b2Vec2 b2p = VEC2_TO_B2VEC2(p);
	for (auto& collider : colliders)
	{
		b2Fixture* fixture = collider->m_fixture;
		if (fixture->TestPoint(b2p))
		{
			return collider;
		}
	}

	return nullptr;
}

size_t Physics::OverlapPointAll(const Vec2& p, std::vector<Collider*>* pColliders)
{
	return OverlapPointAll(p, pColliders, 0xFF);
}

size_t Physics::OverlapPointAll(const Vec2& p, std::vector<Collider*>* pColliders, layer_t layerMask)
{
	std::vector<Collider*> colliders;
	AABB aabb(p.y + 0.5f, p.x + 0.5f, p.y - 0.5f, p.x - 0.5f);
	size_t count = QueryAABB(aabb, &colliders, layerMask);
	if (count == 0) return 0;

	b2Vec2 b2p = VEC2_TO_B2VEC2(p);
	for (auto& collider : colliders)
	{
		b2Fixture* fixture = collider->m_fixture;
		if (fixture->TestPoint(b2p))
		{
			pColliders->push_back(collider);
		}
	}

	return pColliders->size();
}

b2World* Physics::GetWorld() const
{
	return m_world;
}

ContactListener* Physics::GetContactListener() const
{
	return m_contactListener;
}

ContactFilter* Physics::GetContactFilter() const
{
	return m_contactFilter;
}
