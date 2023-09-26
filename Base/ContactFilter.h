#pragma once

#include "../box2d/box2d/b2_world_callbacks.h"

class ContactFilter : public b2ContactFilter
{
	PRIVATE virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
};

