#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
	COMPONENT_OVERRIDE(Awake);
	virtual void UpdateShape(
		b2Shape** ppShape,
		const Vec2& localPositionFromBody,
		const float& localAngleFromBody,
		const Vec2& localScaleFromBody)
		override;

	PUBLIC void SetRadius(float radius);
	PUBLIC float GetRadius() const;

	PRIVATE float m_radius;
};

