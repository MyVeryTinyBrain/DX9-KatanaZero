#pragma once

#include "StageGraphStruct.h"

class StageGraph
{
	using node_vector_t = std::vector<StageGraphNode*>;
	using edge_vector_t = std::vector<StageGraphEdge*>;
	using map_edge_t = std::map<StageGraphNode*, edge_vector_t>;

	PUBLIC StageGraph();
	PUBLIC ~StageGraph();
	PUBLIC StageGraph(const StageGraph& rhs);

	// �߰��Ϸ��� Ground, Stair�� ���� ��� �߰��մϴ�.
	// ���� BuildEdges ���� Ȥ�� ���Ŀ� Door�� ��� �߰��մϴ�.
	// ����: CreateDoor() �Լ��� �̹� ������ Ground, Stiar ���� ������ �̿��Ͽ� ������ �����մϴ�.

	PUBLIC void Clear();
	PUBLIC void InsertGround(Vec2 positionA, Vec2 positionB);
	PUBLIC void InsertStair(Vec2 positionA, Vec2 positionB);
	PUBLIC void CreateDoor(Vec2 positionA, Vec2 positionB);
	PUBLIC void BuildEdges();
	PUBLIC bool PathFind(Vec2 from, Vec2 to, std::vector<StageGraphPathNode>* pPath);
	
	PRIVATE void ConnectNearVertices(int typeMask);
	PRIVATE StageGraphEdge* FindNearEdge(int typeMask, Vec2 position, edge_vector_t::iterator* pIterator);
	PRIVATE StageGraphEdge* FindEdge(StageGraphType type, StageGraphNode* current, StageGraphNode* next, edge_vector_t::iterator* pIterator);
	PRIVATE StageGraphNode* SplitEdge(Vec2 position);
	PRIVATE StageGraphNode* FindNearNode(Vec2 position);
	PRIVATE bool ReversedPathFind(StageGraphNode* from, StageGraphNode* to, std::stack<StageGraphNode*>* pRevPath);
	PRIVATE bool PathFind(StageGraphNode* from, StageGraphNode* to, std::vector<StageGraphPathNode>* pPath);

	PRIVATE node_vector_t m_nodes;
	PUBLIC map_edge_t m_edges;
};

