#pragma once

#include "StageGraphStruct.h"

class StageGraph;

class StageMap
{
	DECLARE_SINGLETON(StageMap)

	PUBLIC void Clear();
	PUBLIC void InsertGroundCollider(BoxCollider* collider);
	PUBLIC void InsertPlatformCollider(LineCollider* collider);
	PUBLIC void InsertPlatformCollider(BoxCollider* collider);
	PUBLIC void InsertStairCollider(RightTriangleCollider* collider);
	PUBLIC void InsertStairCollider(TriangleCollider* collider);
	PUBLIC void InsertDoorPair(Vec2 positionA, Vec2 positionB);
	PUBLIC void BuildEdges();
	PUBLIC StageGraph* GetGraph() const;

	PUBLIC static bool PathFind(Vec2 from, Vec2 to, std::vector<StageGraphPathNode>* pPath);

	PRIVATE StageGraph* m_graph;
};

