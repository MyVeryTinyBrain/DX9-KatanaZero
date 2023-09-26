#include "base_stdafx.h"
#include "RightTriangleCollider.h"
#include "Box2dHeader.h"

void RightTriangleCollider::Awake()
{
    m_width = 1;

    Collider::Awake();
}

void RightTriangleCollider::UpdateShape(b2Shape** ppShape, const Vec2& localPositionFromBody, const float& localAngleFromBody, const Vec2& localScaleFromBody)
{
    SAFE_DELETE(*ppShape);

    float hw = m_width * 0.5f;
    const Mat4& localToBodyLocalMatrix = Mat4::SRT(localScaleFromBody, { 0, 0, localAngleFromBody * DEG2RAD }, localPositionFromBody);

    // Default Shape
    // *
    // * *
    // *   *
    // *     *
    // * * * * *
    // CCW orientation
    Vec2 vertices[3];
    vertices[0] = Vec2(-hw, +hw) * localToBodyLocalMatrix;
    vertices[1] = Vec2(-hw, -hw) * localToBodyLocalMatrix;
    vertices[2] = Vec2(+hw, -hw) * localToBodyLocalMatrix;

    b2Vec2 v[3] = { VEC2_TO_B2VEC2(vertices[0]), VEC2_TO_B2VEC2(vertices[1]), VEC2_TO_B2VEC2(vertices[2]) };
    b2PolygonShape* polygon = new b2PolygonShape;
    polygon->Set(v, 3);

    *ppShape = polygon;
}

void RightTriangleCollider::SetWidth(float width)
{
    m_width = width;
    UpdateCollider();
}

float RightTriangleCollider::GetWidth() const
{
    return m_width;
}

Vec2 RightTriangleCollider::GetTopLeft() const
{
    float hw = m_width * 0.5f;
    return Vec4(-hw, +hw, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 RightTriangleCollider::GetBottomLeft() const
{
    float hw = m_width * 0.5f;
    return Vec4(-hw, -hw, 0, 1) * GetTransform()->LocalToWorldMatrix();
}

Vec2 RightTriangleCollider::GetBottomRight() const
{
    float hw = m_width * 0.5f;
    return Vec4(+hw, -hw, 0, 1) * GetTransform()->LocalToWorldMatrix();
}
