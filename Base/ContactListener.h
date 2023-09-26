#pragma once

#include "../box2d/box2d/b2_world_callbacks.h"
#include "Box2dStruct.h"

class ContactListener : public b2ContactListener
{
	PRIVATE virtual void BeginContact(b2Contact* contact) override;
	PRIVATE virtual void EndContact(b2Contact* contact) override;
	PRIVATE virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	PUBLIC void MoveBeginCollisions(std::vector<CollisionResult>* pCollisionResults);
	PUBLIC void MoveBeginTriggers(std::vector<TriggerResult>* pTriggerResults);
	PUBLIC void MoveEndCollisions(std::vector<CollisionExitResult>* pCollisionResults);
	PUBLIC void MoveEndTriggers(std::vector<TriggerResult>* pTriggerResults);
	PUBLIC void MoveStayCollisions(std::vector<CollisionResult>* pCollisionResults);
	PUBLIC void MoveStayTriggers(std::vector<TriggerResult>* pTriggerResults);
	PUBLIC void Clear();

	PRIVATE std::vector<CollisionResult> m_beginCollisions;
	PRIVATE std::vector<TriggerResult> m_beginTriggers;
	PRIVATE std::vector<CollisionExitResult> m_endCollisions;
	PRIVATE std::vector<TriggerResult> m_endTriggers;
	PRIVATE std::vector<CollisionResult> m_stayCollisions;
	PRIVATE std::vector<TriggerResult> m_stayTriggers;
};

