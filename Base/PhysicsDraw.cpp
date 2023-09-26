#include "base_stdafx.h"
#include "PhysicsDraw.h"
#include "GraphicDevice.h"
#include "LineDevice.h"
#include "Box2dHeader.h"
#include "Collider.h"
#include "Body.h"

#define GD GraphicDevice
#define LINE GD::GetLineDevice()

void PhysicsDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	auto Vertex = [&](int32 index) -> Vec2
	{
		if (index >= vertexCount) index = 0;
		return B2VEC2_TO_VEC2(vertices[index]);
	};
	Draw2DLineParameters p;
	p.color = Color(0, 1, 0, 1);
	Vec2 prev = Vertex(0);
	for (int32 i = 1; i <= vertexCount; ++i)
	{
		Vec2 current = Vertex(i);
		LINE->Draw2DLine(prev, current, &p);
		prev = current;
	}
}

void PhysicsDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color, b2Fixture* fixture)
{
	auto Vertex = [&](int32 index) -> Vec2
	{
		if (index >= vertexCount) index = 0;
		return B2VEC2_TO_VEC2(vertices[index]);
	};

	Collider* collider = (Collider*)fixture->GetUserData().pointer;
	if (!collider->GetActive()) return;
	if (!collider->DrawDebug) return;

	Draw2DLineParameters p;
	if (collider->IsTriggerMode() == true) p.color = Color(0, 1, 1, 1);
	else p.color = Color(0, 1, 0, 1);

	Vec2 prev = Vertex(0);
	for (int32 i = 1; i <= vertexCount; ++i)
	{
		Vec2 current = Vertex(i);
		LINE->Draw2DLine(prev, current, &p);
		prev = current;
	}
}

void PhysicsDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	Draw2DLineParameters p;
	p.color = Color(0, 1, 0, 1);
	LINE->Draw2DCircle(B2VEC2_TO_VEC2(center), radius, &p);
}

void PhysicsDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color, b2Fixture* fixture)
{
	Collider* collider = (Collider*)fixture->GetUserData().pointer;
	if (!collider->GetActive()) return;
	if (!collider->DrawDebug) return;

	Draw2DLineParameters p;
	if (collider->IsTriggerMode() == true) p.color = Color(0, 1, 1, 1);
	else p.color = Color(0, 1, 0, 1);
	LINE->Draw2DCircle(B2VEC2_TO_VEC2(center), radius, &p);
}

void PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color, b2Fixture* fixture)
{
	if (!fixture) return;

	Collider* collider = (Collider*)fixture->GetUserData().pointer;
	if (!collider->GetActive()) return;
	if (!collider->DrawDebug) return;

	Draw2DLineParameters p;
	p.color = Color(1, 0, 0, 1);
	LINE->Draw2DLine(B2VEC2_TO_VEC2(p1), B2VEC2_TO_VEC2(p2), &p);
}

void PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	Draw2DLineParameters p;
	p.color = Color(1, 0, 0, 1);
	LINE->Draw2DLine(B2VEC2_TO_VEC2(p1), B2VEC2_TO_VEC2(p2), &p);
}

void PhysicsDraw::DrawTransform(const b2Transform& xf)
{
	Draw2DLineParameters p;
	p.color = Color(1, 0, 0, 1);
}

void PhysicsDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	Draw2DLineParameters parameters;
	parameters.color = Color(1, 0, 0, 1);
	LINE->Draw2DCircle(B2VEC2_TO_VEC2(p), 10, &parameters);
}
