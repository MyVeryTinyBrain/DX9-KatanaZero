#pragma once

#include "ThrowObject.h"

class ExplosiveBottle : public ThrowObject
{
	COMPONENT_OVERRIDE(Awake);

	virtual void OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType) override;
	virtual wstring SpriteKey() override;

	PRIVATE float m_rangeRadius;
};

