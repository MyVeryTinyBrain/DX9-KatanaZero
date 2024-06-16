#include "stdafx.h"
#include "StageMap.h"
#include "StageGraph.h"
#include "GameLayer.h"

IMPLEMENT_SINGLETON(StageMap)

IMPLEMENT_SINGLETON_CONSTRUCTOR(StageMap)
{
	m_graph = new StageGraph();
}

IMPLEMENT_SINGLETON_DESTRUCTOR(StageMap)
{
	SAFE_DELETE(m_graph);
}

void StageMap::Clear()
{
	m_graph->Clear();
}

void StageMap::InsertGroundCollider(BoxCollider* collider)
{
	m_graph->InsertGround(collider->GetTopLeft(), collider->GetTopRight());
}

void StageMap::InsertPlatformCollider(LineCollider* collider)
{
	m_graph->InsertGround(collider->GetLeft(), collider->GetRight());
}

void StageMap::InsertPlatformCollider(BoxCollider* collider)
{
	m_graph->InsertGround(collider->GetTopLeft(), collider->GetTopRight());
}

void StageMap::InsertStairCollider(RightTriangleCollider* collider)
{
	if (collider->GetTransform()->GetScale().x >= 0)
	{
		m_graph->InsertStair(collider->GetTopLeft(), collider->GetBottomRight());
	}
	else
	{
		m_graph->InsertStair(collider->GetBottomRight(), collider->GetTopLeft());
	}
}

void StageMap::InsertStairCollider(TriangleCollider* collider)
{
	if (collider->GetTransform()->GetScale().x >= 0)
	{
		m_graph->InsertStair(collider->GetTopLeft(), collider->GetBottomRight());
	}
	else
	{
		m_graph->InsertStair(collider->GetBottomRight(), collider->GetTopLeft());
	}
}

void StageMap::InsertDoorPair(Vec2 positionA, Vec2 positionB)
{
	// 문의 아래방향으로 레이캐스트해 문의 바닥 위치를 찾습니다.
	RaycastResult res1, res2;
	if (!Physics::Raycast(positionA + Vec2::up(), positionA + Vec2::down() * 1000, &res1, LAYER_GROUND | LAYER_PLATFORM)) return;
	if (!Physics::Raycast(positionB + Vec2::up(), positionB + Vec2::down() * 1000, &res2, LAYER_GROUND | LAYER_PLATFORM)) return;
	m_graph->CreateDoor(res1.point, res2.point);
}

void StageMap::BuildEdges()
{
	m_graph->BuildEdges();
}

StageGraph* StageMap::GetGraph() const
{
	return m_graph;
}

bool StageMap::PathFind(Vec2 from, Vec2 to, std::vector<StageGraphPathNode>* pPath)
{
	if (pPath) pPath->clear();
	// 시작, 종료 위치에서 아래 방향으로 레이캐스트해 바닥 위치를 찾습니다.
	RaycastResult res1, res2;
	if (!Physics::Raycast(from + Vec2::up(), from + Vec2::down() * 1000, &res1, LAYER_GROUND | LAYER_PLATFORM)) return false;
	if (!Physics::Raycast(to + Vec2::up(), to + Vec2::down() * 1000, &res2, LAYER_GROUND | LAYER_PLATFORM)) return false;
	return g_instance.m_graph->PathFind(res1.point, res2.point, pPath);
}
