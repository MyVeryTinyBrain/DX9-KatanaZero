#include "base_stdafx.h"
#include "TriangleCollider.h"
#include "Box2dHeader.h"

void TriangleCollider::Awake()
{
	m_width = 1;
	m_height = 1;

	Collider::Awake();
}

void TriangleCollider::UpdateShape(b2Shape** ppShape, const Vec2& localPositionFromBody, const float& localAngleFromBody, const Vec2& localScaleFromBody)
{
    SAFE_DELETE(*ppShape);

    float hw = m_width * 0.5f;
    float hh = m_height * 0.5f;
    const Mat4& localToBodyLocalMatrix = Mat4::SRT(localScaleFromBody, { 0, 0, localAngleFromBody * DEG2RAD }, localPositionFromBody);

    // Default Shape
    // *
    // * *
    // *   *
    // *     *
    // * * * * *
    // CCW orientation
    Vec2 vertices[3];
    vertices[0] = Vec2(-hw, +hh) * localToBodyLocalMatrix;
    vertices[1] = Vec2(-hw, -hh) * localToBodyLocalMatrix;
    vertices[2] = Vec2(+hw, -hh) * localToBodyLocalMatrix;

    b2Vec2 v[3] = { VEC2_TO_B2VEC2(vertices[0]), VEC2_TO_B2VEC2(vertices[1]), VEC2_TO_B2VEC2(vertices[2]) };
    b2PolygonShape* polygon = new b2PolygonShape;
    polygon->Set(v, 3);

    *ppShape = polygon;
}

void TriangleCollider::SetWidth(float width)
{
    m_width = width;
    UpdateCollider();
}

float TriangleCollider::GetWidth() const
{
	return m_width;
}

void TriangleCollider::SetHeight(float height)
{
    m_height = height;
    UpdateCollider();
}

float TriangleCollider::GetHeight() const
{
	return m_height;
}

Vec2 TriangleCollider::GetTopLeft() const
{
    float hw = m_width * 0.5f;
    float hh = m_height * 0.5f;
    return Vec4(-hw, +hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 TriangleCollider::GetBottomLeft() const
{
    float hw = m_width * 0.5f;
    float hh = m_height * 0.5f;
    return Vec4(-hw, -hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 TriangleCollider::GetBottomRight() const
{
    float hw = m_width * 0.5f;
    float hh = m_height * 0.5f;
    return Vec4(+hw, -hh, 0, 1) * GetTransform()->LocalToWorldMatrix();
}
