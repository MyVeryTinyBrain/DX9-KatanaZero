#pragma once

#include "../box2d/box2d/b2_world_callbacks.h"

class Body;

class MemoryListener : public b2MemoryListener
{
	virtual void OnCreateBody(b2Body* body) override;
	virtual void OnDestroyBody(b2Body* body) override;
	virtual void OnDestroyJoint(b2Joint* joint) override;
	virtual void OnDestroyFixture(b2Fixture* fixture) override;
};

