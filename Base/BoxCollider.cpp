#include "base_stdafx.h"
#include "BoxCollider.h"
#include "Box2dHeader.h"
#include "Transform.h"

void BoxCollider::Awake()
{
	m_width = 1.0f;
	m_height = 1.0f;

	Collider::Awake();
}

void BoxCollider::UpdateShape(
	b2Shape** ppShape,
	const Vec2& localPositionFromBody,
	const float& localAngleFromBody,
	const Vec2& localScaleFromBody)
{
	SAFE_DELETE(*ppShape);

	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	const Mat4& localToBodyLocalMatrix = Mat4::TRS(localPositionFromBody, Vec3(0, 0, localAngleFromBody * DEG2RAD), localScaleFromBody);

	// CCW 순서로 정점을 정의해야 합니다.
	Vec2 vertices[4];
	vertices[0] = Vec4(+hw, +hh, 0, 1) * localToBodyLocalMatrix;
	vertices[1] = Vec4(-hw, +hh, 0, 1) * localToBodyLocalMatrix;
	vertices[2] = Vec4(-hw, -hh, 0, 1) * localToBodyLocalMatrix;
	vertices[3] = Vec4(+hw, -hh, 0, 1) * localToBodyLocalMatrix;
	
	b2Vec2 v[4] = { VEC2_TO_B2VEC2(vertices[0]), VEC2_TO_B2VEC2(vertices[1]), VEC2_TO_B2VEC2(vertices[2]), VEC2_TO_B2VEC2(vertices[3]) };
	b2PolygonShape* polygonShape = new b2PolygonShape();
	polygonShape->Set(v, 4);

	*ppShape = polygonShape;
}

void BoxCollider::SetWidth(float width)
{
	m_width = width;
	UpdateCollider();
}

float BoxCollider::GetWidth() const
{
	return m_width;
}

void BoxCollider::SetHeight(float height)
{
	m_height = height;
	UpdateCollider();
}

float BoxCollider::GetHeight() const
{
	return m_height;
}

Vec2 BoxCollider::GetTopLeft() const
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return Vec4(-hw, +hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 BoxCollider::GetTopRight() const
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return Vec4(+hw, +hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 BoxCollider::GetBottomLeft() const
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return Vec4(-hw, -hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 BoxCollider::GetBottomRight() const
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return Vec4(+hw, -hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}
