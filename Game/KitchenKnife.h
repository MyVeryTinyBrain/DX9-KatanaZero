#pragma once

#include "ThrowObject.h"

class Door;
class BreakablePlatform;

class KitchenKnife : public ThrowObject
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(AfterRender);

	virtual void OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType) override;
	virtual void OnPutIn() override;
	virtual wstring SpriteKey() override;

	void Check();

	Door* m_door;
	BreakablePlatform* m_platform;
};

