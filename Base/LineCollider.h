#pragma once

#include "Collider.h"

class LineCollider : public Collider
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
	PUBLIC Vec2 GetLeft() const;
	PUBLIC Vec2 GetRight() const;

	PRIVATE float m_width;
};

