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

	// 추가하려는 Ground, Stair을 먼저 모두 추가합니다.
	// 이후 BuildEdges 이전 혹은 이후에 Door를 모두 추가합니다.
	// 이유: CreateDoor() 함수는 이미 생성된 Ground, Stiar 들의 엣지를 이용하여 엣지를 생성합니다.

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

