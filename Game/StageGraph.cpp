#include "stdafx.h"
#include "StageGraph.h"

#define DISTANCE_EPSILON (0.1f)

StageGraph::StageGraph()
{
	
}

StageGraph::~StageGraph()
{
	Clear();
}

StageGraph::StageGraph(const StageGraph& rhs)
{
	for (auto& node : rhs.m_nodes)
	{
		m_nodes.push_back(new StageGraphNode(*node));
	}
	for (auto& edge_vector : rhs.m_edges)
	{
		StageGraphNode* current = m_nodes[edge_vector.first->index];
		for (auto& edge : edge_vector.second)
		{
			StageGraphNode* next = m_nodes[edge->next->index];
			m_edges[current].push_back(new StageGraphEdge(edge->type, current, next, edge->weight));
		}
	}
}

void StageGraph::Clear()
{
	for (auto& edge_vector : m_edges)
	{
		for (auto& edge : edge_vector.second) SAFE_DELETE(edge);
		edge_vector.second.clear();
	}
	m_edges.clear();

	for (auto& node : m_nodes) SAFE_DELETE(node);
	m_nodes.clear();
}

void StageGraph::InsertGround(Vec2 positionA, Vec2 positionB)
{
	StageGraphNode* nodeA = new StageGraphNode(m_nodes.size(), StageGraphType::GROUND, positionA);
	StageGraphNode* nodeB = new StageGraphNode(m_nodes.size() + 1, StageGraphType::GROUND, positionB);
	m_nodes.push_back(nodeA);
	m_nodes.push_back(nodeB);

	float distance = Vec2::Distance(positionA, positionB);
	StageGraphEdge* edgeA2B = new StageGraphEdge(StageGraphType::GROUND, nodeA, nodeB, distance);
	StageGraphEdge* edgeB2A = new StageGraphEdge(StageGraphType::GROUND, nodeB, nodeA, distance);
	m_edges[nodeA].push_back(edgeA2B);
	m_edges[nodeB].push_back(edgeB2A);
}

void StageGraph::InsertStair(Vec2 positionA, Vec2 positionB)
{
	StageGraphNode* nodeA = new StageGraphNode(m_nodes.size(), StageGraphType::STAIR, positionA);
	StageGraphNode* nodeB = new StageGraphNode(m_nodes.size() + 1, StageGraphType::STAIR, positionB);
	m_nodes.push_back(nodeA);
	m_nodes.push_back(nodeB);

	float distance = Vec2::Distance(positionA, positionB);
	StageGraphEdge* edgeA2B = new StageGraphEdge(StageGraphType::STAIR, nodeA, nodeB, distance);
	StageGraphEdge* edgeB2A = new StageGraphEdge(StageGraphType::STAIR, nodeB, nodeA, distance);
	m_edges[nodeA].push_back(edgeA2B);
	m_edges[nodeB].push_back(edgeB2A);
}

void StageGraph::CreateDoor(Vec2 positionA, Vec2 positionB)
{
	StageGraphNode* doorNodeA = SplitEdge(positionA);
	if (!doorNodeA) return;
	StageGraphNode* doorNodeB = SplitEdge(positionB);
	if (!doorNodeB) return;

	doorNodeA->type = StageGraphType::DOOR;
	doorNodeB->type = StageGraphType::DOOR;

	StageGraphEdge* A2B = new StageGraphEdge(StageGraphType::DOOR, doorNodeA, doorNodeB, Vec2::Distance(doorNodeA->position, doorNodeB->position));
	StageGraphEdge* B2A = new StageGraphEdge(StageGraphType::DOOR, doorNodeB, doorNodeA, Vec2::Distance(doorNodeA->position, doorNodeB->position));
	m_edges[doorNodeA].push_back(A2B);
	m_edges[doorNodeB].push_back(B2A);
}

void StageGraph::BuildEdges()
{
	// @: ��忡 ���� ��ġ�Դϴ�.
	// +: ��带 �մ� �����Դϴ�.
	/*
		�׸� 1.
		Collider A
				  @
				+ *
			  +   *
			+     *
		  +       *
		@ * * * * *
	*/
	/*
		�׸� 2.
		Collider B
		@ + + + + @
		^         ^
		^         ^
		^ ^ ^ ^ ^ ^
	*/
	/*
		�׸� 3.
		#: ������ �κ��Դϴ�.
				  # + + + + +
				+ #         ^
			  +   #         ^
			+     # ^ ^ ^ ^ ^
		  +       *
		+ * * * * *
	*/
	/*
		�׸� 4.
		������ ������ ���� ª�� ������ �ս��ϴ�.
		�� �׸������� & ǥ�õ� ���� �̾��� ���Դϴ�.
				  & + + + + @
				+ #         ^
			  +   #         ^
			+     # ^ ^ ^ ^ ^
		  +       *
		@ * * * * *
	*/
	// �ٸ� �ݶ��̴��� �̾��� ������ �����Ϸ��� �ݵ�� �ش� ������ ���� �� ���� ��ġ���� ��ġ�ؾ� �մϴ�.

	ConnectNearVertices(int(StageGraphType::GROUND) | int(StageGraphType::STAIR));
}

bool StageGraph::PathFind(Vec2 from, Vec2 to, std::vector<StageGraphPathNode>* pPath)
{
	if (pPath) pPath->clear();

	StageGraph g = *this;

	StageGraphNode* f = g.SplitEdge(from);
	StageGraphNode* t = g.SplitEdge(to);

	// from �Ǵ� to�� ��ġ�� ���� ���� �����ϴ� ��ġ�� �ƴ� ��쿡
	// �ش� ��ġ�� ���� ����� ��带 ä���մϴ�.
	if (!f) f = g.FindNearNode(from);
	if (!t) t = g.FindNearNode(to);

	if (!f || !t) return false;

	return g.PathFind(f, t, pPath);
}

void StageGraph::ConnectNearVertices(int typeMask)
{
	size_t nodeCount = m_nodes.size();
	for (size_t i = 0; i < nodeCount; ++i)
	{
		StageGraphNode* nodeI = m_nodes[i];
		if ((int(nodeI->type) & typeMask) == 0) continue;

		for (size_t j = i + 1; j < nodeCount; ++j)
		{
			StageGraphNode* nodeJ = m_nodes[j];
			if ((int(nodeJ->type) & typeMask) == 0) continue;

			// �ſ� ����� �� ���� �ƴ� ��쿡�� ��ŵ�մϴ�.
			float distance = Vec2::Distance(nodeI->position, nodeJ->position);
			if (distance > DISTANCE_EPSILON) continue;

			// �� ��(���)�� �����մϴ�.
			StageGraphEdge* edgeI2J = new StageGraphEdge(nodeI->type, nodeI, nodeJ, distance);
			StageGraphEdge* edgeJ2I = new StageGraphEdge(nodeJ->type, nodeJ, nodeI, distance);
			m_edges[nodeI].push_back(edgeI2J);
			m_edges[nodeJ].push_back(edgeJ2I);
		}
	}
}

StageGraphEdge* StageGraph::FindNearEdge(int typeMask, Vec2 position, edge_vector_t::iterator* pIterator)
{
	for (auto& edge_vector : m_edges)
	{
		edge_vector_t& v = edge_vector.second;

		for (auto it = v.begin(); it != v.end(); ++it)
		{
			StageGraphEdge* edge = *it;

			if ((int(edge->type) & typeMask) == 0) continue;
			if (!edge->InFaceRegion(position)) continue;
			if (edge->DistanceToPoint(position) > DISTANCE_EPSILON) continue;
			*pIterator = it;
			return edge;
		}
	}
	return nullptr;
}

StageGraphEdge* StageGraph::FindEdge(StageGraphType type, StageGraphNode* current, StageGraphNode* next, edge_vector_t::iterator* pIterator)
{
	for (auto& edge_vector : m_edges)
	{
		edge_vector_t& v = edge_vector.second;

		for (auto it = v.begin(); it != v.end(); ++it)
		{
			StageGraphEdge* edge = *it;

			if (edge->type != type) continue;
			if (edge->current != current) continue;
			if (edge->next != next) continue;
			*pIterator = it;
			return edge;
		}
	}
	return nullptr;
}

StageGraphNode* StageGraph::SplitEdge(Vec2 position)
{
	edge_vector_t::iterator nearEdgeIt;
	StageGraphEdge* nearEdge = FindNearEdge((int(StageGraphType::GROUND) | int(StageGraphType::STAIR)), position, &nearEdgeIt);
	if (!nearEdge) return nullptr;
	m_edges[nearEdge->current].erase(nearEdgeIt);

	edge_vector_t::iterator revNearEdgeIt;
	StageGraphEdge* revNearEdge = FindEdge(nearEdge->type, nearEdge->next, nearEdge->current, &revNearEdgeIt);
	if (!revNearEdge) return nullptr;
	m_edges[revNearEdge->current].erase(revNearEdgeIt);

	StageGraphType type = nearEdge->type;

	StageGraphNode* A = nearEdge->current;
	StageGraphNode* B = nearEdge->next;
	StageGraphNode* middle = new StageGraphNode(m_nodes.size(), type, position);
	m_nodes.push_back(middle);

	StageGraphEdge* A2M = new StageGraphEdge(type, A, middle, Vec2::Distance(A->position, middle->position));
	StageGraphEdge* M2A = new StageGraphEdge(type, middle, A, Vec2::Distance(A->position, middle->position));
	StageGraphEdge* B2M = new StageGraphEdge(type, B, middle, Vec2::Distance(B->position, middle->position));
	StageGraphEdge* M2B = new StageGraphEdge(type, middle, B, Vec2::Distance(B->position, middle->position));
	m_edges[A].push_back(A2M);
	m_edges[middle].push_back(M2A);
	m_edges[middle].push_back(M2B);
	m_edges[B].push_back(B2M);

	SAFE_DELETE(nearEdge);
	SAFE_DELETE(revNearEdge);

	return middle;
}

StageGraphNode* StageGraph::FindNearNode(Vec2 position)
{
	float minSqrDistance = FLT_MAX;
	StageGraphNode* minSqrDistanceNode = nullptr;
	for (auto& node : m_nodes)
	{
		float sqrDistance = Vec2::SqrDistance(position, node->position);
		if (sqrDistance < minSqrDistance)
		{
			minSqrDistance = sqrDistance;
			minSqrDistanceNode = node;
		}
	}
	return minSqrDistanceNode;
}

bool StageGraph::ReversedPathFind(StageGraphNode* from, StageGraphNode* to, std::stack<StageGraphNode*>* pRevPath)
{
	struct Vertex
	{
		float distance;
		StageGraphNode* node;
		Vertex(float distance, StageGraphNode* node) :
			distance(distance),
			node(node)
		{ }
	};
	struct VertexCompare
	{
		bool operator() (const Vertex& lhs, const Vertex& rhs)
		{
			return lhs.distance > rhs.distance;
		}
	};

	float* distance = new float[m_nodes.size()];
	std::fill(distance, distance + m_nodes.size(), FLT_MAX);
	distance[from->index] = 0;

	std::map<StageGraphNode*, StageGraphNode*> parent;
	parent[from] = nullptr;

	bool success = false;

	PriorityQueue<Vertex, VertexCompare> q;
	// ���� ��带 ť�� �߰��մϴ�.
	q.push(Vertex(0, from));
	while (!q.empty() && !success)
	{
		// �켱���� ť���� ���� ���� �Ÿ��� ������ ��带 �����ϴ�.
		Vertex vertex = q.top();
		q.pop();
		// ��忡 ����� �ٸ� ������ ��ȸ�մϴ�.
		for (auto& edge : m_edges[vertex.node])
		{
			float new_distance = distance[vertex.node->index] + edge->weight;

			// �ٸ� �������� ��ΰ� �ִ� ����̸�
			if (new_distance < distance[edge->next->index])
			{
				// �� ������ �����ϰ� �Ǵ� �ִܰŸ��� �缳���մϴ�.
				distance[edge->next->index] = new_distance;
				// �� ����� �θ� ��带 ������Ʈ�մϴ�.
				parent[edge->next] = edge->current;
				// �� ��带 ť�� �����մϴ�.
				q.push(Vertex(distance[edge->next->index], edge->next));
			}

			// ��ǥ ��忡 ����������
			if (edge->next == to)
			{
				// success �÷��׸� ������� ��ǥ ��带 ã���� �˸��ϴ�.
				success = true;
				break;
			}
		}
	}

	StageGraphNode* node = nullptr;
	if (success) 
	{
		node = to;
	}
	else
	{
		float minDistance = FLT_MAX;;
		for (size_t i = 0; i < m_nodes.size(); ++i)
		{
			if (distance[i] == FLT_MAX) continue;
			float nodeToDistance = Vec2::Distance(m_nodes[i]->position, to->position);
			if (nodeToDistance < minDistance)
			{
				minDistance = nodeToDistance;
				node = m_nodes[i];
			}
		}
	}

	while (node)
	{
		pRevPath->push(node);
		node = parent[node];
	}

	SAFE_DELETE_ARRAY(distance);

	return success;
}

bool StageGraph::PathFind(StageGraphNode* from, StageGraphNode* to, std::vector<StageGraphPathNode>* pPath)
{
	std::stack<StageGraphNode*> revPath;
	bool result = ReversedPathFind(from, to, &revPath);

	if (!pPath)
		return result;

	while (!revPath.empty())
	{
		StageGraphNode* node = revPath.top();
		revPath.pop();

		if (!pPath->empty() &&
			Vec2::Distance(pPath->back().position, node->position) <= DISTANCE_EPSILON)
		{
			if (pPath->back().type == StageGraphType::GROUND)
			{
				(*pPath)[pPath->size() - 1] = StageGraphPathNode(node->type, node->position);
			}
			else
			{
				continue;
			}
		}
		else
		{
			pPath->push_back(StageGraphPathNode(node->type, node->position));
		}

		if (pPath->size() >= 3)
		{
			size_t i = pPath->size() - 1;
			Vec2 v3 = pPath->at(i).position;
			Vec2 v2 = pPath->at(i - 1).position;
			Vec2 v1 = pPath->at(i - 2).position;
			Vec2 cur_dir = v3 - v2;
			Vec2 prev_dir = v2 - v1;
			if (Vec2::Angle(cur_dir, prev_dir) == 0)
			{
				(*pPath)[i - 1] = (*pPath)[i];
				pPath->erase(pPath->end() - 2);
			}
		}
	}

	return result;
}

bool StageGraphEdge::InFaceRegion(Vec2 position)
{
	/*
		 v4 |   Face Region  | v5
		----|��������������������������������|----
		 v1 |       v2       | v3
	*/
	// Face ������ ������ �� �Ǵ� �Ʒ��� �����ϴ� ���Դϴ�.
	// v1, v3, v4, v5 ������ Face ���� �ۿ� �����մϴ�.
	// v2 ���� Face ���� �ȿ� �����մϴ�.

	auto FSign = [&](float value) { return value < 0 ? -1.0f : +1.0f; };

	Vec2 edge_dir = (next->position - current->position).normalized();
	Vec2 current_to_pos = position - current->position;
	Vec2 next_to_pos = position - next->position;
	float dot_with_c2p = Vec2::Dot(edge_dir, current_to_pos);
	float dot_with_p2c = Vec2::Dot(edge_dir, next_to_pos);

	// ��ȣ�� ���� ������ Face ���� �ȿ� �������� �ʽ��ϴ�.
	if (FSign(dot_with_c2p) == FSign(dot_with_p2c)) return false;

	return true;
}

Vec2 StageGraphEdge::ProjectOnEdge(Vec2 position)
{
	/*
		       v1
			   ��
			   ��
		��������������v2��������������
	*/
	// v1�� ���� ���п� ������ v2�� ��ġ��ŵ�ϴ�.

	position -= current->position;

	Vec2 dir_forward = (next->position - current->position).normalized();
	Vec2 projectedOnVec2 = Vec2::ProjectOnVec2(position, dir_forward);
	Vec2 projectedOnEdge = projectedOnVec2 + current->position;

	return projectedOnEdge;
}

float StageGraphEdge::DistanceToPoint(Vec2 position)
{
	Vec2 projectedOnEdge = ProjectOnEdge(position);
	return Vec2::Distance(position, projectedOnEdge);
}
