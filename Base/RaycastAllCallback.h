#pragma once

#include "../box2d/box2d/b2_world_callbacks.h"
#include "Box2dStruct.h"

class RaycastAllCallback : public b2RayCastCallback
{
	PUBLIC RaycastAllCallback();
	PUBLIC RaycastAllCallback(layer_t layerMask);
	PRIVATE virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	PUBLIC const std::vector<RaycastResult>& GetResults() const;
	PUBLIC void MoveResults(std::vector<RaycastResult>* pResults);

	PRIVATE std::vector<RaycastResult> m_results;
	PRIVATE bool m_useLayerMask;
	PRIVATE layer_t m_layerMask;
};

