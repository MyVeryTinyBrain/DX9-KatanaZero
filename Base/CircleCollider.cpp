#include "base_stdafx.h"
#include "CircleCollider.h"
#include "Box2dHeader.h"

void CircleCollider::Awake()
{
	m_radius = 0.5f;

	Collider::Awake();
}

void CircleCollider::UpdateShape(
	b2Shape** ppShape,
	const Vec2& localPositionFromBody,
	const float& localAngleFromBody,
	const Vec2& localScaleFromBody)
{
	SAFE_DELETE(*ppShape);

	b2CircleShape* circle = new b2CircleShape();
	circle->m_p.Set(localPositionFromBody.x, localPositionFromBody.y);
	circle->m_radius = m_radius * max(localScaleFromBody.x, localScaleFromBody.y);

	*ppShape = circle;
}

void CircleCollider::SetRadius(float radius)
{
	m_radius = radius;
	UpdateCollider();
}

float CircleCollider::GetRadius() const
{
	return m_radius;
}
