#pragma once

#include "ThrowObject.h"

class BearBottle : public ThrowObject
{
	virtual void OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType) override;
	virtual wstring SpriteKey() override;
};

