#pragma once
enum class StageGraphType : int
{
	GROUND = 1 << 0,
	STAIR = 1 << 1,
	DOOR = 1 << 2,
};

struct StageGraphNode
{
	size_t			index;
	StageGraphType	type;
	Vec2			position;
	StageGraphNode(size_t index, StageGraphType type, Vec2 position) :
		index(index),
		type(type),
		position(position)
	{
	}
	StageGraphNode(const StageGraphNode& rhs) :
		index(rhs.index),
		type(rhs.type),
		position(rhs.position)
	{
	}
};

struct StageGraphEdge
{
	StageGraphType	type;
	StageGraphNode* current;
	StageGraphNode* next;
	float			weight;
	StageGraphEdge(StageGraphType type, StageGraphNode* current, StageGraphNode* next, float weight) :
		type(type),
		current(current),
		next(next),
		weight(weight)
	{
	}
	bool InFaceRegion(Vec2 position);
	Vec2 ProjectOnEdge(Vec2 position);
	float DistanceToPoint(Vec2 position);
};

struct StageGraphPathNode
{
	StageGraphType	type;
	Vec2			position;
	StageGraphPathNode() :
		type(StageGraphType::GROUND),
		position(Vec2::zero())
	{}
	StageGraphPathNode(StageGraphType type, Vec2 position) :
		type(type),
		position(position)
	{ }
};
