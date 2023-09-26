#include "base_stdafx.h"
#include "ContactListener.h"
#include "Box2dHeader.h"
#include "Collider.h"
#include "Transform.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();

	Collider* CA = (Collider*)A->GetUserData().pointer;
	Collider* CB = (Collider*)B->GetUserData().pointer;

	if (A->IsSensor() || B->IsSensor())
	{
		TriggerResult tr;
		tr.A = CA;
		tr.B = CB;
		m_beginTriggers.push_back(std::move(tr));
	}
	else
	{
		CollisionResult cr;
		cr.A = CA;
		cr.B = CB;

		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);
		b2Manifold* localManifold = contact->GetManifold();

		// 노멀 벡터의 방향은 A --> B 입니다.

		cr.localNormal = B2VEC2_TO_VEC2(localManifold->localNormal);
		cr.worldNormal = B2VEC2_TO_VEC2(worldManifold.normal);

		cr.localPoints[0] = B2VEC2_TO_VEC2(localManifold->points[0].localPoint);
		cr.worldPoints[0] = B2VEC2_TO_VEC2(worldManifold.points[0]);
		cr.worldSeparations[0] = worldManifold.separations[0];

		if (localManifold->pointCount == 2)
		{
			cr.localPoints[1] = B2VEC2_TO_VEC2(localManifold->points[1].localPoint);
			cr.worldPoints[1] = B2VEC2_TO_VEC2(worldManifold.points[1]);
			cr.worldSeparations[1] = worldManifold.separations[1];
		}

		cr.pointCount = localManifold->pointCount;
		
		m_beginCollisions.push_back(std::move(cr));
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();

	Collider* CA = (Collider*)A->GetUserData().pointer;
	Collider* CB = (Collider*)B->GetUserData().pointer;

	if (A->IsSensor() || B->IsSensor())
	{
		TriggerResult tr;
		tr.A = CA;
		tr.B = CB;
		m_endTriggers.push_back(std::move(tr));
	}
	else
	{
		CollisionExitResult cr;
		cr.A = CA;
		cr.B = CB;

		m_endCollisions.push_back(std::move(cr));
	}
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();

	Collider* CA = (Collider*)A->GetUserData().pointer;
	Collider* CB = (Collider*)B->GetUserData().pointer;

	if (A->IsSensor() || B->IsSensor())
	{
		TriggerResult tr;
		tr.A = CA;
		tr.B = CB;
		m_stayTriggers.push_back(std::move(tr));
	}
	else
	{
		CollisionResult cr;
		cr.A = CA;
		cr.B = CB;

		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);
		b2Manifold* localManifold = contact->GetManifold();

		// 노멀 벡터의 방향은 A --> B 입니다.

		cr.localNormal = B2VEC2_TO_VEC2(localManifold->localNormal);
		cr.worldNormal = B2VEC2_TO_VEC2(worldManifold.normal);

		cr.localPoints[0] = B2VEC2_TO_VEC2(localManifold->points[0].localPoint);
		cr.worldPoints[0] = B2VEC2_TO_VEC2(worldManifold.points[0]);
		cr.worldSeparations[0] = worldManifold.separations[0];

		if (localManifold->pointCount == 2)
		{
			cr.localPoints[1] = B2VEC2_TO_VEC2(localManifold->points[1].localPoint);
			cr.worldPoints[1] = B2VEC2_TO_VEC2(worldManifold.points[1]);
			cr.worldSeparations[1] = worldManifold.separations[1];
		}

		cr.pointCount = localManifold->pointCount;

		m_stayCollisions.push_back(std::move(cr));
	}
}

void ContactListener::MoveBeginCollisions(std::vector<CollisionResult>* pCollisionResults)
{
	*pCollisionResults = std::move(m_beginCollisions);
}

void ContactListener::MoveBeginTriggers(std::vector<TriggerResult>* pTriggerResults)
{
	*pTriggerResults = std::move(m_beginTriggers);
}

void ContactListener::MoveEndCollisions(std::vector<CollisionExitResult>* pCollisionResults)
{
	*pCollisionResults = std::move(m_endCollisions);
}

void ContactListener::MoveEndTriggers(std::vector<TriggerResult>* pTriggerResults)
{
	*pTriggerResults = std::move(m_endTriggers);
}

void ContactListener::MoveStayCollisions(std::vector<CollisionResult>* pCollisionResults)
{
	*pCollisionResults = std::move(m_stayCollisions);
}

void ContactListener::MoveStayTriggers(std::vector<TriggerResult>* pTriggerResults)
{
	*pTriggerResults = std::move(m_stayTriggers);
}

void ContactListener::Clear()
{
	m_beginCollisions.clear();
	m_beginTriggers.clear();
	m_endCollisions.clear();
	m_endTriggers.clear();
	m_stayCollisions.clear();
	m_stayTriggers.clear();
}
