#pragma once

#include "../box2d/box2d/b2_world_callbacks.h"

class Collider;

class AABBQueryCallback : public b2QueryCallback
{
	PUBLIC AABBQueryCallback();
	PUBLIC AABBQueryCallback(layer_t layerMask);
	PRIVATE virtual bool ReportFixture(b2Fixture* fixture) override;

	PUBLIC void MoveColliders(std::vector<Collider*>* pColliders);
	PRIVATE std::vector<Collider*> m_colliders;
	PRIVATE bool m_useLayerMask;
	PRIVATE layer_t m_layerMask;
};

