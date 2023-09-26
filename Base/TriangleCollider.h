#pragma once

#include "Collider.h"

class TriangleCollider : public Collider
{
	COMPONENT_OVERRIDE(Awake);
	virtual void UpdateShape(
		b2Shape** ppShape,
		const Vec2& localPositionFromBody,
		const float& localAngleFromBody,
		const Vec2& localScaleFromBody)
		override;

	PUBLIC void SetWidth(float width);
	PUBLIC float GetWidth() const;
	PUBLIC void SetHeight(float height);
	PUBLIC float GetHeight() const;
	PUBLIC Vec2 GetTopLeft() const;
	PUBLIC Vec2 GetBottomLeft() const;
	PUBLIC Vec2 GetBottomRight() const;

	PRIVATE float m_width;
	PRIVATE float m_height;
};

