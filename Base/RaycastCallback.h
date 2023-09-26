#pragma once

#include "../box2d/box2d/b2_world_callbacks.h"
#include "Box2dStruct.h"

class RaycastCallback : public b2RayCastCallback
{
	PUBLIC RaycastCallback();
	PUBLIC RaycastCallback(layer_t layerMask);
	PRIVATE virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	PUBLIC const bool& HasCollision() const;
	PUBLIC const RaycastResult& GetResult() const;
	PUBLIC const float& GetFraction() const;

	PRIVATE bool m_collision;
	PRIVATE RaycastResult m_result;
	PRIVATE float m_fraction;
	PRIVATE bool m_useLayerMask;
	PRIVATE layer_t m_layerMask;
};

