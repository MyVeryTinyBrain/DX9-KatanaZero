#pragma once

class Collider;

struct RaycastResult
{
	Collider* collider;
	Vec2 point;
	Vec2 normal;
};

struct AABB
{
	Vec2 rightTop;
	Vec2 leftBottom;
	AABB() {}
	AABB(const Vec2& RT, const Vec2& LB) { rightTop = RT; leftBottom = LB; }
	AABB(float top, float right, float bottom, float left) { rightTop = { right, top }; leftBottom = { left, bottom }; }
};

struct CollisionResult
{
	Collider* A;
	Collider* B;
	Vec2 localNormal;
	Vec2 worldNormal;
	Vec2 localPoints[2];
	Vec2 worldPoints[2];
	float worldSeparations[2];
	size_t pointCount;
};

struct CollisionExitResult
{
	Collider* A;
	Collider* B;
};

struct TriggerResult
{
	Collider* A;
	Collider* B;
};