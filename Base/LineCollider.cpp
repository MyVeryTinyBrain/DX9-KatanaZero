#include "base_stdafx.h"
#include "LineCollider.h"
#include "Box2dHeader.h"
#include "Transform.h"

void LineCollider::Awake()
{
	m_width = 1;

	Collider::Awake();
}

void LineCollider::UpdateShape(b2Shape** ppShape, const Vec2& localPositionFromBody, const float& localAngleFromBody, const Vec2& localScaleFromBody)
{
	SAFE_DELETE(*ppShape);

	float w = m_width;
	float hw = m_width * 0.5f;
	const Mat4& localToBodyLocalMatrix = Mat4::SRT(localScaleFromBody, { 0, 0, localAngleFromBody * DEG2RAD }, localPositionFromBody);

	// CCW orientation
	Vec2 vertices[4];
	vertices[0] = Vec4(+w, 0, 0, 1) * localToBodyLocalMatrix;
	vertices[1] = Vec4(+hw, 0, 0, 1) * localToBodyLocalMatrix;
	vertices[2] = Vec4(-hw, 0, 0, 1) * localToBodyLocalMatrix;
	vertices[3] = Vec4(-w, 0, 0, 1) * localToBodyLocalMatrix;

	b2Vec2 prevGhost = VEC2_TO_B2VEC2(vertices[0]);
	b2Vec2 v[2] = { VEC2_TO_B2VEC2(vertices[1]), VEC2_TO_B2VEC2(vertices[2]) };
	b2Vec2 nextGhost = VEC2_TO_B2VEC2(vertices[0]);
	b2ChainShape* chainShape = new b2ChainShape;
	chainShape->CreateChain(v, 2, prevGhost, nextGhost);

	*ppShape = chainShape;
}

void LineCollider::SetWidth(float width)
{
	m_width = width;
	UpdateCollider();
}

float LineCollider::GetWidth() const
{
	return m_width;
}

Vec2 LineCollider::GetLeft() const
{
	return Vec4(-m_width * 0.5f, 0, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 LineCollider::GetRight() const
{
	return Vec4(+m_width * 0.5f, 0, 0, 1) * GetTransform()->LocalToWorldMatrix();
}
